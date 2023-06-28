#include "hitablelist.h"

bool HitableList::Hit(const Ray& r, float t_min, float t_max, HitRecord& record) const {
	HitRecord tempRecord;
	bool hitAnything = false;
	float closestSoFar = t_max;
	for (int i = 0; i < listSize; i++) {
		if (list[i]->Hit(r, t_min, closestSoFar, tempRecord)) {
			hitAnything = true;
			closestSoFar = tempRecord.t;
			record = tempRecord;
		}
	}
	return hitAnything;
}