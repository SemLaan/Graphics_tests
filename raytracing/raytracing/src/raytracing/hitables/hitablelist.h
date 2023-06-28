#pragma once
#include "hitable.h"

class HitableList : public Hitable {
public:
	Hitable** list;
	int listSize;

	HitableList() = default;
	HitableList(Hitable** l, int n) { list = l; listSize = n; }
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const;
};