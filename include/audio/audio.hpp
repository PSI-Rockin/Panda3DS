#pragma once
#include <functional>
#include <memory>
#include <vector>
#include "helpers.hpp"
#include "logger.hpp"
#include "memory.hpp"

namespace Audio {

class Frontend {
protected:
	Memory& mem;

	MAKE_LOG_FUNCTION(log, audioLogger)
public:
	enum class Type {
		Null,
		Lle
	};

	Frontend(Memory& mem) : mem(mem) {}

	virtual void reset() = 0;
	virtual void runAudioFrame() = 0;
	virtual u8* getDspMemory() = 0;

	virtual u16 recvData(u32 regId) = 0;
	virtual bool recvDataIsReady(u32 regId) = 0;
	virtual void setSemaphore(u16 value) = 0;
	virtual void writeProcessPipe(u32 channel, u32 size, u32 buffer) = 0;
	virtual std::vector<u8> readPipe(u32 channel, u32 peer, u32 size, u32 buffer) = 0;
	virtual void loadComponent(std::vector<u8>& data, u32 programMask, u32 dataMask) = 0;
	virtual void unloadComponent() = 0;
	virtual void setSemaphoreMask(u16 value) = 0;
};

bool initialize(Frontend::Type frontend, Memory& mem);
std::shared_ptr<Frontend> getFrontend();

}