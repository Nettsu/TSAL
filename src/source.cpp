#include <map>
#include "tsal.hpp"

using namespace std;

TSAL_Source::~TSAL_Source()
{
	TSAL_Mixer::get_mixer()->forget_source(this);
}

TSAL_Source::TSAL_Source()
{
	pos[0] = pos[1] = pos[2] = 0;
	vel[0] = vel[1] = vel[2] = 0;
	name = "";
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
	TSAL_Mixer::get_mixer()->register_source(this);
}

TSAL_Source::TSAL_Source(string n, bool l)
{
	pos[0] = pos[1] = pos[2] = 0;
	vel[0] = vel[1] = vel[2] = 0;
	name = n;
	loudness = 1;
	pitch = 1;
	falloff = 1;
	loop = l;
	playing = false;
	id_taken_away = true;
	sample_changed = true;
	offset_changed = false;
	TSAL_Mixer::get_mixer()->register_source(this);
}

void TSAL_Source::start()
{
	playing = true;
	playbacks = 0;
}

void TSAL_Source::stop()
{
	playing = false;
}

void TSAL_Source::pause()
{
	if (playing == true)
		paused = true;
}

string TSAL_Source::sample()
{
	return name;
}

void TSAL_Source::change_sample(string n)
{
	if (n == name) return;
	
	name = n;
	sample_changed = true;
}

void TSAL_Source::update(float p, float loud, bool l)
{
	if (p == p)	pitch = p;
	loudness = loud;
	loop = l;
}

void TSAL_Source::set_pos(float pos_x, float pos_y, float pos_z)
{
	pos[0] = pos_x;
	pos[1] = pos_y;
	pos[2] = pos_z;
}

void TSAL_Source::set_vel(float vel_x, float vel_y, float vel_z)
{
	vel[0] = vel_x;
	vel[1] = vel_y;
	vel[2] = vel_z;
}

void TSAL_Source::set_offset(float off)
{
	offset = off;
	if (offset < 0) offset = 0;
	if (offset > 1) offset = 1;
	offset_changed = true;
}

bool TSAL_Source::is_playing()
{
	return playing;
}

void TSAL_Source::take_id()
{
	id_taken_away = true;
}
