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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <locale.h>

#include "aprs.h"

const struct APRS::APRS_ICON_DATA APRS::ICON_DATA[APRS_NUM_ICONS] = {
	{ '.' }, // AI_X
	{ '[' }, // AI_JOGGER
	{ 'b' }, // AI_BIKE
	{ '>' }, // AI_CAR
	{ 'j' }, // AI_JEEP
	{ 'v' }, // AI_VAN
	{ 'k' }, // AI_TRUCK
	{ 'U' }, // AI_BUS
	{ 'O' }, // AI_BALLOON
	{ 'R' }, // AI_RECREATIONAL_VEHICLE
	{ 'X' }, // AI_HELICOPTER
	{ 'Y' }, // AI_YACHT
	{ 'a' }, // AI_AMBULANCE
	{ 'f' }, // AI_FIRE_TRUCK
	{ 's' }, // AI_SHIP
};

// PRIVATE FUNCTIONS

void APRS::import_ascii(uint8_t *out, char *in, size_t n)
{
	for(size_t i = 0; i < n; i++) {
		uint8_t tmp = (uint8_t)in[i];
		out[i] = tmp << 1;
	}
}

/*
 * http://n1vg.net/packet/
 *
 * "Start with the 16-bit FCS set to 0xffff. For each data bit sent, shift the
 * FCS value right one bit. If the bit that was shifted off (formerly bit 1)
 * was not equal to the bit being sent, exclusive-OR the FCS value with 0x8408.
 * After the last data bit, take the ones complement (inverse) of the FCS value
 * and send it low-byte first."
 */
uint16_t APRS::calculate_fcs(uint8_t *data, size_t len)
{
	const uint16_t poly = 0x8408;

	uint16_t lfsr = 0xFFFF;

	for(size_t i = 0; i < len; i++) {
		for(int j = 0; j < 8; j++) {
			uint8_t b = (data[i] >> j) & 0x01;
			uint8_t o = lfsr & 0x0001;

			lfsr >>= 1;

			if(b != o) {
				lfsr ^= poly;
			}
		}
	}

	return ~lfsr; // invert all bits in the end
}

void APRS::append_address(uint8_t **frameptr, struct APRS_ADDR *addr, uint8_t is_last)
{
	size_t len = strlen(addr->call);

	// convert the data to the AX.25 representation
	APRS::import_ascii(*frameptr, addr->call, len);

	// pad to 6 bytes
	for(size_t i = len; i < 6; i++) {
		(*frameptr)[i] = ((uint8_t)' ') << 1;
	}

	// the 7th byte is special
	(*frameptr)[6] = 0x60 | ((addr->ssid << 1) & 0x1E);

	if(is_last) {
		(*frameptr)[6] |= 0x01;
	}

	*frameptr += 7;
}

void APRS::update_info_field(void)
{
	float lat = m_lat;
	float lon = m_lon;

	char lat_ns, lon_ew;
	int lat_deg, lon_deg;
	float lat_min, lon_min;

	float alt_ft;

	struct tm tms;

	// convert sign -> north/south, east/west
	if(lat < 0) {
		lat = -lat;
		lat_ns = 'S';
	} else {
		lat_ns = 'N';
	}

	if(lon < 0) {
		lon = -lon;
		lon_ew = 'W';
	} else {
		lon_ew = 'E';
	}

	// calculate integer degrees
	lat_deg = (int)lat;
	lon_deg = (int)lon;

	// calculate fractional arc minutes
	lat_min = ((lat - lat_deg) * 60);
	lon_min = ((lon - lon_deg) * 60);

	// convert meters to feet
	alt_ft = m_alt_m / 0.3048;

	// generate time string from timestamp
	gmtime_r(&m_time, &tms);

	// make sure decimals are separated by '.'
	setlocale(LC_ALL, "C");

	/* Time as ddhhmm
	sprintf((char*)m_info, "/%02i%02i%02iz%02i%05.2f%c/%03i%05.2f%c%c%s /a=%06i",
			tms.tm_mday, tms.tm_hour, tms.tm_min,
			lat_deg, lat_min, lat_ns,
			lon_deg, lon_min, lon_ew,
			ICON_DATA[m_icon].aprs_char, m_comment,
			(int)alt_ft);
	*/
	// time as hhmmss
	sprintf((char*)m_info, "/%02i%02i%02ih%02i%05.2f%c/%03i%05.2f%c%c%s /a=%06i",
			tms.tm_hour, tms.tm_min, tms.tm_sec,
			lat_deg, lat_min, lat_ns,
			lon_deg, lon_min, lon_ew,
			ICON_DATA[m_icon].aprs_char, m_comment,
			(int)alt_ft);

	// back to internationality
	setlocale(LC_ALL, "");
}

// PUBLIC FUNCTIONS

APRS::APRS()
{
	m_type     = 0x03; // UI frame
	m_protocol = 0xF0; // no level 3 protocol

	m_dest.call[6] = '\0';
	m_src.call[6] = '\0';

	for(uint8_t i = 0; i < 8; i++) {
		m_path[i].call[6] = '\0';
	}

	m_npath = 0;

	m_icon = AI_X;

	m_comment[0] = '\0';
	m_comment[APRS_MAX_COMMENT_LEN] = '\0';
}

void APRS::set_dest(const char *dest, uint8_t ssid)
{
	strncpy(m_dest.call, dest, 6);
	m_dest.ssid = ssid;
}

void APRS::set_source(const char *call, uint8_t ssid)
{
	strncpy(m_src.call, call, 6);
	m_src.ssid = ssid;
}

void APRS::clear_path()
{
	m_npath = 0;
}

uint8_t APRS::add_path(const char *call, uint8_t ssid)
{
	if(m_npath == 8) {
		return 0;
	} else {
		strncpy(m_path[m_npath].call, call, 6);
		m_path[m_npath].ssid = ssid;

		m_npath++;

		return 1;
	}
}

void APRS::update_pos_time(float lat, float lon, float alt_m, time_t t)
{
	m_lat = lat;
	m_lon = lon;
	m_alt_m = alt_m;
	m_time = t;
}

void APRS::set_icon(enum APRS_ICON icon)
{
	m_icon = icon;
}

void APRS::set_comment(const char *comment)
{
	strncpy(m_comment, comment, APRS_MAX_COMMENT_LEN);
}

size_t APRS::build_frame(uint8_t *frame)
{
	uint8_t *frameptr = frame;
	uint8_t *infoptr = m_info;
	uint16_t fcs;

	APRS::append_address(&frameptr, &(m_dest), 0);
	APRS::append_address(&frameptr, &(m_src), (m_npath == 0) ? 1 : 0);

	for(uint8_t i = 0; i < m_npath; i++) {
		APRS::append_address(&frameptr, &(m_path[i]), (m_npath == (i+1)) ? 1 : 0);
	}

	*(frameptr++) = m_type;
	*(frameptr++) = m_protocol;

	update_info_field();

	while(*infoptr != '\0') {
		*frameptr = *infoptr;

		frameptr++;
		infoptr++;
	}

	fcs = APRS::calculate_fcs(frame, (frameptr-frame));

	//*(frameptr++) = bit_reverse(fcs >> 8);
	//*(frameptr++) = bit_reverse(fcs & 0xFF);
	*(frameptr++) = fcs & 0xFF;
	*(frameptr++) = fcs >> 8;

	*frameptr = '\0';

	return (size_t)(frameptr - frame);
}
