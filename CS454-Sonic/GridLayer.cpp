#include "GridLayer.h"

GridLayer::GridLayer(std::string path)
{
	totalColumns = totalRows = 0;
	for (auto i = 0; i < MAX; i++) {
		for (auto j = 0; j < MAX; j++) {
			map[i][j] = GRID_EMPTY_TILE;
		}
	}
	std::ifstream input{ path };

	//initialize the set that contains the solid tiles
	for (std::string line; std::getline(input, line);) {
		solidtiles.insert(stoi(line));
	}

}

void GridLayer::SetGridMap(Tilemap* m)
{
	int rows = m->getHeight();
	int cols = m->getWidth();
	for (auto i = 0; i < rows; i++) {
		for (auto j = 0; j < cols; j++) {
			if (solidtiles.find(m->GetTile(i, j)) != solidtiles.end()) {
				SetSolidGridTile(j, i);
			}
			else {
				SetEmptyGridTile(j, i);
			}
		}
	}
}