#pragma once
#include <Windows.h>
#include <string>
#include <filesystem>
#include "../Utils/xor.h"

namespace fs = std::filesystem;

struct ColorV2
{
	float red, green, blue, alpha;
};


enum
{
	check_box,
	slider,
	combo_box,
	multi_box

};
class Menu
{
public:
	bool menuOpened;
	void WaterMark();
	void Render();

	struct
	{
		//Legit
		bool LegitAimbot;
		int LegitAimKey;
		bool LegitBacktrack;
		bool RecoilControlSystem;
		int RCS_X;
		int RCS_Y;
		int LegitAimFov;
		int AimSpeed;
		int LegitAimHitbox;
		bool LegitAA;
		int Fliplegitaa;
		bool esv;

		//Rage
		bool Aimbot;
		bool SilentAim;
		int  AimMode;
		bool OnKeyAim;
		int  AimHotkey;
		bool FriendlyFire;
		int  Hitchance;
		int  HitchanceValue;
		int  Mindmg;
		bool Resolver;
		bool Resolver_static;
		bool Resolverr;
		bool ResolverBeta;
		int  BodyScale;
		int  HeadScale;
		bool ForceBaim;
		int fbaimkey;
		int BaimAfterXShots;
		int BaimAfterXMissedShots;
		int test = 3;
		bool DelayShot;
		bool IgnoreLimbs;
		bool Autostop;
		int customstop;
		int AutoStopMode;
		bool AutoScope;
		bool AutoRevolver = true;
		bool Doubletap;
		int rtime = 2;
		bool Hideshots;
		int hideshotskey;
		bool AutomaticWeapons;
		bool PosBacktrack;
		int posbacktrk_option;
		bool BaimLethal;
		bool BaimPitch;
		bool BaimInAir;
		bool Jumpscout;

		bool Antiaim;
		int AntiAimMode;
		int JitterRange;
		bool FakeSide;
		bool LegitSide;
		int FakeSideHotkey;
		int FakeDuckKey;
		bool FakeDuck;
		int LegitSideHotkey;
		int PitchMode;
		int YawMode;
		int CustomPitchValue;
		bool LbyBreaker;
		int Lbymode;
		int LbyMult;
		int LbyAdd;
		bool AntiAimKeys;
		bool RealAngles;
		bool DesyncAngle;
		int DesyncStyle;
		int DesyncCustomStatic;
		int DesyncMode;
		float DesyncY;
		float MaxDesyncDelta;
		bool ZeroPitchOnLand;
		bool CustomPitchOnLand;
		int LandPitch;
		bool AntiAimIndicator;
		bool AdvancedAAIndicator;
		bool SlideWalk;
		int	FakeLagInAir;
		int FakeLagOnGround;
		bool FakeLagOnPeek;
		bool BreakLagComp;
		bool ChokeShotOnPeek;
		bool sv_cheats;

		bool triggerbot_recoil;
		bool triggerbot_deadmatch;
		bool trigetbot_enable;
		int TrigerKey;
		bool triggerbotbox_head;
		bool triggerbotbox_chest;
		bool triggerbotbox_stomach;
		bool triggerbotbox_arms;
		bool triggerbotbox_legs;

		bool Airstuck;
		int Airstuckkey;
		bool ShowServerHitboxes;
		bool aspectratio;

		//Spectator list
		int SpecList;
		struct SpecPos
		{
			float x = 100.f, y = 100.f;
		}SpecPos;

		int Esp;
		bool TeamEsp;
		bool SkeletonEsp;
		ColorV2 SkeletonColor = { 255.f,255.f,255.f,255.f };

		bool HbEsp;
		bool galaxysky;

		int Name;
		bool YawPitch;
		bool Money;
		bool Health;
		int HealthBar;
		int HealthStyle;
		int Weapon;
		bool Box;
		bool skeldead;
		bool zeusrange;

		bool OffscreenEsp;
		bool AlwaysIndicate;
		ColorV2 OffscreenEspColor = { 255.f,255.f,255.f,255.f };
		ColorV2 FovColor = { 255.f,255.f,255.f,255.f };
		ColorV2 NameColor = { 255.f,255.f,255.f,255.f };
		ColorV2 HealthColor = { 255.f,255.f,255.f,255.f };
		ColorV2 HealthBarColor = { 255.f,255.f,255.f,255.f };
		ColorV2 WeaponColor = { 255.f,255.f,255.f,255.f };
		ColorV2 YawPitchColor = { 255.f,255.f,255.f,255.f };
		ColorV2 MoneyColor = { 255.f,255.f,255.f,255.f };
		ColorV2 fakeduckcolor = { 255.f,255.f,255.f,255.f };
		ColorV2 BoxColor = { 255.f,255.f,255.f,255.f };
		ColorV2 AmmoEspColor = { 255.f,255.f,255.f,255.f };

		bool EnemyGlow;
		ColorV2 EnemyGlowColor = { 255.f,255.f,255.f,255.f };
		int EnemyGlowOpacity;

		bool TeamGlow;
		ColorV2 TeamGlowColor = { 255.f,255.f,255.f,255.f };
		int TeamGlowOpacity;

		bool LocalGlow;
		ColorV2 LocalGlowColor = { 255.f,255.f,255.f,255.f };
		int LocalGlowOpacity;

		bool Nightmode;
		int preset = 0;
		bool ProperNightmode;

		bool AmbientLight;
		ColorV2 AmbientLightColor = { 255.f, 255.f, 255.f, 255.f };

		// Chams

		bool viewmodelchams;
		bool ArmsChams;
		ColorV2 ArmsChamsColor = { 255.f,255.f,255.f,255.f };
		ColorV2 Glowoverlaycolor = { 255.f,255.f,255.f,255.f };
		ColorV2 PearlColor = { 255.f,255.f,255.f,255.f };
		int ArmsChamsTransparency;
		int name_esp_color_r = 255, name_esp_color_g = 255, name_esp_color_b = 255, name_esp_color_a = 255;

		bool WeaponChams;
		ColorV2 WeaponChamsColor = { 255.f,255.f,255.f,255.f };
		int WeaponChamsTransparency;

		int ChamsMaterial;
		int ChamSelectbox;

		bool redglowchams;
		bool GlowChams;
		bool EnemyChams;
		bool EnemyXQZChams;
		bool EnemyVisOnly;
		int EnemyChamsMode;
		ColorV2 EnemyVisChamsColor = { 255.f,255.f,255.f,255.f };
		ColorV2 EnemyXQZChamsColor = { 255.f,255.f,255.f,255.f };
		int LocalTransparency;
		int scopeblend;
		int EnemyChamsTransparency;
		int TeamChamsTransparency;
		ColorV2 HandGlowColor = { 255.f,255.f,255.f };



		int BacktrackingChams;
		ColorV2 BacktrackChamsColor = { 255.f,255.f,255.f,255.f };

		bool TeamChams;
		bool TeamXQZChams;
		bool TeamVisOnly;
		ColorV2 TeamVisChamsColor = { 255.f,255.f,255.f,255.f };
		ColorV2 TeamXQZChamsColor = { 255.f,255.f,255.f,255.f };

		bool LocalChams;
		bool LocalXQZChams;
		ColorV2 LocalVisChamsColor = { 255.f,255.f,255.f,255.f };
		ColorV2 LocalXQZChamsColor = { 255.f,255.f,255.f,255.f };

		bool DesyncChams;
		ColorV2 DesyncChamsColor = { 255.f,255.f,255.f,255.f };
		int DesyncChamsTransparency;

		bool LbyBreakerChams;
		ColorV2 LbyBreakerChamsColor = { 255.f,255.f,255.f,255.f };
		int LbyBreakerChamsTransparency;


		// End Chams

		bool NoSmoke;
		bool NoFlash;

		bool NoZoom;
		bool NoScope;
		bool NoRecoil;
		bool DisablePostProcess;
		int AimPunchVal;

		bool OverrideViewmodel;
		int ViewmodelOffsetX;
		int ViewmodelOffsetY;
		int ViewmodelOffsetZ;
		int roll;

		bool thirdperson;
		bool thirdperson_alive = true;

		int Fov;
		int CurrentFov;
		int ViewmodelFov;
		int ThirdpersonFov;
		int Thirdpersondistance;

		bool PreserveKillFeed;

		bool UPCrosshair;
		bool WCrosshair;
		bool Crosshair;
		int NOCrosshair;
		bool AutoWallCrosshair;
		bool IsThirdperson = false;
		int ThirdpersonHotkey;
		bool Thirdpersonondeath;
		bool SlowWalk;
		int SlowWalkAmount;
		bool StopLegAnim;
		bool ShowAccurateLby;
		bool StopInBetweenShots;
		int ResolverMode = 1;


		bool KeyVisualizer;
		bool KnifeLeftSide;
		int Hitmarker;
		bool screenfx;
		int screenfxtime;

		bool Bhop;
		int hophitchance;
		bool Legitbhop;
		bool Enableminhops;
		bool Enablehitchancehops;
		bool Enablemaxhops;
		int Hoplimit;
		int MaxHops;
		bool AutoStrafe;
		bool WASD;
		int ClanTagMode = 0;
		std::string ClantagText = SteamStrings("Alpheus ALPHA");
		std::string Threat = SteamStrings("Alpheuscheat.cf");
		bool DisablePanoramaBlur;
		bool PlayerList;
		int CurrentConfig = 0;
		bool Unload;
		bool indicators;
		bool velocity;
		bool AntiUntrusted = true;
		int weaponmode;
		bool BulletTracers;
		bool Impacts;
		ColorV2 BulletTracerEnemyColor = { 255.f,255.f,255.f,255.f };
		ColorV2 BulletTracerLocalColor = { 255.f,255.f,255.f,255.f };
		ColorV2 CapsuleColor = { 255.f,255.f,255.f,255.f };
		ColorV2 ShowLagColor = { 255.f,255.f,255.f,255.f };
		bool KillCounter;
		bool VizualizeHits;
		ColorV2 VizualizeHitsColor = { 255.f,255.f,255.f,255.f };
		int moonmode;
		bool ShowLag;
		int fakelagonpeek;
		bool FakeLagPrediction;
		bool ammo;
		int FovDistance;
		int FovSize;
		bool FovAlways;
		bool FovEnable;
		int LocalGlowMode;
		int EnemyGlowMode;
		int TeamGlowMode;
		int MMRegionChanger;
		bool TopInfoBar;
		bool monkey;
		bool monkey2;
		bool AntiClientCrasher;
		bool DrawAimFov;
		bool namestealer;
		bool quickerplant;
		float aspectvalue;
		int BlockBotKey;
		bool blockbotonhead;
		bool crouchcooldown;
		ColorV2 AimFovColor = { 255.f,255.f,255.f,255.f };

		// More ESP
		bool BombEsp;
		ColorV2 BombEspColor = { 255.f,255.f,255.f,255.f };


		bool Rainbowwatermark;
		ColorV2 MenuColor = { 255.f,255.f,255.f,255.f };
		ColorV2 ConsoleColor = { 255.f,255.f,255.f,255.f };
		bool updateworld;


		//Weapon Config
		int AutoHC;
		int AutoMD;
		int AutoBS;
		int AutoHS;

		int RifleHC;
		int RifleMD;
		int RifleBS;
		int RifleHS;

		int ScoutHC;
		int ScoutMD;
		int ScoutBS;
		int ScoutHS;

		int PistolHC;
		int PistolMD;
		int PistolBS;
		int PistolHS;

		int AwpHC;
		int AwpMD;
		int AwpBS;
		int AwpHS;
		int awpglobal_hitscan;
		int scoutglobal_hitscan;
		int autoglobal_hitscan;
		int pistolglobal_hitscan;
		int rifleglobal_hitscan;
		int deagleglobal_hitscan;
		int AwpAWDM;
		int ScoutAWDM;
		int AutoAWDM;
		int PistolAWDM;
		int RifleAWDM;
		int DeagleAWDM;

		int SmgHC;
		int SmgMD;
		int SmgBS;
		int SmgHS;
		int SmgWDM;

		int DeagleHC;
		int DeagleMD;
		int DeagleBS;
		int DeagleHS;
		bool EdgeJump;
		int EdgeJumpKey;
		bool shotcounter;
		bool quickswitch;
		float ExpireTime = 1.3f;
		float MarkerSize = 12.f;
		float Ragebottime = 100.f;
		bool VisHitmarker;
		bool LogDamage;
		bool LogPurchases;
		bool log_chat;
		bool log_logger;
		bool Killsay;
		bool autokillself;
		int killsaytype;
		bool ForceCrosshair;
		class BuyBot
		{
		public:
			bool enable;
			int armor;
			int main;
			int second;
			int grenade;
			int misc;
		}BuyBot;
		class RankChanger
		{
		public:
			int cmd_friendly;
			int cmd_teaching;
			int cmd_leader;
			int rank_id;
			int wins;
			int player_level;
			int ban;
			int time;
		}RankChanger;
		class Skinchanger
		{
		public:
			bool skinenabled;
			int Knife;
			int gloves;
			int skingloves;
			float glovewear;
			bool glovesenabled;
			bool rankchanger;
			int KnifeSkin;
			int AK47Skin;
			int GalilSkin;
			int M4A1SSkin;
			int M4A4Skin;
			int AUGSkin;
			int FAMASSkin;
			int AWPSkin;
			int SSG08Skin;
			bool fixshit;
			bool knifes;
			int SCAR20Skin;
			int P90Skin;
			int Mp7Skin;
			int NovaSkin;
			int UMP45Skin;
			int GlockSkin;
			int SawedSkin;
			int USPSkin;
			int MagSkin;
			int XmSkin;
			int DeagleSkin;
			int DualSkin;
			int FiveSkin;
			int RevolverSkin;
			int Mac10Skin;
			int tec9Skin;
			int Cz75Skin;
			int NegevSkin;
			int M249Skin;
			int Mp9Skin;
			int P2000Skin;
			int BizonSkin;
			int Sg553Skin;
			int P250Skin;
			int G3sg1Skin;
			int globalskins;
			struct stickers
			{
				int Stikers1;
				int Stikers2;
				int Stikers3;
				int Stikers4;
			}allin1skins[530];



			bool stickers;
			bool customchams;
			int awpcustom;
			int ak47custom;
			int scarcustom;
			int ssgcustom;
			int m4a1scustom;
			int knifecustom;
			int glovecustom;
			int GlobalSkin;
			std::string killicon_config;
		}Skinchanger;

		bool knivchanger;
		int Knife;
		int KnifeSkin;
		int G3sg1Skin;
		bool fix;
		bool Back;
		bool Left;
		bool Right;
		int BackHotkey;
		int LeftHotkey;
		int RightHotkey;
		std::string killicon_config;
		bool GrenadePrediction;
		bool Nadetracer2;
		bool Radar;
		bool jumpbug;
		int jbugkey;
		bool rainbowhud;
		bool chamsthruscope;
		bool rainbowmenu;
		bool Ragdollgrav;
		int ExplosionKey;
		int ArmsChamsMode;
		int WeaponChamsMode;
		bool ArmsWireFrame;
		bool EnemyWireFrame;
		bool TeamWireFrame;
		bool LocalWireFrame;
		bool DesyncWireFrame;
		int ImpactType;
		bool AnimLayers;
		bool LbyConfuser;
		bool StaticLby;
		int LBY = 58;
		bool SendPacketOnBreak;
		bool invis_name;
		bool ShowDmg;
		bool ChokeShot;
		int FakeLagType;
		int FakeLagValue;
		int FakeLagValueGround;
		int Extbt_value = 0;
		bool tombstone;
		bool Spreadbox;
		bool Keystrokes;
		bool Drawspread;
		bool walkwithmenu;
		bool trustedmode;
		bool Doorspam;
		int Doorspamkey;
		bool bullettracers2;
		int bullettracerlife2;
		bool Walkbot;
		bool Autoaccept;
		bool revealranks;
		bool ColorModulateConsole;
	}Config;

private:
	struct
	{
		float x = 200.f, y = 200.f;
	}Pos; // lol

	enum
	{
		check_box,
		slider,
		sliderfloat,
		combo_box,
		multi_box,
		button,
		spacer,
		label,
		key_bind,
		colorpicker
	};

	int ControlsX;
	int ControlsX2;
	int menu_color_r = 255, menu_color_g = 255, menu_color_b = 255, menu_color_a = 255;
	int GroupTabBottom;
	int OffsetY;
	int OffsetX;
	int OldOffsetY;
	int y_offset;
	int x_offset;
	int TabOffset;
	int SubTabOffset;
	float SubTabSize; // cpp fuckin sux had to make this a float or the whole thing crashes
	float TabSize;
	int GroupTabPos[4];
	bool ControlsReturn;

	int TabNum = 0;
	int SubTabNum = 0;
	int PreviousControl = -1;

	typedef void(*ButtonCallback_t)(void);

	//void KeyBind(std::string name, int* item);
	void Tab(std::string name);
	void SubTab(std::string name);
	void CheckBox(std::string name, bool* item, std::string tooltip = "");
	void KeyBind(std::string name, int* item, std::string tooltip = "");
	void Slider(int max, std::string name, int* item);
	void SliderFloat(float max, std::string name, float* item);
	void ColorPicker(std::string name, ColorV2& item);
	void color_selector(std::string name, int* red, int* green, int* blue, int* alpha);
	void Button(std::string text, ButtonCallback_t callback);
	void Space(int spaceval);
	void TextBox(std::string name, std::string* text);
	void Text(std::string text);
	void WeaponChooser();
	void ComboBox(std::string name, std::vector< std::string > itemname, int* item);
	void ComboBox_Large(std::string name, std::vector<std::string> itemname, int* item);
	void GroupBox(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string string, bool show_label);
	void GroupBox_Active(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string string, bool show_label);

};
extern Menu g_Menu;

class NewMenu
{
public:
	void group_box(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string string, bool show_label);

	void tab(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string, std::int32_t& tab, std::int32_t count, bool show_outline);
	void check_box(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, bool& value);
	void slider(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, float& value, float min_value, float max_value);
	void menu_movement(std::int32_t& x, std::int32_t& y, std::int32_t w, std::int32_t h);

	bool should_drag = false;
	bool should_move = false;

	int current_tab = 0;
	void Draw();
	void toggle();
	bool opened = false;
	bool IsOpened = false;
	int x = 280, y = 240;
	int w = 800, h = 400;
	bool test_bool = false;
	float test_float = 0.f;
}; extern NewMenu g_nMenu;