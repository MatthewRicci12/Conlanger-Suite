#include "FontFileSerializer.h"
#include "FontCreator.h"
#include <cmath>
#include <fstream>

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
//char s[128];
//src::severity_logger<severity_level> lg;


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
	//LOG_MSG("Quotient: %lf, ceiling: %lf complement: %lf\n", quotient, ceil(quotient), complement);
	if (complement == 0.0) return (int)quotient;
	return complement >= 0.5 ? floor(quotient) : ceil(quotient);
}

void FontFileSerializer::cleanUpInstance() {
	if (instance == nullptr) return;
	delete instance;
}

void FontFileSerializer::saveFontFile(std::string fileName, const std::unordered_map<char, Lines>& charMapping) {
	fileName = "FontFiles/" + fileName + ".txt";

	std::ofstream stream(fileName);

	for (const auto& pair : charMapping) {
		const char& curChar = pair.first;
		const Lines& lines = pair.second;

		stream << curChar;
		stream << lines.size();
		
		for (Line line : lines) {
			stream << line.size();
			for (const wxPoint& point : line) {
				stream << point.x;
				stream << point.y;
			}
		}

	}

	wxLogMessage("File saved successfully!"); // I better have some error checking then
}
	
FontFileSerializer* FontFileSerializer::instance = nullptr;