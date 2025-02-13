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

bool BoundingBox::IntersectsAbove(const BoundingBox& box)
{
    return !(
        box.x2 < x1 || //at left
        x2 < box.x1 || //at right
        box.y2 < y1 // above
        );
}

bool BoundingBox::IntersectsBelow(const BoundingBox& box)
{
    return !(
        box.x2 < x1 || //at left
        x2 < box.x1 || //at right
        y2 < box.y1    //below
        );
}

bool BoundingBox::IntersectsLeft(const BoundingBox& box) const
{
    return !(
        box.x1 < x2 && //at left
        box.y2 < y1 //|| // above
        //y2 < box.y1    //below
        );
}

bool BoundingBox::IntersectsRight(const BoundingBox& box) const
{
    return !(
        x1 < box.x2 && //at right
        box.y2 < y1 //|| // above
        //y2 < box.y1    //below
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

int BoundingBox::GetX1()
{
    return x1;
}

int BoundingBox::GetY1()
{
    return y1;
}

int BoundingBox::GetX2()
{
    return x2;
}

int BoundingBox::GetY2()
{
    return y2;
}

BoundingArea* BoundingBox::Clone(void) const
{
    return new BoundingBox(x1, y1, x2, y2);
}