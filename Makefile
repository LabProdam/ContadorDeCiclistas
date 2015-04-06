OBJ=CycloTracker.o \
	VideoOutput.o \
	ImageProcessor.o \
	ObjectCounter.o \
	ObjectLocator.o \
	ObjectTracker.o \
	PointTracker.o \
	TrackedObject.o \
	Sensors.o \
	Camera.o \
	CoordTransform.o \
	Utils.o

	CFLAGS=-Wall \
	   -Wextra \
	   -O2

all:bin/CycloTracker

clean:
	rm -f bin/*
	rm -f $(OBJ)

bin/CycloTracker: $(OBJ)
	@mkdir -p bin
	@mkdir -p tmp
	g++ $^ -o $@ `pkg-config --libs opencv` --std=c++11 -g

%.o: %.cpp
	g++ -c $< -o $@ --std=c++11 -pthread -g
