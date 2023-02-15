#pragma once
#include "..\SDK\CInput.h"
#include "..\SDK\CEntity.h"
#define MAXSTUDIOBONES		128		// total bones actually used

namespace g
{
	extern CUserCmd* pCmd;
	extern C_BaseEntity* pLocalEntity;
	extern std::uintptr_t uRandomSeed;
	extern Vector         OriginalView;
	extern bool           bSendPacket;
	extern bool           offakelag;
	extern bool			  offdt;
	extern int			  lastDoubletapTickcount;
	extern bool			  LagPeek;
	extern int            TargetIndex;
	extern Vector         EnemyEyeAngs[65];
	extern Vector         AimbotHitbox[65][28];
	extern Vector         RealAngle;
	extern Vector         AngleAA;
	extern Vector         FakeAngle;
	extern Vector         LastAngle;
	extern matrix3x4_t fake_matrix[MAXSTUDIOBONES];
	extern bool setuping_bones;
	extern bool           Shot[65];
	extern bool           Hit[65];
	extern int            Shots[65];
	extern int            MissedShots[65];
	extern bool DesyncEnabledSpecialChecks;
	extern bool			  PreserveDelete;
	extern int            flHurtTime;
	extern bool           isfakeducking;
	extern bool           fakeducked;
	extern bool           Walkbotting;
	extern bool           IsFakewalking;
	extern bool           IsSlowwalking;
	extern bool			  PreCheck;
	extern int            kills;
	extern float          killtime;
	extern int            globalAlpha;
	extern float          GoalLby;
	extern int            hits;
	extern int            shots;
	extern bool           knifeapply;
	extern float          viewMatrix[4][4];
	extern int m_nTickbaseShift;
	extern bool bInSendMove;
	extern int32_t nSinceUse;
	extern bool bFirstSendMovePack;
	extern bool fTime;

	extern DWORD Tahoma;
	extern DWORD SmallText;
	extern DWORD TahomaSmall;
	extern DWORD EspSmallText;
	extern DWORD WeaponIcons;
	extern DWORD Icons;
	extern DWORD Logo;
	extern DWORD WeaponESP;
	extern DWORD SansSerif;
	extern DWORD SansSerifBold;
	extern DWORD Indicators;
	extern DWORD ToolTip;
	extern DWORD Velocity;
	extern DWORD Watermark;
	extern DWORD KillCounter;
	extern DWORD x88Font;
	extern DWORD AAarrows;
	extern DWORD IndicatorFont;


	//Remastered Esp
	extern DWORD ReEspText;
}