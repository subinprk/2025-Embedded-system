#!/usr/bin/env python3
"""
MLX90640 Thermal Image Viewer
Receives raw pixel data from ATmega4809 via serial and displays as heatmap.

Usage:
    python mlx_viewer.py COM9 115200

Requirements:
    pip install pyserial numpy matplotlib

Performance tips:
    - For even faster display, consider: pip install opencv-python
    - Run with: python mlx_viewer.py COM9 115200 --opencv
"""

import serial
import numpy as np
import sys
import time
import re
import threading
from queue import Queue, Empty

# Pre-compile regex for hex parsing (faster than split + validate)
HEX_PATTERN = re.compile(r'[0-9A-Fa-f]{4}')

class MLX90640Viewer:
    def __init__(self, port, baudrate=115200, use_opencv=False):
        self.port = port
        self.baudrate = baudrate
        self.ser = None
        self.frame_data = np.zeros((24, 32), dtype=np.uint16)
        self.use_opencv = use_opencv
        self.frame_count = 0
        self.last_fps_time = time.time()
        self.fps = 0
        self.running = True
        
        # Thread-safe queue for frames
        self.frame_queue = Queue(maxsize=2)
        
        if use_opencv:
            self._setup_opencv()
        else:
            self._setup_matplotlib()
    
    def _setup_opencv(self):
        """Setup OpenCV display - much faster than matplotlib"""
        import cv2
        self.cv2 = cv2
        # Create window
        cv2.namedWindow('MLX90640 Thermal', cv2.WINDOW_NORMAL)
        cv2.resizeWindow('MLX90640 Thermal', 640, 480)
        
    def _setup_matplotlib(self):
        """Setup matplotlib display with optimizations"""
        import matplotlib
        matplotlib.use('TkAgg')  # Faster backend on Windows
        import matplotlib.pyplot as plt
        self.plt = plt
        
        plt.style.use('fast')
        self.fig, self.ax = plt.subplots(figsize=(8, 6))
        
        # Pre-allocate image with fixed color limits initially
        self.im = self.ax.imshow(self.frame_data, cmap='inferno', 
                                  interpolation='nearest',
                                  vmin=0, vmax=65535,
                                  animated=True)  # Enable blitting
        self.colorbar = plt.colorbar(self.im, ax=self.ax, label='Raw Value')
        self.title = self.ax.set_title('MLX90640 Thermal Image')
        self.ax.set_xlabel('X (32 pixels)')
        self.ax.set_ylabel('Y (24 pixels)')
        
        # Disable autoscaling for speed
        self.ax.set_autoscale_on(False)
        
        # Draw once and cache background for blitting
        self.fig.canvas.draw()
        self.bg = self.fig.canvas.copy_from_bbox(self.ax.bbox)
        
        plt.ion()  # Interactive mode
        plt.show(block=False)
        plt.pause(0.01)
        
    def connect(self):
        """Connect to serial port"""
        try:
            self.ser = serial.Serial(
                self.port, 
                self.baudrate, 
                timeout=0.5,  # Shorter timeout for responsiveness
                write_timeout=0.5
            )
            # Increase buffer size for faster reads
            self.ser.set_buffer_size(rx_size=65536, tx_size=4096)
            print(f"Connected to {self.port} at {self.baudrate} baud")
            time.sleep(1)  # Reduced wait time
            self.ser.reset_input_buffer()
            return True
        except serial.SerialException as e:
            print(f"Error connecting to {self.port}: {e}")
            return False
    
    def wait_for_frame_start(self):
        """Wait for FRAME_START marker or detect frame data directly"""
        timeout = time.time() + 5  # 5 second timeout
        while self.running and time.time() < timeout:
            if self.ser.in_waiting > 0:
                line = self.ser.readline().decode('utf-8', errors='ignore').strip()
                if 'FRAME_START' in line:
                    return True
                # Also detect if we see a line that looks like pixel data (32 hex values)
                # This handles case where FRAME_START was missed
                if ',' in line and len(HEX_PATTERN.findall(line)) >= 20:
                    # Push this line back by storing it for read_frame
                    self._pending_line = line
                    return True
                # Print other messages for debugging (limit output)
                if line and 'FRAME' not in line and not line.startswith('FF'):
                    print(f"[MCU] {line}")
        return False
    
    def read_frame(self):
        """Read one frame of pixel data - accepts ALL hex values including 0xFFFF"""
        pixels = []
        error_count = 0
        max_lines = 100
        lines_read = 0
        frame_start_time = time.time()
        
        # Check for pending line from wait_for_frame_start
        if hasattr(self, '_pending_line') and self._pending_line:
            line = self._pending_line
            self._pending_line = None
            matches = HEX_PATTERN.findall(line)
            for v in matches:
                pixels.append(int(v, 16))
        
        while self.running and len(pixels) < 768 and lines_read < max_lines:
            lines_read += 1
            
            try:
                line = self.ser.readline().decode('utf-8', errors='ignore').strip()
            except:
                continue
            
            if 'FRAME_END' in line:
                break
            if 'FRAME_START' in line:
                pixels = []
                error_count = 0
                lines_read = 0
                frame_start_time = time.time()
                continue
            if not line:
                continue
            
            # Skip non-data lines (errors, debug messages)
            if 'Errors:' in line or 'Loop' in line or 'Hotspot' in line or 'MPU' in line:
                continue
            if 'FAIL' in line or 'Timeout' in line or 'WHO_AM' in line:
                continue
            
            # Parse hex values - accept ALL values including 0xFFFF
            matches = HEX_PATTERN.findall(line)
            if matches:
                for v in matches:
                    val = int(v, 16)
                    pixels.append(val)
                    if val == 0xFFFF:
                        error_count += 1
        
        frame_time = time.time() - frame_start_time
        
        if len(pixels) >= 768:
            frame_data = np.array(pixels[:768], dtype=np.uint16).reshape(24, 32)
            return frame_data, error_count, frame_time
        elif len(pixels) >= 384:
            # Partial frame - pad with zeros or repeat last value
            print(f"[Frame] Partial: {len(pixels)}/768 pixels, {error_count} I2C errors")
            padded = np.zeros(768, dtype=np.uint16)
            padded[:len(pixels)] = pixels
            if len(pixels) > 0:
                padded[len(pixels):] = pixels[-1]  # Repeat last value
            return padded.reshape(24, 32), error_count, frame_time
        else:
            if len(pixels) > 0:
                print(f"[Frame] Incomplete: {len(pixels)}/768 pixels")
            return None, error_count, frame_time
    
    def serial_reader_thread(self):
        """Background thread to read serial data without blocking GUI"""
        print("[Thread] Serial reader started")
        while self.running:
            try:
                if self.wait_for_frame_start():
                    result = self.read_frame()
                    if result[0] is not None:
                        # Put frame in queue (drop old frames if queue full)
                        try:
                            self.frame_queue.put_nowait(result)
                        except:
                            # Queue full, drop oldest and add new
                            try:
                                self.frame_queue.get_nowait()
                                self.frame_queue.put_nowait(result)
                            except:
                                pass
            except Exception as e:
                if self.running:
                    print(f"[Thread] Error: {e}")
                    time.sleep(0.1)
        print("[Thread] Serial reader stopped")
    
    def update_plot(self, frame_data, error_count, frame_time):
        """Update the plot with new data - optimized for speed"""
        self.frame_data = frame_data
        self.frame_count += 1
        
        # Calculate FPS every second
        now = time.time()
        if now - self.last_fps_time >= 1.0:
            self.fps = self.frame_count / (now - self.last_fps_time)
            self.frame_count = 0
            self.last_fps_time = now
        
        if self.use_opencv:
            self._update_opencv(error_count, frame_time)
        else:
            self._update_matplotlib(error_count, frame_time)
    
    def _update_opencv(self, error_count=0, frame_time=0):
        """Update OpenCV display - very fast"""
        # Normalize to 0-255 for display
        vmin, vmax = self.frame_data.min(), self.frame_data.max()
        if vmax > vmin:
            normalized = ((self.frame_data - vmin) / (vmax - vmin) * 255).astype(np.uint8)
        else:
            normalized = np.zeros((24, 32), dtype=np.uint8)
        
        # Apply colormap (COLORMAP_INFERNO or COLORMAP_JET)
        colored = self.cv2.applyColorMap(normalized, self.cv2.COLORMAP_INFERNO)
        
        # Upscale for better visibility
        display = self.cv2.resize(colored, (640, 480), interpolation=self.cv2.INTER_NEAREST)
        
        # Add text overlay with more info
        self.cv2.putText(display, f"Min: {vmin:.0f} Max: {vmax:.0f}", 
                        (10, 30), self.cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)
        self.cv2.putText(display, f"FPS: {self.fps:.1f} | MCU frame: {frame_time*1000:.0f}ms", 
                        (10, 60), self.cv2.FONT_HERSHEY_SIMPLEX, 0.6, (200, 200, 200), 1)
        if error_count > 0:
            self.cv2.putText(display, f"I2C errors: {error_count}", 
                            (10, 90), self.cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 0, 255), 1)
        
        self.cv2.imshow('MLX90640 Thermal', display)
        
        # Process window events (1ms wait)
        if self.cv2.waitKey(1) & 0xFF == ord('q'):
            self.running = False
    
    def _update_matplotlib(self, error_count=0, frame_time=0):
        """Update matplotlib display with blitting for speed"""
        vmin, vmax = self.frame_data.min(), self.frame_data.max()
        
        # Update image data
        self.im.set_data(self.frame_data)
        
        # Update color limits only if range changed significantly
        if vmax > vmin:
            self.im.set_clim(vmin=vmin, vmax=vmax)
        
        # Update title with timing info
        self.title.set_text(f'Min: {vmin:.0f}, Max: {vmax:.0f} | FPS: {self.fps:.1f} | MCU: {frame_time*1000:.0f}ms')
        
        # Use blitting for faster updates
        self.fig.canvas.restore_region(self.bg)
        self.ax.draw_artist(self.im)
        self.ax.draw_artist(self.title)
        self.fig.canvas.blit(self.ax.bbox)
        self.fig.canvas.flush_events()
    
    def run(self):
        """Main loop with threaded serial reading"""
        if not self.connect():
            return
        
        print("="*50)
        print("MLX90640 Thermal Viewer")
        print("="*50)
        print("Waiting for thermal data...")
        print("Press Ctrl+C to exit" + (" or 'q' in window" if self.use_opencv else ""))
        print("\nNOTE: If 'MCU frame' time is >500ms, the bottleneck is")
        print("      your ATmega's I2C speed, not Python!")
        print("="*50)
        
        # Start serial reader in background thread
        reader_thread = threading.Thread(target=self.serial_reader_thread, daemon=True)
        reader_thread.start()
        
        try:
            while self.running:
                try:
                    # Get frame from queue with timeout (keeps GUI responsive)
                    frame_data, error_count, frame_time = self.frame_queue.get(timeout=0.1)
                    self.update_plot(frame_data, error_count, frame_time)
                except Empty:
                    # No frame ready, just keep GUI responsive
                    if self.use_opencv:
                        if self.cv2.waitKey(1) & 0xFF == ord('q'):
                            self.running = False
                    else:
                        self.fig.canvas.flush_events()
        except KeyboardInterrupt:
            print("\nExiting...")
        finally:
            self.running = False
            reader_thread.join(timeout=1.0)
            if self.ser:
                self.ser.close()
            if self.use_opencv:
                self.cv2.destroyAllWindows()
            else:
                self.plt.close()


def main():
    if len(sys.argv) < 2:
        print("Usage: python mlx_viewer.py <COM_PORT> [BAUDRATE] [--opencv]")
        print("Example: python mlx_viewer.py COM9 115200")
        print("         python mlx_viewer.py COM9 115200 --opencv  (faster display)")
        print("\nAvailable ports:")
        import serial.tools.list_ports
        for port in serial.tools.list_ports.comports():
            print(f"  {port.device}: {port.description}")
        sys.exit(1)
    
    port = sys.argv[1]
    baudrate = int(sys.argv[2]) if len(sys.argv) > 2 and sys.argv[2].isdigit() else 115200
    use_opencv = '--opencv' in sys.argv
    
    if use_opencv:
        try:
            import cv2
            print("Using OpenCV for faster display")
        except ImportError:
            print("OpenCV not found, falling back to matplotlib")
            print("Install with: pip install opencv-python")
            use_opencv = False
    
    viewer = MLX90640Viewer(port, baudrate, use_opencv)
    viewer.run()


if __name__ == "__main__":
    main()
