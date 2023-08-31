#include "Sound.h"
#include <cstdio>
#include <cstring>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

/* static member function */
void Sound::stopAll() {
	al_stop_samples();
}

/* member functions */
Sound::Sound()
	: filename(nullptr), sample(nullptr), 
	volume(1.0f), pan(0.0f), speed(1.0f),
	muted(false)
{
	printf("Sound()\n");
}

Sound::~Sound() {
	printf("~Sound(): %s\n", filename);
	if (filename)
		delete filename;
	if (sample)
		al_destroy_sample(sample);
}

bool Sound::load(const char *filename) {
	printf("Sound::load(): %s\n", filename);
	this->filename = new char[strlen(filename) + 1];
	strcpy(this->filename, filename);
	sample = al_load_sample(filename);
	if (!sample) {
		return false;
	}
	return true;
}

bool Sound::play(bool loop) {
	if (muted)
		return true;

	ALLEGRO_SAMPLE_ID sid;
	ALLEGRO_PLAYMODE mode = loop ? ALLEGRO_PLAYMODE_LOOP : ALLEGRO_PLAYMODE_ONCE;

	al_play_sample(
		sample, 
		volume, 
		pan, 
		speed, 
		mode, 
		&sid);

	if (sid._id == -1)
		return false;
	return true;
}

void Sound::stop() {
	//al_stop_sample(sample);
	printf("Sound::stop()\n");
}

const char* Sound::getFilename() const {
	return filename;
}

float Sound::getVolume() const {
	return volume;
}

float Sound::getPan() const {
	return pan;
}

float Sound::getSpeed() const {
	return speed;
}

void Sound::setPan(float p) {
	if (p < 0.0f)
		p = 0.0f;
	else if (p > 1.0f)
		p = 1.0f;
	pan = p;
}
void Sound::setVolume(float v) {
	if (v < 0.0f)
		v = 0.0f;
	volume = v;
}
void Sound::setSpeed(float s) {
	if (s <= 0.0f)
		s = 1.0f;
	speed = s;
}

bool Sound::isMuted() const {
	return muted;
}

void Sound::setMute(bool m) {
	muted = m;
}