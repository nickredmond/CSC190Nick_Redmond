#ifndef AUTOPROFILE_H
#define AUTOPROFILE_H

#include "Timer.h"
#include "Profiler.h"

class AutoProfile{

#ifdef PROFILING_ENABLED

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
#else

public:
	AutoProfile(const char* _name){_name;}
	~AutoProfile(){}

#endif
};

#define PROFILE(name) AutoProfile p(name)

#endif