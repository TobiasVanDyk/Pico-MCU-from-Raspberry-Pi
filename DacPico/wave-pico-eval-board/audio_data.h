/*****************************************************************************
* |	This version:   V1.0
* | Date        :   2021-04-20
* | Info        :   
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/

#ifndef _PICO_AUDIO_DATA_H
#define _PICO_AUDIO_DATA_H

#include "pico/types.h"
#include "hardware/pio.h"

#define PICO_AUDIO_FREQ 40000
#define PICO_AUDIO_COUNT 2
#define PICO_AUDIO_DATA_PIN 3
#define PICO_AUDIO_CLOCK_PIN_BASE 27
#define PICO_AUDIO_PIO 0
#define PICO_AUDIO_SM 0



extern const uint16_t tones[3][8];
extern const uint8_t Happy_birthday[];


extern const int16_t sine_wave_table[];


#endif //_PICO_AUDIO_DATA_H
