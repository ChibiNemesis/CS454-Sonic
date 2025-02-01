#pragma once
#include "BoundingArea.h"

class BoundingBox :
	public BoundingArea
{
protected:
	unsigned x1, y1, x2, y2;
public:
	BoundingBox(unsigned _x1, unsigned _y1, unsigned _x2, unsigned _y2);
	BoundingBox();

	bool Intersects(const BoundingBox& box) const;
	//virtual bool Intersects(const BoundingCircle& circle);
	//virtual bool Intersects(const BoundingPolygon& polygon) const;

	bool In(unsigned x, unsigned y) const;
	bool Intersects(const BoundingArea& area) const;

	BoundingArea* Clone(void) const;
	~BoundingBox() {};
};

