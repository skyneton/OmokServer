#pragma once
#include "Transform.h"

class RadiusCollider
{
public:
	RadiusCollider(Transform* transform, float r);
	~RadiusCollider();
	float r;
	Transform* transform;

	bool Intersected(RadiusCollider* col);
};

