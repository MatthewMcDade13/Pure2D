#include "Listener.h"
#include "Private/ErrorHandling.h"
#include <Pure2D/Math/Vec3.h>
#include <AL/al.h>

using namespace pure;

static Vec3f l_up = { 0.f, 1.f, 0.f };
static Vec3f l_direction = { 0.f, 0.f, -1.f };
static Vec3f l_position = { 0.f, 0.f, 0.f };
static Vec3f l_velocity = { 0.f, 0.f, 0.f };
static float l_volume = 100.f;

static constexpr float toAlPercent(float volume) { return volume * 0.01f; }

// TODO: Error Handling for OpenAL calls

void pure::listener::setUpVector(const Vec3<float>& up)
{
	const float orientation[] = {
		l_direction.x, l_direction.y, l_direction.z,
		up.x, up.y, up.z
	};

	alCall(alListenerfv(AL_ORIENTATION, orientation));

	l_up = up;
}

void pure::listener::setDirection(const Vec3<float>& direction)
{
	const float orientation[] = {
		l_direction.x, l_direction.y, l_direction.z,
		l_up.x, l_up.y, l_up.z
	};

	alCall(alListenerfv(AL_ORIENTATION, orientation));

	l_direction = direction;
}

void pure::listener::setOrientation(const Vec3<float>& direction, const Vec3<float>& up)
{
	const float orientation[] = {
		direction.x, direction.y, direction.z,
		up.x, up.y, up.z
	};

	alCall(alListenerfv(AL_ORIENTATION, orientation));

	l_up = up;
	l_direction = direction;
}

void pure::listener::resetDefaults()
{
	const float orientation[] = {
		l_direction.x, l_direction.y, l_direction.z,
		l_up.x, l_up.y, l_up.z
	};

	alCall(alListenerfv(AL_ORIENTATION, orientation));
	alCall(alListener3f(AL_POSITION, l_position.x, l_position.y, l_position.z));
	alCall(alListener3f(AL_VELOCITY, l_velocity.x, l_velocity.y, l_velocity.z));
	alCall(alListenerf(AL_GAIN, toAlPercent(l_volume)));
}

void pure::listener::setPosition(const Vec3<float>& pos)
{
	alCall(alListener3f(AL_POSITION, pos.x, pos.y, pos.z));
	l_position = pos;
}

void pure::listener::setVelocity(const Vec3<float>& vel)
{
	alCall(alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z));
	l_velocity = vel;
}

void pure::listener::setVolume(float volume)
{
	alCall(alListenerf(AL_GAIN, toAlPercent(volume)));
	l_volume = volume;
}


float pure::listener::getVolume() { return l_volume; }

Vec3<float> pure::listener::getUpVector() {	return l_up; }

Vec3<float> pure::listener::getDirection() { return l_direction; }

Vec3<float> pure::listener::getPosition() { return l_position; }

Vec3<float> pure::listener::getVelocity() { return l_velocity; }
