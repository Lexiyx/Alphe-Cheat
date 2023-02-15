#include "cyan.h"

namespace cyan {
	map<int, hotkeys::fn> hotkeys::functions;

	void hotkeys::add(int key, fn function) {
		functions[key] = function;
	}

	void hotkeys::handle(int key) {
		if (functions.count(key)) functions[key]();
	}
}