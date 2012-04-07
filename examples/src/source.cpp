#include "tsal.hpp"
#include <cstdio>
#include <unistd.h>

int main()
{
  TSAL_Manager manager;
  manager.load_sound("sound/sound.ogg", "sound");
  
  // Utworz wirtualne źródło dźwięku i nadaj mu początkową pozycję
  TSAL_Source source = manager.create_source("sound", 100.0f, 0.0f, 0.0f);
  
  // Niech dźwięk źródła będzie odtwarzany w pętli
  source.set_loop(true);
	
  // Nadaj pozycję i kierunek wirtualnemu słuchaczowi
  manager.listener_pos(0.0f, 0.0f, 0.0f);
  manager.listener_facing(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	
  // Rozpocznij odtwarzanie dźwięku ze źródła
  source.start();
	
  for (int i = 0; i <= 10; i++)
  {
		// Funkcja zarządzająca źródłami, powinna być wykonywana regularnie
		manager.manage_all_sources();
    // Przesuń wirtualne źródło wzdłuż osi OX i poczekaj pół sekundy
    source.set_pos(100.0f - i*20.0f, 0.0f, 0.0f);
    usleep(500000);
  }

  return 0;
}
