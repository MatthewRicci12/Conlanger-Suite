#pragma once
#include <bitset>
#include <unordered_map>

// D = dimension, A = area
#define CANVAS_D 200
#define CANVAS_A CANVAS_D*CANVAS_D

class FontFileSerializer
{
public:

private:
	std::unordered_map<char, std::bitset<CANVAS_A>> map;
};