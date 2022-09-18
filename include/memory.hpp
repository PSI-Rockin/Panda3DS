#pragma once
#include <bitset>
#include <fstream>
#include <optional>
#include <vector>
#include "helpers.hpp"

namespace VirtualAddrs {
	enum : u32 {
		ExecutableStart = 0x00100000,
		MaxExeSize = 0x03F00000,
		ExecutableEnd = 0x00100000 + 0x03F00000,

		// Stack for main ARM11 thread.
		// Typically 0x4000 bytes, determined by exheader
		StackTop = 0x10000000,
		StackBottom = 0x0FFFC000,
		StackSize = 0x4000,

		NormalHeapStart = 0x08000000,
		LinearHeapStart = 0x14000000,

		// Start of TLS for first thread. Next thread's storage will be at TLSBase + 0x1000, and so on
		TLSBase = 0xFF400000,
		TLSSize = 0x1000
	};
}

// Types for svcQueryMemory
namespace KernelMemoryTypes {
	// This makes no sense
	enum MemoryState : u32 {
		Free = 0,
		Reserved = 1,
		IO = 2,
		Static = 3,
		Code = 4,
		Private = 5,
		Shared = 6,
		Continuous = 7,
		Aliased = 8,
		Alias = 9,
		AliasCode = 10,
		Locked = 11,

		PERMISSION_R = 1 << 0,
		PERMISSION_W = 1 << 1,
		PERMISSION_X = 1 << 2
	};
	
	// I assume this is referring to a single piece of allocated memory? If it's for pages, it makes no sense.
	// If it's for multiple allocations, it also makes no sense
	struct MemoryInfo {
		u32 baseVaddr; // Base process virtual address. TODO: What even is this
		u32 size;      // Of what?
		u32 perms;     // Is this referring to a single page or?
		u32 state;

		u32 end() { return baseVaddr + size; }
		MemoryInfo(u32 baseVaddr, u32 size, u32 perms, u32 state) : baseVaddr(baseVaddr), size(size)
			, perms(perms), state(state) {}
	};
}

class Memory {
	u8* fcram;

	// Our dynarmic core uses page tables for reads and writes with 4096 byte pages
	std::vector<uintptr_t> readTable, writeTable;
	std::vector<KernelMemoryTypes::MemoryInfo> memoryInfo;

	static constexpr u32 pageShift = 12;
	static constexpr u32 pageSize = 1 << pageShift;
	static constexpr u32 pageMask = pageSize - 1;
	static constexpr u32 totalPageCount = 1 << (32 - pageShift);
	
	static constexpr u32 FCRAM_SIZE = 128_MB;
	static constexpr u32 FCRAM_APPLICATION_SIZE = 64_MB;
	static constexpr u32 FCRAM_PAGE_COUNT = FCRAM_SIZE / pageSize;
	static constexpr u32 FCRAM_APPLICATION_PAGE_COUNT = FCRAM_APPLICATION_SIZE / pageSize;

	std::bitset<FCRAM_PAGE_COUNT> usedFCRAMPages;
	std::optional<u32> findPaddr(u32 size);

public:
	u32 usedUserMemory = 0;

	Memory();
	void reset();
	void* getReadPointer(u32 address);
	void* getWritePointer(u32 address);
	std::optional<u32> loadELF(std::ifstream& file);

	u8 read8(u32 vaddr);
	u16 read16(u32 vaddr);
	u32 read32(u32 vaddr);
	u64 read64(u32 vaddr);

	void write8(u32 vaddr, u8 value);
	void write16(u32 vaddr, u16 value);
	void write32(u32 vaddr, u32 value);
	void write64(u32 vaddr, u64 value);

	// Allocate "size" bytes of RAM starting from physical FCRAM address "paddr" (We pick it ourself if paddr == 0)
	// And map them to virtual address "vaddr" (We also pick it ourself if vaddr == 0).
	// If the "linear" flag is on, the paddr pages must be adjacent in FCRAM
	// r, w, x: Permissions for the allocated memory
	// Returns the vaddr the FCRAM was mapped to or nullopt if allocation failed
	std::optional<u32> allocateMemory(u32 vaddr, u32 paddr, u32 size, bool linear, bool r = true, bool w = true, bool x = true);

	// Returns whether "addr" is aligned to a page (4096 byte) boundary
	static constexpr bool isAligned(u32 addr) {
		return (addr & pageMask) == 0;
	}

	std::string readString(u32 vaddr, u32 maxCharacters);
	KernelMemoryTypes::MemoryInfo queryMemory(u32 vaddr);
};