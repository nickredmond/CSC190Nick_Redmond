#ifndef AUTOPROFILE_H
#define AUTOPROFILE_H

#include "Timer.h"
#include "Profiler.h"

class AutoProfile{
private:
	const char* name;
	Timer time;
	Profiler profiler;
public:
	AutoProfile(const char* _name, Profiler& _profiler){
		name = _name;
		profiler = _profiler;
		time = Timer();
		time.Start();
	}
	~AutoProfile(){
		float elapsed = time.Stop();
		profiler.AddEntry(name, elapsed);
	}
};

#define PROFILE(name) "AutoProfile p = AutoProfile(" ## name ## ", " ## #profiler ## ")"

#endif