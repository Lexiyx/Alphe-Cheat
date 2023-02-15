#pragma once
#include "../../VirtualMethod.h"

// deadcell https://github.com/EternityX/DEADCELL-CSGO/blob/05c81db8713da0228181fc2342d543028fab97a1/csgo/sdk/interfaces/def.h

class c_clock_drift_mgr
{
public:
	float m_clock_offsets[17];   //0x0000
	uint32_t m_cur_clock_offset; //0x0044
	uint32_t m_server_tick;     //0x0048
	uint32_t m_client_tick;     //0x004C
}; //Size: 0x0050

class IClientState {
public:
public:
	void force_full_update() {
		*reinterpret_cast<int*>(std::uintptr_t(this) + 0x174) = -1;
		//m_nDeltaTick = -1;
	};
	/*char pad_0000[156];
	NetChannel* m_net_channel;
	uint32_t challenge_count;
	double reconnect_time;
	int32_t retry_count;
	char pad_00A8[88];
	int32_t signon_state_count;
	char pad_0104[8];
	float next_cmd_time;
	int32_t server_count;
	uint32_t current_sequence;
	char pad_0118[8];
	char pad_0120[0x4C];
	int32_t delta_tick;
	bool m_paused;
	char pad_0171[3];
	int32_t view_entity;
	int32_t player_slot;
	char pad_017C[4];
	char level_name[260];
	char level_name_short[40];
	char pad_02AC[92];
	int32_t max_clients;
	char pad_030C[4083];
	uint32_t string_table_container;
	char pad_1303[14737];
	float last_server_tick_time;
	bool is_in_simulation;
	char pad_4C99[3];
	uint32_t old_tick_count;
	float tick_remainder;
	float frame_time;
	int32_t m_last_outgoing_command;
	int32_t m_choked_commands;
	int32_t last_command_ack;
	int32_t command_ack;
	int32_t sound_sequence;
	char pad_4CBC[80];
	Vector viewangles;*/

	std::byte        pad0[0x9C];                //0x0000
	NetChannel* m_net_channel;            //0x009C
	int                challenge_count;            //0x00A0
	std::byte        pad1[0x64];                //0x00A4
	int                iSignonState;            //0x0108
	std::byte        pad2[0x8];                //0x010C
	float            next_cmd_time;            //0x0114
	int                server_count;            //0x0118
	int                current_sequence;        //0x011C
	char _0x0120[4];
	__int32 m_iClockDriftMgr; //0x0124 
	char _0x0128[68];
	__int32 m_iServerTick; //0x016C 
	__int32 m_iClientTick; //0x0170 
	int                delta_tick;                //0x0174
	bool            m_paused;                //0x0178
	std::byte        pad4[0x7];                //0x0179
	int                view_entity;            //0x0180
	int                player_slot;            //0x0184
	char            level_name[260];        //0x0188
	char            level_name_short[80];    //0x028C
	char            szGroupName[80];        //0x02DC
	std::byte        pad5[0x5C];                //0x032C
	int                max_clients;            //0x0388
	std::byte        pad6[0x4984];            //0x038C
	float            last_server_tick_time;    //0x4D10
	bool            is_in_simulation;            //0x4D14
	std::byte        pad7[0xB];                //0x4D15
	int                old_tick_count;            //0x4D18
	float            tick_remainder;        //0x4D1C
	float            frame_time;            //0x4D20
	int                m_last_outgoing_command;    //0x4D38
	int                m_choked_commands;        //0x4D30
	int                last_command_ack;        //0x4D2C
	int                command_ack;            //0x4D30
	int                sound_sequence;            //0x4D34
	std::byte        pad8[0x50];                //0x4D38
	Vector            viewangles;            //0x4D88
	std::byte        pad9[0xD0];                //0x4D9A
};

extern IClientState* g_pClientState;