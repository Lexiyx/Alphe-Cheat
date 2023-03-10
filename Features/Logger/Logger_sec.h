#pragma once
// used: std::ofstream
#include <fstream>

// used: winapi, fmt includes
#include "../../Hooks.h"

#pragma region logging_console_colors
#define FOREGROUND_WHITE		    (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FOREGROUND_YELLOW       	(FOREGROUND_RED | FOREGROUND_GREEN)
#define FOREGROUND_CYAN		        (FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FOREGROUND_MAGENTA	        (FOREGROUND_RED | FOREGROUND_BLUE)
#define FOREGROUND_BLACK		    0

#define FOREGROUND_INTENSE_RED		(FOREGROUND_RED | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_GREEN	(FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_BLUE		(FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_WHITE	(FOREGROUND_WHITE | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_YELLOW	(FOREGROUND_YELLOW | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_CYAN		(FOREGROUND_CYAN | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_MAGENTA	(FOREGROUND_MAGENTA | FOREGROUND_INTENSITY)
#pragma endregion

/*
 * LOGGING
 */
namespace L
{
	// Values
	/* console write stream */
	inline FILE* pStream;
	/* current color of console text */
	inline std::uint8_t		wConsoleColor = FOREGROUND_WHITE;
	/* current file used for file-logging */
	inline std::ofstream	ofsFile;

	// Get
	/* attach console to current window with write permission and given title */
	bool Attach(const char* szConsoleTitle);
	/* close write streams and detach console from current window */
	void Detach();
	/* prints given text to the console/file */
	void Print(std::string_view szText);

	/* set given color to console */
	inline const void PushConsoleColor(const std::uint8_t wColor)
	{
		wConsoleColor = wColor;
	}

	/* reset console color */
	inline const void PopConsoleColor()
	{
		wConsoleColor = FOREGROUND_WHITE;
	}
}
