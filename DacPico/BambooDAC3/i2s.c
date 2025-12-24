// SPDX-License-Identifier: MIT

/**
 * @file i2s.c
 * @author BambooMaster (https://misskey.hakoniwa-project.com/@BambooMaster)
 * @brief pico-i2s-pio
 * @version 0.5
 * @date 2025-11-08
 * 
 */

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/dma.h"
#include "hardware/sync.h"
#include "pico/multicore.h"
#include "hardware/pll.h"
#include "hardware/vreg.h"

#include "i2s.pio.h"
#include "i2s.h"

static spin_lock_t* queue_spin_lock;

static uint i2s_dout_pin        = 18;
static uint i2s_clk_pin_base    = 20;
static uint i2s_mclk_pin        = 22;
static PIO  i2s_pio             = pio0;
static uint i2s_sm              = 0;

static int i2s_dma_chan         = 0;
static bool i2s_use_core1       = false;
static CLOCK_MODE i2s_clock_mode = CLOCK_MODE_DEFAULT;
static I2S_MODE i2s_mode        = MODE_I2S;

static int8_t i2s_buf_length;
static uint8_t enqueue_pos;
static uint8_t dequeue_pos;

static int32_t i2s_buf[I2S_BUF_DEPTH][I2S_DATA_LEN];
static uint32_t i2s_sample[I2S_BUF_DEPTH];

static int32_t mul_l;
static int32_t mul_r;

//-100dB ~ 0dB (1dB step)
static const int32_t db_to_vol[101] = {
	0x20000000,     0x1c8520af,     0x196b230b,     0x16a77dea,     0x1430cd74,     0x11feb33c,     0x1009b9cf,     0xe4b3b63,      0xcbd4b3f,      0xb5aa19b,
    0xa1e89b1,      0x904d1bd,      0x809bcc3,      0x729f5d9,      0x66284d5,      0x5b0c438,      0x5125831,      0x4852697,      0x4074fcb,      0x3972853,
    0x3333333,      0x2da1cde,      0x28ab6b4,      0x243f2fd,      0x204e158,      0x1ccab86,      0x19a9294,      0x16dec56,      0x146211f,      0x122a9c2,
    0x1030dc4,      0xe6e1c6,       0xcdc613,       0xb76562,       0xa373ae,       0x91ad38,       0x81d59e,       0x73b70f,       0x672194,       0x5bea6e,
    0x51eb85,       0x4902e3,       0x411245,       0x39feb2,       0x33b022,       0x2e1127,       0x290ea8,       0x2497a2,       0x209ce9,       0x1d10f9,
    0x19e7c6,       0x171693,       0x1493ce,       0x1256f0,       0x10585e,       0xe9152,        0xcfbc3,        0xb924e,        0xa5028,        0x9310b,
    0x83126,        0x74d16,        0x681d3,        0x5ccab,        0x52b36,        0x49b50,        0x41b10,        0x3a8c3,        0x342e4,        0x2e818,
    0x2972d,        0x24f0e,        0x20ec7,        0x1d57e,        0x1a26f,        0x174ee,        0x14c60,        0x1283b,        0x10804,        0xeb4d,
    0xd1b7,         0xbae8,         0xa695,         0x9477,         0x8452,         0x75ee,         0x691b,         0x5dad,         0x537d,         0x4a68,
    0x4251,         0x3b1b,         0x34ad,         0x2ef3,         0x29d7,         0x254b,         0x213c,         0x1d9f,         0x1a66,         0x1787,
    0x14f8,
};

/**
 * @brief set_playback_stateのデフォルトハンドラ
 * 
 * @param state 再生状態 true:再生開始 false:再生停止
 * @note PICO_DEFAULT_LED_PINで通知
 */
static inline void default_playback_handler(bool state){
    gpio_put(PICO_DEFAULT_LED_PIN, state);
}
static ExternalFunction playback_handler = default_playback_handler;

/**
 * @brief i2s再生状態の切り替わりを通知する
 * 
 * @param state 再生状態 true:再生開始 false:再生停止
 */
static inline void set_playback_state(bool state){
    playback_handler(state);
}

/**
 * @brief システムクロックを180.75MHzに設定する
 * 
 * @note 44.1kHz系 180.75 / 8 = 22.59375MHz
 */
static void set_sys_clock_180750khz(void){
    while (running_on_fpga()) tight_loop_contents();
    clock_configure_undivided(clk_sys, CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX, CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB, USB_CLK_HZ);
    pll_init(pll_sys, 2, 1446 * MHZ, 4, 2);
    clock_configure_undivided(clk_sys, CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX, CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS, 180750 * KHZ);
}

/**
 * @brief システムクロックを196.5MHzに設定する
 * 
 * @note 48.0kHz系 196.5 / 8 = 24.5625MHz
 */
static void set_sys_clock_196500khz(void){
    while (running_on_fpga()) tight_loop_contents();
    clock_configure_undivided(clk_sys, CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX, CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB, USB_CLK_HZ);
    pll_init(pll_sys, 1, 1572 * MHZ, 4, 2);
    clock_configure_undivided(clk_sys, CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX, CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS, 196500 * KHZ);
}

/**
 * @brief システムクロックをgpin0に設定する
 * 
 * @note gpin0 = 45.1584MHz
 */
static void set_sys_clock_gpin0(void){
    while (running_on_fpga()) tight_loop_contents();
    clock_configure_undivided(clk_sys, CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX, CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB, USB_CLK_HZ);
    clock_configure_gpin(clk_sys, 20, 45158400, 45158400);
}

/**
 * @brief システムクロックをgpin1に設定する
 * 
 * @note gpin1 = 49.152MHz
 */
static void set_sys_clock_gpin1(void){
    while (running_on_fpga()) tight_loop_contents();
    clock_configure_undivided(clk_sys, CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX, CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB, USB_CLK_HZ);
    clock_configure_gpin(clk_sys, 22, 49152 * KHZ, 49152 * KHZ);
}

/**
 * @brief i2sのバッファからデータを取り出すハンドラ
 * 
 * @note use_core1がfalseのときに呼び出される
 */
static void __isr __time_critical_func(i2s_handler)(){
	static bool mute;
	static int32_t mute_buff[96 * 2] = {0};
	static uint32_t mute_len = sizeof(mute_buff) / sizeof(int32_t);
	
	if (i2s_buf_length == 0 && mute == false){
        mute = true;
        set_playback_state(false);
    }
	else if (i2s_buf_length >= I2S_START_LEVEL && mute == true){
        mute = false;
        set_playback_state(true);
    }

	if (mute == false){
		dma_channel_transfer_from_buffer_now(i2s_dma_chan, i2s_buf[dequeue_pos], i2s_sample[dequeue_pos]);
		dequeue_pos++;
		if (dequeue_pos >= I2S_BUF_DEPTH){
            dequeue_pos = 0;
        }
		i2s_buf_length--;
	}
	else{
		dma_channel_transfer_from_buffer_now(i2s_dma_chan, mute_buff, mute_len);
	}
    
   	dma_hw->ints0 = 1u << i2s_dma_chan;
}

/**
 * @brief core1のメイン関数
 * 
 * @note use_core1がtrueのときに呼び出される
 */
static void defalut_core1_main(void){
    int32_t* buff;
    int dma_sample[2], sample;
    bool mute = false;
    int32_t mute_buff[96 * 2] = {0};
    uint32_t mute_len = sizeof(mute_buff) / sizeof(int32_t);
    int8_t buf_length;
    static int32_t dma_buff[2][I2S_DATA_LEN];
    uint8_t dma_use = 0;

    while (1){
        buf_length = i2s_get_buf_length();

        if (buf_length == 0 && mute == false){
            mute = true;
            set_playback_state(false);
        }
        else if (buf_length >= I2S_START_LEVEL && mute == true){
            mute = false;
            set_playback_state(true);
        }

        if (mute == true){
            buff = mute_buff;
            sample = mute_len;
        }
        else if (i2s_dequeue(&buff, &sample) == false){
            buff = mute_buff;
            sample = mute_len;
        }
        
        //i2sバッファに格納
        if (i2s_mode == MODE_EXDF){
            //並び替え
            int l = 0;
            for (int i = 0, j = 0; i < sample; i += 2) {
                uint32_t left_upper = part1by1_16(buff[i] & 0xFFFF);
                uint32_t left_lower = part1by1_16(buff[i] >> 16);
                uint32_t right_upper = part1by1_16(buff[i + 1] & 0xFFFF);
                uint32_t right_lower = part1by1_16(buff[i + 1] >> 16);

                dma_buff[dma_use][i] = (left_upper << 1) | right_upper;
                dma_buff[dma_use][i + 1] = (left_lower << 1) | right_lower;
            }
        }
        else if (i2s_mode == MODE_PT8211_DUAL || i2s_mode == MODE_I2S_DUAL){
            //並び替え
            for (int i = 0, j = 0; i < sample; i += 2) {
                uint32_t left_upper = part1by1_16(buff[i] & 0xFFFF);
                uint32_t left_lower = part1by1_16(buff[i] >> 16);
                uint32_t right_upper = part1by1_16(buff[i + 1] & 0xFFFF);
                uint32_t right_lower = part1by1_16(buff[i + 1] >> 16);

                dma_buff[dma_use][j++] = (left_upper << 1) | right_upper;
                dma_buff[dma_use][j++] = (left_lower << 1) | right_lower;

                //反転
                int32_t d_r, d_l;
                if (buff[i] == INT32_MIN){
                    d_l = INT32_MAX;
                }
                else{
                    d_l = -buff[i];
                }
                if (buff[i + 1] == INT32_MIN){
                    d_r = INT32_MAX;
                }
                else{
                    d_r = -buff[i + 1];
                }

                left_upper = part1by1_16(d_l & 0xFFFF);
                left_lower = part1by1_16(d_l >> 16);
                right_upper = part1by1_16(d_r & 0xFFFF);
                right_lower = part1by1_16(d_r >> 16);

                dma_buff[dma_use][j++] = (left_upper << 1) | right_upper;
                dma_buff[dma_use][j++] = (left_lower << 1) | right_lower;
            }
            sample *= 2;
        }
        else {
            for (int i = 0; i < sample; i++){
                dma_buff[dma_use][i] = buff[i];
            }
        }
        dma_sample[dma_use] = sample;

        dma_channel_wait_for_finish_blocking(i2s_dma_chan);
        dma_channel_transfer_from_buffer_now(i2s_dma_chan, dma_buff[dma_use], dma_sample[dma_use]);
        dma_use ^= 1;
    }
}
static Core1MainFunction core1_main_funcion = defalut_core1_main;

void i2s_mclk_set_pin(uint data_pin, uint clock_pin_base, uint mclk_pin){
    i2s_dout_pin = data_pin;
    i2s_clk_pin_base = clock_pin_base;
    i2s_mclk_pin = mclk_pin;
}

//ロージッターモードを使うときはuart,i2s,spi設定よりも先に呼び出す
void i2s_mclk_set_config(PIO pio, uint sm, int dma_ch, bool use_core1, CLOCK_MODE clock_mode, I2S_MODE mode){
    i2s_pio = pio;
    i2s_sm = sm;
    i2s_dma_chan = dma_ch;
    i2s_use_core1 = use_core1;
    i2s_clock_mode = clock_mode;
    i2s_mode = mode;

    //あらかじめclk_periをclk_sysから分離する
    if (i2s_clock_mode == CLOCK_MODE_LOW_JITTER){
        vreg_set_voltage(VREG_VOLTAGE_1_15);
    }
    if (i2s_clock_mode != CLOCK_MODE_DEFAULT){
        clock_configure_undivided(clk_peri, 0, CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB, USB_CLK_HZ);
    }
}

void i2s_init(void){
    pio_sm_config sm_config, sm_config_mclk;
    PIO pio = i2s_pio;
    uint sm = i2s_sm;
    uint data_pin = i2s_dout_pin;
    uint clock_pin_base = i2s_clk_pin_base;
    uint offset, offset_mclk;
    uint pin_mask;

    //i2s pin init
    pio_gpio_init(pio, data_pin);
    pio_gpio_init(pio, clock_pin_base);
    pio_gpio_init(pio, clock_pin_base + 1);
    pio_gpio_init(pio, i2s_mclk_pin);

    //mclk init
    pio_sm_set_consecutive_pindirs(pio, sm + 1, i2s_mclk_pin, 1, true);
    offset_mclk = pio_add_program(pio, &i2s_mclk_program);
    sm_config_mclk = i2s_mclk_program_get_default_config(offset_mclk);
    sm_config_set_set_pins(&sm_config_mclk, i2s_mclk_pin, 1);
    pio_sm_init(pio, sm + 1, offset_mclk, &sm_config_mclk);
    pio_sm_set_enabled(pio, sm + 1, true);

    //i2s data init
    offset = pio_add_program(pio, &i2s_data_program);
    sm_config = i2s_data_program_get_default_config(offset);
    sm_config_set_out_pins(&sm_config, data_pin, 1);
    sm_config_set_sideset_pins(&sm_config, clock_pin_base);
    sm_config_set_out_shift(&sm_config, false, false, 32);
    sm_config_set_fifo_join(&sm_config, PIO_FIFO_JOIN_TX);

    pio_sm_init(pio, sm, offset, &sm_config);
    pin_mask = (1u << data_pin) | (3u << clock_pin_base);
    pio_sm_set_pindirs_with_mask(pio, sm, pin_mask, pin_mask);
    pio_sm_exec(pio, sm, pio_encode_jmp(offset));
    pio_sm_set_pins(pio, sm, 0);
    pio_sm_clear_fifos(pio, sm);
    pio_sm_set_enabled(pio, sm, true);
}

void pt8211_init(void){
    pio_sm_config sm_config;
    PIO pio = i2s_pio;
    uint sm = i2s_sm;
    uint data_pin = i2s_dout_pin;
    uint clock_pin_base = i2s_clk_pin_base;
    uint offset;
    uint pin_mask;

    //pt8211 pin init
    pio_gpio_init(pio, data_pin);
    pio_gpio_init(pio, clock_pin_base);
    pio_gpio_init(pio, clock_pin_base + 1);

    //pt8211 data init
    offset = pio_add_program(pio, &i2s_pt8211_program);
    sm_config = i2s_pt8211_program_get_default_config(offset);
    sm_config_set_out_pins(&sm_config, data_pin, 1);
    sm_config_set_sideset_pins(&sm_config, clock_pin_base);
    sm_config_set_out_shift(&sm_config, false, false, 32);
    sm_config_set_fifo_join(&sm_config, PIO_FIFO_JOIN_TX);

    pio_sm_init(pio, sm, offset, &sm_config);
    pin_mask = (1u << data_pin) | (3u << clock_pin_base);
    pio_sm_set_pindirs_with_mask(pio, sm, pin_mask, pin_mask);
    pio_sm_exec(pio, sm, pio_encode_jmp(offset));
    pio_sm_set_pins(pio, sm, 0);
    pio_sm_clear_fifos(pio, sm);
    pio_sm_set_enabled(pio, sm, true);
}

void exdf_init(void){
    pio_sm_config sm_config;
    PIO pio = i2s_pio;
    uint sm = i2s_sm;
    uint data_pin = i2s_dout_pin;
    uint clock_pin_base = i2s_clk_pin_base;
    uint offset;
    uint pin_mask;

    //exdf pin init
    pio_gpio_init(pio, data_pin);
    pio_gpio_init(pio, data_pin + 1);
    pio_gpio_init(pio, clock_pin_base);
    pio_gpio_init(pio, clock_pin_base + 1);
    pio_gpio_init(pio, clock_pin_base + 2);

    //exdf data init
    offset = pio_add_program(pio, &i2s_exdf_program);
    sm_config = i2s_exdf_program_get_default_config(offset);
    sm_config_set_out_pins(&sm_config, data_pin, 2);
    sm_config_set_sideset_pins(&sm_config, clock_pin_base);
    sm_config_set_out_shift(&sm_config, false, false, 32);
    sm_config_set_fifo_join(&sm_config, PIO_FIFO_JOIN_TX);

    pio_sm_init(pio, sm, offset, &sm_config);
    pin_mask = (3u << data_pin) | (7u << clock_pin_base);
    pio_sm_set_pindirs_with_mask(pio, sm, pin_mask, pin_mask);
    pio_sm_exec(pio, sm, pio_encode_jmp(offset));
    pio_sm_set_pins(pio, sm, 0);
    pio_sm_clear_fifos(pio, sm);
    pio_sm_set_enabled(pio, sm, true);
}

void i2s_dual_init(void){
    pio_sm_config sm_config, sm_config_mclk;
    PIO pio = i2s_pio;
    uint sm = i2s_sm;
    uint data_pin = i2s_dout_pin;
    uint clock_pin_base = i2s_clk_pin_base;
    uint offset, offset_mclk;
    uint pin_mask;

    //i2s dual pin init
    pio_gpio_init(pio, data_pin);
    pio_gpio_init(pio, data_pin + 1);
    pio_gpio_init(pio, clock_pin_base);
    pio_gpio_init(pio, clock_pin_base + 1);
    pio_gpio_init(pio, i2s_mclk_pin);

    //i2s dual data init
    pio_sm_set_consecutive_pindirs(pio, sm + 1, i2s_mclk_pin, 1, true);
    offset_mclk = pio_add_program(pio, &i2s_mclk_program);
    sm_config_mclk = i2s_mclk_program_get_default_config(offset_mclk);
    sm_config_set_set_pins(&sm_config_mclk, i2s_mclk_pin, 1);

    offset = pio_add_program(pio, &i2s_data_dual_program);
    sm_config = i2s_data_dual_program_get_default_config(offset);
    sm_config_set_out_pins(&sm_config, data_pin, 2);
    sm_config_set_sideset_pins(&sm_config, clock_pin_base);
    sm_config_set_out_shift(&sm_config, false, false, 32);
    sm_config_set_fifo_join(&sm_config, PIO_FIFO_JOIN_TX);
}

void pt8211_dual_init(void){
    pio_sm_config sm_config;
    PIO pio = i2s_pio;
    uint sm = i2s_sm;
    uint data_pin = i2s_dout_pin;
    uint clock_pin_base = i2s_clk_pin_base;
    uint offset;
    uint pin_mask;

    //pt8211 dual pin init
    pio_gpio_init(pio, data_pin);
    pio_gpio_init(pio, data_pin + 1);
    pio_gpio_init(pio, clock_pin_base);
    pio_gpio_init(pio, clock_pin_base + 1);

    //pt8211 dual data init
    offset = pio_add_program(pio, &i2s_pt8211_dual_program);
    sm_config = i2s_pt8211_dual_program_get_default_config(offset);
    sm_config_set_out_pins(&sm_config, data_pin, 2);
    sm_config_set_sideset_pins(&sm_config, clock_pin_base);
    sm_config_set_out_shift(&sm_config, false, false, 32);
    sm_config_set_fifo_join(&sm_config, PIO_FIFO_JOIN_TX);

    pio_sm_init(pio, sm, offset, &sm_config);
    pin_mask = (3u << data_pin) | (3u << clock_pin_base);
    pio_sm_set_pindirs_with_mask(pio, sm, pin_mask, pin_mask);
    pio_sm_exec(pio, sm, pio_encode_jmp(offset));
    pio_sm_set_pins(pio, sm, 0);
    pio_sm_clear_fifos(pio, sm);
    pio_sm_set_enabled(pio, sm, true);
}

void i2s_slave_init(void){
    pio_sm_config sm_config;
    PIO pio = i2s_pio;
    uint sm = i2s_sm;
    uint data_pin = i2s_dout_pin;
    uint clock_pin_base = i2s_clk_pin_base;
    uint offset;
    uint pin_mask;
    
    //i2s slave pin init
    pio_gpio_init(pio, data_pin);
    pio_gpio_init(pio, clock_pin_base);
    pio_gpio_init(pio, clock_pin_base + 1);

    //i2s svalse data init
    pio_sm_set_consecutive_pindirs(pio, sm, data_pin, 1, true);
    pio_sm_set_consecutive_pindirs(pio, sm, clock_pin_base, 2, false);
    
    offset = pio_add_program(pio, &i2s_slave_program);
    sm_config = i2s_slave_program_get_default_config(offset);
    sm_config_set_out_pins(&sm_config, data_pin, 1);
    sm_config_set_in_pin_base(&sm_config, clock_pin_base);
    sm_config_set_out_shift(&sm_config, false, false, 32);
    sm_config_set_fifo_join(&sm_config, PIO_FIFO_JOIN_TX);
    pio_sm_init(pio, sm, offset, &sm_config);
    pio_sm_set_enabled(pio, sm, true);
}

void i2s_mclk_init(uint32_t audio_clock){
    pio_sm_config sm_config, sm_config_mclk;
    PIO pio = i2s_pio;
    uint sm = i2s_sm;

    //再生状態をGPIO25で通知
    if (playback_handler == default_playback_handler){
        gpio_init(PICO_DEFAULT_LED_PIN);
        gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    }

    switch (i2s_mode){
        case MODE_I2S:
            i2s_init();
            break;
        case MODE_PT8211:
            pt8211_init();
            break;
        case MODE_EXDF:
            exdf_init();
            break;
        case MODE_I2S_DUAL:
            i2s_dual_init();
            break;
        case MODE_PT8211_DUAL:
            pt8211_dual_init();
            break;
        case MODE_I2S_SLAVE:
            i2s_slave_init();
            break;
    }
    i2s_mclk_change_clock(audio_clock);

    queue_spin_lock = spin_lock_init(spin_lock_claim_unused(true));
    i2s_buf_length = 0;
    enqueue_pos = 0;
    dequeue_pos = 0;


    //dma init
    dma_channel_config conf = dma_channel_get_default_config(i2s_dma_chan);
    
    channel_config_set_read_increment(&conf, true);
    channel_config_set_write_increment(&conf, false);
    channel_config_set_transfer_data_size(&conf, DMA_SIZE_32);
    channel_config_set_dreq(&conf, pio_get_dreq(pio, sm, true));
    
    dma_channel_configure(
        i2s_dma_chan,
        &conf,
        &i2s_pio->txf[i2s_sm],
        NULL,
        0,
        false
    );

    dma_channel_set_irq0_enabled(i2s_dma_chan, true);
    if (i2s_use_core1 == false){
        irq_set_exclusive_handler(DMA_IRQ_0, i2s_handler);
        irq_set_priority(DMA_IRQ_0, 0);
        irq_set_enabled(DMA_IRQ_0, true);
        i2s_handler();
    }

    //core1スタート
    if (i2s_use_core1 == true){
        multicore_launch_core1(core1_main_funcion);
    }
}

void i2s_mclk_change_clock(uint32_t audio_clock){
    //周波数変更
    if (i2s_mode == MODE_I2S_SLAVE){
        int div;
        if (audio_clock % 48000 == 0){
            div = 384000 / audio_clock;
            //ここで外部のクロック変更
            //picoのGPIOクロック出力だとクロック間の同期ができない
        }
        else{
            div = 352800 / audio_clock;
            //ここで外部のクロック変更
            //picoのGPIOクロック出力だとクロック間の同期ができない
        }
    }
    else if (i2s_clock_mode == CLOCK_MODE_DEFAULT){
        float div;
        div = (float)clock_get_hz(clk_sys) / (float)(audio_clock * 128);
        pio_sm_set_clkdiv(i2s_pio, i2s_sm, div);

        //mclk
        if (i2s_mode == MODE_I2S || i2s_mode == MODE_I2S_DUAL){
            if (audio_clock % 48000 == 0){
                div = (float)clock_get_hz(clk_sys) / (49.152f * (float)MHZ);
                pio_sm_set_clkdiv(i2s_pio, i2s_sm + 1, div);
            }
            else{
                div = (float)clock_get_hz(clk_sys) / (45.1584f * (float)MHZ);
                pio_sm_set_clkdiv(i2s_pio, i2s_sm + 1, div);
            }
        }
    }
    else{
        //mclk出力
        if (i2s_mode == MODE_I2S || i2s_mode == MODE_I2S_DUAL){
            switch (i2s_clock_mode){
                case CLOCK_MODE_LOW_JITTER:
                    pio_sm_set_clkdiv_int_frac(i2s_pio, i2s_sm + 1, 4, 0);
                    break;
                case CLOCK_MODE_EXTERNAL:
                    pio_sm_set_clkdiv_int_frac(i2s_pio, i2s_sm + 1, 1, 0);
                    break;
            }
        }

        //pio周波数変更
        uint dev;
        if (audio_clock % 48000 == 0){
            switch (i2s_clock_mode){
                case CLOCK_MODE_LOW_JITTER:
                    set_sys_clock_196500khz();
                    dev = 8 * 192000 / audio_clock;
                    break;
                case CLOCK_MODE_EXTERNAL:
                    set_sys_clock_gpin1();
                    dev = 2 * 192000 / audio_clock;
                    break;
            }
        }
        else {
            switch (i2s_clock_mode){
                case CLOCK_MODE_LOW_JITTER:
                    set_sys_clock_180750khz();
                    dev = 8 * 176400 / audio_clock;
                    break;
                case CLOCK_MODE_EXTERNAL:
                    set_sys_clock_gpin0();
                    dev = 2 * 176400 / audio_clock;
                    break;
            }
        }
        pio_sm_set_clkdiv_int_frac(i2s_pio, i2s_sm, dev, 0);
    }
}

//i2sのバッファにusb受信データを積む
bool i2s_enqueue(uint8_t* in, int sample, uint8_t resolution){
    int i, j;
    static int32_t lch_buf[I2S_DATA_LEN / 2];
    static int32_t rch_buf[I2S_DATA_LEN / 2];

	if (i2s_get_buf_length() < I2S_BUF_DEPTH){
        if (resolution == 16){
            int16_t *d = (int16_t*)in;
            sample /= 2;
            for (i = 0; i < sample / 2; i++){
                lch_buf[i] = *d++ << 16;
                rch_buf[i] = *d++ << 16;
            }
        }
        else if (resolution == 24){
            uint8_t *d = in;
            int32_t e;
            sample /= 3;
            for (i = 0; i < sample / 2; i++){
                e = 0;
                e |= *d++ << 8;
                e |= *d++ << 16;
                e |= *d++ << 24;
                lch_buf[i] = e;
                e = 0;
                e |= *d++ << 8;
                e |= *d++ << 16;
                e |= *d++ << 24;
                rch_buf[i] = e;
            }
        }
        else if (resolution == 32){
            int32_t *d = (int32_t*)in;
            sample /= 4;
            for (i = 0; i < sample / 2; i++){
                lch_buf[i] = *d++;
                rch_buf[i] = *d++;
            }
        }

        //音量処理
        for (i = 0; i < sample / 2; i++){
            lch_buf[i] = (int32_t)(((int64_t)lch_buf[i] * mul_l) >> 29u);
            rch_buf[i] = (int32_t)(((int64_t)rch_buf[i] * mul_r) >> 29u);
        }

        //i2sバッファに格納
        if (i2s_mode == MODE_EXDF && i2s_use_core1 == false){
            //並び替え
            for (int i = 0, j = 0; i < sample / 2; i++) {
                uint32_t left_upper = part1by1_16(lch_buf[i] & 0xFFFF);
                uint32_t left_lower = part1by1_16(lch_buf[i] >> 16);
                uint32_t right_upper = part1by1_16(rch_buf[i] & 0xFFFF);
                uint32_t right_lower = part1by1_16(rch_buf[i] >> 16);

                i2s_buf[enqueue_pos][j++] = (left_upper << 1) | right_upper;
                i2s_buf[enqueue_pos][j++] = (left_lower << 1) | right_lower;
            }
        }
        else if ((i2s_mode == MODE_PT8211_DUAL || i2s_mode == MODE_I2S_DUAL) && i2s_use_core1 == false){
            //並び替え
            for (int i = 0, j = 0; i < sample / 2; i++) {
                uint32_t left_upper = part1by1_16(lch_buf[i] & 0xFFFF);
                uint32_t left_lower = part1by1_16(lch_buf[i] >> 16);
                uint32_t right_upper = part1by1_16(rch_buf[i] & 0xFFFF);
                uint32_t right_lower = part1by1_16(rch_buf[i] >> 16);

                i2s_buf[enqueue_pos][j++] = (left_upper << 1) | right_upper;
                i2s_buf[enqueue_pos][j++] = (left_lower << 1) | right_lower;

                //反転
                int32_t d_r, d_l;
                if (lch_buf[i] == INT32_MIN){
                    d_l = INT32_MAX;
                }
                else{
                    d_l = -lch_buf[i];
                }
                if (rch_buf[i] == INT32_MIN){
                    d_r = INT32_MAX;
                }
                else{
                    d_r = -rch_buf[i];
                }

                left_upper = part1by1_16(d_l & 0xFFFF);
                left_lower = part1by1_16(d_l >> 16);
                right_upper = part1by1_16(d_r & 0xFFFF);
                right_lower = part1by1_16(d_r >> 16);

                i2s_buf[enqueue_pos][j++] = (left_upper << 1) | right_upper;
                i2s_buf[enqueue_pos][j++] = (left_lower << 1) | right_lower;
            }
            sample *= 2;
        }
        else {
            j = 0;
            for (i = 0; i < sample / 2; i++){
                i2s_buf[enqueue_pos][j++] = lch_buf[i];
                i2s_buf[enqueue_pos][j++] = rch_buf[i];
            }
        }
        
        i2s_sample[enqueue_pos] = sample;
		enqueue_pos++;
		if (enqueue_pos >= I2S_BUF_DEPTH){
            enqueue_pos = 0;
        }
        
        uint32_t save = spin_lock_blocking(queue_spin_lock);
        i2s_buf_length++;
        spin_unlock(queue_spin_lock, save);

		return true;
	}
	else return false;
}

bool i2s_dequeue(int32_t** buff, int* sample){
    if (i2s_get_buf_length()){
        *buff = i2s_buf[dequeue_pos];
        *sample = i2s_sample[dequeue_pos];

        dequeue_pos++;
        if (dequeue_pos >= I2S_BUF_DEPTH){
            dequeue_pos = 0;
        }

        uint32_t save = spin_lock_blocking(queue_spin_lock);
        i2s_buf_length--;
        spin_unlock(queue_spin_lock, save);

        return true;
    }
    else return false;
}

int8_t i2s_get_buf_length(void){
    int8_t d;

    uint32_t save = spin_lock_blocking(queue_spin_lock);
	d = i2s_buf_length;
    spin_unlock(queue_spin_lock, save);

    return d;
}

void i2s_volume_change(int16_t v, int8_t ch){
    if (ch == 0){
        mul_l = db_to_vol[-v >> 8];
        mul_r = db_to_vol[-v >> 8];
    }
    else if (ch == 1){
        mul_l = db_to_vol[-v >> 8];
    }
    else if (ch == 2){
        mul_r = db_to_vol[-v >> 8];
    }
}

void set_playback_handler(ExternalFunction func){
    playback_handler = func;
}

void set_core1_main_function(Core1MainFunction func){
    core1_main_funcion = func;
}
