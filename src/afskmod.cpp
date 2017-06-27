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

#include <math.h>

#include "afskmod.h"

#define PI 3.141592654f

AFSKMod::AFSKMod(uint32_t samp_rate)
{
	float omega;

	m_samp_rate = samp_rate;
	m_cnt_ones = 0;

	m_samples_per_bit = samp_rate / AFSK_BAUD_RATE;

	omega = 2*PI*AFSK_F0/samp_rate;

	m_twiddle_f0_re = cosf(omega);
	m_twiddle_f0_im = sinf(omega);

	omega = 2*PI*AFSK_F1/samp_rate;

	m_twiddle_f1_re = cosf(omega);
	m_twiddle_f1_im = sinf(omega);

	m_signal_re = 0;
	m_signal_im = 0.8;
}

uint32_t AFSKMod::mod_bit(uint8_t bit, int16_t *samples)
{
	float twiddle_re, twiddle_im;
	float new_re, new_im;

	// NRZI encoding
	if(bit == 0) {
		// toggle state for each 0 bit
		m_last_state = !m_last_state;
	} else {
		// no change
	}

	if(m_last_state == 0) {
		twiddle_re = m_twiddle_f0_re;
		twiddle_im = m_twiddle_f0_im;
	} else {
		twiddle_re = m_twiddle_f1_re;
		twiddle_im = m_twiddle_f1_im;
	}

	for(uint32_t i = 0; i < m_samples_per_bit; i++) {
		// y[k] = y[k-1] * exp(j*omega)

		new_re = twiddle_re * m_signal_re - twiddle_im * m_signal_im;
		new_im = twiddle_re * m_signal_im + twiddle_im * m_signal_re;

		m_signal_re = new_re;
		m_signal_im = new_im;

		*samples = (uint16_t)(32767 * new_re);
		samples++;
	}

	return m_samples_per_bit;
}

uint32_t AFSKMod::mod_byte(uint8_t byte, int16_t *samples, uint8_t enable_bitstuffing)
{
	uint32_t samples_generated = 0;

	for(uint32_t i = 0; i < 8; i++) {
		uint8_t bit = byte & (1 << i);

		samples_generated += AFSKMod::mod_bit(bit, samples + samples_generated);

		if(enable_bitstuffing) {
			if(bit != 0) {
				m_cnt_ones++;

				// insert a 0 after 5 continuous 1s
				if(m_cnt_ones == 5) {
					samples_generated += AFSKMod::mod_bit(0, samples + samples_generated);
					m_cnt_ones = 0;
				}
			}
		}

		if(bit == 0) {
			m_cnt_ones = 0;
		}
	}

	return samples_generated;
}

uint32_t AFSKMod::gen_vox_tone(uint32_t duration_ms, int16_t *samples)
{
	//float new_re, new_im;
	uint32_t nsamples = duration_ms * m_samp_rate / 1000;
	uint32_t samples_generated = 0;

	// send flags until the given number of milliseconds/samples is reached
	while(samples_generated < nsamples) {
		samples_generated += AFSKMod::mod_byte(0x7E, samples + samples_generated, 0);
	}

	m_cnt_ones = 0;

	return samples_generated;
}
