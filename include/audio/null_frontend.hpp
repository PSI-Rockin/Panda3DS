#pragma once
#include <array>
#include "audio/audio.hpp"

namespace Audio {

class NullFrontend : public Frontend {
	enum class DSPState : u32 {
		Off, On, Slep
	};

	// Number of DSP pipes
	static constexpr size_t pipeCount = 8;
	DSPState dspState;

	std::array<std::vector<u8>, pipeCount> pipeData; // The data of each pipe

	void resetAudioPipe();
public:
	NullFrontend(Memory& mem) : Frontend(mem) {}

	void reset() override;

	u16 recvData(u32 regId) override;
	bool recvDataIsReady(u32 regId) override;
	void setSemaphore(u16 value) override;
	void writeProcessPipe(u32 channel, u32 size, u32 buffer) override;
	std::vector<u8> readPipe(u32 channel, u32 peer, u32 size, u32 buffer) override;
	void loadComponent(u32 size, u32 programMask, u32 dataMask, u32 buffer) override;
	void unloadComponent() override;
	void setSemaphoreMask(u16 value) override;
};

}