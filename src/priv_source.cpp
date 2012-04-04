#include "tsal.hpp"
#include "priv_source.hpp"

TSAL_Priv_Source::~TSAL_Priv_Source()
{
	mixer->forget_source(this);
}

TSAL_Priv_Source::TSAL_Priv_Source()
{
	pos[0] = pos[1] = pos[2] = 0;
	vel[0] = vel[1] = vel[2] = 0;
	sample = "";
	loudness = 1;
	pitch = 1;
	falloff = 1;
	offset = 0;
	loop = false;
	playing = false;
	paused = false;
	id_taken_away = true;
	sample_changed = true;
	offset_changed = false;
}

void TSAL_Priv_Source::take_id()
{
	id_taken_away = true;
}
