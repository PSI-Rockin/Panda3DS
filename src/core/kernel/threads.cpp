#include "kernel.hpp"
#include "resource_limits.hpp"

// Internal OS function to spawn a thread
Handle Kernel::makeThread(u32 entrypoint, u32 initialSP, u32 priority, u32 id) {
	if (threadCount >= appResourceLimits.maxThreads) {
		Helpers::panic("Overflowed the number of threads");
	}
	threadCount++;

	// TODO: Actually create the thread
	return makeObject(KernelObjectType::Thread);
}

// Result CreateThread(s32 priority, ThreadFunc entrypoint, u32 arg, u32 stacktop, s32 threadPriority, s32 processorID)	
void Kernel::createThread() {
	u32 priority = regs[0];
	u32 entrypoint = regs[1];
	u32 initialSP = regs[3] & ~7; // SP is force-aligned to 8 bytes
	u32 id = regs[4];

	printf("CreateThread(entry = %08X, stacktop = %08X, priority = %X, processor ID = %d)\n", entrypoint,
		initialSP, priority, id);

	if (!(priority <= 0x3F)) [[unlikely]] {
		Helpers::panic("Created thread with bad priority value %X", priority);
		regs[0] = SVCResult::BadThreadPriority;
		return;
	}

	regs[0] = SVCResult::Success;
	regs[1] = makeThread(entrypoint, initialSP, priority, id);
}