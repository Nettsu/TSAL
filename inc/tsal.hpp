#include <AL/al.h>
#include <AL/alc.h>
#include <string>
#include <list>
#include <map>

#define TSAL_NUM_BUFFERS 128
#define TSAL_NUM_SOURCES 16

#define TSAL_REF_DIST 20
#define TSAL_FALLOFF_MULT 2

class TSAL_Mixer;

class TSAL_Source
{
	private:
		ALfloat pos[3];
		ALfloat vel[3];
		std::string name;
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
		TSAL_Mixer* mixer;
		
		TSAL_Source(TSAL_Mixer* mix);
		TSAL_Source(TSAL_Mixer* mix, std::string n, bool l);
		void manage();
		void take_id();

	public:
		~TSAL_Source();
		void start();
		void stop();
		void pause();
		void update(float p, float loud, bool l);
		void set_pos(float pos_x, float pos_y, float pos_z = 0);
		void set_vel(float vel_x, float vel_y, float vel_z = 0);
		void set_offset(float off);
		void change_sample(std::string n);
		bool is_playing();
		std::string sample();
		
	friend class TSAL_Mixer;
};

class TSAL_Mixer
{
	private:
		std::map <std::string, ALuint> sounds;
		ALfloat listenerPos[3];
		ALfloat listenerVel[3];
		ALfloat listenerOri[6];
		std::list<TSAL_Source*> csources;
		ALuint source[TSAL_NUM_SOURCES];
		TSAL_Source* reserved_sources[TSAL_NUM_SOURCES];
		float global_volume;
		ALCdevice* device;
		ALCcontext* context;
		
		int get_source(float x, float y, float z);
		TSAL_Mixer();
		static TSAL_Mixer* instance;
		
	public:
		static TSAL_Mixer* get_mixer();
		void init_sfx();
		void stop_sfx();
		void load_sound(std::string file, std::string name);
		void play_global(std::string name, float loudness = 1, float pitch = 1);
		void play_sound(std::string name, float x, float y, float z = 0, float loudness = 1, float pitch = 1, float falloff = 1);
		void listener_pos(float pos_x, float pos_y, float pos_z = 0);
		void listener_vel(float vel_x, float vel_y, float vel_z = 0);
		void listener_facing(float front_x, float front_y, float front_z, float up_x, float up_y, float up_z);
		void manage_sources();
		void set_volume(float v);
		TSAL_Source* create_source();
		TSAL_Source* create_source(std::string n, bool l);
		
	friend class TSAL_Source;
};
