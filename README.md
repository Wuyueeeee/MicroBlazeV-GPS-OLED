# MicroBlaze V GPS + OLED System on Arty A7

This project implements a RISC-V based embedded system using the MicroBlaze V soft processor core on the Digilent Arty A7-100T FPGA board. The system receives GPS data via UART from a PmodGPS module and displays key information on a PmodOLED screen. The user can switch between different data views using onboard slide switches (SW0–SW3).

---

## 🔧 Tools & Hardware

- **FPGA Board**: Digilent Arty A7-100T (Artix-7)
- **GPS Module**: Digilent PmodGPS (UART interface)
- **Display Module**: Digilent PmodOLED (SPI interface)
- **Processor**: MicroBlaze V (RISC-V soft-core)
- **Tools**:
  - AMD Vivado Design Suite (for hardware design)
  - AMD Vitis Unified IDE (for software development)
  - Tera Term or equivalent UART terminal (optional debugging)

---

## 🚀 Quick Start

### 1. Vivado – Hardware Design

- Open `project_22_ddr3.xpr` in Vivado
- Review/edit the Block Design (AXI UART16550, SPI OLED, GPIO, MicroBlaze V)
- Generate Bitstream
- Export Hardware with Bitstream → creates `design_3_wrapper.xsa`

### 2. Vitis – Platform & App

- Open Vitis
- Create a new **Platform Project**, import `design_3_wrapper.xsa`
- Create a new **Application Project** (empty C app)
- Import files from `gpstest/` and driver sources (e.g. `PmodGPS.c`, `PmodOLED.c`)
- Modify `lscript.ld` if using DDR memory
- Build and run on hardware

---

## 📊 Features

- UART data reception from GPS module
- NMEA GPGGA sentence parsing
- Data display on OLED:
  - UTC Time
  - Latitude & Longitude
  - Altitude
  - HDOP (Horizontal Dilution of Precision)
- Controlled via onboard switches (SW0–SW3)
