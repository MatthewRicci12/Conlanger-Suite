#include "FontFileSerializer.h"

FontFileSerializer* FontFileSerializer::getInstance() {
	if (instance == nullptr) {
		instance = new FontFileSerializer();
	}
	return instance;
}

FontFileSerializer* FontFileSerializer::instance = nullptr;