/* 
 * This file is part of the TSAL library source code.
 * Use, distribution and reproduction of this library source is
 * governed by a BSD-style source license included with this source
 * in 'COPYING'. Please read these terms before distributing.
 * 
 * The TSAL source code is (C) COPYRIGHT 2012
 * by Marcin Pawłowski
 *
 */
 
#include "tsal.hpp"
#include "priv_source.hpp"

using namespace std;

TSAL_Source::~TSAL_Source()
{
	if (priv != NULL)
	{
		if (priv->reference_counter > 0)
			priv->reference_counter--;
		if (priv->reference_counter == 0)
			delete priv;
	}
}

TSAL_Source::TSAL_Source()
{
	priv = NULL;
}

TSAL_Source::TSAL_Source(TSAL_Priv_Source* ptr)
{
	priv = ptr;
	priv->reference_counter++;
}

TSAL_Source::TSAL_Source(const TSAL_Source& source)
{
	priv = source.priv;
	if (priv != NULL)
		priv->reference_counter++;
}

TSAL_Source& TSAL_Source::operator=(const TSAL_Source& source)
{
	if (priv != source.priv)
	{
		if (priv != NULL)
		{
			if (priv->reference_counter > 0)
				priv->reference_counter--;
			if (priv->reference_counter == 0)
				delete priv;
		}

		priv = source.priv;
		if (priv != NULL)
			priv->reference_counter++;
	}
	return *this;
}

void TSAL_Source::start()
{
	priv->playing = true;
	priv->playbacks = 0;
}

void TSAL_Source::stop()
{
	priv->playing = false;
	priv->sample_changed = true;
}

void TSAL_Source::pause()
{
	if (priv->playing == true)
		priv->paused = true;
}

string TSAL_Source::sample()
{
	return priv->sample;
}

void TSAL_Source::set_sample(string n)
{
	if (n == priv->sample) return;
	
	priv->sample = n;
	priv->sample_changed = true;
}

void TSAL_Source::set_pitch(float p)
{
	priv->pitch = p;
}

void TSAL_Source::set_loudness(float loud)
{
	priv->loudness = loud;
}

void TSAL_Source::set_loop(bool l)
{
	priv->loop = l;
}

void TSAL_Source::set_falloff(float f)
{
	priv->falloff = f;
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
