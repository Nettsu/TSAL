#include <vector>
#include <AL/alc.h>
#include <vorbis/vorbisfile.h>
#include "tsal.hpp"
#include "priv_source.hpp"
#include "oal_wrap.hpp"

#define BUFFER_SIZE 32768     // 32 KB buffers

using namespace std;

TSAL_Manager::~TSAL_Manager()
{	
	list<TSAL_Priv_Source*>::iterator iter;
	for (iter = virtual_sources.begin(); iter != virtual_sources.end(); ++iter)
	{
		delete (*iter);
	}
	
	alwDeleteSources(TSAL_NUM_SOURCES, sources);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

TSAL_Manager::TSAL_Manager()
{
	listenerPos[0] = 0;
	listenerPos[1] = 0;
	listenerPos[2] = 0;
	
	listenerVel[0] = 0;
	listenerVel[1] = 0;
	listenerVel[2] = 0;
	
	listenerOri[0] = 0;
	listenerOri[1] = 0;
	listenerOri[2] = -1;
	listenerOri[3] = 0;
	listenerOri[4] = 1;
	listenerOri[5] = 0;
	
	global_volume = 1;
	reference_dist = 20;
	falloff_mult = 2;
	
	device = alcOpenDevice(NULL);
	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);

	alwListenerfv(AL_POSITION,listenerPos);
	alwListenerfv(AL_VELOCITY,listenerVel);
	alwListenerfv(AL_ORIENTATION,listenerOri);
	alwDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
	alwGenSources(TSAL_NUM_SOURCES, sources);
	for (int i = 0; i < TSAL_NUM_SOURCES; i++)
		reservations[i] = NULL;
}

void TSAL_Manager::set_ref_dist(float d)
{
	reference_dist = d;
}

void TSAL_Manager::set_falloff(float f)
{
	falloff_mult = f;
}

void TSAL_Manager::set_volume(float v)
{
	global_volume = v;
}

int TSAL_Manager::get_source(float x, float y, float z)
{
	float max_dist = 0;
	float temp_dist = 0;
	int best_candidate = 0;
	float pos[3];
	int val;
	for (int i = 0; i < TSAL_NUM_SOURCES; i++)
	{
		alwGetSourcei(sources[i], AL_SOURCE_STATE, &val);
		if (val != AL_PLAYING)
		{
			if (reservations[i] != NULL)
			{
				reservations[i]->take_id();
				reservations[i] = NULL;
			}
			return i;
		}
		alwGetSourcefv(sources[i], AL_POSITION, &pos[0]);
		temp_dist = (listenerPos[0] - pos[0])*(listenerPos[0] - pos[0]);
		temp_dist += (listenerPos[1] - pos[1])*(listenerPos[1] - pos[1]);
		temp_dist += (listenerPos[2] - pos[2])*(listenerPos[2] - pos[2]);
		if (temp_dist > max_dist)
		{
			max_dist = temp_dist;
			best_candidate = i;
		}
	}
	
	temp_dist = (listenerPos[0] - x)*(listenerPos[0] - x);
	temp_dist += (listenerPos[1] - y)*(listenerPos[1] - y);
	temp_dist += (listenerPos[2] - z)*(listenerPos[2] - z);
	if (temp_dist >= max_dist) 
		return -1;
	
	if (reservations[best_candidate] != NULL)
	{
		reservations[best_candidate]->take_id();
		reservations[best_candidate] = NULL;
	}
	alwSourceStop(sources[best_candidate]);
	return best_candidate;
}

void TSAL_Manager::listener_pos(float pos_x, float pos_y, float pos_z)
{
	listenerPos[0] = pos_x;
	listenerPos[1] = pos_y;
	listenerPos[2] = pos_z;

	alwListenerfv(AL_POSITION,listenerPos);
}

void TSAL_Manager::listener_vel(float vel_x, float vel_y, float vel_z)
{
	listenerVel[0] = vel_x;
	listenerVel[1] = vel_y;
	listenerVel[2] = vel_z;
	
	alwListenerfv(AL_VELOCITY,listenerVel);
}

void TSAL_Manager::listener_facing(float front_x, float front_y, float front_z, float up_x, float up_y, float up_z)
{
	listenerOri[0] = front_x;
	listenerOri[1] = front_y;
	listenerOri[2] = front_z;
	listenerOri[3] = up_x;
	listenerOri[4] = up_y;
	listenerOri[5] = up_z;

	alwListenerfv(AL_ORIENTATION,listenerOri);
}

void TSAL_Manager::load_sound(string file, string name)
{
	ALuint buffer;
	alGenBuffers(1, &buffer);
	ALenum format;
	vector<char> bufferData;
	
	int endian = 0;
  int bitStream;
  long bytes;
  char array[BUFFER_SIZE];
  FILE* f;

  // Open for binary reading
  f = fopen(file.c_str(), "rb");	
	 
	vorbis_info* pInfo;
  OggVorbis_File oggFile;
  ov_open(f, &oggFile, NULL, 0);

	// Get some information about the OGG file
  pInfo = ov_info(&oggFile, -1);

  // Check the number of channels... always use 16-bit samples
  if (pInfo->channels == 1)
    format = AL_FORMAT_MONO16;
  else
    format = AL_FORMAT_STEREO16;

  do 
  {
    // Read up to a buffer's worth of decoded sound data
    bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);
    // Append to end of buffer
    bufferData.insert(bufferData.end(), array, array + bytes);
  }
  while (bytes > 0);

	alwBufferData(buffer, format, &bufferData[0], static_cast < ALsizei > (bufferData.size()), pInfo->rate);
	
	ov_clear(&oggFile);
	sounds.insert( pair<string,ALuint> (name, buffer) );
}

void TSAL_Manager::play_global(string name, float loudness, float pitch)
{
	int id_index = get_source(listenerPos[0], listenerPos[1], listenerPos[2]);
	if (id_index == -1) return;
	
	ALuint current = sources[id_index];
	
	map<string,ALuint>::iterator snd = sounds.find(name);
	if (sounds.end() == snd)
	{
		if (name != "") printf("%s - sound not found\n", name.c_str());
		return;
	}
	
	alwSourcef(current, AL_PITCH, pitch);
	alwSourcef(current, AL_GAIN, loudness*global_volume);
	alwSourcefv(current, AL_POSITION, listenerPos);
	alwSourcei(current, AL_BUFFER, (ALint)(snd->second));
	alwSourcef(current, AL_REFERENCE_DISTANCE, 1000);
	alwSourcei(current, AL_LOOPING, AL_FALSE);
	alwSourcePlay(current);
}

void TSAL_Manager::play_sound(string name, float x, float y, float z, float loudness, float pitch, float falloff)
{
	int id_index = get_source(x, y, z);
	if (id_index == -1) return;
	
	ALuint current = sources[id_index];
		
	map<string,ALuint>::iterator snd = sounds.find(name);
	if (sounds.end() == snd)
	{
		if (name != "") printf("%s - sound not found\n", name.c_str());
		return;
	}
	
	alwSourcef(current, AL_PITCH, pitch);
	alwSourcef(current, AL_GAIN, loudness*global_volume);
	alwSource3f(current, AL_POSITION, x, y, z);
	alwSource3f(current, AL_VELOCITY, 0, 0, 0);
	alwSourcei(current, AL_BUFFER, (ALint)(snd->second));
	alwSourcei(current, AL_LOOPING, AL_FALSE);
	alwSourcef(current, AL_REFERENCE_DISTANCE, reference_dist);
	alwSourcef(current, AL_ROLLOFF_FACTOR, falloff*falloff_mult);
	alwSourcePlay(current);
}

void TSAL_Manager::manage_source(TSAL_Priv_Source* src)
{
	if (src->id_taken_away)
	{
		if (!src->playing)
			return;
		
		int id_index = get_source(src->pos[0], src->pos[1], src->pos[2]);
		
		if (id_index == -1)
			return;
		else
		{
			src->id_taken_away = false;
			src->sample_changed = true;
			reservations[id_index] = src;
			src->source_id = sources[id_index];
		}
	}
	
	if (src->sample_changed)
	{
		alwSourceStop(src->source_id);
		
		src->sample_changed = false;
		
		map<string,ALuint>::iterator snd = sounds.find(src->sample);
		if (snd == sounds.end()) return;
	
		alwSourcei(src->source_id, AL_BUFFER, (ALint)(snd->second));
	}
	
	if (src->offset_changed == true)
	{
		alwSourceRewind(src->source_id);

    ALint total = 0;
    ALint buffer = 0;
    alwGetSourcei(src->source_id, AL_BUFFER, &buffer);
    alwGetBufferi(buffer, AL_SIZE, &total);
    alwSourcei(src->source_id, AL_BYTE_OFFSET, total*src->offset);
    
    src->offset_changed = false;
	}
	
	alwSourcefv(src->source_id, AL_POSITION, src->pos);
	alwSourcefv(src->source_id, AL_VELOCITY, src->vel);
	
	alwSourcef(src->source_id, AL_PITCH, src->pitch);
	alwSourcef(src->source_id, AL_GAIN, (src->loudness)*global_volume);
	alwSourcei(src->source_id, AL_LOOPING, src->loop);
	alwSourcef(src->source_id, AL_REFERENCE_DISTANCE, reference_dist);
	alwSourcef(src->source_id, AL_ROLLOFF_FACTOR, (src->falloff)*falloff_mult);
	
	int val;
	alwGetSourcei(src->source_id, AL_SOURCE_STATE, &val);
	if (src->playing && val != AL_PLAYING)
	{
		if (!src->loop && src->playbacks > 0)
			return;
		else
			alwSourcePlay(src->source_id);
		
		if (!src->loop) src->playbacks++;
	}
	else if (!src->playing && val == AL_PLAYING)
	{
		alwSourceStop(src->source_id);
	}
	
	if (src->paused && src->playing)
	{
		alwSourcePause(src->source_id);
	}
}

void TSAL_Manager::register_source(TSAL_Priv_Source* src)
{
	virtual_sources.push_back(src);
}

void TSAL_Manager::forget_source(TSAL_Priv_Source* src)
{
	for (int i = 0; i < TSAL_NUM_SOURCES; i++)
		if (reservations[i] == src)
		{
			alwSourceStop(sources[i]);
			reservations[i] = NULL;
			break;
		}
	list<TSAL_Priv_Source*>::iterator iter;
	for (iter = virtual_sources.begin(); iter != virtual_sources.end(); ++iter)
		if (*iter == src) 
		{
			virtual_sources.erase(iter);
			break;
		}
}

TSAL_Source TSAL_Manager::create_source(std::string sample, float x, float y, float z)
{
	TSAL_Priv_Source* src_priv = new TSAL_Priv_Source();
	src_priv->mixer = this;
	src_priv->sample = sample;
	src_priv->pos[0] = x;
	src_priv->pos[1] = y;
	src_priv->pos[2] = z;
	register_source(src_priv);
	TSAL_Source src(src_priv);
	return src;
}

void TSAL_Manager::manage_all_sources()
{
	list<TSAL_Priv_Source*>::iterator iter;
	for (iter = virtual_sources.begin(); iter != virtual_sources.end(); ++iter)
	{
		manage_source(*iter);
	}
}
