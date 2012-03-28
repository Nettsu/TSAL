#include "oal_wrap.hpp"

using namespace std;

string ErrStr(ALenum err)
{
    switch(err)
    {
        case AL_NO_ERROR:
            return string("AL_NO_ERROR");

        case AL_INVALID_NAME:
            return string("AL_INVALID_NAME");

        case AL_INVALID_ENUM:
            return string("AL_INVALID_ENUM");

        case AL_INVALID_VALUE:
            return string("AL_INVALID_VALUE");

        case AL_INVALID_OPERATION:
            return string("AL_INVALID_OPERATION");

        case AL_OUT_OF_MEMORY:
            return string("AL_OUT_OF_MEMORY");
    };
    return "Unknown OpenAL error!";
}

void alwListenerfv(ALenum param, ALfloat* values)
{
	int error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Before alListenerfv %i : %s\n", param, ErrStr(error).c_str());
	alListenerfv(param, values);
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("After alListenerfv %i : %s\n", param, ErrStr(error).c_str());
}

void alwDistanceModel(ALenum value)
{
	int error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Before alDistanceModel %i : %s\n", value, ErrStr(error).c_str());
	alDistanceModel(value);
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("After alDistanceModel %i : %s\n", value, ErrStr(error).c_str());
}

void alwGenSources(ALsizei n, ALuint *sources)
{
	int error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Before alGenSources %i : %s\n", n, ErrStr(error).c_str());
	alGenSources(n, sources);
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("After alGenSources %i : %s\n", n, ErrStr(error).c_str());
}

void alwDeleteSources(ALsizei n, ALuint *sources)
{
	int error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Before alDeleteSources %i : %s\n", n, ErrStr(error).c_str());
	alDeleteSources(n, sources);
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("After alDeleteSources %i : %s\n", n, ErrStr(error).c_str());
}

void alwGenBuffers(ALsizei n, ALuint *buffers)
{
	int error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Before alGenBuffers %i : %s\n", n, ErrStr(error).c_str());
	alGenBuffers(n, buffers);
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("After alGenBuffers %i : %s\n", n, ErrStr(error).c_str());
}

void alwDeleteBuffers(ALsizei n, ALuint* buffers)
{
	int error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Before alDeleteBuffers %i : %s\n", n, ErrStr(error).c_str());
	alDeleteBuffers(n, buffers);
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("After alDeleteBuffers %i : %s\n", n, ErrStr(error).c_str());
}

void alwBufferData(ALuint buffer, ALenum format, const ALvoid *data, ALsizei size, ALsizei freq)
{
	int error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Before alBufferData %i %i : %s\n", buffer, size, ErrStr(error).c_str());
	alBufferData(buffer, format, data, size, freq);
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("After alBufferData %i %i : %s\n", buffer, size, ErrStr(error).c_str());
}

bool alwIsSource(ALuint source)
{
	int error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Before alIsSource %i : %s\n", source, ErrStr(error).c_str());
	bool ret = alIsSource(source);
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("After alIsSource %i : %s\n", source, ErrStr(error).c_str());
	return ret;
}

void alwGetSourcei(ALuint source, ALenum pname, ALint* value)
{
	int error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Before alGetSourcei %i : %s\n", pname, ErrStr(error).c_str());
	alGetSourcei(source, pname, value);
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("After alGetSourcei %i : %s\n", pname, ErrStr(error).c_str());
}

void alwGetSourcefv(ALuint source, ALenum param, ALfloat* values)
{
	int error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Before alGetSourcefv %i : %s\n", param, ErrStr(error).c_str());
	alGetSourcefv(source, param, values);
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("After alGetSourcefv %i : %s\n", param, ErrStr(error).c_str());
}

void alwSourcef(ALuint source, ALenum param, ALfloat value)
{
	int error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Before alSourcef %i %f : %s\n", param, value, ErrStr(error).c_str());
	alSourcef(source, param, value);
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("After alSourcef %i %f : %s\n", param, value, ErrStr(error).c_str());
}

void alwSource3f(ALuint source, ALenum param, ALfloat v1, ALfloat v2, ALfloat v3)
{
	int error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Before alSource3f %i %f %f %f : %s\n", param, v1, v2, v3, ErrStr(error).c_str());
	alSource3f(source, param, v1, v2, v3);
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("After alSource3f %i %f %f %f: %s\n", param, v1, v2, v3, ErrStr(error).c_str());
}

void alwSourcefv(ALuint source, ALenum param, ALfloat *values)
{
	int error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Before alSourcefv %i : %s\n", param, ErrStr(error).c_str());
	alSourcefv(source, param, values);
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("After alSourcefv %i : %s\n", param, ErrStr(error).c_str());
}

void alwSourcei(ALuint source, ALenum param, ALint value)
{
	int error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Before alSourcei %i %i : %s\n", param, value, ErrStr(error).c_str());
	alSourcei(source, param, value);
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("After alSourcei %i %i : %s\n", param, value, ErrStr(error).c_str());
}

void alwSourcePlay(ALuint source)
{
	int error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Before alSourcePlay %i : %s\n", source, ErrStr(error).c_str());
	alSourcePlay(source);
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("After alSourcePlay %i : %s\n", source, ErrStr(error).c_str());
}

void alwSourceStop(ALuint source)
{
	int error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Before alSourceStop %i : %s\n", source, ErrStr(error).c_str());
	alSourceStop(source);
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("After alSourceStop %i : %s\n", source, ErrStr(error).c_str());
}

void alwSourcePause(ALuint source)
{
	int error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Before alSourcePause %i : %s\n", source, ErrStr(error).c_str());
	alSourcePause(source);
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("After alSourcePause %i : %s\n", source, ErrStr(error).c_str());
}

void alwSourceRewind(ALuint source)
{
	int error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Before alSourceRewind %i : %s\n", source, ErrStr(error).c_str());
	alSourceRewind(source);
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("After alSourceRewind %i : %s\n", source, ErrStr(error).c_str());
}

void alwGetBufferi(ALuint buffer, ALenum pname, ALint *value)
{
	int error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Before alGetBufferi %i : %s\n", pname, ErrStr(error).c_str());
	alGetBufferi(buffer, pname, value);
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("After alGetBufferi %i : %s\n", pname, ErrStr(error).c_str());
}

