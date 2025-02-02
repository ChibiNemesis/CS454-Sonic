#include "GridLayer.h"

GridLayer::GridLayer(std::string path)
{
	/*
	totalColumns = totalRows = 0;
	for (auto i = 0; i < MAX; i++) {
		for (auto j = 0; j < MAX; j++) {
			map[i][j] = GRID_EMPTY_TILE;
		}
	}*/
	std::ifstream input{ path };

	for (std::string line; std::getline(input, line);) {
		unsigned point[4] = {0};
		int count = 0;
		std::istringstream ss(std::move(line));
		for (std::string line2; std::getline(ss, line2, ' ');) {
			point[count] = stoi(line2);
			count++;
		}
		BoundingBox box{ 
			point[0],
			point[1],
			point[0] + point[2],
			point[1] + point[3]
		};
		Colliders.push_back(box);
	}

	//To delete
	/*std::vector<std::vector<std::string>> csvRows;
	std::vector<std::vector<int>> csvRowsint;

	for (std::string line; std::getline(input, line);) {
		std::istringstream ss(std::move(line));
		std::vector<std::string> row;
		std::vector<int> rowint;
		if (!csvRows.empty()) {
			row.reserve(csvRows.front().size());
		}
		for (std::string value; std::getline(ss, value, ',');) {
			row.push_back(std::move(value));
		}
		csvRows.push_back(std::move(row));
	}

	for (auto row = 0; row < csvRows.size(); row++) {
		auto current = csvRows[row];
		csvRowsint.push_back(std::vector<int>());
		for (auto col = 0; col < current.size(); col++) {
			csvRowsint[row].push_back(std::stoi(current[col]));
			if (csvRowsint[row][col] == 1) {
				map[row][col] = GRID_SOLID_TILE;
			}
				
		}
	}*/
}

bool GridLayer::FilterMotionDown(const BoundingBox& r, int& dy)
{

	//std::cout << "X1: " << box->GetX1() << " Y1: " << box->GetY1() << " X2: " << box->GetX2() << " Y2 " << box->GetY2() << "\n";
	for (auto Collider : Colliders) {
		std::cout << "Col-- X1: " << Collider.GetX1() << " Y1: " << Collider.GetY1() << " X2: " << Collider.GetX2() << " Y2 " << Collider.GetY2() << "\n";
		if (r.Intersects(Collider)) { 
			std::cout << "Box:\n";
			std::cout << "--------------------------------\n";
			std::cout << "Intersects with: " << "\n";
			std::cout << "X1: " << Collider.GetX1() << " Y1: " << Collider.GetY1()<< "\n";
			std::cout << "X2: " << Collider.GetX2() << " Y2: " << Collider.GetY2() << "\n";
			std::cout << "#################################\n";
			dy = 0;
			//box->~BoundingBox();
			return true;
		}
	}
	return false;
}

bool GridLayer::FilterMotionUp(const SDL_Rect& r, int* dy)
{
	return false;
}

bool GridLayer::FilterMotionLeft(const SDL_Rect& r, int* dx)
{
	return false;
}

bool GridLayer::FilterMotionRight(const SDL_Rect& r, int* dx)
{

	return false;
}

bool GridLayer::IsOnSolidGround(const BoundingBox& r)
{
	int dy = 1; // down 1 pixel
	FilterMotionDown(r, dy);
	return dy == 0; // if true IS attached to solid ground
}
