#include <map>
#include <vector>
#include <AL/alc.h>
#include <vorbis/vorbisfile.h>
#include "tsal.hpp"
#include "oal_wrap.hpp"

#define BUFFER_SIZE 32768     // 32 KB buffers

using namespace std;

TSAL_Mixer* TSAL_Mixer::instance = NULL;  
  
TSAL_Mixer* TSAL_Mixer::get_mixer()
{
	if (!instance)   // Only allow one instance of class to be generated.
		instance = new TSAL_Mixer();

	return instance;
}

TSAL_Mixer::TSAL_Mixer()
{
	listenerOri[0] = 0;
	listenerOri[1] = 0;
	listenerOri[2] = -1;
	global_volume = 1;
}

void TSAL_Mixer::set_volume(float v)
{
	global_volume = v;
}

TSAL_Source* TSAL_Mixer::create_source()
{
	return new TSAL_Source(this);
}

TSAL_Source* TSAL_Mixer::create_source(string n, bool l)
{
	return new TSAL_Source(this, n, l);
}

ALuint TSAL_Mixer::get_source(float x, float y, float z)
{
	float max_dist = 0;
	float temp_dist = 0;
	ALuint best_candidate = 0;
	float pos[3];
	int val;
	for (int i = 0; i < TSAL_NUM_SOURCES; i++)
	{
		alGetSourcei(TSAL_Mixer::source[i], AL_SOURCE_STATE, &val);
		if (val != AL_PLAYING)
		{
			if (TSAL_Mixer::reserved_sources[i] != NULL)
			{
				TSAL_Mixer::reserved_sources[i]->take_id();
				TSAL_Mixer::reserved_sources[i] = NULL;
			}
			return i;
		}
		alGetSourcefv(source[i], AL_POSITION, &pos[0]);
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
	if (temp_dist >= max_dist) return -1;
	
	if (TSAL_Mixer::reserved_sources[best_candidate] != NULL)
	{
		TSAL_Mixer::reserved_sources[best_candidate]->take_id();
		TSAL_Mixer::reserved_sources[best_candidate] = NULL;
	}
	alSourceStop(TSAL_Mixer::source[best_candidate]);
	return best_candidate;
}

void TSAL_Mixer::init_sfx()
{
	//alutInit(0, NULL);
	device = alcOpenDevice(NULL);
	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);

	alListenerfv(AL_POSITION,listenerPos);
	alListenerfv(AL_VELOCITY,listenerVel);
	alListenerfv(AL_ORIENTATION,listenerOri);
	alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
	alGenSources(TSAL_NUM_SOURCES, source);
	for (int i = 0; i < TSAL_NUM_SOURCES; i++)
		reserved_sources[i] = NULL;
}

void TSAL_Mixer::stop_sfx()
{	
	alDeleteSources(TSAL_NUM_SOURCES, source);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

void TSAL_Mixer::listener_pos(float pos_x, float pos_y, float pos_z)
{
	listenerPos[0] = pos_x;
	listenerPos[1] = pos_y;
	listenerPos[2] = pos_z;

	alListenerfv(AL_POSITION,listenerPos);
}

void TSAL_Mixer::listener_vel(float vel_x, float vel_y, float vel_z)
{
	listenerVel[0] = vel_x;
	listenerVel[1] = vel_y;
	listenerVel[2] = vel_z;
}

void TSAL_Mixer::listener_facing(float front_x, float front_y, float front_z, float up_x, float up_y, float up_z)
{
	listenerOri[0] = front_x;
	listenerOri[1] = front_y;
	listenerOri[2] = front_z;
	listenerOri[3] = up_x;
	listenerOri[4] = up_y;
	listenerOri[5] = up_z;

	alListenerfv(AL_ORIENTATION,listenerOri);
}

void TSAL_Mixer::load_sound(string file, string name)
{
	ALuint buffer;            // The OpenAL sound buffer ID
	alGenBuffers(1, &buffer);
	ALenum format;              // The sound data format
	vector<char> bufferData; // The sound buffer data from file
	
	int endian = 0;             // 0 for Little-Endian, 1 for Big-Endian
  int bitStream;
  long bytes;
  char array[BUFFER_SIZE];    // Local fixed size array
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

	alBufferData(buffer, format, &bufferData[0], static_cast < ALsizei > (bufferData.size()), pInfo->rate);
	
	ov_clear(&oggFile);
	sounds.insert( pair<string,ALuint> (name, buffer) );
}

void TSAL_Mixer::play_global(string name, float loudness, float pitch)
{
	int id_index = get_source(listenerPos[0], listenerPos[1], 0);
	if (id_index == -1) return;
	
	ALuint current = source[id_index];
	
	map<string,ALuint>::iterator snd = sounds.find(name);
	if (sounds.end() == snd)
	{
		if (name != "") printf("%s - sound not found\n", name.c_str());
		return;
	}
	
	alSourcef(current, AL_PITCH, pitch);
	alSourcef(current, AL_GAIN, loudness*global_volume);
	alSource3f(current, AL_POSITION, listenerPos[0], listenerPos[1], 0);
	alSourcei(current, AL_BUFFER, snd->second);
	alSourcef(current, AL_REFERENCE_DISTANCE, 1000);
	alSourcei(current, AL_LOOPING, AL_FALSE);
	alSourcePlay(current);
}

void TSAL_Mixer::play_sound(string name, float x, float y, float z, float loudness, float pitch, float falloff)
{
	int id_index = get_source(x, y, z);
	if (id_index == -1) return;
	
	ALuint current = source[id_index];
		
	map<string,ALuint>::iterator snd = sounds.find(name);
	if (sounds.end() == snd)
	{
		if (name != "") printf("%s - sound not found\n", name.c_str());
		return;
	}
	
	alSourcef(current, AL_PITCH, pitch);
	alSourcef(current, AL_GAIN, loudness*global_volume);
	alSource3f(current, AL_POSITION, x, y, z);
	alSource3f(current, AL_VELOCITY, 0, 0, 0);
	alSourcei(current, AL_BUFFER, snd->second);
	alSourcei(current, AL_LOOPING, AL_FALSE);
	alSourcef(current, AL_REFERENCE_DISTANCE, TSAL_REF_DIST);
	alSourcef(current, AL_ROLLOFF_FACTOR, falloff*TSAL_FALLOFF_MULT);
	alSourcePlay(current);
}

void TSAL_Mixer::manage_sources()
{
	for (list<TSAL_Source*>::iterator iter = csources.begin(); iter != csources.end(); ++iter)
	{
		(*iter)->manage();
	}
}
