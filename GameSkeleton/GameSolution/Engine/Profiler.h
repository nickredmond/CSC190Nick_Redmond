#ifndef PROFILER_H
#define PROFILER_H

const int DEFAULT_MAX_CAT = 15;
const int DEFAULT_MAX_SMPL = 100;

enum AddEntryResult { SUCCESS, CATEGORY_NOT_FOUND, NO_SAMPLES_LEFT };

class _declspec(dllexport) Profiler{
private:
	const char* fileName;
	int _maxSamples, _maxCategories, categoryIndex;

	struct ProfileCategory{
		const char* name;
		float samples[DEFAULT_MAX_SMPL];
		int sampleIndex;
	} categories[DEFAULT_MAX_CAT];

	Profiler(int maxSamples, int maxCategories){
		_maxSamples = maxSamples;
		_maxCategories = maxCategories;
		categoryIndex = 0;
	}
	Profiler(){
		_maxSamples = DEFAULT_MAX_SMPL;
		_maxCategories = DEFAULT_MAX_CAT;
		categoryIndex = 0;
	}

	Profiler p;
public:
	static Profiler GetInstance(){
		return p;
	}

	void RunProfile(){}
	bool AddCategory(const char* categoryName){
		bool success = false;

		if (categoryIndex < DEFAULT_MAX_CAT){
			ProfileCategory category = ProfileCategory();
			category.name = categoryName;
			category.sampleIndex = 0;

			categories[categoryIndex] = category;
			categoryIndex++;

			success = true;
		}

		return success;
	}
	AddEntryResult AddEntry(const char* category, float time){
		bool foundCategory = false;
		AddEntryResult result = AddEntryResult::SUCCESS;

		for (int i = 0; i < categoryIndex && !foundCategory; i++){
			const char* nextName = categories[i].name;
			foundCategory = (std::strcmp(category, nextName) == 0);

			if (foundCategory){
				int sIndex = categories[i].sampleIndex;

				if (sIndex < DEFAULT_MAX_SMPL){
					categories[i].samples[sIndex] = time;
					categories[i].sampleIndex++;
				}
				else result = AddEntryResult::NO_SAMPLES_LEFT;
			}
		}

		if (!foundCategory){
			result = AddEntryResult::CATEGORY_NOT_FOUND;
		}

		return result;
	}
};

#endif