#include "FontFileSerializer.h"
#include "FontCreator.h"
#include <cmath>

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
	LOG_MSG("=====================BEFORE=====================\n");
	for (Line line : lines) {
		for (wxPoint point : line) {
			LOG_MSG("(%d, %d)\n", point.x, point.y);
		}
	}
	LOG_MSG("=====================AFTER=====================\n");

	Lines retVal;
	wxPoint downscaledPoint;
	for (Line line : lines) {
		Line curLine;
		for (const wxPoint& point : line) {
			downscaledPoint = downScalePoint(point);
			curLine.push_back(downscaledPoint);
			LOG_MSG("(%d, %d)\n", downscaledPoint.x, downscaledPoint.y);
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

FontFileSerializer* FontFileSerializer::instance = nullptr;