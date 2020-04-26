#ifndef PURE2D_AUDIO_SOUNDBUFFER_H
#define PURE2D_AUDIO_SOUNDBUFFER_H

#include <cinttypes>
#include <cstddef>

namespace pure
{
	enum class SoundFormat
	{
		MONO8, MONO16, STEREO8, STEREO16
	};

	struct SoundBuffer
	{
		static SoundBuffer make();

		uint32_t id() const;

		void free();

		void write(SoundFormat format, void* data, size_t size, size_t frequency = 48000) const;
		bool loadFromFile(const char* filename);

	private:
		uint32_t m_id;
	};
}

#endif // PURE2D_AUDIO_SOUNDBUFFER_H
