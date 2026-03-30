#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/bootrom.h"

#include "tusb.h"
#include "bsp/board.h"

// ==================== GPIO MAP ====================
#define KPC0 0
#define KPC1 1
#define KPC2 2
#define KPC3 3
#define KPR0 4
#define KPR1 5
#define KPR2 6
#define KPR3 7

#define ENC1A 8
#define ENC1B 9
#define ENC2A 10
#define ENC2B 11
#define ENC3A 12
#define ENC3B 13
#define ENC4A 14
#define ENC4B 15
#define ENC5A 16
#define ENC5B 17

#define ENC2SW 18
#define ENC3SW 19
#define ENC4SW 20
#define ENC5SW 21


// ==================== MIDI ====================
#define MIDI_CH 0

static const uint8_t enc_cc[5]       = {16, 18, 20, 22, 24};
static const uint8_t enc_cc_shift[5] = {0, 19, 21, 23, 25};

static const uint encsw_pins[4] = {ENC2SW, ENC3SW, ENC4SW, ENC5SW};

// ==================== KEYPAD NOTES ====================
static const uint8_t note_for_key_index[16] = {
    49,45,41,37,51,47,43,39,
    48,44,40,36,50,46,42,38
};

// ==================== MIDI HELPERS ====================
static inline void midi_send_cc_wheel(uint8_t cc, int8_t delta)
{
    if (!tud_midi_mounted()) return;

    uint8_t val = (delta > 0) ? 65 : (delta < 0 ? 63 : 64);
    uint8_t pkt[4] = {0x0B, (uint8_t)(0xB0 | MIDI_CH), cc, val};
    tud_midi_stream_write(0, pkt, 4);
}

static inline void midi_send_note(uint8_t note, bool on)
{
    if (!tud_midi_mounted()) return;

    uint8_t pkt[4] = {
        on ? 0x09 : 0x08,
        (uint8_t)((on ? 0x90 : 0x80) | MIDI_CH),
        note,
        on ? 0x7F : 0x00
    };
    tud_midi_stream_write(0, pkt, 4);
}

// ==================== ENCODERS ====================
typedef struct {
    uint a_pin, b_pin;
    uint8_t prev;
    int8_t acc;
} encoder_t;

static encoder_t enc[5] = {
    {ENC1A, ENC1B, 0, 0},
    {ENC2A, ENC2B, 0, 0},
    {ENC3A, ENC3B, 0, 0},
    {ENC4A, ENC4B, 0, 0},
    {ENC5A, ENC5B, 0, 0},
};

static const int8_t qdec_table[16] = {
    0,-1, 1, 0,
    1, 0, 0,-1,
   -1, 0, 0, 1,
    0, 1,-1, 0
};

static inline uint8_t enc_state(const encoder_t *e)
{
    return (gpio_get(e->a_pin) << 1) | gpio_get(e->b_pin);
}

static void encoders_init(void)
{
    for (int i = 0; i < 5; i++) {
        gpio_init(enc[i].a_pin);
        gpio_init(enc[i].b_pin);
        gpio_set_dir(enc[i].a_pin, GPIO_IN);
        gpio_set_dir(enc[i].b_pin, GPIO_IN);
        gpio_pull_up(enc[i].a_pin);
        gpio_pull_up(enc[i].b_pin);
        enc[i].prev = enc_state(&enc[i]);
    }
}

volatile uint8_t encsw_state = 0;
static uint8_t encsw_db[4] = {0};
#define DEBOUNCE_N 5

static void encsw_init(void)
{
    for (int i = 0; i < 4; i++) {
        gpio_init(encsw_pins[i]);
        gpio_set_dir(encsw_pins[i], GPIO_IN);
        gpio_pull_up(encsw_pins[i]);
    }
}

static void encsw_poll(void)
{
    for (int i = 0; i < 4; i++) {
        bool raw = (gpio_get(encsw_pins[i]) == 0);
        bool st  = (encsw_state >> i) & 1u;
        if (raw != st) {
            if (++encsw_db[i] >= DEBOUNCE_N) {
                encsw_state ^= (1u << i);
                encsw_db[i] = 0;
            }
        } else encsw_db[i] = 0;
    }
}

static void encoders_poll(void)
{
    for (int i = 0; i < 5; i++) {
        uint8_t cur = enc_state(&enc[i]);
        int8_t step = qdec_table[(enc[i].prev << 2) | cur];
        enc[i].prev = cur;
        if (!step) continue;

        enc[i].acc += step;
        if (enc[i].acc >= 4 || enc[i].acc <= -4) {
            int8_t dir = (enc[i].acc > 0) ? +1 : -1;
            enc[i].acc = 0;

            uint8_t cc = enc_cc[i];
            if (i >= 1 && ((encsw_state >> (i - 1)) & 1u)) {
                cc = enc_cc_shift[i];
            }
            midi_send_cc_wheel(cc, dir);
        }
    }
}

// ==================== KEYPAD ====================
static const uint row_pins[4] = {KPR0, KPR1, KPR2, KPR3};
static const uint col_pins[4] = {KPC0, KPC1, KPC2, KPC3};

static uint16_t keys_state = 0, keys_prev = 0;
static uint8_t key_db[16] = {0};

static void keypad_init(void)
{
    for (int r = 0; r < 4; r++) {
        gpio_init(row_pins[r]);
        gpio_set_dir(row_pins[r], GPIO_OUT);
        gpio_put(row_pins[r], 1);
    }
    for (int c = 0; c < 4; c++) {
        gpio_init(col_pins[c]);
        gpio_set_dir(col_pins[c], GPIO_IN);
        gpio_pull_up(col_pins[c]);
    }
}

static uint16_t keypad_scan(void)
{
    uint16_t m = 0;
    for (int r = 0; r < 4; r++) {
        gpio_put(row_pins[r], 0);
        sleep_us(40);
        for (int c = 0; c < 4; c++)
            if (!gpio_get(col_pins[c])) m |= (1u << (r * 4 + c));
        gpio_put(row_pins[r], 1);
    }
    return m;
}

static void keypad_poll(void)
{
    uint16_t raw = keypad_scan();
    for (int i = 0; i < 16; i++) {
        bool r = (raw >> i) & 1u;
        bool s = (keys_state >> i) & 1u;
        if (r != s) {
            if (++key_db[i] >= DEBOUNCE_N) {
                keys_state ^= (1u << i);
                key_db[i] = 0;
            }
        } else key_db[i] = 0;
    }

    uint16_t chg = keys_state ^ keys_prev;
    if (chg) {
        for (int i = 0; i < 16; i++) {
            if (chg & (1u << i)) {
                midi_send_note(note_for_key_index[i], (keys_state >> i) & 1u);
            }
        }
        keys_prev = keys_state;
    }
}

// ==================== CDC -> BOOTSEL ====================
static void cdc_poll(void)
{
    if (!tud_cdc_connected()) return;
    while (tud_cdc_available()) {
        char c;
        tud_cdc_read(&c, 1);
        if (c == '\n' || c == '\r') {
            reset_usb_boot(0, 0);
        }
    }
}

// ==================== MAIN ====================
int main(void)
{
    stdio_init_all();
    board_init();

    encoders_init();
    encsw_init();
    keypad_init();

    tusb_init();

    absolute_time_t next = make_timeout_time_ms(1);

    while (true) {
        tud_task();
        cdc_poll();

        if (absolute_time_diff_us(get_absolute_time(), next) <= 0) {
            next = delayed_by_ms(next, 1);
            encsw_poll();
            encoders_poll();
            keypad_poll();
        }
    }
}
