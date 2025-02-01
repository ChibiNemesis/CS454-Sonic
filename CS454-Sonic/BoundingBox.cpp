#include "BoundingBox.h"

BoundingBox::BoundingBox(unsigned _x1, unsigned _y1, unsigned _x2, unsigned _y2)
{
    x1 = _x1;
    x2 = _x2;
    y1 = _y1;
    y2 = _y2;
}

BoundingBox::BoundingBox()
{
    x1 = x2 = y1 = y2 = 0;
}

bool BoundingBox::Intersects(const BoundingBox& box) const
{
    return !(
        box.x2 < x1 || //at left
        x2 < box.x1 || //at right
        box.y2 < y1 || // above
        y2 < box.y1    //below
        );
}

bool BoundingBox::In(unsigned x, unsigned y) const
{
    return x1 <= x && x <= x2 && y1 <= y && y <= y2;
}

bool BoundingBox::Intersects(const BoundingArea& area) const
{
    return area.Intersects(*this);
}

BoundingArea* BoundingBox::Clone(void) const
{
    return new BoundingBox(x1, y1, x2, y2);
}