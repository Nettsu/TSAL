#include <map>
#include "tsal.hpp"
#include "oal_wrap.hpp"

using namespace std;

TSAL_Source::~TSAL_Source()
{
	for (int i = 0; i < TSAL_NUM_SOURCES; i++)
		if (mixer->reserved_sources[i] == this)
		{
			alwSourceStop(mixer->source[i]);
			mixer->reserved_sources[i] = NULL;
			break;
		}
	list<TSAL_Source*>::iterator iter;
	for (iter = mixer->csources.begin(); iter != mixer->csources.end(); ++iter)
		if (*iter == this) 
		{
			mixer->csources.erase(iter);
			break;
		}
}

TSAL_Source::TSAL_Source(TSAL_Mixer* mix)
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
	id_taken_away = true;
	sample_changed = true;
	offset_changed = false;
	mixer = mix;
	mixer->csources.push_back(this);
}

TSAL_Source::TSAL_Source(TSAL_Mixer* mix, string n, bool l)
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
	mixer = mix;
	mixer->csources.push_back(this);
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

void TSAL_Source::manage()
{
	if (id_taken_away)
	{
		if (!playing) return;
		int id_index = mixer->get_source(pos[0], pos[1], pos[2]);
		if (id_index == -1) return;
		else
		{
			id_taken_away = false;
			sample_changed = true;
			mixer->reserved_sources[id_index] = this;
			source_id = mixer->source[id_index];
		}
	}
	
	if (sample_changed)
	{
		alwSourceStop(source_id);
		
		sample_changed = false;
		
		map<string,ALuint>::iterator snd = mixer->sounds.find(name);
		if (snd == mixer->sounds.end()) return;
	
		alwSourcei(source_id, AL_BUFFER, (ALint)(snd->second));
	}
	
	if (offset_changed == true)
	{
		alwSourceRewind(source_id);

    ALint total = 0;
    ALint buffer = 0;
    alwGetSourcei(source_id, AL_BUFFER, &buffer);
    alwGetBufferi(buffer, AL_SIZE, &total);
    alwSourcei(source_id, AL_BYTE_OFFSET, total*offset);
    
    offset_changed = false;
	}
	
	alwSourcefv(source_id, AL_POSITION, pos);
	alwSourcefv(source_id, AL_VELOCITY, vel);
	
	alwSourcef(source_id, AL_PITCH, pitch);
	alwSourcef(source_id, AL_GAIN, loudness*(mixer->global_volume));
	alwSourcei(source_id, AL_LOOPING, loop);
	alwSourcef(source_id, AL_REFERENCE_DISTANCE, TSAL_REF_DIST);
	alwSourcef(source_id, AL_ROLLOFF_FACTOR, falloff*TSAL_FALLOFF_MULT);
	
	int val;
	alwGetSourcei(source_id, AL_SOURCE_STATE, &val);
	if (playing && val != AL_PLAYING)
	{
		if (!loop && playbacks > 0) return;
		alwSourcePlay(source_id);
		if (!loop) playbacks++;
	}
	else if (!playing && val == AL_PLAYING)
	{
		alwSourceStop(source_id);
	}
}

void TSAL_Source::take_id()
{
	id_taken_away = true;
}
