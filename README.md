# RBC MIDI Pico (RP2040) - MIDI Console for piHPSDR

A compact MIDI control console designed for piHPSDR, based on RP2040.
<img width="1189" height="450" alt="Zrzut ekranu z 2026-03-31 12-58-39" src="https://github.com/user-attachments/assets/3ab81dd0-f35c-4516-b3b3-90445e27155e" />

![1000032407](https://github.com/user-attachments/assets/973bb262-4eb0-4da8-91bb-ad42e5ed375c)

<img width="902" height="631" alt="Zrzut ekranu z 2026-03-31 12-15-01" src="https://github.com/user-attachments/assets/a1cf056e-2424-40a3-be8c-c5088af72df3" />


## Features
- 16-button matrix keypad (4x4)
- 5 rotary encoders

Encoders 2–5 include push buttons. When pressed, each encoder can act as an additional virtual encoder.

The console is easy to configure directly in piHPSDR using its built-in MIDI learning function.
Each button and encoder can be freely assigned to any function.

A CNC handwheel encoder is used for VFO control. For smooth operation, the detent mechanism was removed.
Other encoders are standard low-cost units with integrated push buttons.

---

## GPIO Pin Mapping (RP2040)

### Keypad (4x4 matrix)

| Function | GPIO | Label |
|----------|------|------|
| Column 0 | GP0  | KPC0 |
| Column 1 | GP1  | KPC1 |
| Column 2 | GP2  | KPC2 |
| Column 3 | GP3  | KPC3 |
| Row 0    | GP4  | KPR0 |
| Row 1    | GP5  | KPR1 |
| Row 2    | GP6  | KPR2 |
| Row 3    | GP7  | KPR3 |

---

### Rotary Encoders

| Encoder | Signal | GPIO | Label |
|--------|--------|------|------|
| ENC1 | A | GP8  | ENC1A |
| ENC1 | B | GP9  | ENC1B |
| ENC2 | A | GP10 | ENC2A |
| ENC2 | B | GP11 | ENC2B |
| ENC2 | SW | GP18 | ENC2SW |
| ENC3 | A | GP12 | ENC3A |
| ENC3 | B | GP13 | ENC3B |
| ENC3 | SW | GP19 | ENC3SW |
| ENC4 | A | GP14 | ENC4A |
| ENC4 | B | GP15 | ENC4B |
| ENC4 | SW | GP20 | ENC4SW |
| ENC5 | A | GP16 | ENC5A |
| ENC5 | B | GP17 | ENC5B |
| ENC5 | SW | GP21 | ENC5SW |

---

## Requirements (Linux)
- pico-sdk
- toolchain: gcc-arm-none-eabi, cmake

## Installation

```bash
sudo apt update
sudo apt install -y cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential git
```

### Clone repository

```bash
git clone https://github.com/SQ5BAR/piHPSDR-MIDI-Console.git
cd piHPSDR-MIDI-Console
```

## Pico SDK

```bash
cd ~
git clone https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init --recursive
```

## Build

From project folder:

### 1) Ensure pico_sdk_import.cmake exists next to CMakeLists.txt

```bash
cp ~/pico-sdk/external/pico_sdk_import.cmake .
```

### 2) Build

```bash
export PICO_SDK_PATH=~/pico-sdk
mkdir -p build
cd build
cmake ..
make -j
```

Output UF2:
```bash
build/rbc_midi_pico.uf2
```

## Programming RP2040

1. Connect RP2040 via USB cable to PC with BOOT button pressed  
2. Copy file:
   ```
   build/rbc_midi_pico.uf2
   ```
   to RP2040 mass storage
