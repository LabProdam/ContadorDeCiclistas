all:bin/CycloTracker

clean:
	rm -f bin/*
	rm -f *.o

bin/CycloTracker: CycloTracker.cpp VideoOutput.o ImageProcessor.o ObjectCounter.o ObjectLocator.o ObjectTracker.o PointTracker.o TrackedObject.o Sensors.o
	g++ $^ -o $@ `pkg-config --libs opencv` --std=c++11

VideoOutput.o: VideoOutput.cpp VideoOutput.hpp
	g++ $< -c -o $@ --std=c++11

ImageProcessor.o: ImageProcessor.cpp ImageProcessor.hpp 
	g++ $< -c -o $@ --std=c++11

ObjectCounter.o: ObjectCounter.cpp ObjectCounter.hpp TrackedObject.hpp
	g++ $< -c -o $@ --std=c++11

ObjectLocator.o: ObjectLocator.cpp ObjectCounter.hpp
	g++ $< -c -o $@ --std=c++11

ObjectTracker.o: ObjectTracker.cpp ObjectTracker.hpp TrackedObject.hpp ObjectCounter.hpp ObjectLocator.hpp PointTracker.hpp
	g++ $< -c -o $@ --std=c++11

PointTracker.o: PointTracker.cpp PointTracker.hpp TrackedObject.hpp
	g++ $< -c -o $@ --std=c++11

TrackedObject.o: TrackedObject.cpp TrackedObject.hpp
	g++ $< -c -o $@ --std=c++11

Sensors.o: Sensors.cpp Sensors.hpp
	g++ $< -c -o $@ --std=c++11 -pthread
