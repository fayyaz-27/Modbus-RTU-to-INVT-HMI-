# ESP32 ↔ INVT HMI Communication via Modbus RTU over RS485

Display real-time voltage readings from an ESP32 ADC on an INVT 7-inch HMI using Modbus RTU over RS485.

---

## Overview

This project establishes serial communication between an ESP32 microcontroller and an INVT HMI display using the Modbus RTU protocol over RS485. The ESP32 reads an analog voltage (0–3.3V) from a potentiometer connected to its ADC pin, scales the value, and writes it to the HMI's local word register `LW0`. The HMI reads and displays the voltage in real time.

---

## Hardware

| Component | Details |
|---|---|
| Microcontroller | ESP32 |
| Display | INVT 7-inch HMI |
| Interface Converter | TTL-to-RS485 module |
| Input | Potentiometer (connected to ADC pin GPIO25) |

**Wiring:**

```
ESP32 GPIO17 (TX) ──► TTL-RS485 DI
ESP32 GPIO16 (RX) ◄── TTL-RS485 RO
ESP32 GPIO4       ──► TTL-RS485 DE/RE (direction control)
ESP32 GPIO25      ──── Potentiometer wiper (ADC input)
TTL-RS485 A/B     ──── INVT HMI RS485 A/B terminals
```

---

## Software

| Tool | Purpose |
|---|---|
| Arduino IDE / ESP-IDF | ESP32 firmware development |
| INVT HMI Tool | HMI screen design and configuration |

---

## Communication Protocol

| Parameter | Value |
|---|---|
| Protocol | Modbus RTU (ESP32 as Master) |
| Baud Rate | 9600 |
| Data Format | 8N1 |
| Slave ID | 0x01 |
| Function Code | 0x06 (Write Single Register) |
| Target Register | `LW0` — Local Word Register 0 (address `0x0000`) |

---

## How It Works

1. The potentiometer output is read by the ESP32 ADC (12-bit resolution, 0–4095).
2. The raw ADC value is converted to voltage: `voltage = (adc / 4095.0) × 3.3`
3. The voltage is scaled by ×10 and cast to a `uint16_t` (e.g., 3.30V → `330`).
4. A Modbus RTU frame is constructed with CRC-16 and transmitted to the HMI over RS485.
5. The HMI reads register `LW0` and displays the voltage value on screen.

---

## Register Map

| Register | Address | Data | Display |
|---|---|---|---|
| `LW0` | `0x0000` | Voltage × 10 (e.g., `330` = 3.30V) | Voltage Measurement (0–3.3V) |

---

## Project Structure

```
├── main.ino / main.cpp     # ESP32 firmware
└── README.md               # This file
```

HMI project files: [Download here](https://lnkd.in/dQmYUuPZ)

---

## Getting Started

1. Wire the hardware as described above.
2. Flash the firmware to the ESP32.
3. Open the HMI project in INVT HMI Tool, configure the display element to read register `LW0`, and upload to the HMI.
4. Power both devices and adjust the potentiometer — the voltage should update on the HMI display in real time.

---

## Notes

- The INVT HMI is a cost-effective alternative to Delta HMI at roughly half the price.
- INVT HMI Tool is free to use and official documentation is available online.
- The DE/RE pin is driven `HIGH` before transmitting and `LOW` immediately after to switch the RS485 transceiver between transmit and receive modes.
- Voltage scaling (×10) allows the HMI to display the value as an integer while retaining one decimal place of precision.

---

## Planned Features

- [ ] Measure and display 4 channels simultaneously
- [ ] DC Voltage measurement
- [ ] DC Current measurement
- [ ] AC Voltage measurement
- [ ] AC Current measurement
- [ ] Beginner tutorial documentation
- [ ] YouTube tutorial series on HMI interfacing

---

## Author

Built from scratch — no AI assistance, just hardware, debugging, and persistence. 😄

---

## License

MIT License — free to use, modify, and distribute.
