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
        """Wait for FRAME_START marker"""
        timeout = time.time() + 5  # 5 second timeout
        while time.time() < timeout:
            if self.ser.in_waiting > 0:
                line = self.ser.readline().decode('utf-8', errors='ignore').strip()
                if 'FRAME_START' in line:
                    return True
                # Print other messages for debugging (less frequently)
                if line and 'FRAME' not in line and self.frame_count % 10 == 0:
                    print(f"[MCU] {line}")
        return False
    
    def read_frame(self):
        """Read one frame of pixel data - optimized for speed"""
        pixels = []
        error_count = 0
        max_lines = 50
        lines_read = 0
        
        # Read all available data at once for efficiency
        while len(pixels) < 768 and lines_read < max_lines:
            lines_read += 1
            
            # Check if data available (non-blocking check)
            if self.ser.in_waiting == 0:
                time.sleep(0.001)  # Brief sleep if no data
                continue
                
            line = self.ser.readline().decode('utf-8', errors='ignore').strip()
            
            if 'FRAME_END' in line:
                break
            if 'FRAME_START' in line:
                pixels = []
                error_count = 0
                lines_read = 0
                continue
            if not line:
                continue
            
            # Skip error lines quickly
            if any(c in line for c in ['F', 'T', 'a', 'M', 'R', 'E']) and \
               any(err in line for err in ['FAIL', 'Timeout', 'TO,', 'arbitration', 'MSTATUS', 'RIF=', 'Errors:']):
                error_count += 1
                continue
            
            # Fast hex parsing using pre-compiled regex
            matches = HEX_PATTERN.findall(line)
            for v in matches:
                val = int(v, 16)
                if val != 0xFFFF:
                    pixels.append(val)
                else:
                    error_count += 1
        
        if error_count > 0 and self.frame_count % 10 == 0:
            print(f"[Warning] {error_count} read errors during frame")
        
        if len(pixels) >= 768:
            self.frame_data = np.array(pixels[:768], dtype=np.uint16).reshape(24, 32)
            return True
        elif len(pixels) >= 384:
            padded = np.zeros(768, dtype=np.uint16)
            padded[:len(pixels)] = pixels
            if len(pixels) > 0:
                avg = int(np.mean(pixels))
                padded[len(pixels):] = avg
            self.frame_data = padded.reshape(24, 32)
            return True
        else:
            if len(pixels) > 0:
                print(f"Incomplete frame: got {len(pixels)} pixels")
            return False
    
    def update_plot(self):
        """Update the plot with new data - optimized for speed"""
        self.frame_count += 1
        
        # Calculate FPS every second
        now = time.time()
        if now - self.last_fps_time >= 1.0:
            self.fps = self.frame_count / (now - self.last_fps_time)
            self.frame_count = 0
            self.last_fps_time = now
        
        if self.use_opencv:
            self._update_opencv()
        else:
            self._update_matplotlib()
    
    def _update_opencv(self):
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
        
        # Add text overlay
        self.cv2.putText(display, f"Min: {vmin:.0f} Max: {vmax:.0f} FPS: {self.fps:.1f}", 
                        (10, 30), self.cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)
        
        self.cv2.imshow('MLX90640 Thermal', display)
        
        # Process window events (1ms wait)
        if self.cv2.waitKey(1) & 0xFF == ord('q'):
            raise KeyboardInterrupt()
    
    def _update_matplotlib(self):
        """Update matplotlib display with blitting for speed"""
        vmin, vmax = self.frame_data.min(), self.frame_data.max()
        
        # Update image data
        self.im.set_data(self.frame_data)
        
        # Update color limits only if range changed significantly
        if vmax > vmin:
            self.im.set_clim(vmin=vmin, vmax=vmax)
        
        # Update title less frequently (every 5 frames)
        if self.frame_count % 5 == 0:
            self.title.set_text(f'MLX90640 - Min: {vmin:.0f}, Max: {vmax:.0f}, FPS: {self.fps:.1f}')
        
        # Use blitting for faster updates
        self.fig.canvas.restore_region(self.bg)
        self.ax.draw_artist(self.im)
        self.fig.canvas.blit(self.ax.bbox)
        self.fig.canvas.flush_events()
    
    def run(self):
        """Main loop"""
        if not self.connect():
            return
        
        print("Waiting for thermal data...")
        print("Press Ctrl+C to exit" + (" or 'q' in window" if self.use_opencv else ""))
        
        try:
            while True:
                if self.wait_for_frame_start():
                    if self.read_frame():
                        self.update_plot()
                        # Print status less frequently
                        if self.frame_count % 10 == 0:
                            print(f"Frame OK - Min: {self.frame_data.min():.0f}, Max: {self.frame_data.max():.0f}, FPS: {self.fps:.1f}")
        except KeyboardInterrupt:
            print("\nExiting...")
        finally:
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
