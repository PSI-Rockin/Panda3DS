#include "audio/lle_frontend.hpp"

namespace Audio {

LleFrontend::LleFrontend(Memory& mem) : Frontend(mem), running(false) {
	teakra.Reset();

	//Set up callbacks for Teakra
	Teakra::AHBMCallback ahbm;

	ahbm.read8 = [&](u32 addr) -> u8 {
		return mem.read8(addr);
	};

	ahbm.read16 = [&](u32 addr) -> u16 {
		return mem.read16(addr);
	};

	ahbm.read32 = [&](u32 addr) -> u32 {
		return mem.read32(addr);
	};

	ahbm.write8 = [&](u32 addr, u8 value) {
		mem.write8(addr, value);
	};

	ahbm.write16 = [&](u32 addr, u16 value) {
		mem.write16(addr, value);
	};

	ahbm.write32 = [&](u32 addr, u32 value) {
		mem.write32(addr, value);
	};

	teakra.SetAHBMCallback(ahbm);
	teakra.SetAudioCallback([=](std::array<s16, 2> sample) {
		//NOP for now
	});
}

void LleFrontend::reset() {
	teakra.Reset();
	running = false;
}

void LleFrontend::runAudioFrame() {
	if (running) {
		teakra.Run(16384);
	}
}

u16 LleFrontend::recvData(u32 regId) {
	return teakra.RecvData(regId);
}

bool LleFrontend::recvDataIsReady(u32 regId) {
	return teakra.RecvDataIsReady(regId);
}

void LleFrontend::setSemaphore(u16 value) {
	teakra.SetSemaphore(value);
}

void LleFrontend::writeProcessPipe(u32 channel, u32 size, u32 buffer) {
	//TODO
}

std::vector<u8> LleFrontend::readPipe(u32 channel, u32 peer, u32 size, u32 buffer) {
	//TODO
	return std::vector<u8>();
}

void LleFrontend::loadComponent(std::vector<u8>& data, u32 programMask, u32 dataMask) {
	//TODO
}

void LleFrontend::unloadComponent() {
	if (!running) {
		Helpers::panic("Audio: unloadComponent called without a running program");
	}

	//Wait for SEND2 to be ready, then send the shutdown command to the DSP
	while (!teakra.SendDataIsEmpty(2)) {
		runAudioFrame();
	}

	teakra.SendData(2, 0x8000);

	//Wait for shutdown to be acknowledged
	while (!teakra.RecvDataIsReady(2)) {
		runAudioFrame();
	}

	//Read the value and discard it, completing shutdown
	teakra.RecvData(2);

	running = false;
}

void LleFrontend::setSemaphoreMask(u16 value) {
	teakra.MaskSemaphore(value);
}

}