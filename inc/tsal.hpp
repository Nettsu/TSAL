#include <AL/al.h>
#include <AL/alc.h>
#include <string>
#include <list>
#include <map>

#define TSAL_NUM_BUFFERS 128
#define TSAL_NUM_SOURCES 16

class TSAL_Priv_Source;

class TSAL_Source
{
	friend class TSAL_Manager;
	
	public:
		TSAL_Source();
		~TSAL_Source();
		TSAL_Source(const TSAL_Source& source);
		TSAL_Source& operator=(const TSAL_Source& source);
		void start();
		void stop();
		void pause();
		void set_pitch(float p);
		void set_loudness(float loud);
		void set_loop(bool l);
		void set_pos(float pos_x, float pos_y, float pos_z = 0);
		void set_vel(float vel_x, float vel_y, float vel_z = 0);
		void set_offset(float off);
		void set_falloff(float f);
		void set_sample(std::string n);
		bool is_playing();
		std::string sample();
		
	private:
		TSAL_Priv_Source* priv;
		TSAL_Source(TSAL_Priv_Source* ptr);
};

class TSAL_Manager
{
	friend class TSAL_Priv_Source;
		
	public:
		TSAL_Manager();
		~TSAL_Manager();
		void load_sound(std::string file, std::string name);
		void play_global(std::string name, float loudness = 1, float pitch = 1);
		void play_sound(std::string name, float x, float y, float z = 0, float loudness = 1, float pitch = 1, float falloff = 1);
		void listener_pos(float pos_x, float pos_y, float pos_z = 0);
		void listener_vel(float vel_x, float vel_y, float vel_z = 0);
		void listener_facing(float front_x, float front_y, float front_z, float up_x, float up_y, float up_z);
		void set_volume(float v);
		void set_ref_dist(float d);
		void set_falloff(float f);
		void manage_all_sources();
		TSAL_Source create_source(std::string sample = "", float x = 0, float y = 0, float z = 0);
		
	private:
		std::map <std::string, ALuint> sounds;
		ALfloat listenerPos[3];
		ALfloat listenerVel[3];
		ALfloat listenerOri[6];
		
		std::list<TSAL_Priv_Source*> virtual_sources;
		ALuint sources[TSAL_NUM_SOURCES];
		TSAL_Priv_Source* reservations[TSAL_NUM_SOURCES];
		
		float global_volume;
		float reference_dist;
		float falloff_mult;
		
		ALCdevice* device;
		ALCcontext* context;
		
		int get_source(float x, float y, float z);
		void manage_source(TSAL_Priv_Source* src);
		void register_source(TSAL_Priv_Source* src);
		void forget_source(TSAL_Priv_Source* src);
};
