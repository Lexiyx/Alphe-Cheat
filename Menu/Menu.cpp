#include "Menu.h"
#include "Config.h"
#include "Hotkeys.h"
#include "..\SDK\Vector.h"
#include "..\SDK\ISurface.h"
#include "..\Utils\Color.h"
#include "..\Utils\GlobalVars.h"
#include "..\Utils\XorStr.h"
#include "..\Features\Misc\Misc.h"
#include "..\Utils\Color.h"
#include "..\Features\Features.h"
#include "KeyVisualizer.h"
#include "SpecList.h"
#include "models.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>
#include <fstream>
#include "../Features/ProfileChanger/ProtoUtils/Protobuffs.h"
#include "../Features/Misc/Logger.h"

using namespace std;

Menu g_Menu;
NewMenu g_nMenu;
Hotkeys g_Hotkeys;
KeyVisualizer g_KeyVisualizer;
#ifdef RELEASE
Protobuffs write_r;
#endif

// NEW MENU
Color menu_background(0, 0, 0);  //33, 33, 33
Color menu_background2(22, 22, 22);  //66, 66, 66
Color menu_text(190, 190, 190);  //255, 255, 255
Color menu_accent(255, 40, 0); //132, 0, 195
Color console_color(255, 40, 0); //132, 0, 195
Color menu_background3(0, 0, 0);
Color menu_header(0, 0, 0);

Color menu2_background(22, 22, 22);

Color button_light(239, 239, 239); //239, 239, 239
Color button_idle_gray(191, 184, 191); //191, 184, 191
Color button_dark(23, 23, 23); //23, 23, 23

Color EnemyColor(255, 255, 255); //255, 255, 255
Color TeamColor(255, 255, 255); //255, 255, 255
Color LocalColor(255, 255, 255); //255, 255, 255
Color World2Color(50, 50, 50); //50, 50, 50
Color Prop2Color(50, 50, 50); //50, 50, 50
Color AmbientLight2Color(255, 255, 255); //255, 255, 255
Color ArmsChams2Color(255, 255, 255); //255, 255, 255
Color WeaponChams2Color(255, 255, 255); //255, 255, 255
Color GlowOverlayColor2(255, 255, 255); //255, 255, 255
Color PearlescentColor2(255, 255, 255); //255, 255, 255239, 239, 239

int GlobalOpacity = 0;

bool shouldopenmenu = false;
bool Debug = false;

int MenuWidth = 800, MenuHeight = 480;
int TabX, TabY, TabW, TabH;
int Chamstype = 0;

//Get username from file
std::string username;
bool failed = false;

void GradientH(int x, int y, int w, int h, Color c1, Color c2)
{
	g_pSurface->FilledRect(x, y, w, h, c1);
	BYTE first = c2.red;
	BYTE second = c2.green;
	BYTE third = c2.blue;
	for (int i = 0; i < w; i++)
	{
		float fi = i, fw = w;
		float a = fi / fw;
		DWORD ia = a * 255;
		g_pSurface->FilledRect(x + i, y, 1, h, Color(first, second, third, ia));
	}

}

void GradientVEnd(int x, int y, int w, int h, Color c1, Color c2)
{
	BYTE first = c2.red;
	BYTE second = c2.green;
	BYTE third = c2.blue;
	for (float i = 0; i < h / 1.5; i++)
	{
		float fi = i, fh = h / 1.5;
		float a = fi / fh;
		DWORD ia = a * 255;
		g_pSurface->FilledRect(x, y + i, w, 2, Color(first, second, third, ia));
	}
}
void update_ranks()
{
	g_Hooks.MessageSend();
	/*FEATURES::MISC::InGameLogger::Log SendClientHello;
	SendClientHello.color_line.PushBack("[Alpheus] ", Color(190, 0, 255, 255));
	SendClientHello.color_line.PushBack("SendClientHello", Color(255, 255, 255, 255));
	FEATURES::MISC::in_game_logger.AddLog(SendClientHello);*/
	FEATURES::MISC::InGameLogger::Log SendMatchmakingClient2GCHello;
	SendMatchmakingClient2GCHello.color_line.PushBack("[Alpheus] ", Color(190, 0, 255, 255));
	SendMatchmakingClient2GCHello.color_line.PushBack("Game profile updated", Color(255, 255, 255, 255));
	FEATURES::MISC::in_game_logger.AddLog(SendMatchmakingClient2GCHello);
}
void update() { g_Menu.Config.updateworld = true; }
void save()
{
	auto filter = CHudChat::ChatFilters::CHAT_FILTER_NONE;
#ifdef _PROFILE
	std::string cfgsave = SteamStrings(" \x0D{Alpheus} \x01 Config Saved");
	g_ChatElement->ChatPrintf(0, filter, (cfgsave).c_str());
#endif
	g_Config->Save();
	FEATURES::MISC::InGameLogger::Log Saved;
	Saved.color_line.PushBack("[Alpheus] ", Color(190, 0, 255, 255));
	Saved.color_line.PushBack("Config saved", Color(255, 255, 255, 255));
	FEATURES::MISC::in_game_logger.AddLog(Saved);
}
void unload() { g_Menu.Config.Unload = true; }
void forceupdate() { g::knifeapply = true; }
void reset() { g::shots == 0; g::hits == 0; }
void load() {
	g_Config->Load();
	{
		FEATURES::MISC::InGameLogger::Log loaded;
		loaded.color_line.PushBack("[Alpheus] ", Color(190, 0, 255, 255));
		loaded.color_line.PushBack("Config loaded", Color(255, 255, 255, 255));
		FEATURES::MISC::in_game_logger.AddLog(loaded);
		update_ranks();
		//forceupdate();
#ifdef _PROFILE
		if (g_Menu.Config.RankChanger.player_level > 0 || g_Menu.Config.RankChanger.wins > 0 || g_Menu.Config.RankChanger.cmd_friendly > 0
			|| g_Menu.Config.RankChanger.cmd_teaching > 0 || g_Menu.Config.RankChanger.cmd_leader > 0 || g_Menu.Config.RankChanger.time > 0)
		{
			FEATURES::MISC::InGameLogger::Log profupdated;
			profupdated.color_line.PushBack("[Alpheus] ", Color(190, 0, 255, 255));
			profupdated.color_line.PushBack("Game profile updated", Color(255, 255, 255, 255));
			FEATURES::MISC::in_game_logger.AddLog(profupdated);
			update_ranks();
		}
#endif
	}
}
void OpenForum()
{
	system("start http://Alpheuscheat.cf");
}


void InvisName()
{
	g_Misc.SetName(SteamStrings("\n\xAD\xAD\xAD\xAD"));
	g_Misc.SetName(SteamStrings("\n\xAD\xAD\xAD\xAD"));
	g_Misc.SetName(SteamStrings("\n\xAD\xAD\xAD\xAD"));
}
bool isupdated = false;


void resetclantag()
{
	g_pEngine->ExecuteClientCmd(SteamStrings("cl_clanid 0"));
}
static wchar_t* charToWChar(const char* text)
{
	size_t size = strlen(text) + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs(wa, text, size);
	return wa;
}
void GetUserNameFromFile()
{
	//ifstream myfile("C:\\Windows\\win_user.txt");
	//if (myfile.is_open())
	//{
	//	failed = false;
	//	while (getline(myfile, username))
	//	{
	//		username;
	//	}
	//	myfile.close();
	//}
	//else
	//{
	//	failed = true;
	//}
	failed = false;
	username = "U.C. USER";
}
std::string getTimeStr() {
	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	std::string s(30, '\0');
	std::strftime(&s[0], s.size(), "%H:%M:%S", std::localtime(&now));
	return s;
}
void Menu::WaterMark()
{

	int width, height;
	g_pEngine->GetScreenSize(width, height);
	Color color(255, 40, 0); //132, 0, 195
	color = Color(g_Menu.Config.MenuColor.red, g_Menu.Config.MenuColor.green, g_Menu.Config.MenuColor.blue);

	static int fps, old_tick_count;
	if (!g_pEngine->IsConnected())
	{
		static int fps, old_tick_count = 0;
		return;
	}
	if ((g_pGlobalVars->tickcount - old_tick_count) > 50) {
		fps = static_cast<int>(1.f / g_pGlobalVars->frametime);
		old_tick_count = g_pGlobalVars->tickcount;
	}
	if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame())
		return;
	char FPS;
	int radix = 10;
	static char buffer[256];
	std::stringstream ss;
	static unsigned int last_time;

	/////rainbow sync//////
	last_time = GetTickCount();
	Color rainbow;
	rainbow.FromHSV(fmod(last_time * 0.0002f, 1.f), 1.f, 0.5f);
	//////////////////////

	//Color menu_Accent;

	auto net_channel = g_pEngine->GetNetChannelInfo();
	auto local_player = reinterpret_cast<C_BaseEntity*>(g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer()));
	std::string incoming = local_player ? std::to_string((int)(net_channel->GetLatency(FLOW_INCOMING) * 1000)) : SteamStrings("0");
	std::string outgoing = local_player ? std::to_string((int)(net_channel->GetLatency(FLOW_OUTGOING) * 1000)) : SteamStrings("0");

	PlayerInfo_t pInfo;
	g_pEngine->GetPlayerInfo(g_pEngine->GetLocalPlayer(), &pInfo);
	std::string name = pInfo.szName;
	int sum = 0;
	int x;
	if (failed)
		ss << SteamStrings("Alpheus|") << SteamStrings("|incoming:") << incoming.c_str() << SteamStrings("ms|") << SteamStrings("|outgoing:") << outgoing.c_str() << SteamStrings("ms|") << SteamStrings("|fps: ") << fps/* << SteamStrings("||Licensed to:") << SteamStrings(" FAILED TO GET USER") << "||"*/;
	else
		ss << SteamStrings("Alpheus|") << SteamStrings("|incoming:") << incoming.c_str() << SteamStrings("ms|") << SteamStrings("|outgoing:") << outgoing.c_str() << SteamStrings("ms|") << SteamStrings("|fps: ") << fps << "||" /*<< SteamStrings("|Licensed to:") << username  << "||" */ << getTimeStr();
	RECT TextSize = g_pSurface->GetTextSizeRect(g::Tahoma, ss.str().c_str());
	if (g_Menu.Config.Rainbowwatermark)
	{
		g_pSurface->FilledRect(1585, 25, 22, 2, rainbow);
		g_pSurface->FilledRect(width - (TextSize.right + 20), 25, TextSize.right + 10, 2, rainbow);
	}
	else
	{
		g_pSurface->FilledRect(1585, 25, 22, 2, color);
		g_pSurface->FilledRect(width - (TextSize.right + 20), 25, TextSize.right + 10, 2, color);
	}



	g_pSurface->FilledRect(width - (TextSize.right + 20), 5, TextSize.right + 10, 20, Color(33, 33, 33));
	g_pSurface->DrawT(width - (TextSize.right + 15), 8, Color(200, 200, 200), g::Tahoma, false, ss.str().c_str());

}
void Menu::WeaponChooser()
{
#pragma region SkinList
	if (Config.Skinchanger.globalskins == 0)
		ComboBox("Skin", {
"Default",
"Aquamarine Revenge",
"Asiimov",
"Baroque Purple",
"Black Laminate",
"Bloodsport",
"Blue Laminate",
"Cartel",
"Case Hardened",
"Elite Build",
"Emerald Pinstripe",
"Fire Serpent",
"First Class",
"Frontside Misty",
"Fuel Injector",
"Hydroponic",
"Jaguar",
"Jet Set",
"Jungle Spray",
"Legion of Anubis",
"Neon Revolution",
"Neon Rider",
"Orbit Mk01",
"Panthera onca",
"Phantom Disruptor",
"Point Disarray",
"Predator",
"Rat Rod ",
"Red Laminate",
"Redline",
"Safari Mesh",
"Safety Net",
"The Empress",
"Uncharted",
"Vulcan",
"Wasteland Rebel",
"Wild Lotus",
"X-Ray", }, &Config.Skinchanger.AK47Skin);
	if (Config.Skinchanger.globalskins == 3) //dual
		ComboBox("Skin", {
			"Default",
			"Anodized Navy",
			"Balance",
			"Black Limba",
			"Briar",
			"Cartel",
			"Cobalt Quartz",
			"Cobra Strike",
			"Colony",
			"Contractor",
			"Demolition",
			"Dezastre",
			"Dualing Dragons",
			"Duelist",
			"Elite 1.6",
			"Emerald",
			"Heist",
			"Hemoglobin",
			"Marina",
			"Moon in Libra",
			"Panther",
			"Pyre",
			"Retribution",
			"Royal Consorts",
			"Shred",
			"Stained",
			"Switch Board",
			"Twin Turbo",
			" Urban Shock",
			"Ventilators"
			}, &Config.Skinchanger.DualSkin);
	if (Config.Skinchanger.globalskins == 4) //awp
		ComboBox("Skin", {
			"Default",
			"Acheron",
			"Asiimov",
			"Atheris",
			"BOOM",
			"Capillary",
			"Containment Breach",
			"Corticera",
			"Dragon Lore",
			"Electric Hive",
			"Elite Build",
			"Exoskeleton",
			"Fade",
			"Fever Dream",
			"Graphite",
			"Gungnir",
			"Hyper Beast",
			"Lightning Strike",
			"Man-o'-war",
			"Medusa",
			"Mortis",
			"Neo-Noir",
			"Oni Taiji",
			"PAW",
			"Phobos",
			"Pink DDPAT",
			"Pit Viper",
			"Redline",
			"Safari Mesh",
			"Silk Tiger",
			"Snake Camo",
			"Sun in Leo",
			"The Prince",
			"Wildfire",
			"Worm God"
			}, &Config.Skinchanger.AWPSkin);
	if (Config.Skinchanger.globalskins == 22) //scout
		ComboBox("Skin", {
"Default",
"Lichen Dashed",
"Dark Water",
"Blue Spruce",
"Sand Dune",
"Palm",
"Mayan Dreams",
"Blood in the Water",
"Tropical Storm",
"Acid Fade",
"Slashed",
"Detour",
"Abyss",
"Big Iron",
"Necropos",
"Ghost Crusader",
"Dragonfire" }, &Config.Skinchanger.SSG08Skin);
	if (Config.Skinchanger.globalskins == 20) //scar
		ComboBox("Skin", {
"Default",
"Splash Jam",
"Storm",
"Contractor",
"Carbon Fiber",
"Sand Mesh",
"Palm",
"Crimson Web",
"Cardiac",
"Army Sheen",
"Cyrex",
"Grotto",
"Bloodsport" }, &Config.Skinchanger.SCAR20Skin);
	if (Config.Skinchanger.globalskins == 6) //glock
		ComboBox("Skin", {
"Default",
"Blue Fissure",
"Brass",
"Bullet Queen",
"Bunsen Burner",
"Candy Apple",
"Catacombs",
"Death Rattle",
"Dragon Tattoo",
"Fade",
"Franklin",
"Grinder",
"Groundwater",
"High Beam",
"Ironwork",
"Moonrise",
"Neo-Noir",
"Night",
"Nuclear Garden",
"Off World",
"Oxide Blaze",
"Reactor",
"Royal Legion",
"Sacrifice",
"Sand Dune",
"Steel Disruption",
"Synth Leaf",
"Twilight Galaxy",
"Vogue",
"Warhawk",
"Wasteland Rebel",
"Water Elemental",
"Weasel",
"Wraiths"
			}, &Config.Skinchanger.GlockSkin);
	if (Config.Skinchanger.globalskins == 11) //g3sg1
		ComboBox("Skin", {
"Default",
"Ancient Ritual",
"Arctic Camo",
"Azure Zebra",
"Black Sand",
"Chronos",
"Contractor",
"Demeter",
"Desert Storm",
"Digital Mesh",
"Flux",
"Green Apple",
"High Seas",
"Hunter",
"Jungle Dashed",
"Murky",
"Orange Crash",
"Orange Kimono",
"Polar Camo",
"Safari Mesh",
"Scavenger",
"Stinger",
"The Executioner",
"VariCamo",
"Ventilator",
"Violet Murano"
			}, &Config.Skinchanger.G3sg1Skin);
	if (Config.Skinchanger.globalskins == 12) //galil
		ComboBox("Skin", {
"Default",
"Akoben",
"Aqua Terrace",
"Black Sand",
"Blue Titanium",
"Cerberus",
"Chatterbox",
"Cold Fusion",
"Connexion",
"Crimson Tsunami",
"Dusk Ruins",
"Eco",
"Firefight",
"Hunting Blind",
"Kami",
"Orange DDPAT",
"Phoenix Blacklight",
"Rocket Pop",
"Sage Spray",
"Sandstorm",
"Shattered",
"Signal",
"Stone Cold",
"Sugar Rush",
"Tornado",
"Tuxedo",
"Urban Rubble",
"Vandal",
"VariCamo",
"Winter Fores"
			}, &Config.Skinchanger.G3sg1Skin);
	if (Config.Skinchanger.globalskins == 13) //m249
		ComboBox("Skin", {
"Default",
"Aztec",
"Blizzard Marbleized",
"Contrast Spray",
"Deep Relief",
"Emerald Poison Dart",
"Gator Mesh",
"Impact Drill",
"Jungle",
"Jungle DDPAT",
"Magma",
"Nebula Crusader",
"Predator",
"Shipping Forecast",
"Spectre",
"System Lock",
"Warbird" }, &Config.Skinchanger.G3sg1Skin);
	if (Config.Skinchanger.globalskins == 7) //deagle
		ComboBox("Skin", {
"Default",
"Blaze",
"Blue Ply",
"Bronze Deco",
"Cobalt Disruption",
"Code Red",
"Conspiracy",
"Corinthian",
"Crimson Web",
"Directive",
"Emerald Jörmungandr",
"Golden Koi",
"Hand Cannon",
"Heirloom",
"Hypnotic",
"Kumicho Dragon",
"Light Rail",
"Mecha Industries",
"Meteorite",
"Midnight Storm",
"Mudder",
"Naga",
"Night",
"Night Heist",
"Oxide Blaze",
"Pilot",
"Printstream",
"Sunset Storm",
"Sunset Storm",
"The Bronze",
"Urban DDPAT",
"Urban Rubble"
			}, &Config.Skinchanger.DeagleSkin);
	if (Config.Skinchanger.globalskins == 5) //usps
		ComboBox("Skin", {
"Default",
"Ancient Visions",
"Blood Tiger",
"Blueprint",
"Business Class",
"Caiman",
"Check Engine",
"Cortex",
"Cyrex",
"Dark Water",
"Flashback",
"Forest Leaves",
"Guardian",
"Kill Confirmed",
"Lead Conduit",
"Monster Mashup",
"Neo-Noir",
"Night Ops",
"Orion",
"Overgrowth",
"Para Green",
"Pathfinder",
"Road Rash",
"Royal Blue",
"Serum",
"Stainless",
"Target Acquired",
"Torque"
			}, &Config.Skinchanger.USPSkin);
	if (Config.Skinchanger.globalskins == 16) //p2000
		ComboBox("Skin", {
			"Default",
			"Granite Marbleized",
			"Silver",
			"Scorpion",
			"Grassland",
			"Grassland Leaves",
			"Corticera",
			"Ocean Foam",
			"Amber Fade",
			"Red FragCam",
			"Chainmail",
			"Pulse",
			"Coach Class",
			"Ivory",
			"Fire Elemental",
			"Pathfinder",
			"Handgun",
			"Imperial",
			"Turf",
			"Woodsman",
			"Urban Hazard",
			"Obsidian",
			"Acid Etched" }, &Config.Skinchanger.P2000Skin);
	if (Config.Skinchanger.globalskins == 23) //revolver
		ComboBox("Skin", {
		"Default",
		"Crimson Web",
		"Bone Mask",
		"Urban Perforated",
		"Waves Perforated",
		"Orange Peel",
		"Urban Masked",
		"Jungle Dashed",
		"Sand Dashed",
		"Urban Dashed",
		"Dry Season",
		"Fade",
		"Amber Fade",
		"Reboot",
		"Llama Cannon",
		"Grip",
		"Survivalist",
		"Nitro",
		"Skull Crusher",
		"Canal Spray",
		"Memento",
		"Bone Forged"
			}, &Config.Skinchanger.RevolverSkin);
	if (Config.Skinchanger.globalskins == 1) //m4a4
		ComboBox("Skin", {
"Default",
"Asiimov",
"Bullet Rain",
"Buzz Kill",
"Converter",
"Cyber Security",
"Dark Blossom",
"Daybreak",
"Desert Storm",
"Desert-Strike",
"Desolate Space",
"Evil Daimyo",
"Faded Zebra",
"Global Offensive",
"Griffin",
"Hellfire",
"Howl",
"Jungle Tiger",
"Magnesium",
"Mainframe",
"Modern Hunter",
"Neo-Noir",
"Poseidon",
"Radiation Hazard",
"Royal Paladin",
"The Battlestar",
"The Emperor",
"Tooth Fairy",
"Tornado",
"Urban DDPAT",
"X-Ray",
"Zirka",
"Dragon King"

			}, &Config.Skinchanger.M4A4Skin);
	if (Config.Skinchanger.globalskins == 8) //fiveseven
		ComboBox("Skin", {
"Default",
"Angry Mob",
"Anodized Gunmetal",
"Berries And Cherries",
"Buddy",
"Candy Apple",
"Capillary",
"Case Hardened",
"Contractor",
"Coolant",
"Copper Galaxy",
"Crimson Blossom",
"Fairy Tale",
"Flame Test",
"Forest Night",
"Fowl Play",
"Hot Shot",
"Hyper Beast",
"Jungle",
"Kami",
"Monkey Business",
"Neon Kimono",
"Nightshade",
"Nitro",
"Orange Peel",
"Retrobution",
"Scumbria",
"Silver Quartz",
"Triumvirate",
"Urban Hazard",
"Violent Daimyo"
			}, &Config.Skinchanger.FiveSkin);
	if (Config.Skinchanger.globalskins == 9) //aug
		ComboBox("Skin", {
"Default",
"Akihabara Accept",
"Amber Slipstream",
"Anodized Navy",
"Arctic Wolf",
"Aristocrat",
"Bengal Tiger",
"Carved Jade",
"Chameleon",
"Colony",
"Condemned",
"Contractor",
"Copperhead",
"Daedalus",
"Death by Puppy",
"Flame Jörmungandr",
"Fleet Flock",
"Hot Rod",
"Midnight Lily",
"Momentum",
"Navy Murano",
"Radiation Hazard",
"Random Access",
"Ricochet",
"Storm",
"Stymphalian",
"Surveillance",
"Sweeper",
"Syd Mead",
"Tom Cat",
"Torque",
"Triqua",
"Wings"
			}, &Config.Skinchanger.AUGSkin);
	if (Config.Skinchanger.globalskins == 10) //famas
		ComboBox("Skin", {
"Default",
"Afterimage",
"Colony",
"Commemoration",
"Contrast Spray",
"Crypsis",
"Cyanospatter",
"Dark Water",
"Decommissioned",
"Djinn",
"Doomkitty",
"Eye of Athena",
"Hexane",
"Macabre",
"Mecha Industries",
"Neural Net",
"Night Borre",
"Prime Conspiracy",
"Pulse",
"Roll Cage",
"Sergeant",
"Spitfire",
"Styx",
"Sundown",
"Survivor",
"Teardown",
"Valence"
			}, &Config.Skinchanger.FAMASSkin);
	if (Config.Skinchanger.globalskins == 4) //m4a1s
		ComboBox("Skin", {
			"Default",
			"Atomic Alloy",
			"Basilisk",
			"Blood Tige",
			"Blue Phosphor",
			"Boreal Forest",
			"Briefing",
			"Bright Water",
			"Chantico's Fire",
			"Control Panel",
			"Cyrex",
			"Dark Water",
			"Decimator",
			"Flashback",
			"Golden Coil",
			"Guardian",
			"Hot Rod",
			"Hyper Beast",
			"Icarus Fell",
			"Knight",
			"Leaded Glass",
			"Master Piece",
			"Mecha Industries",
			"Moss Quartz",
			"Nightmare",
			"Nitro",
			"Player Two",
			"Printstream",
			"VariCamo",
			"Welcome to the Jungle" }, &Config.Skinchanger.M4A1SSkin);
	if (Config.Skinchanger.globalskins == 14) //mac10
		ComboBox("Skin", {
			"Default",
			"Candy Apple",
			"Urban DDPAT",
			"Silver",
			"Fade",
			"Ultraviolet",
			"Tornado",
			"Palm",
			"Graven",
			"Amber Fade",
			"Heat",
			"Curse",
			"Indigo",
			"Tatter",
			"Commuter",
			"Nuclear Garden",
			"Malachite",
			"Neon Rider",
			"Rangeen",
			"Lapis Gator",
			"Oceanic",
			"Carnivore",
			"Last Dive",
			"Aloha",
			"Red Filigree",
			"Calf Skin",
			"Copper Borre",
			"Pipe Down",
			"Whitefish",
			"Surfwood",
			"Stalker",
			"Classic Crate",
			"Disco Tech"
			}, &Config.Skinchanger.Mac10Skin);
	if (Config.Skinchanger.globalskins == 15) //ump45
		ComboBox("Skin", {
			"Default",
			"Gunsmoke",
			"Urban DDPAT",
			"Blaze",
			"Carbon Fiber",
			"Mudder",
			"Caramel",
			"Fallout Warning",
			"Scorched",
			"Bone Pile",
			"Corporal",
			"Indigo",
			"Labyrinth",
			"Delusion",
			"Grand Prix",
			"Minotaur's Labyrinth",
			"Riot",
			"Primal Saber",
			"Briefing",
			"Scaffold",
			"Metal Flowers",
			"Exposure",
			"Arctic Wolf",
			"Day Lily",
			"Facility Dark",
			"Momentum",
			"Moonrise",
			"Plastique"
			}, &Config.Skinchanger.UMP45Skin);
	if (Config.Skinchanger.globalskins == 24) //p90
		ComboBox("Skin", {
			"Default",
			"Virus",
			"Cold Blooded",
			"Storm",
			"Glacier Mesh",
			"Sand Spray",
			"Death by Kitty",
			"Fallout Warning",
			"Scorched",
			"Emerald Dragon",
			"Blind Spot",
			"Ash Wood",
			"Teardown",
			"Trigon",
			"Desert Warfare",
			"Module",
			"Leather",
			"Asiimov",
			"Elite Build",
			"Shapewood",
			"Chopper",
			"Grim",
			"Shallow Grave",
			"Death Grip",
			"Traction",
			"Sunset Lily",
			"Baroque Red",
			"Astral Jörmungandr",
			"Facility Negative",
			"Off World",
			"Nostalgia"
			}, &Config.Skinchanger.P90Skin);
	if (Config.Skinchanger.globalskins == 18) //mp9
		ComboBox("Skin", {
			"Default",
			"Hot Rod",
			"Bulldozer",
			"Hypnotic",
			"Orange Peel",
			"Sand Dashed",
			"Dry Season",
			"Rose Iron",
			"Dark Age",
			"Green Plaid",
			"Setting Sun",
			"Dart",
			"Deadly Poison",
			"Pandora's Box",
			"Ruby Poison Dart",
			"Bioleak",
			"Airlock",
			"Sand Scale",
			"Goo",
			"Black Sand",
			"Capillary",
			"Wild Lily",
			"Slide",
			"Modest Threat",
			"Stained Glass",
			"Hydra"
			}, &Config.Skinchanger.Mp9Skin);
	if (Config.Skinchanger.globalskins == 17) //mp7
		ComboBox("Skin", {
			"Default",
			"Forest DDPAT",
			"Skulls",
			"Gunsmoke",
			"Anodized Navy",
			"Whiteout",
			"Orange Peel",
			"Scorched",
			"Groundwater",
			"Ocean Foam",
			"Army Recon",
			"Full Stop",
			"Urban Hazard",
			"Olive Plaid",
			"Armor Core",
			"Asterion",
			"Nemesis",
			"Special Delivery",
			"Impire",
			"Cirrus",
			"Akoben",
			"Bloodsport",
			"Powercore",
			"Teal Blossom",
			"Fade",
			"Motherboard",
			"Mischief",
			"Neon Ply"
			}, &Config.Skinchanger.Mp7Skin);
	if (Config.Skinchanger.globalskins == 19) //p250
		ComboBox("Skin", {
			"Default",
			"Gunsmoke",
			"Bone Mask",
			"Metallic DDPAT",
			"Boreal Forest",
			"Sand Dune",
			"Whiteout",
			"Splash",
			"Modern Hunter",
			"Nuclear Threat",
			"Facets",
			"X-Ray",
			"Hive",
			"Steel Disruption",
			"Mehndi",
			"Undertow",
			"Franklin",
			"Supernova",
			"Contamination",
			"Cartel",
			"Muertos",
			"Valence",
			"Crimson Kimono",
			"Mint Kimono",
			"Wingshot",
			"Asiimov",
			"Iron Clad",
			"Ripple",
			"Red Rock",
			"See Ya Later",
			"Dark Filigree",
			"Vino Primo",
			"Facility Draft",
			"Exchanger",
			"Nevermore",
			"Verdigris",
			"Inferno"
			}, &Config.Skinchanger.P250Skin);
#pragma endregion
}
auto do_frame = [&](std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, Color bg, Color header_text, Color header_line, const std::string& name) {
	g_pSurface->FilledRect(x, y, w, h, bg);
	g_pSurface->FilledRect(x, y, w, 30, header_text);
	g_pSurface->FilledRect(x, y + 30, w, 2, header_line);
	g_pSurface->DrawT(x + 10, y + 8, Color(200, 200, 200), g::Tahoma, false, name.c_str());
};
void NewMenu::Draw()
{
	if (!g_Menu.menuOpened)
		return;

	do_frame(x, y, w, h, Color(36, 36, 36, 255), Color(25, 25, 25, 255), Color(36, 36, 36, 255), "Alpheus cheat");

	group_box(x + 5, y + 35, 100, 260, g::Tahoma, "tabs", false); {
		tab(x + 5, y + (260 / 2) - 70, 100, 30, g::Tahoma, "Legitbot", current_tab, 0, false);
		tab(x + 5, y + (260 / 2) - 35, 100, 30, g::Tahoma, "Ragebot", current_tab, 1, false);
		tab(x + 5, y + (260 / 2), 100, 30, g::Tahoma, "Anti-Aim", current_tab, 2, false);
		tab(x + 5, y + (260 / 2) + 35, 100, 30, g::Tahoma, "Visuals", current_tab, 3, false);
		tab(x + 5, y + (260 / 2) + 70, 100, 30, g::Tahoma, "Misc", current_tab, 4, false);
		tab(x + 5, y + (260 / 2) + 105, 100, 30, g::Tahoma, "Config", current_tab, 5, false);
	}

	switch (current_tab) {
	case 0:
		group_box(x + 110, y + 35, 285, 260, g::Tahoma, "Legitbot", false); {
			check_box(x + 120, y + 45, x + 375, g::Tahoma, "test bool", test_bool);
			check_box(x + 120, y + 55, x + 375, g::Tahoma, "test bool", test_bool);
			slider(x + 120, y + 65, 125, g::Tahoma, "test slider", test_float, -101.f, 101.f);
		}
		group_box(x + 400, y + 35, 285, 260, g::Tahoma, "Legitbot2", false); {
			check_box(x + 410, y + 45, x + 375, g::Tahoma, "test bool2", test_bool);
			check_box(x + 120, y + 55, x + 375, g::Tahoma, "test bool", test_bool);
			slider(x + 410, y + 65, 125, g::Tahoma, "test slider2", test_float, -101.f, 101.f);
		}
		break;
	case 1:
		group_box(x + 110, y + 35, 285, 260, g::Tahoma, "Ragebot", false); {

		}
		break;
	case 2:
		group_box(x + 110, y + 35, 285, 260, g::Tahoma, "Anti-Aim", false); {

		}
		break;
	case 3:
		group_box(x + 110, y + 35, 285, 260, g::Tahoma, "Visuals", false); {

		}
		break;
	case 4:
		group_box(x + 110, y + 35, 285, 260, g::Tahoma, "Misc", false); {

		}
		break;
	case 5:
		group_box(x + 110, y + 35, 285, 260, g::Tahoma, "Config", false); {

		}
		break;
	}

	menu_movement(x, y, w, 30);
}
POINT cursor;
POINT cursor_corrected;

void NewMenu::group_box(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string string, bool show_label) {
	//groupbox background
	g_pSurface->FilledRect(x, y, w, h, Color(25, 25, 25, 255));

	//groupbox outline
	g_pSurface->OutlinedRect(x, y, w, h, Color(45, 45, 45, 255));

	//groupbox label
	if (show_label)
		g_pSurface->DrawT(x + 2, y - 12, Color(200, 200, 200), font, false, string.c_str());
}
void Menu::GroupBox(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string string, bool show_label) {
	//groupbox background
	g_pSurface->FilledRect(x, y, w, h, Color(25, 25, 25, 255));

	//groupbox outline
	g_pSurface->OutlinedRect(x, y, w, h, Color(45, 45, 45, 255));

	//groupbox label
	if (show_label)
		g_pSurface->DrawT(x + 2, y - 12, Color(200, 200, 200), font, false, string.c_str());
}
void Menu::GroupBox_Active(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string string, bool show_label) {
	//groupbox background
	g_pSurface->FilledRect(x, y, w, h, Color(180, 30, 0, 255));

	//groupbox outline
	g_pSurface->OutlinedRect(x, y, w, h, Color(45, 45, 45, 255));

	//groupbox label
	if (show_label)
		g_pSurface->DrawT(x + 2, y - 12, Color(200, 200, 200), font, false, string.c_str());
}
void NewMenu::tab(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string string, std::int32_t& tab, std::int32_t count, bool show_outline) {
	GetCursorPos(&cursor);

	if ((cursor.x > x) && (cursor.x < x + w) && (cursor.y > y) && (cursor.y < y + h) && (GetAsyncKeyState(VK_LBUTTON) & 1))
		tab = count;

	//tab background
	if (show_outline)
		g_pSurface->FilledRect(x, y, w, h, tab == count ? Color(52, 134, 235, 255) : Color(25, 25, 25, 255));

	//tab label
	//g_pSurface->DrawT(x - g_pSurface->get_text_size(font, string).x / 2 + 50, y + h / 2 - 8, show_outline ? Color::White() : tab == count ? Color(52, 134, 235, 255) : Color::White(), font, false, string.c_str());
}

void NewMenu::check_box(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, bool& value) {
	GetCursorPos(&cursor);

	int w = 10, h = 10;

	if ((cursor.x > position) && (cursor.x < position + w) && (cursor.y > y) && (cursor.y < y + h) && GetAsyncKeyState(VK_LBUTTON) & 1)
		value = !value;

	//checkbox background
	g_pSurface->FilledRect(position, y, w, h, value ? Color(52, 134, 235, 255) : Color(36, 36, 36, 255));

	//checkbox label
	g_pSurface->DrawT(x + 2, y - 1, Color::White(), font, false, string.c_str());
}

void NewMenu::slider(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, float& value, float min_value, float max_value) {
	GetCursorPos(&cursor);

	int ix = x + 140;
	int yi = y + 4;

	if ((cursor.x > ix) && (cursor.x < ix + position) && (cursor.y > yi) && (cursor.y < yi + 6) && (GetAsyncKeyState(VK_LBUTTON)))
		value = (cursor.x - ix) / (float(position) / float(max_value));

	//slider background
	g_pSurface->FilledRect(ix, yi, position, 6, Color(36, 36, 36, 255));
	g_pSurface->FilledRect(ix, yi, value * (float(position) / float(max_value)), 6, Color(52, 134, 235, 255));

	//slider label
	g_pSurface->DrawT(x + 2, y - 1, Color::White(), font, false, (std::stringstream{ } << string << ": " << std::setprecision(3) << value).str().c_str());
}

void NewMenu::menu_movement(std::int32_t& x, std::int32_t& y, std::int32_t w, std::int32_t h) {
	GetCursorPos(&cursor);

	if (GetAsyncKeyState(VK_LBUTTON) < 0 && (cursor.x > x && cursor.x < x + w && cursor.y > y && cursor.y < y + h)) {
		should_drag = true;

		if (!should_move) {
			cursor_corrected.x = cursor.x - x;
			cursor_corrected.y = cursor.y - y;
			should_move = true;
		}
	}

	if (should_drag) {
		x = cursor.x - cursor_corrected.x;
		y = cursor.y - cursor_corrected.y;
	}

	if (GetAsyncKeyState(VK_LBUTTON) == 0) {
		should_drag = false;
		should_move = false;
	}
}
void NewMenu::toggle() {
	/*	static bool Pressed = false;

		if (!Pressed && GetAsyncKeyState(VK_INSERT))
			Pressed = true;
		else if (Pressed && !GetAsyncKeyState(VK_INSERT))
		{
			Pressed = false;
			shouldopenmenu = !shouldopenmenu;
		}

		if (shouldopenmenu)
		{
			// opening
			if (GlobalOpacity < 240)
			{
				GlobalOpacity += 15;
				g_Menu.menuOpened = true;
			}
		}
		else
		{
			// closing
			if (GlobalOpacity > 0)
				GlobalOpacity -= 15;
			else
				g_Menu.menuOpened = false;
		}*/
}

int Width = MenuWidth;
int Height = MenuHeight;
void Menu::Render()
{

	static int iWidth, iHeight;
	g_pEngine->GetScreenSize(iWidth, iHeight);

	GetUserNameFromFile();

	menu_accent = Color(Config.MenuColor.red, Config.MenuColor.green, Config.MenuColor.blue);
	console_color = Color(Config.ConsoleColor.red, Config.ConsoleColor.green, Config.ConsoleColor.blue);
	EnemyColor = Color(Config.EnemyGlowColor.red, Config.EnemyGlowColor.green, Config.EnemyGlowColor.blue);
	TeamColor = Color(Config.TeamGlowColor.red, Config.TeamGlowColor.green, Config.TeamGlowColor.blue);
	LocalColor = Color(Config.LocalGlowColor.red, Config.LocalGlowColor.green, Config.LocalGlowColor.blue);
	AmbientLight2Color = Color(Config.AmbientLightColor.red, Config.AmbientLightColor.green, Config.AmbientLightColor.blue);
	ArmsChams2Color = Color(Config.ArmsChamsColor.red, Config.ArmsChamsColor.green, Config.ArmsChamsColor.blue);
	WeaponChams2Color = Color(Config.WeaponChamsColor.red, Config.WeaponChamsColor.green, Config.WeaponChamsColor.blue);
	GlowOverlayColor2 = Color(Config.Glowoverlaycolor.red, Config.Glowoverlaycolor.green, Config.Glowoverlaycolor.blue);
	PearlescentColor2 = Color(Config.PearlColor.red, Config.PearlColor.green, Config.PearlColor.blue);
	static bool Pressed = false;

	if (!Pressed && GetAsyncKeyState(VK_INSERT))
		Pressed = true;
	else if (Pressed && !GetAsyncKeyState(VK_INSERT))
	{
		Pressed = false;
		shouldopenmenu = !shouldopenmenu;
	}

	if (shouldopenmenu)
	{
		// opening
		if (GlobalOpacity < 230)
		{
			GlobalOpacity += 5;
			menuOpened = true;
		}
	}
	else
	{
		// closing
		if (GlobalOpacity > 0)
			GlobalOpacity -= 15;
		else
			menuOpened = false;
	}

	static Vector2D oldPos;
	static Vector2D mousePos;

	static int dragX = 0;
	static int dragY = 0;

	static int iScreenWidth, iScreenHeight;

	static bool Dragging = false;
	bool click = false;

	static bool changed_Debug = false;
	if (!changed_Debug && GetAsyncKeyState(VK_DELETE))
		changed_Debug = true;
	else if (changed_Debug && !GetAsyncKeyState(VK_DELETE))
	{
		changed_Debug = false;
		Debug = !Debug;
	}

	if (Config.OnKeyAim)
	{
		if (GetAsyncKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.AimHotkey)))
			Config.Aimbot = true;
		else
			Config.Aimbot = false;
	}

	g_KeyVisualizer.Draw();
	if (g::pLocalEntity && g_pEngine->IsInGame())
	{
		if (Config.Crosshair)
		{
			Vector punchAngle = g::pLocalEntity->GetAimPunchAngle();

			int x = iWidth / 2;
			int y = iHeight / 2;
			int dy = iHeight / 90;
			int dx = iWidth / 90;
			x -= (dx * (punchAngle.y));
			y += (dy * (punchAngle.x));

			g_pSurface->DrawT(x + 1, y - 6, Color(255, 255, 255), g::TahomaSmall, true, SteamStrings("+"));
		}


		if (Config.LegitAA)
		{
			static bool changed_FakeSide = false;
			if (!changed_FakeSide && GetAsyncKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.LegitSideHotkey)))
				changed_FakeSide = true;
			else if (changed_FakeSide && !GetAsyncKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.LegitSideHotkey)))
			{
				changed_FakeSide = false;
				if (Config.LegitSide == 0)
					Config.LegitSide = 2;
				else
					Config.LegitSide = 0;
			}
		}
		if (Config.Antiaim && Config.DesyncAngle)
		{
			static bool changed_FakeSide = false;
			if (!changed_FakeSide && GetAsyncKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.FakeSideHotkey)))
				changed_FakeSide = true;
			else if (changed_FakeSide && !GetAsyncKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.FakeSideHotkey)))
			{
				changed_FakeSide = false;
				if (Config.FakeSide == 0)
					Config.FakeSide = 2;
				else
					Config.FakeSide = 0;
			}
		}
		if (Config.Antiaim && (Config.YawMode == 3) && g::pLocalEntity->IsAlive())
		{
			if (!g_Menu.menuOpened)
			{
				bool _flip = GetKeyState(g_Menu.Config.RightHotkey);
				if (GetKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.BackHotkey)))
				{
					Config.Right = false;
					Config.Left = false;
					Config.Back = true;
				}
				if (GetKeyState(g_Hotkeys.GetHotkey(g_Menu.Config.RightHotkey)))
				{
					Config.Back = false;
					_flip = !_flip;
				}
				if (!_flip)
				{
					Config.Left = false;
					Config.Right = true;
				}
				else
				{
					Config.Right = false;
					Config.Left = true;
				}

				if (g_Menu.Config.Back == true)
				{
					g_pSurface->DrawT((iWidth / 2) - 20, (iHeight / 2) - 6, Color(255, 255, 255), g::AAarrows, true, SteamStrings("<-"));
					g_pSurface->DrawT((iWidth / 2) + 20, (iHeight / 2) - 6, Color(255, 255, 255), g::AAarrows, true, SteamStrings("->"));
					g_pSurface->DrawT(iWidth / 2, (iHeight / 2) + 20, menu_accent, g::AAarrows, true, SteamStrings("|"));
				}
				if (g_Menu.Config.Right == true)
				{
					g_pSurface->DrawT((iWidth / 2) - 20, (iHeight / 2) - 6, Color(255, 255, 255), g::AAarrows, true, SteamStrings("<-"));
					g_pSurface->DrawT((iWidth / 2) + 20, (iHeight / 2) - 6, menu_accent, g::AAarrows, true, SteamStrings("->"));
					g_pSurface->DrawT(iWidth / 2, (iHeight / 2) + 20, Color(255, 255, 255), g::AAarrows, true, SteamStrings("|"));
				}

				if (g_Menu.Config.Left == true)
				{
					g_pSurface->DrawT((iWidth / 2) - 20, (iHeight / 2) - 6, menu_accent, g::AAarrows, true, SteamStrings("<-"));
					g_pSurface->DrawT((iWidth / 2) + 20, (iHeight / 2) - 6, Color(255, 255, 255), g::AAarrows, true, SteamStrings("->"));
					g_pSurface->DrawT(iWidth / 2, (iHeight / 2) + 20, Color(255, 255, 255), g::AAarrows, true, SteamStrings("|"));
				}
			}
		}

		if (Config.indicators)
		{
			if (g::pLocalEntity->IsAlive())
			{
				float desyncAmt = g::pLocalEntity->get_max_desync_delta();
				float diffrence = (g::RealAngle.y - g::pLocalEntity->GetLowerBodyYaw());
				float Velocity = g::pLocalEntity->GetVelocity().Length2D();
				int offset = 40;
				Vector velocityy = g::pLocalEntity->GetVelocity(); velocityy.z = 0;
				float speed = velocityy.Length();
				Color fake = desyncAmt <= 29 ? Color(255, 0, 0) : (desyncAmt >= 55 ? Color(132, 195, 16) : Color(255 - (desyncAmt * 2.55), desyncAmt * 2.55, 0));
				Color shpeed = speed <= 160 ? Color(255, 0, 0) : (desyncAmt >= 55 ? Color(132, 195, 16) : Color(255 - (desyncAmt * 2.55), desyncAmt * 2.55, 0));

				auto netchannel = g_pClientState->m_net_channel;
				if (!netchannel)
					return;

				if (GetAsyncKeyState(g_Menu.Config.fbaimkey))
					g_pSurface->DrawT(20, (iHeight - offset - 210), Color(0, 255, 0), g::Indicators, false, SteamStrings("Baim"));

				if (GetAsyncKeyState(g_Menu.Config.FakeDuckKey) && !g_pEngine->IsVoiceRecording() && g_Menu.Config.FakeDuck)
					g_pSurface->DrawT(20, (iHeight - offset - 180), Color(0, 255, 0), g::Indicators, false, SteamStrings("Fake Duck"));


				if (Config.FakeLagType != 0)
				{
					std::string choke = SteamStrings("Choked packets: ") + std::to_string(netchannel->m_nChokedPackets);
					g_pSurface->DrawT(20, (iHeight - offset - 120), Color(255, 255, 255), g::Indicators, false, choke.c_str());
				}


				if (diffrence > 35 && Velocity < 0.1f)
					g_pSurface->DrawT(20, (iHeight - offset - 90), Color(132, 195, 16), g::Indicators, false, SteamStrings("LBY"));
				else
					g_pSurface->DrawT(20, (iHeight - offset - 90), Color(255, 0, 0), g::Indicators, false, SteamStrings("LBY"));

				/*if (!(desyncAmt < 29) && g_Menu.Config.DesyncAngle)
					g_pSurface->DrawT(20, (iHeight - offset - 60), fake, g::Indicators, false, SteamStrings("FAKE"));*/

				if (Config.Doubletap)
				{
					if (!g::offdt)
						g_pSurface->DrawT(20, (iHeight - offset - 30), Color(255, 0, 0), g::Indicators, false, SteamStrings("DT"));
					else
						g_pSurface->DrawT(20, (iHeight - offset - 30), Color(0, 255, 0), g::Indicators, false, SteamStrings("DT"));
				}
			}
		}

		if (Config.velocity)
		{
			if (g::pLocalEntity->IsAlive())
			{
				std::string velocity;
				float Velocity = g::pLocalEntity->GetVelocity().Length2D();
				int Velocity2 = Velocity;
				Vector velocityy = g::pLocalEntity->GetVelocity(); velocityy.z = 0;

				if (Velocity >= 150 && Velocity < 240)//orange
				{
					Color Speed = Color(255, 150, 0);
					velocity += std::to_string(static_cast<int>(velocityy.Length()));
					g_pSurface->DrawT(iWidth / 2 - 50, iHeight - 50, Speed, g::Velocity, false, velocity.c_str());
				}
				else if (Velocity < 150 && Velocity > 1)//red
				{
					Color Speed = Color(255, 0, 0);
					velocity += std::to_string(static_cast<int>(velocityy.Length()));
					g_pSurface->DrawT(iWidth / 2 - 50, iHeight - 50, Speed, g::Velocity, false, velocity.c_str());

				}
				else if (Velocity > 239)//green
				{
					Color Speed = Color(0, 255, 0);
					velocity += std::to_string(static_cast<int>(velocityy.Length()));
					g_pSurface->DrawT(iWidth / 2 - 50, iHeight - 50, Speed, g::Velocity, false, velocity.c_str());
				}
				else if (Velocity < 1)//white
				{
					Color Speed = Color(255, 255, 255);
					velocity += std::to_string(static_cast<int>(velocityy.Length()));
					g_pSurface->DrawT(iWidth / 2 - 20, iHeight - 50, Speed, g::Velocity, false, velocity.c_str());
				}
			}
		}

		if (Config.shotcounter)
		{
			if (g::pLocalEntity->IsAlive())
			{
				std::string shotscounter;
				std::string shotscounter2;
				std::string shotscounter3;
				shotscounter += SteamStrings("   shot: ") + std::to_string(g::shots);
				shotscounter2 += SteamStrings("  hit: ") + std::to_string(g::hits);
				shotscounter3 += SteamStrings("  miss: ") + std::to_string(g::shots - g::hits);
				g_pSurface->DrawT(55, 400, Color(0, 170, 255), g::Indicators, true, shotscounter.c_str());
				g_pSurface->DrawT(55, 430, Color(0, 255, 60), g::Indicators, true, shotscounter2.c_str());
				g_pSurface->DrawT(55, 460, Color(255, 0, 0), g::Indicators, true, shotscounter3.c_str());

			}
		}

		if (Config.DrawAimFov && Config.LegitAimbot && Config.LegitAimFov > 0) //draw fov
		{
			Color FovColor(Config.AimFovColor.red, Config.AimFovColor.green, Config.AimFovColor.blue, 100);
			g_pSurface->OutlineCircle(iWidth / 2, iHeight / 2, Config.LegitAimFov * 11, 100, FovColor);
		}
	}

	if (menuOpened)
	{
		menu_background.alpha = GlobalOpacity;
		menu_background2.alpha = GlobalOpacity;
		menu_header.alpha = GlobalOpacity;
		menu_background3.alpha = 175;
		menu_text.alpha = GlobalOpacity;
		menu_accent.alpha = GlobalOpacity;

		if (GetAsyncKeyState(VK_LBUTTON))
			click = true;

		g_pEngine->GetScreenSize(iScreenWidth, iScreenHeight);
		Vector2D MousePos = g_pSurface->GetMousePosition();

		if (Dragging && !click)
			Dragging = false;

		if (Dragging && click)
		{
			Pos.x = MousePos.x - dragX;
			Pos.y = MousePos.y - dragY;
		}

		if (g_pSurface->MouseInRegion(Pos.x - 120, Pos.y, 120, Height))
		{
			Dragging = true;
			dragX = MousePos.x - Pos.x;
			dragY = MousePos.y - Pos.y;
		}

		if (Pos.x < 0)
			Pos.x = 0;
		if (Pos.y < 0)
			Pos.y = 0;
		if ((Pos.x + Width) > iScreenWidth)
			Pos.x = iScreenWidth - Width;
		if ((Pos.y + Height) > iScreenHeight)
			Pos.y = iScreenHeight - Height;

		// START MENU
#pragma region Menu

		static unsigned int last_time;

		last_time = GetTickCount();
		Color rainbow;
		rainbow.FromHSV(fmod(last_time * 0.0002f, 1.f), 1.f, 0.5f);

		// Main Background
		g_pSurface->FilledRect(0, 0, iWidth, iHeight, menu_background3);
		g_pSurface->FilledRect(Pos.x, Pos.y, Width, Height, menu_background);

		//Colored header
		g_pSurface->FilledRect(Pos.x - 120, Pos.y, 120, Height, menu_accent);
		g_pSurface->OutlinedRect(Pos.x - 120, Pos.y, 120, Height, menu_background);

		// Header
		if (failed)
			g_pSurface->DrawT(Pos.x + (Width / 2), Pos.y - 60, rainbow, g::Logo, true, SteamStrings("INVALID USER"));
		else
			g_pSurface->DrawT(Pos.x + (Width / 2), Pos.y - 60, rainbow, g::Logo, true, SteamStrings("Alpheus"));
#pragma endregion



		GroupTabPos[0] = Pos.x + 16;
		GroupTabPos[1] = Pos.y + 14;
		GroupTabPos[2] = Width - 32;
		GroupTabPos[3] = Height - 98;

		OffsetY = GroupTabPos[1] + 8;
		ControlsX = GroupTabPos[0];
		ControlsX2 = ControlsX + ((Width / 2) - 30);
		GroupTabBottom = GroupTabPos[1] + GroupTabPos[3];

		TabOffset = 0;
		SubTabOffset = 0;
		PreviousControl = -1;
		OldOffsetY = 0;

		if (failed)
		{
			Tab(SteamStrings("INVALID USER"));//Legit H
			return;
		}
		Tab("C");//Legit H
		{

			ControlsX = ControlsX + 10;
			//Gray menu background box
			GroupBox(GroupTabPos[0] - 6, GroupTabPos[1] - 2, 778, 456, g::Tahoma, "", false); {}
			//Gray tab background box
			GroupBox(TabX, TabY + 315, TabW, 160, g::Tahoma, "", false); {}
			//Line
			GroupBox(GroupTabPos[0] - 6, GroupTabPos[1] - 2, 389, 456, g::Tahoma, "", false); {}
			CheckBox(SteamStrings("Legit Aimbot"), &Config.LegitAimbot);
			Slider(100, SteamStrings("Legit Aim Speed"), &Config.AimSpeed);
			Slider(15, SteamStrings("Legit Aim Fov"), &Config.LegitAimFov);
			KeyBind(SteamStrings("Legit Aim Key"), &Config.LegitAimKey, SteamStrings("Leave blank for mouse 1"));
			ComboBox("Legit Aim Hitbox", { "Head", "Chest", "Pelvis" }, &Config.LegitAimHitbox);
			CheckBox(SteamStrings("Automatic Weapons"), &Config.AutomaticWeapons, SteamStrings("Weapon shoots automatic when you hold fire button"));
			CheckBox(SteamStrings("Legit AA"), &Config.LegitAA, SteamStrings("Desyncs head to make it harder to hit"));
			if (Config.LegitAA)
				ComboBox(SteamStrings("Desync Fake Side Key"), { SteamStrings("Mouse 1"), SteamStrings("Mouse 2"), SteamStrings("Middle Mouse"), SteamStrings("Mouse 5"), SteamStrings("Mouse 4"), SteamStrings("Left Shift"), SteamStrings("Left Control"), SteamStrings("Left Arrow"), SteamStrings("Up Arrow"), SteamStrings("Right Arrow"), SteamStrings("Down Arrow"), SteamStrings("C Key"), SteamStrings("H Key"), SteamStrings("X Key"), SteamStrings("V Key") }, &Config.LegitSideHotkey);
			CheckBox(SteamStrings("Legit Bhop"), &Config.Legitbhop);
			if (Config.Legitbhop)
			{
				Slider(100, SteamStrings("Bhop Hitchance"), &Config.hophitchance);
				Slider(10, SteamStrings("Most Misses Allowed"), &Config.Hoplimit);
				Slider(10, SteamStrings("Max Hops"), &Config.MaxHops);
			}

			ControlsX = ControlsX - 10;
			OffsetY = GroupTabPos[1] + 8;
			ControlsX = GroupTabPos[0] + (Width / 2);
			ControlsX2 = ControlsX + ((Width / 2) - 40);

			CheckBox(SteamStrings("Triggerbot"), &Config.trigetbot_enable);
			CheckBox(SteamStrings("Deathmatch"), &Config.triggerbot_deadmatch);
			CheckBox(SteamStrings("Recoil"), &Config.triggerbot_recoil);
			KeyBind(SteamStrings("Key"), &Config.TrigerKey);
			Text(SteamStrings("Hitboxes"));
			CheckBox(SteamStrings("Head"), &Config.triggerbotbox_head);
			CheckBox(SteamStrings("Chest"), &Config.triggerbotbox_chest);
			CheckBox(SteamStrings("Pelvis"), &Config.triggerbotbox_stomach);
			CheckBox(SteamStrings("Arms"), &Config.triggerbotbox_arms);
			CheckBox(SteamStrings("Legs"), &Config.triggerbotbox_legs);
			Text(SteamStrings(""));
			CheckBox(SteamStrings("Recoil Control System"), &Config.RecoilControlSystem, SteamStrings("Not standalone"));
			if (Config.RecoilControlSystem)
			{
				Slider(100, SteamStrings("RCS X"), &Config.RCS_X);
				Slider(100, SteamStrings("RCS Y"), &Config.RCS_Y);
			}
			CheckBox(SteamStrings("Legit Backtracking"), &Config.LegitBacktrack, SteamStrings("Uses closest recording tickcount to crosshair for backtracking"));

			OffsetY = GroupTabPos[1] + 8;
			ControlsX = GroupTabPos[0];
			ControlsX2 = ControlsX + ((Width / 2) - 30);

		}

		Tab(SteamStrings("A"));//Legit H
		{
			SubTab(SteamStrings("Main"));
			{
				ControlsX = ControlsX + 10;
				CheckBox(SteamStrings("Enable"), &Config.Aimbot);
				CheckBox(SteamStrings("Silent Aim"), &Config.SilentAim, SteamStrings("Removes visualized aimbot"));
				CheckBox(SteamStrings("Auto Stop"), &Config.Autostop, SteamStrings("Stops when aimbot can shoot"));
				if (Config.Autostop)
				{
					ComboBox(SteamStrings("Autostop Mode"), { SteamStrings("Fullstop"), SteamStrings("Minspeed") , SteamStrings("Custom") }, &Config.AutoStopMode);
					if (Config.AutoStopMode == 2)
						Slider(100, SteamStrings("Custom Autostop Speed"), &Config.customstop);
					CheckBox(SteamStrings("Stop In Between Shots"), &Config.StopInBetweenShots);
				}
				CheckBox(SteamStrings("Auto Scope"), &Config.AutoScope, SteamStrings("Scopes automatic when aimbot can shoot"));
				CheckBox(SteamStrings("Friendly Fire"), &Config.FriendlyFire);
				CheckBox(SteamStrings("Position Adjustment"), &Config.PosBacktrack, SteamStrings("Andjust player position for backtracking"));
				if (Config.PosBacktrack)
					ComboBox(SteamStrings("Backtrack type"), { SteamStrings("Experimental"), SteamStrings("Newest") , SteamStrings("All records") }, &Config.posbacktrk_option);
				CheckBox(SteamStrings("On Key Aim"), &Config.OnKeyAim);
				KeyBind(SteamStrings("Aimbot Hotkey"), &Config.AimHotkey, SteamStrings("Leave blank for mouse 1"));
				CheckBox(SteamStrings("Automatic Weapons"), &Config.AutomaticWeapons, SteamStrings("Weapon shoots automatic when you hold fire button"));

				ControlsX = ControlsX - 10;
				OffsetY = GroupTabPos[1] + 28;
				ControlsX = GroupTabPos[0] + (Width / 2);
				ControlsX2 = ControlsX + ((Width / 2) - 40);
				CheckBox(SteamStrings("Fake Lag Prediction"), &Config.FakeLagPrediction, SteamStrings("Predict fakelag for aimbot"));
				CheckBox(SteamStrings("Force Baim"), &Config.ForceBaim);
				if (g_Menu.Config.ForceBaim)
				{
					KeyBind(SteamStrings("Force Baim Key"), &Config.fbaimkey);
				}
				Slider(100, SteamStrings("Head Scale"), &Config.HeadScale);
				Slider(100, SteamStrings("Body Scale"), &Config.BodyScale);
				ComboBox(SteamStrings("Weapon"), { SteamStrings("Auto"), SteamStrings("Rifle") , SteamStrings("Scout") ,SteamStrings("Pistol"), SteamStrings("Awp") , SteamStrings("Deagle/R8") }, &Config.weaponmode);
				if (Config.weaponmode == 0)
				{
					Slider(100, SteamStrings("Auto Minimum Damage"), &Config.AutoMD);
					Slider(100, SteamStrings("Auto Wall Damage"), &Config.AutoAWDM);
					Slider(100, SteamStrings("Auto Hitchance"), &Config.AutoHC);
					ComboBox(SteamStrings("Hitscan"), { SteamStrings("Head"), SteamStrings("Body") , SteamStrings("Head-Body"),SteamStrings("Mixed") }, &Config.autoglobal_hitscan);

				}
				if (Config.weaponmode == 1)
				{
					Slider(100, SteamStrings("Rifle Minimum Damage"), &Config.RifleMD);
					Slider(100, SteamStrings("Rifle Wall Damage"), &Config.RifleAWDM);
					Slider(100, SteamStrings("Rifle Hitchance"), &Config.RifleHC);
					ComboBox(SteamStrings("Hitscan"), { SteamStrings("Head"), SteamStrings("Body") , SteamStrings("Head-Body"),SteamStrings("Mixed") }, &Config.rifleglobal_hitscan);

				}
				if (Config.weaponmode == 2)
				{
					Slider(100, SteamStrings("Scout Minimum Damage"), &Config.ScoutMD);
					Slider(100, SteamStrings("Scout Wall Damage"), &Config.ScoutAWDM);
					Slider(100, SteamStrings("Scout Hitchance"), &Config.ScoutHC);
					ComboBox(SteamStrings("Hitscan"), { SteamStrings("Head"), SteamStrings("Body") , SteamStrings("Head-Body"),SteamStrings("Mixed") }, &Config.scoutglobal_hitscan);

				}
				if (Config.weaponmode == 3)
				{
					Slider(100, SteamStrings("Pistol Minimum Damage"), &Config.PistolMD);
					Slider(100, SteamStrings("Pistol Wall Damage"), &Config.PistolAWDM);
					Slider(100, SteamStrings("Pistol Hitchance"), &Config.PistolHC);
					ComboBox(SteamStrings("Hitscan"), { SteamStrings("Head"), SteamStrings("Body") ,SteamStrings("Head-Body"), SteamStrings("Mixed") }, &Config.pistolglobal_hitscan);

				}
				if (Config.weaponmode == 4)
				{
					Slider(150, SteamStrings("Awp Minimum Damage"), &Config.AwpMD);
					Slider(100, SteamStrings("Awp Wall Damage"), &Config.AwpAWDM);
					Slider(100, SteamStrings("Awp Hitchance"), &Config.AwpHC);
					ComboBox(SteamStrings("Hitscan"), { SteamStrings("Head"), SteamStrings("Body") ,SteamStrings("Head-Body"), SteamStrings("Mixed") }, &Config.awpglobal_hitscan);

				}
				if (Config.weaponmode == 5)
				{
					Slider(150, SteamStrings("D/R8 Minimum Damage"), &Config.DeagleMD);
					Slider(100, SteamStrings("D/R8 Wall Damage"), &Config.DeagleAWDM);
					Slider(100, SteamStrings("D/R8 Hitchance"), &Config.DeagleHC);
					ComboBox(SteamStrings("Hitscan"), { SteamStrings("Head"), SteamStrings("Body"), SteamStrings("Head-Body"), SteamStrings("Mixed") }, &Config.deagleglobal_hitscan);
				}
			}

			OffsetY = GroupTabPos[1] + 8;
			ControlsX = GroupTabPos[0];
			ControlsX2 = ControlsX + ((Width / 2) - 30);

			SubTab(SteamStrings("Extra"));
			{
				ControlsX = ControlsX + 10;
				CheckBox(SteamStrings("Resolver"), &Config.Resolver, SteamStrings("Recommended when enemy use Anti-Aim"));
				CheckBox(SteamStrings("Baim Lethal"), &Config.BaimLethal);
				CheckBox(SteamStrings("Baim In Air"), &Config.BaimInAir);
				Slider(20, SteamStrings("Baim After X Shots"), &Config.BaimAfterXShots);
				Slider(20, SteamStrings("Baim After X Missed Shots"), &Config.BaimAfterXMissedShots);
				CheckBox(SteamStrings("Jumpscout"), &Config.Jumpscout);
				CheckBox(SteamStrings("Rapid fire"), &Config.Doubletap, SteamStrings("You can kill an enemy with two bullet at the same time"));
				if (Config.Doubletap)
				{
					Slider(20, SteamStrings("Recover Time "), &Config.rtime);
				}
				ControlsX = ControlsX - 10;
			}
		}

		Tab(SteamStrings("B"));//Legit H
		{
			SubTab(SteamStrings("Main"));
			{
				ControlsX = ControlsX + 10;
				CheckBox(SteamStrings("Enable"), &Config.Antiaim);
				{
					ComboBox(SteamStrings("Yaw"), { SteamStrings("Freestanding"), SteamStrings("FreeStanding(Beta)"),SteamStrings("TraceStanding")/*,SteamStrings("Manual") */ }, &Config.YawMode);
					//if (Config.YawMode == 3)
					//{
						//KeyBind(SteamStrings("Manual Left-Right"), &Config.RightHotkey);
						//KeyBind(SteamStrings("Manual Back"), &Config.BackHotkey);
					//}

					ComboBox(SteamStrings("Pitch"), { SteamStrings("Down"), SteamStrings("Emotion"), SteamStrings("Half Down"), SteamStrings("Zero"), SteamStrings("Up"), SteamStrings("Jitter Up/Down"), SteamStrings("Custom"), SteamStrings("Nospread"), SteamStrings("Nospread 2"), SteamStrings("View") }, &Config.PitchMode);
					if (Config.PitchMode == 6)
						Slider(180, SteamStrings("Custom Pitch"), &Config.CustomPitchValue);

					CheckBox(SteamStrings("Desync Angle"), &Config.DesyncAngle);
					if (Config.DesyncAngle)
					{

						//if (Config.YawMode != 9)
						ComboBox(SteamStrings("Desync Style"), { SteamStrings("Static"), SteamStrings("Jitter") }, &Config.DesyncStyle);
						ComboBox(SteamStrings("Desync Mode"), { SteamStrings("Static"), SteamStrings("Automatic"),SteamStrings("Calculated"),SteamStrings("Custom static") }, &Config.DesyncMode);
						if (Config.DesyncStyle == 1)
							Slider(180, SteamStrings("Jitter Range"), &Config.JitterRange);
						if (Config.DesyncMode == 3)
							Slider(180, SteamStrings("Custom Static Modifier"), &Config.DesyncCustomStatic);
						//if (Config.YawMode == 2 || Config.YawMode == 3)
							//ComboBox(SteamStrings("Desync Fake Side Key"), { SteamStrings("Mouse 1"), SteamStrings("Mouse 2"), SteamStrings("Middle Mouse"), SteamStrings("Mouse 5"), SteamStrings("Mouse 4"), SteamStrings("Left Shift"), SteamStrings("Left Control"), SteamStrings("Left Arrow"), SteamStrings("Up Arrow"), SteamStrings("Right Arrow"), SteamStrings("Down Arrow"), SteamStrings("C Key"), SteamStrings("H Key"), SteamStrings("X Key"), SteamStrings("V Key") }, &Config.FakeSideHotkey);
					}
					CheckBox(SteamStrings("Fakeduck"), &Config.FakeDuck);
					if (Config.FakeDuck)
						KeyBind(SteamStrings("Fakeduck Key"), &Config.FakeDuckKey);

					CheckBox(SteamStrings("Airstuck"), &Config.Airstuck, SteamStrings("You can stuck in the air"));
					if (Config.Airstuck)
						KeyBind(SteamStrings("Airstuck Key"), &Config.Airstuckkey);

				}

				ControlsX = ControlsX - 10;
				OffsetY = GroupTabPos[1] + 8;
				ControlsX = GroupTabPos[0] + (Width / 2);
				ControlsX2 = ControlsX + ((Width / 2) - 40);

				CheckBox(SteamStrings("Lby Breaker"), &Config.LbyBreaker);
				if (Config.LbyBreaker)
				{
					Slider(360, SteamStrings("Lby Offset Add"), &Config.LbyAdd);
					CheckBox(SteamStrings("SendPacket On Break"), &Config.SendPacketOnBreak);
				}
				CheckBox(SteamStrings("Lby Static"), &Config.StaticLby);
				CheckBox(SteamStrings("Zero Pitch On Land"), &Config.ZeroPitchOnLand);
				CheckBox(Config.Antiaim ? SteamStrings("Slide Fix") : SteamStrings("Slide Walk"), &Config.SlideWalk);
				CheckBox(SteamStrings("Anti Aim Indicators"), &Config.AntiAimIndicator);
				if (Config.AntiAimIndicator)
					CheckBox(SteamStrings("Advanced AA Indicators"), &Config.AdvancedAAIndicator);
				CheckBox(SteamStrings("Show Server Hitboxes"), &Config.ShowServerHitboxes, SteamStrings("You need to be a host!"));
			}


			OffsetY = GroupTabPos[1] + 8;
			ControlsX = GroupTabPos[0];
			ControlsX2 = ControlsX + ((Width / 2) - 30);

			SubTab(SteamStrings("Extra"));
			{
				ControlsX = ControlsX + 10;
				ComboBox(SteamStrings("Fake Lag Type"), { SteamStrings("Off"),SteamStrings("Factor"), SteamStrings("Adaptive"), SteamStrings("Switch") }, &Config.FakeLagType);
				Slider(14, SteamStrings("Fake Lag Value On Ground"), &Config.FakeLagValue);
				Slider(14, SteamStrings("Fake Lag Value In Air"), &Config.FakeLagValueGround);
				CheckBox(SteamStrings("Fake Lag On Peek"), &Config.FakeLagOnPeek);
				CheckBox(SteamStrings("Show FakeLag"), &Config.ShowLag);
				ColorPicker(SteamStrings("Show FakeLag Color"), Config.ShowLagColor);
				CheckBox(SteamStrings("Slow Walk"), &Config.SlowWalk);

				ControlsX = ControlsX - 10;
				OffsetY = GroupTabPos[1] + 28;
				ControlsX = GroupTabPos[0] + (Width / 2);
				ControlsX2 = ControlsX + ((Width / 2) - 30);

				if (Config.SlowWalk)
				{
					Slider(100, SteamStrings("Speed"), &Config.SlowWalkAmount);
					CheckBox(SteamStrings("Stop Leg Animations"), &Config.StopLegAnim);
				}
			}
		}

		OffsetY = GroupTabPos[1] + 8;
		ControlsX = GroupTabPos[0];
		ControlsX2 = ControlsX + ((Width / 2) - 30);


		Tab(SteamStrings("D"));//Legit H
		{
			SubTab(SteamStrings("Esp"));
			{
				ControlsX = ControlsX + 10;
				ComboBox(SteamStrings("Enable"), { SteamStrings("Off"),SteamStrings("On"),SteamStrings("Dead") }, &Config.Esp);
				CheckBox(SteamStrings("2D Box"), &Config.Box);
				ColorPicker(SteamStrings("Esp Box Color"), Config.BoxColor);
				CheckBox(SteamStrings("Health Based Esp"), &Config.HbEsp, SteamStrings("ESP color based on health"));
				CheckBox(SteamStrings("Team Esp"), &Config.TeamEsp);
				ComboBox(SteamStrings("Show Name"), { SteamStrings("Off"), SteamStrings("Left Align"), SteamStrings("Center") }, &Config.Name);
				ColorPicker(SteamStrings("Name Color"), Config.NameColor);
				CheckBox(SteamStrings("Show Health"), &Config.Health);
				ColorPicker(SteamStrings("Health Color"), Config.HealthColor);
				ComboBox(SteamStrings("Show Health Bar"), { SteamStrings("Off"), SteamStrings("Static Color"), SteamStrings("Health Based") }, &Config.HealthBar);
				ComboBox(SteamStrings("Health Bar Style"), { SteamStrings("Static"), SteamStrings("Small Line") }, &Config.HealthStyle);
				ColorPicker(SteamStrings("Health Bar Color"), Config.HealthBarColor);
				CheckBox(SteamStrings("Show Money"), &Config.Money);
				CheckBox(SteamStrings("Ammo Bar"), &Config.ammo);
				ColorPicker(SteamStrings("Ammo Color"), Config.AmmoEspColor);
				//ColorPicker(SteamStrings("Money Color"), Config.MoneyColor); //FIXME
				ComboBox(SteamStrings("Show Weapon"), { SteamStrings("Off"), SteamStrings("Right"), SteamStrings("Bottom") }, &Config.Weapon);
				ColorPicker(SteamStrings("Weapon Color"), Config.WeaponColor);
				CheckBox(SteamStrings("Show Yaw/Pitch"), &Config.YawPitch);
				ColorPicker(SteamStrings("Yaw/Pitch Color"), Config.YawPitchColor);
				//CheckBox(SteamStrings("Skeleton Esp"), &Config.SkeletonEsp);
				//ColorPicker(SteamStrings("Skeleton Esp Color"), Config.SkeletonColor);

				ControlsX = ControlsX - 10;
				OffsetY = GroupTabPos[1] + 28;
				ControlsX = GroupTabPos[0] + (Width / 2);
				ControlsX2 = ControlsX + ((Width / 2) - 40);


				//CheckBox(SteamStrings("Player Indicators"), &Config.FovEnable); //FIXME
				//ColorPicker(SteamStrings("Indicator Color"), Config.FovColor);
				if (Config.FovEnable)
				{
					CheckBox(SteamStrings("Always Indicate"), &Config.AlwaysIndicate);
					Slider(125, SteamStrings("Indicator Distance"), &Config.FovSize);
					Slider(30, SteamStrings("Indicator Size"), &Config.FovDistance);
				}
				CheckBox(SteamStrings("Visualize Player Hits"), &Config.VizualizeHits);
				ColorPicker(SteamStrings("Vizualize Hits Color"), Config.CapsuleColor);

				CheckBox(SteamStrings("Bomb Esp"), &Config.BombEsp, SteamStrings("Visualize bomb"));
				CheckBox(SteamStrings("AutoWall Crosshair"), &Config.AutoWallCrosshair);
				CheckBox(SteamStrings("No Smoke"), &Config.NoSmoke, SteamStrings("Removes smoke effect"));
				CheckBox(SteamStrings("No Flash"), &Config.NoFlash, SteamStrings("Removes flash effect"));
				CheckBox(SteamStrings("Preserve Kill Feed"), &Config.PreserveKillFeed, SteamStrings("Your kills and deaths stay in killfeed"));
				CheckBox(SteamStrings("Aspect Ratio Changer"), &Config.aspectratio, SteamStrings("You need to force sv_cheats first!"));
				if (Config.aspectratio)
					SliderFloat(10.f, SteamStrings("Aspect Ratio Value"), &Config.aspectvalue);
			}

			OffsetY = GroupTabPos[1] + 8;
			ControlsX = GroupTabPos[0];
			ControlsX2 = ControlsX + ((Width / 2) - 30);

			SubTab(SteamStrings("Glow"));
			{
				ControlsX = ControlsX + 10;
				CheckBox(SteamStrings("Enemy Glow"), &Config.EnemyGlow);
				if (Config.EnemyGlow)
				{
					ComboBox(SteamStrings("Enemy Glow Mode"), { SteamStrings("Basic"), SteamStrings("Pulse"), SteamStrings("Outline") }, &Config.EnemyGlowMode);
					ColorPicker(SteamStrings("Enemy Glow"), Config.EnemyGlowColor);
					Slider(255, SteamStrings("Enemy Glow Opacity"), &Config.EnemyGlowOpacity);
				}
				CheckBox(SteamStrings("Team Glow"), &Config.TeamGlow);
				if (Config.TeamGlow)
				{
					ComboBox(SteamStrings("Team Glow Mode"), { SteamStrings("Basic"), SteamStrings("Pulse"), SteamStrings("Outline") }, &Config.TeamGlowMode);
					ColorPicker(SteamStrings("Team Glow"), Config.TeamGlowColor);
					Slider(255, SteamStrings("Team Glow Opacity"), &Config.TeamGlowOpacity);
				}

				ControlsX = ControlsX - 10;
				OffsetY = GroupTabPos[1] + 28;
				ControlsX = GroupTabPos[0] + (Width / 2);
				ControlsX2 = ControlsX + ((Width / 2) - 40);

				CheckBox(SteamStrings("Local Glow"), &Config.LocalGlow);
				if (Config.LocalGlow)
				{
					ComboBox(SteamStrings("Local Glow Mode"), { SteamStrings("Basic"), SteamStrings("Pulse"), SteamStrings("Outline") }, &Config.LocalGlowMode);
					ColorPicker(SteamStrings("Local Glow"), Config.LocalGlowColor);
					Slider(255, SteamStrings("Local Glow Opacity"), &Config.LocalGlowOpacity);
				}
			}

			OffsetY = GroupTabPos[1] + 8;
			ControlsX = GroupTabPos[0];
			ControlsX2 = ControlsX + ((Width / 2) - 30);

			SubTab(SteamStrings("Chams"));
			{
				ControlsX = ControlsX + 10;
				ComboBox(SteamStrings("Chams Model"), { SteamStrings("Enemy"), SteamStrings("Team"), SteamStrings("Local"), SteamStrings("Viewmodel") }, &Chamstype);
				if (Chamstype != 3)
				{
					ComboBox(SteamStrings("Chams Material"), { SteamStrings("Normal"), SteamStrings("Flat"), SteamStrings("Metallic"), SteamStrings("Animated"), SteamStrings("Light"), SteamStrings("Arms Race"), SteamStrings("Glow"), SteamStrings("Glow2"), SteamStrings("Pearlescent"), SteamStrings("Pulse"), SteamStrings("Test1"), SteamStrings("Test2"), SteamStrings("Test3") }, &Config.ChamsMaterial);
				}
				switch (Chamstype)
				{
				case 0:
					CheckBox(SteamStrings("Enemy Chams"), &Config.EnemyChams);
					if (Config.EnemyChams)
					{
						ColorPicker(SteamStrings("Enemy Chams Visible"), Config.EnemyVisChamsColor);
						CheckBox(SteamStrings("Enemy XQZ Chams"), &Config.EnemyXQZChams);
						ColorPicker(SteamStrings("Enemy Chams XQZ"), Config.EnemyXQZChamsColor);
						CheckBox(SteamStrings("Enemy Chams Wireframe"), &Config.EnemyWireFrame);
						ComboBox(SteamStrings("Backtrack Chams"), { SteamStrings("Off"), SteamStrings("Purple->Cyan"), SteamStrings("White->Black"), SteamStrings("Chams->Transparent"), SteamStrings("Last Tick") }, &Config.BacktrackingChams);
						if (Config.BacktrackingChams == 4)
						{
							ColorPicker(SteamStrings("Backtrack Chams Color"), Config.BacktrackChamsColor);

						}
					}
					Slider(255, SteamStrings("Enemy Opacity"), &Config.EnemyChamsTransparency);

					ControlsX = ControlsX - 10;
					OffsetY = GroupTabPos[1] + 28;
					ControlsX = GroupTabPos[0] + (Width / 2);
					ControlsX2 = ControlsX + ((Width / 2) - 30);

					if (g_Menu.Config.ChamsMaterial == 6 || g_Menu.Config.ChamsMaterial == 9 || g_Menu.Config.ChamsMaterial == 5)
					{
						ColorPicker(SteamStrings("Glow Chams"), Config.Glowoverlaycolor);
					}
					if (g_Menu.Config.ChamsMaterial == 8)
					{
						ColorPicker(SteamStrings("Pearlescent Cham"), Config.PearlColor);
					}

					break;
				case 1:
					CheckBox(SteamStrings("Team Chams"), &Config.TeamChams);
					if (Config.TeamChams)
					{
						ColorPicker(SteamStrings("Team Chams Visible"), Config.TeamVisChamsColor);
						CheckBox(SteamStrings("Team XQZ Chams"), &Config.TeamXQZChams);
						ColorPicker(SteamStrings("Team Chams XQZ"), Config.TeamXQZChamsColor);
					}
					Slider(255, SteamStrings("Team Opacity"), &Config.TeamChamsTransparency);

					ControlsX = ControlsX - 10;
					OffsetY = GroupTabPos[1] + 28;
					ControlsX = GroupTabPos[0] + (Width / 2);
					ControlsX2 = ControlsX + ((Width / 2) - 40);

					if (g_Menu.Config.ChamsMaterial == 6 || g_Menu.Config.ChamsMaterial == 9 || g_Menu.Config.ChamsMaterial == 5)
					{
						ColorPicker(SteamStrings("Glow Chams"), Config.Glowoverlaycolor);
					}
					if (g_Menu.Config.ChamsMaterial == 8)
					{
						ColorPicker(SteamStrings("Pearlescent Chams"), Config.PearlColor);
					}

					break;
				case 2:
					CheckBox(SteamStrings("Local Chams"), &Config.LocalChams);
					if (Config.LocalChams)
					{
						ColorPicker(SteamStrings("Local Chams Visible"), Config.LocalVisChamsColor);
					}
					Slider(255, SteamStrings("Local Player Opacity"), &Config.LocalTransparency);
					if (Config.Antiaim && Config.DesyncAngle || Config.LegitAA)
					{
						CheckBox(SteamStrings("Desync Chams"), &Config.DesyncChams);
						if (Config.DesyncChams)
						{
							ColorPicker(SteamStrings("Desync Chams Color"), Config.DesyncChamsColor);
							Slider(255, SteamStrings("Desync Chams Transparency"), &Config.DesyncChamsTransparency);
						}
					}
					Slider(255, SteamStrings("Scoped Blend"), &Config.scopeblend);

					ControlsX = ControlsX - 10;
					OffsetY = GroupTabPos[1] + 28;
					ControlsX = GroupTabPos[0] + (Width / 2);
					ControlsX2 = ControlsX + ((Width / 2) - 40);

					if (g_Menu.Config.ChamsMaterial == 6 || g_Menu.Config.ChamsMaterial == 9 || g_Menu.Config.ChamsMaterial == 5)
					{
						ColorPicker(SteamStrings("Glow Chams"), Config.Glowoverlaycolor);
					}
					if (g_Menu.Config.ChamsMaterial == 8)
					{
						ColorPicker(SteamStrings("Pearlescent Chams"), Config.PearlColor);
					}

					break;
				case 3:
					CheckBox(SteamStrings("Viewmodel Changer"), &Config.viewmodelchams);
					if (Config.viewmodelchams)
					{

						CheckBox(SteamStrings("Weapon Chams"), &Config.WeaponChams);
						if (Config.WeaponChams)
						{
							ComboBox(SteamStrings("Weapon Chams Mode"), { SteamStrings("Normal"),SteamStrings("Light"), SteamStrings("Arms Race"), SteamStrings("Glow"), SteamStrings("Eso"), SteamStrings("Pearlescent"), SteamStrings("Pulse") }, &Config.WeaponChamsMode);
							ColorPicker(SteamStrings("Weapon Chams"), Config.WeaponChamsColor);
						}

						ControlsX = ControlsX - 10;
						OffsetY = GroupTabPos[1] + 28;
						ControlsX = GroupTabPos[0] + (Width / 2);
						ControlsX2 = ControlsX + ((Width / 2) - 40);

						CheckBox(SteamStrings("Hand Chams"), &Config.ArmsChams);
						if (Config.ArmsChams)
						{
							ComboBox(SteamStrings("Arms Chams Mode"), { SteamStrings("Normal"),SteamStrings("Light"), SteamStrings("Arms Race"), SteamStrings("Glow"), SteamStrings("Eso"), SteamStrings("Pearlescent"), SteamStrings("Pulse") }, &Config.ArmsChamsMode);
							ColorPicker(SteamStrings("Arms Chams"), Config.ArmsChamsColor);
							Slider(255, SteamStrings("Arms Chams Transparency"), &Config.ArmsChamsTransparency);
							CheckBox(SteamStrings("Arms Chams WireFrame"), &Config.ArmsWireFrame);
						}
						if (g_Menu.Config.ArmsChamsMode == 5 || g_Menu.Config.WeaponChamsMode == 5)
						{
							ColorPicker(SteamStrings("Pearlescent Chams"), Config.PearlColor);
						}

						if (g_Menu.Config.ArmsChamsMode == 2 || g_Menu.Config.ArmsChamsMode == 3 || g_Menu.Config.WeaponChams == 2 || g_Menu.Config.WeaponChams == 3)
						{
							ColorPicker(SteamStrings("Glow Chams"), Config.Glowoverlaycolor);
						}

					}

					break;
				default:
					break;
				}
			}

			OffsetY = GroupTabPos[1] + 8;
			ControlsX = GroupTabPos[0];
			ControlsX2 = ControlsX + ((Width / 2) - 30);

			SubTab(SteamStrings("Rendering"));
			{
				ControlsX = ControlsX + 10;
				CheckBox(SteamStrings("Ambient Light Controller"), &Config.AmbientLight);
				if (Config.AmbientLight)
				{
					ColorPicker(SteamStrings("Ambient Ligh"), Config.AmbientLightColor);
				}
				Slider(150, SteamStrings("Fov"), &Config.Fov);
				Slider(250, SteamStrings("Viewmodel Fov"), &Config.ViewmodelFov);
				CheckBox(SteamStrings("Recoil Crosshair"), &Config.Crosshair);
				CheckBox(SteamStrings("<> Crosshair"), &Config.WCrosshair);
				CheckBox(SteamStrings("^ Crosshair"), &Config.UPCrosshair);
				CheckBox(SteamStrings("Tombstone"), &Config.tombstone);
				CheckBox(SteamStrings("Ragdoll Gravity"), &Config.Ragdollgrav, SteamStrings("You need to force sv_cheats first!"));
				CheckBox(SteamStrings("Kill Counter"), &Config.KillCounter);
				CheckBox(SteamStrings("Draw zeus range"), &Config.zeusrange);
				ControlsX = ControlsX - 10;
				OffsetY = GroupTabPos[1] + 28;
				ControlsX = GroupTabPos[0] + (Width / 2);
				ControlsX2 = ControlsX + ((Width / 2) - 40);
				CheckBox(SteamStrings("Trace Bullets"), &Config.BulletTracers, SteamStrings("Visualize fired bullets"));
				if (Config.BulletTracers)
				{
					CheckBox(SteamStrings("Trace Bullets Impacts"), &Config.Impacts, SteamStrings("Visualize bullets impacts"));
					ComboBox(SteamStrings("Impact Type"), { SteamStrings("Square"), SteamStrings("Sphere") }, &Config.ImpactType);
					CheckBox(SteamStrings("Trace Enemy Bullets"), &Config.monkey2, SteamStrings("Visualize bullets impacts"));
					ColorPicker(SteamStrings("Bullet Tracer Enemy Color"), Config.BulletTracerEnemyColor);
					CheckBox(SteamStrings("Trace Local Player Bullets"), &Config.monkey, SteamStrings("Visualize bullets impacts"));
					ColorPicker(SteamStrings("Bullet Tracer Local Color"), Config.BulletTracerLocalColor);
				}
				CheckBox(SteamStrings("Engine Radar"), &Config.Radar, SteamStrings("Show enemy on radar"));
				CheckBox(SteamStrings("Draw Aim Fov Circle"), &Config.DrawAimFov);
				ColorPicker(SteamStrings("Aim Fov Circle Color"), Config.AimFovColor);
				CheckBox(SteamStrings("Disable Post Processing"), &Config.DisablePostProcess);
				CheckBox(SteamStrings("Grenade Tracer"), &Config.GrenadePrediction);
				CheckBox(SteamStrings("Engine Grenade Tracer"), &Config.Nadetracer2, SteamStrings("You need to force sv_cheats first!"));
				CheckBox(SteamStrings("Force Crosshair"), &Config.ForceCrosshair, SteamStrings("Crosshair with AWP"));
				CheckBox(SteamStrings("No Zoom"), &Config.NoZoom, SteamStrings("Removes enlargement while zoomed in"));
				CheckBox(SteamStrings("No Scope"), &Config.NoScope, SteamStrings("Removes black corner while zoomed in"));
				CheckBox(SteamStrings("No Recoil"), &Config.NoRecoil, SteamStrings("Removes visual recoil"));
				OffsetY = GroupTabPos[1] + 8;
				ControlsX = GroupTabPos[0];
				ControlsX2 = ControlsX + ((Width / 2) - 30);
			}
		}

		OffsetY = GroupTabPos[1] + 8;
		ControlsX = GroupTabPos[0];
		ControlsX2 = ControlsX + ((Width / 2) - 30);


		Tab(SteamStrings("I"));//Legit H
		{
			SubTab(SteamStrings("First"));
			{
				ControlsX = ControlsX + 10;
				CheckBox(SteamStrings("Hitmarker"), &Config.VisHitmarker, SteamStrings("Hit Visualization"));
				if (Config.VisHitmarker)
					CheckBox(SteamStrings("Show Dmg"), &Config.ShowDmg);
				CheckBox(SteamStrings("Damage"), &Config.LogDamage);
				CheckBox(SteamStrings("Purchases"), &Config.LogPurchases);
				//CheckBox(SteamStrings("Log to chat"), &Config.log_chat);
				CheckBox(SteamStrings("Log to screen"), &Config.log_logger);
				CheckBox(SteamStrings("BlockBot"), &Config.blockbotonhead, SteamStrings("You can block teammates movement with it"));
				if (Config.blockbotonhead)
					KeyBind(SteamStrings("BlockBot Key"), &Config.BlockBotKey);
				CheckBox(SteamStrings("Killsay"), &Config.Killsay, SteamStrings("Send message to chat when you killed an enemy"));
				if (g_Menu.Config.Killsay)
				{
					ComboBox(SteamStrings("Killsay Type"), { SteamStrings("Random"), SteamStrings("'rekted by Alpheus'") }, &Config.killsaytype);
				}
				Slider(500, SteamStrings("Extend Backtrack"), &Config.Extbt_value);

				ControlsX = ControlsX - 10;
				OffsetY = GroupTabPos[1] + 28;
				ControlsX = GroupTabPos[0] + (Width / 2);
				ControlsX2 = ControlsX + ((Width / 2) - 40);

				CheckBox(SteamStrings("Buy Bot"), &Config.BuyBot.enable);
				if (Config.BuyBot.enable)
				{
					ComboBox(SteamStrings("Primary Weapon"), { SteamStrings("none"),SteamStrings("auto"),SteamStrings("awp"), SteamStrings("scout") }, &Config.BuyBot.main);
					ComboBox(SteamStrings("Secondary Weapon"), { SteamStrings("none"),SteamStrings("heavy"), SteamStrings("dualies") }, &Config.BuyBot.second);
					ComboBox(SteamStrings("Armor"), { SteamStrings("none"),SteamStrings("full") }, &Config.BuyBot.armor);
					ComboBox(SteamStrings("Misc"), { SteamStrings("none"), SteamStrings("defuser"), SteamStrings("taser"),SteamStrings("full") }, &Config.BuyBot.misc);
					ComboBox(SteamStrings("Grenades"), { SteamStrings("none"), SteamStrings("full") }, &Config.BuyBot.grenade);
				}
				CheckBox(SteamStrings("Walkbot"), &Config.Walkbot, SteamStrings("You can be AFK with it"));
				CheckBox(SteamStrings("Awp Quick Switch"), &Config.quickswitch);
				CheckBox(SteamStrings("Auto kill self"), &Config.autokillself);
				CheckBox(SteamStrings("RankRevealer"), &Config.revealranks, SteamStrings("It reveal all of the ranks"));
				CheckBox(SteamStrings("Auto Accept"), &Config.Autoaccept, SteamStrings("Automatic acceptance of the match"));
				ComboBox(SteamStrings("Spectator list"), { SteamStrings("Off"), SteamStrings("All -> Local"), SteamStrings("Team -> All"), SteamStrings("Enemy -> All"),SteamStrings("All -> All") }, &Config.SpecList);

				OffsetY = GroupTabPos[1] + 8;
				ControlsX = GroupTabPos[0];
				ControlsX2 = ControlsX + ((Width / 2) - 30);
			}


			SubTab(SteamStrings("Second"));
			{
				ControlsX = ControlsX + 10;
				ComboBox(SteamStrings("Hitsound"), { SteamStrings("None"),SteamStrings("Metalic"), SteamStrings("?????"), SteamStrings("Beep1"), SteamStrings("Beep2"), SteamStrings("Bell"),SteamStrings("Headshot"), SteamStrings("Headshot2"), SteamStrings("Bubble"), SteamStrings("Fatality"),SteamStrings("Cod"), SteamStrings("Laser") }, &Config.Hitmarker);
				CheckBox(SteamStrings("Thirdperson"), &Config.thirdperson);
				KeyBind(SteamStrings("Thirdperson Hotkey"), &Config.ThirdpersonHotkey);
				CheckBox(SteamStrings("Thirdperson On Death"), &Config.Thirdpersonondeath);
				ComboBox(SteamStrings("Clantag"), { SteamStrings("Off"), SteamStrings("Static"), SteamStrings("Animated"), SteamStrings("gamesense"), SteamStrings("fatality.win"), SteamStrings("AIMWARE.net"), SteamStrings("Forum"), SteamStrings("Move name") }, &Config.ClanTagMode); //SteamStrings("8=m=D"),SteamStrings("Zero"), SteamStrings("Bigward"),
				//Button(SteamStrings("Reset Clantag"), &resetclantag);
				CheckBox(SteamStrings("Bunnyhop"), &Config.Bhop);
				CheckBox(SteamStrings("Auto Strafe"), &Config.AutoStrafe, SteamStrings("Helps you to strafe"));
				if (g_Menu.Config.AutoStrafe)
				{
					CheckBox(SteamStrings("WASD Movement"), &Config.WASD);
				}
				CheckBox(SteamStrings("Override Viewmodel"), &Config.OverrideViewmodel);
				if (Config.OverrideViewmodel)
				{
					Slider(60, SteamStrings("Viewmodel X"), &Config.ViewmodelOffsetX);
					Slider(60, SteamStrings("Viewmodel Y"), &Config.ViewmodelOffsetY);
					Slider(60, SteamStrings("Viewmodel Z"), &Config.ViewmodelOffsetZ);
					Slider(360, SteamStrings("Rotate"), &Config.roll);
				}


				ControlsX = ControlsX - 10;
				OffsetY = GroupTabPos[1] + 28;
				ControlsX = GroupTabPos[0] + (Width / 2);
				ControlsX2 = ControlsX + ((Width / 2) - 40);

				CheckBox(SteamStrings("Indicators"), &Config.indicators, SteamStrings("Indicate: LBY, FAKE, BAIM, FAKEDUCK"));


				CheckBox(SteamStrings("Shot Counter"), &Config.shotcounter);
				CheckBox(SteamStrings("Disable Panorama Blur"), &Config.DisablePanoramaBlur, SteamStrings("Makes panorama's in-game blur disabled"));
				CheckBox(SteamStrings("Anti Client Crasher"), &Config.AntiClientCrasher, SteamStrings("Prevents you from exploiting the invite system to crash your CS:GO"));
				CheckBox(SteamStrings("Namestealer"), &Config.namestealer, SteamStrings("Steals players name"));
				CheckBox(SteamStrings("Disable Crouch Cooldown"), &Config.crouchcooldown, SteamStrings("Makes crouch infinitely"));
				Button(SteamStrings("Hide Name"), &InvisName);
				CheckBox(SteamStrings("Knife Left Side"), &Config.KnifeLeftSide);
				CheckBox(SteamStrings("Key Visualization"), &Config.KeyVisualizer, SteamStrings("Visualize WSAD, Space buttons"));
				CheckBox(SteamStrings("Velocity"), &Config.velocity, SteamStrings("You can see you velocity"));
				CheckBox(SteamStrings("Force sv_cheats"), &Config.sv_cheats, SteamStrings("NOT RECOMMENDED ON VAC SECURE SERVERS!"));
				OffsetY = GroupTabPos[1] + 8;
				ControlsX = GroupTabPos[0];
				ControlsX2 = ControlsX + ((Width / 2) - 30);
			}

			SubTab(SteamStrings("Profile Changer"));
			{
#ifdef RELEASE
				ControlsX = ControlsX + 10;
				ComboBox(SteamStrings("MM Rank"), {
					"Off",
					"Silver 1",
					"Silver 2",
					"Silver 3",
					"Silver 4",
					"Silver elite",
					"Silver elite master",
					"Gold Nova 1",
					"Gold Nova 2",
					"Gold Nova 3",
					"Gold Nova Master",
					"Master Guardian 1",
					"Master Guardian 2",
					"Master Guardian Elite",
					"Distinguished Master Guardian",
					"Legendary Eagle",
					"Legendary Eagle Master",
					"Supreme Master Ffirst Class",
					"The Global Elite"
					}, &Config.RankChanger.rank_id);

				ControlsX = ControlsX - 10;
				OffsetY = GroupTabPos[1] + 28;
				ControlsX = GroupTabPos[0] + (Width / 2);
				ControlsX2 = ControlsX + ((Width / 2) - 40);

				Slider(40, SteamStrings("Level"), &Config.RankChanger.player_level);
				Slider(5000, SteamStrings("Wins"), &Config.RankChanger.wins);
				Slider(5000, SteamStrings("Friend"), &Config.RankChanger.cmd_friendly);
				Slider(5000, SteamStrings("Teach"), &Config.RankChanger.cmd_teaching);
				Slider(5000, SteamStrings("Leader"), &Config.RankChanger.cmd_leader);
				Button(SteamStrings("                                                    Update                                                    "), &update_ranks);
#else
				Text(SteamStrings("DEBUG MODE"));
#endif			OffsetY = GroupTabPos[1] + 8;
				ControlsX = GroupTabPos[0];
				ControlsX2 = ControlsX + ((Width / 2) - 30);
		}
	}

		OffsetY = GroupTabPos[1] + 8;
		ControlsX = GroupTabPos[0];
		ControlsX2 = ControlsX + ((Width / 2) - 30);


		Tab(SteamStrings("H"));//Legit H
		{
			ControlsX = ControlsX + 10;
			CheckBox(SteamStrings("Skin changer"), &Config.Skinchanger.skinenabled);
			if (Config.Skinchanger.skinenabled)
			{
				ComboBox_Large("Weapon", { "AK-47","M4A4","Dual","M4A1-S","AWP","USP-S","Glock 18","Deagle","Five Seven","AUG","Famas","G3sg1","Galil-AR","M249","Mac-10","UMP-45","P2000","MP7","MP9","P250","SCAR 20","Sg553","SSG08","Revolver","P90" }, &Config.Skinchanger.globalskins);
				WeaponChooser();
			}
			ControlsX = ControlsX - 10;
			OffsetY = GroupTabPos[1] + 8;
			ControlsX = GroupTabPos[0] + (Width / 2);
			ControlsX2 = ControlsX + ((Width / 2) - 40);

			CheckBox(SteamStrings("Knife Changer"), &Config.knivchanger);
			if (Config.knivchanger)
			{
				ComboBox(SteamStrings("Knife Model"), { "Bayonet","Flip Knife","Gut Knife","Karambit","M9 Bayonet","Huntsman Knife","Butterfly Knife","Falchion Knife","Shadow Daggers","Bowie Knife","Navaja Knife","Stiletto Knife","Ursus Knife","Talon Knife", "Classic Knife", "Cord Knife", "Survival? Knife", "Skeleton Knife", "Nomad" }, &Config.Knife);
				ComboBox_Large(SteamStrings("Skin"), { "Vanilla","Crimson Web","Bone Mask","Fade","Night","Blue Steel","Stained","Case Hardened","Slaughter","Safari Mesh","Boreal Forest","Ultraviolet","Urban Masked","Scorched","Rust Coat","Tiger Tooth","Damascus Steel","Damascus Steel","Marble Fade","Rust Coat","Doppler Ruby","Doppler Sapphire","Doppler Blackpearl","Doppler","Doppler","Doppler","Doppler","Gamma Doppler","Gamma Doppler","Gamma Doppler","Gamma Doppler","Gamma Doppler","Lore","Black Laminate","Autotronic","Freehand","Doppler 1","Doppler 2","Doppler 3","Doppler 4","Marble Fade_new","Damascus Steel" }, &Config.KnifeSkin);
			}
			Space(1);
			Button(SteamStrings("                                                    Update                                                   "), &forceupdate);
		}

		OffsetY = GroupTabPos[1] + 8;
		ControlsX = GroupTabPos[0];
		ControlsX2 = ControlsX + ((Width / 2) - 30);

		Tab(SteamStrings("G"));//Legit H
		{
			ControlsX = ControlsX + 10;
			static bool overridemenucolor;
			CheckBox(SteamStrings("Trusted mode"), &Config.trustedmode);
			CheckBox(SteamStrings("Accent Color"), &overridemenucolor, SteamStrings("You can change the menu color"));
			if (overridemenucolor)
			{
				ColorPicker(SteamStrings("Accent"), Config.MenuColor);
			}
			CheckBox(SteamStrings("Console color modulate"), &Config.ColorModulateConsole);
			if (Config.ColorModulateConsole)
			{
				ColorPicker(SteamStrings("Console"), Config.ConsoleColor);
			}
			CheckBox(SteamStrings("Walk With Menu"), &Config.walkwithmenu);
			Space(10);

			ComboBox(SteamStrings("Config"), { SteamStrings("Legit 1"),SteamStrings("Legit 2"),SteamStrings("Semi Rage"), SteamStrings("HvH 1"),SteamStrings("HvH 2"), SteamStrings("Misc 1"), SteamStrings("Misc 2"), SteamStrings("Misc 3"), SteamStrings("Misc 4"), SteamStrings("Misc 5") }, &Config.CurrentConfig);

			Space(5);

			Button(SteamStrings("                                                    Save                                                      "), &save);
			Space(2);
			Button(SteamStrings("                                                    Load                                                      "), &load);
			Space(60);
			/*Button(SteamStrings("                                                   Forum                                                     "), &OpenForum);
			Space(1);*/
			Button(SteamStrings("                                                Unload cheat                                              "), &unload);
		}


		TabSize = TabOffset;
		SubTabSize = SubTabOffset;
}
}

Color Shade(Color clr, float factor)
{
	Color newcolor(255, 255, 255);
	newcolor.red = clr.red * (1 - factor);
	newcolor.green = clr.green * (1 - factor);
	newcolor.blue = clr.blue * (1 - factor);

	return newcolor;
}

void Menu::Tab(std::string name)
{
	static unsigned int last_timee;
	last_timee = GetTickCount();
	Color rainbowfading;
	rainbowfading.FromHSV(fmod(last_timee * 0.0002f, 1.f), 1.f, 0.5f);

	int height = 45;
	int width = (MenuWidth - 2) / 6;
	int TabArea[4] = { Pos.x - 125 , Pos.y + (TabOffset * height), width, height };
	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(TabArea[0], TabArea[1], TabArea[2] - 10, TabArea[3]))
		TabNum = TabOffset;

	//Gray menu background box
	GroupBox(TabArea[0] + 8, TabArea[1] + 2, TabArea[2] - 19, TabArea[3] - 2, g::Tahoma, "tabs", false); {}

	TabX = TabArea[0] + 8;
	TabY = TabArea[1] + 2;
	TabW = TabArea[2] - 19;
	TabH = TabArea[3] - 2;

	if (TabOffset == TabNum)
	{
		GroupBox_Active(TabArea[0] + 8, TabArea[1] + 2, TabArea[2] - 19, TabArea[3] - 2, g::Tahoma, "tabs", false); {}
		g_pSurface->DrawT(TabArea[0] + (TabArea[2] / 2), TabArea[1] + 10, menu_text, g::Icons, true, name.c_str());
	}
	else
	{
		g_pSurface->DrawT(TabArea[0] + (TabArea[2] / 2), TabArea[1] + 10, menu_text, g::Icons, true, name.c_str());
	}

	TabOffset += 1;
	PreviousControl = -1;
}

void Menu::SubTab(std::string name)
{
	if (TabOffset - 1 != TabNum || TabOffset == 0)
		return;

	static int TabSkip = 0;

	if (SubTabSize != 0 && TabSkip == TabNum)
	{

		int TabLength = (GroupTabPos[2] / SubTabSize);

		int GroupTabArea[4] = { (GroupTabPos[0]) + (TabLength * SubTabOffset), GroupTabPos[1], TabLength, 20 };

		if ((GroupTabArea[0] + GroupTabArea[3]) <= (GroupTabPos[0] + GroupTabPos[2]))
		{
			if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(GroupTabArea[0], GroupTabArea[1], GroupTabArea[2], GroupTabArea[3]))
				SubTabNum = SubTabOffset;

			int Offset = ((SubTabSize - 1) == SubTabOffset) ? 0 : 1;

			if (((SubTabSize - 1) == SubTabOffset) && (((TabLength * SubTabSize) > GroupTabPos[2]) || ((TabLength * SubTabSize) < GroupTabPos[2])))
				Offset = (GroupTabPos[2] - (TabLength * SubTabSize));

			if (SubTabNum == SubTabOffset)
			{
				g_pSurface->FilledRect(GroupTabArea[0], GroupTabArea[1], GroupTabArea[2], GroupTabArea[3], menu_background2);
			}

			g_pSurface->DrawT(GroupTabArea[0] + (GroupTabArea[2] / 2), GroupTabArea[1] + 4, menu_text, g::SansSerif, true, name.c_str());
		}
	}

	if (TabSkip != TabNum) // frame skip for drawing
		TabSkip = TabNum;

	if (SubTabOffset == SubTabNum)
		OffsetY += 20;

	SubTabOffset += 1;
	PreviousControl = -1;
}

bool dont_recieve_input = false;

void Menu::ColorPicker(std::string name, ColorV2& item)
{
	if (GroupTabBottom <= OffsetY + 16)
		return;

	if (TabOffset - 1 != TabNum || TabOffset == 0)
		return;

	if (SubTabOffset != 0)
		if (SubTabOffset - 1 != SubTabNum)
			return;

	if (PreviousControl == slider || PreviousControl == -1)
		return;

	int CtrXoffset = 132;
	int yoffset = OldOffsetY + 6;
	int xoffset = ControlsX;

	int width, height, halfwidth, halfheight, xpicker;
	g_pEngine->GetScreenSize(width, height);
	halfwidth = (width / width);
	halfheight = (height / height);
	xpicker = halfwidth;

	Color rainbow;

	bool pressed = false;
	bool open = false;
	static bool selectedOpened = false;
	static bool clickRest;
	static bool rest;
	static std::string nameSelected;

	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + CtrXoffset, OldOffsetY + 3, 20, 12) && !clickRest)
	{
		nameSelected = name;
		pressed = true;
		clickRest = true;
	}
	else if (!GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + CtrXoffset, OldOffsetY + 3, 20, 12))
		clickRest = false;

	if (pressed)
	{
		if (!rest)
			selectedOpened = !selectedOpened;

		rest = true;
	}
	else
		rest = false;

	if (nameSelected == name)
		open = selectedOpened;

	if (open)
	{
		//g_pSurface->RoundedFilledRect(Pos.x, 50, 500, 32, menu_background2);
		g_pSurface->DrawT(Pos.x, 24, menu_text, g::ToolTip, false, "Choose your color"); //568
		g_pSurface->RoundedFilledRect(Pos.x, 48, 802, 35, 5, Color(0, 0, 0, 255));
		//g_pSurface->RoundedFilledRect(xoffset, OldOffsetY + 100, 100, 20, 5, Color(255, 255, 255, 255));

		if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(xoffset, OldOffsetY, 100, 10))
		{
			item.red = 0;
			item.green = 0;
			item.blue = 0;
			item.alpha = 255;
		}

		if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(xoffset, OldOffsetY + 110, 100, 10))
		{
			item.red = 255;
			item.green = 255;
			item.blue = 255;
			item.alpha = 255;
		}

		for (int i = 0; i < 78; i++)
		{
			if (xpicker >= ControlsX + 780)
			{
				//xoffset -= 100;
				//yoffset += 10;
			}

			float hue = (i * .01f);

			rainbow.FromHSV(hue, 1.f, 1.f);

			g_pSurface->FilledRect(Pos.x + xpicker, 50, 30, 30, rainbow);

			if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(Pos.x + xpicker, 50, 30, 30))
			{
				item.red = rainbow.red;
				item.green = rainbow.green;
				item.blue = rainbow.blue;
				item.alpha = 255.f;
			}

			xpicker += 10;
		}
	}

	rainbow.red = item.red;
	rainbow.green = item.green;
	rainbow.blue = item.blue;
	rainbow.alpha = 255;

	g_pSurface->FilledRect(ControlsX + CtrXoffset, OldOffsetY + 3, 20, 12, rainbow);
	g_pSurface->OutlinedRect(ControlsX + CtrXoffset, OldOffsetY + 3, 20, 12, menu_background2);
}

void Menu::color_selector(std::string name, int* red, int* green, int* blue, int* alpha)
{
	if (GroupTabBottom <= OffsetY + 16)
		return;

	if (TabOffset - 1 != TabNum || TabOffset == 0)
		return;

	if (SubTabOffset != 0)
		if (SubTabOffset - 1 != SubTabNum)
			return;

	if (PreviousControl == slider || PreviousControl == -1)
		return;

	int CtrXoffset = 0;

	if (PreviousControl != check_box)
		CtrXoffset = 132;
	else
		CtrXoffset = 165;

	int yoffset = OldOffsetY + 6;
	int xoffset = ControlsX + 330;


	bool pressed = false;
	bool open = false;
	static bool selected_opened = false;
	static bool click_rest;
	static bool rest;
	static std::string name_selected;

	int ComboSize = 127; // 163
	int ComboSizeH = 127; // 163
	static int offset = 0;
	if (name == "sadasd")
	{
		offset = 0;
	}
	else
	{
		offset = 14;
	}

	if (menuOpened)
	{
		if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(x_offset + 225, y_offset + 1, 15, 8) && !
			click_rest)
		{
			name_selected = name;
			pressed = true;
			click_rest = true;
		}
		else if (!GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(x_offset + 225, y_offset + 1, 15, 8))
			click_rest = false;

		if (pressed)
		{
			if (!rest)
				selected_opened = !selected_opened;
			rest = true;
		}
		else
			rest = false;

		if (name_selected == name)
			open = selected_opened;
	}


	static unsigned int r = 255;
	static unsigned int g = 0;
	static unsigned int b = 0;


	static unsigned int selected_r = 0;
	static unsigned int selected_g = 0;
	static unsigned int selected_b = 0;

	static Vector2D MousePos;
	COLORREF color;
	COLORREF selected_color;

	if (open)
	{
		dont_recieve_input = true;
		g_pSurface->FilledRect(x_offset + 225 - ComboSize - 5, y_offset, ComboSize, ComboSizeH,
			Color(20, 20, 20, GlobalOpacity));

		g_pSurface->OutlinedRect(x_offset + 225 - ComboSize - 5, y_offset, ComboSize, ComboSizeH,
			Color(42, 46, 46, GlobalOpacity));
		g_pSurface->OutlinedRect(x_offset + 225 - ComboSize - 5 - 1, y_offset - 1, ComboSize + 2, ComboSizeH + 2,
			Color(34, 38, 37, GlobalOpacity));

		float _pixel_value = 255 / (ComboSize - 5 - 17);

		//if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(x_offset + 20 - 3, y_offset + 14, SliderSize + (3 * 2), 8)) {
		//	*item = abs(g_pSurface->GetMousePosition().x - (x_offset + 20)) * _pixel_value;
		//}

		if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(x_offset + 225 - ComboSize - 5 + 5,
			y_offset + ComboSizeH - 11, (ComboSize - 5 - 17),
			7) && menuOpened)
		{
			*alpha = abs(g_pSurface->GetMousePosition().x - (x_offset + 225 - ComboSize - 5 + 5)) * _pixel_value;
		}


		if (*alpha < 3)
		{
			*alpha = 0;
		}

		if (*alpha > 204)
		{
			*alpha = 255;
		}

		g_pSurface->FilledRect(x_offset + 225 - ComboSize - 5 + 5, y_offset + ComboSizeH - 11, ComboSize - 5 - 17, 7,
			Color(255, 255, 255, *alpha));
		g_pSurface->OutlinedRect(x_offset + 225 - ComboSize - 5 + 5, y_offset + ComboSizeH - 11, ComboSize - 5 - 17, 7,
			Color(0, 0, 0, GlobalOpacity));


		if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(x_offset + 233 - ComboSize + 101 + 1,
			y_offset + 5 + 1, 7 - 2, 107 - 1))
		{
			HDC hdc = GetDC(nullptr);
			color = GetPixel(hdc, g_pSurface->GetMousePosition().x, g_pSurface->GetMousePosition().y);

			r = GetRValue(color);
			g = GetGValue(color);
			b = GetBValue(color);
		}


		GradientH(x_offset + 225 - ComboSize - 5 + 5, y_offset + 5, ComboSize - 5 - 17, ComboSizeH - 20,
			Color(255, 255, 255), Color(r, g, b));
		GradientVEnd(x_offset + 225 - ComboSize - 5 + 5, y_offset + 40, ComboSize - 5 - 17, ComboSizeH - 20,
			Color(0, 0, 0), Color(0, 0, 0));


		//g_pSurface->OutlinedRect(MousePos.x - 4, MousePos.y - 4, 8, 8, Color(0, 0, 0, MenuAlpha_Main));
		g_pSurface->OutlinedRect(x_offset + 225 - ComboSize - 5 + 5, y_offset + 5, ComboSize - 5 - 17, ComboSizeH - 20,
			Color(0, 0, 0, GlobalOpacity));

		if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(x_offset + 225 - ComboSize - 5 + 5 + 1,
			y_offset + 5 + 1, ComboSize - 5 - 17 - 2,
			ComboSizeH - 20 - 2))
		{
			HDC hdc = GetDC(nullptr);
			selected_color = GetPixel(hdc, g_pSurface->GetMousePosition().x, g_pSurface->GetMousePosition().y);

			selected_r = GetRValue(selected_color);
			selected_g = GetGValue(selected_color);
			selected_b = GetBValue(selected_color);

			*red = selected_r;
			*green = selected_g;
			*blue = selected_b;

			MousePos = Vector2D(g_pSurface->GetMousePosition().x, g_pSurface->GetMousePosition().y);
		}

		Color rainbow; int hue;
		for (int i = 0; i < 100; i++)
		{
			float hue = (i * .01f);
			rainbow.FromHSV(hue, 1.f, 1.f);
			g_pSurface->FilledRect(x_offset + 233 - ComboSize + 101, y_offset + 5 + i, 7, 8, rainbow);
			g_pSurface->OutlinedRect(x_offset + 233 - ComboSize + 101, y_offset + 5, 7, 107, Color(0, 0, 0, 255));
		}
	}
	else
	{
		dont_recieve_input = false;
	}


	if (*red > 20 && *green > 20 && *blue > 20)
	{
		g_pSurface->FilledRect(x_offset + 225, y_offset, 15, 7, Color(*red, *green, *blue, GlobalOpacity));
	}
	else
	{
		g_pSurface->FilledRect(x_offset + 225, y_offset, 15, 7, Color(*red, *green, *blue, GlobalOpacity));
	}


	g_pSurface->OutlinedRect(x_offset + 225 - 1, y_offset - 1, 15 + 2, 7 + 2, Color(0, 0, 0, GlobalOpacity));

	if (open)
	{
		y_offset += ComboSizeH;
	}
}

void Menu::CheckBox(std::string name, bool* item, std::string tooltip)
{
	static unsigned int last_timee;
	last_timee = GetTickCount();
	Color rainbowfading;
	rainbowfading.FromHSV(fmod(last_timee * 0.0002f, 1.f), 1.f, 0.5f);

	if (GroupTabBottom <= OffsetY + 16)
		return;

	if (TabOffset - 1 != TabNum || TabOffset == 0)
		return;

	if (SubTabOffset != 0)
		if (SubTabOffset - 1 != SubTabNum)
			return;

	static bool pressed = false;

	if (!GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX2 - 32, OffsetY + 6, 30, 12))
	{
		if (pressed)
			*item = !*item;
		pressed = false;
	}

	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX2 - 32, OffsetY + 6, 30, 12) && !pressed)
		pressed = true;

	g_pSurface->FilledRect(ControlsX2 - 32, OffsetY + 6, 24, 12, menu_background2);

	if (*item == true)
	{
		g_pSurface->FilledRect(ControlsX2 - 32, OffsetY + 6, 30, 12, Color(0, 255, 0));
	}
	else
	{
		g_pSurface->FilledRect(ControlsX2 - 32, OffsetY + 6, 30, 12, Shade(menu_accent, 0.6));
	}


	g_pSurface->DrawT(ControlsX, OffsetY + 6, menu_text, g::SansSerif, false, name.c_str());

	if (g_pSurface->MouseInRegion(ControlsX2 - 32, OffsetY + 6, 30, 12))
	{
		if (*item == true)
		{
			g_pSurface->OutlinedRect(ControlsX2 - 32, OffsetY + 4, 32, 16, menu_accent);
		}
		else
		{
			g_pSurface->OutlinedRect(ControlsX2 - 32, OffsetY + 4, 32, 16, Color(0, 255, 0));
		}
		Vector halfPos;
		halfPos.x = (Pos.x / Pos.x);

		if (!tooltip.empty())
		{
			Vector2D MousePos = g_pSurface->GetMousePosition();
			g_pSurface->DrawT(Pos.x, Pos.y + Height, menu_text, g::ToolTip, false, tooltip.c_str()); //568
		}
	}

	OldOffsetY = OffsetY;
	OffsetY += 24;
	PreviousControl = check_box;
}

void Menu::Slider(int max, std::string name, int* item)
{
	static unsigned int last_timee;
	last_timee = GetTickCount();
	Color rainbowfading;
	rainbowfading.FromHSV(fmod(last_timee * 0.0002f, 1.f), 1.f, 0.5f);

	if (GroupTabBottom <= OffsetY + 16)
		return;

	if (TabOffset - 1 != TabNum || TabOffset == 0)
		return;

	if (SubTabOffset != 0)
		if (SubTabOffset - 1 != SubTabNum)
			return;

	RECT TextSize = g_pSurface->GetTextSizeRect(g::SansSerif, std::to_string(*item).c_str());

	int width = (ControlsX2 - ControlsX);

	float pixelValue = max / 360.f;//this fixed sliders

	/*g_pSurface->DrawT(ControlsX - 8, OffsetY + 22, menu_text, g::Tahoma, false, "-");
	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX - 8, OffsetY + 22, 8, 8))
		*item -= 1;

	g_pSurface->DrawT(ControlsX + width + 4, OffsetY + 22, menu_text, g::Tahoma, false, "+");
	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + width + 4, OffsetY + 22, 8, 8))
		*item += 1;*/

	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX - 6, OffsetY + 24, width + 6, 10))
		*item = (g_pSurface->GetMousePosition().x - (ControlsX - 4)) * pixelValue;

	if (*item > max)
		*item = max;
	if (*item < 0)
		*item = 0;

	g_pSurface->DrawT(ControlsX - 2, OffsetY + 6, menu_text, g::SansSerif, false, name.c_str());
	g_pSurface->DrawT(ControlsX2 - TextSize.right - 2, OffsetY + 6, menu_text, g::SansSerif, true, std::to_string(*item).c_str());

	g_pSurface->RoundedFilledRect(ControlsX - 2, OffsetY + 24, width, 10, 4, menu_background2);

	g_pSurface->RoundedFilledRect(ControlsX - 2, OffsetY + 24, *item / pixelValue, 10, 4, menu_accent);
	g_pSurface->FilledRect((ControlsX + (*item / pixelValue)) - 5, OffsetY + 23, 8, 12, Color(255, 255, 255));

	OldOffsetY = OffsetY;
	OffsetY += 36;
	PreviousControl = slider;
}

void Menu::SliderFloat(float max, std::string name, float* item)
{
	static unsigned int last_timee;
	last_timee = GetTickCount();
	Color rainbowfading;
	rainbowfading.FromHSV(fmod(last_timee * 0.0002f, 1.f), 1.f, 0.5f);

	if (GroupTabBottom <= OffsetY + 16)
		return;

	if (TabOffset - 1 != TabNum || TabOffset == 0)
		return;

	if (SubTabOffset != 0)
		if (SubTabOffset - 1 != SubTabNum)
			return;

	RECT TextSize = g_pSurface->GetTextSizeRect(g::SansSerif, std::to_string(*item).c_str());

	int width = ControlsX2 - ControlsX;

	float pixelValue = max / 265.f;

	g_pSurface->DrawT(ControlsX - 8, OffsetY + 22, menu_text, g::Tahoma, false, "-");
	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX - 8, OffsetY + 22, 4, 8))
		*item -= 1;

	g_pSurface->DrawT(ControlsX + width + 4, OffsetY + 22, menu_text, g::Tahoma, false, "+");
	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + width + 4, OffsetY + 22, 4, 8))
		*item += 1;

	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX - 2, OffsetY + 24, width, 8))
		*item = (g_pSurface->GetMousePosition().x - (ControlsX - 2)) * pixelValue;

	if (*item > max)
		*item = max;
	if (*item < 0.f)
		*item = 0.f;

	g_pSurface->DrawT(ControlsX, OffsetY + 6, menu_text, g::SansSerif, false, name.c_str());
	g_pSurface->DrawT(ControlsX2 - TextSize.right, OffsetY + 6, menu_text, g::SansSerif, true, std::to_string(*item).c_str());

	g_pSurface->RoundedFilledRect(ControlsX, OffsetY + 24, width, 8, 4, menu_background2);


	g_pSurface->RoundedFilledRect(ControlsX, OffsetY + 24, *item / pixelValue, 8, 4, menu_accent);

	g_pSurface->FilledRect((ControlsX + (*item / pixelValue)) - 3, OffsetY + 22, 6, 12, Color(255, 255, 255));

	OldOffsetY = OffsetY;
	OffsetY += 36;
	PreviousControl = sliderfloat;
}

void Menu::ComboBox(std::string name, std::vector< std::string > itemname, int* item)
{
	if (GroupTabBottom <= OffsetY + 16)
		return;

	if (TabOffset - 1 != TabNum || TabOffset == 0)
		return;

	if (SubTabOffset != 0)
		if (SubTabOffset - 1 != SubTabNum)
			return;

	bool pressed = false;
	bool open = false;
	int i;
	int max;
	static bool selectedOpened = false;
	static bool clickRest;
	static bool rest;
	static std::string nameSelected;

	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + 175, OffsetY, 183, 16) && !clickRest)
	{
		nameSelected = name;
		pressed = true;
		clickRest = true;
	}
	else if (!GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + 175, OffsetY, 183, 16))
		clickRest = false;

	if (pressed)
	{
		if (!rest)
			selectedOpened = !selectedOpened;

		rest = true;
	}
	else
		rest = false;

	if (nameSelected == name)
		open = selectedOpened;

	int arrowpos = 240;

	g_pSurface->DrawT(ControlsX, OffsetY + 6, menu_text, g::SansSerif, false, name.c_str());

	if (g_pSurface->MouseInRegion(ControlsX + 175, OffsetY + 2, 183, 16))
	{
		g_pSurface->OutlinedRect(ControlsX + 175, OffsetY, 184, 20, Color(255, 255, 255));
	}


	if (open)
	{
		g_pSurface->FilledRect(ControlsX + 177, OffsetY + 2, 183, 16, menu_background2);
		g_pSurface->FilledRect(ControlsX + arrowpos + 4, OffsetY + 2 + 7, 5, 1, menu_background2);
		g_pSurface->FilledRect(ControlsX + arrowpos + 5, OffsetY + 2 + 8, 3, 1, menu_background2);
		g_pSurface->FilledRect(ControlsX + arrowpos + 6, OffsetY + 2 + 9, 1, 1, menu_background2);
		for (i = 0; i < itemname.size(); i++)
		{
			if (itemname.size() < 21)
			{

				g_pSurface->OutlinedRect(ControlsX + 175, OffsetY + 2, 183, 17 + (itemname.size() * 16), Color(menu_background2.red, menu_background2.green, menu_background2.blue, 100));
				if (*item == i)
					g_pSurface->FilledRect(ControlsX + 175, OffsetY + 2 + 16 + (i * 16), 183, 16, Color(255, 255, 255));
			}
			if (i < 21)
			{
				if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + 173, OffsetY + 2 + 16 + i * 16, 90, 16))
					*item = i;

				if (*item == i)
					g_pSurface->FilledRect(ControlsX + 175, OffsetY + 2 + 16 + (i * 16), 90, 16, Color(255, 255, 255));
				g_pSurface->DrawT(ControlsX + 181, OffsetY + 2 + 16 + (i * 16), Color(menu_text.red, menu_text.green, menu_text.blue, 255), g::SansSerif, false, itemname.at(i).c_str());
			}
			else
			{
				g_pSurface->OutlinedRect(ControlsX + 175, OffsetY + 2, 184, 17 + (21 * 16), Color(menu_background2.red, menu_background2.green, menu_background2.blue, 100));

				if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + 240, OffsetY + 2 + 16 + i * 16 - 337, 90, 16))
					*item = i;

				if (*item == i)
					g_pSurface->FilledRect(ControlsX + 262, OffsetY + 2 + 16 + (i * 16) - 337, 90, 16, Color(255, 255, 255));

				g_pSurface->DrawT(ControlsX + 268, OffsetY + 2 + 16 + (i * 16) - 337, Color(menu_text.red, menu_text.green, menu_text.blue, 255), g::SansSerif, false, itemname.at(i).c_str());
			}
		}
	}

	g_pSurface->DrawT(ControlsX + 181, OffsetY + 2, menu_text, g::SansSerif, false, itemname.at(*item).c_str());

	OldOffsetY = OffsetY;

	if (open)
		OffsetY += 24 + (itemname.size() * 16);
	else
		OffsetY += 24;

	PreviousControl = combo_box;
}
void Menu::ComboBox_Large(std::string name, std::vector< std::string > itemname, int* item)
{
	if (GroupTabBottom <= OffsetY + 16)
		return;

	if (TabOffset - 1 != TabNum || TabOffset == 0)
		return;

	if (SubTabOffset != 0)
		if (SubTabOffset - 1 != SubTabNum)
			return;

	bool pressed = false;
	bool open = false;
	int i;
	int max;
	static bool selectedOpened = false;
	static bool clickRest;
	static bool rest;
	static std::string nameSelected;

	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + 175, OffsetY, 183, 16) && !clickRest)
	{
		nameSelected = name;
		pressed = true;
		clickRest = true;
	}
	else if (!GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + 175, OffsetY, 183, 16))
		clickRest = false;

	if (pressed)
	{
		if (!rest)
			selectedOpened = !selectedOpened;

		rest = true;
	}
	else
		rest = false;

	if (nameSelected == name)
		open = selectedOpened;

	int arrowpos = 240;

	g_pSurface->DrawT(ControlsX, OffsetY + 6, menu_text, g::SansSerif, false, name.c_str());

	if (g_pSurface->MouseInRegion(ControlsX + 175, OffsetY + 2, 183, 16))
	{
		g_pSurface->OutlinedRect(ControlsX + 175, OffsetY, 184, 20, Color(255, 255, 255));
	}


	if (open)
	{
		g_pSurface->FilledRect(ControlsX + 177, OffsetY + 2, 183, 16, menu_background2);
		g_pSurface->FilledRect(ControlsX + arrowpos + 4, OffsetY + 2 + 7, 5, 1, menu_background2);
		g_pSurface->FilledRect(ControlsX + arrowpos + 5, OffsetY + 2 + 8, 3, 1, menu_background2);
		g_pSurface->FilledRect(ControlsX + arrowpos + 6, OffsetY + 2 + 9, 1, 1, menu_background2);
		for (i = 0; i < itemname.size(); i++)
		{
			if (i < 21)
			{
				if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + 173, OffsetY + 2 + 16 + i * 16, 90, 16))
					*item = i;

				if (*item == i)
					g_pSurface->FilledRect(ControlsX + 175, OffsetY + 2 + 16 + (i * 16), 90, 16, Color(255, 255, 255));
				g_pSurface->DrawT(ControlsX + 181, OffsetY + 2 + 16 + (i * 16), Color(menu_text.red, menu_text.green, menu_text.blue, 255), g::SansSerif, false, itemname.at(i).c_str());
			}
			else
			{
				g_pSurface->OutlinedRect(ControlsX + 175, OffsetY + 2, 184, 17 + (21 * 16), Color(menu_background2.red, menu_background2.green, menu_background2.blue, 100));

				if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + 262, OffsetY + 2 + 16 + i * 16 - 337, 90, 16))
					*item = i;

				if (*item == i)
					g_pSurface->FilledRect(ControlsX + 262, OffsetY + 2 + 16 + (i * 16) - 337, 92, 16, Color(255, 255, 255));

				g_pSurface->DrawT(ControlsX + 268, OffsetY + 2 + 16 + (i * 16) - 337, Color(menu_text.red, menu_text.green, menu_text.blue, 255), g::SansSerif, false, itemname.at(i).c_str());
			}
		}
	}

	g_pSurface->DrawT(ControlsX + 181, OffsetY + 2, menu_text, g::SansSerif, false, itemname.at(*item).c_str());

	OldOffsetY = OffsetY;

	if (open)
		OffsetY += 24 + (itemname.size() * 16);
	else
		OffsetY += 24;

	PreviousControl = combo_box;
}


void Menu::KeyBind(std::string name, int* item, std::string tooltip)
{
	if (ControlsReturn)
		return;
	if (GroupTabBottom <= OffsetY + 16)
		return;

	if (TabOffset - 1 != TabNum || TabOffset == 0)
		return;

	if (SubTabOffset != 0)
		if (SubTabOffset - 1 != SubTabNum)
			return;

	static bool gotKey = false;
	bool pressed = false;
	if (gotKey)
		pressed = true;
	gotKey = false;
	bool open = false;
	static bool selectedOpened = false;
	static bool clickRest;
	static bool rest;
	static std::string nameSelected;

	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + 172, OffsetY, 183, 16) && !clickRest)
	{
		nameSelected = name;
		pressed = true;
		clickRest = true;
	}
	else if (!GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + 172, OffsetY, 183, 16))
		clickRest = false;

	if (pressed)
	{
		if (!rest)
			selectedOpened = !selectedOpened;

		rest = true;
	}
	else
		rest = false;

	if (nameSelected == name)
		open = selectedOpened;

	g_pSurface->DrawT(ControlsX, OffsetY, menu_text, g::SansSerif, false, name.c_str());
	g_pSurface->AutoGradientRect(ControlsX + 175, OffsetY, 183, 16, Color(119, 119, 119, 255));

	if (g_pSurface->MouseInRegion(ControlsX + 175, OffsetY, 183, 16))
	{
		if (!tooltip.empty())
		{
			Vector2D MousePos = g_pSurface->GetMousePosition();
			g_pSurface->DrawT(Pos.x, Pos.y, menu_text, g::ToolTip, false, tooltip.c_str()); //568
		}
	}
	if (open)
	{
		for (int i = 0; i < 255; i++)
		{
			if ((GetAsyncKeyState(i) & 0x8000) && i != 1 && i != 45)
			{
				*item = i;
				gotKey = true;
			}
		}
		g_pSurface->AutoGradientRect(ControlsX + 175, OffsetY, 183, 16, Color(53, 53, 53, 255));
	}

	g_pSurface->DrawT(ControlsX + 175, OffsetY, Color(255, 255, 255, 255), g::SansSerif, false, std::string(1, char(*item)).c_str()); // K LOL

	OldOffsetY = OffsetY;
	OffsetY += 18;

	PreviousControl = key_bind;
}


void Menu::Button(std::string text, Menu::ButtonCallback_t callback)
{
	static unsigned int last_timee;
	last_timee = GetTickCount();
	Color rainbowfading;
	rainbowfading.FromHSV(fmod(last_timee * 0.0002f, 1.f), 1.f, 0.5f);

	if (GroupTabBottom <= OffsetY + 16)
		return;

	if (TabOffset - 1 != TabNum || TabOffset == 0)
		return;

	if (SubTabOffset != 0)
		if (SubTabOffset - 1 != SubTabNum)
			return;

	bool pressed = false;
	static bool clickRest;
	static bool rest;

	int height = 20;
	int width = 360;

	if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX, OffsetY + 6, width, height) && !clickRest)
	{
		pressed = true;
		clickRest = true;
		g_pSurface->FilledRect(ControlsX, OffsetY + 6, width, height, menu_accent);
	}
	else if (!GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX, OffsetY + 5, width, height))
		clickRest = false;

	if (g_pSurface->MouseInRegion(ControlsX, OffsetY + 6, width, height))
	{
		g_pSurface->OutlinedRect(ControlsX - 2, OffsetY + 4, width + 4, height + 4, Color(255, 255, 255));
	}

	if (pressed)
	{
		if (!rest)
			callback();

		rest = true;
	}
	else
	{
		rest = false;

		g_pSurface->FilledRect(ControlsX, OffsetY + 6, width, height, menu_accent);
	}

	g_pSurface->DrawT(ControlsX + (width / 2) - 2, OffsetY + 8, menu_text, g::SansSerif, true, text.c_str());

	OldOffsetY = OffsetY;

	OffsetY += 32;

	PreviousControl = button;
}

void Menu::Space(int spaceval)
{
	OffsetY += spaceval;
	PreviousControl = spacer;
}

bool keys[256];
bool oldKeys[256];

bool GetKeyPress(unsigned int key)
{
	if (keys[key] == true && oldKeys[key] == false)
		return true;
	return false;
}

void Menu::TextBox(std::string name, std::string* text)
{
	if (TabOffset - 1 != TabNum || TabOffset == 0)
		return;

	if (SubTabOffset != 0)
		if (SubTabOffset - 1 != SubTabNum)
			return;

	if (GroupTabBottom <= OffsetY + 16)
		return;

	bool pressed = false;
	bool open = false;
	static bool selected_opened = false;
	static bool click_rest;
	static bool rest;
	static std::string name_selected;

	int ComboSize = 127; // 163
	int ComboSizeH = 127; // 163
	static int offset = 0;
	if (name == "")
	{
		offset = 0;
	}
	else
	{
		offset = 14;
	}

	std::copy(keys, keys + 255, oldKeys);
	for (int x = 0; x < 255; x++)
	{
		//oldKeys[x] = oldKeys[x] & keys[x];
		keys[x] = (GetAsyncKeyState(x));
	}


	if (menuOpened)
	{
		if (GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + 225, OffsetY + 1, 15, 8) && !
			click_rest)
		{
			name_selected = name;
			pressed = true;
			click_rest = true;
		}
		else if (!GetAsyncKeyState(VK_LBUTTON) && g_pSurface->MouseInRegion(ControlsX + 225, OffsetY + 1, 15, 8))
			click_rest = false;

		if (pressed)
		{
			if (!rest)
				selected_opened = !selected_opened;
			rest = true;
		}
		else
			rest = false;

		if (name_selected == name)
			open = selected_opened;
	}

	const char* strg = text->c_str();
	if (open)
	{
		for (int i = 0; i < 255; i++)
		{
			if (GetKeyPress(i))
			{
				if (i == VK_ESCAPE || i == VK_RETURN || i == VK_INSERT)
				{
					open = false;
					return;
				}

				if (i == VK_BACK && strlen(strg) != 0)
				{
					*text = text->substr(0, strlen(strg) - 1);
				}

				if (strlen(strg) < 6 && (i == 0x30 || i == 0x31 || i == 0x32 || i == 0x33 || i == 0x34 || i == 0x35 || i
					== 0x36 || i == 0x37 || i == 0x38 || i == 0x39 || i == VK_NUMPAD0 || i == VK_NUMPAD1 || i ==
					VK_NUMPAD2 || i == VK_NUMPAD3 || i == VK_NUMPAD4 || i == VK_NUMPAD5 || i == VK_NUMPAD6 || i ==
					VK_NUMPAD7 || i == VK_NUMPAD8 || i == VK_NUMPAD9))
				{
					*text = *text + g_Hotkeys.KeyDigits[i];
					return;
				}
			}
		}
	}

	const char* cstr = text->c_str();
	g_pSurface->FilledRect(ControlsX + 20, OffsetY, ComboSize, 20, button_dark);
	g_pSurface->OutlinedRect(ControlsX + 20, OffsetY, ComboSize, 20, menu_text);
	g_pSurface->DrawT(ControlsX + 20 + 5, OffsetY + 3, menu_text, g::SansSerif, false, cstr);

	OffsetY += 29;
}

void Menu::Text(std::string text)
{
	if (GroupTabBottom <= OffsetY + 16)
		return;

	if (TabOffset - 1 != TabNum || TabOffset == 0)
		return;

	if (SubTabOffset != 0)
		if (SubTabOffset - 1 != SubTabNum)
			return;
	g_pSurface->DrawT(ControlsX, OffsetY + 6, menu_text, g::SansSerif, false, text.c_str());
	OffsetY += 24;
	PreviousControl = label;
}