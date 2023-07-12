#include "audio/audio.hpp"
#include "audio/lle_frontend.hpp"
#include "audio/null_frontend.hpp"

namespace Audio {

static std::shared_ptr<Frontend> frontend = nullptr;

bool initialize(Frontend::Type type, Memory& mem) {
	switch (type) {
	case Frontend::Type::Null:
		frontend = std::make_shared<NullFrontend>(mem);
		break;
	case Frontend::Type::Lle:
		frontend = std::make_shared<LleFrontend>(mem);
		break;
	default:
		return false;
	}

	mem.setDSPMem(frontend->getDspMemory());

	return true;
}

std::shared_ptr<Frontend> getFrontend() {
	if (!frontend) {
		Helpers::panic("Audio: frontend not initialized");
	}

	return frontend;
}

}