# RBC MIDI Pico (RP2040) - 5 encoders (with shift) + 4x4 keypad -> USB-MIDI

## Requirements (Linux)
- pico-sdk
- toolchain: gcc-arm-none-eabi, cmake

## Install

```bash
sudo apt update
sudo apt install -y cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential git
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

Copy once:
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
2. Copy output file:
   ```
   build/rbc_midi_pico.uf2
   ```
   to RP2040 mass storage
