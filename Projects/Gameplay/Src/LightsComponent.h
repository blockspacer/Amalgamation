#pragma once
#include <Component.h>

#include <LightInstanceData.h>
#include <vector>
#include <utility>

using namespace std;

struct AglMatrix;
struct AglVector3;
struct AglQuaternion;

struct Light
{
	//AglVector3 scale;
	//AglQuaternion rotation;
	//AglVector3 translation;
	AglMatrix offset;
	LightInstanceData instanceData;
	Light();
};

class LightsComponent : public Component
{
public:
	LightsComponent();
	virtual ~LightsComponent();

	vector<Light>* getLightsPtr();
	void addLight( Light p_light );

private:
	vector<Light> m_lights;
};

