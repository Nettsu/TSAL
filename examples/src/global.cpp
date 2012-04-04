#include "tsal.hpp"
#include <cstdio>
#include <unistd.h>

int main()
{
	TSAL_Mixer mixer;

	// Wczytaj plik "sound.ogg" i nazwij wczytany dźwięk "sound"
	mixer.load_sound("sound/sound.ogg", "sound");

	// Odtwórz globalnie dźwięk "sound" w oryginalnej głośności ale w częstotliwości
	// dwukrotnie niższej niż oryginalna
	mixer.play_global("sound", 1.0f, 0.5f);

	// Poczekaj sekundę na zakończenie odtwarzania
	sleep(1);

	return 0;
}
