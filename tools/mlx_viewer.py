#!/usr/bin/env python3
"""
MLX90640 Thermal Image Viewer
Receives raw pixel data from ATmega4809 via serial and displays as heatmap.

Usage:
    python mlx_viewer.py COM9 115200

Requirements:
    pip install pyserial numpy matplotlib
"""

import serial
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import sys
import time

class MLX90640Viewer:
    def __init__(self, port, baudrate=115200):
        self.port = port
        self.baudrate = baudrate
        self.ser = None
        self.frame_data = np.zeros((24, 32))
        
        # Setup plot
        plt.ion()  # Interactive mode
        self.fig, self.ax = plt.subplots(figsize=(10, 7))
        self.im = self.ax.imshow(self.frame_data, cmap='inferno', 
                                  interpolation='bilinear',
                                  vmin=0, vmax=65535)
        self.colorbar = plt.colorbar(self.im, ax=self.ax, label='Raw Value')
        self.ax.set_title('MLX90640 Thermal Image (Raw)')
        self.ax.set_xlabel('X (32 pixels)')
        self.ax.set_ylabel('Y (24 pixels)')
        
    def connect(self):
        """Connect to serial port"""
        try:
            self.ser = serial.Serial(self.port, self.baudrate, timeout=2)
            print(f"Connected to {self.port} at {self.baudrate} baud")
            time.sleep(2)  # Wait for connection to stabilize
            self.ser.flushInput()
            return True
        except serial.SerialException as e:
            print(f"Error connecting to {self.port}: {e}")
            return False
    
    def wait_for_frame_start(self):
        """Wait for FRAME_START marker"""
        while True:
            line = self.ser.readline().decode('utf-8', errors='ignore').strip()
            if 'FRAME_START' in line:
                return True
            # Print other messages for debugging
            if line and 'FRAME' not in line:
                print(f"[MCU] {line}")
    
    def is_valid_hex(self, s):
        """Check if string is a valid 4-digit hex value"""
        if len(s) != 4:
            return False
        try:
            int(s, 16)
            return True
        except ValueError:
            return False
    
    def read_frame(self):
        """Read one frame of pixel data"""
        pixels = []
        error_count = 0
        max_attempts = 50  # Max lines to read before giving up
        attempts = 0
        
        while len(pixels) < 768 and attempts < max_attempts:
            attempts += 1
            line = self.ser.readline().decode('utf-8', errors='ignore').strip()
            
            if 'FRAME_END' in line:
                break
            if 'FRAME_START' in line:
                pixels = []  # Reset if we see another start
                error_count = 0
                attempts = 0
                continue
            if not line:
                continue
            
            # Check if line contains error keywords - skip entire line
            if any(err in line for err in ['FAIL', 'Timeout', 'TO,', 'arbitration', 'MSTATUS', 'RIF=', 'Errors:']):
                error_count += 1
                continue
                
            # Parse hex values separated by commas
            values = line.split(',')
            for v in values:
                v = v.strip()
                # Only accept exactly 4-character hex values (0000-FFFF)
                if self.is_valid_hex(v):
                    val = int(v, 16)
                    # Skip 0xFFFF values (I2C error indicators)
                    if val != 0xFFFF:
                        pixels.append(val)
                    else:
                        error_count += 1
        
        if error_count > 0:
            print(f"[Warning] {error_count} read errors during frame")
        
        if len(pixels) >= 768:
            # Reshape to 24x32 image
            self.frame_data = np.array(pixels[:768]).reshape(24, 32)
            return True
        elif len(pixels) >= 384:
            # Got at least half a frame - pad with last known values or zeros
            print(f"Partial frame: got {len(pixels)} pixels, padding to 768")
            padded = np.zeros(768)
            padded[:len(pixels)] = pixels
            # Fill remaining with average of received pixels
            if len(pixels) > 0:
                avg = np.mean(pixels)
                padded[len(pixels):] = avg
            self.frame_data = padded.reshape(24, 32)
            return True
        else:
            print(f"Incomplete frame: got {len(pixels)} pixels (need at least 384)")
            return False
    
    def update_plot(self):
        """Update the plot with new data"""
        self.im.set_data(self.frame_data)
        self.im.set_clim(vmin=self.frame_data.min(), vmax=self.frame_data.max())
        self.ax.set_title(f'MLX90640 Thermal Image\nMin: {self.frame_data.min():.0f}, Max: {self.frame_data.max():.0f}')
        self.fig.canvas.draw()
        self.fig.canvas.flush_events()
    
    def run(self):
        """Main loop"""
        if not self.connect():
            return
        
        print("Waiting for thermal data...")
        print("Press Ctrl+C to exit")
        
        try:
            while True:
                if self.wait_for_frame_start():
                    if self.read_frame():
                        self.update_plot()
                        print(f"Frame received - Min: {self.frame_data.min():.0f}, Max: {self.frame_data.max():.0f}")
        except KeyboardInterrupt:
            print("\nExiting...")
        finally:
            if self.ser:
                self.ser.close()
            plt.close()


def main():
    if len(sys.argv) < 2:
        print("Usage: python mlx_viewer.py <COM_PORT> [BAUDRATE]")
        print("Example: python mlx_viewer.py COM9 115200")
        print("\nAvailable ports:")
        import serial.tools.list_ports
        for port in serial.tools.list_ports.comports():
            print(f"  {port.device}: {port.description}")
        sys.exit(1)
    
    port = sys.argv[1]
    baudrate = int(sys.argv[2]) if len(sys.argv) > 2 else 115200
    
    viewer = MLX90640Viewer(port, baudrate)
    viewer.run()


if __name__ == "__main__":
    main()
