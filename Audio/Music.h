#ifndef PURE2D_AUDIO_SOUND_H
#define PURE2D_AUDIO_SOUND_H

namespace pure
{
	struct Music
	{
		void free();

		void setVolume(float percentage);
		bool loadFromFile(const char* filename);

		bool play(int loops = 0);

	private:
		void* m_handle;
	};
}

#endif // PURE2D_AUDIO_SOUND_H

