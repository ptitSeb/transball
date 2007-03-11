#ifdef _MSC_VER
#pragma warning(disable: 4514) // Unreferenced inline function
#endif
#ifdef __INTEL_COMPILER
#pragma warning(disable: 171) // Invalid type conversion remark
#endif

#ifdef KITSCHY_DEBUG_MEMORY

#include "debug_memorymanager.h"
#include <cassert>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

// Copyright (c) Juha Hiekkamäki

// Remove overloading
#ifdef new
#undef new
#endif

/*
 * C-style code. Can't really help it since using new/delete inside
 * allocation routines wouldn't be very wise. C-runtime used for same
 * reason.
 */
namespace {

#ifdef _MSC_VER
	typedef unsigned __int32 uint32;
#else
	// This should be changed if portability is a concern.
	typedef unsigned int uint32;
#endif

	// Default filename where to dump leak report.
	char logFileName[1024] = "memory_leaks.txt";

	// Identifiers which are placed to allocated buffer
	const uint32 memPrefix = 0xBAADF00D;
	const uint32 memPostfix = 0xBABE2BED;
	const uint32 memInitialValue = 0xDEADC0DE;

	// Safe boundaries. Be carefull, can be a memory overkill
	const int numberPrefix = 8; // 32 bytes
	const int numberPostfix = 16; // 64 bytes

	struct AllocationUnit
	{
		// For convenience
		uint32 *prefixPointer;
		uint32 *postfixPointer;
		uint32 *dataPointer;

		// Size w/ and w/o manager extras
		size_t requestedSize;
		size_t overallSize;
	
		// Catches mixing new[]/delete and new/delete[]
		bool arrayAllocated;

		// Allocation info which may or may not be present
		char *allocatedFrom;
	};

	// 'Constructor'
	AllocationUnit *createAllocationUnit()
	{
		AllocationUnit *unit = static_cast<AllocationUnit *> (malloc(sizeof(AllocationUnit)));
		
		unit->prefixPointer = 0;
		unit->postfixPointer = 0;
		unit->dataPointer = 0;
	
		unit->requestedSize = 0;
		unit->overallSize = 0;

		unit->arrayAllocated = false;
		unit->allocatedFrom = 0;
		
		return unit;
	}

	// 'Destructor'
	void deleteAllocationUnit(AllocationUnit *unit)
	{
		if(unit->allocatedFrom)
			free(unit->allocatedFrom);
		if(unit->prefixPointer)
			free(unit->prefixPointer);
		free(unit);
	}

	/* 
	 * Here are our allocation infos. 
	 * Implemented with simple open hashing.
	 */

	struct AllocationLink
	{
		AllocationUnit *allocationUnit;
		AllocationLink *next;
	};

	struct AllocationRoot
	{
		AllocationLink *first;

		AllocationRoot()
		:	first(0) 
		{
		}
	};

	// Hash data
	static const int hashSize = 3677; // Should be big enough?
	static AllocationRoot hashMap[hashSize];

	// Statistics
	struct Statistics
	{
		static int allocationCount;
		static int allocationMemoryUsage;
		static int allocationPeakCount;
		static int allocationPeakMemoryUsage;

		static void addAllocation(AllocationUnit *allocation)
		{
			++allocationCount;
			allocationMemoryUsage += allocation->requestedSize;

			// Update peaks?
			if(allocationCount > allocationPeakCount)
				allocationPeakCount = allocationCount;
			if(allocationMemoryUsage > allocationPeakMemoryUsage)
				allocationPeakMemoryUsage = allocationMemoryUsage;
		}

		static void removeAllocation(AllocationUnit *allocation)
		{
			--allocationCount;
			allocationMemoryUsage -= allocation->requestedSize;
		}
	};

	int Statistics::allocationCount = 0;
	int Statistics::allocationMemoryUsage = 0;
	int Statistics::allocationPeakCount = 0;
	int Statistics::allocationPeakMemoryUsage = 0;

	// Hash value from pointer
	int calculateHashValue(const void *buffer)
	{
		// Invalid conversion actually but it works in here. 
		// We just need some value for hashing ;-)
		int value = reinterpret_cast<int> (buffer);
		value >>= 4;

		// Create index
		value %= hashSize;
		return value;
	}

	// Add allocation to table
	void addAllocation(AllocationUnit *allocation)
	{
		Statistics::addAllocation(allocation);

		AllocationLink *link = static_cast<AllocationLink *> (malloc(sizeof(AllocationLink)));
		link->allocationUnit = allocation;
		link->next = 0;

		int hashIndex = calculateHashValue(allocation->dataPointer);
		if(hashMap[hashIndex].first == 0)
			hashMap[hashIndex].first = link;
		else
		{
			// Push front
			link->next = hashMap[hashIndex].first;
			hashMap[hashIndex].first = link;
		}
	}

	// Find allocation from table
	AllocationUnit *findAllocation(const void *pointer)
	{
		int hashIndex = calculateHashValue(pointer);
		AllocationLink *current = hashMap[hashIndex].first;

		while(current)
		{
			if(current->allocationUnit->dataPointer == pointer)
				return current->allocationUnit;

			current = current->next;
		}

		assert(!"Allocation not found. Uninitialized or already deleted pointer.");
		return 0;
	}

	// Remove allocation from table
	void removeAllocation(AllocationUnit *allocation)
	{
		assert(Statistics::allocationCount > 0);
		int hashIndex = calculateHashValue(allocation->dataPointer);

		AllocationLink *current = hashMap[hashIndex].first;
		AllocationLink *previous = 0;

		while(current)
		{
			if(current->allocationUnit == allocation)
			{
				// Remove
				if(previous)
					previous->next = current->next;
				else
					hashMap[hashIndex].first = current->next;

				Statistics::removeAllocation(current->allocationUnit);

				// Free memory
				deleteAllocationUnit(current->allocationUnit);
				free(current);
		
				return;
			}

			previous = current;
			current = current->next;
		}

		assert(!"Allocation not found. Unintialized or already deleter pointer.");
	}

	void dumpLeakReport()
	{
		FILE *fp = fopen(logFileName, "wt");
		if(fp == 0)
		{
			assert(!"Can't open leak file.");
			return;
		}

		// Statistics
		fprintf(fp, "Peak memory allocations: %d\n", Statistics::allocationPeakCount);
		fprintf(fp, "Peak memory usage: %d bytes\n\n", Statistics::allocationPeakMemoryUsage);

		if(Statistics::allocationCount > 0)
		{
			// Leaks
			fprintf(fp, "Overall memory leaked: %d bytes\n", Statistics::allocationMemoryUsage);
			fprintf(fp, "Pointers left: %d\n\n", Statistics::allocationCount);

			int currentIndex = 0;
			for(int i = 0; i < hashSize; ++i)
			{
				AllocationLink *currentLink = hashMap[i].first;
				while(currentLink != 0)
				{
					fprintf(fp, "Allocation %d:\n", ++currentIndex);
					fprintf(fp, "\tAllocated from: %s\n", currentLink->allocationUnit->allocatedFrom);
					fprintf(fp, "\tAllocation size: %u bytes\n", currentLink->allocationUnit->requestedSize);
					if(currentLink->allocationUnit->arrayAllocated == false)
						fprintf(fp, "\tAllocated with new()\n");
					else
						fprintf(fp, "\tAllocated with new[]\n");
				
					fprintf(fp, "\n");
					currentLink = currentLink->next;
				}
			}

			// Manager bugs if this fails.
			assert(currentIndex == Statistics::allocationCount);
		}
		else
		{
			fprintf(fp, "No leaks found.");
		}

		fclose(fp);	
	}

	void testIdentifiers(AllocationUnit *allocation)
	{
		for(int i = 0; i < numberPrefix; ++i)
		{
			if(allocation->prefixPointer[i] != memPrefix)
			{
				assert(!"Buffer prefix messed up. You have wrote out-of-bounds.");
				break;
			}
		}
		for(int j = 0; j < numberPostfix; ++j)
		{
			if(allocation->postfixPointer[j] != memPostfix)
			{
				assert(!"Buffer postfix messed up. You have wrote out-of-bounds.");
				break;
			}
		}
	}

	// After deinitialization, dump leak report on every deallocation. Only reasonable 
	// way of doing this since there can be memory allocated on other static instances.
	struct InitializationTracker
	{
		static bool programExiting;

		InitializationTracker()
		{
		}
		~InitializationTracker()
		{	
			programExiting = true; 
			dumpLeakReport();
		}
	};
	
	bool InitializationTracker::programExiting = false;
	static InitializationTracker tracker;

} // end of unnamed namespace

// operator new implementation as suggested by Meyers on Effective C++ (item 8)
void *operator new(size_t originalSize, const char *fileName, int lineNumber, bool arrayAllocated)
{
	// Handle 0-byte request. The Holy Standard says we must return unique pointer 
	// (or unique value actually, this is just simpler)
	if(originalSize == 0)
		originalSize = 1;

	// To 4-byte boundary (since our identifiers are uint32`s)
	if(originalSize % 4)
		originalSize += 4 - originalSize % 4;

	// Make room for prefix & postfix
	size_t size = originalSize;
	size += numberPrefix * 4;
	size += numberPostfix * 4;

	// Yes. Infinite loop(tm) really is the way to go.
	for(;;)
	{
		AllocationUnit *allocation = createAllocationUnit();
		void *buffer = malloc(size);
		
		// Both have to succeed. We want to handle out-of-memory properly at least
		// on our mighty memory manager ;-)
		if((buffer) && (allocation))
		{
			char *info = static_cast<char *> (malloc(strlen(fileName) + 20));
			if(info)
				sprintf(info, "(%s: line %d)", fileName, lineNumber);

			// Fill in allocation info
			allocation->prefixPointer = static_cast<uint32 *> (buffer);
			allocation->dataPointer = allocation->prefixPointer + numberPrefix;
			allocation->postfixPointer = allocation->dataPointer + (originalSize/4);
			
			allocation->allocatedFrom = info;
			allocation->arrayAllocated = arrayAllocated;
			allocation->overallSize = size;
			allocation->requestedSize = originalSize;

			// Fill in our identifiers
			for(int i = 0; i < numberPrefix; ++i)
				allocation->prefixPointer[i] = memPrefix;
			for(int j = 0; j < int(originalSize / 4); ++j)
				allocation->dataPointer[j] = memInitialValue;
			for(int k = 0; k < numberPostfix; ++k)
				allocation->postfixPointer[k] = memPostfix;

			addAllocation(allocation);
			return allocation->dataPointer;
		}

		// If only one of them succeeded, free it first
		if(buffer)
			free(buffer);
		if(allocation)
			deleteAllocationUnit(allocation);

		// Test error-handling function. Some compilers (MSC, ...) 
		// declare these incorrectly to global scope (w/o newer headers)
		using namespace std;
		new_handler global_handler = set_new_handler(0);
		set_new_handler(global_handler);

		// If has one, try it. Otherwise throw bad_alloc 
		// (and hope for someone to catch it)
		if(global_handler)
			(*global_handler) ();
		else
			throw std::bad_alloc();
	}
}

void operator delete(void *buffer, bool arrayDeleted) throw()
{
	// Deleting null-pointer is legal
	if(buffer == 0)
		return;

	AllocationUnit *allocation = findAllocation(buffer);
	assert(allocation);

	// Test boundaries
	testIdentifiers(allocation);

	// Test array operator mixing
	if(allocation->arrayAllocated == arrayDeleted)
	{
		removeAllocation(allocation);

		// If quitting, dump leak report on each deallocation
		if(InitializationTracker::programExiting == true)
			dumpLeakReport();
	}
	else
	{
		assert(!"Mixed arrayed and normal versions of new/delete");
	}
}


/*
  These get called from client code
*/
void *operator new(size_t size, const char *fileName, int lineNumber) throw(std::bad_alloc)
{
	return operator new(size, fileName, lineNumber, false);
}
void *operator new(size_t size) throw(std::bad_alloc)
{
	return operator new(size, "???", 0, false);
}
void *operator new[](size_t size, const char *fileName, int lineNumber) throw(std::bad_alloc)
{
	return operator new(size, fileName, lineNumber, true);
}
void *operator new[](size_t size) throw(std::bad_alloc)
{
	return operator new(size, "???", 0, true);
}

void operator delete(void *buffer) throw()
{
	operator delete(buffer, false);
}
void operator delete[](void *buffer) throw()
{
	operator delete(buffer, true);
}

/*
  Utility functions
*/

namespace kitschy {
namespace debug {

// Pointers 
/*
void MemoryManager::validatePointer(const void *pointer)
{
	// Try to find
	AllocationUnit *allocation = findAllocation(pointer);

	// Test out-of-bounds
	if(allocation)
		testIdentifiers(allocation);
	else
	{
		assert(!"Allocation not found. Uninitialized or already deleted pointer.");
	}
} 
*/

// Logging
void MemoryManager::logStatisticsTo(const char *fileName)
{
	assert(fileName);
	assert(strlen(fileName) + 1 < sizeof(logFileName));

	strcpy(logFileName, fileName);
}

// Memory statistics
int MemoryManager::amountMemoryInUse()
{
	return Statistics::allocationMemoryUsage;
}

int MemoryManager::amountPeakMemoryInUse()
{
	return Statistics::allocationPeakMemoryUsage;
}

int MemoryManager::amountMemoryAllocations()
{
	return Statistics::allocationCount;
}

int MemoryManager::amountPeakMemoryAllocations()
{
	return Statistics::allocationPeakCount;
}

} // end of namespace debug
} // end of namespace kitschy

#endif // KITSCHY_DEBUG_MEMORY
