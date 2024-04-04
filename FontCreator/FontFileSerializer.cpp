#include "FontFileSerializer.h"
#include "FontCreator.h"
#include <cmath>
#include <fstream>
#include <bitset>

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#ifndef LOG_MSG
#define LOG_MSG(message, ...) sprintf(s, message, __VA_ARGS__); BOOST_LOG_SEV(lg, info) << s;
#endif


namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
using namespace logging::trivial;
char s[128];
src::severity_logger<severity_level> lg;


FontFileSerializer* FontFileSerializer::getInstance() {
	if (instance == nullptr) {
		instance = new FontFileSerializer();
	}
	return instance;
}

Lines FontFileSerializer::downSizeLines(Lines& lines) {
	Lines retVal;
	wxPoint downscaledPoint;
	for (Line line : lines) {
		Line curLine;
		for (const wxPoint& point : line) {
			downscaledPoint = downScalePoint(point);
			curLine.push_back(downscaledPoint);
		}
		retVal.push_back(curLine);
	}
	return retVal;
}

wxPoint FontFileSerializer::downScalePoint(wxPoint point) {
	return wxPoint(normalizeQuotient(((double)point.x) / 4), normalizeQuotient((double)(point.y) / 4));
}

int FontFileSerializer::normalizeQuotient(double quotient) {
	double complement = ceil(quotient) - quotient;
	if (complement == 0.0) return (int)quotient;
	return complement >= 0.5 ? floor(quotient) : ceil(quotient);
}

void FontFileSerializer::cleanUpInstance() {
	if (instance == nullptr) return;
	delete instance;
}

void FontFileSerializer::loadFontFile(std::string fileName, std::unordered_map<char, Lines>& lines) {
	//Need some SERIOUS error checking here
	std::ifstream stream(fileName, std::ifstream::binary);
	if (!stream.good()) {
		wxLogMessage("Loaded in file was bad.");
		exit(1);
	}

	char curChar = 0;
	size_t linesSize = 0;
	size_t curLineSize = 0;
	int curX = 0;
	int curY = 0;

	//while stream...see https://softwareengineering.stackexchange.com/questions/318081/why-does-ifstream-eof-not-return-true-after-reading-the-last-line-of-a-file
	while (stream >> curChar) {
		//stream >> curChar; //Read this char
		readFromFileToIntegral(stream, &linesSize);

		Lines curLines(linesSize); //Initialize its Lines --> BUG HERE

		for (int i = 0; i < linesSize; ++i) {

			readFromFileToIntegral(stream, &curLineSize); //Within that Lines, read the size of this Line
			Line curLine(curLineSize); //Initialize

			for (int j = 0; j < curLineSize; ++j) {

				readFromFileToIntegral(stream, &curX);
				readFromFileToIntegral(stream, &curY);
				wxPoint curPoint(curX, curY);
				curLine[j] = curPoint;
				
			}
			curLines[i] = curLine;

		}
		lines[curChar] = curLines;
	}
}

void FontFileSerializer::saveFontFile(std::string fileName, const std::unordered_map<char, Lines>& charMapping) {
	fileName = "FontFiles/" + fileName + ".txt";

	std::ofstream stream(fileName, std::ofstream::binary);

	char curChar = 0;
	size_t linesSize = 0;
	size_t curLineSize = 0;
	int curX = 0;
	int curY = 0;

	for (const auto& pair : charMapping) {
		const char curChar = pair.first;
		const Lines& lines = pair.second;

		stream << curChar;
		linesSize = lines.size();
		writeIntegralToFile(stream, &linesSize);
		
		for (Line line : lines) {
			curLineSize = line.size();
			writeIntegralToFile(stream, &curLineSize);
			for (const wxPoint& point : line) {
				writeIntegralToFile(stream, &point.x);
				writeIntegralToFile(stream, &point.y);
			}
		}

	}

	wxLogMessage("File saved successfully!"); // I better have some error checking then
}

//IMPORTANT NOTE: I can get away with this being an unsigned, because wxPoint x and y are bounded by
//(0, 50).
void FontFileSerializer::writeIntegralToFile(std::ofstream& file, const unsigned int* integral) {
	file.write(reinterpret_cast<const char*>(integral), sizeof(integral));
}

void FontFileSerializer::writeIntegralToFile(std::ofstream& file, const int* integral) {
	file.write(reinterpret_cast<const char*>(integral), sizeof(integral));
}

void FontFileSerializer::readFromFileToIntegral(std::ifstream& file, unsigned int* integral) {
	file.read(reinterpret_cast<char*>(integral), sizeof(integral));
}
void FontFileSerializer::readFromFileToIntegral(std::ifstream& file, int* integral) {
	file.read(reinterpret_cast<char*>(integral), sizeof(integral));
}
	
FontFileSerializer* FontFileSerializer::instance = nullptr;