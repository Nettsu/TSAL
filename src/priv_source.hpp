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

#include <AL/al.h>
#include <AL/alc.h>
#include <string>

class TSAL_Manager;
class TSAL_Source;

class TSAL_Priv_Source
{
	friend class TSAL_Manager;
	friend class TSAL_Source;
	
	public:
		TSAL_Priv_Source(TSAL_Manager* mgr);
		~TSAL_Priv_Source();
		
	private:
		ALfloat pos[3];
		ALfloat vel[3];
		std::string sample;
		ALuint source_id;
		float loudness;
		float pitch;
		float falloff;
		float offset;
		bool loop;
		bool id_taken_away;
		bool sample_changed;
		bool playing;
		bool offset_changed;
		bool paused;
		int playbacks;
		unsigned int reference_counter;
		TSAL_Manager* mixer;
		void take_id();
};
