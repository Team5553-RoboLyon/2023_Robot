#ifndef __NUT_MATRIX_H
#define __NUT_MATRIX_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUT_Matrix.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
// Utilities for MATRIX 4x4 and 3x3
#include "../../NCStandard.h"
#include "../../NType.h"
#include "../../NMatrix.h"
#include "../../Maths/NMatrix3x3.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************************************************************************************
// ***************************************************************************************************************************************************************
// **																																							**
// **																			FUNCTIONS																		**
// **																																							**
// ***************************************************************************************************************************************************************
// ***************************************************************************************************************************************************************
void	NUT_ExtractCameraMatrixFromGLModelViewMatrix(NMATRIX *pmat);
void	NUT_ExtractCameraMatrix3x3FromGLModelViewMatrix(NMATRIX3x3 *pbase);

	
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NUT_MATRIX_H 
