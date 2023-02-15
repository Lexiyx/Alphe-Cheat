#include "Animstate.h"
#include "mem_alloc.h"
#include "..//..//Utils/GlobalVars.h"

void c_animations::on_create_move(const bool send_packet)
{
	if (send_packet)
		fake_animation();

	real_animation(send_packet);
}

void c_animations::fake_animation()
{
	if (!g::pLocalEntity || !g::pLocalEntity->IsAlive())
		return;

	if (m_fake_spawntime != g::pLocalEntity->spawn_time() || m_should_update_fake)
	{
		init_fake_anim = false;
		m_fake_spawntime = g::pLocalEntity->spawn_time();
		m_should_update_fake = false;
	}

	if (!init_fake_anim)
	{
		m_fake_state = static_cast<C_AnimState*> (mem_alloc->Alloc(sizeof(C_AnimState)));

		if (m_fake_state != nullptr)
			g::pLocalEntity->create_animation_state(m_fake_state);

		init_fake_anim = true;
	}

	animation_layer_t layer_backup[13];
	float pose_backup[20];

	std::memcpy(layer_backup, g::pLocalEntity->AnimOverlays(), sizeof(layer_backup));
	std::memcpy(pose_backup, g::pLocalEntity->pose_parameter().data(), sizeof(pose_backup));

	const auto ba_curtime = g_pGlobalVars->curtime;

	g_pGlobalVars->curtime = g::pLocalEntity->simulation_time();

	g::pLocalEntity->update_animation_state(m_fake_state, g::pCmd->viewangles);

	m_fake_rotation = reinterpret_cast<c_base_player_anim_state*> (m_fake_state)->m_flGoalFeetYaw;
	g::pLocalEntity->SetAbsAngles(Vector(0, reinterpret_cast<c_base_player_anim_state*> (m_fake_state)->m_flGoalFeetYaw, 0));
	g::should_setup_local_bones = true;
	m_got_fake_matrix = g::pLocalEntity->SetupBones(m_fake_matrix, 128, bone_used_by_anything & ~bone_used_by_attachment,
		g_pGlobalVars->curtime);

	memcpy(m_fake_position_matrix, m_fake_matrix, sizeof(m_fake_position_matrix));

	const auto org_tmp = g::pLocalEntity->GetRenderOrigin();

	if (m_got_fake_matrix)
	{
		for (auto& i : m_fake_matrix)
		{
			i[0][3] -= org_tmp.x;
			i[1][3] -= org_tmp.y;
			i[2][3] -= org_tmp.z;
		}
	}

	g::pLocalEntity->SetAbsAngles(Vector(0, m_server_abs_rotation, 0)); // restore real abs rotation

	std::memcpy(m_fake_layers.data(), g::pLocalEntity->get_anim_overlays(), sizeof(m_fake_layers));
	std::memcpy(m_fake_poses.data(), g::pLocalEntity->pose_parameter().data(), sizeof(m_fake_poses));

	g_pGlobalVars->curtime = ba_curtime;

	std::memcpy(g::pLocalEntity->get_anim_overlays(), layer_backup, sizeof(layer_backup));
	std::memcpy(g::pLocalEntity->pose_parameter().data(), pose_backup, sizeof(pose_backup));
}

void c_animations::real_animation(const bool send_packet)
{
	if (!g::pLocalEntity || !g::pLocalEntity->IsAlive())
		return;

	if (m_real_spawntime != g::pLocalEntity->spawn_time() || m_should_update_real)
	{
		init_real_anim = false;
		m_real_spawntime = g::pLocalEntity->spawn_time();
		m_should_update_real = false;
	}

	if (!init_real_anim)
	{
		m_real_state = static_cast<CCSGOPlayerAnimState*> (mem_alloc->Alloc(sizeof(CCSGOPlayerAnimState)));

		if (m_real_state != nullptr)
			g::pLocalEntity->create_animation_state(m_real_state);

		init_real_anim = true;
	}
	auto anim_state = reinterpret_cast<c_base_player_anim_state*> (m_real_state);
	anim_state->m_iLastClientSideAnimationUpdateFramecount -= 3;
	anim_state->m_flLastClientSideAnimationUpdateTime -= 3;

	g::pLocalEntity->invalidate_bone_cache();

	g::pLocalEntity->update_animation_state(m_real_state, Vector(g::pLocalEntity->eye_angles().x, Globals::pCmd->viewangles.y, 0.f));

	if (send_packet)
	{
		m_server_abs_rotation = reinterpret_cast<c_base_player_anim_state*> (m_real_state)->m_flGoalFeetYaw;
		g::pLocalEntity->SetAbsAngles(Vector(0, reinterpret_cast<c_base_player_anim_state*> (m_real_state)->m_flGoalFeetYaw, 0));
		Globals::should_setup_local_bones = true;
		m_got_real_matrix = g::pLocalEntity->SetupBones(m_real_matrix, maxstudiobones, bone_used_by_anything, 0.f);
		m_current_real_angle = Globals::pCmd->viewangles;
		const auto org_tmp = g::pLocalEntity->GetRenderOrigin();

		if (m_got_real_matrix) // todo maybe not use this and set abs yaw in setup_bones,..
		{
			for (auto& i : m_real_matrix)
			{
				i[0][3] -= org_tmp.x;
				i[1][3] -= org_tmp.y;
				i[2][3] -= org_tmp.z;
			}
		}
	}

	if (m_fake_state && m_real_state)
		m_fake_delta = m_server_abs_rotation - reinterpret_cast<c_base_player_anim_state*> (m_fake_state)->m_flGoalFeetYaw;

	//if ( m_real_state )
	//    console::write_line ( *reinterpret_cast< float* > ( uintptr_t ( m_real_state ) + 0x110 ) );

	std::memcpy(m_real_layers.data(), g::pLocalEntity->get_anim_overlays(), sizeof(m_real_layers));
	std::memcpy(m_real_poses.data(), g::pLocalEntity->pose_parameter().data(), sizeof(m_real_poses));
}

void c_animations::player_animations()
{
	if (!g::pLocalEntity)
		return;

	for (auto idx = 1; idx < g_pGlobalVars->maxClients; idx++)
	{
		auto player = reinterpret_cast<C_BaseEntity*> (g_pEntityList->GetClientEntity(idx));

		auto& data = m_player_states.at(idx);

		if (!player || player->IsDormant() || !player->IsAlive())
		{
			data.m_init = false;
			continue;
		}

		if (player->EntIndex() == g_pEngine->GetLocalPlayer())
			continue;

		if (data.m_spawntime != player->spawn_time() || m_should_update_entity_animstate)
		{
			data.m_init = false;
			data.m_spawntime = player->spawn_time();
		}

		if (!data.m_state)
			data.m_init = false;

		if (!data.m_init)
		{
			data.m_state = static_cast<CCSGOPlayerAnimState*> (mem_alloc->Alloc(sizeof(CCSGOPlayerAnimState)));

			if (data.m_state != nullptr)
				player->create_animation_state(data.m_state);

			data.m_init = true;
		}
		auto anim_state = reinterpret_cast<c_base_player_anim_state*> (data.m_state);
		anim_state->m_iLastClientSideAnimationUpdateFramecount -= 3;
		anim_state->m_flLastClientSideAnimationUpdateTime -= 3;

		if (*reinterpret_cast<float*> (uintptr_t(data.m_state) + 0x110) > 0.8f)
			*reinterpret_cast<float*> (uintptr_t(data.m_state) + 0x110) = 0.f;

		if (data.m_last_simtime == player->simulation_time())
		{
			/*const auto add = g_features.resolver.rdata.at(player->EntIndex()).should_resolve
				? g_features.resolver.rdata.at(player->EntIndex()).desync_add < 0.f
				? -60.f
				: 60.f
				: 0.f;*/
			player->update_animation_state(data.m_state, Vector(player->eye_angles().x,
				player->eye_angles().y + 58.f, 0.f));

			if (*reinterpret_cast<float*> (uintptr_t(data.m_state) + 0x110) > 0.8f)
				*reinterpret_cast<float*> (uintptr_t(data.m_state) + 0x110) = 0.f;
		}
		else
		{
			player->update_animation_state(data.m_state, player->eye_angles());

			if (*reinterpret_cast<float*> (uintptr_t(data.m_state) + 0x110) > 0.8f)
				*reinterpret_cast<float*> (uintptr_t(data.m_state) + 0x110) = 0.f;

			// set var to setup bones and set them up
			player->invalidate_bone_cache();
			player->SetAbsAngles(Vector(0, reinterpret_cast<c_base_player_anim_state*> (data.m_state)->m_flGoalFeetYaw, 0));

			data.m_should_recalculate_matrix = true;

			data.m_got_matrix = player->SetupBones(data.m_matrix, maxstudiobones, bone_used_by_anything, 0.f);

			data.m_should_recalculate_matrix = false;
		}

		data.m_last_simtime = player->simulation_time();
	}

	m_should_update_entity_animstate = false;
}

c_animations Animations;
