#pragma once
#include"simpleclasses.h"
#include"MotionQuantizer.h"
#include"BoundingArea.h"
#include <functional>
#include<cassert>
#include<string>
#include"Clipper.h"
#include"GravityHandler.h"
#include"AnimationFilm.h"
#include"GridLayer.h"

class Sprite {
public:
	using Mover = std::function<void(const SDL_Rect&, int* dx, int* dy)>;
protected:
	byte frameNo = 0;
	SDL_Rect frameBox; // inside the film
	int x = 0, y = 0;
	bool isVisible = false;
	AnimationFilm* currFilm = nullptr;
	BoundingArea* boundingArea = nullptr;
	unsigned zorder = 0;
	std::string typeId, stateId;
	Mover mover;

	int StaticWidth;
	int StaticHeight;
	//MotionQuantizer quantizer;
public:
	bool directMotion = false;
	GravityHandler gravity;

	template <typename Tfunc>
	void SetMover(const Tfunc& f)
	{
		//quantizer.SetMover(mover = f);
	}
	const SDL_Rect GetBox(void) const
	{
		return { x, y, frameBox.w, frameBox.h };
	}
	Sprite& Move(int dx, int dy) {
		if (directMotion) // apply unconditionally offsets!
			x += dx, y += dy;
		else {
			//quantizer.Move(GetBox(), &dx, &dy);
			gravity.Check(GetBox());
		}
		if (x < 0) {
			x = 0;
		}
		return *this;
	}

	void SetPos(int _x, int _y) { x = _x; y = _y; }
	void SetZorder(unsigned z) { zorder = z; }
	unsigned GetZorder(void) { return zorder; }

	void SetFrame(byte i) {
		if (i != frameNo) {
			assert(i < currFilm->GetTotalFrames());
			frameBox = currFilm->GetFrameBox(frameNo = i);
		}
	}
	byte GetFrame(void) const { return frameNo; }
	void SetBoundingArea(const BoundingArea& area)
	{
		assert(!boundingArea); boundingArea = area.Clone();
	}
	void SetBoundingArea(BoundingArea* area)
	{
		assert(!boundingArea); boundingArea = area;
	}
	auto GetBoundingArea(void) const -> const BoundingArea*
	{
		return boundingArea;
	}
	auto GetTypeId(void) -> const std::string& { return typeId; }
	void SetVisibility(bool v) { isVisible = v; }
	bool IsVisible(void) const { return isVisible; }
	bool CollisionCheck(const Sprite* s) const;
	void Display(SDL_Surface& dest, const SDL_Rect& dpyArea, const Clipper& clipper) const;
	void Display(SDL_Surface& dest, const SDL_Rect& dpyArea) const;
	Sprite(int _x, int _y, AnimationFilm* film, const std::string& _typeId = "") :
		x(_x), y(_y), currFilm(film), typeId(_typeId)
	{
		frameNo = currFilm->GetTotalFrames(); 
		SetFrame(0);

	}

	Sprite(){
		directMotion = false;
	}

	GravityHandler& GetGravityHandler(void)
	{
		return gravity;
	}
	Sprite& SetHasDirectMotion(bool v) { directMotion = v; return *this; }
	bool GetHasDirectMotion(void) const { return directMotion; }

	void SetAnimationFilm(AnimationFilm* _film) {
		currFilm = _film;
		frameNo = currFilm->GetTotalFrames(); SetFrame(0);
	}

	void SetStaticHeight(int _h);
	void SetStaticWidth(int _w);
	int getStaticHeight();
	int getStaticWidth();
};

//grid layer
/*
void PrepareSpriteGravityHandler(GridLayer* gridLayer, Sprite* sprite) {
	sprite->GetGravityHandler().SetOnSolidGround(
		[gridLayer](const Rect& r)
		{ return gridLayer->IsOnSolidGround(r); }
	);
}*/
//sprite->SetHasDirectMotion(true).Move(dx, dy).SetHasDirectMotion(false);

