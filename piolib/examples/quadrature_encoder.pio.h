// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// ------------------ //
// quadrature_encoder //
// ------------------ //

#define quadrature_encoder_wrap_target 15
#define quadrature_encoder_wrap 23
#define quadrature_encoder_pio_version 0

static const uint16_t quadrature_encoder_program_instructions[] = {
    0x000f, //  0: jmp    15                         
    0x000e, //  1: jmp    14                         
    0x0015, //  2: jmp    21                         
    0x000f, //  3: jmp    15                         
    0x0015, //  4: jmp    21                         
    0x000f, //  5: jmp    15                         
    0x000f, //  6: jmp    15                         
    0x000e, //  7: jmp    14                         
    0x000e, //  8: jmp    14                         
    0x000f, //  9: jmp    15                         
    0x000f, // 10: jmp    15                         
    0x0015, // 11: jmp    21                         
    0x000f, // 12: jmp    15                         
    0x0015, // 13: jmp    21                         
    0x008f, // 14: jmp    y--, 15                    
            //     .wrap_target
    0xa0c2, // 15: mov    isr, y                     
    0x8000, // 16: push   noblock                    
    0x60c2, // 17: out    isr, 2                     
    0x4002, // 18: in     pins, 2                    
    0xa0e6, // 19: mov    osr, isr                   
    0xa0a6, // 20: mov    pc, isr                    
    0xa04a, // 21: mov    y, !y                      
    0x0097, // 22: jmp    y--, 23                    
    0xa04a, // 23: mov    y, !y                      
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program quadrature_encoder_program = {
    .instructions = quadrature_encoder_program_instructions,
    .length = 24,
    .origin = 0,
    .pio_version = 0,
#if PICO_PIO_VERSION > 0
    .used_gpio_ranges = 0x0
#endif
};

static inline pio_sm_config quadrature_encoder_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + quadrature_encoder_wrap_target, offset + quadrature_encoder_wrap);
    return c;
}

#include "hardware/clocks.h"
#include "hardware/gpio.h"
// max_step_rate is used to lower the clock of the state machine to save power
// if the application doesn't require a very high sampling rate. Passing zero
// will set the clock to the maximum
static inline void quadrature_encoder_program_init(PIO pio, uint sm, uint pin, int max_step_rate)
{
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 2, false);
    pio_gpio_init(pio, pin);
    pio_gpio_init(pio, pin + 1);
    gpio_pull_up(pin);
    gpio_pull_up(pin + 1);
    pio_sm_config c = quadrature_encoder_program_get_default_config(0);
    sm_config_set_in_pins(&c, pin); // for WAIT, IN
    sm_config_set_jmp_pin(&c, pin); // for JMP
    // shift to left, autopull disabled
    sm_config_set_in_shift(&c, false, false, 32);
    // don't join FIFO's
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_NONE);
    // passing "0" as the sample frequency,
    if (max_step_rate == 0) {
        sm_config_set_clkdiv(&c, 1.0);
    } else {
        // one state machine loop takes at most 10 cycles
        float div = (float)clock_get_hz(clk_sys) / (10 * max_step_rate);
        sm_config_set_clkdiv(&c, div);
    }
    pio_sm_init(pio, sm, 0, &c);
    pio_sm_set_enabled(pio, sm, true);
}
static inline int32_t quadrature_encoder_get_count(PIO pio, uint sm)
{
    uint ret = 0;
    int n;
    // if the FIFO has N entries, we fetch them to drain the FIFO,
    // plus one entry which will be guaranteed to not be stale
    n = pio_sm_get_rx_fifo_level(pio, sm) + 1;
    while (n > 0) {
        ret = pio_sm_get_blocking(pio, sm);
        n--;
    }
    return ret;
}

#endif

