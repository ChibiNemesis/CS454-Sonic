#pragma once
#include<list>
#include"BoundingArea.h"
#include<functional>
#include"Sprite.h"
#include<cassert>
#include"SDL.h"

class CollisionChecker final {
public:
	using Action = std::function<void(Sprite* s1, Sprite* s2)>;
	//static CollisionChecker singleton;
protected:
	using Entry = std::tuple<Sprite*, Sprite*, Action>;
	std::list<Entry> entries;
	auto Find(Sprite* s1, Sprite* s2) -> std::list<Entry>::iterator;
public:
	void Register(Sprite* s1, Sprite* s2, const Action& f);
	void Cancel(Sprite* s1, Sprite* s2);
	void Check(void) const;
	//static auto GetSingleton(void) -> CollisionChecker&;
	//static auto GetSingletonConst(void) -> const CollisionChecker&;
};

