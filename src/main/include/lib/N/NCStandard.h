#ifndef __NCSTANDARD_H
#define __NCSTANDARD_H

#include "NCompilerPragma.h"
#include "lib/N/NCompilerPragma.h"

#ifdef _NWINDOWS
// standard libraries for WINDOWS
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#endif

#ifdef _NIOS
// standard libraries for IOS
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <ctype.h>
#include <wchar.h>
#include <math.h>
#endif

#ifdef _NANDROID
// standard libraries for Android
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <ctype.h>
#include <wchar.h>
#include <math.h>
#endif

#include "NMemory.h"

/*
#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
*/
#endif // __NCSTANDARD_H
