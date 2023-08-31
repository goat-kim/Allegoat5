#pragma once
struct ALLEGRO_SAMPLE;
class Sound
{
private:
	char* filename;
	ALLEGRO_SAMPLE* sample;
	float volume;	// relative volume; 1.0 is normal
	float pan;		// 0.0: centered, -1.0: left, 1.0: right
	float speed;	// relative speed; 1.0 is normal
	bool muted;

public:
	Sound();
	~Sound();
	bool load(const char *filename);
	bool play(bool loop = false);
	void stop();
	static void stopAll();

	const char* getFilename() const;

	float getVolume() const;
	float getPan() const;
	float getSpeed() const;

	void setPan(float p);
	void setVolume(float v);
	void setSpeed(float s);

	bool isMuted() const;
	void setMute(bool m);
};