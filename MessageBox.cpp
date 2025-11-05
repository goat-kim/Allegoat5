#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "UI.h"
// #include "MessageBox.h"
// -> 굳이 헤더 파일을 나눌 필요는 없을 듯?

DialogBox::DialogBox() {
#ifdef DEBUG
	printf("MessageBox()\n");
#endif
}

DialogBox::~DialogBox() {
#ifdef DEBUG
	printf("~MessageBox()\n");
#endif
}

void DialogBox::update() {
	//printf("MessageBox::update()\n");
}

