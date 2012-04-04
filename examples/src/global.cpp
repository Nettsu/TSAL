#include "tsal.hpp"
#include <cstdio>
#include <unistd.h>

int main()
{
	TSAL_Manager manager;

	// Wczytaj plik "sound.ogg" i nazwij wczytany dźwięk "sound"
	manager.load_sound("sound/sound.ogg", "sound");

	// Odtwórz globalnie dźwięk "sound" w oryginalnej głośności ale w częstotliwości
	// dwukrotnie niższej niż oryginalna
	manager.play_global("sound", 1.0f, 0.5f);

	// Poczekaj sekundę na zakończenie odtwarzania
	sleep(1);

	return 0;
}
