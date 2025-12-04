# Arduino DUE Serial Bridge

ATmega4809의 UART 데이터를 PC로 전달하는 시리얼 브릿지

## 연결
- ATmega4809 PF0 (TX) → DUE Pin 18 (TX1)
- ATmega4809 PF1 (RX) → DUE Pin 19 (RX1)
- GND 공통 연결

## 사용 방법

### 1. Arduino CLI 설치 (필요시)
```powershell
winget install ArduinoSA.CLI
# 또는
scoop install arduino-cli
```

### 2. Arduino SAM Core 설치
```powershell
make install-core
```

### 3. DUE COM 포트 확인
```powershell
make list-ports
```

### 4. 업로드 (COM 포트 지정)
```powershell
make flash PORT=COM5
```

## PuTTY 설정
- Port: DUE의 COM 포트
- Speed: 115200
- Data bits: 8
- Stop bits: 1
- Parity: None
