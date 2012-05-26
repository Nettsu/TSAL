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

TSAL_Priv_Source::~TSAL_Priv_Source()
{
	if (mixer != NULL)
		mixer->forget_source(this);
}

TSAL_Priv_Source::TSAL_Priv_Source(TSAL_Manager* mgr)
{
	mixer = mgr;
	reference_counter = 0;
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
