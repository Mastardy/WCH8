#include "AudioManager.hpp"

void AudioManager::PlaySound(unsigned short frequency, unsigned int duration)
{
	Beep(frequency, duration);
}
