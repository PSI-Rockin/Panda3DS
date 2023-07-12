#pragma once
#include "teakra/teakra.h"
#include "audio/audio.hpp"

namespace Audio {

class LleFrontend : public Frontend {
	Teakra::Teakra teakra;
	bool running;
public:
	LleFrontend(Memory& mem);

	void reset() override;
	void runAudioFrame() override;
	u8* getDspMemory() override { return teakra.GetDspMemory().data(); }

	u16 recvData(u32 regId) override;
	bool recvDataIsReady(u32 regId) override;
	void setSemaphore(u16 value) override;
	void writeProcessPipe(u32 channel, u32 size, u32 buffer) override;
	std::vector<u8> readPipe(u32 channel, u32 peer, u32 size, u32 buffer) override;
	void loadComponent(std::vector<u8>& data, u32 programMask, u32 dataMask) override;
	void unloadComponent() override;
	void setSemaphoreMask(u16 value) override;
};

}