#pragma once

#include<string>

class TerrainNode {
public:
	int total;
	std::string path;
	TerrainNode* next;

	TerrainNode(std::string _path) {
		this->path = _path;
		this->total = 0;
		this->next = NULL;
	}
};