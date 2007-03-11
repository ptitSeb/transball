#ifndef INCLUDED_KITSCHY_DEBUG_MEMORYMANAGER_H
#define INCLUDED_KITSCHY_DEBUG_MEMORYMANAGER_H

// Copyright (c) Juha Hiekkamäki. 

// Define this to enable memory tracking.
#ifdef KITSCHY_DEBUG_MEMORY

namespace kitschy {
namespace debug {

/**
 * Debug versions of C++ heap memory handling operators.
 * Debug versions of C++ heap memory handling operators. Use is 
 * transparent to client code. Note that this system might increase 
 * memory footprint considerably if you are allocating lots of small chunks.
 * 
 * This header doesn`t have to be included from client code. Operators
 * will be overrided anyway (on link time), this just allows us to
 * store source file / line number to leak log.
 * 
 * This does the following:
 * 	- Keeps list of all occured allocations
 * 		- Reports if forgot to delete some block
 * 		- Reports if tried to delete uninitialized block
 * 	- Keeps some space around allocated buffer
 * 		- Detects most out-of-bounds writes
 * 		- Checking done only on deallocation, can`t tell where it happened.
 * 	- Notices if one tries to delete() new[]`ed memory or vice versa
 * 		- Mixing array new/delete to normal versions, that is 
 * 		- At least on MSVC/Intel, requires including this header thought
 * 		- GCC works fine without it. Surprise, surprise
 * 	- Ability to query some statistics
 * 
 * Started coding this on my own, but later on found Paul Nettle's similar system. 
 * Some ideas from there. http://www.fluidstudios.com/publications.html might be 
 * worth a look. Small article of the subject can be found on GPGems 2.
 * 
 * It's nice knowing what smart pointers (std::auto_ptr<> and friends)
 * are for. Whole package of them can be found on boost, http://www.boost.org/ . As 
 * Stroustrup puts it, "resource acquisition is initialization". Templatized wrappers
 * around arrays (with assert()'s for operator[] indexing etc) is very powerfull technique
 * for catching out-of-bounds accesses (STL-port with debug-mode is nice too). Still, you can 
 * never be too sure ...
 * 
 * @note Following compilers have been somewhat tested.
 *  - GCC (2.95.3+): Works
 *	- MS Visual C++ (6): Works.
 *	- Intel C++ (6): Works.
 *	- MingW (Dev-C++ default, GCC variant): Works.
 *  - Watcom C++ (11): Does not work w/o STLport.
 *  - Others should too, doesn't use any too fancy language features ;-).
 *
 * @note Some 'leaks' might come from external libraries, such as from STLport. These
 *       aren't actual bugs, it's just the way things have been implemented. If you have 
 *       #included this header to every source file, you can just ignore all leaks coming
 *       from unknown sources (assuming there aren't that many of them).
 *
 * Safer coding.
 *
 * @todo Better queries (certain amount of buffer unused or such).
 * @todo Option to store freed pointers (?).
 * @todo Option to break on given pointer deallocation (?).
**/
class MemoryManager
{
	/**
	 * Not implemented, contains only static methods.
	 * Not implemented, contains only static methods.
	 */
	MemoryManager();
	~MemoryManager();

public:
	/**
	 * Tests whether pointer is valid.
	 * Tests whether pointer is valid. Works only in debug mode. Breaks onto assert
	 * if pointer has never been allocated or data has been written out-of-bounds.
	 * 
	 * @param pointer Pointer to test.
	 * @note Use with care. It seems that most compilers append some
	 */
	//static void validatePointer(const void *pointer);
		// Does not work reliably for user types. Uncomment if you wish but be 
		// carefull with it. Compilers seem to move returned pointer few bytes
		// forward. That, of course, disables ability for this to work.
		// Maybe some compiler specific hack^H^H^H^H analysis would help ...

	/**
	 * Specify filename where to dump leak report.
	 * Specify filename where to dump leak report when program is quitting.
	 * Default file is "memory_leaks.txt" on working directory.
	 *
	 * @param fileName Filename where to dump report.
	 */
	static void logStatisticsTo(const char *fileName);

	/**
	 * Returns amount of memory in use.
	 * Returns amount of memory in use. Does not include manager extras.
	 */
	static int amountMemoryInUse();
	/**
	 * Returns peak amount of memory which was in use.
	 * Returns peak amount of memory which was in use. Does not include manager extras.
	 */
	static int amountPeakMemoryInUse();
	/**
	 * Returns amount of memory allocations stored.
	 * Returns amount of memory allocations stored.
	 */
	static int amountMemoryAllocations();
	/**
	 * Returns peak amount of memory allocations.
	 * Returns peak amount of memory allocations.
	 */
	static int amountPeakMemoryAllocations();
};

} // end of namespace debug
} // end of namespace kitschy

#ifdef _MSC_VER
#if _MSC_VER <= 1200
#pragma warning(disable : 4290) // Exception specification ignored. (Not properly suppported)
#endif
#endif

#ifndef INCLUDED_NEW
#define INCLUDED_NEW
#include <new>
#endif

/**
 * Global new-operator with extra information.
 * Global new-operator with extra information. Handles actual memory allocation.
 */
void *operator new(size_t size, const char *fileName, int lineNumber) throw(std::bad_alloc);
/**
 * Global new-operator.
 * Global new-operator. Handles actual memory allocation.
 */
void *operator new(size_t size) throw(std::bad_alloc);
/**
 * Global arrayed new-operator with extra information.
 * Global arrayed new-operator with extra information. Handles actual memory allocation.
 */
void *operator new[](size_t size, const char *fileName, int lineNumber)  throw(std::bad_alloc);
/**
 * Global arrayed new-operator.
 * Global arrayed new-operator. Handles actual memory allocation.
 */
void *operator new[](size_t size) throw(std::bad_alloc);
/**
 * Global delete-operator.
 * Global delete-operator. Frees memory.
 */
void operator delete(void *buffer) throw();
/**
 * Global arrayed delete-operator.
 * Global arrayed delete-operator. Frees memory.
 */
void operator delete[](void *buffer) throw();

// Just to play safe.
#ifndef __FILE__
#define __FILE__ "???"
#endif
#ifndef __LINE__
#define __LINE__ 0
#endif

/**
 * Macro which allows us to add info which is passed to new-operators.
 * Macro which allows us to add info which is passed to new-operators.
 *
 * Eg "int *foo = new int()" expands to 
 * "int *foo = new (__FILE__,__LINE__) int()". __FILE__ and __LINE__ are those
 * additional parameters for operators.
 */
#define KITSCHY_DEBUG_NEW new (__FILE__, __LINE__)
#define new KITSCHY_DEBUG_NEW

#endif // kitschy_debug_memory
#endif
