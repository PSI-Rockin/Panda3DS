#include "audio/audio.hpp"
#include "audio/null_frontend.hpp"

namespace Audio {

static std::shared_ptr<Frontend> frontend = nullptr;

bool initialize(Frontend::Type type, Memory& mem) {
	switch (type) {
	case Frontend::Type::Null:
		frontend = std::make_shared<NullFrontend>(mem);
		break;
	default:
		return false;
	}

	return true;
}

std::shared_ptr<Frontend> getFrontend() {
	if (!frontend) {
		Helpers::panic("Audio: frontend not initialized");
	}

	return frontend;
}

}