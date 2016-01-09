#pragma once
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

namespace Indecisive
{

	class SoundClass
	{
	private:

		bool InitializeDirectSound(HWND);
		void ShutdownDirectSound();

		bool LoadWaveFile(char*, IDirectSoundBuffer8**);
		void ShutdownWaveFile(IDirectSoundBuffer8**);

		bool PlayWaveFile();

		void GetSecondaryBuffer(IDirectSoundBuffer8* sound);

		struct WaveHeaderType
		{
			char chunkId[4];
			unsigned long chunkSize;
			char format[4];
			char subChunkId[4];
			unsigned long subChunkSize;
			unsigned short audioFormat;
			unsigned short numChannels;
			unsigned long sampleRate;
			unsigned long bytesPerSecond;
			unsigned short blockAlign;
			unsigned short bitsPerSample;
			char dataChunkId[4];
			unsigned long dataSize;
		};

		IDirectSound8* m_DirectSound;
		IDirectSoundBuffer* m_primaryBuffer;
		IDirectSoundBuffer8* m_secondaryBuffer1;

	public:
		SoundClass();
		SoundClass(const SoundClass&);
		~SoundClass();

		bool Initialize(HWND);
		void Shutdown();
	};
}


