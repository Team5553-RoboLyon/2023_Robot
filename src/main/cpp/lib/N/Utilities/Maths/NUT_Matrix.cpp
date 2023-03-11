// ==========================================================================================
// NUT_Matrix.cpp
// ------------------------------------------------------------------------------------------
// Author	: Jean-Marie Nazaret
// Create	: 07/05/2012
// Modified : 
// ==========================================================================================
#include "lib/N/GL/Ngl.h"
#include "../../NErrorHandling.h"
#include "NUT_MathsMisc.h"
#include "NUT_Matrix.h"



// --------------------------------------------------------------------------- ---------------
// NUT_ExtractCameraMatrixFromGLModelViewMatrix
// ------------------------------------------------------------------------------------------
// Description :
//	Setup a Matrix(4x4) from the current GL_MODELVIEW_MATRIX.
//	Because it's a view matrix we need to transpose the source matrix into the camera matrix.
// ------------------------------------------------------------------------------------------
// In  :
//		NMATRIX			*pmat				"THE" Matrix to setup from the gl data
// Out :
//
// ------------------------------------------------------------------------------------------
void NUT_ExtractCameraMatrixFromGLModelViewMatrix(NMATRIX *pmat)
{
	GLfloat modelview[16];

	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	pmat->Side.x		=  modelview[0];
	pmat->Side.y		=  modelview[4];
	pmat->Side.z		=  modelview[8];

	pmat->Up.x			=  modelview[1];
	pmat->Up.y			=  modelview[5];
	pmat->Up.z			=  modelview[9];

	pmat->Forward.x		= -modelview[2];
	pmat->Forward.y		= -modelview[6];
	pmat->Forward.z		= -modelview[10];

	pmat->Translation.x = -modelview[12];
	pmat->Translation.y = -modelview[13];
	pmat->Translation.z = -modelview[14];

	// all the w
	pmat->f[3]			= 0.0f; // modelview[3];	should have a value of ZERO
	pmat->f[7]			= 0.0f;	// modelview[7];	should have a value of ZERO
	pmat->f[11]			= 0.0f;	// modelview[11];	should have a value of ZERO
	pmat->f[15]			= 1.0f;	// modelview[15];	should have a value of ONE
}

// --------------------------------------------------------------------------- ---------------
// NUT_ExtractCameraMatrix3x3FromGLModelViewMatrix
// ------------------------------------------------------------------------------------------
// Description :
//	Setup a Matrix(3x3) from the current GL_MODELVIEW_MATRIX.
//	Because it's a view matrix we need to transpose the source matrix into the camera matrix.
// ------------------------------------------------------------------------------------------
// In  :
//		NMATRIX			*pmat				"THE" Matrix to setup from the gl data
// Out :
//
// ------------------------------------------------------------------------------------------
void NUT_ExtractCameraMatrix3x3FromGLModelViewMatrix(NMATRIX3x3 *pbase)
{

	GLfloat modelview[16];

	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	pbase->Side.x		= modelview[0];
	pbase->Side.y		= modelview[4];
	pbase->Side.z		= modelview[8];

	pbase->Up.x			= modelview[1];
	pbase->Up.y			= modelview[5];
	pbase->Up.z			= modelview[9];

	pbase->Forward.x	= -modelview[2];
	pbase->Forward.y	= -modelview[6];
	pbase->Forward.z	= -modelview[10];
}
