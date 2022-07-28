CXXFLAGS=-Wall -Wextra -O3 -std=c++17

cube: AudioContext.o AudioBuffer.o AudioSource.o Serial.o main.o
	$(CXX) -o $@ $^ `pkg-config --cflags --libs openal sndfile` $(CXXFLAGS)

debug: CFLAGS += -g
debug: AudioContext.o AudioBuffer.o AudioSource.o Serial.o main.o
	$(CXX) -o $@ $^ `pkg-config --cflags --libs openal sndfile` $(CXXFLAGS)

clean:
	rm -f *.o cube
