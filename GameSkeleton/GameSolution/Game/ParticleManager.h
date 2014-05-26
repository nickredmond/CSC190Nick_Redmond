#include <vector>
#include "ParticleEffect.h"
#include "UpdateFunctions.h"

using std::vector;

class ParticleManager{
private:
	vector<ParticleEffect*> particleEffects;
public:
	void AddEffect(ParticleEffect* effect){
		particleEffects.push_back(effect);
	}
	void RemoveEffect(int index){
		vector<ParticleEffect*>::iterator removedEff = particleEffects.begin() + index;
		particleEffects.erase(removedEff);
	}
	void RemoveEffect(ParticleEffect* effect){
		bool found = false;

		unsigned int index = 0;
		vector<ParticleEffect*>::iterator iter = particleEffects.begin(); 
		while(iter != particleEffects.end() && !found && (index < particleEffects.size())){
			if (effect == *iter){
				iter = particleEffects.erase(iter);
				found = true;
			}
			else iter = particleEffects.begin() + index;

			index++;
		}
	}

	void Update(bool isThrusting, float angle, Vector2 pos, float dt){
		vector<ParticleEffect*> deletedEffects;

		for(vector<ParticleEffect*>::iterator iter = particleEffects.begin(); 
			iter != particleEffects.end(); iter++){
				ParticleEffect* nextEffect = *iter;
				UpdateColors(nextEffect->GetColorChangeType(), nextEffect->GetParticles(), nextEffect->GetNrParticles());
				bool isAlive = nextEffect->Update(isThrusting, angle, pos, dt);

				if (!isAlive){
					deletedEffects.push_back(nextEffect);
				}
		}

		for(vector<ParticleEffect*>::iterator iter = deletedEffects.begin(); 
			iter != deletedEffects.end(); iter++){
				ParticleEffect* nextEffect = *iter;
				RemoveEffect(nextEffect);
		}
	}
	void Draw(Core::Graphics& graphics){
		for(vector<ParticleEffect*>::iterator iter = particleEffects.begin(); 
			iter != particleEffects.end(); iter++){
				ParticleEffect* nextEffect = *iter;
				nextEffect->Draw(graphics);
		}
	}
};