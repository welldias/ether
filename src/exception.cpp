#include "ether.h"

namespace ether {

	Exception::Exception()
		: m_errcode(0) {
	}

	NotFound::NotFound(const std::string& objName)
		: Exception(0, std::string("Object \"" + objName + "\" is not found in the dictionary.")) {
	}

	AlreadyExists::AlreadyExists(const std::string& objName)
		: Exception(0, std::string("Object \"" + objName + "\" already exists in the dictionary.")) {
	}
}