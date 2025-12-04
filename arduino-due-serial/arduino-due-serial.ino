// Serial bridge for Arduino DUE
// Forwards data between ATmega4809 (Serial1) and PC (USB Serial)

void setup() {
  Serial.begin(115200);   // USB to PC
  Serial1.begin(9600);    // UART to ATmega4809 (Pin 19=RX1, 18=TX1)
  
  // Wait for USB serial to connect (optional)
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  
  Serial.println("Arduino Due Serial Bridge Ready!");
  Serial.println("Waiting for data from ATmega4809...");
  Serial.println("Connections needed:");
  Serial.println("  ATmega4809 PF0 (TX) --> Due Pin 19 (RX1)");
  Serial.println("  ATmega4809 GND      --> Due GND");
  Serial.println("----------------------------------------");
  Serial.println("DEBUG MODE: Will show hex values of received bytes");
  Serial.println("----------------------------------------");
}

unsigned long lastCheck = 0;
unsigned long byteCount = 0;

void loop() {
  // Forward data from ATmega4809 to PC with debug info
  if (Serial1.available()) {
    char c = Serial1.read();
    byteCount++;
    
    // Show the character and its hex value
    if (c >= 32 && c < 127) {
      // Printable character
      Serial.write(c);
    } else if (c == '\r') {
      Serial.write(c);
    } else if (c == '\n') {
      Serial.write(c);
    } else {
      // Non-printable: show as hex
      Serial.print("[0x");
      if (c < 16) Serial.print("0");
      Serial.print((uint8_t)c, HEX);
      Serial.print("]");
    }
    
    // Reset heartbeat timer on receiving data
    lastCheck = millis();
  }
  
  // Forward data from PC to ATmega4809
  if (Serial.available()) {
    Serial1.write(Serial.read());
  }
  
  // Heartbeat every 5 seconds to show bridge is alive (only if no data)
  if (millis() - lastCheck > 5000) {
    lastCheck = millis();
    Serial.print("[Bridge alive - bytes received so far: ");
    Serial.print(byteCount);
    Serial.println("]");
  }
}
