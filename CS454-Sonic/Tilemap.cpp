#include "Tilemap.h"

Tilemap::Tilemap()
{
    for (auto x = 0; x < MAX; x++)
        for (auto y = 0; y < MAX; y++)
            map[x][y] = 0;

    this->height = 0;
    this->width = 0;
}

Tilemap::Tilemap(int w, int h)
{
    for (auto x = 0; x < MAX; x++)
        for (auto y = 0; y < MAX; y++)
            map[x][y] = 0;

    this->height = h;
    this->width = w;
}

void Tilemap::SetDimensions(int _width, int _height)
{
    this->height = _height;
    this->width = _width;
}

int Tilemap::getWidth()
{
    return this->width;
}

int Tilemap::getHeight()
{
    return this->height;
}

void Tilemap::SetTile(Dim row, Dim col, Index index)
{
    map[row][col] = index;
}

Index Tilemap::GetTile(Dim row, Dim col)
{
    return map[row][col];
}

bool Tilemap::ReadTextMap(std::string filename)
{
    //Format:
    //Width
    //Height
    //csv-values

    std::ifstream input{ filename + "\\Tilemap.txt" };

    //std::cout << std::filesystem::current_path() << std::endl;

    if (!input) {
        std::cout << "File not found" << std::endl;
        return false;
    }

    std::vector<std::vector<std::string>> csvRows;
    std::vector<std::vector<int>> csvRowsint;

    std::string height, width;
    std::getline(input, width);
    std::getline(input, height);

    this->width = std::stoi(width);
    this->height = std::stoi(height);

    //std::cout << this->width << ", " << this->height << std::endl;

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
            //map[row][col] = csvRowsint[row][col];
            map[row][col] = csvRowsint[row][col];
        }
    }

    return true;
}
