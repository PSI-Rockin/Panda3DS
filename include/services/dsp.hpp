#pragma once
#include <array>
#include <optional>
#include "helpers.hpp"
#include "logger.hpp"
#include "memory.hpp"
#include "result/result.hpp"

// Circular dependencies!
class Kernel;

class DSPService {
	Handle handle = KernelHandles::DSP;
	Memory& mem;
	Kernel& kernel;
	MAKE_LOG_FUNCTION(log, dspServiceLogger)

	// DSP service event handles
	using DSPEvent = std::optional<Handle>;

	// Number of DSP pipes
	static constexpr size_t pipeCount = 8;

	DSPEvent semaphoreEvent;
	DSPEvent interrupt0;
	DSPEvent interrupt1;
	std::array<DSPEvent, pipeCount> pipeEvents;

	DSPEvent& getEventRef(u32 type, u32 pipe);
	static constexpr size_t maxEventCount = 6;

	// Total number of DSP service events registered with registerInterruptEvents
	size_t totalEventCount;

	// Service functions
	void convertProcessAddressFromDspDram(u32 messagePointer); // Nice function name
	void flushDataCache(u32 messagePointer);
	void getHeadphoneStatus(u32 messagePointer);
	void getSemaphoreEventHandle(u32 messagePointer);
	void invalidateDCache(u32 messagePointer);
	void loadComponent(u32 messagePointer);
	void readPipeIfPossible(u32 messagePointer);
	void recvData(u32 messagePointer);
	void recvDataIsReady(u32 messagePointer);
	void registerInterruptEvents(u32 messagePointer);
	void setSemaphore(u32 messagePointer);
	void setSemaphoreMask(u32 messagePointer);
	void unloadComponent(u32 messagePointer);
	void writeProcessPipe(u32 messagePointer);

public:
	DSPService(Memory& mem, Kernel& kernel) : mem(mem), kernel(kernel) {}
	void reset();
	void handleSyncRequest(u32 messagePointer);

	enum class SoundOutputMode : u8 {
		Mono = 0,
		Stereo = 1,
		Surround = 2
	};

	void signalEvents();
};