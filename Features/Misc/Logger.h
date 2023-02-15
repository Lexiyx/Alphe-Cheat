#pragma once
#include <vector>
#include <string>
#include <Windows.h>
#include "../../Utils/Color.h"
#include <deque>


extern float GetCurtime();

namespace FEATURES
{

	namespace MISC
	{
		class ColorLine
		{
		public:
			ColorLine() {}

			ColorLine(std::string text, Color color = Color(255, 255, 255, 255))
			{
				texts.push_back(text);
				colors.push_back(color);
			}

			void PushBack(std::string text, Color color = Color(255, 255, 255, 255))
			{
				texts.push_back(text);
				colors.push_back(color);
			}

			void PushFront(std::string text, Color color = Color(255, 255, 255, 255))
			{
				texts.insert(texts.begin(), text);
				colors.insert(colors.begin(), color);
			}

			void ChangeAlpha(int alpha)
			{
				for (auto& color : colors)
					color.alpha = alpha;
			}

			void Draw(int x, int y, unsigned int font);

			std::string Text()
			{
				std::string text;
				for (const auto string : texts)
					text += string;

				return text;
			}

		private:
			std::vector<std::string> texts;
			std::vector<Color> colors;
		};

		class InGameLogger
		{
		public:
			struct Log
			{
				Log()
				{
					time = GetCurtime();
				}

				Log(std::string text, Color color = Color(255, 255, 255, 255))
				{
					color_line = ColorLine(text, color);
					time = GetCurtime();
				}

				ColorLine color_line;
				int time;
			};

		public:
			void Do();

			void AddLog(Log log)
			{
				log_queue.insert(log_queue.begin(), log);
			}

		private:
			std::vector<Log> log_queue;

		private:
			const float text_time = 8.f;
			const float text_fade_in_time = 0.3f;
			const float text_fade_out_time = 0.4f;

			const int max_lines_at_once = 10;

			const int ideal_height = 20;
			const int ideal_width = 20;

			const int slide_in_distance = 20;
			const int slide_out_distance = 20;
			const float slide_out_speed = 0.2f;
		};

		extern InGameLogger in_game_logger;
	}
}