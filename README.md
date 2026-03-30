# RBC MIDI Pico (RP2040) - 5 encoders (with shift) + 4x4 keypad -> USB-MIDI

## Requirements (Linux)
- pico-sdk
- toolchain: gcc-arm-none-eabi, cmake

Install:
    sudo apt update
    sudo apt install -y cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential git
    
Pico SDK:
    cd ~
    git clone https://github.com/raspberrypi/pico-sdk.git
    cd pico-sdk
    git submodule update --init --recursive
    
## Build
From project folder:

1) Ensure pico_sdk_import.cmake exists next to CMakeLists.txt
   Copy once:
          cp ~/pico-sdk/external/pico_sdk_import.cmake .

2) Build:
       export PICO_SDK_PATH=~/pico-sdk
       mkdir -p build
       cd build
       cmake ..
       make -j
       
Output UF2:
    build/rbc_midi_pico.uf2
    
## Programing RP2040
1) Connect RP2040 via USB cable to PC with BOOT button pressed on RP2040
2) Copy output file (build/rbc_midi_pico.uf2) to mass storage of RP2040

