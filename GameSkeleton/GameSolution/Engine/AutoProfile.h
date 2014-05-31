#ifndef AUTOPROFILE_H
#define AUTOPROFILE_H

#include "Timer.h"
#include "Profiler.h"

class AutoProfile{
private:
	const char* name;
	Timer time;
public:
	AutoProfile(const char* _name){
		name = _name;
		time = Timer();
		time.Start();
	}
	~AutoProfile(){
		float elapsed = time.Stop();
		profiler.AddEntry(name, elapsed);
	}
};

#define PROFILE(name) AutoProfile p(name)

#endif