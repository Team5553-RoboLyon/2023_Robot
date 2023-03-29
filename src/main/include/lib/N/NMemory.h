#pragma once
/*
#ifndef __NMEMORY_H
#define __NMEMORY_H
*/
// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NMemory.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"

/*
#ifdef __cplusplus
extern "C"
{
#endif
*/
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

//#ifdef _DEBUG
    /*
    #ifdef _NWINDOWS
    #include <crtdbg.h>
    // !!! WARNING !!!
    // !!! Don't use these  functions USE the #define just below !!!  
    void*	Nmalloc_debug(Nu32 size,const char *pfname,int nbline);
    void*	Nrealloc_debug(void* ptr,Nu32 newsize,const char *pfname,int nbline);
    void	Nfree_debug(void *ptr);
    void	NMemoryStats_Debug();
    void	NMemoryCheck_Debug(const void *ptr,const Nu32 memory_size, const Nu8 check_u8 );
    //

    #define Nmalloc(size)							Nmalloc_debug(size,__FILE__,__LINE__)
    #define Nmalloc_mem0(size)						memset(Nmalloc_debug(size,__FILE__,__LINE__),0,size)
    #define Nrealloc(ptr,newsize)					Nrealloc_debug(ptr,newsize,__FILE__,__LINE__)
    #define Nfree(ptr)								if(ptr){Nfree_debug(ptr);}
    #define Nfree_NULL(ptr)							if(ptr){Nfree_debug(ptr);(ptr) = NULL;}
    #define NMemoryStats()							NMemoryStats_Debug()
    #define NMemoryCheck(ptr,memory_size,check_u8)	NMemoryCheck_Debug(ptr, memory_size, check_u8 )

    #define Nmem0(ptr,type)							memset((ptr),0,sizeof(type))
    #endif
    */

    /*
    #ifdef _NANDROID
    #define Nmalloc(size)							malloc(size)
    #define Nmalloc_mem0(size)						memset(malloc(size),0,size)
    #define Nrealloc(ptr,newsize)					realloc(ptr,newsize)
    #define Nfree(ptr)								if(ptr){free(ptr);}
    #define Nfree_NULL(ptr)							if(ptr){free(ptr);(ptr) = NULL;}
    #define NMemoryStats()
    #define NMemoryCheck(ptr,memory_size,check_u8)
    #define Nmem0(ptr,type)							memset((ptr),0,sizeof(type))
    #endif
    */
//#endif

//#ifndef _DEBUG
#define Nmalloc(size)							malloc(size)
#define Nmalloc_mem0(size)						memset(malloc(size),0,size)
#define Nrealloc(ptr,newsize)					realloc(ptr,newsize)
#define Nfree(ptr)								if(ptr){free(ptr);}
#define Nfree_NULL(ptr)							if(ptr){free(ptr);(ptr) = NULL;}
#define NMemoryStats()	
#define NMemoryCheck(ptr,memory_size,check_u8)
#define Nmem0(ptr,type)							memset((ptr),0,sizeof(type))
//#endif


#define NEW(base)	( (base*)Nmalloc(sizeof(base)) )

/*
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NMEMORY_H 
*/