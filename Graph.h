#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <time.h>
#include <cmath>
#include <cstdint>
#include <limits>
#include <string>
#include <stdint.h>
#include "BMP.h"
#include "Vector2d.h"


class Graph 
{
	int num_verteces_ = 0;
	int num_edges_ = 0;

	std::vector< std::pair<int, int> > edges_;

	Vector2d fRep(double edge_len, Vector2d v1, Vector2d v2);

	Vector2d fAttr(double edge_len, Vector2d v1, Vector2d v2);

	void AlgorithmFrick(int iterations, std::vector<Vector2d>& coordinates, double edge_len);

	void DrawEdges(std::vector<std::vector<uint8_t>>& vertex_map, std::vector<Vector2d>& coordinates);

	void AddCircle(Vector2d p, std::vector<std::vector<uint8_t>>& vertex_map, unsigned int image_w, unsigned int image_h);

	void AddNumber(int num, Vector2d p, std::vector<std::vector<uint8_t>>& vertex_map);

	void DrawVertices(std::vector<Vector2d>& coordinates, std::vector<std::vector<uint8_t>>& vertex_map, unsigned int image_w, unsigned int image_h);

	void SaveTo(std::string& filename, std::vector<std::vector<uint8_t>>& vertex_map);

public:
	explicit Graph(std::string path);

	std::vector<std::pair<int, int> > get_edges_() const { return edges_; }

	void Print(int image_h, int image_w, int iteration, std::string& filename);
};





