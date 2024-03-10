#pragma once


// D = dimension, A = area
#define CANVAS_D 200
#define CANVAS_A CANVAS_D*CANVAS_D

class FontFileSerializer
{
public:
	FontFileSerializer* getInstance();
	FontFileSerializer(FontFileSerializer& other) = delete;
	void operator=(const FontFileSerializer&) = delete;
private:
	static FontFileSerializer* instance;
	FontFileSerializer() {}
};