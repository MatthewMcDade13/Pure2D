#ifndef PURE2D_AUDIO_AUDIOSOURCE_H
#define PURE2D_AUDIO_AUDIOSOURCE_H

#include <cinttypes>
#include <Pure2D/Math/Vec3.h>
#include <Pure2D/Audio/SoundBuffer.h>

namespace pure
{
	struct SoundBuffer;

	struct AudioSource
	{
		static AudioSource make();

		uint32_t id() const;
		void free();

		// expects percentages as value 0.f to 100.f
		void setPitch(float pitch);
		float pitch() const;

		// expects percentages as value 0.f to 100.f
		void setVolume(float volume);
		float volume() const;

		void setPosition(const Vec3f& position);
		const Vec3f& position() const;

		void setVelocity(const Vec3f& velocity);
		const Vec3f& velocity() const;

		void setLooping(bool isLooping);
		bool isLooping() const;

		void unbind();
		void bindBuffer(SoundBuffer buffer);
		SoundBuffer buffer();

		void play() const;
		void stop() const;
		void pause() const;
		void rewind() const;

	private:
		uint32_t m_id;
		float m_pitch;
		float m_volume;
		Vec3f m_position;
		Vec3f m_velocity;
		bool m_looping;

		SoundBuffer m_buffer = {};
	};
}

#endif // PURE2D_AUDIO_AUDIOSOURCE_H
