# ESP32-CAM WiFi Smart Car Learning Kit

## Live Video Streaming | Open-Source Arduino | No App Required

Control your smart car and watch real-time camera footage directly from your phone browser. Perfect for learning ESP32, Arduino, motor control, and WiFi networking.

[YouTube Tutorial (coming soon)] | [中文文档 →](docs/product-manual.txt)

---

## Repository Contents

```
├── README.txt                          # This file (English)
├── docs/
│   ├── product-manual.txt              # Full product manual (Chinese)
│   ├── product-manual.docx             # Manual in Word format
│   └── wiring-guide.txt                # Wiring instructions (English)
├── hardware/
│   └── BOM.txt                         # Bill of Materials (English)
├── firmware/                           # PlatformIO project
│   ├── platformio.ini                  # Project configuration
│   ├── src/main.cpp                    # Main firmware (English comments)
│   ├── src/test_jpeg.h                 # Fallback test image
│   └── precompiled/firmware.bin        # Pre-compiled binary
└── firmware_arduino/                   # Arduino IDE version
    ├── README.txt                      # Arduino setup guide (English)
    ├── esp32_cam_car/esp32_cam_car.ino # Arduino sketch
    └── firmware.bin                    # Pre-compiled binary
```

## Quick Start

1. **Buy the hardware** (see [BOM.txt](hardware/BOM.txt))
2. **Wire it up** (see [wiring-guide.txt](docs/wiring-guide.txt))
3. **Flash the firmware** (PlatformIO or Arduino IDE)
4. **Connect your phone** - browser to the car's IP
5. **Drive!**

## Hardware Requirements

| Component | Qty | Notes |
|-----------|-----|-------|
| ESP32-CAM (AI-Thinker) | 1 | With OV2640 or OV3660 camera |
| L298N Motor Driver | 1 | Dual H-bridge |
| 4-wheel chassis kit | 1 | With 4 DC motors |
| LM2596S Buck Converter | 1 | 7.4V -> 5.0V |
| 18650 Batteries | 2 | 3.7V each, wired in series |
| Jumper wires | Many | Female-to-female, male-to-female |

## Features

- **Live video** at 320x240 (QVGA), ~5-8 FPS
- **Motor control**: forward, backward, left, right, stop
- **Adjustable speed**: default ~70%, boost to 100%
- **LED flash light** toggle for dark conditions
- **Dual WiFi mode**: connect to home WiFi or use ESP32 as hotspot
- **Mobile browser UI**: no app installation needed
- **Open source**: PlatformIO + Arduino IDE compatible

## Development

### PlatformIO (Recommended)

Open the `firmware/` folder in VS Code with PlatformIO.

### Arduino IDE

Follow the guide in `firmware_arduino/README.txt`.

## License

MIT License - Free to use, modify, and distribute.
