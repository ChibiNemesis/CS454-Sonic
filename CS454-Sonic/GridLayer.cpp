#include "GridLayer.h"

GridLayer::GridLayer(std::string path)
{
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
}

bool GridLayer::FilterMotionDown(const BoundingBox& r, int& dy)
{
	for (auto Collider : Colliders) {
		if (r.Intersects(Collider)) { 
			std::cout << "Box:\n";
			std::cout << "--------------------------------\n";
			std::cout << "Intersects Below" << "\n";
			std::cout << "#################################\n";
			dy = 0;
			return true;
		}
	}
	return false;
}

bool GridLayer::FilterMotionUp(const BoundingBox& r, int* dy)
{
	for (auto Collider : Colliders) {
		if (r.Intersects(Collider)) {
			std::cout << "Box:\n";
			std::cout << "--------------------------------\n";
			std::cout << "Intersects Up" << "\n";
			std::cout << "#################################\n";
			dy = 0;
			return true;
		}
	}
	return false;
}

bool GridLayer::FilterMotionLeft(const BoundingBox& r, int* dx)
{
	for (auto Collider : Colliders) {
		if (r.Intersects(Collider)) {
			std::cout << "Box:\n";
			std::cout << "--------------------------------\n";
			std::cout << "Intersects Left" << "\n";
			std::cout << "#################################\n";
			dx = 0;
			return true;
		}
	}
	return false;
}

bool GridLayer::FilterMotionRight(const BoundingBox& r, int* dx)
{

	for (auto Collider : Colliders) {
		if (r.Intersects(Collider)) {
			std::cout << "Box:\n";
			std::cout << "--------------------------------\n";
			std::cout << "Intersects Right" << "\n";
			std::cout << "#################################\n";
			dx = 0;
			return true;
		}
	}
	return false;
}

bool GridLayer::IsOnSolidGround(const BoundingBox& r)
{
	int dy = 1; // down 1 pixel
	FilterMotionDown(r, dy);
	return dy == 0; // if true IS attached to solid ground
}
