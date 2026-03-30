# RBC MIDI Pico (RP2040) - MIDI Console for piHPSDR

A compact MIDI control console designed for piHPSDR, based on RP2040.

## Features
- 16-button matrix keypad (4x4)
- 5 rotary encoders

Encoders 2–5 include push buttons. When pressed, each encoder can act as an additional virtual encoder.

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
