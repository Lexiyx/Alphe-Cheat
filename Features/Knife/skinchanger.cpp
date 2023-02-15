#include "skinchanger.h"
#include <Windows.h>
#include "../../SDK/CEntity.h"
#include "../../Utils/GlobalVars.h"
#include "../../SDK/Definitions.h"
#include "../../SDK/ClientClass.h"
#include "../../SDK/IVModelInfo.h"
#include "../../Menu/Menu.h"
#include "../../Utils/xor.h"

#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);
std::unordered_map<char*, char*> killIcons = {};
#define INVALID_EHANDLE_INDEX 0xFFFFFFFF
HANDLE worldmodel_handle;
C_BaseCombatWeapon* worldmodel;
C_BaseCombatWeapon* pWeapon;
int custommodel;
#define MakePtr(cast, ptr, addValue) (cast)( (DWORD)(ptr) + (DWORD)(addValue))





enum class EStickerAttributeType
{
	Index,
	Wear,
	Scale,
	Rotation
};

static uint16_t s_iwoff = 0;

static void* o_uint_fn;

void knife_correct_load()
{
	if (custommodel == 1 || custommodel == 5 || custommodel == 10 || custommodel == 11)
	{
		auto iM9Bayonet = g_pModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
		pWeapon->SetWeaponModelIndex(iM9Bayonet); // m_nModelIndex
		pWeapon->SetViewModelIndex(iM9Bayonet);
		if (worldmodel) pWeapon->SetWeaponModelIndex(iM9Bayonet + 1);
		pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET);
		pWeapon->SetEntityQuality(3);
		killIcons.clear();
		killIcons["knife_default_ct"] = "knife_m9_bayonet";
		killIcons["knife_t"] = "knife_m9_bayonet";
	}
	if (custommodel == 2)
	{
		int iBayonet = g_pModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
		pWeapon->SetWeaponModelIndex(iBayonet);
		pWeapon->SetViewModelIndex(iBayonet);
		if (worldmodel) pWeapon->SetWeaponModelIndex(iBayonet + 1);
		pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_BAYONET);
		pWeapon->SetEntityQuality(3);
		killIcons.clear();
		killIcons["knife_default_ct"] = "bayonet";
		killIcons["knife_t"] = "bayonet";
	}
	if (custommodel == 3)
	{
		int iFlip = g_pModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
		pWeapon->SetWeaponModelIndex(iFlip);
		pWeapon->SetViewModelIndex(iFlip);
		if (worldmodel) pWeapon->SetWeaponModelIndex(iFlip + 1);
		pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_KNIFE_FLIP);
		pWeapon->SetEntityQuality(3);
		killIcons.clear();
		killIcons["knife_default_ct"] = "knife_flip";
		killIcons["knife_t"] = "knife_flip";
	}
	if (custommodel == 4 || custommodel == 6 || custommodel == 7 || custommodel == 8 || custommodel == 9)
	{
		int iKarambit = g_pModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
		pWeapon->SetWeaponModelIndex(iKarambit);
		pWeapon->SetViewModelIndex(iKarambit);
		if (worldmodel) pWeapon->SetWeaponModelIndex(iKarambit + 1);
		pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT);
		pWeapon->SetEntityQuality(3);
		killIcons.clear();
		killIcons["knife_default_ct"] = "knife_karambit";
		killIcons["knife_t"] = "knife_karambit";
	}
	if (custommodel == 12 || custommodel == 14)
	{
		int iFalchion = g_pModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
		pWeapon->SetWeaponModelIndex(iFalchion);
		pWeapon->SetViewModelIndex(iFalchion);
		if (worldmodel) pWeapon->SetWeaponModelIndex(iFalchion + 1);
		pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_KNIFE_FALCHION);
		pWeapon->SetEntityQuality(3);
		killIcons.clear();
		killIcons["knife_default_ct"] = "knife_falchion";
		killIcons["knife_t"] = "knife_falchion";

	}
	if (custommodel == 13)
	{
		int iButterfly = g_pModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
		pWeapon->SetWeaponModelIndex(iButterfly);
		pWeapon->SetViewModelIndex(iButterfly);
		if (worldmodel) pWeapon->SetWeaponModelIndex(iButterfly + 1);
		pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY);
		pWeapon->SetEntityQuality(3);
		killIcons.clear();
		killIcons["knife_default_ct"] = "knife_butterfly";
		killIcons["knife_t"] = "knife_butterfly";
	}
	if (custommodel == 15)
	{
		int iHuntsman = g_pModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
		pWeapon->SetWeaponModelIndex(iHuntsman);
		pWeapon->SetViewModelIndex(iHuntsman);
		if (worldmodel) pWeapon->SetWeaponModelIndex(iHuntsman + 1);
		pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL);
		pWeapon->SetEntityQuality(3);
		killIcons.clear();
		killIcons["knife_default_ct"] = "knife_tactical";
		killIcons["knife_t"] = "knife_tactical";
	}
}

void skinchanger()
{
	if (!g::pLocalEntity) return;
	auto weapons = g::pLocalEntity->m_hMyWeapons();
	if (weapons)
		for (size_t i = 0; weapons[i] != INVALID_EHANDLE_INDEX; i++)
		{
			{
				C_BaseEntity* pEntity = (C_BaseEntity*)g_pEntityList->GetClientEntityFromHandle(weapons[i]);
				if (pEntity)
				{
					pWeapon = (C_BaseCombatWeapon*)pEntity;
					if (g_Menu.Config.knivchanger || g_Menu.Config.Skinchanger.skinenabled)
					{
						int Model = g_Menu.Config.Knife;
						ItemDefinitionIndex weapon = pWeapon->GetItemDefinitionIndex();
						switch (weapon)
						{
						case ItemDefinitionIndex::WEAPON_AK47: // AK47 
						{
							switch (g_Menu.Config.Skinchanger.AK47Skin)
							{
							case 0:  pWeapon->SetFallbackPaintKit(0);   break; //none
							case 1:  pWeapon->SetFallbackPaintKit(474);   break; //Aquamarine Revenge
							case 2:  pWeapon->SetFallbackPaintKit(801); break;  //Asiimov
							case 3:  pWeapon->SetFallbackPaintKit(745); break;  //Baroque Purple
							case 4:  pWeapon->SetFallbackPaintKit(172); break;  // Black Laminate
							case 5:  pWeapon->SetFallbackPaintKit(639); break;  //Bloodsport
							case 6:  pWeapon->SetFallbackPaintKit(226);  break;  //Blue Laminate
							case 7:  pWeapon->SetFallbackPaintKit(394); break;  //Cartel
							case 8:  pWeapon->SetFallbackPaintKit(44); break;  //Case Hardened
							case 9:  pWeapon->SetFallbackPaintKit(422); break;  //Elite Build
							case 10:  pWeapon->SetFallbackPaintKit(300); break;  //Emerald Pinstripe
							case 11: pWeapon->SetFallbackPaintKit(180); break;  //Fire Serpent
							case 12: pWeapon->SetFallbackPaintKit(341);  break;  //First Class
							case 13: pWeapon->SetFallbackPaintKit(490); break;  //Frontside Misty
							case 14: pWeapon->SetFallbackPaintKit(524); break;  //Fuel Injector
							case 15: pWeapon->SetFallbackPaintKit(456); break;  //Hydroponic
							case 16: pWeapon->SetFallbackPaintKit(316); break;  //Jaguar
							case 17: pWeapon->SetFallbackPaintKit(340); break;  //Jet Set
							case 18: pWeapon->SetFallbackPaintKit(122); break;  //Jungle Spray
							case 19: pWeapon->SetFallbackPaintKit(959); break;  //Legion of Anubis
							case 20: pWeapon->SetFallbackPaintKit(600); break;  //Neon Revolution
							case 21: pWeapon->SetFallbackPaintKit(707); break;  //Neon Rider
							case 22: pWeapon->SetFallbackPaintKit(656); break;  //Orbit Mk01
							case 23: pWeapon->SetFallbackPaintKit(1018); break;  //Panthera onca 
							case 24: pWeapon->SetFallbackPaintKit(941); break;  //Phantom Disruptor
							case 25: pWeapon->SetFallbackPaintKit(506); break;  //Point Disarray
							case 26: pWeapon->SetFallbackPaintKit(170); break;  //Predator
							case 27: pWeapon->SetFallbackPaintKit(885); break;  //Rat Rod
							case 28: pWeapon->SetFallbackPaintKit(14); break;  //Red Laminate
							case 29: pWeapon->SetFallbackPaintKit(282); break;  //Redline
							case 30: pWeapon->SetFallbackPaintKit(72); break;  //Safari Mesh
							case 31: pWeapon->SetFallbackPaintKit(795); break;  //Safety Net
							case 32: pWeapon->SetFallbackPaintKit(675); break;  //The Empress
							case 33: pWeapon->SetFallbackPaintKit(836); break;  //Uncharted
							case 34: pWeapon->SetFallbackPaintKit(302); break;  //Vulcan
							case 35: pWeapon->SetFallbackPaintKit(380); break;  //Wasteland Rebel
							case 36: pWeapon->SetFallbackPaintKit(724); break;  //Wild Lotus
							case 37: pWeapon->SetFallbackPaintKit(1004); break;  //X-Ray
							default: break;
							}





						}break;
						case ItemDefinitionIndex::WEAPON_M4A1: // M4A4
						{

							switch (g_Menu.Config.Skinchanger.M4A4Skin)
							{
							case 0:  pWeapon->SetFallbackPaintKit(0);   break; //none
							case 1:  pWeapon->SetFallbackPaintKit(255);   break; //Asiimov
							case 2:  pWeapon->SetFallbackPaintKit(155);   break; //Bullet Rain
							case 3:  pWeapon->SetFallbackPaintKit(632);   break; //Buzz Kill
							case 4:  pWeapon->SetFallbackPaintKit(793);   break; //Converter
							case 5:  pWeapon->SetFallbackPaintKit(985);   break; //Cyber Security
							case 6:  pWeapon->SetFallbackPaintKit(730);   break; //Dark Blossom
							case 7:  pWeapon->SetFallbackPaintKit(471);   break; //Daybreak
							case 8:  pWeapon->SetFallbackPaintKit(8);   break; //Desert Storm
							case 9:  pWeapon->SetFallbackPaintKit(336);   break; //Desert-Strike
							case 10:  pWeapon->SetFallbackPaintKit(588);   break; //Desolate Space
							case 11:  pWeapon->SetFallbackPaintKit(480);   break; //Evil Daimyo
							case 12:  pWeapon->SetFallbackPaintKit(176);   break; //Faded Zebra
							case 13:  pWeapon->SetFallbackPaintKit(993);   break; //Global Offensive
							case 14:  pWeapon->SetFallbackPaintKit(384);   break; //Griffin
							case 15:  pWeapon->SetFallbackPaintKit(664);   break; //Hellfire
							case 16:  pWeapon->SetFallbackPaintKit(309);   break; //Howl
							case 17:  pWeapon->SetFallbackPaintKit(16);   break; //Jungle Tiger
							case 18:  pWeapon->SetFallbackPaintKit(811);   break; //Magnesium
							case 19:  pWeapon->SetFallbackPaintKit(780);   break; //Mainframe
							case 20:  pWeapon->SetFallbackPaintKit(164);   break; //Modern Hunter
							case 21:  pWeapon->SetFallbackPaintKit(695);   break; //Neo-Noir
							case 22:  pWeapon->SetFallbackPaintKit(449);   break; //Poseidon
							case 23:  pWeapon->SetFallbackPaintKit(167);   break; //Radiation Hazard
							case 24:  pWeapon->SetFallbackPaintKit(512);   break; //Royal Paladin
							case 25:  pWeapon->SetFallbackPaintKit(533);   break; //The Battlestar
							case 26:  pWeapon->SetFallbackPaintKit(844);   break; //The Emperor
							case 27:  pWeapon->SetFallbackPaintKit(971);   break; //Tooth Fairy
							case 28:  pWeapon->SetFallbackPaintKit(101);   break; //Tornado
							case 29:  pWeapon->SetFallbackPaintKit(17);   break; //Urban DDPAT
							case 30:  pWeapon->SetFallbackPaintKit(215);   break; //X-Ray
							case 31:  pWeapon->SetFallbackPaintKit(187);   break; //Zirka
							case 32:  pWeapon->SetFallbackPaintKit(400);   break; // Dragon King

							default: break;
							}


						} break;
						case ItemDefinitionIndex::WEAPON_ELITE: // dual
						{
							switch (g_Menu.Config.Skinchanger.DualSkin)
							{
							case 0:  pWeapon->SetFallbackPaintKit(0);   break; //none
							case 1:  pWeapon->SetFallbackPaintKit(28);   break; //Anodized Navy
							case 2:  pWeapon->SetFallbackPaintKit(895);   break; //Balance
							case 3:  pWeapon->SetFallbackPaintKit(190);   break; //Black Limba
							case 4:  pWeapon->SetFallbackPaintKit(330);   break; //Briar
							case 5:  pWeapon->SetFallbackPaintKit(528);   break; //Cartel
							case 6:  pWeapon->SetFallbackPaintKit(249);   break; //Cobalt Quartz
							case 7:  pWeapon->SetFallbackPaintKit(658);   break; //Cobra Strike
							case 8:  pWeapon->SetFallbackPaintKit(47);   break; //Colony
							case 9:  pWeapon->SetFallbackPaintKit(46);   break; //Contractor
							case 10:  pWeapon->SetFallbackPaintKit(153);   break; //Demolition
							case 11:  pWeapon->SetFallbackPaintKit(978);   break; //Dezastre
							case 12:  pWeapon->SetFallbackPaintKit(491);   break; //Dualing Dragons
							case 13:  pWeapon->SetFallbackPaintKit(447);   break; //Duelist
							case 14:  pWeapon->SetFallbackPaintKit(903);   break; //Elite 1.6
							case 15:  pWeapon->SetFallbackPaintKit(453);   break; //Emerald
							case 16:  pWeapon->SetFallbackPaintKit(1005);   break; //Heist
							case 17:  pWeapon->SetFallbackPaintKit(220);   break; //Hemoglobin
							case 18:  pWeapon->SetFallbackPaintKit(261);   break; //Marina
							case 19:  pWeapon->SetFallbackPaintKit(450);   break; //Moon in Libra
							case 20:  pWeapon->SetFallbackPaintKit(276);   break; //Panther
							case 21:  pWeapon->SetFallbackPaintKit(860);   break; //Pyre
							case 22:  pWeapon->SetFallbackPaintKit(307);   break; //Retribution
							case 23:  pWeapon->SetFallbackPaintKit(625);   break; //Royal Consorts
							case 24:  pWeapon->SetFallbackPaintKit(710);   break; //Shred
							case 25:  pWeapon->SetFallbackPaintKit(43);   break; //Stained
							case 26:  pWeapon->SetFallbackPaintKit(998);   break; //Switch Board
							case 27:  pWeapon->SetFallbackPaintKit(747);   break; //Twin Turbo
							case 28:  pWeapon->SetFallbackPaintKit(396);   break; //Urban Shock
							case 29:  pWeapon->SetFallbackPaintKit(544);   break; //Ventilators

							default: break;
							}
						} break;
						case ItemDefinitionIndex::WEAPON_M4A1_SILENCER: // M4A1
						{
							switch (g_Menu.Config.Skinchanger.M4A1SSkin)
							{
							case 0:  pWeapon->SetFallbackPaintKit(0);   break; //none
							case 1:  pWeapon->SetFallbackPaintKit(301); break; //Atomic Alloy
							case 2:  pWeapon->SetFallbackPaintKit(383); break; //Basilisk
							case 3:  pWeapon->SetFallbackPaintKit(217); break; //Blood Tige
							case 4:  pWeapon->SetFallbackPaintKit(1017); break; //Blue Phosphor
							case 5:  pWeapon->SetFallbackPaintKit(77); break; //Boreal Forest
							case 6:  pWeapon->SetFallbackPaintKit(663); break; //Briefing
							case 7:  pWeapon->SetFallbackPaintKit(189); break; //Bright Water
							case 8:  pWeapon->SetFallbackPaintKit(548); break; //Chantico's Fire
							case 9:  pWeapon->SetFallbackPaintKit(792); break; //Control Panel
							case 10:  pWeapon->SetFallbackPaintKit(360); break; //Cyrex
							case 11:  pWeapon->SetFallbackPaintKit(60); break; //Dark Water
							case 12:  pWeapon->SetFallbackPaintKit(644); break; //Decimator
							case 13:  pWeapon->SetFallbackPaintKit(631); break; //Flashback
							case 14:  pWeapon->SetFallbackPaintKit(497); break; //Golden Coil
							case 15:  pWeapon->SetFallbackPaintKit(257); break; //Guardian
							case 16:  pWeapon->SetFallbackPaintKit(445); break; //Hot Rod
							case 17:  pWeapon->SetFallbackPaintKit(430); break; //Hyper Beast
							case 18:  pWeapon->SetFallbackPaintKit(440); break; //Icarus Fell
							case 19:  pWeapon->SetFallbackPaintKit(326); break; //Knight
							case 20:  pWeapon->SetFallbackPaintKit(681); break; //Leaded Glass
							case 21:  pWeapon->SetFallbackPaintKit(321); break; //Master Piece
							case 22:  pWeapon->SetFallbackPaintKit(587); break; //Mecha Industries
							case 23:  pWeapon->SetFallbackPaintKit(862); break; //Moss Quart
							case 24:  pWeapon->SetFallbackPaintKit(714); break; //Nightmare
							case 25:  pWeapon->SetFallbackPaintKit(254); break; //Nitro
							case 26:  pWeapon->SetFallbackPaintKit(946); break; //Player Two
							case 27:  pWeapon->SetFallbackPaintKit(984); break; //Printstream
							case 28:  pWeapon->SetFallbackPaintKit(235); break; //VariCamo
							case 29:  pWeapon->SetFallbackPaintKit(1001); break; //Welcome to the Jungle
							default: break;
							}
						} break;
						case ItemDefinitionIndex::WEAPON_AWP: // AWP
						{
							switch (g_Menu.Config.Skinchanger.AWPSkin)
							{
							case 0:  pWeapon->SetFallbackPaintKit(0);  break; //none
							case 1:  pWeapon->SetFallbackPaintKit(788); break; //Acheron
							case 2:  pWeapon->SetFallbackPaintKit(279); break; //Asiimov
							case 3:  pWeapon->SetFallbackPaintKit(838); break; //Atheris
							case 4:  pWeapon->SetFallbackPaintKit(174); break; //BOOM
							case 5:  pWeapon->SetFallbackPaintKit(943); break; //Capillary
							case 6:  pWeapon->SetFallbackPaintKit(887); break; //Containment Breach
							case 7:  pWeapon->SetFallbackPaintKit(181); break; //Corticera
							case 8:  pWeapon->SetFallbackPaintKit(344); break; //Dragon Lore
							case 9:  pWeapon->SetFallbackPaintKit(227); break; //Electric Hive
							case 10: pWeapon->SetFallbackPaintKit(525); break; //Elite Build
							case 11: pWeapon->SetFallbackPaintKit(975); break; //Exoskeleton
							case 12: pWeapon->SetFallbackPaintKit(1026); break; //Fade
							case 13: pWeapon->SetFallbackPaintKit(640); break; //Fever Dream
							case 14: pWeapon->SetFallbackPaintKit(212); break; //Graphite
							case 15: pWeapon->SetFallbackPaintKit(756); break; //Gungnir
							case 16: pWeapon->SetFallbackPaintKit(475); break; //Hyper Beas
							case 17: pWeapon->SetFallbackPaintKit(51); break; //Lightning Strike
							case 18: pWeapon->SetFallbackPaintKit(395); break; //Man-o'-war
							case 19: pWeapon->SetFallbackPaintKit(446); break; //Medusa
							case 20: pWeapon->SetFallbackPaintKit(691); break; //Mortis
							case 21: pWeapon->SetFallbackPaintKit(803); break; //Neo-Noir
							case 22: pWeapon->SetFallbackPaintKit(662); break; //Oni Taiji
							case 23: pWeapon->SetFallbackPaintKit(718); break; //PAW
							case 24: pWeapon->SetFallbackPaintKit(584); break; //Phobos
							case 25: pWeapon->SetFallbackPaintKit(84); break; //Pink DDPAT
							case 26: pWeapon->SetFallbackPaintKit(251); break; //Pit Viper
							case 27: pWeapon->SetFallbackPaintKit(259); break; //Redline
							case 28: pWeapon->SetFallbackPaintKit(72); break; //Safari Mesh
							case 29: pWeapon->SetFallbackPaintKit(1029); break; //Silk Tiger
							case 30: pWeapon->SetFallbackPaintKit(30); break; //Snake Camo
							case 31: pWeapon->SetFallbackPaintKit(451); break; //Sun in Leo
							case 32: pWeapon->SetFallbackPaintKit(736); break; //The Prince
							case 33: pWeapon->SetFallbackPaintKit(917); break; //Wildfire
							case 34: pWeapon->SetFallbackPaintKit(424); break; //Worm God
							default: break;
							}
						} break;
						case ItemDefinitionIndex::WEAPON_USP_SILENCER: // USP
						{
							switch (g_Menu.Config.Skinchanger.USPSkin)
							{
							case 0:  pWeapon->SetFallbackPaintKit(0);   break; //none
							case 1:  pWeapon->SetFallbackPaintKit(1031); break; //Ancient Visions
							case 2:  pWeapon->SetFallbackPaintKit(217); break; //Blood Tiger
							case 3:  pWeapon->SetFallbackPaintKit(657); break; //Blueprint
							case 4:  pWeapon->SetFallbackPaintKit(364); break; //Business Class
							case 5:  pWeapon->SetFallbackPaintKit(339); break; //Caiman
							case 6:  pWeapon->SetFallbackPaintKit(796); break; //Check Engine
							case 7:  pWeapon->SetFallbackPaintKit(705); break; //Cortex
							case 8:  pWeapon->SetFallbackPaintKit(637); break; //Cyrex
							case 9:  pWeapon->SetFallbackPaintKit(60); break; //Dark Water
							case 10:  pWeapon->SetFallbackPaintKit(817); break; //Flashback
							case 11:  pWeapon->SetFallbackPaintKit(25); break; //Forest Leaves
							case 12:  pWeapon->SetFallbackPaintKit(290); break; //Guardian
							case 13:  pWeapon->SetFallbackPaintKit(504); break; //Kill Confirmed
							case 14:  pWeapon->SetFallbackPaintKit(540); break; //Lead Conduit
							case 15:  pWeapon->SetFallbackPaintKit(991); break; //Monster Mashup
							case 16:  pWeapon->SetFallbackPaintKit(653); break; //Neo-Noir
							case 17:  pWeapon->SetFallbackPaintKit(236); break; //Night
							case 18:  pWeapon->SetFallbackPaintKit(313); break; //Orion
							case 19:  pWeapon->SetFallbackPaintKit(183); break; //Overgrowth
							case 20:  pWeapon->SetFallbackPaintKit(454); break; //Para Green
							case 21:  pWeapon->SetFallbackPaintKit(443); break; //Pathfinder
							case 22:  pWeapon->SetFallbackPaintKit(318); break; //Road Rash
							case 23:  pWeapon->SetFallbackPaintKit(332); break; //Royal Blue
							case 24:  pWeapon->SetFallbackPaintKit(221); break; //Serum
							case 25:  pWeapon->SetFallbackPaintKit(277); break; //Stainless
							case 26:  pWeapon->SetFallbackPaintKit(1027); break; //Target Acquired
							case 27:  pWeapon->SetFallbackPaintKit(489); break; //Torque
							default: break;
							}
						} break;
						case ItemDefinitionIndex::WEAPON_GLOCK: // Glock
						{
							switch (g_Menu.Config.Skinchanger.GlockSkin)
							{
							case 0:	pWeapon->SetFallbackPaintKit(0); break; //none
							case 1:	pWeapon->SetFallbackPaintKit(278); break; //Blue Fissure
							case 2:	pWeapon->SetFallbackPaintKit(159); break; //Brass
							case 3:	pWeapon->SetFallbackPaintKit(957); break; //Bullet Queen
							case 4:	pWeapon->SetFallbackPaintKit(479); break; //Bunsen Burner
							case 5:	pWeapon->SetFallbackPaintKit(3); break; //Candy Apple
							case 6:	pWeapon->SetFallbackPaintKit(399); break; //Catacombs
							case 7:	pWeapon->SetFallbackPaintKit(293); break; //Death Rattle
							case 8:	pWeapon->SetFallbackPaintKit(48); break; //Dragon Tattoo
							case 9:	pWeapon->SetFallbackPaintKit(38); break; //Fade
							case 10: pWeapon->SetFallbackPaintKit(1016); break; //Franklin
							case 11: pWeapon->SetFallbackPaintKit(381); break; //Grinder
							case 12: pWeapon->SetFallbackPaintKit(2); break; //Groundwater
							case 13: pWeapon->SetFallbackPaintKit(799); break; //High Beam
							case 14: pWeapon->SetFallbackPaintKit(623); break; //Ironwork
							case 15: pWeapon->SetFallbackPaintKit(694); break; //Moonrise
							case 16: pWeapon->SetFallbackPaintKit(988); break; //Neo-Noir
							case 17: pWeapon->SetFallbackPaintKit(40); break; //Night
							case 18: pWeapon->SetFallbackPaintKit(789); break; //Nuclear Garden
							case 19: pWeapon->SetFallbackPaintKit(680); break; //Off World
							case 20: pWeapon->SetFallbackPaintKit(808); break; //Oxide Blaze
							case 21: pWeapon->SetFallbackPaintKit(367); break; //Reactor
							case 22: pWeapon->SetFallbackPaintKit(532); break; //Royal Legion
							case 23: pWeapon->SetFallbackPaintKit(918); break; //Sacrifice
							case 24: pWeapon->SetFallbackPaintKit(208); break; //Sand Dune
							case 25: pWeapon->SetFallbackPaintKit(230); break; //Steel Disruption
							case 26: pWeapon->SetFallbackPaintKit(732); break; //Synth Leaf
							case 27: pWeapon->SetFallbackPaintKit(437); break; //Twilight Galaxy
							case 28: pWeapon->SetFallbackPaintKit(963); break; //Vogue
							case 29: pWeapon->SetFallbackPaintKit(713); break; //Warhawk
							case 30: pWeapon->SetFallbackPaintKit(586); break; //Wasteland Rebel
							case 31: pWeapon->SetFallbackPaintKit(353); break; //Water Elemental
							case 32: pWeapon->SetFallbackPaintKit(607); break; //Weasel
							case 33: pWeapon->SetFallbackPaintKit(495); break; //Wraiths
							default: break;
							}
						}
						break;
						case ItemDefinitionIndex::WEAPON_DEAGLE: // Deagle
						{
							switch (g_Menu.Config.Skinchanger.DeagleSkin)
							{
							case 0:	pWeapon->SetFallbackPaintKit(0); break; //none
							case 1:	pWeapon->SetFallbackPaintKit(37); break; //Blaze
							case 2:	pWeapon->SetFallbackPaintKit(945); break; //Blue Ply
							case 3:	pWeapon->SetFallbackPaintKit(425); break; //Bronze Deco
							case 4:	pWeapon->SetFallbackPaintKit(231); break; //Cobalt Disruption
							case 5:	pWeapon->SetFallbackPaintKit(711); break; //Code Red
							case 6:	pWeapon->SetFallbackPaintKit(351); break; //Conspiracy
							case 7:	pWeapon->SetFallbackPaintKit(509); break; //Corinthian
							case 8:	pWeapon->SetFallbackPaintKit(232); break; //Crimson Web
							case 9:	pWeapon->SetFallbackPaintKit(603); break; //Directive
							case 10: pWeapon->SetFallbackPaintKit(757); break; //Emerald Jörmungandr
							case 11: pWeapon->SetFallbackPaintKit(185); break; //Golden Koi
							case 12: pWeapon->SetFallbackPaintKit(328); break; //Hand Cannon
							case 13: pWeapon->SetFallbackPaintKit(273); break; //Heirloom
							case 14: pWeapon->SetFallbackPaintKit(61); break; //Hypnotic
							case 15: pWeapon->SetFallbackPaintKit(527); break; //Kumicho Dragon
							case 16: pWeapon->SetFallbackPaintKit(841); break; //Light Rail
							case 17: pWeapon->SetFallbackPaintKit(805); break; //Mecha Industries
							case 18: pWeapon->SetFallbackPaintKit(296); break; //Meteorite
							case 19: pWeapon->SetFallbackPaintKit(468); break; //Midnight Storm
							case 20: pWeapon->SetFallbackPaintKit(90); break; //Mudder
							case 21: pWeapon->SetFallbackPaintKit(397); break; //Naga
							case 22: pWeapon->SetFallbackPaintKit(40); break; //Night
							case 23: pWeapon->SetFallbackPaintKit(1006); break; //Night Heist
							case 24: pWeapon->SetFallbackPaintKit(645); break; //Oxide Blaze
							case 25: pWeapon->SetFallbackPaintKit(347); break; //Pilot
							case 26: pWeapon->SetFallbackPaintKit(962); break; //Printstream
							case 27: pWeapon->SetFallbackPaintKit(469); break; //Sunset Storm
							case 28: pWeapon->SetFallbackPaintKit(470); break; //Sunset Storm
							case 29: pWeapon->SetFallbackPaintKit(992); break; //The Bronze
							case 30: pWeapon->SetFallbackPaintKit(17); break; //Urban DDPAT
							case 31: pWeapon->SetFallbackPaintKit(237); break; //Urban Rubble
							default:
								break;
							}
						}
						break;
						case ItemDefinitionIndex::WEAPON_FIVESEVEN: // Five Seven
						{
							switch (g_Menu.Config.Skinchanger.FiveSkin)
							{
							case 0: pWeapon->SetFallbackPaintKit(0); break; //none
							case 1: pWeapon->SetFallbackPaintKit(837); break; //Angry Mob
							case 2: pWeapon->SetFallbackPaintKit(210); break; //Anodized Gunmetal
							case 3: pWeapon->SetFallbackPaintKit(1002); break; //Berries And Cherries
							case 4: pWeapon->SetFallbackPaintKit(906); break; //Buddy
							case 5: pWeapon->SetFallbackPaintKit(3); break; //Candy Apple
							case 6: pWeapon->SetFallbackPaintKit(646); break; //Capillary
							case 7: pWeapon->SetFallbackPaintKit(44); break; //Case Hardene
							case 8: pWeapon->SetFallbackPaintKit(46); break; //Contractor
							case 9: pWeapon->SetFallbackPaintKit(784); break; //Coolant
							case 10: pWeapon->SetFallbackPaintKit(274); break; //Copper Galaxy
							case 11: pWeapon->SetFallbackPaintKit(729); break; //Crimson Blossom
							case 12: pWeapon->SetFallbackPaintKit(979); break; //Fairy Tale
							case 13: pWeapon->SetFallbackPaintKit(693); break; //Flame Test
							case 14: pWeapon->SetFallbackPaintKit(78); break; //Forest Night
							case 15: pWeapon->SetFallbackPaintKit(352); break; //Fowl Play
							case 16: pWeapon->SetFallbackPaintKit(377); break; //Hot Shot
							case 17: pWeapon->SetFallbackPaintKit(660); break; //Hyper Beast
							case 18: pWeapon->SetFallbackPaintKit(151); break; //Jungle
							case 19: pWeapon->SetFallbackPaintKit(265); break; //Kami
							case 20: pWeapon->SetFallbackPaintKit(427); break; //Monkey Business
							case 21: pWeapon->SetFallbackPaintKit(464); break; //Neon Kimono
							case 22: pWeapon->SetFallbackPaintKit(223); break; //Nightshade
							case 23: pWeapon->SetFallbackPaintKit(254); break; //Nitro
							case 24: pWeapon->SetFallbackPaintKit(141); break; //Orange Peel
							case 26: pWeapon->SetFallbackPaintKit(510); break; //Retrobution
							case 27: pWeapon->SetFallbackPaintKit(605); break; //Scumbria
							case 28: pWeapon->SetFallbackPaintKit(252); break; //Silver Quartz
							case 29: pWeapon->SetFallbackPaintKit(530); break; //Triumvirate
							case 30: pWeapon->SetFallbackPaintKit(387); break; // Urban Hazard
							case 31: pWeapon->SetFallbackPaintKit(585); break; //Violent Daimyo
							default:
								break;
							}
						}
						break;
						case ItemDefinitionIndex::WEAPON_AUG: // AUG
						{
							switch (g_Menu.Config.Skinchanger.AUGSkin)
							{
							case 0: pWeapon->SetFallbackPaintKit(0); break; //none
							case 1: pWeapon->SetFallbackPaintKit(455); break; //Akihabara Accept
							case 2: pWeapon->SetFallbackPaintKit(708); break; //Amber Slipstream
							case 3: pWeapon->SetFallbackPaintKit(197); break; //Anodized Navy
							case 4: pWeapon->SetFallbackPaintKit(886); break; //Arctic Wolf
							case 5: pWeapon->SetFallbackPaintKit(583); break; //Aristocrat
							case 6: pWeapon->SetFallbackPaintKit(9); break; //Bengal Tiger
							case 7: pWeapon->SetFallbackPaintKit(1033); break; //Carved Jade
							case 8: pWeapon->SetFallbackPaintKit(280); break; //Chameleon
							case 9: pWeapon->SetFallbackPaintKit(47); break; //Colony
							case 10: pWeapon->SetFallbackPaintKit(110); break; //Condemned
							case 11: pWeapon->SetFallbackPaintKit(46); break; //Contractor
							case 12: pWeapon->SetFallbackPaintKit(10); break; //Copperhead
							case 13: pWeapon->SetFallbackPaintKit(444); break; //Daedalus
							case 14: pWeapon->SetFallbackPaintKit(913); break; //Death by Puppy
							case 15: pWeapon->SetFallbackPaintKit(758); break; //Flame Jörmungandr
							case 16: pWeapon->SetFallbackPaintKit(541); break; //Fleet Flock
							case 17: pWeapon->SetFallbackPaintKit(33); break; //Hot Rod
							case 18: pWeapon->SetFallbackPaintKit(727); break; //Midnight Lily
							case 19: pWeapon->SetFallbackPaintKit(845); break; //Momentum
							case 20: pWeapon->SetFallbackPaintKit(740); break; //Navy Murano
							case 21: pWeapon->SetFallbackPaintKit(375); break; //Radiation Hazard
							case 22: pWeapon->SetFallbackPaintKit(779); break; //Random Access
							case 23: pWeapon->SetFallbackPaintKit(507); break; //Ricochet
							case 24: pWeapon->SetFallbackPaintKit(100); break; //Storm
							case 25: pWeapon->SetFallbackPaintKit(690); break; //Stymphalian
							case 26: pWeapon->SetFallbackPaintKit(995); break; //Surveillance
							case 27: pWeapon->SetFallbackPaintKit(794); break; //Sweeper
							case 28: pWeapon->SetFallbackPaintKit(601); break; //Syd Mead
							case 29: pWeapon->SetFallbackPaintKit(942); break; //Tom Cat
							case 30: pWeapon->SetFallbackPaintKit(305); break; //Torque
							case 31: pWeapon->SetFallbackPaintKit(674); break; //Triqua
							case 32: pWeapon->SetFallbackPaintKit(73); break; //Wings
							default:
								break;
							}
						}
						break;
						case ItemDefinitionIndex::WEAPON_FAMAS: // Famas
						{
							switch (g_Menu.Config.Skinchanger.FAMASSkin)
							{
							case 0: pWeapon->SetFallbackPaintKit(0); break; //none
							case 1: pWeapon->SetFallbackPaintKit(154); break; //Afterimage
							case 2: pWeapon->SetFallbackPaintKit(47); break; //Colony
							case 3: pWeapon->SetFallbackPaintKit(919); break; //Commemoration
							case 4: pWeapon->SetFallbackPaintKit(22); break; //Contrast Spray
							case 5: pWeapon->SetFallbackPaintKit(835); break; //Crypsis
							case 6: pWeapon->SetFallbackPaintKit(92); break; //Cyanospatter
							case 7: pWeapon->SetFallbackPaintKit(60); break; //Dark Water
							case 8: pWeapon->SetFallbackPaintKit(904); break; //Decommissioned
							case 9: pWeapon->SetFallbackPaintKit(429); break; //Djinn
							case 10: pWeapon->SetFallbackPaintKit(178); break; //Doomkitty
							case 11: pWeapon->SetFallbackPaintKit(723); break; //Eye of Athena
							case 12: pWeapon->SetFallbackPaintKit(218); break; //Hexane
							case 13: pWeapon->SetFallbackPaintKit(659); break; //Macabre
							case 14: pWeapon->SetFallbackPaintKit(626); break; //Mecha Industries
							case 15: pWeapon->SetFallbackPaintKit(477); break; //Neural Net
							case 16: pWeapon->SetFallbackPaintKit(863); break; //Night Borre
							case 17: pWeapon->SetFallbackPaintKit(999); break; //Prime Conspiracy
							case 18: pWeapon->SetFallbackPaintKit(260); break; //Pulse
							case 19: pWeapon->SetFallbackPaintKit(604); break; //Roll Cage
							case 20: pWeapon->SetFallbackPaintKit(288); break; //Sergeant
							case 21: pWeapon->SetFallbackPaintKit(194); break; //Spitfire
							case 22: pWeapon->SetFallbackPaintKit(371); break; //Styx
							case 23: pWeapon->SetFallbackPaintKit(869); break; //Sundown
							case 24: pWeapon->SetFallbackPaintKit(492); break; //Survivor
							case 25: pWeapon->SetFallbackPaintKit(244); break; //Teardown
							case 26: pWeapon->SetFallbackPaintKit(529); break; //Valence
							default:
								break;
							}
						}
						break;
						case ItemDefinitionIndex::WEAPON_G3SG1: // G3SG1
						{
							switch (g_Menu.Config.Skinchanger.G3sg1Skin)
							{
							case 0: pWeapon->SetFallbackPaintKit(0); break; //none
							case 1: pWeapon->SetFallbackPaintKit(1034); break; //
							case 2: pWeapon->SetFallbackPaintKit(6); break; //Ancient Ritual
							case 3: pWeapon->SetFallbackPaintKit(229); break; //Arctic Camo
							case 4: pWeapon->SetFallbackPaintKit(891); break; //Azure Zebra
							case 5: pWeapon->SetFallbackPaintKit(438); break; //Black Sand
							case 6: pWeapon->SetFallbackPaintKit(46); break; //Chronos
							case 7: pWeapon->SetFallbackPaintKit(195); break; //Contractor
							case 8: pWeapon->SetFallbackPaintKit(8); break; //Demeter
							case 9: pWeapon->SetFallbackPaintKit(980); break; //Desert Storm
							case 10: pWeapon->SetFallbackPaintKit(493); break; //Digital Mesh
							case 11: pWeapon->SetFallbackPaintKit(294); break; //Flux
							case 12: pWeapon->SetFallbackPaintKit(712); break; //Green Apple
							case 13: pWeapon->SetFallbackPaintKit(677); break; //High Seas
							case 14: pWeapon->SetFallbackPaintKit(147); break; //Hunter
							case 15: pWeapon->SetFallbackPaintKit(382); break; //Jungle Dashed
							case 16: pWeapon->SetFallbackPaintKit(545); break; //Murky
							case 17: pWeapon->SetFallbackPaintKit(465); break; //Orange Crash
							case 18: pWeapon->SetFallbackPaintKit(74); break; //Orange Kimono
							case 19: pWeapon->SetFallbackPaintKit(72); break; //Polar Camo
							case 20: pWeapon->SetFallbackPaintKit(806); break; //Safari Mesh
							case 21: pWeapon->SetFallbackPaintKit(628); break; //Scavenger
							case 22: pWeapon->SetFallbackPaintKit(511); break; //Stinger
							case 23: pWeapon->SetFallbackPaintKit(235); break; //The Executioner
							case 24: pWeapon->SetFallbackPaintKit(606); break; //VariCamo
							case 25: pWeapon->SetFallbackPaintKit(739); break; //Ventilator - Violet Murano
							default:
								break;
							}
						}
						break;
						case ItemDefinitionIndex::WEAPON_GALILAR: // Galil
						{
							switch (g_Menu.Config.Skinchanger.GalilSkin)
							{
							case 0: pWeapon->SetFallbackPaintKit(0); break; //none
							case 1: pWeapon->SetFallbackPaintKit(842); break; //Akoben
							case 2: pWeapon->SetFallbackPaintKit(460); break; //Aqua Terrace
							case 3: pWeapon->SetFallbackPaintKit(629); break; //Black Sand
							case 4: pWeapon->SetFallbackPaintKit(216); break; //Blue Titanium
							case 5: pWeapon->SetFallbackPaintKit(379); break; //Cerberus
							case 6: pWeapon->SetFallbackPaintKit(398); break; //Chatterbox
							case 7: pWeapon->SetFallbackPaintKit(790); break; //Cold Fusion
							case 8: pWeapon->SetFallbackPaintKit(972); break; //Connexion
							case 9: pWeapon->SetFallbackPaintKit(647); break; //Crimson Tsunami
							case 10: pWeapon->SetFallbackPaintKit(1032); break; //Dusk Ruins
							case 11: pWeapon->SetFallbackPaintKit(428); break; //Eco
							case 12: pWeapon->SetFallbackPaintKit(546); break; //Firefight
							case 13: pWeapon->SetFallbackPaintKit(241); break; //Hunting Blind
							case 14: pWeapon->SetFallbackPaintKit(308); break; //Kami
							case 15: pWeapon->SetFallbackPaintKit(83); break; //Orange DDPAT
							case 16: pWeapon->SetFallbackPaintKit(1013); break; //Phoenix Blacklight
							case 17: pWeapon->SetFallbackPaintKit(478); break; //Rocket Pop
							case 18: pWeapon->SetFallbackPaintKit(119); break; //Sage Spray
							case 19: pWeapon->SetFallbackPaintKit(264); break; //Sandstorm
							case 20: pWeapon->SetFallbackPaintKit(192); break; //Shattered
							case 21: pWeapon->SetFallbackPaintKit(807); break; //Signal
							case 22: pWeapon->SetFallbackPaintKit(494); break; //Stone Cold
							case 23: pWeapon->SetFallbackPaintKit(661); break; //Sugar Rush
							case 24: pWeapon->SetFallbackPaintKit(101); break; //Tornado
							case 25: pWeapon->SetFallbackPaintKit(297); break; //Tuxedo
							case 26: pWeapon->SetFallbackPaintKit(237); break; //Urban Rubble
							case 27: pWeapon->SetFallbackPaintKit(981); break; //Vandal
							case 28: pWeapon->SetFallbackPaintKit(235); break; //VariCamo
							case 29: pWeapon->SetFallbackPaintKit(76); break; //Winter Forest
							default:
								break;
							}
						}
						break;
						case ItemDefinitionIndex::WEAPON_M249: // M249
						{
							switch (g_Menu.Config.Skinchanger.M249Skin)
							{
							case 0: pWeapon->SetFallbackPaintKit(0); break; //none
							case 1: pWeapon->SetFallbackPaintKit(902); break; //Aztec
							case 2: pWeapon->SetFallbackPaintKit(75); break; //Blizzard Marbleized
							case 3: pWeapon->SetFallbackPaintKit(22); break; //Contrast Spray
							case 4: pWeapon->SetFallbackPaintKit(983); break; //Deep Relief
							case 5: pWeapon->SetFallbackPaintKit(648); break; //Emerald Poison Dart
							case 6: pWeapon->SetFallbackPaintKit(243); break; //Gator Mesh
							case 7: pWeapon->SetFallbackPaintKit(472); break; //Impact Drill
							case 8: pWeapon->SetFallbackPaintKit(151); break; //Jungle
							case 9: pWeapon->SetFallbackPaintKit(202); break; //Jungle DDPAT
							case 10: pWeapon->SetFallbackPaintKit(266); break; //Magma
							case 11: pWeapon->SetFallbackPaintKit(496); break; //Nebula Crusader
							case 12: pWeapon->SetFallbackPaintKit(170); break; //Predator
							case 13: pWeapon->SetFallbackPaintKit(452); break; //Shipping Forecast 
							case 14: pWeapon->SetFallbackPaintKit(547); break; //Spectre
							case 15: pWeapon->SetFallbackPaintKit(401); break; //System Lock
							case 16: pWeapon->SetFallbackPaintKit(900); break; //Warbird
							default:
								break;
							}
						}
						break;
						case ItemDefinitionIndex::WEAPON_MAC10: // Mac 10
						{
							switch (g_Menu.Config.Skinchanger.Mac10Skin)
							{
							case 0: pWeapon->SetFallbackPaintKit(0); break; //none
							case 1: pWeapon->SetFallbackPaintKit(3); break; //Candy Apple
							case 2: pWeapon->SetFallbackPaintKit(17); break; //Urban DDPAT
							case 3: pWeapon->SetFallbackPaintKit(32); break; //Silver
							case 4: pWeapon->SetFallbackPaintKit(38); break; //Fade
							case 5: pWeapon->SetFallbackPaintKit(98); break; //Ultraviolet
							case 6: pWeapon->SetFallbackPaintKit(101); break; //Tornado
							case 7: pWeapon->SetFallbackPaintKit(157); break; //Palm
							case 8: pWeapon->SetFallbackPaintKit(188); break; //Graven
							case 9: pWeapon->SetFallbackPaintKit(246); break; //Amber Fade
							case 10: pWeapon->SetFallbackPaintKit(284); break; //Heat
							case 11: pWeapon->SetFallbackPaintKit(310); break; //Curse
							case 12: pWeapon->SetFallbackPaintKit(333); break; //Indigo
							case 13: pWeapon->SetFallbackPaintKit(337); break; //Tatter
							case 14: pWeapon->SetFallbackPaintKit(343); break; //Commuter
							case 15: pWeapon->SetFallbackPaintKit(372); break; //Nuclear Garden
							case 16: pWeapon->SetFallbackPaintKit(402); break; //Malachite
							case 17: pWeapon->SetFallbackPaintKit(433); break; //Neon Rider
							case 18: pWeapon->SetFallbackPaintKit(498); break; //Rangeen
							case 19: pWeapon->SetFallbackPaintKit(534); break; //Lapis Gator
							case 20: pWeapon->SetFallbackPaintKit(550); break; //Oceanic
							case 21: pWeapon->SetFallbackPaintKit(589); break; //Carnivore
							case 22: pWeapon->SetFallbackPaintKit(651); break; //Last Dive
							case 23: pWeapon->SetFallbackPaintKit(665); break; //Aloha
							case 24: pWeapon->SetFallbackPaintKit(742); break; //Red Filigree
							case 25: pWeapon->SetFallbackPaintKit(748); break; //Calf Skin
							case 26: pWeapon->SetFallbackPaintKit(761); break; //Copper Borre
							case 27: pWeapon->SetFallbackPaintKit(812); break; //Pipe Down
							case 28: pWeapon->SetFallbackPaintKit(840); break; //Whitefish
							case 29: pWeapon->SetFallbackPaintKit(871); break; //Surfwood
							case 30: pWeapon->SetFallbackPaintKit(898); break; //Stalker
							case 31: pWeapon->SetFallbackPaintKit(908); break; //Classic Crate
							case 32: pWeapon->SetFallbackPaintKit(947); break; //Disco Tech
								break;
							}
						}
						break;
						case ItemDefinitionIndex::WEAPON_P90: // P90
						{
							switch (g_Menu.Config.Skinchanger.P90Skin)
							{
							case 0: pWeapon->SetFallbackPaintKit(0); break; //none
							case 1: pWeapon->SetFallbackPaintKit(20); break; //Virus
							case 2: pWeapon->SetFallbackPaintKit(67); break; //Cold Blooded
							case 3: pWeapon->SetFallbackPaintKit(100); break; //Storm
							case 4: pWeapon->SetFallbackPaintKit(111); break; //Glacier Mesh
							case 5: pWeapon->SetFallbackPaintKit(124); break; //Sand Spray
							case 6: pWeapon->SetFallbackPaintKit(156); break; //Death by Kitty
							case 7: pWeapon->SetFallbackPaintKit(169); break; //Fallout Warning
							case 8: pWeapon->SetFallbackPaintKit(175); break; //Scorched
							case 9: pWeapon->SetFallbackPaintKit(182); break; //Emerald Dragon
							case 10: pWeapon->SetFallbackPaintKit(228); break; //Blind Spot
							case 11: pWeapon->SetFallbackPaintKit(234); break; //Ash Wood
							case 12: pWeapon->SetFallbackPaintKit(244); break; //Teardown
							case 13: pWeapon->SetFallbackPaintKit(283); break; //Trigon
							case 14: pWeapon->SetFallbackPaintKit(311); break; //Desert Warfare
							case 15: pWeapon->SetFallbackPaintKit(335); break; //Module
							case 16: pWeapon->SetFallbackPaintKit(342); break; //Leather
							case 17: pWeapon->SetFallbackPaintKit(359); break; //Asiimov
							case 18: pWeapon->SetFallbackPaintKit(486); break; //Elite Build
							case 19: pWeapon->SetFallbackPaintKit(516); break; //Shapewood
							case 20: pWeapon->SetFallbackPaintKit(593); break; //Chopper
							case 21: pWeapon->SetFallbackPaintKit(611); break; //Grim
							case 22: pWeapon->SetFallbackPaintKit(636); break; //Shallow Grave
							case 23:  pWeapon->SetFallbackPaintKit(669); break; //Death Grip
							case 24: pWeapon->SetFallbackPaintKit(717); break; //Traction
							case 25: pWeapon->SetFallbackPaintKit(726); break; //Sunset Lily
							case 26: pWeapon->SetFallbackPaintKit(744); break; //Baroque Red
							case 27: pWeapon->SetFallbackPaintKit(759); break; //Astral Jörmungandr
							case 28: pWeapon->SetFallbackPaintKit(776); break; //Facility Negative
							case 29: pWeapon->SetFallbackPaintKit(849); break; //Off World
							case 30: pWeapon->SetFallbackPaintKit(911); break; //Nostalgia
								break;
							}
						}
						break;
						case ItemDefinitionIndex::WEAPON_UMP45: // UMP-45
						{
							switch (g_Menu.Config.Skinchanger.UMP45Skin)
							{
							case 0: pWeapon->SetFallbackPaintKit(0); break; //none
							case 1: pWeapon->SetFallbackPaintKit(15); break; //Gunsmoke
							case 2: pWeapon->SetFallbackPaintKit(17); break; //Urban DDPAT
							case 3: pWeapon->SetFallbackPaintKit(37); break; //Blaze
							case 4: pWeapon->SetFallbackPaintKit(70); break; //Carbon Fiber
							case 5: pWeapon->SetFallbackPaintKit(90); break; //Mudder
							case 6: pWeapon->SetFallbackPaintKit(93); break; //Caramel
							case 7: pWeapon->SetFallbackPaintKit(169); break; //Fallout Warning
							case 8: pWeapon->SetFallbackPaintKit(175); break; //Scorched
							case 9: pWeapon->SetFallbackPaintKit(193); break; //Bone Pile
							case 10: pWeapon->SetFallbackPaintKit(281); break; //Corporal
							case 11: pWeapon->SetFallbackPaintKit(333); break; //Indigo
							case 12: pWeapon->SetFallbackPaintKit(362); break; //Labyrinth
							case 13: pWeapon->SetFallbackPaintKit(392); break; //Delusion
							case 14: pWeapon->SetFallbackPaintKit(436); break; //Grand Prix
							case 15: pWeapon->SetFallbackPaintKit(441); break; //Minotaur's Labyrinth
							case 16: pWeapon->SetFallbackPaintKit(488); break; //Riot
							case 17: pWeapon->SetFallbackPaintKit(556); break; //Primal Saber
							case 18: pWeapon->SetFallbackPaintKit(615); break; //Briefing
							case 19: pWeapon->SetFallbackPaintKit(652); break; //Scaffold
							case 20: pWeapon->SetFallbackPaintKit(672); break; //Metal Flowers
							case 21: pWeapon->SetFallbackPaintKit(688); break; //Exposure
							case 22: pWeapon->SetFallbackPaintKit(704); break; //Arctic Wolf
							case 23: pWeapon->SetFallbackPaintKit(725); break; //Day Lily
							case 24: pWeapon->SetFallbackPaintKit(778); break; //Facility Dark
							case 25: pWeapon->SetFallbackPaintKit(802); break; //Momentum
							case 26: pWeapon->SetFallbackPaintKit(851); break; //Moonrise
							case 27: pWeapon->SetFallbackPaintKit(916); break; //Plastique
							default:
								break;
							}
						}
						break;
						case ItemDefinitionIndex::WEAPON_HKP2000: // P2000
						{
							switch (g_Menu.Config.Skinchanger.P2000Skin)
							{
							case 0: pWeapon->SetFallbackPaintKit(0); break; //none
							case 1: pWeapon->SetFallbackPaintKit(21); break; //Granite Marbleized
							case 2: pWeapon->SetFallbackPaintKit(32); break; //Silver
							case 3: pWeapon->SetFallbackPaintKit(71); break; //Scorpion
							case 4: pWeapon->SetFallbackPaintKit(95); break; //Grassland
							case 5: pWeapon->SetFallbackPaintKit(104); break; //Grassland Leaves
							case 6: pWeapon->SetFallbackPaintKit(184); break; //Corticera
							case 7: pWeapon->SetFallbackPaintKit(211); break; //Ocean Foam
							case 8: pWeapon->SetFallbackPaintKit(246); break; //Amber Fade
							case 9: pWeapon->SetFallbackPaintKit(275); break; //Red FragCam
							case 10: pWeapon->SetFallbackPaintKit(327); break; //Chainmail
							case 11: pWeapon->SetFallbackPaintKit(338); break; //Pulse
							case 12: pWeapon->SetFallbackPaintKit(346); break; //Coach Class
							case 13: pWeapon->SetFallbackPaintKit(357); break; //Ivory
							case 14: pWeapon->SetFallbackPaintKit(389); break; //Fire Elemental
							case 15: pWeapon->SetFallbackPaintKit(443); break; //Pathfinder
							case 16: pWeapon->SetFallbackPaintKit(485); break; //Handgun
							case 17: pWeapon->SetFallbackPaintKit(515); break; //Imperial
							case 18: pWeapon->SetFallbackPaintKit(550); break; //Oceanic
							case 19: pWeapon->SetFallbackPaintKit(591); break; //Imperial Dragon
							case 20: pWeapon->SetFallbackPaintKit(635); break; //Turf
							case 21: pWeapon->SetFallbackPaintKit(667); break; //Woodsman
							case 22: pWeapon->SetFallbackPaintKit(700); break; //Urban Hazard
							case 23: pWeapon->SetFallbackPaintKit(894); break; //Obsidian
							case 24: pWeapon->SetFallbackPaintKit(951); break; //Acid Etched
							default:
								break;
							}
						}
						break;
						case ItemDefinitionIndex::WEAPON_MP7: // MP7
						{
							switch (g_Menu.Config.Skinchanger.Mp7Skin)
							{
							case 0: pWeapon->SetFallbackPaintKit(0); break; //none
							case 1: pWeapon->SetFallbackPaintKit(5); break; //Forest DDPAT
							case 2: pWeapon->SetFallbackPaintKit(11); break; //Skulls
							case 3: pWeapon->SetFallbackPaintKit(15); break; //Gunsmoke
							case 4: pWeapon->SetFallbackPaintKit(28); break; //Anodized Navy
							case 5: pWeapon->SetFallbackPaintKit(102); break; //Whiteout
							case 6: pWeapon->SetFallbackPaintKit(141); break; //Orange Peel
							case 7: pWeapon->SetFallbackPaintKit(175); break; //Scorched
							case 8: pWeapon->SetFallbackPaintKit(209); break; //Groundwater
							case 9: pWeapon->SetFallbackPaintKit(213); break; //Ocean Foam
							case 10: pWeapon->SetFallbackPaintKit(245); break; //Army Recon
							case 11: pWeapon->SetFallbackPaintKit(250); break; //Full Stop
							case 12: pWeapon->SetFallbackPaintKit(354); break; //Urban Hazard
							case 13: pWeapon->SetFallbackPaintKit(365); break; //Olive Plaid
							case 14: pWeapon->SetFallbackPaintKit(423); break; //Armor Core
							case 15: pWeapon->SetFallbackPaintKit(442); break; //Asterion
							case 16: pWeapon->SetFallbackPaintKit(481); break; //Nemesis
							case 17: pWeapon->SetFallbackPaintKit(500); break; //Special Delivery
							case 18: pWeapon->SetFallbackPaintKit(536); break; //Impire
							case 19: pWeapon->SetFallbackPaintKit(627); break; //Cirrus
							case 20: pWeapon->SetFallbackPaintKit(649); break; //Akoben
							case 21: pWeapon->SetFallbackPaintKit(696); break; //Bloodsport
							case 22: pWeapon->SetFallbackPaintKit(719); break; //Powercore
							case 23: pWeapon->SetFallbackPaintKit(728); break; //Teal Blossom
							case 24: pWeapon->SetFallbackPaintKit(752); break; //Fade
							case 25: pWeapon->SetFallbackPaintKit(782); break; //Motherboard
							case 26: pWeapon->SetFallbackPaintKit(847); break; //Mischief
							case 27: pWeapon->SetFallbackPaintKit(893); break; //Neon Ply
							default:
								break;
							}
						}
						break;
						case ItemDefinitionIndex::WEAPON_MP9: // MP9
						{
							switch (g_Menu.Config.Skinchanger.Mp9Skin)
							{
							case 0: pWeapon->SetFallbackPaintKit(0); break; //none
							case 1: pWeapon->SetFallbackPaintKit(33); break; //Hot Rod
							case 2: pWeapon->SetFallbackPaintKit(39); break; //Bulldozer
							case 3: pWeapon->SetFallbackPaintKit(61); break; //Hypnotic
							case 4: pWeapon->SetFallbackPaintKit(100); break; //Storm
							case 5: pWeapon->SetFallbackPaintKit(141); break; //Orange Peel
							case 6: pWeapon->SetFallbackPaintKit(148); break; //Sand Dashed
							case 7: pWeapon->SetFallbackPaintKit(199); break; //Dry Season
							case 8: pWeapon->SetFallbackPaintKit(262); break; //Rose Iron
							case 9: pWeapon->SetFallbackPaintKit(329); break; //Dark Age
							case 10: pWeapon->SetFallbackPaintKit(366); break; //Green Plaid
							case 11: pWeapon->SetFallbackPaintKit(368); break; //Setting Sun
							case 12: pWeapon->SetFallbackPaintKit(386); break; //Dart
							case 13: pWeapon->SetFallbackPaintKit(403); break; //Deadly Poison
							case 14: pWeapon->SetFallbackPaintKit(448); break; //Pandora's Box
							case 15: pWeapon->SetFallbackPaintKit(482); break; //Ruby Poison Dart
							case 16: pWeapon->SetFallbackPaintKit(549); break; //Bioleak
							case 17: pWeapon->SetFallbackPaintKit(609); break; //Airlock
							case 18: pWeapon->SetFallbackPaintKit(630); break; //Sand Scale
							case 19: pWeapon->SetFallbackPaintKit(679); break; //Goo
							case 20: pWeapon->SetFallbackPaintKit(697); break; //Black Sand
							case 21: pWeapon->SetFallbackPaintKit(715); break; //Capillary
							case 22: pWeapon->SetFallbackPaintKit(734); break; //Wild Lily
							case 23: pWeapon->SetFallbackPaintKit(755); break; //Slide
							case 24: pWeapon->SetFallbackPaintKit(804); break; //Modest Threat
							case 25: pWeapon->SetFallbackPaintKit(867); break; //Stained Glass
							case 26: pWeapon->SetFallbackPaintKit(910); break; //Hydra
							default:
								break;
							}
						}
						break;
						case ItemDefinitionIndex::WEAPON_P250: // P250
						{
							switch (g_Menu.Config.Skinchanger.P250Skin)
							{
							case 0: pWeapon->SetFallbackPaintKit(0); break; //none
							case 1: pWeapon->SetFallbackPaintKit(15); break; //Gunsmoke
							case 2: pWeapon->SetFallbackPaintKit(27); break; //Bone Mask
							case 3: pWeapon->SetFallbackPaintKit(34); break; //Metallic DDPAT
							case 4: pWeapon->SetFallbackPaintKit(77); break; //Boreal Forest
							case 5: pWeapon->SetFallbackPaintKit(99); break; //Sand Dune
							case 6: pWeapon->SetFallbackPaintKit(102); break; //Whiteout
							case 7: pWeapon->SetFallbackPaintKit(162); break; //Splash
							case 8: pWeapon->SetFallbackPaintKit(164); break; //Modern Hunter
							case 9: pWeapon->SetFallbackPaintKit(168); break; //Nuclear Threat
							case 10: pWeapon->SetFallbackPaintKit(207); break; //Facets
							case 11: pWeapon->SetFallbackPaintKit(125); break; //X-Ray
							case 12: pWeapon->SetFallbackPaintKit(219); break; //Hive
							case 13: pWeapon->SetFallbackPaintKit(230); break; //Steel Disruption
							case 14: pWeapon->SetFallbackPaintKit(258); break; //Mehndi
							case 15: pWeapon->SetFallbackPaintKit(271); break; //Undertow
							case 16: pWeapon->SetFallbackPaintKit(295); break; //Franklin
							case 17: pWeapon->SetFallbackPaintKit(358); break; //Supernova
							case 18: pWeapon->SetFallbackPaintKit(373); break; //Contamination
							case 19: pWeapon->SetFallbackPaintKit(388); break; //Cartel
							case 20: pWeapon->SetFallbackPaintKit(404); break; //Muertos
							case 21: pWeapon->SetFallbackPaintKit(426); break; //Valence
							case 22: pWeapon->SetFallbackPaintKit(466); break; //Crimson Kimon
							case 23: pWeapon->SetFallbackPaintKit(467); break; //Mint Kimono
							case 24: pWeapon->SetFallbackPaintKit(501); break; //Wingshot
							case 25: pWeapon->SetFallbackPaintKit(551); break; //Asiimov
							case 26: pWeapon->SetFallbackPaintKit(592); break; //Iron Clad
							case 27: pWeapon->SetFallbackPaintKit(650); break; //Ripple
							case 28: pWeapon->SetFallbackPaintKit(668); break; //Red Rock
							case 29: pWeapon->SetFallbackPaintKit(678); break; //See Ya Later
							case 30: pWeapon->SetFallbackPaintKit(741); break; //Dark Filigree
							case 31: pWeapon->SetFallbackPaintKit(749); break; //Vino Primo
							case 32: pWeapon->SetFallbackPaintKit(777); break; //Facility Draft
							case 33: pWeapon->SetFallbackPaintKit(786); break; //Exchanger
							case 34: pWeapon->SetFallbackPaintKit(813); break; //Nevermore
							case 35: pWeapon->SetFallbackPaintKit(848); break; //Verdigris
							case 36: pWeapon->SetFallbackPaintKit(907); break; //Inferno
							default:
								break;
							}
						}
						break;
						case ItemDefinitionIndex::WEAPON_SCAR20: // Scar 20
						{
							switch (g_Menu.Config.Skinchanger.SCAR20Skin)
							{
							case 0:
								pWeapon->SetFallbackPaintKit(0);//none
								break;
							case 1:
								pWeapon->SetFallbackPaintKit(165);//Splash Jam
								break;
							case 2:
								pWeapon->SetFallbackPaintKit(100);//Storm
								break;
							case 3:
								pWeapon->SetFallbackPaintKit(46);//Contractor
								break;
							case 4:
								pWeapon->SetFallbackPaintKit(70);//Carbon Fiber
								break;
							case 5:
								pWeapon->SetFallbackPaintKit(116);//Sand Mesh
								break;
							case 6:
								pWeapon->SetFallbackPaintKit(157);//Palm
								break;
							case 7:
								pWeapon->SetFallbackPaintKit(232);//Crimson Web
								break;
							case 8:
								pWeapon->SetFallbackPaintKit(391);//Cardiac
								break;
							case 9:
								pWeapon->SetFallbackPaintKit(298);//Army Sheen
								break;
							case 10:
								pWeapon->SetFallbackPaintKit(312);//Cyrex
								break;
							case 11:
								pWeapon->SetFallbackPaintKit(406);//Grotto
								break;
							case 12:
								pWeapon->SetFallbackPaintKit(597);//Bloodsport
								break;
							default:
								break;
							}
						}
						break;
						case ItemDefinitionIndex::WEAPON_SG556: // SG553
						{
							switch (g_Menu.Config.Skinchanger.Sg553Skin)
							{
							case 0:
								pWeapon->SetFallbackPaintKit(0);//none
								break;
							case 1:
								pWeapon->SetFallbackPaintKit(519);
								break;
							case 2:
								pWeapon->SetFallbackPaintKit(487);
								break;
							case 3:
								pWeapon->SetFallbackPaintKit(287);
								break;
							case 4:
								pWeapon->SetFallbackPaintKit(586);
								break;
							default:
								break;
							}
						}
						break;
						case ItemDefinitionIndex::WEAPON_SSG08: // SSG08
						{
							switch (g_Menu.Config.Skinchanger.SSG08Skin)
							{
							case 0:
								pWeapon->SetFallbackPaintKit(0);//none
								break;
							case 1:
								pWeapon->SetFallbackPaintKit(26);//26 - Lichen Dashed
								break;
							case 2:
								pWeapon->SetFallbackPaintKit(60);//60 - Dark Water
								break;
							case 3:
								pWeapon->SetFallbackPaintKit(96);//96 - Blue Spruce
								break;
							case 4:
								pWeapon->SetFallbackPaintKit(99);//99 - Sand Dune
								break;
							case 5:
								pWeapon->SetFallbackPaintKit(157);//157 - Palm
								break;
							case 6:
								pWeapon->SetFallbackPaintKit(200);//200 - Mayan Dreams
								break;
							case 7:
								pWeapon->SetFallbackPaintKit(222);//222 - Blood in the Water
								break;
							case 8:
								pWeapon->SetFallbackPaintKit(233);//233 - Tropical Storm
								break;
							case 9:
								pWeapon->SetFallbackPaintKit(253);//253 - Acid Fade
								break;
							case 10:
								pWeapon->SetFallbackPaintKit(304);//304 - Slashed
								break;
							case 11:
								pWeapon->SetFallbackPaintKit(319);//319 - Detour
								break;
							case 12:
								pWeapon->SetFallbackPaintKit(361);//361 - Abyss
								break;
							case 13:
								pWeapon->SetFallbackPaintKit(503);//503: 'Big Iron'
								break;
							case 14:
								pWeapon->SetFallbackPaintKit(538);//538: 'Necropos'
								break;
							case 15:
								pWeapon->SetFallbackPaintKit(554);//554 - Ghost Crusader
								break;
							case 16:
								pWeapon->SetFallbackPaintKit(624);//624: 'Dragonfire'
								break;
							default:
								break;
							}
						}
						break;
						case ItemDefinitionIndex::WEAPON_REVOLVER: // Revolver
						{
							switch (g_Menu.Config.Skinchanger.RevolverSkin)
							{
							case 0: pWeapon->SetFallbackPaintKit(0); break;//none
							case 1: pWeapon->SetFallbackPaintKit(12); break;//Crimson Web
							case 2: pWeapon->SetFallbackPaintKit(27); break;//Bone Mask
							case 3: pWeapon->SetFallbackPaintKit(135); break;//Urban Perforated
							case 4: pWeapon->SetFallbackPaintKit(136); break;//Waves Perforated
							case 5: pWeapon->SetFallbackPaintKit(141); break;//Orange Peel
							case 6: pWeapon->SetFallbackPaintKit(143); break;//Urban Masked
							case 7: pWeapon->SetFallbackPaintKit(147); break;//Jungle Dashed
							case 8: pWeapon->SetFallbackPaintKit(148); break;//Sand Dashed
							case 9: pWeapon->SetFallbackPaintKit(149); break;//Urban Dashed
							case 10: pWeapon->SetFallbackPaintKit(199); break;//Dry Season
							case 11: pWeapon->SetFallbackPaintKit(522); break;//Fade
							case 12: pWeapon->SetFallbackPaintKit(523); break;//Amber Fade
							case 13: pWeapon->SetFallbackPaintKit(595); break;//Reboot
							case 14: pWeapon->SetFallbackPaintKit(683); break;//Llama Cannon
							case 15: pWeapon->SetFallbackPaintKit(701); break;//Grip
							case 16: pWeapon->SetFallbackPaintKit(721); break;//Survivalist
							case 17: pWeapon->SetFallbackPaintKit(798); break;//Nitro
							case 18: pWeapon->SetFallbackPaintKit(843); break;//Skull Crusher
							case 19: pWeapon->SetFallbackPaintKit(866); break;//Canal Spray
							case 20: pWeapon->SetFallbackPaintKit(892); break;//Memento
							case 21: pWeapon->SetFallbackPaintKit(952); break;//Bone Forged
							default:

								break;
							}
						}
						break;
						default:
							break;
						}

						if (pEntity->GetClientClass()->ClassID == (int)107)
						{
							g_Menu.Config.fix = true;
							//auto pCustomName1 = MakePtr(char*, pWeapon, 0x301C);

							HANDLE worldmodel_handle2 = pWeapon->m_hWeaponWorldModel_h();
							if (worldmodel_handle2) worldmodel = (C_BaseCombatWeapon*)g_pEntityList->GetClientEntityFromHandle(pWeapon->m_hWeaponWorldModel_c());
							if (Model == 0) // Bayonet
							{
								int iBayonet = g_pModelInfo->GetModelIndex(SteamStrings("models/weapons/v_knife_bayonet.mdl"));
								pWeapon->SetWeaponModelIndex(iBayonet);
								pWeapon->SetViewModelIndex(iBayonet);
								if (worldmodel)pWeapon->SetWeaponModelIndex(iBayonet + 1);
								pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_BAYONET);
								pWeapon->SetEntityQuality(3);
								g_Menu.Config.killicon_config = SteamStrings("bayonet");
								int Skin = g_Menu.Config.KnifeSkin;
								return;
								if (Skin == 0)
								{
									pWeapon->SetFallbackPaintKit(0); // Forest DDPAT
								}
								else if (Skin == 1)
								{
									pWeapon->SetFallbackPaintKit(12); // Crimson Web
								}
								else if (Skin == 2)
								{
									pWeapon->SetFallbackPaintKit(27); // Bone Mask
								}
								else if (Skin == 3)
								{
									pWeapon->SetFallbackPaintKit(38); // Fade
								}
								else if (Skin == 4)
								{
									pWeapon->SetFallbackPaintKit(40); // Night
								}
								else if (Skin == 5)
								{
									pWeapon->SetFallbackPaintKit(42); // Blue Steel
								}
								else if (Skin == 6)
								{
									pWeapon->SetFallbackPaintKit(43); // Stained
								}
								else if (Skin == 7)
								{
									pWeapon->SetFallbackPaintKit(44); // Case Hardened
								}
								else if (Skin == 8)
								{
									pWeapon->SetFallbackPaintKit(59); // Slaughter
								}
								else if (Skin == 9)
								{
									pWeapon->SetFallbackPaintKit(72); // Safari Mesh
								}
								else if (Skin == 10)
								{
									pWeapon->SetFallbackPaintKit(77); // Boreal Forest
								}
								else if (Skin == 11)
								{
									pWeapon->SetFallbackPaintKit(98); // Ultraviolet
								}
								else if (Skin == 12)
								{
									pWeapon->SetFallbackPaintKit(143); // Urban Masked
								}
								else if (Skin == 13)
								{
									pWeapon->SetFallbackPaintKit(175); // Scorched
								}
								else if (Skin == 14)
								{
									pWeapon->SetFallbackPaintKit(323); // Rust Coat
								}
								else if (Skin == 15)
								{
									pWeapon->SetFallbackPaintKit(409); // Tiger Tooth
								}
								else if (Skin == 16)
								{
									pWeapon->SetFallbackPaintKit(410); // Damascus Steel
								}
								else if (Skin == 17)
								{
									pWeapon->SetFallbackPaintKit(411); // Damascus Steel
								}
								else if (Skin == 18)
								{
									pWeapon->SetFallbackPaintKit(413); // Marble Fade
								}
								else if (Skin == 19)
								{
									pWeapon->SetFallbackPaintKit(414); // Rust Coat
								}
								else if (Skin == 20)
								{
									pWeapon->SetFallbackPaintKit(415); // Doppler Ruby
								}
								else if (Skin == 21)
								{
									pWeapon->SetFallbackPaintKit(416); // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									pWeapon->SetFallbackPaintKit(417); // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									pWeapon->SetFallbackPaintKit(418); // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									pWeapon->SetFallbackPaintKit(419); // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									pWeapon->SetFallbackPaintKit(420); // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									pWeapon->SetFallbackPaintKit(421); // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									pWeapon->SetFallbackPaintKit(569); // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									pWeapon->SetFallbackPaintKit(570); // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									pWeapon->SetFallbackPaintKit(571); // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									pWeapon->SetFallbackPaintKit(558); // Lore
								}
								else if (Skin == 33)
								{
									pWeapon->SetFallbackPaintKit(563); // Black Laminate
								}
								else if (Skin == 34)
								{
									pWeapon->SetFallbackPaintKit(573); // Autotronic
								}
								else if (Skin == 35)
								{
									pWeapon->SetFallbackPaintKit(580); // Freehand
								}
								else if (Skin == 36)
								{
									pWeapon->SetFallbackPaintKit(852); // 
								}
								else if (Skin == 37)
								{
									pWeapon->SetFallbackPaintKit(853); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(854); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(855); // 
								}
								else if (Skin == 39)
								{
									pWeapon->SetFallbackPaintKit(856); // 
								}
								else if (Skin == 40)
								{
									pWeapon->SetFallbackPaintKit(857); // 
								}
								else if (Skin == 41)
								{
									pWeapon->SetFallbackPaintKit(858); // 
								}
							}
							else if (Model == 9) // Bowie Knife
							{
								int iBowie = g_pModelInfo->GetModelIndex(SteamStrings("models/weapons/v_knife_survival_bowie.mdl"));
								pWeapon->SetWeaponModelIndex(iBowie);
								pWeapon->SetViewModelIndex(iBowie);
								if (worldmodel)pWeapon->SetWeaponModelIndex(iBowie + 1);
								pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_KNIFE_SURVIVAL_BOWIE);
								pWeapon->SetEntityQuality(3);
								g_Menu.Config.killicon_config = SteamStrings("knife_survival_bowie");
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									pWeapon->SetFallbackPaintKit(0); // Forest DDPAT
								}
								else if (Skin == 1)
								{
									pWeapon->SetFallbackPaintKit(12); // Crimson Web
								}
								else if (Skin == 2)
								{
									pWeapon->SetFallbackPaintKit(27); // Bone Mask
								}
								else if (Skin == 3)
								{
									pWeapon->SetFallbackPaintKit(38); // Fade
								}
								else if (Skin == 4)
								{
									pWeapon->SetFallbackPaintKit(40); // Night
								}
								else if (Skin == 5)
								{
									pWeapon->SetFallbackPaintKit(42); // Blue Steel
								}
								else if (Skin == 6)
								{
									pWeapon->SetFallbackPaintKit(43); // Stained
								}
								else if (Skin == 7)
								{
									pWeapon->SetFallbackPaintKit(44); // Case Hardened
								}
								else if (Skin == 8)
								{
									pWeapon->SetFallbackPaintKit(59); // Slaughter
								}
								else if (Skin == 9)
								{
									pWeapon->SetFallbackPaintKit(72); // Safari Mesh
								}
								else if (Skin == 10)
								{
									pWeapon->SetFallbackPaintKit(77); // Boreal Forest
								}
								else if (Skin == 11)
								{
									pWeapon->SetFallbackPaintKit(98); // Ultraviolet
								}
								else if (Skin == 12)
								{
									pWeapon->SetFallbackPaintKit(143); // Urban Masked
								}
								else if (Skin == 13)
								{
									pWeapon->SetFallbackPaintKit(175); // Scorched
								}
								else if (Skin == 14)
								{
									pWeapon->SetFallbackPaintKit(323); // Rust Coat
								}
								else if (Skin == 15)
								{
									pWeapon->SetFallbackPaintKit(409); // Tiger Tooth
								}
								else if (Skin == 16)
								{
									pWeapon->SetFallbackPaintKit(410); // Damascus Steel
								}
								else if (Skin == 17)
								{
									pWeapon->SetFallbackPaintKit(411); // Damascus Steel
								}
								else if (Skin == 18)
								{
									pWeapon->SetFallbackPaintKit(413); // Marble Fade
								}
								else if (Skin == 19)
								{
									pWeapon->SetFallbackPaintKit(414); // Rust Coat
								}
								else if (Skin == 20)
								{
									pWeapon->SetFallbackPaintKit(415); // Doppler Ruby
								}
								else if (Skin == 21)
								{
									pWeapon->SetFallbackPaintKit(416); // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									pWeapon->SetFallbackPaintKit(417); // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									pWeapon->SetFallbackPaintKit(418); // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									pWeapon->SetFallbackPaintKit(419); // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									pWeapon->SetFallbackPaintKit(420); // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									pWeapon->SetFallbackPaintKit(421); // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									pWeapon->SetFallbackPaintKit(569); // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									pWeapon->SetFallbackPaintKit(570); // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									pWeapon->SetFallbackPaintKit(571); // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									pWeapon->SetFallbackPaintKit(558); // Lore
								}
								else if (Skin == 33)
								{
									pWeapon->SetFallbackPaintKit(567); // Black Laminate
								}
								else if (Skin == 34)
								{
									pWeapon->SetFallbackPaintKit(574); // Autotronic
								}
								else if (Skin == 35)
								{
									pWeapon->SetFallbackPaintKit(580); // Freehand
								}
								else if (Skin == 36)
								{
									pWeapon->SetFallbackPaintKit(852); // 
								}
								else if (Skin == 37)
								{
									pWeapon->SetFallbackPaintKit(853); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(854); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(855); // 
								}
								else if (Skin == 39)
								{
									pWeapon->SetFallbackPaintKit(856); // 
								}
								else if (Skin == 40)
								{
									pWeapon->SetFallbackPaintKit(857); // 
								}
								else if (Skin == 41)
								{
									pWeapon->SetFallbackPaintKit(858); // 
								}
							}
							else if (Model == 6) // Butterfly Knife
							{
								int iButterfly = g_pModelInfo->GetModelIndex(SteamStrings("models/weapons/v_knife_butterfly.mdl"));
								pWeapon->SetWeaponModelIndex(iButterfly);
								pWeapon->SetViewModelIndex(iButterfly);
								if (worldmodel)pWeapon->SetWeaponModelIndex(iButterfly + 1);
								pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY);
								pWeapon->SetEntityQuality(3);
								g_Menu.Config.killicon_config = SteamStrings("knife_butterfly");
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									pWeapon->SetFallbackPaintKit(0); // Forest DDPAT
								}
								else if (Skin == 1)
								{
									pWeapon->SetFallbackPaintKit(12); // Crimson Web
								}
								else if (Skin == 2)
								{
									pWeapon->SetFallbackPaintKit(27); // Bone Mask
								}
								else if (Skin == 3)
								{
									pWeapon->SetFallbackPaintKit(38); // Fade
								}
								else if (Skin == 4)
								{
									pWeapon->SetFallbackPaintKit(40); // Night
								}
								else if (Skin == 5)
								{
									pWeapon->SetFallbackPaintKit(42); // Blue Steel
								}
								else if (Skin == 6)
								{
									pWeapon->SetFallbackPaintKit(43); // Stained
								}
								else if (Skin == 7)
								{
									pWeapon->SetFallbackPaintKit(44); // Case Hardened
								}
								else if (Skin == 8)
								{
									pWeapon->SetFallbackPaintKit(59); // Slaughter
								}
								else if (Skin == 9)
								{
									pWeapon->SetFallbackPaintKit(72); // Safari Mesh
								}
								else if (Skin == 10)
								{
									pWeapon->SetFallbackPaintKit(77); // Boreal Forest
								}
								else if (Skin == 11)
								{
									pWeapon->SetFallbackPaintKit(98); // Ultraviolet
								}
								else if (Skin == 12)
								{
									pWeapon->SetFallbackPaintKit(143); // Urban Masked
								}
								else if (Skin == 13)
								{
									pWeapon->SetFallbackPaintKit(175); // Scorched
								}
								else if (Skin == 14)
								{
									pWeapon->SetFallbackPaintKit(323); // Rust Coat
								}
								else if (Skin == 15)
								{
									pWeapon->SetFallbackPaintKit(409); // Tiger Tooth
								}
								else if (Skin == 16)
								{
									pWeapon->SetFallbackPaintKit(410); // Damascus Steel
								}
								else if (Skin == 17)
								{
									pWeapon->SetFallbackPaintKit(411); // Damascus Steel
								}
								else if (Skin == 18)
								{
									pWeapon->SetFallbackPaintKit(413); // Marble Fade
								}
								else if (Skin == 19)
								{
									pWeapon->SetFallbackPaintKit(414); // Rust Coat
								}
								else if (Skin == 20)
								{
									pWeapon->SetFallbackPaintKit(415); // Doppler Ruby
								}
								else if (Skin == 21)
								{
									pWeapon->SetFallbackPaintKit(416); // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									pWeapon->SetFallbackPaintKit(417); // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									pWeapon->SetFallbackPaintKit(418); // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									pWeapon->SetFallbackPaintKit(419); // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									pWeapon->SetFallbackPaintKit(420); // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									pWeapon->SetFallbackPaintKit(421); // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									pWeapon->SetFallbackPaintKit(569); // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									pWeapon->SetFallbackPaintKit(570); // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									pWeapon->SetFallbackPaintKit(571); // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									pWeapon->SetFallbackPaintKit(558); // Lore
								}
								else if (Skin == 33)
								{
									pWeapon->SetFallbackPaintKit(567); // Black Laminate
								}
								else if (Skin == 34)
								{
									pWeapon->SetFallbackPaintKit(574); // Autotronic
								}
								else if (Skin == 35)
								{
									pWeapon->SetFallbackPaintKit(580); // Freehand
								}
								else if (Skin == 36)
								{
									pWeapon->SetFallbackPaintKit(852); // 
								}
								else if (Skin == 37)
								{
									pWeapon->SetFallbackPaintKit(853); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(854); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(855); // 
								}
								else if (Skin == 39)
								{
									pWeapon->SetFallbackPaintKit(856); // 
								}
								else if (Skin == 40)
								{
									pWeapon->SetFallbackPaintKit(857); // 
								}
								else if (Skin == 41)
								{
									pWeapon->SetFallbackPaintKit(858); // 
								}
							}
							else if (Model == 7) // Falchion Knife
							{
								int iFalchion = g_pModelInfo->GetModelIndex(SteamStrings("models/weapons/v_knife_falchion_advanced.mdl"));
								pWeapon->SetWeaponModelIndex(iFalchion);
								pWeapon->SetViewModelIndex(iFalchion);
								if (worldmodel)pWeapon->SetWeaponModelIndex(iFalchion + 1);
								pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_KNIFE_FALCHION);
								pWeapon->SetEntityQuality(3);
								g_Menu.Config.killicon_config = SteamStrings("knife_falchion");
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									pWeapon->SetFallbackPaintKit(0); // Forest DDPAT
								}
								else if (Skin == 1)
								{
									pWeapon->SetFallbackPaintKit(12); // Crimson Web
								}
								else if (Skin == 2)
								{
									pWeapon->SetFallbackPaintKit(27); // Bone Mask
								}
								else if (Skin == 3)
								{
									pWeapon->SetFallbackPaintKit(38); // Fade
								}
								else if (Skin == 4)
								{
									pWeapon->SetFallbackPaintKit(40); // Night
								}
								else if (Skin == 5)
								{
									pWeapon->SetFallbackPaintKit(42); // Blue Steel
								}
								else if (Skin == 6)
								{
									pWeapon->SetFallbackPaintKit(43); // Stained
								}
								else if (Skin == 7)
								{
									pWeapon->SetFallbackPaintKit(44); // Case Hardened
								}
								else if (Skin == 8)
								{
									pWeapon->SetFallbackPaintKit(59); // Slaughter
								}
								else if (Skin == 9)
								{
									pWeapon->SetFallbackPaintKit(72); // Safari Mesh
								}
								else if (Skin == 10)
								{
									pWeapon->SetFallbackPaintKit(77); // Boreal Forest
								}
								else if (Skin == 11)
								{
									pWeapon->SetFallbackPaintKit(98); // Ultraviolet
								}
								else if (Skin == 12)
								{
									pWeapon->SetFallbackPaintKit(143); // Urban Masked
								}
								else if (Skin == 13)
								{
									pWeapon->SetFallbackPaintKit(175); // Scorched
								}
								else if (Skin == 14)
								{
									pWeapon->SetFallbackPaintKit(323); // Rust Coat
								}
								else if (Skin == 15)
								{
									pWeapon->SetFallbackPaintKit(409); // Tiger Tooth
								}
								else if (Skin == 16)
								{
									pWeapon->SetFallbackPaintKit(410); // Damascus Steel
								}
								else if (Skin == 17)
								{
									pWeapon->SetFallbackPaintKit(411); // Damascus Steel
								}
								else if (Skin == 18)
								{
									pWeapon->SetFallbackPaintKit(413); // Marble Fade
								}
								else if (Skin == 19)
								{
									pWeapon->SetFallbackPaintKit(414); // Rust Coat
								}
								else if (Skin == 20)
								{
									pWeapon->SetFallbackPaintKit(415); // Doppler Ruby
								}
								else if (Skin == 21)
								{
									pWeapon->SetFallbackPaintKit(416); // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									pWeapon->SetFallbackPaintKit(417); // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									pWeapon->SetFallbackPaintKit(418); // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									pWeapon->SetFallbackPaintKit(419); // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									pWeapon->SetFallbackPaintKit(420); // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									pWeapon->SetFallbackPaintKit(421); // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									pWeapon->SetFallbackPaintKit(569); // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									pWeapon->SetFallbackPaintKit(570); // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									pWeapon->SetFallbackPaintKit(571); // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									pWeapon->SetFallbackPaintKit(558); // Lore
								}
								else if (Skin == 33)
								{
									pWeapon->SetFallbackPaintKit(567); // Black Laminate
								}
								else if (Skin == 34)
								{
									pWeapon->SetFallbackPaintKit(574); // Autotronic
								}
								else if (Skin == 35)
								{
									pWeapon->SetFallbackPaintKit(580); // Freehand
								}
								else if (Skin == 36)
								{
									pWeapon->SetFallbackPaintKit(852); // 
								}
								else if (Skin == 37)
								{
									pWeapon->SetFallbackPaintKit(853); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(854); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(855); // 
								}
								else if (Skin == 39)
								{
									pWeapon->SetFallbackPaintKit(856); // 
								}
								else if (Skin == 40)
								{
									pWeapon->SetFallbackPaintKit(857); // 
								}
								else if (Skin == 41)
								{
									pWeapon->SetFallbackPaintKit(858); // 
								}
							}
							else if (Model == 1) // Flip Knife
							{
								int iFlip = g_pModelInfo->GetModelIndex(SteamStrings("models/weapons/v_knife_flip.mdl"));
								pWeapon->SetWeaponModelIndex(iFlip);
								pWeapon->SetViewModelIndex(iFlip);
								if (worldmodel)pWeapon->SetWeaponModelIndex(iFlip + 1);
								pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_KNIFE_FLIP);
								pWeapon->SetEntityQuality(3);
								g_Menu.Config.killicon_config = SteamStrings("knife_flip");
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									pWeapon->SetFallbackPaintKit(0); // Forest DDPAT
								}
								else if (Skin == 1)
								{
									pWeapon->SetFallbackPaintKit(12); // Crimson Web
								}
								else if (Skin == 2)
								{
									pWeapon->SetFallbackPaintKit(27); // Bone Mask
								}
								else if (Skin == 3)
								{
									pWeapon->SetFallbackPaintKit(38); // Fade
								}
								else if (Skin == 4)
								{
									pWeapon->SetFallbackPaintKit(40); // Night
								}
								else if (Skin == 5)
								{
									pWeapon->SetFallbackPaintKit(42); // Blue Steel
								}
								else if (Skin == 6)
								{
									pWeapon->SetFallbackPaintKit(43); // Stained
								}
								else if (Skin == 7)
								{
									pWeapon->SetFallbackPaintKit(44); // Case Hardened
								}
								else if (Skin == 8)
								{
									pWeapon->SetFallbackPaintKit(59); // Slaughter
								}
								else if (Skin == 9)
								{
									pWeapon->SetFallbackPaintKit(72); // Safari Mesh
								}
								else if (Skin == 10)
								{
									pWeapon->SetFallbackPaintKit(77); // Boreal Forest
								}
								else if (Skin == 11)
								{
									pWeapon->SetFallbackPaintKit(98); // Ultraviolet
								}
								else if (Skin == 12)
								{
									pWeapon->SetFallbackPaintKit(143); // Urban Masked
								}
								else if (Skin == 13)
								{
									pWeapon->SetFallbackPaintKit(175); // Scorched
								}
								else if (Skin == 14)
								{
									pWeapon->SetFallbackPaintKit(323); // Rust Coat
								}
								else if (Skin == 15)
								{
									pWeapon->SetFallbackPaintKit(409); // Tiger Tooth
								}
								else if (Skin == 16)
								{
									pWeapon->SetFallbackPaintKit(410); // Damascus Steel
								}
								else if (Skin == 17)
								{
									pWeapon->SetFallbackPaintKit(411); // Damascus Steel
								}
								else if (Skin == 18)
								{
									pWeapon->SetFallbackPaintKit(413); // Marble Fade
								}
								else if (Skin == 19)
								{
									pWeapon->SetFallbackPaintKit(414); // Rust Coat
								}
								else if (Skin == 20)
								{
									pWeapon->SetFallbackPaintKit(415); // Doppler Ruby
								}
								else if (Skin == 21)
								{
									pWeapon->SetFallbackPaintKit(416); // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									pWeapon->SetFallbackPaintKit(417); // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									pWeapon->SetFallbackPaintKit(418); // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									pWeapon->SetFallbackPaintKit(419); // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									pWeapon->SetFallbackPaintKit(420); // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									pWeapon->SetFallbackPaintKit(421); // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									pWeapon->SetFallbackPaintKit(569); // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									pWeapon->SetFallbackPaintKit(570); // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									pWeapon->SetFallbackPaintKit(571); // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									pWeapon->SetFallbackPaintKit(559); // Lore
								}
								else if (Skin == 33)
								{
									pWeapon->SetFallbackPaintKit(564); // Black Laminate
								}
								else if (Skin == 34)
								{
									pWeapon->SetFallbackPaintKit(574); // Autotronic
								}
								else if (Skin == 35)
								{
									pWeapon->SetFallbackPaintKit(580); // Freehand
								}
								else if (Skin == 36)
								{
									pWeapon->SetFallbackPaintKit(852); // 
								}
								else if (Skin == 37)
								{
									pWeapon->SetFallbackPaintKit(853); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(854); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(855); // 
								}
								else if (Skin == 39)
								{
									pWeapon->SetFallbackPaintKit(856); // 
								}
								else if (Skin == 40)
								{
									pWeapon->SetFallbackPaintKit(857); // 
								}
								else if (Skin == 41)
								{
									pWeapon->SetFallbackPaintKit(858); // 
								}

							}
							else if (Model == 2) // Gut Knife
							{
								int iGut = g_pModelInfo->GetModelIndex(SteamStrings("models/weapons/v_knife_gut.mdl"));
								pWeapon->SetWeaponModelIndex(iGut);
								pWeapon->SetViewModelIndex(iGut);
								if (worldmodel)pWeapon->SetWeaponModelIndex(iGut + 1);
								pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_KNIFE_GUT);
								pWeapon->SetEntityQuality(3);
								g_Menu.Config.killicon_config = SteamStrings("knife_gut");
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									pWeapon->SetFallbackPaintKit(0); // Forest DDPAT
								}
								else if (Skin == 1)
								{
									pWeapon->SetFallbackPaintKit(12); // Crimson Web
								}
								else if (Skin == 2)
								{
									pWeapon->SetFallbackPaintKit(27); // Bone Mask
								}
								else if (Skin == 3)
								{
									pWeapon->SetFallbackPaintKit(38); // Fade
								}
								else if (Skin == 4)
								{
									pWeapon->SetFallbackPaintKit(40); // Night
								}
								else if (Skin == 5)
								{
									pWeapon->SetFallbackPaintKit(42); // Blue Steel
								}
								else if (Skin == 6)
								{
									pWeapon->SetFallbackPaintKit(43); // Stained
								}
								else if (Skin == 7)
								{
									pWeapon->SetFallbackPaintKit(44); // Case Hardened
								}
								else if (Skin == 8)
								{
									pWeapon->SetFallbackPaintKit(59); // Slaughter
								}
								else if (Skin == 9)
								{
									pWeapon->SetFallbackPaintKit(72); // Safari Mesh
								}
								else if (Skin == 10)
								{
									pWeapon->SetFallbackPaintKit(77); // Boreal Forest
								}
								else if (Skin == 11)
								{
									pWeapon->SetFallbackPaintKit(98); // Ultraviolet
								}
								else if (Skin == 12)
								{
									pWeapon->SetFallbackPaintKit(143); // Urban Masked
								}
								else if (Skin == 13)
								{
									pWeapon->SetFallbackPaintKit(175); // Scorched
								}
								else if (Skin == 14)
								{
									pWeapon->SetFallbackPaintKit(323); // Rust Coat
								}
								else if (Skin == 15)
								{
									pWeapon->SetFallbackPaintKit(409); // Tiger Tooth
								}
								else if (Skin == 16)
								{
									pWeapon->SetFallbackPaintKit(410); // Damascus Steel
								}
								else if (Skin == 17)
								{
									pWeapon->SetFallbackPaintKit(411); // Damascus Steel
								}
								else if (Skin == 18)
								{
									pWeapon->SetFallbackPaintKit(413); // Marble Fade
								}
								else if (Skin == 19)
								{
									pWeapon->SetFallbackPaintKit(414); // Rust Coat
								}
								else if (Skin == 20)
								{
									pWeapon->SetFallbackPaintKit(415); // Doppler Ruby
								}
								else if (Skin == 21)
								{
									pWeapon->SetFallbackPaintKit(416); // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									pWeapon->SetFallbackPaintKit(417); // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									pWeapon->SetFallbackPaintKit(418); // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									pWeapon->SetFallbackPaintKit(419); // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									pWeapon->SetFallbackPaintKit(420); // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									pWeapon->SetFallbackPaintKit(421); // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									pWeapon->SetFallbackPaintKit(569); // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									pWeapon->SetFallbackPaintKit(570); // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									pWeapon->SetFallbackPaintKit(571); // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									pWeapon->SetFallbackPaintKit(560); // Lore
								}
								else if (Skin == 33)
								{
									pWeapon->SetFallbackPaintKit(565); // Black Laminate
								}
								else if (Skin == 34)
								{
									pWeapon->SetFallbackPaintKit(575); // Autotronic
								}
								else if (Skin == 35)
								{
									pWeapon->SetFallbackPaintKit(580); // Freehand
								}
								else if (Skin == 36)
								{
									pWeapon->SetFallbackPaintKit(852); // 
								}
								else if (Skin == 37)
								{
									pWeapon->SetFallbackPaintKit(853); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(854); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(855); // 
								}
								else if (Skin == 39)
								{
									pWeapon->SetFallbackPaintKit(856); // 
								}
								else if (Skin == 40)
								{
									pWeapon->SetFallbackPaintKit(857); // 
								}
								else if (Skin == 41)
								{
									pWeapon->SetFallbackPaintKit(858); // 
								}
							}
							else if (Model == 5) // Huntsman Knife
							{
								int iHuntsman = g_pModelInfo->GetModelIndex(SteamStrings("models/weapons/v_knife_tactical.mdl"));
								pWeapon->SetWeaponModelIndex(iHuntsman);
								pWeapon->SetViewModelIndex(iHuntsman);
								if (worldmodel)pWeapon->SetWeaponModelIndex(iHuntsman + 1);
								pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL);
								pWeapon->SetEntityQuality(3);
								g_Menu.Config.killicon_config = SteamStrings("knife_tactical");
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									pWeapon->SetFallbackPaintKit(0); // Forest DDPAT
								}
								else if (Skin == 1)
								{
									pWeapon->SetFallbackPaintKit(12); // Crimson Web
								}
								else if (Skin == 2)
								{
									pWeapon->SetFallbackPaintKit(27); // Bone Mask
								}
								else if (Skin == 3)
								{
									pWeapon->SetFallbackPaintKit(38); // Fade
								}
								else if (Skin == 4)
								{
									pWeapon->SetFallbackPaintKit(40); // Night
								}
								else if (Skin == 5)
								{
									pWeapon->SetFallbackPaintKit(42); // Blue Steel
								}
								else if (Skin == 6)
								{
									pWeapon->SetFallbackPaintKit(43); // Stained
								}
								else if (Skin == 7)
								{
									pWeapon->SetFallbackPaintKit(44); // Case Hardened
								}
								else if (Skin == 8)
								{
									pWeapon->SetFallbackPaintKit(59); // Slaughter
								}
								else if (Skin == 9)
								{
									pWeapon->SetFallbackPaintKit(72); // Safari Mesh
								}
								else if (Skin == 10)
								{
									pWeapon->SetFallbackPaintKit(77); // Boreal Forest
								}
								else if (Skin == 11)
								{
									pWeapon->SetFallbackPaintKit(98); // Ultraviolet
								}
								else if (Skin == 12)
								{
									pWeapon->SetFallbackPaintKit(143); // Urban Masked
								}
								else if (Skin == 13)
								{
									pWeapon->SetFallbackPaintKit(175); // Scorched
								}
								else if (Skin == 14)
								{
									pWeapon->SetFallbackPaintKit(323); // Rust Coat
								}
								else if (Skin == 15)
								{
									pWeapon->SetFallbackPaintKit(409); // Tiger Tooth
								}
								else if (Skin == 16)
								{
									pWeapon->SetFallbackPaintKit(410); // Damascus Steel
								}
								else if (Skin == 17)
								{
									pWeapon->SetFallbackPaintKit(411); // Damascus Steel
								}
								else if (Skin == 18)
								{
									pWeapon->SetFallbackPaintKit(413); // Marble Fade
								}
								else if (Skin == 19)
								{
									pWeapon->SetFallbackPaintKit(414); // Rust Coat
								}
								else if (Skin == 20)
								{
									pWeapon->SetFallbackPaintKit(415); // Doppler Ruby
								}
								else if (Skin == 21)
								{
									pWeapon->SetFallbackPaintKit(416); // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									pWeapon->SetFallbackPaintKit(417); // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									pWeapon->SetFallbackPaintKit(418); // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									pWeapon->SetFallbackPaintKit(419); // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									pWeapon->SetFallbackPaintKit(420); // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									pWeapon->SetFallbackPaintKit(421); // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									pWeapon->SetFallbackPaintKit(569); // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									pWeapon->SetFallbackPaintKit(570); // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									pWeapon->SetFallbackPaintKit(571); // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									pWeapon->SetFallbackPaintKit(559); // Lore
								}
								else if (Skin == 33)
								{
									pWeapon->SetFallbackPaintKit(567); // Black Laminate
								}
								else if (Skin == 34)
								{
									pWeapon->SetFallbackPaintKit(574); // Autotronic
								}
								else if (Skin == 35)
								{
									pWeapon->SetFallbackPaintKit(580); // Freehand
								}
								else if (Skin == 36)
								{
									pWeapon->SetFallbackPaintKit(852); // 
								}
								else if (Skin == 37)
								{
									pWeapon->SetFallbackPaintKit(853); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(854); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(855); // 
								}
								else if (Skin == 39)
								{
									pWeapon->SetFallbackPaintKit(856); // 
								}
								else if (Skin == 40)
								{
									pWeapon->SetFallbackPaintKit(857); // 
								}
								else if (Skin == 41)
								{
									pWeapon->SetFallbackPaintKit(858); // 
								}
							}
							else if (Model == 3) // Karambit
							{
								int iKarambit = g_pModelInfo->GetModelIndex(SteamStrings("models/weapons/v_knife_karam.mdl"));
								pWeapon->SetWeaponModelIndex(iKarambit);
								pWeapon->SetViewModelIndex(iKarambit);
								if (worldmodel)pWeapon->SetWeaponModelIndex(iKarambit + 1);
								pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT);
								pWeapon->SetEntityQuality(3);
								g_Menu.Config.killicon_config = SteamStrings("knife_karambit");
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									pWeapon->SetFallbackPaintKit(0); // Forest DDPAT
								}
								else if (Skin == 1)
								{
									pWeapon->SetFallbackPaintKit(12); // Crimson Web
								}
								else if (Skin == 2)
								{
									pWeapon->SetFallbackPaintKit(27); // Bone Mask
								}
								else if (Skin == 3)
								{
									pWeapon->SetFallbackPaintKit(38); // Fade
								}
								else if (Skin == 4)
								{
									pWeapon->SetFallbackPaintKit(40); // Night
								}
								else if (Skin == 5)
								{
									pWeapon->SetFallbackPaintKit(42); // Blue Steel
								}
								else if (Skin == 6)
								{
									pWeapon->SetFallbackPaintKit(43); // Stained
								}
								else if (Skin == 7)
								{
									pWeapon->SetFallbackPaintKit(44); // Case Hardened
								}
								else if (Skin == 8)
								{
									pWeapon->SetFallbackPaintKit(59); // Slaughter
								}
								else if (Skin == 9)
								{
									pWeapon->SetFallbackPaintKit(72); // Safari Mesh
								}
								else if (Skin == 10)
								{
									pWeapon->SetFallbackPaintKit(77); // Boreal Forest
								}
								else if (Skin == 11)
								{
									pWeapon->SetFallbackPaintKit(98); // Ultraviolet
								}
								else if (Skin == 12)
								{
									pWeapon->SetFallbackPaintKit(143); // Urban Masked
								}
								else if (Skin == 13)
								{
									pWeapon->SetFallbackPaintKit(175); // Scorched
								}
								else if (Skin == 14)
								{
									pWeapon->SetFallbackPaintKit(323); // Rust Coat
								}
								else if (Skin == 15)
								{
									pWeapon->SetFallbackPaintKit(409); // Tiger Tooth
								}
								else if (Skin == 16)
								{
									pWeapon->SetFallbackPaintKit(410); // Damascus Steel
								}
								else if (Skin == 17)
								{
									pWeapon->SetFallbackPaintKit(411); // Damascus Steel
								}
								else if (Skin == 18)
								{
									pWeapon->SetFallbackPaintKit(413); // Marble Fade
								}
								else if (Skin == 19)
								{
									pWeapon->SetFallbackPaintKit(414); // Rust Coat
								}
								else if (Skin == 20)
								{
									pWeapon->SetFallbackPaintKit(415); // Doppler Ruby
								}
								else if (Skin == 21)
								{
									pWeapon->SetFallbackPaintKit(416); // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									pWeapon->SetFallbackPaintKit(417); // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									pWeapon->SetFallbackPaintKit(418); // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									pWeapon->SetFallbackPaintKit(419); // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									pWeapon->SetFallbackPaintKit(420); // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									pWeapon->SetFallbackPaintKit(421); // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									pWeapon->SetFallbackPaintKit(570); // Doppler Phase 4
								}
								else if (Skin == 28)
								{
									pWeapon->SetFallbackPaintKit(568); // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									pWeapon->SetFallbackPaintKit(569); // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									pWeapon->SetFallbackPaintKit(570); // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									pWeapon->SetFallbackPaintKit(571); // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									pWeapon->SetFallbackPaintKit(561); // Lore
								}
								else if (Skin == 33)
								{
									pWeapon->SetFallbackPaintKit(566); // Black Laminate
								}
								else if (Skin == 34)
								{
									pWeapon->SetFallbackPaintKit(576); // Autotronic
								}
								else if (Skin == 35)
								{
									pWeapon->SetFallbackPaintKit(582); // Freehand
								}
								else if (Skin == 36)
								{
									pWeapon->SetFallbackPaintKit(852); // 
								}
								else if (Skin == 37)
								{
									pWeapon->SetFallbackPaintKit(853); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(854); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(855); // 
								}
								else if (Skin == 39)
								{
									pWeapon->SetFallbackPaintKit(856); // 
								}
								else if (Skin == 40)
								{
									pWeapon->SetFallbackPaintKit(857); // 
								}
								else if (Skin == 41)
								{
									pWeapon->SetFallbackPaintKit(858); // 
								}

							}
							else if (Model == 4) // M9 Bayonet
							{
								auto iM9Bayonet = g_pModelInfo->GetModelIndex(SteamStrings("models/weapons/v_knife_m9_bay.mdl"));
								pWeapon->SetWeaponModelIndex(iM9Bayonet);
								pWeapon->SetViewModelIndex(iM9Bayonet);
								if (worldmodel)pWeapon->SetWeaponModelIndex(iM9Bayonet + 1);
								pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET);
								pWeapon->SetEntityQuality(3);
								g_Menu.Config.killicon_config = SteamStrings("knife_m9_bayonet");
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									pWeapon->SetFallbackPaintKit(0); // Forest DDPAT
								}
								else if (Skin == 1)
								{
									pWeapon->SetFallbackPaintKit(12); // Crimson Web
								}
								else if (Skin == 2)
								{
									pWeapon->SetFallbackPaintKit(27); // Bone Mask
								}
								else if (Skin == 3)
								{
									pWeapon->SetFallbackPaintKit(38); // Fade
								}
								else if (Skin == 4)
								{
									pWeapon->SetFallbackPaintKit(40); // Night
								}
								else if (Skin == 5)
								{
									pWeapon->SetFallbackPaintKit(42); // Blue Steel
								}
								else if (Skin == 6)
								{
									pWeapon->SetFallbackPaintKit(43); // Stained
								}
								else if (Skin == 7)
								{
									pWeapon->SetFallbackPaintKit(44); // Case Hardened
								}
								else if (Skin == 8)
								{
									pWeapon->SetFallbackPaintKit(59); // Slaughter
								}
								else if (Skin == 9)
								{
									pWeapon->SetFallbackPaintKit(72); // Safari Mesh
								}
								else if (Skin == 10)
								{
									pWeapon->SetFallbackPaintKit(77); // Boreal Forest
								}
								else if (Skin == 11)
								{
									pWeapon->SetFallbackPaintKit(98); // Ultraviolet
								}
								else if (Skin == 12)
								{
									pWeapon->SetFallbackPaintKit(143); // Urban Masked
								}
								else if (Skin == 13)
								{
									pWeapon->SetFallbackPaintKit(175); // Scorched
								}
								else if (Skin == 14)
								{
									pWeapon->SetFallbackPaintKit(323); // Rust Coat
								}
								else if (Skin == 15)
								{
									pWeapon->SetFallbackPaintKit(409); // Tiger Tooth
								}
								else if (Skin == 16)
								{
									pWeapon->SetFallbackPaintKit(410); // Damascus Steel
								}
								else if (Skin == 17)
								{
									pWeapon->SetFallbackPaintKit(411); // Damascus Steel
								}
								else if (Skin == 18)
								{
									pWeapon->SetFallbackPaintKit(413); // Marble Fade
								}
								else if (Skin == 19)
								{
									pWeapon->SetFallbackPaintKit(414); // Rust Coat
								}
								else if (Skin == 20)
								{
									pWeapon->SetFallbackPaintKit(415); // Doppler Ruby
								}
								else if (Skin == 21)
								{
									pWeapon->SetFallbackPaintKit(416); // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									pWeapon->SetFallbackPaintKit(417); // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									pWeapon->SetFallbackPaintKit(418); // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									pWeapon->SetFallbackPaintKit(419); // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									pWeapon->SetFallbackPaintKit(420); // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									pWeapon->SetFallbackPaintKit(421); // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									pWeapon->SetFallbackPaintKit(570); // Doppler Phase 4
								}
								else if (Skin == 28)
								{
									pWeapon->SetFallbackPaintKit(568); // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									pWeapon->SetFallbackPaintKit(569); // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									pWeapon->SetFallbackPaintKit(570); // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									pWeapon->SetFallbackPaintKit(571); // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									pWeapon->SetFallbackPaintKit(562); // Lore
								}
								else if (Skin == 33)
								{
									pWeapon->SetFallbackPaintKit(567); // Black Laminate
								}
								else if (Skin == 34)
								{
									pWeapon->SetFallbackPaintKit(577); // Autotronic
								}
								else if (Skin == 35)
								{
									pWeapon->SetFallbackPaintKit(581); // Freehand
								}
								else if (Skin == 36)
								{
									pWeapon->SetFallbackPaintKit(852); // 
								}
								else if (Skin == 37)
								{
									pWeapon->SetFallbackPaintKit(853); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(854); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(855); // 
								}
								else if (Skin == 39)
								{
									pWeapon->SetFallbackPaintKit(856); // 
								}
								else if (Skin == 40)
								{
									pWeapon->SetFallbackPaintKit(857); // 
								}
								else if (Skin == 41)
								{
									pWeapon->SetFallbackPaintKit(858); // 
								}
							}
							else if (Model == 8)
							{
								int iDagger = g_pModelInfo->GetModelIndex(SteamStrings("models/weapons/v_knife_push.mdl"));
								pWeapon->SetWeaponModelIndex(iDagger);
								pWeapon->SetViewModelIndex(iDagger);
								if (worldmodel)pWeapon->SetWeaponModelIndex(iDagger + 1);
								pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_KNIFE_PUSH);
								pWeapon->SetEntityQuality(3);
								g_Menu.Config.killicon_config = SteamStrings("knife_push");
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									pWeapon->SetFallbackPaintKit(5); // Forest DDPAT
								}
								else if (Skin == 1)
								{
									pWeapon->SetFallbackPaintKit(12); // Crimson Web
								}
								else if (Skin == 2)
								{
									pWeapon->SetFallbackPaintKit(27); // Bone Mask
								}
								else if (Skin == 3)
								{
									pWeapon->SetFallbackPaintKit(38); // Fade
								}
								else if (Skin == 4)
								{
									pWeapon->SetFallbackPaintKit(40); // Night
								}
								else if (Skin == 5)
								{
									pWeapon->SetFallbackPaintKit(42); // Blue Steel
								}
								else if (Skin == 6)
								{
									pWeapon->SetFallbackPaintKit(43); // Stained
								}
								else if (Skin == 7)
								{
									pWeapon->SetFallbackPaintKit(44); // Case Hardened
								}
								else if (Skin == 8)
								{
									pWeapon->SetFallbackPaintKit(59); // Slaughter
								}
								else if (Skin == 9)
								{
									pWeapon->SetFallbackPaintKit(72); // Safari Mesh
								}
								else if (Skin == 10)
								{
									pWeapon->SetFallbackPaintKit(77); // Boreal Forest
								}
								else if (Skin == 11)
								{
									pWeapon->SetFallbackPaintKit(98); // Ultraviolet
								}
								else if (Skin == 12)
								{
									pWeapon->SetFallbackPaintKit(143); // Urban Masked
								}
								else if (Skin == 13)
								{
									pWeapon->SetFallbackPaintKit(175); // Scorched
								}
								else if (Skin == 14)
								{
									pWeapon->SetFallbackPaintKit(323); // Rust Coat
								}
								else if (Skin == 15)
								{
									pWeapon->SetFallbackPaintKit(409); // Tiger Tooth
								}
								else if (Skin == 16)
								{
									pWeapon->SetFallbackPaintKit(410); // Damascus Steel
								}
								else if (Skin == 17)
								{
									pWeapon->SetFallbackPaintKit(411); // Damascus Steel
								}
								else if (Skin == 18)
								{
									pWeapon->SetFallbackPaintKit(413); // Marble Fade
								}
								else if (Skin == 19)
								{
									pWeapon->SetFallbackPaintKit(414); // Rust Coat
								}
								else if (Skin == 20)
								{
									pWeapon->SetFallbackPaintKit(415); // Doppler Ruby
								}
								else if (Skin == 21)
								{
									pWeapon->SetFallbackPaintKit(416); // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									pWeapon->SetFallbackPaintKit(417); // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									pWeapon->SetFallbackPaintKit(418); // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									pWeapon->SetFallbackPaintKit(419); // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									pWeapon->SetFallbackPaintKit(420); // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									pWeapon->SetFallbackPaintKit(421); // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									pWeapon->SetFallbackPaintKit(569); // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									pWeapon->SetFallbackPaintKit(570); // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									pWeapon->SetFallbackPaintKit(571); // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									pWeapon->SetFallbackPaintKit(561); // Lore
								}
								else if (Skin == 33)
								{
									pWeapon->SetFallbackPaintKit(567); // Black Laminate
								}
								else if (Skin == 34)
								{
									pWeapon->SetFallbackPaintKit(574); // Autotronic
								}
								else if (Skin == 35)
								{
									pWeapon->SetFallbackPaintKit(580); // Freehand
								}
								else if (Skin == 36)
								{
									pWeapon->SetFallbackPaintKit(852); // 
								}
								else if (Skin == 37)
								{
									pWeapon->SetFallbackPaintKit(853); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(854); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(855); // 
								}
								else if (Skin == 39)
								{
									pWeapon->SetFallbackPaintKit(856); // 
								}
								else if (Skin == 40)
								{
									pWeapon->SetFallbackPaintKit(857); // 
								}
								else if (Skin == 41)
								{
									pWeapon->SetFallbackPaintKit(858); // 
								}
							}
							else if (Model == 10)
							{
								int Navaja = g_pModelInfo->GetModelIndex(SteamStrings("models/weapons/v_knife_gypsy_jackknife.mdl"));
								pWeapon->SetWeaponModelIndex(Navaja);
								pWeapon->SetViewModelIndex(Navaja);
								if (worldmodel)pWeapon->SetWeaponModelIndex(Navaja + 1);
								pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_KNIFE_GYPSY_JACKKNIFE);
								pWeapon->SetEntityQuality(3);
								g_Menu.Config.killicon_config = SteamStrings("knife_gypsy_jackknife");
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									pWeapon->SetFallbackPaintKit(5); // Forest DDPAT
								}
								else if (Skin == 1)
								{
									pWeapon->SetFallbackPaintKit(12); // Crimson Web
								}
								else if (Skin == 2)
								{
									pWeapon->SetFallbackPaintKit(27); // Bone Mask
								}
								else if (Skin == 3)
								{
									pWeapon->SetFallbackPaintKit(38); // Fade
								}
								else if (Skin == 4)
								{
									pWeapon->SetFallbackPaintKit(40); // Night
								}
								else if (Skin == 5)
								{
									pWeapon->SetFallbackPaintKit(42); // Blue Steel
								}
								else if (Skin == 6)
								{
									pWeapon->SetFallbackPaintKit(43); // Stained
								}
								else if (Skin == 7)
								{
									pWeapon->SetFallbackPaintKit(44); // Case Hardened
								}
								else if (Skin == 8)
								{
									pWeapon->SetFallbackPaintKit(59); // Slaughter
								}
								else if (Skin == 9)
								{
									pWeapon->SetFallbackPaintKit(72); // Safari Mesh
								}
								else if (Skin == 10)
								{
									pWeapon->SetFallbackPaintKit(77); // Boreal Forest
								}
								else if (Skin == 11)
								{
									pWeapon->SetFallbackPaintKit(98); // Ultraviolet
								}
								else if (Skin == 12)
								{
									pWeapon->SetFallbackPaintKit(143); // Urban Masked
								}
								else if (Skin == 13)
								{
									pWeapon->SetFallbackPaintKit(175); // Scorched
								}
								else if (Skin == 14)
								{
									pWeapon->SetFallbackPaintKit(323); // Rust Coat
								}
								else if (Skin == 15)
								{
									pWeapon->SetFallbackPaintKit(409); // Tiger Tooth
								}
								else if (Skin == 16)
								{
									pWeapon->SetFallbackPaintKit(410); // Damascus Steel
								}
								else if (Skin == 17)
								{
									pWeapon->SetFallbackPaintKit(411); // Damascus Steel
								}
								else if (Skin == 18)
								{
									pWeapon->SetFallbackPaintKit(413); // Marble Fade
								}
								else if (Skin == 19)
								{
									pWeapon->SetFallbackPaintKit(414); // Rust Coat
								}
								else if (Skin == 20)
								{
									pWeapon->SetFallbackPaintKit(415); // Doppler Ruby
								}
								else if (Skin == 21)
								{
									pWeapon->SetFallbackPaintKit(416); // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									pWeapon->SetFallbackPaintKit(417); // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									pWeapon->SetFallbackPaintKit(418); // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									pWeapon->SetFallbackPaintKit(419); // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									pWeapon->SetFallbackPaintKit(420); // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									pWeapon->SetFallbackPaintKit(421); // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									pWeapon->SetFallbackPaintKit(569); // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									pWeapon->SetFallbackPaintKit(570); // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									pWeapon->SetFallbackPaintKit(571); // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									pWeapon->SetFallbackPaintKit(561); // Lore
								}
								else if (Skin == 33)
								{
									pWeapon->SetFallbackPaintKit(567); // Black Laminate
								}
								else if (Skin == 34)
								{
									pWeapon->SetFallbackPaintKit(574); // Autotronic
								}
								else if (Skin == 35)
								{
									pWeapon->SetFallbackPaintKit(580); // Freehand
								}
								else if (Skin == 36)
								{
									pWeapon->SetFallbackPaintKit(852); // 
								}
								else if (Skin == 37)
								{
									pWeapon->SetFallbackPaintKit(853); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(854); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(855); // 
								}
								else if (Skin == 39)
								{
									pWeapon->SetFallbackPaintKit(856); // 
								}
								else if (Skin == 40)
								{
									pWeapon->SetFallbackPaintKit(857); // 
								}
								else if (Skin == 41)
								{
									pWeapon->SetFallbackPaintKit(858); // 
								}
							}
							else if (Model == 11) // Shadow Daggers
							{
								int Stiletto = g_pModelInfo->GetModelIndex(SteamStrings("models/weapons/v_knife_stiletto.mdl"));
								pWeapon->SetWeaponModelIndex(Stiletto);
								pWeapon->SetViewModelIndex(Stiletto);
								if (worldmodel)pWeapon->SetWeaponModelIndex(Stiletto + 1);
								pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_KNIFE_STILETTO);
								pWeapon->SetEntityQuality(3);
								g_Menu.Config.killicon_config = SteamStrings("knife_stiletto");
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									pWeapon->SetFallbackPaintKit(5); // Forest DDPAT
								}
								else if (Skin == 1)
								{
									pWeapon->SetFallbackPaintKit(12); // Crimson Web
								}
								else if (Skin == 2)
								{
									pWeapon->SetFallbackPaintKit(27); // Bone Mask
								}
								else if (Skin == 3)
								{
									pWeapon->SetFallbackPaintKit(38); // Fade
								}
								else if (Skin == 4)
								{
									pWeapon->SetFallbackPaintKit(40); // Night
								}
								else if (Skin == 5)
								{
									pWeapon->SetFallbackPaintKit(42); // Blue Steel
								}
								else if (Skin == 6)
								{
									pWeapon->SetFallbackPaintKit(43); // Stained
								}
								else if (Skin == 7)
								{
									pWeapon->SetFallbackPaintKit(44); // Case Hardened
								}
								else if (Skin == 8)
								{
									pWeapon->SetFallbackPaintKit(59); // Slaughter
								}
								else if (Skin == 9)
								{
									pWeapon->SetFallbackPaintKit(72); // Safari Mesh
								}
								else if (Skin == 10)
								{
									pWeapon->SetFallbackPaintKit(77); // Boreal Forest
								}
								else if (Skin == 11)
								{
									pWeapon->SetFallbackPaintKit(98); // Ultraviolet
								}
								else if (Skin == 12)
								{
									pWeapon->SetFallbackPaintKit(143); // Urban Masked
								}
								else if (Skin == 13)
								{
									pWeapon->SetFallbackPaintKit(175); // Scorched
								}
								else if (Skin == 14)
								{
									pWeapon->SetFallbackPaintKit(323); // Rust Coat
								}
								else if (Skin == 15)
								{
									pWeapon->SetFallbackPaintKit(409); // Tiger Tooth
								}
								else if (Skin == 16)
								{
									pWeapon->SetFallbackPaintKit(410); // Damascus Steel
								}
								else if (Skin == 17)
								{
									pWeapon->SetFallbackPaintKit(411); // Damascus Steel
								}
								else if (Skin == 18)
								{
									pWeapon->SetFallbackPaintKit(413); // Marble Fade
								}
								else if (Skin == 19)
								{
									pWeapon->SetFallbackPaintKit(414); // Rust Coat
								}
								else if (Skin == 20)
								{
									pWeapon->SetFallbackPaintKit(415); // Doppler Ruby
								}
								else if (Skin == 21)
								{
									pWeapon->SetFallbackPaintKit(416); // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									pWeapon->SetFallbackPaintKit(417); // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									pWeapon->SetFallbackPaintKit(418); // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									pWeapon->SetFallbackPaintKit(419); // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									pWeapon->SetFallbackPaintKit(420); // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									pWeapon->SetFallbackPaintKit(421); // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									pWeapon->SetFallbackPaintKit(569); // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									pWeapon->SetFallbackPaintKit(570); // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									pWeapon->SetFallbackPaintKit(571); // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									pWeapon->SetFallbackPaintKit(561); // Lore
								}
								else if (Skin == 33)
								{
									pWeapon->SetFallbackPaintKit(567); // Black Laminate
								}
								else if (Skin == 34)
								{
									pWeapon->SetFallbackPaintKit(574); // Autotronic
								}
								else if (Skin == 35)
								{
									pWeapon->SetFallbackPaintKit(580); // Freehand
								}
								else if (Skin == 36)
								{
									pWeapon->SetFallbackPaintKit(852); // 
								}
								else if (Skin == 37)
								{
									pWeapon->SetFallbackPaintKit(853); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(854); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(855); // 
								}
								else if (Skin == 39)
								{
									pWeapon->SetFallbackPaintKit(856); // 
								}
								else if (Skin == 40)
								{
									pWeapon->SetFallbackPaintKit(857); // 
								}
								else if (Skin == 41)
								{
									pWeapon->SetFallbackPaintKit(858); // 
								}
							}
							else if (Model == 12)
							{
								int Ursus = g_pModelInfo->GetModelIndex(SteamStrings("models/weapons/v_knife_ursus.mdl"));
								pWeapon->SetWeaponModelIndex(Ursus);
								pWeapon->SetViewModelIndex(Ursus);
								if (worldmodel)pWeapon->SetWeaponModelIndex(Ursus + 1);
								pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_KNIFE_URSUS);
								pWeapon->SetEntityQuality(3);
								g_Menu.Config.killicon_config = SteamStrings("knife_ursus");
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									pWeapon->SetFallbackPaintKit(5); // Forest DDPAT
								}
								else if (Skin == 1)
								{
									pWeapon->SetFallbackPaintKit(12); // Crimson Web
								}
								else if (Skin == 2)
								{
									pWeapon->SetFallbackPaintKit(27); // Bone Mask
								}
								else if (Skin == 3)
								{
									pWeapon->SetFallbackPaintKit(38); // Fade
								}
								else if (Skin == 4)
								{
									pWeapon->SetFallbackPaintKit(40); // Night
								}
								else if (Skin == 5)
								{
									pWeapon->SetFallbackPaintKit(42); // Blue Steel
								}
								else if (Skin == 6)
								{
									pWeapon->SetFallbackPaintKit(43); // Stained
								}
								else if (Skin == 7)
								{
									pWeapon->SetFallbackPaintKit(44); // Case Hardened
								}
								else if (Skin == 8)
								{
									pWeapon->SetFallbackPaintKit(59); // Slaughter
								}
								else if (Skin == 9)
								{
									pWeapon->SetFallbackPaintKit(72); // Safari Mesh
								}
								else if (Skin == 10)
								{
									pWeapon->SetFallbackPaintKit(77); // Boreal Forest
								}
								else if (Skin == 11)
								{
									pWeapon->SetFallbackPaintKit(98); // Ultraviolet
								}
								else if (Skin == 12)
								{
									pWeapon->SetFallbackPaintKit(143); // Urban Masked
								}
								else if (Skin == 13)
								{
									pWeapon->SetFallbackPaintKit(175); // Scorched
								}
								else if (Skin == 14)
								{
									pWeapon->SetFallbackPaintKit(323); // Rust Coat
								}
								else if (Skin == 15)
								{
									pWeapon->SetFallbackPaintKit(409); // Tiger Tooth
								}
								else if (Skin == 16)
								{
									pWeapon->SetFallbackPaintKit(410); // Damascus Steel
								}
								else if (Skin == 17)
								{
									pWeapon->SetFallbackPaintKit(411); // Damascus Steel
								}
								else if (Skin == 18)
								{
									pWeapon->SetFallbackPaintKit(413); // Marble Fade
								}
								else if (Skin == 19)
								{
									pWeapon->SetFallbackPaintKit(414); // Rust Coat
								}
								else if (Skin == 20)
								{
									pWeapon->SetFallbackPaintKit(415); // Doppler Ruby
								}
								else if (Skin == 21)
								{
									pWeapon->SetFallbackPaintKit(416); // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									pWeapon->SetFallbackPaintKit(417); // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									pWeapon->SetFallbackPaintKit(418); // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									pWeapon->SetFallbackPaintKit(419); // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									pWeapon->SetFallbackPaintKit(420); // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									pWeapon->SetFallbackPaintKit(421); // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									pWeapon->SetFallbackPaintKit(569); // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									pWeapon->SetFallbackPaintKit(570); // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									pWeapon->SetFallbackPaintKit(571); // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									pWeapon->SetFallbackPaintKit(561); // Lore
								}
								else if (Skin == 33)
								{
									pWeapon->SetFallbackPaintKit(567); // Black Laminate
								}
								else if (Skin == 34)
								{
									pWeapon->SetFallbackPaintKit(574); // Autotronic
								}
								else if (Skin == 35)
								{
									pWeapon->SetFallbackPaintKit(580); // Freehand
								}
								else if (Skin == 36)
								{
									pWeapon->SetFallbackPaintKit(852); // 
								}
								else if (Skin == 37)
								{
									pWeapon->SetFallbackPaintKit(853); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(854); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(855); // 
								}
								else if (Skin == 39)
								{
									pWeapon->SetFallbackPaintKit(856); // 
								}
								else if (Skin == 40)
								{
									pWeapon->SetFallbackPaintKit(857); // 
								}
								else if (Skin == 41)
								{
									pWeapon->SetFallbackPaintKit(858); // 
								}
							}
							else if (Model == 13)
							{
								int Talon = g_pModelInfo->GetModelIndex(SteamStrings("models/weapons/v_knife_widowmaker.mdl"));
								pWeapon->SetWeaponModelIndex(Talon);
								pWeapon->SetViewModelIndex(Talon);
								if (worldmodel)pWeapon->SetWeaponModelIndex(Talon + 1);
								pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_KNIFE_WIDOWMAKER);
								pWeapon->SetEntityQuality(3);
								g_Menu.Config.killicon_config = SteamStrings("knife_widowmaker");
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									pWeapon->SetFallbackPaintKit(5); // Forest DDPAT
								}
								else if (Skin == 1)
								{
									pWeapon->SetFallbackPaintKit(12); // Crimson Web
								}
								else if (Skin == 2)
								{
									pWeapon->SetFallbackPaintKit(27); // Bone Mask
								}
								else if (Skin == 3)
								{
									pWeapon->SetFallbackPaintKit(38); // Fade
								}
								else if (Skin == 4)
								{
									pWeapon->SetFallbackPaintKit(40); // Night
								}
								else if (Skin == 5)
								{
									pWeapon->SetFallbackPaintKit(42); // Blue Steel
								}
								else if (Skin == 6)
								{
									pWeapon->SetFallbackPaintKit(43); // Stained
								}
								else if (Skin == 7)
								{
									pWeapon->SetFallbackPaintKit(44); // Case Hardened
								}
								else if (Skin == 8)
								{
									pWeapon->SetFallbackPaintKit(59); // Slaughter
								}
								else if (Skin == 9)
								{
									pWeapon->SetFallbackPaintKit(72); // Safari Mesh
								}
								else if (Skin == 10)
								{
									pWeapon->SetFallbackPaintKit(77); // Boreal Forest
								}
								else if (Skin == 11)
								{
									pWeapon->SetFallbackPaintKit(98); // Ultraviolet
								}
								else if (Skin == 12)
								{
									pWeapon->SetFallbackPaintKit(143); // Urban Masked
								}
								else if (Skin == 13)
								{
									pWeapon->SetFallbackPaintKit(175); // Scorched
								}
								else if (Skin == 14)
								{
									pWeapon->SetFallbackPaintKit(323); // Rust Coat
								}
								else if (Skin == 15)
								{
									pWeapon->SetFallbackPaintKit(409); // Tiger Tooth
								}
								else if (Skin == 16)
								{
									pWeapon->SetFallbackPaintKit(410); // Damascus Steel
								}
								else if (Skin == 17)
								{
									pWeapon->SetFallbackPaintKit(411); // Damascus Steel
								}
								else if (Skin == 18)
								{
									pWeapon->SetFallbackPaintKit(413); // Marble Fade
								}
								else if (Skin == 19)
								{
									pWeapon->SetFallbackPaintKit(414); // Rust Coat
								}
								else if (Skin == 20)
								{
									pWeapon->SetFallbackPaintKit(415); // Doppler Ruby
								}
								else if (Skin == 21)
								{
									pWeapon->SetFallbackPaintKit(416); // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									pWeapon->SetFallbackPaintKit(417); // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									pWeapon->SetFallbackPaintKit(418); // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									pWeapon->SetFallbackPaintKit(419); // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									pWeapon->SetFallbackPaintKit(420); // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									pWeapon->SetFallbackPaintKit(421); // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									pWeapon->SetFallbackPaintKit(569); // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									pWeapon->SetFallbackPaintKit(570); // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									pWeapon->SetFallbackPaintKit(571); // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									pWeapon->SetFallbackPaintKit(561); // Lore
								}
								else if (Skin == 33)
								{
									pWeapon->SetFallbackPaintKit(567); // Black Laminate
								}
								else if (Skin == 34)
								{
									pWeapon->SetFallbackPaintKit(574); // Autotronic
								}
								else if (Skin == 35)
								{
									pWeapon->SetFallbackPaintKit(580); // Freehand
								}
								else if (Skin == 36)
								{
									pWeapon->SetFallbackPaintKit(852); // 
								}
								else if (Skin == 37)
								{
									pWeapon->SetFallbackPaintKit(853); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(854); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(855); // 
								}
								else if (Skin == 39)
								{
									pWeapon->SetFallbackPaintKit(856); // 
								}
								else if (Skin == 40)
								{
									pWeapon->SetFallbackPaintKit(857); // 
								}
								else if (Skin == 41)
								{
									pWeapon->SetFallbackPaintKit(858); // 
								}
							}
							else if (Model == 14)
							{
								int classicKnife = g_pModelInfo->GetModelIndex(SteamStrings("models/weapons/v_knife_css.mdl"));
								pWeapon->SetWeaponModelIndex(classicKnife);
								pWeapon->SetViewModelIndex(classicKnife);
								if (worldmodel)pWeapon->SetWeaponModelIndex(classicKnife + 1);
								pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_KNIFE_CSS);
								pWeapon->SetEntityQuality(3);
								g_Menu.Config.killicon_config = SteamStrings("knife_css");
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									pWeapon->SetFallbackPaintKit(5); // Forest DDPAT
								}
								else if (Skin == 1)
								{
									pWeapon->SetFallbackPaintKit(12); // Crimson Web
								}
								else if (Skin == 2)
								{
									pWeapon->SetFallbackPaintKit(27); // Bone Mask
								}
								else if (Skin == 3)
								{
									pWeapon->SetFallbackPaintKit(38); // Fade
								}
								else if (Skin == 4)
								{
									pWeapon->SetFallbackPaintKit(40); // Night
								}
								else if (Skin == 5)
								{
									pWeapon->SetFallbackPaintKit(42); // Blue Steel
								}
								else if (Skin == 6)
								{
									pWeapon->SetFallbackPaintKit(43); // Stained
								}
								else if (Skin == 7)
								{
									pWeapon->SetFallbackPaintKit(44); // Case Hardened
								}
								else if (Skin == 8)
								{
									pWeapon->SetFallbackPaintKit(59); // Slaughter
								}
								else if (Skin == 9)
								{
									pWeapon->SetFallbackPaintKit(72); // Safari Mesh
								}
								else if (Skin == 10)
								{
									pWeapon->SetFallbackPaintKit(77); // Boreal Forest
								}
								else if (Skin == 11)
								{
									pWeapon->SetFallbackPaintKit(98); // Ultraviolet
								}
								else if (Skin == 12)
								{
									pWeapon->SetFallbackPaintKit(143); // Urban Masked
								}
								else if (Skin == 13)
								{
									pWeapon->SetFallbackPaintKit(175); // Scorched
								}
								else if (Skin == 14)
								{
									pWeapon->SetFallbackPaintKit(323); // Rust Coat
								}
								else if (Skin == 15)
								{
									pWeapon->SetFallbackPaintKit(409); // Tiger Tooth
								}
								else if (Skin == 16)
								{
									pWeapon->SetFallbackPaintKit(410); // Damascus Steel
								}
								else if (Skin == 17)
								{
									pWeapon->SetFallbackPaintKit(411); // Damascus Steel
								}
								else if (Skin == 18)
								{
									pWeapon->SetFallbackPaintKit(413); // Marble Fade
								}
								else if (Skin == 19)
								{
									pWeapon->SetFallbackPaintKit(414); // Rust Coat
								}
								else if (Skin == 20)
								{
									pWeapon->SetFallbackPaintKit(415); // Doppler Ruby
								}
								else if (Skin == 21)
								{
									pWeapon->SetFallbackPaintKit(416); // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									pWeapon->SetFallbackPaintKit(417); // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									pWeapon->SetFallbackPaintKit(418); // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									pWeapon->SetFallbackPaintKit(419); // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									pWeapon->SetFallbackPaintKit(420); // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									pWeapon->SetFallbackPaintKit(421); // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									pWeapon->SetFallbackPaintKit(569); // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									pWeapon->SetFallbackPaintKit(570); // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									pWeapon->SetFallbackPaintKit(571); // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									pWeapon->SetFallbackPaintKit(561); // Lore
								}
								else if (Skin == 33)
								{
									pWeapon->SetFallbackPaintKit(567); // Black Laminate
								}
								else if (Skin == 34)
								{
									pWeapon->SetFallbackPaintKit(574); // Autotronic
								}
								else if (Skin == 35)
								{
									pWeapon->SetFallbackPaintKit(580); // Freehand
								}
								else if (Skin == 36)
								{
									pWeapon->SetFallbackPaintKit(852); // 
								}
								else if (Skin == 37)
								{
									pWeapon->SetFallbackPaintKit(853); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(854); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(855); // 
								}
								else if (Skin == 39)
								{
									pWeapon->SetFallbackPaintKit(856); // 
								}
								else if (Skin == 40)
								{
									pWeapon->SetFallbackPaintKit(857); // 
								}
								else if (Skin == 41)
								{
									pWeapon->SetFallbackPaintKit(858); // 
								}
							}
							else if (Model == 15)
							{
								int cordKnife = g_pModelInfo->GetModelIndex(SteamStrings("models/weapons/v_knife_cord.mdl"));
								pWeapon->SetWeaponModelIndex(cordKnife);
								pWeapon->SetViewModelIndex(cordKnife);
								if (worldmodel)pWeapon->SetWeaponModelIndex(cordKnife + 1);
								pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_KNIFE_CORD);
								pWeapon->SetEntityQuality(3);
								g_Menu.Config.killicon_config = SteamStrings("knife_cord");
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									pWeapon->SetFallbackPaintKit(5); // Forest DDPAT
								}
								else if (Skin == 1)
								{
									pWeapon->SetFallbackPaintKit(12); // Crimson Web
								}
								else if (Skin == 2)
								{
									pWeapon->SetFallbackPaintKit(27); // Bone Mask
								}
								else if (Skin == 3)
								{
									pWeapon->SetFallbackPaintKit(38); // Fade
								}
								else if (Skin == 4)
								{
									pWeapon->SetFallbackPaintKit(40); // Night
								}
								else if (Skin == 5)
								{
									pWeapon->SetFallbackPaintKit(42); // Blue Steel
								}
								else if (Skin == 6)
								{
									pWeapon->SetFallbackPaintKit(43); // Stained
								}
								else if (Skin == 7)
								{
									pWeapon->SetFallbackPaintKit(44); // Case Hardened
								}
								else if (Skin == 8)
								{
									pWeapon->SetFallbackPaintKit(59); // Slaughter
								}
								else if (Skin == 9)
								{
									pWeapon->SetFallbackPaintKit(72); // Safari Mesh
								}
								else if (Skin == 10)
								{
									pWeapon->SetFallbackPaintKit(77); // Boreal Forest
								}
								else if (Skin == 11)
								{
									pWeapon->SetFallbackPaintKit(98); // Ultraviolet
								}
								else if (Skin == 12)
								{
									pWeapon->SetFallbackPaintKit(143); // Urban Masked
								}
								else if (Skin == 13)
								{
									pWeapon->SetFallbackPaintKit(175); // Scorched
								}
								else if (Skin == 14)
								{
									pWeapon->SetFallbackPaintKit(323); // Rust Coat
								}
								else if (Skin == 15)
								{
									pWeapon->SetFallbackPaintKit(409); // Tiger Tooth
								}
								else if (Skin == 16)
								{
									pWeapon->SetFallbackPaintKit(410); // Damascus Steel
								}
								else if (Skin == 17)
								{
									pWeapon->SetFallbackPaintKit(411); // Damascus Steel
								}
								else if (Skin == 18)
								{
									pWeapon->SetFallbackPaintKit(413); // Marble Fade
								}
								else if (Skin == 19)
								{
									pWeapon->SetFallbackPaintKit(414); // Rust Coat
								}
								else if (Skin == 20)
								{
									pWeapon->SetFallbackPaintKit(415); // Doppler Ruby
								}
								else if (Skin == 21)
								{
									pWeapon->SetFallbackPaintKit(416); // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									pWeapon->SetFallbackPaintKit(417); // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									pWeapon->SetFallbackPaintKit(418); // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									pWeapon->SetFallbackPaintKit(419); // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									pWeapon->SetFallbackPaintKit(420); // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									pWeapon->SetFallbackPaintKit(421); // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									pWeapon->SetFallbackPaintKit(569); // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									pWeapon->SetFallbackPaintKit(570); // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									pWeapon->SetFallbackPaintKit(571); // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									pWeapon->SetFallbackPaintKit(561); // Lore
								}
								else if (Skin == 33)
								{
									pWeapon->SetFallbackPaintKit(567); // Black Laminate
								}
								else if (Skin == 34)
								{
									pWeapon->SetFallbackPaintKit(574); // Autotronic
								}
								else if (Skin == 35)
								{
									pWeapon->SetFallbackPaintKit(580); // Freehand
								}
								else if (Skin == 36)
								{
									pWeapon->SetFallbackPaintKit(852); // 
								}
								else if (Skin == 37)
								{
									pWeapon->SetFallbackPaintKit(853); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(854); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(855); // 
								}
								else if (Skin == 39)
								{
									pWeapon->SetFallbackPaintKit(856); // 
								}
								else if (Skin == 40)
								{
									pWeapon->SetFallbackPaintKit(857); // 
								}
								else if (Skin == 41)
								{
									pWeapon->SetFallbackPaintKit(858); // 
								}
							}
							else if (Model == 16)
							{
								int canisKnife = g_pModelInfo->GetModelIndex(SteamStrings("models/weapons/v_knife_canis.mdl"));
								pWeapon->SetWeaponModelIndex(canisKnife);
								pWeapon->SetViewModelIndex(canisKnife);
								if (worldmodel)pWeapon->SetWeaponModelIndex(canisKnife + 1);
								pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_KNIFE_CANIS);
								pWeapon->SetEntityQuality(3);
								g_Menu.Config.killicon_config = SteamStrings("knife_canis");
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									pWeapon->SetFallbackPaintKit(5); // Forest DDPAT
								}
								else if (Skin == 1)
								{
									pWeapon->SetFallbackPaintKit(12); // Crimson Web
								}
								else if (Skin == 2)
								{
									pWeapon->SetFallbackPaintKit(27); // Bone Mask
								}
								else if (Skin == 3)
								{
									pWeapon->SetFallbackPaintKit(38); // Fade
								}
								else if (Skin == 4)
								{
									pWeapon->SetFallbackPaintKit(40); // Night
								}
								else if (Skin == 5)
								{
									pWeapon->SetFallbackPaintKit(42); // Blue Steel
								}
								else if (Skin == 6)
								{
									pWeapon->SetFallbackPaintKit(43); // Stained
								}
								else if (Skin == 7)
								{
									pWeapon->SetFallbackPaintKit(44); // Case Hardened
								}
								else if (Skin == 8)
								{
									pWeapon->SetFallbackPaintKit(59); // Slaughter
								}
								else if (Skin == 9)
								{
									pWeapon->SetFallbackPaintKit(72); // Safari Mesh
								}
								else if (Skin == 10)
								{
									pWeapon->SetFallbackPaintKit(77); // Boreal Forest
								}
								else if (Skin == 11)
								{
									pWeapon->SetFallbackPaintKit(98); // Ultraviolet
								}
								else if (Skin == 12)
								{
									pWeapon->SetFallbackPaintKit(143); // Urban Masked
								}
								else if (Skin == 13)
								{
									pWeapon->SetFallbackPaintKit(175); // Scorched
								}
								else if (Skin == 14)
								{
									pWeapon->SetFallbackPaintKit(323); // Rust Coat
								}
								else if (Skin == 15)
								{
									pWeapon->SetFallbackPaintKit(409); // Tiger Tooth
								}
								else if (Skin == 16)
								{
									pWeapon->SetFallbackPaintKit(410); // Damascus Steel
								}
								else if (Skin == 17)
								{
									pWeapon->SetFallbackPaintKit(411); // Damascus Steel
								}
								else if (Skin == 18)
								{
									pWeapon->SetFallbackPaintKit(413); // Marble Fade
								}
								else if (Skin == 19)
								{
									pWeapon->SetFallbackPaintKit(414); // Rust Coat
								}
								else if (Skin == 20)
								{
									pWeapon->SetFallbackPaintKit(415); // Doppler Ruby
								}
								else if (Skin == 21)
								{
									pWeapon->SetFallbackPaintKit(416); // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									pWeapon->SetFallbackPaintKit(417); // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									pWeapon->SetFallbackPaintKit(418); // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									pWeapon->SetFallbackPaintKit(419); // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									pWeapon->SetFallbackPaintKit(420); // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									pWeapon->SetFallbackPaintKit(421); // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									pWeapon->SetFallbackPaintKit(569); // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									pWeapon->SetFallbackPaintKit(570); // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									pWeapon->SetFallbackPaintKit(571); // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									pWeapon->SetFallbackPaintKit(561); // Lore
								}
								else if (Skin == 33)
								{
									pWeapon->SetFallbackPaintKit(567); // Black Laminate
								}
								else if (Skin == 34)
								{
									pWeapon->SetFallbackPaintKit(574); // Autotronic
								}
								else if (Skin == 35)
								{
									pWeapon->SetFallbackPaintKit(580); // Freehand
								}
								else if (Skin == 36)
								{
									pWeapon->SetFallbackPaintKit(852); // 
								}
								else if (Skin == 37)
								{
									pWeapon->SetFallbackPaintKit(853); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(854); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(855); // 
								}
								else if (Skin == 39)
								{
									pWeapon->SetFallbackPaintKit(856); // 
								}
								else if (Skin == 40)
								{
									pWeapon->SetFallbackPaintKit(857); // 
								}
								else if (Skin == 41)
								{
									pWeapon->SetFallbackPaintKit(858); // 
								}
							}
							else if (Model == 17)
							{
								int skellyKnife = g_pModelInfo->GetModelIndex(SteamStrings("models/weapons/v_knife_skeleton.mdl"));
								pWeapon->SetWeaponModelIndex(skellyKnife);
								pWeapon->SetViewModelIndex(skellyKnife);
								if (worldmodel)pWeapon->SetWeaponModelIndex(skellyKnife + 1);
								pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_KNIFE_SKELETON);
								pWeapon->SetEntityQuality(3);
								g_Menu.Config.killicon_config = SteamStrings("knife_skeleton");
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									pWeapon->SetFallbackPaintKit(5); // Forest DDPAT
								}
								else if (Skin == 1)
								{
									pWeapon->SetFallbackPaintKit(12); // Crimson Web
								}
								else if (Skin == 2)
								{
									pWeapon->SetFallbackPaintKit(27); // Bone Mask
								}
								else if (Skin == 3)
								{
									pWeapon->SetFallbackPaintKit(38); // Fade
								}
								else if (Skin == 4)
								{
									pWeapon->SetFallbackPaintKit(40); // Night
								}
								else if (Skin == 5)
								{
									pWeapon->SetFallbackPaintKit(42); // Blue Steel
								}
								else if (Skin == 6)
								{
									pWeapon->SetFallbackPaintKit(43); // Stained
								}
								else if (Skin == 7)
								{
									pWeapon->SetFallbackPaintKit(44); // Case Hardened
								}
								else if (Skin == 8)
								{
									pWeapon->SetFallbackPaintKit(59); // Slaughter
								}
								else if (Skin == 9)
								{
									pWeapon->SetFallbackPaintKit(72); // Safari Mesh
								}
								else if (Skin == 10)
								{
									pWeapon->SetFallbackPaintKit(77); // Boreal Forest
								}
								else if (Skin == 11)
								{
									pWeapon->SetFallbackPaintKit(98); // Ultraviolet
								}
								else if (Skin == 12)
								{
									pWeapon->SetFallbackPaintKit(143); // Urban Masked
								}
								else if (Skin == 13)
								{
									pWeapon->SetFallbackPaintKit(175); // Scorched
								}
								else if (Skin == 14)
								{
									pWeapon->SetFallbackPaintKit(323); // Rust Coat
								}
								else if (Skin == 15)
								{
									pWeapon->SetFallbackPaintKit(409); // Tiger Tooth
								}
								else if (Skin == 16)
								{
									pWeapon->SetFallbackPaintKit(410); // Damascus Steel
								}
								else if (Skin == 17)
								{
									pWeapon->SetFallbackPaintKit(411); // Damascus Steel
								}
								else if (Skin == 18)
								{
									pWeapon->SetFallbackPaintKit(413); // Marble Fade
								}
								else if (Skin == 19)
								{
									pWeapon->SetFallbackPaintKit(414); // Rust Coat
								}
								else if (Skin == 20)
								{
									pWeapon->SetFallbackPaintKit(415); // Doppler Ruby
								}
								else if (Skin == 21)
								{
									pWeapon->SetFallbackPaintKit(416); // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									pWeapon->SetFallbackPaintKit(417); // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									pWeapon->SetFallbackPaintKit(418); // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									pWeapon->SetFallbackPaintKit(419); // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									pWeapon->SetFallbackPaintKit(420); // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									pWeapon->SetFallbackPaintKit(421); // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									pWeapon->SetFallbackPaintKit(569); // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									pWeapon->SetFallbackPaintKit(570); // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									pWeapon->SetFallbackPaintKit(571); // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									pWeapon->SetFallbackPaintKit(561); // Lore
								}
								else if (Skin == 33)
								{
									pWeapon->SetFallbackPaintKit(567); // Black Laminate
								}
								else if (Skin == 34)
								{
									pWeapon->SetFallbackPaintKit(574); // Autotronic
								}
								else if (Skin == 35)
								{
									pWeapon->SetFallbackPaintKit(580); // Freehand
								}
								else if (Skin == 36)
								{
									pWeapon->SetFallbackPaintKit(852); // 
								}
								else if (Skin == 37)
								{
									pWeapon->SetFallbackPaintKit(853); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(854); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(855); // 
								}
								else if (Skin == 39)
								{
									pWeapon->SetFallbackPaintKit(856); // 
								}
								else if (Skin == 40)
								{
									pWeapon->SetFallbackPaintKit(857); // 
								}
								else if (Skin == 41)
								{
									pWeapon->SetFallbackPaintKit(858); // 
								}
							}
							else if (Model == 18)
							{
								int nomadKnife = g_pModelInfo->GetModelIndex(SteamStrings("models/weapons/v_knife_outdoor.mdl"));
								pWeapon->SetWeaponModelIndex(nomadKnife);
								pWeapon->SetViewModelIndex(nomadKnife);
								if (worldmodel)pWeapon->SetWeaponModelIndex(nomadKnife + 1);
								pWeapon->SetItemDefinitionIndex(ItemDefinitionIndex::WEAPON_KNIFE_OUTDOOR);
								pWeapon->SetEntityQuality(3);
								g_Menu.Config.killicon_config = SteamStrings("knife_outoor");
								int Skin = g_Menu.Config.KnifeSkin;
								if (Skin == 0)
								{
									pWeapon->SetFallbackPaintKit(5); // Forest DDPAT
								}
								else if (Skin == 1)
								{
									pWeapon->SetFallbackPaintKit(12); // Crimson Web
								}
								else if (Skin == 2)
								{
									pWeapon->SetFallbackPaintKit(27); // Bone Mask
								}
								else if (Skin == 3)
								{
									pWeapon->SetFallbackPaintKit(38); // Fade
								}
								else if (Skin == 4)
								{
									pWeapon->SetFallbackPaintKit(40); // Night
								}
								else if (Skin == 5)
								{
									pWeapon->SetFallbackPaintKit(42); // Blue Steel
								}
								else if (Skin == 6)
								{
									pWeapon->SetFallbackPaintKit(43); // Stained
								}
								else if (Skin == 7)
								{
									pWeapon->SetFallbackPaintKit(44); // Case Hardened
								}
								else if (Skin == 8)
								{
									pWeapon->SetFallbackPaintKit(59); // Slaughter
								}
								else if (Skin == 9)
								{
									pWeapon->SetFallbackPaintKit(72); // Safari Mesh
								}
								else if (Skin == 10)
								{
									pWeapon->SetFallbackPaintKit(77); // Boreal Forest
								}
								else if (Skin == 11)
								{
									pWeapon->SetFallbackPaintKit(98); // Ultraviolet
								}
								else if (Skin == 12)
								{
									pWeapon->SetFallbackPaintKit(143); // Urban Masked
								}
								else if (Skin == 13)
								{
									pWeapon->SetFallbackPaintKit(175); // Scorched
								}
								else if (Skin == 14)
								{
									pWeapon->SetFallbackPaintKit(323); // Rust Coat
								}
								else if (Skin == 15)
								{
									pWeapon->SetFallbackPaintKit(409); // Tiger Tooth
								}
								else if (Skin == 16)
								{
									pWeapon->SetFallbackPaintKit(410); // Damascus Steel
								}
								else if (Skin == 17)
								{
									pWeapon->SetFallbackPaintKit(411); // Damascus Steel
								}
								else if (Skin == 18)
								{
									pWeapon->SetFallbackPaintKit(413); // Marble Fade
								}
								else if (Skin == 19)
								{
									pWeapon->SetFallbackPaintKit(414); // Rust Coat
								}
								else if (Skin == 20)
								{
									pWeapon->SetFallbackPaintKit(415); // Doppler Ruby
								}
								else if (Skin == 21)
								{
									pWeapon->SetFallbackPaintKit(416); // Doppler Sapphire
								}
								else if (Skin == 22)
								{
									pWeapon->SetFallbackPaintKit(417); // Doppler Blackpearl
								}
								else if (Skin == 23)
								{
									pWeapon->SetFallbackPaintKit(418); // Doppler Phase 1
								}
								else if (Skin == 24)
								{
									pWeapon->SetFallbackPaintKit(419); // Doppler Phase 2
								}
								else if (Skin == 25)
								{
									pWeapon->SetFallbackPaintKit(420); // Doppler Phase 3
								}
								else if (Skin == 26)
								{
									pWeapon->SetFallbackPaintKit(421); // Doppler Phase 4
								}
								else if (Skin == 27)
								{
									pWeapon->SetFallbackPaintKit(569); // Gamma Doppler Phase1
								}
								else if (Skin == 28)
								{
									pWeapon->SetFallbackPaintKit(570); // Gamma Doppler Phase2
								}
								else if (Skin == 29)
								{
									pWeapon->SetFallbackPaintKit(571); // Gamma Doppler Phase3
								}
								else if (Skin == 30)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Phase4
								}
								else if (Skin == 31)
								{
									pWeapon->SetFallbackPaintKit(568); // Gamma Doppler Emerald
								}
								else if (Skin == 32)
								{
									pWeapon->SetFallbackPaintKit(561); // Lore
								}
								else if (Skin == 33)
								{
									pWeapon->SetFallbackPaintKit(567); // Black Laminate
								}
								else if (Skin == 34)
								{
									pWeapon->SetFallbackPaintKit(574); // Autotronic
								}
								else if (Skin == 35)
								{
									pWeapon->SetFallbackPaintKit(580); // Freehand
								}
								else if (Skin == 36)
								{
									pWeapon->SetFallbackPaintKit(852); // 
								}
								else if (Skin == 37)
								{
									pWeapon->SetFallbackPaintKit(853); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(854); // 
								}
								else if (Skin == 38)
								{
									pWeapon->SetFallbackPaintKit(855); // 
								}
								else if (Skin == 39)
								{
									pWeapon->SetFallbackPaintKit(856); // 
								}
								else if (Skin == 40)
								{
									pWeapon->SetFallbackPaintKit(857); // 
								}
								else if (Skin == 41)
								{
									pWeapon->SetFallbackPaintKit(858); // 
								}
							}
						}
						else
							knife_correct_load();
						pWeapon->OwnerXuidLow(0);
						pWeapon->OwnerXuidHigh(0);
						pWeapon->FallbackWear(0.001);
						pWeapon->ItemIDHigh(1);
					}
				}
			}

		}
}
