# MicroBlaze V GPS + OLED System

This project demonstrates a RISC-V based embedded system using MicroBlaze V on the Arty A7-100 FPGA board. It receives GPS data from a PmodGPS module and displays parsed results (time, location, altitude, HDOP) on a PmodOLED module, using onboard switches to control the display mode.

## Structure
- `Vivado/`: Hardware block design, IP configuration
- `Vitis/`: Application source code, GPS parser, OLED driver
