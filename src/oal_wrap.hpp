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
#include <string>
#include <cstdio>

std::string ErrStr(ALenum err);

void alwListenerfv(ALenum param, ALfloat *values);

void alwDistanceModel(ALenum value);

void alwGenSources(ALsizei n, ALuint *sources);

void alwDeleteSources(ALsizei n, ALuint *sources);

void alwGenBuffers(ALsizei n, ALuint *buffers);

void alwDeleteBuffers(ALsizei n, ALuint *buffers);

void alwBufferData(ALuint buffer, ALenum format, const ALvoid *data, 
  ALsizei size, ALsizei freq);

bool alwIsSource(ALuint source);

void alwGetSourcei(ALuint source, ALenum pname, ALint *value);

void alwGetSourcefv(ALuint source, ALenum param, ALfloat *values);

void alwSourcef(ALuint source, ALenum param, ALfloat value);

void alwSource3f(ALuint source, ALenum param, ALfloat v1, ALfloat v2, 
  ALfloat v3);

void alwSourcefv(ALuint source, ALenum param, ALfloat* values);

void alwSourcei(ALuint source, ALenum param, ALint value);

void alwSourcePlay(ALuint source);

void alwSourceStop(ALuint source);

void alwSourcePause(ALuint source);

void alwSourceRewind(ALuint source);

void alwGetBufferi(ALuint buffer, ALenum pname, ALint *value);
