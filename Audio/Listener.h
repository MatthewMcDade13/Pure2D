#ifndef PURE2D_AUDIO_LISTENER_H
#define PURE2D_AUDIO_LISTENER_H

namespace pure
{
	template<typename T> struct Vec3;

	namespace listener
	{
		void setUpVector(const Vec3<float>& up);
		void setDirection(const Vec3<float>& direction);
		void setPosition(const Vec3<float>& pos);
		void setVelocity(const Vec3<float>& vel);
		void setOrientation(const Vec3<float>& direction, const Vec3<float>& up);

		void resetDefaults();

		// Set listener volume between 0.0f and 100.0f
		void setVolume(float volume);

		// Betweeen range 0.0f and 100.0f;
		float getVolume();

		Vec3<float> getUpVector();
		Vec3<float> getDirection();
		Vec3<float> getPosition();
		Vec3<float> getVelocity();
	}
}

#endif // PURE2D_AUDIO_LISTENER_H
