#include "tsal.hpp"
#include <cstdio>

int main()
{
	TSAL_Mixer* mixer = TSAL_Mixer::get_mixer();
	mixer->init_sfx();

	// Wczytaj plik "sound.ogg" i nazwij wczytany dźwięk "sound"
	mixer->load_sound("sound/sound.ogg", "sound");

	// Odtwórz globalnie dźwięk "sound" w oryginalnej głośności ale w częstotliwości
	// dwukrotnie niższej niż oryginalna
	mixer->play_global("sound", 1.0f, 0.5f);

	// Poczekaj 2 sekundy na zakończenie odtwarzania
	sleep(2);

	mixer->stop_sfx();
	return 0;
}
