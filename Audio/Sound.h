#ifndef PURE2D_AUDIO_SOUND_H
#define PURE2D_AUDIO_SOUND_H

namespace pure
{
	struct Sound
	{
		void free();

		void setVolume(float percentage);
		bool loadWavFromFile(const char* filename);

		bool play(int loops = 0, int channel = -1);

	private:
		void* m_handle;
	};
}

#endif // PURE2D_AUDIO_SOUND_H

