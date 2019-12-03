#include "AudioSource.h"
#include "Private/ErrorHandling.h"
#include <AL/al.h>
#include <cassert>


using namespace pure;

static constexpr float toAlPercent(float volume) { return volume * 0.01f; }

AudioSource pure::AudioSource::make()
{
	AudioSource as = {};
	alCall(alGenSources(1, &as.m_id));

	as.setPitch(100.f);
	as.setVolume(100.f);
	as.setPosition(Vec3f::single(0.f));
	as.setVelocity(Vec3f::single(0.f));
	as.setLooping(false);

	return as;
}

uint32_t pure::AudioSource::id() const { return m_id; }

void pure::AudioSource::free()
{
	alCall(alDeleteSources(1, &m_id));
	m_id = 0;
}

float pure::AudioSource::pitch() const { return m_pitch; }
void pure::AudioSource::setPitch(float pitch)
{
	alCall(alSourcef(m_id, AL_PITCH, toAlPercent(pitch)));
	m_pitch = pitch;
}

float pure::AudioSource::volume() const { return m_volume; }
void pure::AudioSource::setVolume(float volume)
{
	alCall(alSourcef(m_id, AL_GAIN, toAlPercent(volume)));
	m_volume = volume;
}

const Vec3f & pure::AudioSource::position() const { return m_position; }
void pure::AudioSource::setPosition(const Vec3f & position)
{
	alCall(alSource3f(m_id, AL_POSITION, position.x, position.y, position.z));
	m_position = position;
}

const Vec3f & pure::AudioSource::velocity() const { return m_velocity; }
void pure::AudioSource::setVelocity(const Vec3f & velocity)
{
	alCall(alSource3f(m_id, AL_VELOCITY, velocity.x, velocity.y, velocity.z));
	m_velocity = velocity;
}

bool pure::AudioSource::isLooping() const { return m_looping; }
void pure::AudioSource::setLooping(bool isLooping)
{
	alCall(alSourcei(m_id, AL_LOOPING, isLooping));
	m_looping = isLooping;
}

SoundBuffer pure::AudioSource::buffer() { return m_buffer; }

void pure::AudioSource::unbind()
{
	alCall(alSourcei(m_id, AL_BUFFER, 0));
	m_buffer = {};
}

void pure::AudioSource::bindBuffer(SoundBuffer buffer)
{
	alCall(alSourcei(m_id, AL_BUFFER, buffer.id()));
	m_buffer = buffer;
}

void pure::AudioSource::play() const
{
	assert(m_buffer.id() != 0);
	alCall(alSourcePlay(m_id));
}

void pure::AudioSource::stop() const
{
	assert(m_buffer.id() != 0);
	alCall(alSourceStop(m_id));
}

void pure::AudioSource::pause() const
{
	assert(m_buffer.id() != 0);
	alCall(alSourcePause(m_id));
}

void pure::AudioSource::rewind() const
{
	assert(m_buffer.id() != 0);
	alCall(alSourceRewind(m_id));
}

