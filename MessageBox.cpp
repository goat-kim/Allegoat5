#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "UI.h"
// #include "MessageBox.h"
// -> 굳이 헤더 파일을 나눌 필요는 없을 듯?

DialogBox::DialogBox() {
	printf("MessageBox()\n");
}

DialogBox::~DialogBox() {
	printf("~MessageBox()\n");
}

void DialogBox::update() {
	//printf("MessageBox::update()\n");
}

