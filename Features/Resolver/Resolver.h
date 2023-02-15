#pragma once
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\CGlobalVarsBase.h"
#include "..\..\SDK\IClientMode.h"
#include <deque>

class Resolver
{ /// ???
public:
	bool UseFreestandAngle[65];
	float FreestandAngle[65];
	Vector absOriginBackup;
	float pitchHit[65];

	void lbyResolve(C_BaseEntity* pEnt, C_BaseEntity* pLocalEnt, int i);

	void AnimationFixyes(C_BaseEntity* pEnt);

	void HandleHits(C_BaseEntity* pEnt, C_BaseEntity* pLocalEnt);

	void OnCreateMove();
	void FrameStage(ClientFrameStage_t stage);
private:
	void AnimationFix(C_BaseEntity* pEnt);
	void Skeet(C_BaseEntity* pEnt);
	void MissedShots(C_BaseEntity* pEnt);

public:
	struct record_t {
		//void FrameStage(ClientFrameStage_t stage);
		void OnCreateMove();
		float velocity = 0.f;
		float simtime = 0.f;
		int flags = 0;
		float lowerbody = 0.f;
		float lby180 = 0.f;
		int moving_lby = INT_MAX;
		float at_target_yaw = 0.0f;
		float last_lby = INT_MAX;
		float pre_anim_lby = 0.f;
		float eye_angle = 0.f;
		float bodyeye_delta = 0.f;
		float lbydelta = 0.f;
		int tickcount = 0;
		float lby_comparing_tick = 0.f;
		float delta_comparing_tick = 0.f;
		int last_ammo = 0;

		bool fake_angles = false;
		bool moving_lby_valid = false;
		bool fakewalk = false;
		bool in_air = false;
		bool moving = false;
		bool lby_updated = false;
		bool breaking_lby = false;
		bool breaking_lby_u120 = false;

		int layer_count;
		AnimationLayer layers[13];

		bool adjusting_balance(AnimationLayer* set);
		bool adjusting_stop(AnimationLayer* set);
		bool suppressing_979(AnimationLayer* set);
		bool fakewalk_check;
	};
	struct info_t {

		std::string m_resolver_mode;

		record_t m_current;
		record_t m_previous;

		std::deque< record_t > m_records;

		bool m_initiazlied_previous = false;

		void update();
		void resolve();
		void pitch_resolve();
		float delta(float a, float b);
		bool different(float a, float b, float tolerance);
		int get_different_lbys(float tolerance);
		float get_lby_comparing_ticks();
		bool lby_keeps_changing(float tolerance);
		int get_different_deltas(float tolerance);
		float get_delta_comparing_ticks();
		bool delta_keeps_changing(float tolerance);
		bool has_static_real(float tolerance);
		bool has_static_difference(float tolerance);
		bool has_steady_difference(float tolerance);
		bool in_air[64] = { false };
		bool change[64] = { false };
		bool fakewalk[64] = { false };
		bool wasmoving[64] = { false };
		bool wasfakewalk[64] = { false };
		bool lowdelta[64] = { false };
		bool highdelta[64] = { false };
		bool didsync[64] = { false };
		bool spin[64] = { false };
		bool moving[64] = { false };
		bool hadmovingdif[64] = { false };
		bool hadslowlby[64] = { false };
		bool has_fake979[64] = { false };
		bool was_inair[64] = { false };
		bool did_fakewalk_change[64] = { false };
		bool prebreak[64] = { false };
		bool was_stopping[64] = { false };
		bool did_shoot[64] = { false };
		bool nofake[64] = { false };
		float oldlby[64] = { 0.f };
		float last_sim[64] = { 0.f };
		float last_sync[64] = { 0.f };
		float movinglby[64] = { 0.f };
		float deltaDiff[64] = { 0.f };
		float slowlby[64] = { 0.f };
		float movingdif[64] = { 0.f };
		float fakewalk_change[64] = { 0.f };
		float last_forced_shot[64] = { 0.0f };
		float stand_lby[64] = { 0.0f };
	};
};
extern Resolver g_Resolver;