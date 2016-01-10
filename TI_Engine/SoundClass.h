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

		void ShutdownWaveFile(IDirectSoundBuffer8**);

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

		IDirectSound8* m_DirectSound = nullptr;
		IDirectSoundBuffer* m_primaryBuffer = nullptr;
		IDirectSoundBuffer8* m_secondaryBuffer1 = nullptr;

	public:
		SoundClass();
		SoundClass(const SoundClass&);
		~SoundClass();


		bool LoadWaveFile(char*, IDirectSoundBuffer8**);
		bool PlayWaveFile(IDirectSoundBuffer8* secondaryBuffer, DWORD dwReserved1, DWORD dwPriority, DWORD dwFlags);

		bool Initialize(HWND);
		void Shutdown();

		HRESULT CreateBasicBuffer(LPDIRECTSOUND8 lpDirectSound, IDirectSoundBuffer8* ppDsb8);

		IDirectSound8* GetDirectSound() const							{ return m_DirectSound; }

		IDirectSoundBuffer8* GetSecondaryBuffer() const					{ return m_secondaryBuffer1; }
		void SetSecondaryBuffer(IDirectSoundBuffer8* secondaryBuffer)	{ this->m_secondaryBuffer1 = secondaryBuffer; }
	};
}


