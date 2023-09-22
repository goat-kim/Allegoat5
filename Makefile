CXX := g++
OBJS := main.o \
		Util.o \
		GameState.o \
		Sprite.o \
		SpriteAnimation.o \
		Scroller.o \
		Tilemap.o \
		GameObject.o \
		Player.o \
		Sound.o \
		UI.o \
		Npc1.o \

TARGET = allegoat
LIBS := allegro-5 allegro_acodec-5 allegro_audio-5 allegro_color-5 allegro_font-5 allegro_image-5 allegro_primitives-5 allegro_ttf-5
CXXFLAGS := -Wall `pkg-config --cflags --libs $(LIBS)`

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $^ -o $(TARGET) $(CXXFLAGS)

main.o: main.cpp
	$(CXX) $(CXXFLGAS) -c $<

# $@: rule에 있는 target의 이름 (즉, 출력 파일)
# $<: 첫 번째 dependency의 이름 (즉, 입력 파일 중 첫 번째)
# $^: 모든 dependency들의 이름 (즉, 입력 파일들)
# $*: 입력 파일에서 확장자를 떼어낸 파일 이름

Util.o: Util.cpp Util.h
	$(CXX) $(CXXFLAGS) -c $<

GameState.o: GameState.cpp GameState.h
	$(CXX) $(CXXFLAGS) -c $<

Sprite.o: Sprite.cpp Sprite.h
	$(CXX) $(CXXFLAGS) -c $<

SpriteAnimation.o: SpriteAnimation.cpp SpriteAnimation.h
	$(CXX) $(CXXFLAGS) -c $<

Scroller.o: Scroller.cpp Scroller.h
	$(CXX) $(CXXFLAGS) -c $<

Tilemap.o: Tilemap.cpp Tilemap.h
	$(CXX) $(CXXFLAGS) -c $<

GameObject.o: GameObject.cpp GameObject.h
	$(CXX) $(CXXFLAGS) -c $<

Sound.o: Sound.cpp Sound.h
	$(CXX) $(CXXFLAGS) -c $<

Player.o: Player.cpp Player.h
	$(CXX) $(CXXFLAGS) -c $<

UI.o: UI.cpp UI.h
	$(CXX) $(CXXFLAGS) -c $<

Npc1.o: Npc1.cpp Npc1.h
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o
	rm -f $(TARGET)

