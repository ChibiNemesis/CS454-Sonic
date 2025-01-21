#pragma once
#include"SDL.h"
#include<map>
#include<string>
#include<cassert>
#include <SDL_image.h>

class BitmapLoader {
private:
	using Bitmaps = std::map<std::string, SDL_Surface*>;
	Bitmaps bitmaps;
	SDL_Surface* GetBitmap(const std::string& path) const {
		auto i = bitmaps.find(path);
		return i != bitmaps.end() ? i->second : nullptr;
	}
public:
	SDL_Surface* Load(const std::string& path) {
		auto b = GetBitmap(path);
		if (!b) {
			bitmaps[path] = b = IMG_Load(path.c_str()); //Load bitmap and add to map
			assert(b);
		}
		return b;
	}
	// prefer to massively clear bitmaps at the end than
	// to destroy individual bitmaps during gameplay
	void CleanUp(void) {
		for (auto& i : bitmaps)
			SDL_FreeSurface(i.second);
		bitmaps.clear();
	}
	BitmapLoader(void) {}
	~BitmapLoader() { CleanUp(); }
};