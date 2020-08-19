#include "stdafx.h"
#include "RadiusCollider.h"


RadiusCollider::RadiusCollider(Transform* transform, float r) : transform(transform), r(r)
{
}


RadiusCollider::~RadiusCollider()
{
}

bool RadiusCollider::Intersected(RadiusCollider* other) {
	float myX = transform->position.x;
	float myY = transform->position.y;

	float otherX = other->transform->position.x;
	float otherY = other->transform->position.y;

	return pow(myX - otherX, 2) + pow(myY - otherY, 2) <= pow(other->r * other->transform->scale.x + r * transform->scale.x, 2);
}
