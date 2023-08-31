#include "Util.h"
#include <cstdio>
#include <ctime>
#include <allegro5/allegro5.h>

const char* getCurrentTimestamp(void) {
	static char timestamp[20];
	time_t rawTime = time(NULL);
	struct tm* ptTime;
	ptTime = localtime(&rawTime);

	int year = ptTime->tm_year + 1900;
	int month = ptTime->tm_mon + 1;
	int day = ptTime->tm_mday;
	int hour = ptTime->tm_hour;
	int min = ptTime->tm_min;
	int sec = ptTime->tm_sec;
	// "yyyymmddhhmmss"
	sprintf(timestamp, "%04d%02d%02d%02d%02d%02d", year, month, day, hour, min, sec);
	return timestamp;
}

void ustrDisplayCodes(ALLEGRO_USTR* str) {
	int pos = 0;
	int32_t code;
	while ((code = al_ustr_get_next(str, &pos)) != -1) {
		printf("%d ", code);
	}
	printf("\n");
}

UstrVector::UstrVector() {
	printf("UstrVector()\n");
	vec.clear();
}

UstrVector::~UstrVector() {
	std::vector<ALLEGRO_USTR*>::iterator iter;
	printf("~UstrVector()\n");
	for (iter = vec.begin(); iter != vec.end(); iter++) {
		al_ustr_free(*iter);
	}
}

bool UstrVector::load(const char* pathname) {
	FILE* fp = fopen(pathname, "rt");

	char buf[BUF_SIZE];

	if (!fp) {
		fprintf(stderr, "E: failed to load ustr file: %s", pathname);
		return false;
	}

	while (!feof(fp)) {
		fgets(buf, BUF_SIZE, fp);
		//fgets(buf, 512, fp);
		char chEnd = buf[strlen(buf) - 1];
		if (chEnd == '\n') {
			buf[strlen(buf) - 1] = '\0';
		}
		vec.push_back(al_ustr_new(buf));
		ALLEGRO_USTR *curUstr = vec.back();
		printf("strlen: %03zu, al_ustr_size: %03zu, al_ustr_length: %03zu\n", 
			strlen(buf), al_ustr_size(curUstr), al_ustr_length(curUstr));
	}
	fclose(fp);
	return true;
}

ALLEGRO_USTR* UstrVector::ustrFront() const {
	return vec.front();
}

ALLEGRO_USTR* UstrVector::ustrBack() const {
	return vec.back();
}

ALLEGRO_USTR* UstrVector::ustrAt(int idx) const {
	if (idx < 0 || idx >= (int)vec.size())
		return nullptr;
	return vec[idx];
}

size_t UstrVector::length() const {
	return vec.size();
}

//ALLEGRO_USTR* ustrAt(int idx) const;
//ALLEGRO_USTR* ustrFront() const;
//ALLEGRO_USTR* ustrBack() const;