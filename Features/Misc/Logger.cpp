#include "Logger.h"
#include "../../Utils/GlobalVars.h"
#include "../../SDK/CGlobalVarsBase.h"
#include "../../SDK/ISurface.h"
#include "../Visuals/ESP.h"

float GetCurtime()
{

	if (!g::pLocalEntity)
		return -1;

	return static_cast<float>(g::pLocalEntity->GetTickBase()) * g_pGlobalVars->intervalPerTick;


	// get current time
	/*tm time = { };
	const std::chrono::system_clock::time_point systemNow = std::chrono::system_clock::now();
	const std::time_t timeNow = std::chrono::system_clock::to_time_t(systemNow);
	long int t = static_cast<long int> (timeNow);
	return t;*/
}


void DrawFF(int X, int Y, unsigned int Font, bool center_width, bool center_height, Color Color, std::string Input)
{
	/* char -> wchar */
	size_t size = Input.size() + 1;
	auto wide_buffer = std::make_unique<wchar_t[]>(size);
	mbstowcs_s(0, wide_buffer.get(), size, Input.c_str(), size - 1);

	/* check center */
	int width = 0, height = 0;
	g_pSurface->LogGetTextSize(Font, wide_buffer.get(), width, height);
	if (!center_width)
		width = 0;
	if (!center_height)
		height = 0;

	/* call and draw*/
	g_pSurface->LogDrawSetTextColor(Color);
	g_pSurface->LogDrawSetTextFont(Font);
	g_pSurface->DrawSetTextPos(X - (width * .5), Y - (height * .5));
	g_pSurface->LogDrawPrintText(wide_buffer.get(), wcslen(wide_buffer.get()));
}
Vector2D GetTextSize(unsigned int Font, std::string Input)
{
	/* char -> wchar */
	size_t size = Input.size() + 1;
	auto wide_buffer = std::make_unique<wchar_t[]>(size);
	mbstowcs_s(0, wide_buffer.get(), size, Input.c_str(), size - 1);

	int width, height;
	g_pSurface->LogGetTextSize(Font, wide_buffer.get(), width, height);

	return Vector2D(width, height);
}


namespace FEATURES
{
	namespace MISC
	{
		InGameLogger in_game_logger;
		void ColorLine::Draw(int x, int y, unsigned int font)
		{
			for (int i = 0; i < texts.size(); i++)
			{
				//DrawFF(x, y, font, false, false, colors[i], texts[i]);
				//fixed that shit with nodraw at all
				g_pSurface->DrawT(x, y, colors[i], g::x88Font, false, texts[i].c_str());
				x += GetTextSize(font, texts[i]).x;
			}
		}

		void InGameLogger::Do()
		{
			if (log_queue.size() > max_lines_at_once)
				log_queue.erase(log_queue.begin() + max_lines_at_once, log_queue.end());

			for (int i = 0; i < log_queue.size(); i++)
			{
				auto log = log_queue[i];
				float time_delta = fabs(GetCurtime() - log.time);

				int height = ideal_height + (16 * i);

				/// erase dead logs
				if (time_delta > text_time)
				{
					log_queue.erase(log_queue.begin() + i);
					break;
				}
				if (time_delta > text_time - slide_out_speed)
					height = height + (((slide_out_speed - (text_time - time_delta)) / slide_out_speed) * slide_out_distance);

				/// fade out
				if (time_delta > text_time - text_fade_out_time)
					log.color_line.ChangeAlpha(255 - (((time_delta - (text_time - text_fade_out_time)) / text_fade_out_time) * 255.f));
				/// fade in
				if (time_delta < text_fade_in_time)
					log.color_line.ChangeAlpha((time_delta / text_fade_in_time) * 255.f);

				int width = ideal_width;

				/// slide from left
				if (time_delta < text_fade_in_time)
					width = (time_delta / text_fade_in_time) * static_cast<float>(slide_in_distance) + (ideal_width - slide_in_distance);
				/// slider from right
				if (time_delta > text_time - text_fade_out_time)
					width = ideal_width + (((time_delta - (text_time - text_fade_out_time)) / text_fade_out_time) * static_cast<float>(slide_out_distance));


				log.color_line.Draw(width, height, g::x88Font);

			}
		}
	}
}
