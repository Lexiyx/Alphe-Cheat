
#include "../../SDK/CGlobalVarsBase.h"
#include "../../SDK/Vector.h"
#include <vector>
#include <deque>
struct GrenadePath
{
	GrenadePath(const std::deque<std::pair<Vector, float>>& path, const float& flThrowTime, const float& flDuration, const float& flFadeSpeed)
	{
		this->path = path;
		this->flThrowTime = flThrowTime;
		this->flDuration = flDuration;
		this->flFadeSpeed = flFadeSpeed;
		this->iInitialPathSize = path.size();
		this->bThrown = true;
		this->step = 0;
	}
	void Fade()
	{
		step += g_pGlobalVars->frametime;
		if (step <= flDuration)
			return;

		for (int i = path.size() - 1; i >= 0; i--)
		{
			auto it = path.begin() + i;

			if (!it->second)
			{
				path.erase(it);
				continue;
			}

			it->second -= (iInitialPathSize * flFadeSpeed / 255) / (i + 6);
			if (it->second < 0) it->second = 0.f;
		}
	}
	std::deque<std::pair<Vector, float>> path;
	float flThrowTime;
	float flDuration;
	float flFadeSpeed;
	size_t iInitialPathSize;
	bool bThrown;
	float step;
};


extern std::deque<std::pair<Vector, float>> newpath;
extern std::deque<GrenadePath> deques;
extern void paintgrenadeparth();
