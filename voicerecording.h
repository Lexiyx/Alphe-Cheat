#pragma once
#include <vector>
#include <string>
#include "Alpheus/SDK/ICvar.h"
#include "Alpheus/Utils/Interfaces.h"
#include "Alpheus/Utils/Interfaces.cpp"
class c_music_player {
	using sound_files_t = std::vector< std::string >;
public:
	void initialize(void);

	void play(std::size_t file_idx);

	void stop(void);

	void load_sound_files(void);
private:
	std::string		m_sound_files_path;
	sound_files_t	m_sound_files;
	ConVar* m_voice_loopback = nullptr;
};
