#ifdef RELEASE
#pragma once
#include "../../../Utils/Utils.h"
/*
	https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/tier0/memalloc.h
	Structures aren't checked, if using, refer to your favorite disassembler.
	Needed for certain things when messing with the source engine (example: rebuilding anim stuff and allocating mem with
	win32 functions will cause an unexpected crash.)
*/

struct _CrtMemState;

#define MEMALLOC_VERSION 1

typedef size_t(*MemAllocFailHandler_t) (size_t);

//-----------------------------------------------------------------------------
// NOTE! This should never be called directly from leaf code
// Just use new,delete,malloc,free etc. They will call into this eventually
//-----------------------------------------------------------------------------
template <typename FuncType>
__forceinline static FuncType call_virtual(void* ppClass, int index)
{
	int* pVTable = *(int**)ppClass; //-V206
	int dwAddress = pVTable[index]; //-V108
	return (FuncType)(dwAddress);
}
class MemAlloc
{
public:
	auto Alloc(int nSize) -> void* {
		using Fn = void* (__thiscall*)(void*, int);
		return call_virtual< Fn >(this, 1)(this, nSize);
	}

	auto Realloc(void* pMem, int nSize) -> void* {
		using Fn = void* (__thiscall*)(void*, void*, int);
		return call_virtual< Fn >(this, 3)(this, pMem, nSize);
	}

	auto Free(void* pMem) -> void {
		using Fn = void(__thiscall*)(void*, void*);
		return call_virtual< Fn >(this, 5)(this, pMem);
	}
}; extern MemAlloc* g_pMemAlloc;
#endif