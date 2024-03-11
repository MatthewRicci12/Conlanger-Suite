#pragma once
#ifndef FONTFILESERIALIZER_H
#define FONTFILESERIALIZER_H
#include "FontCreator.h"

// D = dimension, A = area
#define CANVAS_D 200
#define CANVAS_A 40_000
#define LETTER_D 50
#define LETTER_A 2500

class FontFileSerializer
{
public:
	static FontFileSerializer* getInstance();
	Lines downSizeLines(Lines&);
	wxPoint downScalePoint(wxPoint point);
	int normalizeQuotient(double quotient);
	FontFileSerializer(FontFileSerializer& other) = delete;
	void operator=(const FontFileSerializer&) = delete;
private:
	static FontFileSerializer* instance;
	FontFileSerializer() {}
};

#endif