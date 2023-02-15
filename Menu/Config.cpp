#include "Menu.h"
#include "Config.h"
#include "../Utils/xor.h"
void CConfig::Setup()
{
	//////LEGIT AIMBOT//////
	//Legit
	SetupValue(g_Menu.Config.LegitAimbot, false, ("legitaimbot"));
	SetupValue(g_Menu.Config.LegitAimKey, 0, ("legitaimkey"));
	SetupValue(g_Menu.Config.LegitBacktrack, false, ("legitbacktrack"));
	SetupValue(g_Menu.Config.RecoilControlSystem, false, ("rcs"));
	SetupValue(g_Menu.Config.RCS_X, 0, ("rcsx"));
	SetupValue(g_Menu.Config.RCS_Y, 0, ("rcsy"));
	SetupValue(g_Menu.Config.LegitAimFov, 0, ("legitaimbotfov"));
	SetupValue(g_Menu.Config.AimSpeed, 0, ("legitaimbotspeed"));
	SetupValue(g_Menu.Config.LegitAimHitbox, 0, ("legitaimbothitbox"));

	//TriggerBot
	SetupValue(g_Menu.Config.triggerbot_recoil, false, ("triggerbot_recoil"));
	SetupValue(g_Menu.Config.triggerbot_deadmatch, false, ("triggerbot_deadmatch"));
	SetupValue(g_Menu.Config.trigetbot_enable, false, ("trigetbot_enable"));
	SetupValue(g_Menu.Config.TrigerKey, 0, ("TrigerKey"));
	SetupValue(g_Menu.Config.triggerbotbox_head, false, ("triggerbotbox_head"));
	SetupValue(g_Menu.Config.triggerbotbox_chest, false, ("triggerbotbox_chest"));
	SetupValue(g_Menu.Config.triggerbotbox_stomach, false, ("triggerbotbox_stomach"));
	SetupValue(g_Menu.Config.triggerbotbox_arms, false, ("triggerbotbox_arms"));
	SetupValue(g_Menu.Config.triggerbotbox_legs, false, ("triggerbotbox_legs"));


	//////AIMBOT//////
	SetupValue(g_Menu.Config.Aimbot, false, ("aimbot"));
	SetupValue(g_Menu.Config.Doubletap, false, ("Doubletap"));
	SetupValue(g_Menu.Config.rtime, 2, ("rtime"));
	SetupValue(g_Menu.Config.SilentAim, false, ("slientaim"));

	SetupValue(g_Menu.Config.AimMode, 0, ("aimmode"));
	SetupValue(g_Menu.Config.OnKeyAim, false, ("onkeyaim"));
	SetupValue(g_Menu.Config.AimHotkey, 0, ("aimhotkey"));
	SetupValue(g_Menu.Config.FriendlyFire, false, ("friendlyfire"));
	SetupValue(g_Menu.Config.Hitchance, 1, ("hitchance"));
	SetupValue(g_Menu.Config.HitchanceValue, 30, ("hitchanceval"));
	SetupValue(g_Menu.Config.Mindmg, 25, ("mindmg"));
	SetupValue(g_Menu.Config.Resolver, false, ("resolver"));
	SetupValue(g_Menu.Config.ResolverBeta, false, ("resolverbeta"));
	SetupValue(g_Menu.Config.ResolverMode, 0, ("resolvermode"));
	SetupValue(g_Menu.Config.Resolverr, 0, ("resolverr"));

	SetupValue(g_Menu.Config.BodyScale, 100, ("bodyscale"));
	SetupValue(g_Menu.Config.HeadScale, 100, ("headscale"));
	SetupValue(g_Menu.Config.ForceBaim, false, ("forcebaim"));
	SetupValue(g_Menu.Config.fbaimkey, 11, ("forcebaimkey"));
	SetupValue(g_Menu.Config.BaimAfterXShots, 0, ("baimafterxshots"));
	SetupValue(g_Menu.Config.BaimAfterXMissedShots, 0, ("baimafterxmissedshots"));
	SetupValue(g_Menu.Config.DelayShot, false, ("delayshot"));
	SetupValue(g_Menu.Config.IgnoreLimbs, false, ("ignorelimbs"));
	SetupValue(g_Menu.Config.Autostop, false, ("autostop"));
	SetupValue(g_Menu.Config.AutoStopMode, 0, ("autostopmode"));
	SetupValue(g_Menu.Config.customstop, 0, ("customautostopvalue"));
	SetupValue(g_Menu.Config.AutoScope, false, ("autoscope"));
	SetupValue(g_Menu.Config.AutomaticWeapons, false, ("automaticweapons"));
	SetupValue(g_Menu.Config.PosBacktrack, false, ("posbacktrk"));
	SetupValue(g_Menu.Config.posbacktrk_option, 0, ("posbacktrk_option"));
	SetupValue(g_Menu.Config.BaimLethal, false, ("baimlethal"));
	SetupValue(g_Menu.Config.BaimPitch, false, ("baimpitch"));
	SetupValue(g_Menu.Config.BaimInAir, false, ("baiminair"));
	SetupValue(g_Menu.Config.StopInBetweenShots, false, ("shopstop"));
	SetupValue(g_Menu.Config.FakeLagPrediction, false, ("fakepred"));
	SetupValue(g_Menu.Config.Ragdollgrav, false, ("ragdollgrav"));
	SetupValue(g_Menu.Config.Jumpscout, false, ("Jumpscout"));


	//////HITBOXES//////
	SetupValue(g_Menu.Config.autoglobal_hitscan, 2, ("autoglobal_hitscan"));
	SetupValue(g_Menu.Config.rifleglobal_hitscan, 2, ("rifleglobal_hitscan"));
	SetupValue(g_Menu.Config.scoutglobal_hitscan, 2, ("scoutglobal_hitscan"));
	SetupValue(g_Menu.Config.pistolglobal_hitscan, 2, ("pistolglobal_hitscan"));
	SetupValue(g_Menu.Config.awpglobal_hitscan, 2, ("awpglobal_hitscan"));
	SetupValue(g_Menu.Config.deagleglobal_hitscan, 2, ("deagleglobal_hitscan"));

	//////AUTOWALL//////
	SetupValue(g_Menu.Config.AutoAWDM, 16, ("AutoAWDM"));
	SetupValue(g_Menu.Config.RifleAWDM, 26, ("RifleAWDM"));
	SetupValue(g_Menu.Config.ScoutAWDM, 75, ("ScoutAWDM"));
	SetupValue(g_Menu.Config.PistolAWDM, 16, ("PistolAWDM"));
	SetupValue(g_Menu.Config.AwpAWDM, 62, ("AwpAWDM"));
	SetupValue(g_Menu.Config.DeagleAWDM, 54, ("DeagleAWDM"));

	//////ANTI AIM//////
	SetupValue(g_Menu.Config.Antiaim, false, ("antiaim"));
	SetupValue(g_Menu.Config.AntiAimMode, 0, ("antiaimmode"));
	SetupValue(g_Menu.Config.JitterRange, 23, ("jitterrange"));
	SetupValue(g_Menu.Config.FakeSide, 1, ("fakeside"));
	SetupValue(g_Menu.Config.LegitSide, 1, ("legitside"));
	SetupValue(g_Menu.Config.LegitSideHotkey, 11, ("legitsidehotkey"));
	SetupValue(g_Menu.Config.FakeSideHotkey, 11, ("fakesidehotkey"));
	SetupValue(g_Menu.Config.RightHotkey, 4, ("right-leftHotkey"));
	SetupValue(g_Menu.Config.BackHotkey, 4, ("backHotkey"));
	SetupValue(g_Menu.Config.PitchMode, 0, ("pitchmode"));
	SetupValue(g_Menu.Config.YawMode, 0, ("yawmode"));
	SetupValue(g_Menu.Config.CustomPitchValue, 0, ("custompitchvalue"));
	SetupValue(g_Menu.Config.AntiAimKeys, false, ("antiaimkeys"));
	SetupValue(g_Menu.Config.RealAngles, false, ("realangles"));
	SetupValue(g_Menu.Config.DesyncAngle, false, ("desyncang"));
	SetupValue(g_Menu.Config.DesyncMode, 0, ("desyncmode"));
	SetupValue(g_Menu.Config.DesyncStyle, 0, ("desyncstyle"));
	SetupValue(g_Menu.Config.DesyncCustomStatic, 0, ("desynccustomstatic"));
	SetupValue(g_Menu.Config.LbyBreaker, false, ("lbybreaker"));
	SetupValue(g_Menu.Config.Lbymode, 0, ("lbymode"));
	SetupValue(g_Menu.Config.LbyMult, 0, ("lbymultiplier"));
	SetupValue(g_Menu.Config.LbyAdd, 0, ("lbyadd"));
	SetupValue(g_Menu.Config.ZeroPitchOnLand, false, ("zeropitchonland"));
	SetupValue(g_Menu.Config.CustomPitchOnLand, false, ("custompitchonland"));
	SetupValue(g_Menu.Config.LandPitch, 0, ("landpitch"));
	SetupValue(g_Menu.Config.AntiAimIndicator, false, ("antiaimindicator"));
	SetupValue(g_Menu.Config.AdvancedAAIndicator, false, ("advancedaaindicator"));
	SetupValue(g_Menu.Config.SlideWalk, false, ("slidewalk"));
	SetupValue(g_Menu.Config.chamsthruscope, false, ("chamsthruscope"));
	SetupValue(g_Menu.Config.StaticLby, false, ("staticlby"));


	//////SEND PACKET STUFF//////

	SetupValue(g_Menu.Config.FakeLagType, 0, ("fakelagtype"));
	SetupValue(g_Menu.Config.FakeLagValue, 0, ("fakelagia"));
	SetupValue(g_Menu.Config.FakeLagValueGround, 0, ("fakelagog"));
	SetupValue(g_Menu.Config.BreakLagComp, false, ("breaklagcomp"));
	SetupValue(g_Menu.Config.ChokeShotOnPeek, false, ("chokeshotonpeek"));
	SetupValue(g_Menu.Config.FakeDuck, false, ("fakeduck"));
	SetupValue(g_Menu.Config.FakeDuckKey, 11, ("fakeduckkey"));
	SetupValue(g_Menu.Config.Airstuck, false, ("airstuck"));
	SetupValue(g_Menu.Config.Airstuckkey, false, ("airstuckkey"));
	SetupValue(g_Menu.Config.ShowServerHitboxes, false, ("showserverhitboxes"));

	//////ESP//////
	SetupValue(g_Menu.Config.Esp, 0, ("esp"));
	SetupValue(g_Menu.Config.TeamEsp, false, ("teamesp"));
	SetupValue(g_Menu.Config.SkeletonEsp, false, ("skeletonesp"));
	SetupValue(g_Menu.Config.SkeletonColor, 255.f, 255.f, 255.f, ("skeletoncolor"));
	SetupValue(g_Menu.Config.HbEsp, false, ("hbesp"));


	SetupValue(g_Menu.Config.Name, 0, ("name"));
	SetupValue(g_Menu.Config.YawPitch, false, ("yawpitch"));
	SetupValue(g_Menu.Config.Money, false, ("money"));
	SetupValue(g_Menu.Config.Health, false, ("health"));
	SetupValue(g_Menu.Config.HealthBar, 0, ("healthbar"));
	SetupValue(g_Menu.Config.HealthStyle, 0, ("healthstyle"));
	SetupValue(g_Menu.Config.Weapon, 2, ("weapon"));
	SetupValue(g_Menu.Config.ammo, false, ("ammbar"));
	SetupValue(g_Menu.Config.AmmoEspColor, 255.f, 255.f, 255.f, ("AmmoEspColor"));

	SetupValue(g_Menu.Config.Box, false, ("box"));
	SetupValue(g_Menu.Config.FovEnable, false, ("fovenable"));
	SetupValue(g_Menu.Config.FovAlways, false, ("fovalways"));
	SetupValue(g_Menu.Config.FovColor, 255.f, 255.f, 255.f, ("fovcolor"));
	SetupValue(g_Menu.Config.FovDistance, 10, ("Fov Distance"));
	SetupValue(g_Menu.Config.FovSize, 10, ("Fov Size"));

	SetupValue(g_Menu.Config.NameColor, 255.f, 255.f, 255.f, ("namecolor"));
	SetupValue(g_Menu.Config.HealthColor, 255.f, 255.f, 255.f, ("healthcolor"));
	SetupValue(g_Menu.Config.HealthBarColor, 255.f, 255.f, 255.f, ("healthbarcolor"));
	SetupValue(g_Menu.Config.WeaponColor, 255.f, 255.f, 255.f, ("weaponcolor"));
	SetupValue(g_Menu.Config.YawPitchColor, 255.f, 255.f, 255.f, ("yawpitchcolor"));
	SetupValue(g_Menu.Config.MoneyColor, 108.f, 192.f, 18.f, ("moneycolor"));
	SetupValue(g_Menu.Config.BoxColor, 255.f, 255.f, 255.f, ("boxcolor"));
	SetupValue(g_Menu.Config.VizualizeHitsColor, 255.f, 255.f, 255.f, ("vizualizehitscolor"));
	SetupValue(g_Menu.Config.CapsuleColor, 255.f, 255.f, 255.f, ("capsulecolor"));


	//////GLOW//////
	SetupValue(g_Menu.Config.EnemyGlow, false, ("enemyglow"));
	SetupValue(g_Menu.Config.EnemyGlowColor, 255.f, 255.f, 255.f, ("enemyglowcolor"));
	SetupValue(g_Menu.Config.EnemyGlowOpacity, 255.f, ("enemyglowopacity"));
	SetupValue(g_Menu.Config.EnemyGlowMode, 1, ("enemyglowmode"));

	SetupValue(g_Menu.Config.TeamGlow, false, ("teamglow"));
	SetupValue(g_Menu.Config.TeamGlowColor, 255.f, 255.f, 255.f, ("teamglowcolor"));
	SetupValue(g_Menu.Config.TeamGlowOpacity, 255.f, ("teamglowopacity"));
	SetupValue(g_Menu.Config.TeamGlowMode, 1, ("teamglowmode"));

	SetupValue(g_Menu.Config.LocalGlow, false, ("localglow"));
	SetupValue(g_Menu.Config.LocalGlowMode, 1, ("localglowmode"));
	SetupValue(g_Menu.Config.LocalGlowColor, 255.f, 255.f, 255.f, ("localglowcolor"));
	SetupValue(g_Menu.Config.LocalGlowOpacity, 255.f, ("localglowopacity"));

	//////WORLD & COLORS//////
	SetupValue(g_Menu.Config.Nightmode, false, ("nightmode"));
	SetupValue(g_Menu.Config.ProperNightmode, false, ("propernightmode"));
	SetupValue(g_Menu.Config.preset, 0, ("preset"));
	SetupValue(g_Menu.Config.ShowLagColor, 255.f, 255.f, 255.f, ("showlagcolor"));
	SetupValue(g_Menu.Config.AmbientLight, false, ("ambientlight"));
	SetupValue(g_Menu.Config.AmbientLightColor, 255.f, 255.f, 255.f, ("ambientlightcolor"));

	//////CHAMS//////
	SetupValue(g_Menu.Config.ArmsChams, false, ("armschams"));
	SetupValue(g_Menu.Config.ArmsChamsColor, 255.f, 255.f, 255.f, ("armschamscolor"));
	SetupValue(g_Menu.Config.ArmsChamsMode, 0, ("armschamsmode"));
	SetupValue(g_Menu.Config.ArmsChamsTransparency, 255.f, ("armschamstransparency"));
	SetupValue(g_Menu.Config.ArmsWireFrame, 255.f, ("armswireframe"));

	SetupValue(g_Menu.Config.WeaponChams, false, ("weaponchams"));
	SetupValue(g_Menu.Config.WeaponChamsColor, 255.f, 255.f, 255.f, ("weaponchamscolor"));
	SetupValue(g_Menu.Config.WeaponChamsTransparency, 255.f, ("weaponchamstransparency"));
	SetupValue(g_Menu.Config.WeaponChamsMode, 0, ("WeaponChamsMode"));
	SetupValue(g_Menu.Config.viewmodelchams, 0, ("viewmodelchams"));



	SetupValue(g_Menu.Config.ChamsMaterial, 0, ("chamsmaterial"));

	SetupValue(g_Menu.Config.GlowChams, false, ("glowchams"));
	SetupValue(g_Menu.Config.EnemyChams, false, ("enemychams"));
	SetupValue(g_Menu.Config.EnemyXQZChams, false, ("enemyxqzchams"));
	SetupValue(g_Menu.Config.EnemyVisOnly, false, ("enemyvisonly"));
	SetupValue(g_Menu.Config.EnemyVisChamsColor, 0.f, 255.f, 255.f, ("enemyvischamscolor"));
	SetupValue(g_Menu.Config.EnemyXQZChamsColor, 0.f, 60.f, 255.f, ("enemyxqzchamscolor"));
	SetupValue(g_Menu.Config.EnemyChamsTransparency, 255.f, ("enemychamstransparency"));
	SetupValue(g_Menu.Config.BacktrackChamsColor, 255.f, 255.f, 255.f, ("backtrackchamscolor"));

	SetupValue(g_Menu.Config.BacktrackingChams, 0, ("backtrackingchams"));

	SetupValue(g_Menu.Config.AlwaysIndicate, false, ("alwaysindicate"));
	SetupValue(g_Menu.Config.TeamChams, false, ("teamchams"));
	SetupValue(g_Menu.Config.TeamXQZChams, false, ("teamxqzchams"));
	SetupValue(g_Menu.Config.TeamVisOnly, false, ("teamvisonly"));
	SetupValue(g_Menu.Config.TeamChamsTransparency, 255.f, ("TeamChamsTransparency"));
	SetupValue(g_Menu.Config.TeamVisChamsColor, 109.f, 181.f, 249.f, ("teamvischamscolor"));
	SetupValue(g_Menu.Config.TeamXQZChamsColor, 52.f, 132.f, 208.f, ("teamxqzchamscolor"));

	SetupValue(g_Menu.Config.LocalChams, false, ("localchams"));
	SetupValue(g_Menu.Config.LocalVisChamsColor, 255.f, 255.f, 255.f, ("localvischamscolor"));
	SetupValue(g_Menu.Config.LocalTransparency, 255.f, ("LocalTransparency"));
	SetupValue(g_Menu.Config.scopeblend, 255.f, ("Scopedopacity"));

	SetupValue(g_Menu.Config.DesyncChams, false, ("desyncchams"));
	SetupValue(g_Menu.Config.DesyncChamsColor, 255.f, 255.f, 255.f, ("desyncchamscolor"));
	SetupValue(g_Menu.Config.DesyncChamsTransparency, 255.f, ("desyncchamstransparency"));

	SetupValue(g_Menu.Config.LbyBreakerChams, false, ("lbybreakerchams"));
	SetupValue(g_Menu.Config.LbyBreakerChamsColor, 255.f, 255.f, 255.f, ("lbybreakerchamscolor"));
	SetupValue(g_Menu.Config.LbyBreakerChamsTransparency, 255.f, ("lbybreakerchamstransparency"));


	SetupValue(g_Menu.Config.NoSmoke, false, ("nosmoke"));
	SetupValue(g_Menu.Config.NoFlash, false, ("noflash"));

	//////REMOVALS//////
	SetupValue(g_Menu.Config.NoZoom, false, ("nozoom"));
	SetupValue(g_Menu.Config.NoScope, false, ("noscope"));
	SetupValue(g_Menu.Config.NoRecoil, false, ("norecoil"));
	SetupValue(g_Menu.Config.AimPunchVal, 1, ("aimpunchval"));
	SetupValue(g_Menu.Config.DisablePostProcess, false, ("disablepostprocess"));

	SetupValue(g_Menu.Config.OverrideViewmodel, false, ("overrideviewmodel"));
	SetupValue(g_Menu.Config.ViewmodelOffsetX, 10, ("viewmodeloffsetx"));
	SetupValue(g_Menu.Config.ViewmodelOffsetY, 10, ("viewmodeloffsety"));
	SetupValue(g_Menu.Config.ViewmodelOffsetZ, 10, ("viewmodeloffsetz"));
	SetupValue(g_Menu.Config.roll, 0, ("viewmodelroll"));

	//////FOV//////
	SetupValue(g_Menu.Config.Fov, 90, ("fov"));
	SetupValue(g_Menu.Config.ViewmodelFov, 69, ("viewmodelfov"));
	SetupValue(g_Menu.Config.ThirdpersonHotkey, 13, ("thirdpersonhotkey"));
	SetupValue(g_Menu.Config.Thirdpersonondeath, false, ("Thirdpersonondeath"));
	SetupValue(g_Menu.Config.thirdperson, false, ("thirdperson"));

	SetupValue(g_Menu.Config.aspectvalue, 0.f, ("ascpectratiovalue"));
	SetupValue(g_Menu.Config.aspectratio, false, ("aspectratio"));
	SetupValue(g_Menu.Config.SlowWalk, false, ("slowwalk"));
	SetupValue(g_Menu.Config.SlowWalkAmount, 30, ("slowwalkamount"));
	SetupValue(g_Menu.Config.StopLegAnim, true, ("stopleganimation"));
	SetupValue(g_Menu.Config.ShowAccurateLby, true, ("showaccuratelowerbody"));

	////// i dunno, actually //////
	SetupValue(g_Menu.Config.Crosshair, false, ("crosshair"));
	SetupValue(g_Menu.Config.quickerplant, false, ("quickerplant"));
	SetupValue(g_Menu.Config.blockbotonhead, false, ("blokbot"));
	SetupValue(g_Menu.Config.BlockBotKey, 0, ("blokbotkey"));
	SetupValue(g_Menu.Config.AutoWallCrosshair, false, ("autowallcrosshair"));
	SetupValue(g_Menu.Config.Rainbowwatermark, false, ("Rainbowwatermark"));
	SetupValue(g_Menu.Config.MenuColor, 255.f, 40.f, 0.f, ("menucolor"));
	SetupValue(g_Menu.Config.ConsoleColor, 255.f, 40.f, 0.f, ("consoleColor"));
	SetupValue(g_Menu.Config.PreserveKillFeed, false, ("preservekillfeed"));
	SetupValue(g_Menu.Config.BulletTracers, false, ("bullettrace"));
	SetupValue(g_Menu.Config.Impacts, false, ("bulletimpact"));
	SetupValue(g_Menu.Config.ImpactType, 1, ("bulletimpacttype"));
	SetupValue(g_Menu.Config.BulletTracerEnemyColor, 255.f, 255.f, 255.f, ("bullettracerenemycolor"));
	SetupValue(g_Menu.Config.BulletTracerLocalColor, 255.f, 255.f, 255.f, ("bullettracerlocalcolor"));
	SetupValue(g_Menu.Config.Glowoverlaycolor, 255.f, 255.f, 255.f, ("glowoverlaycolor"));
	SetupValue(g_Menu.Config.PearlColor, 255.f, 255.f, 255.f, ("PearlescentColor"));

	SetupValue(g_Menu.Config.KillCounter, false, ("killcounter"));
	SetupValue(g_Menu.Config.VizualizeHits, false, ("VHits"));
	SetupValue(g_Menu.Config.VizualizeHitsColor, 255.f, 255.f, 255.f, ("VHitscolor"));




	SetupValue(g_Menu.Config.LogDamage, false, ("logdmg"));
	SetupValue(g_Menu.Config.LogPurchases, false, ("logpurchases"));
	SetupValue(g_Menu.Config.log_chat, false, ("log_chat"));
	SetupValue(g_Menu.Config.log_logger, false, ("log_logger"));
	//////MISC//////
	SetupValue(g_Menu.Config.galaxysky, 0, ("galaxysky"));
	SetupValue(g_Menu.Config.Hitmarker, 0, ("hitsound"));
	SetupValue(g_Menu.Config.KnifeLeftSide, false, ("knifeleftside"));
	SetupValue(g_Menu.Config.KeyVisualizer, false, ("keyvisualizer"));
	SetupValue(g_Menu.Config.VisHitmarker, false, ("VisHitmarker"));
	SetupValue(g_Menu.Config.ShowDmg, false, ("ShowDmg"));
	SetupValue(g_Menu.Config.Bhop, false, ("bhop"));
	SetupValue(g_Menu.Config.Legitbhop, false, ("legitbhop"));

	SetupValue(g_Menu.Config.quickswitch, false, ("quickswitch"));
	SetupValue(g_Menu.Config.Hoplimit, 3, ("Hoplimit"));
	SetupValue(g_Menu.Config.MaxHops, 6, ("MaxHops"));
	SetupValue(g_Menu.Config.hophitchance, 75, ("hophitchance"));

	SetupValue(g_Menu.Config.AutoStrafe, false, ("autostrafe"));
	SetupValue(g_Menu.Config.WASD, false, ("WASD"));

	SetupValue(g_Menu.Config.indicators, false, ("indicators"));
	SetupValue(g_Menu.Config.velocity, false, ("velocity"));
	SetupValue(g_Menu.Config.ShowLag, false, ("showlag"));
	SetupValue(g_Menu.Config.DisablePanoramaBlur, false, ("disablepanblur"));
	SetupValue(g_Menu.Config.MMRegionChanger, 0, ("mmregionchanger"));
	SetupValue(g_Menu.Config.ClanTagMode, 0, ("Clantagmode"));

	SetupValue(g_Menu.Config.BuyBot.enable, false, ("bbe"));
	SetupValue(g_Menu.Config.BuyBot.main, 0, ("bbm"));
	SetupValue(g_Menu.Config.BuyBot.second, 0, ("bbs"));
	SetupValue(g_Menu.Config.BuyBot.armor, 0, ("bba"));
	SetupValue(g_Menu.Config.BuyBot.grenade, 0, ("bbg"));
	SetupValue(g_Menu.Config.BuyBot.misc, 0, ("bbmsc"));
	SetupValue(g_Menu.Config.AntiClientCrasher, true, ("acc"));
	SetupValue(g_Menu.Config.Radar, false, ("Radar"));
	SetupValue(g_Menu.Config.DrawAimFov, false, ("drawaimfov"));
	SetupValue(g_Menu.Config.AimFovColor, 255.f, 255.f, 255.f, ("aimfovcolor"));
	SetupValue(g_Menu.Config.Thirdpersonondeath, false, ("thirdpersonondeath"));
	SetupValue(g_Menu.Config.Killsay, false, ("killsay"));
	SetupValue(g_Menu.Config.killsaytype, 0, ("killsaytype"));
	SetupValue(g_Menu.Config.ForceCrosshair, false, ("forcecrosshair"));
	SetupValue(g_Menu.Config.crouchcooldown, false, ("crouchcooldown"));
	SetupValue(g_Menu.Config.walkwithmenu, true, ("walkwithmenu"));
	SetupValue(g_Menu.Config.trustedmode, true, ("trustedmode"));
	SetupValue(g_Menu.Config.Extbt_value, 0, ("Extbt_value"));
	SetupValue(g_Menu.Config.tombstone, false, ("tombstone"));
	SetupValue(g_Menu.Config.Spreadbox, false, ("Spreadbox"));
	SetupValue(g_Menu.Config.Drawspread, false, ("Drawspread"));
	SetupValue(g_Menu.Config.sv_cheats, false, ("sv_cheats"));
	SetupValue(g_Menu.Config.Autoaccept, false, ("Autoaccept"));
	SetupValue(g_Menu.Config.revealranks, false, ("revealranks"));

	//SKINS
	SetupValue(g_Menu.Config.knivchanger, false, ("kniv"));
	SetupValue(g_Menu.Config.KnifeSkin, 0, ("knivskin"));
	SetupValue(g_Menu.Config.Knife, 0, ("knivmodel"));
	SetupValue(g_Menu.Config.Skinchanger.GlobalSkin, 0, ("globalskin"));
	SetupValue(g_Menu.Config.Skinchanger.AK47Skin, 0, ("AK47Skin"));
	SetupValue(g_Menu.Config.Skinchanger.AUGSkin, 0, ("AUGSkin"));
	SetupValue(g_Menu.Config.Skinchanger.AWPSkin, 0, ("AWPSkin"));
	SetupValue(g_Menu.Config.Skinchanger.BizonSkin, 0, ("BizonSkin"));
	SetupValue(g_Menu.Config.Skinchanger.Cz75Skin, 0, ("Cz75Skin"));
	SetupValue(g_Menu.Config.Skinchanger.DeagleSkin, 0, ("DeagleSkin"));
	SetupValue(g_Menu.Config.Skinchanger.DualSkin, 0, ("DualSkin"));
	SetupValue(g_Menu.Config.Skinchanger.FAMASSkin, 0, ("FAMASSkin"));
	SetupValue(g_Menu.Config.Skinchanger.FiveSkin, 0, ("FiveSkin"));
	SetupValue(g_Menu.Config.Skinchanger.G3sg1Skin, 0, ("G3sg1Skin"));
	SetupValue(g_Menu.Config.Skinchanger.GalilSkin, 0, ("GalilSkin"));
	SetupValue(g_Menu.Config.Skinchanger.GlockSkin, 0, ("GlockSkin"));
	SetupValue(g_Menu.Config.Skinchanger.M249Skin, 0, ("M249Skin"));
	SetupValue(g_Menu.Config.Skinchanger.M4A1SSkin, 0, ("M4A1SSkin"));
	SetupValue(g_Menu.Config.Skinchanger.M4A4Skin, 0, ("M4A4Skin"));
	SetupValue(g_Menu.Config.Skinchanger.Mac10Skin, 0, ("Mac10Skin"));
	SetupValue(g_Menu.Config.Skinchanger.MagSkin, 0, ("MagSkin"));
	SetupValue(g_Menu.Config.Skinchanger.Mp7Skin, 0, ("Mp7Skin"));
	SetupValue(g_Menu.Config.Skinchanger.Mp9Skin, 0, ("Mp9Skin"));
	SetupValue(g_Menu.Config.Skinchanger.NegevSkin, 0, ("NegevSkin"));
	SetupValue(g_Menu.Config.Skinchanger.NovaSkin, 0, ("NovaSkin"));
	SetupValue(g_Menu.Config.Skinchanger.P2000Skin, 0, ("P2000Skin"));
	SetupValue(g_Menu.Config.Skinchanger.P250Skin, 0, ("P250Skin"));
	SetupValue(g_Menu.Config.Skinchanger.P90Skin, 0, ("P90Skin"));
	SetupValue(g_Menu.Config.Skinchanger.RevolverSkin, 0, ("RevolverSkin"));
	SetupValue(g_Menu.Config.Skinchanger.SawedSkin, 0, ("SawedSkin"));
	SetupValue(g_Menu.Config.Skinchanger.SCAR20Skin, 0, ("SCAR20Skin"));
	SetupValue(g_Menu.Config.Skinchanger.SSG08Skin, 0, ("SSG08Skin"));
	SetupValue(g_Menu.Config.Skinchanger.Sg553Skin, 0, ("Sg553Skin"));
	SetupValue(g_Menu.Config.Skinchanger.tec9Skin, 0, ("tec9Skin"));
	SetupValue(g_Menu.Config.Skinchanger.UMP45Skin, 0, ("UMP45Skin"));
	SetupValue(g_Menu.Config.Skinchanger.USPSkin, 0, ("USPSkin"));
	SetupValue(g_Menu.Config.Skinchanger.XmSkin, 0, ("XmSkin"));
	SetupValue(g_Menu.Config.Skinchanger.skinenabled, false, ("skinenabled"));
	// More ESP
	SetupValue(g_Menu.Config.BombEsp, false, ("bombesp"));
	SetupValue(g_Menu.Config.monkey, false, ("monkey2"));
	SetupValue(g_Menu.Config.monkey2, false, ("monkey"));
	SetupValue(g_Menu.Config.Nadetracer2, false, ("nadetracer2"));
	SetupValue(g_Menu.Config.zeusrange, false, ("zeusrange"));

	//Weapon Configs
	SetupValue(g_Menu.Config.AutoHC, 65, ("autohc"));
	SetupValue(g_Menu.Config.AutoMD, 11, ("automd"));
	SetupValue(g_Menu.Config.AutoBS, 55, ("autobs"));
	SetupValue(g_Menu.Config.AutoHS, 45, ("autohs"));

	SetupValue(g_Menu.Config.RifleHC, 39, ("riflehc"));
	SetupValue(g_Menu.Config.RifleMD, 21, ("riflemd"));
	SetupValue(g_Menu.Config.RifleBS, 70, ("riflebs"));
	SetupValue(g_Menu.Config.RifleHS, 95, ("riflehs"));

	SetupValue(g_Menu.Config.ScoutHC, 80, ("scouthc"));
	SetupValue(g_Menu.Config.ScoutMD, 55, ("scoutmd"));
	SetupValue(g_Menu.Config.ScoutBS, 15, ("scoutbs"));
	SetupValue(g_Menu.Config.ScoutHS, 55, ("scouths"));

	SetupValue(g_Menu.Config.PistolHC, 25, ("pistolhc"));
	SetupValue(g_Menu.Config.PistolMD, 10, ("pistolmd"));
	SetupValue(g_Menu.Config.PistolBS, 40, ("pistolbs"));
	SetupValue(g_Menu.Config.PistolHS, 90, ("pistolhs"));

	SetupValue(g_Menu.Config.AwpHC, 97, ("awphc"));
	SetupValue(g_Menu.Config.AwpMD, 65, ("awpmd"));
	SetupValue(g_Menu.Config.AwpBS, 90, ("awpbs"));
	SetupValue(g_Menu.Config.AwpHS, 30, ("awphs"));

	SetupValue(g_Menu.Config.SmgHC, 8, ("smghc"));
	SetupValue(g_Menu.Config.SmgMD, 14, ("smgmd"));
	SetupValue(g_Menu.Config.SmgBS, 100, ("smgbs"));
	SetupValue(g_Menu.Config.SmgHS, 65, ("smghs"));

	SetupValue(g_Menu.Config.DeagleHC, 65, ("deaglehc"));
	SetupValue(g_Menu.Config.DeagleMD, 20, ("deaglemd"));
	SetupValue(g_Menu.Config.DeagleBS, 45, ("deaglebs"));
	SetupValue(g_Menu.Config.DeagleHS, 95, ("deaglehs"));

	SetupValue(g_Menu.Config.RankChanger.rank_id, 0, ("rank_id"));
	SetupValue(g_Menu.Config.RankChanger.player_level, 0, ("player_level"));
	SetupValue(g_Menu.Config.RankChanger.wins, 0, ("wins"));
	SetupValue(g_Menu.Config.RankChanger.cmd_friendly, 0, ("cmd_friendly"));
	SetupValue(g_Menu.Config.RankChanger.cmd_teaching, 0, ("cmd_teaching"));
	SetupValue(g_Menu.Config.RankChanger.cmd_leader, 0, ("cmd_leader"));
	SetupValue(g_Menu.Config.RankChanger.time, 0, ("time"));
	SetupValue(g_Menu.Config.DeagleHS, 95, ("deaglehs"));
	//Spectator list
	SetupValue(g_Menu.Config.SpecList, 0, ("speclist_mode"));
	SetupValue(g_Menu.Config.SpecPos.x, 100.0f, ("speclist_posx"));
	SetupValue(g_Menu.Config.SpecPos.y, 100.0f, ("speclist_posy"));
}

void CConfig::SetAutoConf()
{
	g_Menu.Config.HitchanceValue = g_Menu.Config.AutoHC;
	g_Menu.Config.Mindmg = g_Menu.Config.AutoMD;
	g_Menu.Config.HeadScale = g_Menu.Config.AutoHS;
	g_Menu.Config.BodyScale = g_Menu.Config.AutoBS;
}

void CConfig::SetRifleConf()
{
	g_Menu.Config.HitchanceValue = g_Menu.Config.RifleHC;
	g_Menu.Config.Mindmg = g_Menu.Config.RifleMD;
	g_Menu.Config.HeadScale = g_Menu.Config.RifleHS;
	g_Menu.Config.BodyScale = g_Menu.Config.RifleBS;
}

void CConfig::SetScoutConf()
{
	g_Menu.Config.HitchanceValue = g_Menu.Config.ScoutHC;
	g_Menu.Config.Mindmg = g_Menu.Config.ScoutMD;
	g_Menu.Config.HeadScale = g_Menu.Config.ScoutHS;
	g_Menu.Config.BodyScale = g_Menu.Config.ScoutBS;
}

void CConfig::SetPistolConf()
{
	g_Menu.Config.HitchanceValue = g_Menu.Config.PistolHC;
	g_Menu.Config.Mindmg = g_Menu.Config.PistolMD;
	g_Menu.Config.HeadScale = g_Menu.Config.PistolHS;
	g_Menu.Config.BodyScale = g_Menu.Config.PistolBS;
}

void CConfig::SetAwpConf()
{
	g_Menu.Config.HitchanceValue = g_Menu.Config.AwpHC;
	g_Menu.Config.Mindmg = g_Menu.Config.AwpMD;
	g_Menu.Config.HeadScale = g_Menu.Config.AwpHS;
	g_Menu.Config.BodyScale = g_Menu.Config.AwpBS;
}

void CConfig::SetSmgConf()
{
	g_Menu.Config.HitchanceValue = g_Menu.Config.SmgHC;
	g_Menu.Config.Mindmg = g_Menu.Config.SmgMD;
	g_Menu.Config.HeadScale = g_Menu.Config.SmgHS;
	g_Menu.Config.BodyScale = g_Menu.Config.SmgBS;
}

void CConfig::SetDeagleConf()
{
	g_Menu.Config.HitchanceValue = g_Menu.Config.DeagleHC;
	g_Menu.Config.Mindmg = g_Menu.Config.DeagleMD;
	g_Menu.Config.HeadScale = g_Menu.Config.DeagleHS;
	g_Menu.Config.BodyScale = g_Menu.Config.DeagleBS;
}

void CConfig::SetupValue(int& value, int def, std::string name)
{
	value = def;
	ints.push_back(new ConfigValue< int >(name, &value));
}

void CConfig::SetupValue(float& value, float def, std::string name)
{
	value = def;
	floats.push_back(new ConfigValue< float >(name, &value));
}

void CConfig::SetupValue(bool& value, bool def, std::string name)
{
	value = def;
	bools.push_back(new ConfigValue< bool >(name, &value));
}

void CConfig::SetupValue(ColorV2& value, float defr, float defg, float defb, std::string name)
{
	value.red = defr;
	value.green = defg;
	value.blue = defb;
	value.alpha = 255.f;

	floats.push_back(new ConfigValue< float >(name + ("red"), &value.red));
	floats.push_back(new ConfigValue< float >(name + ("green"), &value.green));
	floats.push_back(new ConfigValue< float >(name + ("blue"), &value.blue));
	floats.push_back(new ConfigValue< float >(name + ("alpha"), &value.alpha));
}

void CConfig::SetupValue(bool* value, bool def, int size, std::string name)
{
	for (int c = 0; c < size; c++)
	{
		value[c] = def;

		name += std::to_string(c);

		bools.push_back(new ConfigValue< bool >(name, &value[c]));
	}
}

static char name[] = "#nrthawsrhbdf";

void CConfig::Save()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + ("\\#nrthawsrhbdf\\");
		switch (g_Menu.Config.CurrentConfig)
		{
		case 0:
			//Legit1
			file = std::string(path) + ("\\#nrthawsrhbdf\\legit1.Alpheus");
			break;
		case 1:
			//Legit2
			file = std::string(path) + ("\\#nrthawsrhbdf\\legit2.Alpheus");
			break;
		case 2:
			//Semi Rage
			file = std::string(path) + ("\\#nrthawsrhbdf\\semirage.Alpheus");
			break;
		case 3:
			//HVH 1
			file = std::string(path) + ("\\#nrthawsrhbdf\\hvh1.Alpheus");
			break;
		case 4:
			//HVH 2
			file = std::string(path) + ("\\#nrthawsrhbdf\\hvh2.Alpheus");
			break;
		case 5:
			//misc 1
			file = std::string(path) + ("\\#nrthawsrhbdf\\misc1.Alpheus");
			break;
		case 6:
			//misc 2
			file = std::string(path) + ("\\#nrthawsrhbdf\\misc2.Alpheus");
			break;
		case 7:
			//misc 3
			file = std::string(path) + ("\\#nrthawsrhbdf\\misc3.Alpheus");
			break;
		case 8:
			//misc 4
			file = std::string(path) + ("\\#nrthawsrhbdf\\misc4.Alpheus");
			break;
		case 9:
			//misc 5
			file = std::string(path) + ("\\#nrthawsrhbdf\\misc5.Alpheus");
			break;
		default:
			file = std::string(path) + ("\\#nrthawsrhbdf\\hvh1.Alpheus");
			break;
		}
	}

	CreateDirectory(folder.c_str(), NULL);

	for (auto value : ints)
		WritePrivateProfileString(name, value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileString(name, value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileString(name, value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void CConfig::Load()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
	{
		folder = std::string(path) + ("\\#nrthawsrhbdf\\");
		switch (g_Menu.Config.CurrentConfig)
		{
		case 0:
			//Legit1
			file = std::string(path) + ("\\#nrthawsrhbdf\\legit1.Alpheus");
			break;
		case 1:
			//Legit2
			file = std::string(path) + ("\\#nrthawsrhbdf\\legit2.Alpheus");
			break;
		case 2:
			//Semi Rage
			file = std::string(path) + ("\\#nrthawsrhbdf\\semirage.Alpheus");
			break;
		case 3:
			//HVH 1
			file = std::string(path) + ("\\#nrthawsrhbdf\\hvh1.Alpheus");
			break;
		case 4:
			//HVH 2
			file = std::string(path) + ("\\#nrthawsrhbdf\\hvh2.Alpheus");
			break;
		default:
			file = std::string(path) + ("\\#nrthawsrhbdf\\hvh1.Alpheus");
			break;
		case 5:
			//misc 1
			file = std::string(path) + ("\\#nrthawsrhbdf\\misc1.Alpheus");
			break;
		case 6:
			//misc 2
			file = std::string(path) + ("\\#nrthawsrhbdf\\misc2.Alpheus");
			break;
		case 7:
			//misc 3
			file = std::string(path) + ("\\#nrthawsrhbdf\\misc3.Alpheus");
			break;
		case 8:
			//misc 4
			file = std::string(path) + ("\\#nrthawsrhbdf\\misc4.Alpheus");
			break;
		case 9:
			//misc 5
			file = std::string(path) + ("\\#nrthawsrhbdf\\misc5.Alpheus");
			break;
		}
	}

	CreateDirectory(folder.c_str(), NULL);

	char value_l[32] = { '\0' };

	for (auto value : ints)
	{
		GetPrivateProfileString(name, value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : floats)
	{
		GetPrivateProfileString(name, value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atof(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileString(name, value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, ("true"));
	}
}

CConfig* g_Config = new CConfig();
