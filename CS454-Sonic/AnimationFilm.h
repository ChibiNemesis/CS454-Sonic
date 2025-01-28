#pragma once
#include<string>
#include<vector>
#include"SDL.h"
#include<cassert>
#include"simpleclasses.h"
#include <sstream>
#include <fstream>
#include <filesystem>
#include <iostream>

class AnimationFilm {
	std::vector<SDL_Rect> boxes;
	SDL_Surface* bitmap = nullptr;
	std::string id;

	void readRects(std::string path) {
		int point[4] = { 0 };
		std::string w, h;
		std::ifstream input{ path };

		for (std::string line; std::getline(input, line);) {
			std::istringstream ss(std::move(line));
			int a = 0;
			for (std::string value; std::getline(ss, value, ' ');) {
				point[a] = std::stoi(value);
				a++;
			}
			SDL_Rect r;
			r.x = point[0];
			r.y = point[1];
			r.w = point[2];
			r.h = point[3];
			Append(r);
		}
	}

public:
	struct Data
	{
		std::string id;
		std::string path;
		std::vector<SDL_Rect> rects;
	};

	byte GetTotalFrames(void) const
	{
		return boxes.size();
	}
	SDL_Surface* GetBitmap(void) const { return bitmap; }
	auto GetId(void) const -> const std::string& { return id; }
	const SDL_Rect& GetFrameBox(byte frameNo) const
	{
		assert(boxes.size() > frameNo); 
		return boxes[(int)frameNo];
	}
	void DisplayFrame(SDL_Surface* dest, const Point& at, byte frameNo) const
	{
		SDL_Rect dst;
		dst.x = at.x;
		dst.y = at.y;
		SDL_BlitSurface(bitmap, &GetFrameBox(frameNo), dest, &dst);
	}
	void SetBitmap(SDL_Surface* b)
	{
		assert(!bitmap); bitmap = b;
	}
	void Append(const SDL_Rect& r) { boxes.push_back(r); }
	AnimationFilm(const std::string& _id) : id(_id) {}
	AnimationFilm(SDL_Surface* _s, const std::string rectspath, const std::string& _id) {
		bitmap = _s;
		readRects(rectspath);
		std::string copy = _id;
		id = copy;
	}
};