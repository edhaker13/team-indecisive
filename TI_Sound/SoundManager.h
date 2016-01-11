#pragma once
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <assert.h>
#include <dsound.h>
#include <string>
#include <map>
#include <mmsystem.h>
#include <stdio.h>
#include <windows.h>
#include "..\TI_Engine\ISoundManager.h"

#ifdef _WINDLL
#	define SOUND_API __declspec(dllexport)
#else
#	define SOUND_API __declspec(dllimport)
#endif

namespace Indecisive
{
	class SoundManager: public ISoundManager
	{
	private:
		std::map<std::string, IDirectSoundBuffer8*> m_Sounds;
		IDirectSound8* m_DirectSound = nullptr;
		IDirectSoundBuffer* m_primaryBuffer = nullptr;

		bool InitializeDirectSound(HWND);
		void ShutdownDirectSound();

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

	public:
		SOUND_API SoundManager() {};		
		SOUND_API bool CanRead(const std::string&) const override;
		SOUND_API const std::string GetInfo() const override { return "Reads a .wav file"; };
		SOUND_API bool Load(const std::string&) override;
		SOUND_API bool Play(const std::string& filename, DWORD dwReserved1, DWORD dwPriority, DWORD dwFlags) override;
		SOUND_API bool IsInitialised() override { return m_DirectSound != nullptr; };
		SOUND_API bool Initialize(HWND) override;
		SOUND_API void Shutdown() override;
	};
}


