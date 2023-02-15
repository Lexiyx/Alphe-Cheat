#pragma once
class Hotkeys
{
public:
	enum
	{
		MOUSE1 = VK_LBUTTON,
		MOUSE2 = VK_RBUTTON,
		MMBUTTON = VK_MBUTTON,
		MOUSE5 = VK_XBUTTON2,
		MOUSE4 = VK_XBUTTON1,
		LSHIFT = 0xA0,
		LCONTROL = 0xA2,
		LEFTARROW = 0x25,
		UPARROW = 0x26,
		RIGHTARROW = 0x27,
		DOWNARROW = 0x28,
		C_KEY = 0x43,
		H_KEY = 0x48,
		X_KEY = 0x58,
		V_KEY = 0x56
	};

	int GetHotkey(int index)
	{
		switch (index)
		{
		case 0:
			return MOUSE1;
			break;
		case 1:
			return MOUSE2;
			break;
		case 2:
			return MMBUTTON;
			break;
		case 3:
			return MOUSE5;
			break;
		case 4:
			return MOUSE4;
			break;
		case 5:
			return LSHIFT;
			break;
		case 6:
			return LCONTROL;
			break;
		case 7:
			return LEFTARROW;
			break;
		case 8:
			return UPARROW;
			break;
		case 9:
			return RIGHTARROW;
			break;
		case 10:
			return DOWNARROW;
			break;
		case 11:
			return C_KEY;
			break;
		case 12:
			return H_KEY;
			break;
		case 13:
			return X_KEY;
			break;
		}
	}

	char* KeyDigits[254] = {
		nullptr, "Left Mouse", "Right Mouse", "Control+Break", "Middle Mouse", "Mouse 4", "Mouse 5",
		nullptr, "Backspace", "TAB", nullptr, nullptr, nullptr, "ENTER", nullptr, nullptr, "SHIFT", "CTRL", "ALT", "PAUSE",
		"CAPS LOCK", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "ESC", nullptr, nullptr, nullptr, nullptr,
		"SPACEBAR",
		"PG UP", "PG DOWN", "END", "HOME", "Left", "Up", "Right", "Down", nullptr, "Print", nullptr, "Print Screen",
		"Insert",
		"Delete", nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr,
		nullptr, "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U",
		"V", "W", "X",
		"Y", "Z", "Left Windows", "Right Windows", nullptr, nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6",
		"7", "8", "9", "*", "+", "_", "-", ".", "/", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11",
		"F12",
		"F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24", nullptr, nullptr, nullptr,
		nullptr, nullptr,
		nullptr, nullptr, nullptr, "NUM LOCK", "SCROLL LOCK", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, "LSHIFT", "RSHIFT", "LCONTROL", "RCONTROL", "LMENU", "RMENU", nullptr,
		nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "Next Track", "Previous Track", "Stop", "Play/Pause",
		nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "-", ".", "/?", "~", nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "[{", "\\|", "}]", "'\"", nullptr, nullptr, nullptr,
		nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
	};
private:

};

extern Hotkeys g_Hotkeys;