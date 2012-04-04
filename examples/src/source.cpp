#include "tsal.hpp"
#include <cstdio>
#include <unistd.h>

int main()
{
  TSAL_Mixer mixer;
  mixer.load_sound("sound/sound.ogg", "sound");
  
  // Utworz wirtualne źródło dźwięku i nadaj mu początkową pozycję
  TSAL_Source source = mixer.create_source();
  source.set_pos(100.0f, 0.0f, 0.0f);
  
  // Ustaw dźwięk źródła na wczytany wcześniej dźwięk "sound"
  source.change_sample("sound");
	
  // Zachowaj oryginalną głośność i wysokość dźwięku i spraw
  // by źródło odtwarzało go w pętli
  source.update(1.0f, 1.0f, true);
	
  // Nadaj pozycję i kierunek wirtualnemu słuchaczowi
  mixer.listener_pos(0.0f, 0.0f, 0.0f);
  mixer.listener_facing(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	
  // Rozpocznij odtwarzanie dźwięku ze źródła
  source.start();
	
  for (int i = 0; i <= 10; i++)
  {
		// Funkcja zarządzająca źródłami, powinna być wykonywana regularnie
		mixer.manage_all_sources();
    // Przesuń wirtualne źródło wzdłuż osi OX i poczekaj pół sekundy
    source.set_pos(100.0f - i*20.0f, 0.0f, 0.0f);
    usleep(500000);
  }

  return 0;
}
