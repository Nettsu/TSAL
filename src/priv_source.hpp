#include <AL/al.h>
#include <AL/alc.h>
#include <string>

class TSAL_Mixer;
class TSAL_Source;

class TSAL_Priv_Source
{
	friend class TSAL_Mixer;
	friend class TSAL_Source;
	
	public:
		TSAL_Priv_Source();
		~TSAL_Priv_Source();
		
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
		void take_id();
};
