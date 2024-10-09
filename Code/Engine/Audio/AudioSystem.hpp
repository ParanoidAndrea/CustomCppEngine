#pragma once


//-----------------------------------------------------------------------------------------------
#include "ThirdParty/fmod/fmod.hpp"
#include "Engine/Math/Vec3.hpp"
#include <string>
#include <vector>
#include <map>


//-----------------------------------------------------------------------------------------------
typedef size_t SoundID;
typedef size_t SoundPlaybackID;
constexpr size_t MISSING_SOUND_ID = (size_t)(-1); // for bad SoundIDs and SoundPlaybackIDs


//-----------------------------------------------------------------------------------------------
struct AudioConfig
{
	float m_mainVolume = 1.f;
	float m_UIVolume = 1.f;
	float m_musicVolume = 1.f;
};
enum class SoundClass
{
	MAIN,
	UI,
	MUSIC
};
/////////////////////////////////////////////////////////////////////////////////////////////////
class AudioSystem
{
public:
	AudioSystem(AudioConfig const& audioConfig);
	virtual ~AudioSystem();

public:
	void						Startup();
	void						Shutdown();
	virtual void				BeginFrame();
	virtual void				EndFrame();

	virtual SoundID CreateOrGetSound( const std::string& soundFilePath, bool is3D = false );
	virtual SoundPlaybackID StartSound( SoundID soundID, bool isLooped=false, float volume=1.f, float balance=0.0f, float speed=1.0f, bool isPaused=false, SoundClass soundClass = SoundClass::MAIN );
	virtual void				StopSound( SoundPlaybackID soundPlaybackID );
	virtual void				SetSoundPlaybackVolume( SoundPlaybackID soundPlaybackID, float volume );	// volume is in [0,1]
	virtual void				SetSoundPlaybackBalance( SoundPlaybackID soundPlaybackID, float balance );	// balance is in [-1,1], where 0 is L/R centered
	virtual void				SetSoundPlaybackSpeed( SoundPlaybackID soundPlaybackID, float speed );		// speed is frequency multiplier (1.0 == normal)

	virtual void				ValidateResult( FMOD_RESULT result );
	void SetVolume(float volume, SoundClass soundClass = SoundClass::MAIN);
	void SetNumListeners(int numListeners);
	void UpdateListener(int listeberIndex, Vec3 const& listenerPosition, Vec3 const& listenerForawrd, Vec3 const& listenerUp);
	virtual SoundPlaybackID StartSoundAt(SoundID soundID, Vec3 const& soundPosition, bool isLooped = false, float volume = 1.0f, float balance = 0.0f, float speed = 1.0f, bool isPaused = false, SoundClass soundClass = SoundClass::MAIN);
	virtual void SetSoundPosition(SoundPlaybackID soundPlaybackID, Vec3 const& soundPosition);
	bool IsPlaying(SoundPlaybackID soundPlaybackID);
	float GetUIVolume() const;
	float GetMusicVolume() const;
	float GetMainVolume() const;
protected:
	FMOD::System*						m_fmodSystem;
	std::map< std::string, SoundPlaybackID >	m_registeredSoundIDs;
	std::vector< FMOD::Sound* >			m_registeredSounds;

private:
	std::vector<FMOD::Channel*> m_registeredChannels;
	std::vector<FMOD::Channel*> m_musicChannels;
	std::vector<FMOD::Channel*> m_UIChannels;
	AudioConfig m_audioConfig;
};

