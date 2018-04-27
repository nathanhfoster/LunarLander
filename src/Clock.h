#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <iostream>
#include "ofApp.h"

class Clock {
public:
	uint64_t startTimeSecs, startTimeMillis, startTimeMicro;

	Clock() : startTimeMillis(ofGetElapsedTimeMillis()), 
			  startTimeMicro(ofGetElapsedTimeMicros()), 
		      startTimeSecs(ofGetElapsedTimef()) { }

	uint64_t getTimeSec() { return ofGetElapsedTimef() - startTimeSecs; }
	uint64_t getTimeMillis() { return ofGetElapsedTimeMillis() - startTimeMillis; }
	uint64_t getTimeMicro() { return ofGetElapsedTimeMicros() - startTimeMicro; }
	void printTimes() { cout << "Elasped time in" << endl 
		<< "Seconds: " << getTimeSec () << endl
		<< "Milliseconds: " << getTimeMillis () << endl
		<< "Microseconds: "<< getTimeMicro() << endl; }
};
#endif