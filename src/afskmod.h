/*
	This file is part of APRSTX - An APRS transmitter for SailfishOS
	Copyright (C) 2017  Thomas Kolb

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <stdint.h>

#define APRS_MAX_FRAME_LEN (1+7+7+8*7+1+1+256+2+1)
#define AFSK_BAUD_RATE 1200

#define AFSK_F0 1200
#define AFSK_F1 2200

class AFSKMod {
private:
	uint32_t m_samp_rate;
	uint32_t m_cnt_ones;

	uint32_t m_samples_per_bit;

	uint8_t m_last_state;

	float m_twiddle_f0_re;
	float m_twiddle_f0_im;

	float m_twiddle_f1_re;
	float m_twiddle_f1_im;

	float m_signal_re;
	float m_signal_im;

	uint32_t mod_bit(uint8_t bit, int16_t *samples);

public:
	AFSKMod(uint32_t samp_rate);

	uint32_t mod_byte(uint8_t byte, int16_t *samples, uint8_t enable_bitstuffing);
	uint32_t gen_vox_tone(uint32_t duration_ms, int16_t *samples);
};
