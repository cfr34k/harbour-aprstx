#pragma once

#include <stdint.h>
#include <time.h>

#define APRS_MAX_FRAME_LEN (1+7+7+8*7+1+1+256+2+1)
#define APRS_MAX_COMMENT_LEN 32

class APRS {
public:

	enum APRS_ICON {
		AI_X = 0,
		AI_BIKE,
		AI_CAR,
		AI_JEEP,
		AI_VAN,
		AI_TRUCK,
		AI_BUS,
		AI_BALLOON,
		AI_RECREATIONAL_VEHICLE,
		AI_HELICOPTER,
		AI_YACHT,
		AI_AMBULANCE,
		AI_FIRE_TRUCK,
		AI_SHIP,

		APRS_NUM_ICONS
	};

private:
	struct APRS_ADDR {
		char call[7]; // 6 characters + \0
		uint8_t ssid;
	};

	struct APRS_ICON_DATA {
		char aprs_char;
	};

	static const struct APRS_ICON_DATA ICON_DATA[APRS_NUM_ICONS];

	float m_lat;
	float m_lon;
	float m_alt_m;
	time_t m_time;

	struct APRS_ADDR m_dest;
	struct APRS_ADDR m_src;

	struct APRS_ADDR m_path[8];
	uint8_t m_npath;

	uint8_t m_type;
	uint8_t m_protocol;
	uint8_t m_info[256];

	enum APRS_ICON m_icon;
	char m_comment[APRS_MAX_COMMENT_LEN+1];


	void import_ascii(uint8_t *out, char *in, size_t n);
	uint16_t calculate_fcs(uint8_t *data, size_t len);
	void append_address(uint8_t **frameptr, struct APRS_ADDR *addr, uint8_t is_last);

	void update_info_field(void);

public:
	APRS();

	void set_dest(const char *dest, uint8_t ssid);
	void set_source(const char *call, uint8_t ssid);

	void clear_path();
	uint8_t add_path(const char *call, uint8_t ssid);

	void set_icon(enum APRS_ICON icon);
	void set_comment(const char *comment);

	void update_pos_time(float lat, float lon, float alt_m, time_t t);

	/*!
	 * Build the APRS frame, excluding the flags (0x7E) in the beginning and end.
	 */
	size_t build_frame(uint8_t *frame);
};
