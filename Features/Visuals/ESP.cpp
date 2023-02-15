#include "ESP.h"
#include "..\..\Utils\GlobalVars.h"
#include "..\Aimbot\Aimbot.h"
#include "..\Aimbot\LagComp.h"
#include "..\..\Utils\Utils.h"
#include "..\..\SDK\IVEngineClient.h"
#include "..\..\SDK\PlayerInfo.h"
#include "..\..\SDK\ISurface.h"
#include "..\..\SDK\Hitboxes.h"
#include "..\..\Utils\Math.h"
#include "..\..\Menu\Menu.h"
#include "..\Misc\Misc.h"
#include "..\..\Utils\Interfaces.h"
#include <stdio.h>
#include <ctype.h>
#include <comdef.h>
#include "../../SDK/vector2d.h"
#include <windows.h>
#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <array>
#include <fstream>
#include <istream>
#include <unordered_map>
#include <intrin.h>

ESP g_ESP;
bool WorldToScreen(const Vector& world, Vector& screen) {
	auto screen_transform = [&world, &screen]() -> bool {
		static auto& matrix = g_pEngine->WorldToScreenMatrix();

		screen.x = matrix[0][0] * world.x + matrix[0][1] * world.y + matrix[0][2] * world.z + matrix[0][3];
		screen.y = matrix[1][0] * world.x + matrix[1][1] * world.y + matrix[1][2] * world.z + matrix[1][3];
		screen.z = 0.0f;

		float w = matrix[3][0] * world.x + matrix[3][1] * world.y + matrix[3][2] * world.z + matrix[3][3];

		if (w < 0.001f) {
			screen.x *= 100000;
			screen.y *= 100000;
			return false;
		}

		screen.x /= w;
		screen.y /= w;

		return true;
	};

	if (screen_transform()) {
		int w, h;
		g_pEngine->GetScreenSize(w, h);

		screen.x = (w / 2.0f) + (screen.x * w) / 2.0f;
		screen.y = (h / 2.0f) - (screen.y * h) / 2.0f;

		return true;
	}

	return false;
}
float flPlayerAlpha[1024];
float StoredCurtimePlayer[1024];

void ESP::RenderBox(C_BaseEntity* pEnt)
{
	g_pSurface->OutlinedRect(Box.left + 2, Box.top + 2, Box.right - 4, Box.bottom - 4, shadow);
	g_pSurface->OutlinedRect(Box.left - 2, Box.top - 2, Box.right + 4, Box.bottom + 4, shadow);

	g_pSurface->OutlinedRect(Box.left, Box.top, Box.right, Box.bottom, color);
}

void ESP::QuakeKillCounter()
{
	if (!g_Menu.Config.KillCounter)
		return;

	int screen_sizex, screen_sizey, cW, cH;
	g_pEngine->GetScreenSize(screen_sizex, screen_sizey);

	cW = screen_sizex / 2;
	cH = screen_sizey / 2;

	float animationAlpha = 0.0;
	int alpha = 0;
	float animation_speed_value = 0.6;
	float step = 255 / 0.6 * g_pGlobalVars->frametime;

	if (g::killtime + 0.4 > g_pGlobalVars->realtime)
	{
		alpha = 255;
	}
	else
		alpha = alpha - step;

	float animate = g_pGlobalVars->curtime;

	float endTime = g::globalAlpha + 1.1f;
	float t = 1.0f - (endTime - g_pGlobalVars->curtime) / (endTime - g::globalAlpha);


	float animationStep = animation_speed_value * g_pGlobalVars->frametime;

	if (g::killtime + 0.1 > g_pGlobalVars->realtime)
	{
		animationAlpha -= 80;
	}
	else
		animationAlpha = animationAlpha - animationStep;

	int customY, customX;

	screen_sizey = (screen_sizey / 2) - (t * 10.f);

	if (alpha > 0)
	{

		switch (g::kills)
		{
		case 1:
			g_pSurface->DrawT(screen_sizex / 2, screen_sizey, Color(255, 0, 0, alpha), g::KillCounter, true, ("FIRST BLOOD"));
			return;
			break;
		case 2:
			g_pSurface->DrawT(screen_sizex / 2, screen_sizey, Color(0, 0, 255, alpha), g::KillCounter, true, ("DOUBLE KILL"));
			return;
			break;
		case 3:
			g_pSurface->DrawT(screen_sizex / 2, screen_sizey, Color(0, 255, 0, alpha), g::KillCounter, true, ("TRIPLE KILL"));
			return;
			break;
		case 4:
			g_pSurface->DrawT(screen_sizex / 2, screen_sizey, Color(155, 200, 0, alpha), g::KillCounter, true, ("MULTI KILL"));
			return;
			break;
		case 5:
			g_pSurface->DrawT(screen_sizex / 2, screen_sizey, Color(200, 0, 167, alpha), g::KillCounter, true, ("ULTRA KILL"));
			return;
			break;
		case 6:
			g_pSurface->DrawT(screen_sizex / 2, screen_sizey, Color(23, 34, 255, alpha), g::KillCounter, true, ("KILLING SPREE"));
			return;
			break;
		default:
			g_pSurface->DrawT(screen_sizex / 2, screen_sizey, Color(23, 34, 255, alpha), g::KillCounter, true, ("HUMILIATION"));
			return;
			break;
		}

	}


}
void Draw_Line(int x1, int y1, int x2, int y2, Color color)
{
	g_pSurface->DrawSetColor(color.red, color.green, color.blue, color.alpha);
	g_pSurface->DrawLine(x1, y1, x2, y2);
}
void ESP::DrawZeusRange() {
	auto local_player = g::pLocalEntity;
	if (!local_player) return;

	if (!g_Menu.Config.zeusrange)
		return;

	if (!local_player->IsAlive())
		return;

	auto weapon = local_player->GetActiveWeapon();

	if (weapon->GetItemDefinitionIndex() != ItemDefinitionIndex::WEAPON_TASER)
		return;

	float step = M_PI * 2.0 / 1023;
	float rad = 130.f;
	Vector origin = local_player->GetEyePosition();

	static double rainbow;

	Vector screenPos;
	static Vector prevScreenPos;

	int screen_sizex, screen_sizey, halfY;
	g_pEngine->GetScreenSize(screen_sizex, screen_sizey);
	halfY = (screen_sizey / screen_sizey);
	for (float rotation = 0; rotation < (M_PI * 2.0); rotation += step)
	{
		Vector pos(rad * cos(rotation) + origin.x, rad * sin(rotation) + origin.y, origin.z);

		CTraceEntity filter(local_player);
		C_Trace trace;
		filter.pSkip1 = local_player;


		g_pTrace->TraceRay(C_Ray(origin, pos), MASK_SHOT_BRUSHONLY, &filter, &trace);

		if (g_pDebugOverlay->ScreenPosition(trace.end, screenPos))
			continue;

		if (!prevScreenPos.IsZero() && !screenPos.IsZero() && screenPos.DistTo(Vector(-107374176, -107374176, -107374176)) > 3.f && prevScreenPos.DistTo(Vector(-107374176, -107374176, -107374176)) > 3.f) {
			rainbow += 0.00001;
			if (rainbow > 1.f)
				rainbow = 0;

			Color color = Color::FromHSB(rainbow, 1.f, 1.f);
			Draw_Line(prevScreenPos.x, prevScreenPos.y - -halfY, screenPos.x, screenPos.y, color);
		}
		prevScreenPos = screenPos;
	}
}
void rotate(std::array< vec2_t, 3 >& points, float rotation)
{
	const auto points_center = (points.at(0) + points.at(1) + points.at(2)) / 3;
	for (auto& point : points) {
		point -= points_center;

		const auto temp_x = point.x;
		const auto temp_y = point.y;

		const auto theta = DEG2RAD(rotation);
		const auto c = cos(theta);
		const auto s = sin(theta);

		point.x = temp_x * c - temp_y * s;
		point.y = temp_x * s + temp_y * c;

		point += points_center;
	}
}
int arrow_alpha[65];
mstudiobbox_t* get_hitbox(C_BaseEntity* entity, int hitbox_index)
{
	if (entity->IsDormant() || entity->GetHealth() <= 0)
		return NULL;

	const auto pModel = entity->GetModel();
	if (!pModel)
		return NULL;

	auto pStudioHdr = g_pModelInfo->GetStudiomodel(pModel);
	if (!pStudioHdr)
		return NULL;

	auto pSet = pStudioHdr->GetHitboxSet(0);
	if (!pSet)
		return NULL;

	if (hitbox_index >= pSet->numhitboxes || hitbox_index < 0)
		return NULL;

	return pSet->GetHitbox(hitbox_index);
}
Vector GetHitboxPosition_S(C_BaseEntity* entity, int hitbox_id)
{
	auto hitbox = get_hitbox(entity, hitbox_id);
	if (!hitbox)
		return Vector(0, 0, 0);

	auto bone_matrix = entity->GetBoneMatrix(hitbox->bone);

	Vector bbmin, bbmax;
	g_Math.VectorTransform(hitbox->min, bone_matrix, bbmin);
	g_Math.VectorTransform(hitbox->max, bone_matrix, bbmax);

	return (bbmin + bbmax) * 0.5f;
}
void  arrows(C_BaseEntity* player) {
	auto local = g::pLocalEntity;

	// another case of unnecessary float divison. gotta watch out for that guys! ~chance
//	const int fade = ( int )( ( ( 1 / 0.25f ) * interfaces::globals->frame_time ) * 255 );
	const int fade = (int)((4 * g_pGlobalVars->frametime) * 255);



	//	if (!elements::get().vis_arrows)
	//		return;

	if (!local)
		return;

	if (!local->IsAlive())
		return;

	if (!player)
		return;

	if (player->IsDormant())
		return;

	arrow_alpha[player->EntIndex()] = 180;
	Vector position;
	if (WorldToScreen(GetHitboxPosition_S(player, 2), position) && !g_Menu.Config.AlwaysIndicate)
		return;

	Vector viewangles;
	int width, height;

	g_pEngine->GetViewAngles(viewangles);
	g_pEngine->GetScreenSize(width, height);

	// more float division, i'm just gonna stop putting comments next to it at this point xD. ~chance
//	const auto screen_center = vec2_t( width / 2.f, height / 2.f );
	const auto screen_center = vec2_t(width * .5f, height * .5f);

	const auto angle_yaw_rad = DEG2RAD(viewangles.y - g_Math.CalcAngle(local->GetEyePosition(), GetHitboxPosition_S(player, 2)).y - 90);

	int radius = g_Menu.Config.FovSize;
	int size = g_Menu.Config.FovDistance;

	const auto new_point_x = screen_center.x + ((((width - (size * 3)) * .5f) * (radius / 100.0f)) * cos(angle_yaw_rad)) + (int)(6.0f * (((float)size - 4.f) / 16.0f));
	const auto new_point_y = screen_center.y + ((((height - (size * 3)) * .5f) * (radius / 100.0f)) * sin(angle_yaw_rad));

	Color ESP = player->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[player->EntIndex()]) : Color(g_Menu.Config.FovColor.red, g_Menu.Config.FovColor.green, g_Menu.Config.FovColor.blue, flPlayerAlpha[player->EntIndex()]);

	Color clr = ESP;

	std::array< vec2_t, 3 >points{ vec2_t(new_point_x - size, new_point_y - size),
		vec2_t(new_point_x + size, new_point_y),
		vec2_t(new_point_x - size, new_point_y + size) };

	rotate(points, viewangles.y - g_Math.CalcAngle(local->GetEyePosition(), GetHitboxPosition_S(player, 2)).y - 90);
	g_pSurface->draw_filled_triangle(points, clr);
}
void ESP::RenderName(C_BaseEntity* pEnt, int iterator)
{
	PlayerInfo_t pInfo;
	g_pEngine->GetPlayerInfo(iterator, &pInfo);

	wchar_t buffer[36];

	if (MultiByteToWideChar(CP_UTF8, 0, pInfo.szName, -1, buffer, 36) > 0)
	{
		_bstr_t b(buffer);
		char* name = b;
		if (g_Menu.Config.Name == 1)
		{
			g_pSurface->DrawT(Box.left, Box.top - 12, namecolor, g::EspSmallText, false, name);
		}
		else if (g_Menu.Config.Name == 2)
		{
			g_pSurface->DrawT(Box.left + (Box.right / 2), Box.top - 12, namecolor, g::EspSmallText, true, name);
		}
	}
}

void ESP::RenderWeaponName(C_BaseEntity* pEnt)
{
	auto weapon = pEnt->GetActiveWeapon();

	if (!weapon)
		return;

	auto strWeaponName = weapon->GetName();

	std::string icon_letter = "";

	switch (reinterpret_cast<C_BaseCombatWeapon*>(weapon)->GetItemDefinitionIndex())
	{
	case ItemDefinitionIndex::WEAPON_DEAGLE:
		icon_letter = "A";
		break;
	case ItemDefinitionIndex::WEAPON_KNIFE:
	case ItemDefinitionIndex::WEAPON_KNIFE_T:
		icon_letter = "2";
		break;
	case ItemDefinitionIndex::WEAPON_AUG:
		icon_letter = "U";
		break;
	case ItemDefinitionIndex::WEAPON_G3SG1:
		icon_letter = "X";
		break;
	case ItemDefinitionIndex::WEAPON_MAC10:
		icon_letter = "K";
		break;
	case ItemDefinitionIndex::WEAPON_P90:
		icon_letter = "P";
		break;
	case ItemDefinitionIndex::WEAPON_SSG08:
		icon_letter = "a";
		break;
	case ItemDefinitionIndex::WEAPON_SCAR20:
		icon_letter = "Y";
		break;
	case ItemDefinitionIndex::WEAPON_UMP45:
		icon_letter = "L";
		break;
	case ItemDefinitionIndex::WEAPON_ELITE:
		icon_letter = "B";
		break;
	case ItemDefinitionIndex::WEAPON_FAMAS:
		icon_letter = "R";
		break;
	case ItemDefinitionIndex::WEAPON_FIVESEVEN:
		icon_letter = "C";
		break;
	case ItemDefinitionIndex::WEAPON_GALILAR:
		icon_letter = "Q";
		break;
	case ItemDefinitionIndex::WEAPON_M4A1:
		icon_letter = "S";
		break;
	case ItemDefinitionIndex::WEAPON_P250:
		icon_letter = "F";
		break;
	case ItemDefinitionIndex::WEAPON_M249:
		icon_letter = "g";
		break;
	case ItemDefinitionIndex::WEAPON_NEGEV:
		icon_letter = "f";
		break;
	case ItemDefinitionIndex::WEAPON_XM1014:
		icon_letter = "S";
		break;
	case ItemDefinitionIndex::WEAPON_C4:
		icon_letter = "C4";
		break;
	case ItemDefinitionIndex::WEAPON_GLOCK:
		icon_letter = "D";
		break;
	case ItemDefinitionIndex::WEAPON_HKP2000:
		icon_letter = "E";
		break;
	case ItemDefinitionIndex::WEAPON_AWP:
		icon_letter = "Z";
		break;
	default:
		break;
	}

	int maxclip = weapon->GetCSWpnData()->iMaxClip1;
	int current = weapon->GetAmmo();

	strWeaponName.erase(0, 7);
	if (g_Menu.Config.Weapon == 1)
	{
		if (!pEnt->IsKnifeorNade())
			strWeaponName += " " + std::to_string(current) + " / " + std::to_string(maxclip);
		g_pSurface->DrawT((Box.left + Box.right) + 1, Box.top + offset, weaponcolor, g::EspSmallText, false, strWeaponName.c_str());
		offset += 10;
	}
	else if (g_Menu.Config.Weapon == 2)
	{
		std::string cliptext = std::to_string(current) + " / " + std::to_string(maxclip);
		g_pSurface->DrawT(Box.left + (Box.right / 2), (Box.top + Box.bottom) + 1, weaponcolor, g::WeaponIcons, true, icon_letter.c_str());
		if (!pEnt->IsKnifeorNade())
			g_pSurface->DrawT(Box.left + (Box.right / 2), (Box.top + Box.bottom) + 26, weaponcolor, g::EspSmallText, true, cliptext.c_str());
	}
}

bool DisplayAmmo[65];
void ESP::Ammo(C_BaseEntity* Entity) {

	Vector min, max;
	Entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = Entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);
	//spectators();
																										//this is the ammo color
	Color ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(g_Menu.Config.AmmoEspColor.red, g_Menu.Config.AmmoEspColor.green, g_Menu.Config.AmmoEspColor.blue, flPlayerAlpha[Entity->EntIndex()]);
	Color ESP2 = Entity->IsDormant() ? Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]) : Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]);

	auto Weapon = Entity->GetActiveWeapon();
	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top) && Weapon)
	{
		int height = (pos.y - top.y);;
		int y = top.y;
		int width2 = height / 2;
		int x = pos.x - (width2 / 2);

		float offset = (height / 4.f) + 5;
		UINT hp = height - (UINT)((height * 3) / 100);

		auto animLayer = Entity->GetAnimOverlay4(1);

		if (!animLayer)
			return;

		auto activity = Entity->GetSequenceActivity(animLayer->m_nSequence);

		if (!activity)
			return;

		int iClip = Weapon->GetAmmo();
		int iClipMax = Weapon->GetCSWpnData()->iMaxClip1;

		if (!animLayer || !activity) {
			DisplayAmmo[Entity->EntIndex()] = false;
			return;
		}


		float box_w = (float)fabs(height / 2);
		float width;

		DisplayAmmo[Entity->EntIndex()] = iClip > 0 || (activity == 967 && animLayer->m_flWeight != 0.f);

		if (activity == 967 && animLayer->m_flWeight != 0.f)
		{
			float cycle = animLayer->m_flCycle;
			width = (((box_w * cycle) / 1.f));
		}
		else
			width = (((box_w * iClip) / iClipMax));

		if (!DisplayAmmo[Entity->EntIndex()])
			return;

		g_pSurface->FilledRect(Box.left, y + height + 2, box_w, 4, ESP2);
		g_pSurface->FilledRect(Box.left, y + height + 2, width, 4, Color(ESP));
		g_pSurface->OutlinedRect(x, y + height + 2, box_w, 4, ESP2);
	}
}

void ESP::RenderEntHealth(C_BaseEntity* pEnt)
{
	if (g_Menu.Config.HealthBar != 0)
	{
		int pixelValue = pEnt->GetHealth() * Box.bottom / 100;
		int speed = pixelValue - g_pGlobalVars->frametime;
		if (g_Menu.Config.HealthStyle == 0)
		{
			g_pSurface->FilledRect(Box.left - 10, Box.top - 2, 6, Box.bottom + 4, shadow);
			g_pSurface->FilledRect(Box.left - 8, Box.top + Box.bottom - pixelValue, 2, pixelValue, hbcolor);
		}
		else
			g_pSurface->FilledRect(Box.left - 8, Box.top + Box.bottom - pixelValue, 2, (pixelValue - speed) * 8, hbcolor);
	}

	if (g_Menu.Config.Health)
	{
		std::string ent_hp = ("HP: ") + std::to_string(pEnt->GetHealth());
		g_pSurface->DrawT((Box.left + Box.right) + 1, Box.top + offset, hcolor, g::EspSmallText, false, ent_hp.c_str());
		offset += 10;
	}
}

void ESP::RenderYawPitch(C_BaseEntity* pEnt)
{
	std::string ent_yaw = ("Yaw: ") + std::to_string(pEnt->GetEyeAngles().y);
	std::string ent_pitch = ("Pitch: ") + std::to_string(pEnt->GetEyeAngles().x);

	g_pSurface->DrawT((Box.left + Box.right) + 1, Box.top + offset, ypcolor, g::EspSmallText, false, ent_pitch.c_str());
	offset += 10;
	g_pSurface->DrawT((Box.left + Box.right) + 1, Box.top + offset, ypcolor, g::EspSmallText, false, ent_yaw.c_str());
	offset += 10;
}

void ESP::RenderMoney(C_BaseEntity* pEnt)
{
	std::string ent_money = ("$") + std::to_string(pEnt->GetMoney());
	g_pSurface->DrawT((Box.left + Box.right) + 1, Box.top + offset, moneycolor, g::EspSmallText, false, ent_money.c_str());
	offset += 10;
}
void ESP::BoundBox(C_BaseEntity* pEnt)
{
	Box.bottom = 0;
	Box.top = 0;
	Box.left = 0;
	Box.right = 0;

	Vector2D w2sBottom, w2sTop;

	Utils::WorldToScreen(pEnt->GetOrigin() - Vector(0, 0, 10), w2sBottom);
	Utils::WorldToScreen(pEnt->GetHitboxPosition(0, g_Aimbot.Matrix[pEnt->EntIndex()]) + Vector(0, 0, 10), w2sTop);

	int Middle = w2sBottom.y - w2sTop.y;
	int Width = Middle / 4.f;

	Box.bottom = Middle;
	Box.top = w2sTop.y;
	Box.left = w2sBottom.x - Width;
	Box.right = Width * 2;
}

void ESP::set_hitmarker_time(float time)
{
	g::flHurtTime = time;
}

void ESP::DrawHitmarker()
{

	if (!g::pLocalEntity)
		return;

	if (!g::pLocalEntity->IsAlive())
		return;

	static int lineSize = 4.25;

	static float alpha = 0;
	float step = 255.f / 0.3f * g_pGlobalVars->frametime;

	if (g_pGlobalVars->realtime - g::flHurtTime < .3f)
		alpha = 255.f;
	else
		alpha -= step;

	if (alpha > 0)
	{
		int screenSizeX, screenCenterX;
		int screenSizeY, screenCenterY;
		g_pEngine->GetScreenSize(screenSizeX, screenSizeY);

		screenCenterX = screenSizeX / 2;
		screenCenterY = screenSizeY / 2;
		Color col = Color(240, 240, 240, alpha);
		g_pSurface->Line(screenCenterX - lineSize * 2, screenCenterY - lineSize * 2, screenCenterX - (lineSize), screenCenterY - (lineSize), col);
		g_pSurface->Line(screenCenterX - lineSize * 2, screenCenterY + lineSize * 2, screenCenterX - (lineSize), screenCenterY + (lineSize), col);
		g_pSurface->Line(screenCenterX + lineSize * 2, screenCenterY + lineSize * 2, screenCenterX + (lineSize), screenCenterY + (lineSize), col);
		g_pSurface->Line(screenCenterX + lineSize * 2, screenCenterY - lineSize * 2, screenCenterX + (lineSize), screenCenterY - (lineSize), col);
	}
}


void ESP::RenderOffScreen(C_BaseEntity* pEnt, C_BaseEntity* pLocal)
{
	if (!pLocal->IsAlive())
		return;

	if (g_LagComp.PlayerRecord[pLocal->EntIndex()].size() == 0)
		return;

	int Height, Width;
	g_pEngine->GetScreenSize(Width, Height);

	Vector2D OriginW2S, HeadW2S;
	Utils::WorldToScreen(pEnt->GetOrigin(), OriginW2S);
	Utils::WorldToScreen(pEnt->GetOrigin() + Vector(0, 0, pEnt->GetHitboxPosition(HITBOX_HEAD, g_Aimbot.Matrix[pEnt->EntIndex()]).z + 10), HeadW2S);

	if (g_Math.InScreen(OriginW2S.x, OriginW2S.y, Width, Height) || g_Math.InScreen(HeadW2S.x, HeadW2S.y, Width, Height))
		return;

	Vector ViewAngles, ViewForward;

	if (g_Menu.Config.IsThirdperson)
	{
		ViewForward = pLocal->GetOrigin();
		ViewForward.z += abs(pLocal->GetOrigin().z - pLocal->GetHitboxPosition(HITBOX_HEAD, g_Aimbot.Matrix[pLocal->EntIndex()]).z);
		ViewForward.z -= 20;
	}
	else
	{
		g_pEngine->GetViewAngles(ViewAngles);

		g_Math.AngleVectors(ViewAngles, &ViewForward);
		g_Math.NormalizeAngles(ViewForward);

		ViewForward = pLocal->GetEyePosition() + (ViewForward * 50);
		ViewForward.z -= 10;
	}

	Vector Angle = g_Math.CalcAngle(ViewForward, pEnt->GetOrigin());
	Angle.y += 90;
	Vector MidTri = g_Math.ProjectPoint(ViewForward, Angle.y, g_Menu.Config.IsThirdperson ? 30 : 25), SidTri1 = g_Math.ProjectPoint(ViewForward, Angle.y + (g_Menu.Config.IsThirdperson ? 6 : 5), g_Menu.Config.IsThirdperson ? 26 : 24), SidTri2 = g_Math.ProjectPoint(ViewForward, Angle.y - (g_Menu.Config.IsThirdperson ? 6 : 5), (g_Menu.Config.IsThirdperson ? 26 : 24));
	Vector2D MidTriW2S, SidTri1W2S, SidTri2W2S;

	Utils::WorldToScreen(MidTri, MidTriW2S);
	Utils::WorldToScreen(SidTri1, SidTri1W2S);
	Utils::WorldToScreen(SidTri2, SidTri2W2S);

	static Vertex_t TriAng[3];

	TriAng[0].mPosition = MidTriW2S;
	TriAng[1].mPosition = SidTri1W2S;
	TriAng[2].mPosition = SidTri2W2S;

	Color offscreenclr(g_Menu.Config.OffscreenEspColor.red, g_Menu.Config.OffscreenEspColor.green, g_Menu.Config.OffscreenEspColor.blue);
	g_pSurface->Polygon(3, TriAng, offscreenclr);
}

void ESP::Render()
{
	if (!g::pLocalEntity)
		return;

	color = Color(g_Menu.Config.BoxColor.red, g_Menu.Config.BoxColor.green, g_Menu.Config.BoxColor.blue, 255);
	namecolor = Color(g_Menu.Config.NameColor.red, g_Menu.Config.NameColor.green, g_Menu.Config.NameColor.blue, 255);
	hbcolor = Color(g_Menu.Config.HealthBarColor.red, g_Menu.Config.HealthBarColor.green, g_Menu.Config.HealthBarColor.blue, 255);
	hcolor = Color(g_Menu.Config.HealthColor.red, g_Menu.Config.HealthColor.green, g_Menu.Config.HealthColor.blue, 255);
	weaponcolor = Color(g_Menu.Config.WeaponColor.red, g_Menu.Config.WeaponColor.green, g_Menu.Config.WeaponColor.blue, 255);
	ypcolor = Color(g_Menu.Config.YawPitchColor.red, g_Menu.Config.YawPitchColor.green, g_Menu.Config.YawPitchColor.blue, 255);
	moneycolor = Color(g_Menu.Config.MoneyColor.red, g_Menu.Config.MoneyColor.green, g_Menu.Config.MoneyColor.blue, 255);
	fakeduckcolor = Color(g_Menu.Config.fakeduckcolor.red, g_Menu.Config.fakeduckcolor.green, g_Menu.Config.fakeduckcolor.blue, 255);
	shadow = Color(0, 0, 0, 100);

	int screen_width, screen_height;
	g_pEngine->GetScreenSize(screen_width, screen_height);

	int center_width = screen_width / 2;
	int center_height = screen_height / 2;


	for (int i = 1; i < g_pEntityList->GetHighestEntityIndex(); i++)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity == g::pLocalEntity
			|| !pPlayerEntity->IsPlayer())
			continue;

		if (!g_Menu.Config.TeamEsp && pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
			continue;

		if (g_Menu.Config.HealthBar == 2)
			hbcolor = g_Misc.getHealthColor(pPlayerEntity);

		if (g_Menu.Config.HbEsp)
		{
			Color clr = g_Misc.getHealthColor(pPlayerEntity);
			color = namecolor = hbcolor = hcolor = weaponcolor = ypcolor = moneycolor = clr;
		}

		if (g_Menu.Config.Radar) {
			pPlayerEntity->spotted() = true;
		}

		offset = 0;

		if (!pPlayerEntity->IsDormant())
			StoredCurtimePlayer[pPlayerEntity->EntIndex()] = g_pGlobalVars->curtime;

		if (pPlayerEntity->IsDormant() && flPlayerAlpha[pPlayerEntity->EntIndex()] > 0 && g_pGlobalVars->curtime - StoredCurtimePlayer[pPlayerEntity->EntIndex()] > 2)
			flPlayerAlpha[pPlayerEntity->EntIndex()] -= 5;
		else if (!(pPlayerEntity->IsDormant()) && flPlayerAlpha[pPlayerEntity->EntIndex()] < 255)
			flPlayerAlpha[pPlayerEntity->EntIndex()] += 5;

		if (pPlayerEntity->IsDormant())
		{
			color = Color(150, 150, 150, flPlayerAlpha[pPlayerEntity->EntIndex()] * 0.75);
			namecolor = hbcolor = hcolor = weaponcolor = ypcolor = moneycolor = Color(150, 150, 150, flPlayerAlpha[pPlayerEntity->EntIndex()]);
			shadow.alpha = flPlayerAlpha[pPlayerEntity->EntIndex()] * 0.33;
		}
		else
		{
			color.alpha = flPlayerAlpha[pPlayerEntity->EntIndex()] * 0.75;
			color.alpha = namecolor.alpha = hbcolor.alpha = hcolor.alpha = weaponcolor.alpha = ypcolor.alpha = moneycolor.alpha = flPlayerAlpha[pPlayerEntity->EntIndex()];
			shadow.alpha = flPlayerAlpha[pPlayerEntity->EntIndex()] * 0.33;
		}

		if (g_Menu.Config.Esp == 1 && g::pLocalEntity->IsAlive())
		{
			offsetY = 0;
			BoundBox(pPlayerEntity);
			if (Box.bottom == 0)
				continue;
			if (g_Menu.Config.Box)
				RenderBox(pPlayerEntity);
			/*	if (g_Menu.Config.Name > 0)
					RenderName(pPlayerEntity, i);*/
			if (g_Menu.Config.Health || g_Menu.Config.HealthBar)
				RenderEntHealth(pPlayerEntity);
			if (g_Menu.Config.Money)
				RenderMoney(pPlayerEntity);
			if (g_Menu.Config.Weapon)
				RenderWeaponName(pPlayerEntity);
			if (g_Menu.Config.YawPitch)
				RenderYawPitch(pPlayerEntity);
			if (g_Menu.Config.OffscreenEsp)
				RenderOffScreen(pPlayerEntity, g::pLocalEntity);
			if (g_Menu.Config.ammo)
				Ammo(pPlayerEntity);
			if (g_Menu.Config.FovEnable)
				arrows(pPlayerEntity);
		}
		else if (g_Menu.Config.Esp == 2 && !g::pLocalEntity->IsAlive())
		{
			offsetY = 0;
			BoundBox(pPlayerEntity);
			if (Box.bottom == 0)
				continue;
			if (g_Menu.Config.Box)
				RenderBox(pPlayerEntity);
			/*if (g_Menu.Config.Name > 0)
				RenderName(pPlayerEntity, i);*/
			if (g_Menu.Config.Health || g_Menu.Config.HealthBar)
				RenderEntHealth(pPlayerEntity);
			if (g_Menu.Config.Money)
				RenderMoney(pPlayerEntity);
			if (g_Menu.Config.Weapon)
				RenderWeaponName(pPlayerEntity);
			if (g_Menu.Config.YawPitch)
				RenderYawPitch(pPlayerEntity);
			if (g_Menu.Config.OffscreenEsp)
				RenderOffScreen(pPlayerEntity, g::pLocalEntity);
			if (g_Menu.Config.ammo)
				Ammo(pPlayerEntity);
		}
	}
}