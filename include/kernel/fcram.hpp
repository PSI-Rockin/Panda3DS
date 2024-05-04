#pragma once
#include <list>
#include <memory>
#include "kernel_types.hpp"

class Memory;

typedef std::list<FcramBlock> FcramBlockList;

class KFcram {
	struct Region {
		struct Block {
			s32 pages;
			s32 pageOffs;
			bool used;

			Block(s32 pages, u32 pageOffs) : pages(pages), pageOffs(pageOffs), used(false) {}
		};

		std::list<Block> blocks;
		u32 start;
		s32 pages;
		s32 freePages;
	public:
		Region() : start(0), pages(0) {}
		void reset(u32 start, size_t size);
		void alloc(std::list<FcramBlock>& out, s32 pages, bool linear);

		u32 getUsedCount();
		u32 getFreeCount();
	};

	Memory& mem;

	Region appRegion, sysRegion, baseRegion;
	uint8_t* fcram;
	std::unique_ptr<u32> refs;
public:
	KFcram(Memory& memory);
	void reset(size_t ramSize, size_t appSize, size_t sysSize, size_t baseSize);
	void alloc(FcramBlockList& out, s32 pages, FcramRegion region, bool linear);

	void incRef(FcramBlockList& list);
	void decRef(FcramBlockList& list);

	u32 getUsedCount(FcramRegion region);
};