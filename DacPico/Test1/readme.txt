Use the the files from my_pico_audio and my_pico_audio_i2s (https://github.com/elehobica/pico_sine_wave_i2s_32b) instead of the raspberry pi /pico/pico-extras/src/common and /pico/pico-extras/src/rp2_common.

Then also use the files included here:  usb_sound_card.c in /pico/pico-playground/apps/usb_sound_card, and the edited audio_i2s.h
Compile as usual, the build file test1.uf2 is included here - make sure to set the sound config as 16 bit, 44.1kHz.

