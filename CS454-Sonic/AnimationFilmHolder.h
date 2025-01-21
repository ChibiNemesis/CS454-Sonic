#pragma once
#include<functional>
#include"AnimationFilm.h"
#include<map>
#include"BitmapLoader.h"
#include<list>

class AnimationFilmHolder final {
public:
	using Parser = std::function<
		bool(std::list<AnimationFilm::Data>& output, const std::string& input)>;
	/*using EntryParser = std::function<
		int(// -1=error, 0=ended gracefully, else #chars read
			int startPos,
			const std::string& input,
			std::string& idOutput,
			std::string& pathOutput,
			std::vector<SDL_Rect>& rectsOutput
			)
	>;*/
private:
	using Films = std::map<std::string, AnimationFilm*>;
	Films films;
	BitmapLoader bitmaps; // only for loading of film bitmaps
	static AnimationFilmHolder holder; // singleton
	AnimationFilmHolder(void) {}
	~AnimationFilmHolder() { CleanUp(); }
	static auto Get(void) -> const AnimationFilmHolder& { return holder; }
	//void Load(const std::string& text, const EntryParser& entryParser);
	//void Load(const std::string& text, const Parser& parser);
	void CleanUp(void);
	auto GetFilm(const std::string& id) -> const AnimationFilm* const;
};

