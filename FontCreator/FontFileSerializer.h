#pragma once
#ifndef FONTFILESERIALIZER_H
#define FONTFILESERIALIZER_H
#include "FontCreator.h"

// D = dimension, A = area
#define CANVAS_D 200
#define CANVAS_A 40_000
#define LETTER_D 100
#define LETTER_A 2500

class FontFileSerializer
{
public:
	static FontFileSerializer* getInstance();
	static void cleanUpInstance();
	Lines downSizeLines(Lines&);
	wxPoint downScalePoint(wxPoint);
	int normalizeQuotient(double);
	FontFileSerializer(FontFileSerializer&) = delete;
	void operator=(const FontFileSerializer&) = delete;
	void submitGlyphToCurrentFile(char, Lines&);
	void saveFontFile();

private:
	static FontFileSerializer* instance;
	static std::ofstream curFileStream;
	static short fontFileNum;
	FontFileSerializer() {}
};

#endif