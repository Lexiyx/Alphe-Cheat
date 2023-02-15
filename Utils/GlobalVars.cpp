#include "GlobalVars.h"

namespace g
{
	CUserCmd* pCmd = nullptr;
	C_BaseEntity* pLocalEntity;
	std::uintptr_t uRandomSeed = NULL;
	Vector         OriginalView;
	bool           bSendPacket = true;
	bool           offakelag = false;
	bool			offdt = false;
	int				lastDoubletapTickcount = 0;
	bool		   LagPeek = false;
	int            TargetIndex = -1;
	Vector         EnemyEyeAngs[65];
	Vector         AimbotHitbox[65][28];
	Vector         RealAngle;
	Vector         AngleAA;
	Vector         FakeAngle;
	Vector         LastAngle;
	matrix3x4_t fake_matrix[MAXSTUDIOBONES];
	bool setuping_bones = false;
	bool           Shot[65];
	bool           Hit[65];
	int            Shots[65];
	bool DesyncEnabledSpecialChecks = false;
	int            MissedShots[65];
	bool		   PreserveDelete;
	int            flHurtTime;
	bool           fakeducked = false;
	bool		   Walkbotting = false;
	bool           isfakeducking = false;
	bool           IsFakewalking = false;
	bool           IsSlowwalking = false;
	bool           PreCheck = true;
	int            kills = 0;
	float          killtime = 0.0f;
	int            globalAlpha = 0;
	float          GoalLby = 0;
	int            hits = 0;
	int            shots = 0;
	bool           knifeapply;
	float          viewMatrix[4][4];
	int m_nTickbaseShift;
	bool bInSendMove;
	int32_t nSinceUse;
	bool bFirstSendMovePack;
	bool fTime;

	DWORD Tahoma;
	DWORD SmallText;
	DWORD TahomaSmall;
	DWORD EspSmallText;
	DWORD Velocity;
	DWORD WeaponIcons;
	DWORD Icons;
	DWORD Logo;
	DWORD SansSerif;
	DWORD WeaponESP;
	DWORD SansSerifBold;
	DWORD Indicators;
	DWORD ToolTip;
	DWORD Watermark;
	DWORD KillCounter;
	DWORD x88Font;
	DWORD AAarrows;
	DWORD IndicatorFont;

	//Remastered Esp
	DWORD ReEspText;
}