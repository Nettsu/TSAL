#include "tsal.hpp"
#include "priv_source.hpp"

using namespace std;

TSAL_Source::~TSAL_Source()
{
	delete priv;
}

TSAL_Source::TSAL_Source()
{
	
}

TSAL_Source::TSAL_Source(TSAL_Priv_Source* ptr)
{
	priv = ptr;
	priv->pos[0] = priv->pos[1] = priv->pos[2] = 0;
	priv->vel[0] = priv->vel[1] = priv->vel[2] = 0;
	priv->name = "";
	priv->loudness = 1;
	priv->pitch = 1;
	priv->falloff = 1;
	priv->offset = 0;
	priv->loop = false;
	priv->playing = false;
	priv->paused = false;
	priv->id_taken_away = true;
	priv->sample_changed = true;
	priv->offset_changed = false;
}

void TSAL_Source::start()
{
	priv->playing = true;
	priv->playbacks = 0;
}

void TSAL_Source::stop()
{
	priv->playing = false;
}

void TSAL_Source::pause()
{
	if (priv->playing == true)
		priv->paused = true;
}

string TSAL_Source::sample()
{
	return priv->name;
}

void TSAL_Source::change_sample(string n)
{
	if (n == priv->name) return;
	
	priv->name = n;
	priv->sample_changed = true;
}

void TSAL_Source::update(float p, float loud, bool l)
{
	if (p == p)	priv->pitch = p;
	priv->loudness = loud;
	priv->loop = l;
}

void TSAL_Source::set_pos(float pos_x, float pos_y, float pos_z)
{
	priv->pos[0] = pos_x;
	priv->pos[1] = pos_y;
	priv->pos[2] = pos_z;
}

void TSAL_Source::set_vel(float vel_x, float vel_y, float vel_z)
{
	priv->vel[0] = vel_x;
	priv->vel[1] = vel_y;
	priv->vel[2] = vel_z;
}

void TSAL_Source::set_offset(float off)
{
	priv->offset = off;
	if (priv->offset < 0)
		priv->offset = 0;
	else if (priv->offset > 1) 
		priv->offset = 1;
	priv->offset_changed = true;
}

bool TSAL_Source::is_playing()
{
	return priv->playing;
}
