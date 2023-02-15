#include "grenadeparth.h"
#include "../../SDK/SDKinc.h"
#include "../../SDK/Drops+.h"
std::deque<std::pair<Vector, float>> newpath;
std::deque<GrenadePath> deques;


bool Shoulddraw(GrenadePath& gp, float prevthrowtime)
{
	for (auto it = gp.path.begin(), end = gp.path.end(); it != end; ++it)
	{
		if (gp.flThrowTime && g_pGlobalVars->curtime + 2 > gp.flThrowTime && gp.flThrowTime != prevthrowtime)
			return true;

		if (gp.bThrown)
		{
			gp.bThrown = false;
			return true;
		}
	}

	return false;
}
void paintgrenadeparth()
{
	float throwtime = 0;
	for (int i = deques.size() - 1; i >= 0; i--)
	{
		auto dq = deques.begin() + i;

		auto currdeque = dq->path;

		if (!Shoulddraw(*dq, throwtime))
		{
			deques.erase(dq);
			continue;
		}

		throwtime = dq->flThrowTime;

		Vector prev = currdeque.at(0).first;
		Vector res1, res2;
		for (auto it = currdeque.begin(), end = currdeque.end(); it != end; it++)
		{
			if (_drops.transf(prev, res1) && _drops.transf(it->first, res2)) //draws the lines between your points
				g_pSurface->Line(res1.x, res1.y, res2.x, res2.y, Color(255, 255, 255, it->second));
			prev = it->first;
		}

		if (_drops.transf(prev, res2)) //draws the cube at the end of the line
			g_pSurface->OutlinedRect(res2.x - 4, res2.y - 4, 8, 8, Color(255, 105, 180, (currdeque.end() - 1)->second));

		if (dq->bThrown)
			dq->Fade();

		if (!currdeque.size())
			deques.erase(dq);
	}
}