#include "../Core/NSafeConversion.h"
#include "NVertexTexCoordBuild.h"

NBUILDBASE_TEXCOORD *NSetupBuildBaseTexCoord(NBUILDBASE_TEXCOORD *pbbtxc, const Nu16 flags, const Nu8 txcset_id, const NRECTf32 *pusedoaarectf32, const NRECTf32 *psubaarectf32)
{
	NErrorIf( ( flags&(MASK_NBUILDBASE_TEXCOORD_ALL_FLAGS) )!=flags, NERROR_INCONSISTENT_FLAGS );

	Nmem0(pbbtxc,NBUILDBASE_TEXCOORD);
	pbbtxc->Flags = flags;
	
	if( ISFLAG_ON(flags,FLAG_NBUILDBASE_TEXCOORD_USEPARAM_TEXCOORDSETID) )
	{
		NErrorIf(txcset_id > CONSTANT_NBUILDBASE_TEXCOORD_TEXCOORDSETID_MAX, NERROR_VALUE_OUTOFRANGE);
		pbbtxc->TexCoordSetID	= txcset_id;
	}
	if( ISFLAG_ON(flags,FLAG_NBUILDBASE_TEXCOORD_USEPARAM_USEDOAARECTf32) )
		pbbtxc->UsedOAARectf32 = *pusedoaarectf32;

	if( ISFLAG_ON(flags,FLAG_NBUILDBASE_TEXCOORD_USEPARAM_SUBAARECTf32) )
		pbbtxc->SubAARectf32	= *psubaarectf32;

	return pbbtxc;
}

NBUILDBASE_TEXCOORD *NCreateBuildBaseTexCoord( const Nu16 flags, const Nu8 txcset_id, const NRECTf32 *pusedoaarectf32, const NRECTf32 *psubaarectf32)
{
	return NSetupBuildBaseTexCoord(NEW(NBUILDBASE_TEXCOORD),flags,txcset_id,pusedoaarectf32,psubaarectf32);
}

void NClearBuildBaseTexCoord(NBUILDBASE_TEXCOORD *pbbtxc)
{
	Nmem0(pbbtxc,NBUILDBASE_TEXCOORD);
}

void NDeleteBuildBaseTexCoord(NBUILDBASE_TEXCOORD *pbbtxc)
{
	NClearBuildBaseTexCoord(pbbtxc);
	Nfree(pbbtxc);
}
// ------------------------------------------------------------------------------------------
// Nu32 NGetBuildBaseTexCoordSetID
// ------------------------------------------------------------------------------------------
// Description :
//	Extract the TexCoordSetID specified by user into NBUILDBASE_TEXCOORD structure
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
Nu8	 NGetBuildBaseTexCoordSetID( const NBUILDBASE_TEXCOORD *pbbtexcoord )
{
	if( ISFLAG_ON(pbbtexcoord->Flags,FLAG_NBUILDBASE_TEXCOORD_USEPARAM_TEXCOORDSETID) )
	{
		NErrorIf(pbbtexcoord->TexCoordSetID > CONSTANT_NBUILDBASE_TEXCOORD_TEXCOORDSETID_MAX, NERROR_VALUE_OUTOFRANGE);
		return pbbtexcoord->TexCoordSetID;
	}
	else
	{
		return DEFAULT_NBUILDBASE_TEXCOORD_TEXCOORDSETID;
	}
}

// ------------------------------------------------------------------------------------------
// NMATRIX*	NGetBuildBaseTexCoordMatrix4x4
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
//		NMATRIX						*pmatrix			Output Matrix ptr to Fill in 
//		const NBUILDBASE_TEXCOORD	*pbbtexcoord		Build base textures coordinates valid pointer
// 
// Out :
//		incoming 'pmatrix' pointer itself	
//
// ------------------------------------------------------------------------------------------
NMATRIX* NGetBuildBaseTexCoordMatrix4x4(NMATRIX *pmatrix, const NBUILDBASE_TEXCOORD *pbbtexcoord )
{
	// Check if "pbbtexcoord->Flags" contains only manageable flags. If not, crash. 
	// It's not a bug, just a security to warn user about the flags. To avoid this "Warning crash" user must use one of the 
	// conversion MACRO to make "texcoord_build_context_flags" in the right way.
	NErrorIf( (pbbtexcoord->Flags&(~MASK_NBUILDBASE_TEXCOORD_ALL_FLAGS))!=0, NERROR_FLAG_INCONSISTENCY ); 

	if( ISFLAG_ON(pbbtexcoord->Flags,FLAG_NBUILDBASE_TEXCOORD_USEPARAM_USEDOAARECTf32) )
	{
		NGetOAARectf32_TexCoords_Building3DMatrix4x4(pmatrix,&pbbtexcoord->UsedOAARectf32, pbbtexcoord->Flags&MASK_NBUILDBASE_TEXCOORD_FLIP_UVW );
	}
	else
	{
		NGetOAARectf32_TexCoords_Building3DMatrix4x4(pmatrix,NULL, pbbtexcoord->Flags&MASK_NBUILDBASE_TEXCOORD_FLIP_UVW );
	}

	if( ISFLAG_ON(pbbtexcoord->Flags,FLAG_NBUILDBASE_TEXCOORD_USEPARAM_SUBAARECTf32) )
	{
		return NGetSubAARectf32_TexCoords_Building3DMatrix4x4(pmatrix,pmatrix,&pbbtexcoord->SubAARectf32);
	}
	else
	{
		return pmatrix; 
	}
}
// ------------------------------------------------------------------------------------------
// NMATRIX3x3*	NGetBuildBaseTexCoordMatrix3x3
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
NMATRIX3x3* NGetBuildBaseTexCoordMatrix3x3(NMATRIX3x3 *pmatrix3x3, const NBUILDBASE_TEXCOORD *pbbtexcoord )
{
	// Check if "texcoord_build_context_flags" contain only manageable flags. If not, crash. 
	// It's not a bug, just a security to warn user about the flags. To avoid this "Warning crash" user must use one of the 
	// conversion MACRO to make "texcoord_build_context_flags" in the right way.
	NErrorIf( (pbbtexcoord->Flags&(~MASK_NBUILDBASE_TEXCOORD_ALL_FLAGS))!=0, NERROR_FLAG_INCONSISTENCY ); 

	if( ISFLAG_ON(pbbtexcoord->Flags,FLAG_NBUILDBASE_TEXCOORD_USEPARAM_USEDOAARECTf32) )
	{
		NGetOAARectf32_TexCoords_Building2DMatrix3x3(pmatrix3x3,&pbbtexcoord->UsedOAARectf32, pbbtexcoord->Flags&MASK_NBUILDBASE_TEXCOORD_FLIP_UV );
	}
	else
	{
		NGetOAARectf32_TexCoords_Building2DMatrix3x3(pmatrix3x3,NULL,pbbtexcoord->Flags&MASK_NBUILDBASE_TEXCOORD_FLIP_UV );
	}
	
	// So, from here pmatrix3x3 is defined from NBUILDBASE_TEXCOORD.OAARectf32 or if there is no OAARectf32 to use, from NULL.
	// but, after that, there is possible to have a SubAArect inside this first Base ...
	if( ISFLAG_ON(pbbtexcoord->Flags,FLAG_NBUILDBASE_TEXCOORD_USEPARAM_SUBAARECTf32) )
	{
		return NGetSubAARectf32_TexCoords_Building2DMatrix3x3(pmatrix3x3,pmatrix3x3,&pbbtexcoord->SubAARectf32);
	}
	else
	{
		return pmatrix3x3; 
	}
}


/*
NMATRIX* NGetTexCoordBuilding3DMatrix4x4(NMATRIX *pmatrix, const NRECTf32 *ptxused_oaarectf32, const NRECTf32 *ptx_subaarectf32, const Nu32 ntxcbuild_flags )
{
	// Check if "texcoord_build_context_flags" contains only manageable flags. If not, crash. 
	// It's not a bug, just a security to warn user about the flags. To avoid this "Warning crash" user must use one of the 
	// conversion MACRO to make "texcoord_build_context_flags" in the right way.
	NErrorIf( (ntxcbuild_flags&(~MASK_NTXCBUILD))!=0, NERROR_FLAG_INCONSISTENCY ); 

	if( ISFLAG_ON(ntxcbuild_flags,FLAG_NTXCBUILD_USEPARAM_USEDOAARECTf32) )
	{
		NGetOAARectf32_TexCoords_Building3DMatrix4x4(pmatrix,ptxused_oaarectf32, ntxcbuild_flags&MASK_NTXCBUILD_FLIP_UVW );
	}
	else
	{
		NGetOAARectf32_TexCoords_Building3DMatrix4x4(pmatrix,NULL, ntxcbuild_flags&MASK_NTXCBUILD_FLIP_UVW );
	}

	if( ISFLAG_ON(ntxcbuild_flags,FLAG_NTXCBUILD_USEPARAM_SUBAARECTf32) )
	{
		return NGetSubAARectf32_TexCoords_Building3DMatrix4x4(pmatrix,pmatrix,ptx_subaarectf32);
	}
	else
	{
		return pmatrix; 
	}
}*/
/*
// ------------------------------------------------------------------------------------------
// NMATRIX3x3*	NGetTexCoordBuilding2DMatrix3x3
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
NMATRIX3x3* NGetTexCoordBuilding2DMatrix3x3(NMATRIX3x3 *pmatrix3x3, const NRECTf32 *ptxused_oaarectf32, const NRECTf32 *ptx_subaarectf32, const Nu32 ntxcbuild_flags )
{
	// Check if "texcoord_build_context_flags" contain only manageable flags. If not, crash. 
	// It's not a bug, just a security to warn user about the flags. To avoid this "Warning crash" user must use one of the 
	// conversion MACRO to make "texcoord_build_context_flags" in the right way.
	NErrorIf( (ntxcbuild_flags&(~MASK_NTXCBUILD))!=0, NERROR_FLAG_INCONSISTENCY ); 

	if( ISFLAG_ON(ntxcbuild_flags,FLAG_NTXCBUILD_USEPARAM_USEDOAARECTf32) )
	{
		NGetOAARectf32_TexCoords_Building2DMatrix3x3(pmatrix3x3,ptxused_oaarectf32, ntxcbuild_flags&MASK_NTXCBUILD_FLIP_UV );
	}
	else
	{
		NGetOAARectf32_TexCoords_Building2DMatrix3x3(pmatrix3x3,NULL,ntxcbuild_flags&MASK_NTXCBUILD_FLIP_UV );
	}

	if( ISFLAG_ON(ntxcbuild_flags,FLAG_NTXCBUILD_USEPARAM_SUBAARECTf32) )
	{
		return NGetSubAARectf32_TexCoords_Building2DMatrix3x3(pmatrix3x3,pmatrix3x3,ptx_subaarectf32);
	}
	else
	{
		return pmatrix3x3; 
	}
}
*/

// ------------------------------------------------------------------------------------------
// NOAARECT_CCW_ENUM NGetOAARects16_CCWOrientation
// ------------------------------------------------------------------------------------------
// Description :
//	Get the orientation of a supposed Oriented Axis Aligned Rect
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
NOAARECT_CCW_ENUM NGetOAARects16_CCWOrientation(const NRECTs16 *poaarects16)	
{
	NErrorIf(!poaarects16,NERROR_NULL_POINTER);
	// Retrieve the AA origin to compare with the incoming origin and conclude the Oriented Base of the incoming RECT ! ...
	Ns16 aatop		= NMIN(poaarects16->top,	poaarects16->bottom);
	Ns16 aaleft		= NMIN(poaarects16->left,	poaarects16->right);

	if(aatop == poaarects16->top)
	{
		// Axis Aligned Base !
		if(aaleft == poaarects16->left)
		{
			return NOAARECT_CCW0;
		}
		// CCW90 Base !
		else
		{
			return NOAARECT_CCW90;
		}
	}
	else
	{
		// CCW270 Base !
		if(aaleft == poaarects16->left)
		{
			return NOAARECT_CCW270;
		}
		// CCW180 Base !
		else
		{
			return NOAARECT_CCW180;
		}
	}
}
// ------------------------------------------------------------------------------------------
// NOAARECT_CCW_ENUM NGetOAARectf32_CCWOrientation
// ------------------------------------------------------------------------------------------
// Description :
//	Get the orientation of a supposed Oriented Axis Aligned Rect
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
NOAARECT_CCW_ENUM NGetOAARectf32_CCWOrientation(const NRECTf32 *poaarectf32)	
{
	NErrorIf(!poaarectf32,NERROR_NULL_POINTER);
	// Retrieve the AA origin to compare with the incoming origin and conclude the Oriented Base of the incoming RECT ! ...
	Nf32 aatop		= NMIN(poaarectf32->top,	poaarectf32->bottom);
	Nf32 aaleft		= NMIN(poaarectf32->left,	poaarectf32->right);

	if(aatop == poaarectf32->top)
	{
		// Axis Aligned Base !
		if(aaleft == poaarectf32->left)
		{
			return NOAARECT_CCW0;
		}
		// CCW90 Base !
		else
		{
			return NOAARECT_CCW90;
		}
	}
	else
	{
		// CCW270 Base !
		if(aaleft == poaarectf32->left)
		{
			return NOAARECT_CCW270;
		}
		// CCW180 Base !
		else
		{
			return NOAARECT_CCW180;
		}
	}
}
// ------------------------------------------------------------------------------------------
// NMATRIX3x3* NGetSubAARectf32_TexCoords_Building2DMatrix3x3
// ------------------------------------------------------------------------------------------
// Description :
//	This function build a 2D orthogonal ( represented by an homogeneous matrix3x3) from an already build 2D Orthogonal
//	AND a Axis Aligned RECTf32.
//	This new Matrix represents also a 2D Orthogonal
//		+ "Origin" of the orthogonal (Matrix3x3.Translation) represents the TOP-LEFT corner point of the rectangle.
//		+ "UAxis" of the orthogonal (Matrix3x3.XAxis) represents the oriented "width" of the rectangle. 
//		  This vector has a length of "width". Is oriented from "LEFT" to "RIGHT" ("relative" to the Oriented rectangle).
//		+ "VAxis" of the orthogonal (Matrix3x3.YAxis) represents the oriented "height" of the rectangle. 
//		  This vector has a length of "height". Is oriented from "TOP" to "BOTTOM" ("relative" to the Oriented rectangle).
//
//	This function is used to define a 'sub-region' of a texture inside a first Oriented Region of this texture.
//	The 'sub-region' is describe by a AA Rectangle (NO Oriented) with coordinates define in the base describe by the "pmatrix"
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
NMATRIX3x3* NGetSubAARectf32_TexCoords_Building2DMatrix3x3(NMATRIX3x3 *psubmatrix, const NMATRIX3x3 *pmatrix,const NRECTf32 *puvrect )
{
	NMATRIX3x3	matrix;
	Nf32		f;

	if(puvrect)
	{
		// DEBUG ONLY: Check 'puvrect' as AARectf32 and crash if not (that means crash if 'puvrect' is a OAARECTf32)
		NErrorIf(NGetOAARectf32_CCWOrientation(puvrect)!=NOAARECT_CCW0, NERROR_RECT_IS_OAARECT);
		
		f = puvrect->right - puvrect->left; // Width
		NErrorIf(!f,NERROR_NULL_VALUE);
		matrix.XAxis.x = pmatrix->XAxis.x*f;
		matrix.XAxis.y = pmatrix->XAxis.y*f;
		matrix.XAxis.z = pmatrix->XAxis.z*f;// should be 0.0f

		f = puvrect->bottom - puvrect->top; // Height (notice that in Texture orthonormal bottom > top )
		NErrorIf(!f,NERROR_NULL_VALUE);
		matrix.YAxis.x = pmatrix->YAxis.x*f;
		matrix.YAxis.y = pmatrix->YAxis.y*f;
		matrix.YAxis.z = pmatrix->YAxis.z*f;// should be 0.0f

		matrix.Origin.x = pmatrix->Origin.x + pmatrix->XAxis.x*puvrect->left + pmatrix->YAxis.x*puvrect->top;
		matrix.Origin.y = pmatrix->Origin.y + pmatrix->XAxis.y*puvrect->left + pmatrix->YAxis.y*puvrect->top;
		matrix.Origin.z = pmatrix->Origin.z + pmatrix->XAxis.z*puvrect->left + pmatrix->YAxis.z*puvrect->top;// should be 1.0f

		*psubmatrix = matrix;
	}
	else // puvrect == NULL;
	{
		if(psubmatrix != pmatrix)
			*psubmatrix = *pmatrix;
	}

	return psubmatrix;
}
// ------------------------------------------------------------------------------------------
// NMATRIX* NGetSubAARectf32_TexCoords_Building3DMatrix4x4
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
NMATRIX* NGetSubAARectf32_TexCoords_Building3DMatrix4x4(NMATRIX *psubmatrix, const NMATRIX *pmatrix,const NRECTf32 *puvrect )
{
	NMATRIX		matrix;
	Nf32		f;

	if(puvrect)
	{
		// DEBUG ONLY: Check 'puvrect' as AARectf32 and crash if not (that means crash if 'puvrect' is a OAARECTf32)
		NErrorIf(NGetOAARectf32_CCWOrientation(puvrect)!=NOAARECT_CCW0, NERROR_RECT_IS_OAARECT);

		f = puvrect->right - puvrect->left; // Width
		matrix.XAxis.x = pmatrix->XAxis.x*f;
		matrix.XAxis.y = pmatrix->XAxis.y*f;
		matrix.XAxis.z = pmatrix->XAxis.z*f;// should be 0.0f
		matrix.XAxis.w = pmatrix->XAxis.w*f;// should be 0.0f

		f = puvrect->bottom - puvrect->top; // Height (notice that in Texture orthonormal bottom > top )
		matrix.YAxis.x = pmatrix->YAxis.x*f;
		matrix.YAxis.y = pmatrix->YAxis.y*f;
		matrix.YAxis.z = pmatrix->YAxis.z*f;// should be 0.0f
		matrix.YAxis.w = pmatrix->YAxis.w*f;// should be 0.0f

		matrix.ZAxis   = pmatrix->ZAxis; // (it's like f = 1.0f)

		matrix.Translation.x = pmatrix->Translation.x + pmatrix->XAxis.x*puvrect->left + pmatrix->YAxis.x*puvrect->top + pmatrix->ZAxis.x;
		matrix.Translation.y = pmatrix->Translation.y + pmatrix->XAxis.y*puvrect->left + pmatrix->YAxis.y*puvrect->top + pmatrix->ZAxis.y;
		matrix.Translation.z = pmatrix->Translation.z + pmatrix->XAxis.z*puvrect->left + pmatrix->YAxis.z*puvrect->top + pmatrix->ZAxis.z;// should be 0.0f
		matrix.Translation.w = pmatrix->Translation.w + pmatrix->XAxis.w*puvrect->left + pmatrix->YAxis.w*puvrect->top + pmatrix->ZAxis.w;// should be 1.0f

		*psubmatrix = matrix;
	}
	else // puvrect == NULL;
	{
		if(psubmatrix != pmatrix)
			*psubmatrix = *pmatrix;
	}

	return psubmatrix;
}

NMATRIX* NGetOAARectf32_TexCoords_Building3DMatrix4x4(NMATRIX *ptm, const NRECTf32 *poaarectf32,const Nu32 ntxcbuild_flip_uvw_flags )
{
	NErrorIf(!ptm, NERROR_NULL_POINTER);

	NVEC2	origin,i,j;
	NGetOAARectf32Orthogonal(&origin,&i,&j,poaarectf32,ntxcbuild_flip_uvw_flags);

	ptm->XAxis.x		= i.x;
	ptm->XAxis.y		= i.y;
	ptm->XAxis.z		= 0.0f;
	ptm->XAxis.w		= 0.0f;

	ptm->YAxis.x		= j.x;
	ptm->YAxis.y		= j.y;
	ptm->YAxis.z		= 0.0f;
	ptm->YAxis.w		= 0.0f;

	ptm->ZAxis.x		= 0.0f;
	ptm->ZAxis.y		= 0.0f;
	ptm->ZAxis.z		= 1.0f;
	ptm->ZAxis.w		= 0.0f;

	ptm->Translation.x	= origin.x;
	ptm->Translation.y	= origin.y;
	ptm->Translation.z	= 0.0f;
	ptm->Translation.w	= 1.0f;

	// FLIPS W (FLIP U and V already done into NGetOAARectf32Orthogonal function )
	if(ISFLAG_ON(ntxcbuild_flip_uvw_flags, FLAG_NBUILDBASE_TEXCOORD_FLIP_W))
	{
		ptm->Translation.z	=  1.0f;
		ptm->ZAxis.z		= -1.0f;
	}
	return ptm;
}
// ------------------------------------------------------------------------------------------
// NGetOAARectf32_TexCoords_Building2DMatrix3x3
//
// ------------------------------------------------------------------------------------------
// Description :
// Usually a RECT is ... a 'AA RECT'(Axis Aligned Rect)  with Top, Bottom, Left and Right are real Top, Bottom,
// Left and Right Coordinates.  (BOTTOM > TOP and RIGHT > LEFT.) 
// BUT ! When a RECT is used to represent UVs Area this rule could be broken to represents possible ATLAS ELEMENT 90° 
// snapped Orientation into texture.
// Very often, ATLAS elements which are aggregate together into a single texture may have different orientation,
// because in that way they fit better the whole texture surface. 
// So in the same texture there will be pure Axis Aligned ATLAS ELEMENT and "Oriented" Axis Aligned ATLAS ELEMENT.
// To store this information, ATLAS element coordinates are just stored into a relative oriented way.
// That means, TOP value doesn't represent the lowest vertical coordinate relatively to the texture, and the BOTTOM value
// doesn't represent the biggest one. same thing for LEFT and RIGHT coordinates. 
// BUT one thing stills always TRUE: From the relative TOP LEFT Origin of the Texture Rect we will reach the relative Bottom Right
// corner by adding successively 2dvector "u" and 2dvector "v".
//			BOTTOM_RIGHT corner = TOP_LEFT Corner + vector U + vector V
// An Oriented Axis Aligned Rectangle gives us its TOP value into its own base. Same thing for BOTOM, RIGHT and LEFT.
// So let's compare them against Axis Aligned Base to define the Oriented Axis Aligned Rect orientation and by this way,
// its Origin its U vector and its V vector.
//
// So to summarize, this function build a 2D orthogonal ( represented by an homogeneous matrix3x3) from an oriented Rectangle.
//		+ "Origin" of the orthogonal (Matrix3x3.Translation) represents the TOP-LEFT corner point of the rectangle.
//		+ "UAxis" of the orthogonal (Matrix3x3.XAxis) represents the oriented "width" of the rectangle. 
//		  This vector has a length of "width". Is oriented from "LEFT" to "RIGHT" ("relative" to the Oriented rectangle).
//		+ "VAxis" of the orthogonal (Matrix3x3.YAxis) represents the oriented "height" of the rectangle. 
//		  This vector has a length of "height". Is oriented from "TOP" to "BOTTOM" ("relative" to the Oriented rectangle).
// ------------------------------------------------------------------------------------------
// In  :
//		NMATRIX3x3		*ptm						a valid Ptr to a Matrix3x3 that will be used as an homogeneous 2D transformation Matrix.
//		const NRECTf32	*poaarectf32				a valid Ptr to a NRECTf32 which could be an Oriented Rect.
//		const Nu32		ntxcbuild_flip_uv_flags		a valid set of FLAGS ( valid means that only authorized FLAGS must be used here ! )
//													They are:	FLAG_NBUILDBASE_TEXCOORD_FLIP_U and FLAG_NBUILDBASE_TEXCOORD_FLIP_V
// Out :		
//		return the incoming 'ptm' pointer.
// ------------------------------------------------------------------------------------------
NMATRIX3x3* NGetOAARectf32_TexCoords_Building2DMatrix3x3(NMATRIX3x3 *ptm, const NRECTf32 *poaarectf32,const Nu32 ntxcbuild_flip_uv_flags )
{
	NErrorIf(!ptm, NERROR_NULL_POINTER);
	// Purpose of this function is building and return a 2D MATRIX !!! It shouldn't be a W Axis. So, even if it isn't a BUG ...
	// ... to warn user, N generate an error here.
	NErrorIf(ISFLAG_ON(ntxcbuild_flip_uv_flags,FLAG_NBUILDBASE_TEXCOORD_FLIP_W), NERROR_INCONSISTENT_FLAGS); 

	// Is Oriented Axis Aligned Rect ?
	// 
	//		Axis Aligned Base		CCW90 Base					CCW180 Base					CCW270 Base
	// 		O									  O					      vector V				 vector U
	// 		+----> vector U			vector V <----+							  ^						^	   
	// 		|									  |							  |						|	  
	// 		|									  |							  |						|	  
	// 		v									  v				vector U <----+						+----> vector V 
	//   vector V							   vector U						  O						O	  	 

	NVEC2	origin,i,j;
	NGetOAARectf32Orthogonal(&origin,&i,&j,poaarectf32,ntxcbuild_flip_uv_flags);
	ptm->XAxis.x		= i.x;
	ptm->XAxis.y		= i.y;
	ptm->XAxis.z		= 0.0f;

	ptm->YAxis.x		= j.x;
	ptm->YAxis.y		= j.y;
	ptm->YAxis.z		= 0.0f;

	ptm->Origin.x	= origin.x;
	ptm->Origin.y	= origin.y;
	ptm->Origin.z	= 1.0f;
	return ptm;
}

NVEC2* NGetOAARectf32Size(NVEC2 *psize, const NRECTf32 *poaarectf32 )
{
	NVEC2	origin,iaxis,jaxis;
	NGetOAARectf32Orthogonal(&origin,&iaxis,&jaxis,poaarectf32,0);
	psize->x = NABS(iaxis.x + iaxis.y); // | Special cases of  "length = sqrt(x*x + y*y)" with "x" OR "y" = ZERO !!
	psize->y = NABS(jaxis.x + jaxis.y); // | So we will have length = sqrt(x*x) or length = sqrt(y*y) ... 
	return psize;
}
NVEC2s16* NGetOAARects16Size(NVEC2s16* psize, const NRECTs16 *poaarects16 )
{
	NVEC2s16	origin,iaxis,jaxis;
	NGetOAARects16Orthogonal(&origin,&iaxis,&jaxis,poaarects16,0);
	psize->x = NABS(iaxis.x + iaxis.y); // | Special cases of  "length = sqrt(x*x + y*y)" with "x" OR "y" = ZERO !!
	psize->y = NABS(jaxis.x + jaxis.y); // | So we will have length = sqrt(x*x) or length = sqrt(y*y) ... 
	return psize;
}

void NGetOAARectf32Orthogonal(NVEC2 *porigin, NVEC2 *puaxis, NVEC2 *pvaxis, const NRECTf32 *poaarectf32,const Nu32 ntxcbuild_flip_uv_flags )
{
	NErrorIf(!porigin||!puaxis||!pvaxis, NERROR_NULL_POINTER);
	
	// Accept only used FLAGS !
	NErrorIf((ntxcbuild_flip_uv_flags&MASK_NBUILDBASE_TEXCOORD_FLIP_UV)!=ntxcbuild_flip_uv_flags, NERROR_INCONSISTENT_FLAGS );

	// Identity Orthogonal ?	
	if(!poaarectf32)
	{
		porigin->x	= 0.0f;
		porigin->y	= 0.0f;
		
		puaxis->x	= 1.0f;
		puaxis->y	= 0.0f;

		pvaxis->x	= 0.0f;
		pvaxis->y	= 1.0f;
	}
	else
	{
		NErrorIf(poaarectf32->left == poaarectf32->right, NERROR_INCONSISTENT_VALUES);
		NErrorIf(poaarectf32->top == poaarectf32->bottom, NERROR_INCONSISTENT_VALUES);

		// Is Oriented Axis Aligned Rect ?
		// ( x represents O + vector U + vector V ) 
		//		Axis Aligned Base		CCW90 Base					CCW180 Base					CCW270 Base
		// 		O									  O					      vector V				 vector U
		// 		+----> vector U			vector V <----+						 x	  ^						^	 x  
		// 		|									  |							  |						|	  
		// 		|									  |							  |						|	  
		// 		v	 x							 x 	  v				vector U <----+						+----> vector V 
		//   vector V							   vector U						  O						O	  	 

		// Origin of the Orthogonal
		porigin->x = poaarectf32->left;
		porigin->y = poaarectf32->top;

		// Retrieve the AA origin to compare with the incoming origin and conclude the Oriented Base of the incoming RECT ! ...
		Nf32 aatop		= NMIN(poaarectf32->top,poaarectf32->bottom);
		Nf32 aaleft		= NMIN(poaarectf32->left,poaarectf32->right);

		if(aatop == poaarectf32->top)
		{
			// Axis Aligned Base !
			if(aaleft == poaarectf32->left)
			{
				puaxis->x = poaarectf32->right - poaarectf32->left;
				puaxis->y = 0.0f;

				pvaxis->x = 0.0f;
				pvaxis->y = poaarectf32->bottom - poaarectf32->top;
			}
			// CCW90 Base !
			else
			{
				puaxis->x = 0.0f;
				puaxis->y = poaarectf32->bottom - poaarectf32->top;

				pvaxis->x = poaarectf32->right - poaarectf32->left;
				pvaxis->y = 0.0f;
			}
		}
		else
		{
			// CCW270 Base !
			if(aaleft == poaarectf32->left)
			{
				puaxis->x = 0.0f;
				puaxis->y = poaarectf32->bottom - poaarectf32->top;

				pvaxis->x = poaarectf32->right - poaarectf32->left;
				pvaxis->y = 0.0f;
			}
			// CCW180 Base !
			else
			{
				puaxis->x = poaarectf32->right - poaarectf32->left;
				puaxis->y = 0.0f;

				pvaxis->x = 0.0f;
				pvaxis->y = poaarectf32->bottom - poaarectf32->top;
			}
		}
	}

	// FLIPS U & V
	if(ISFLAG_ON(ntxcbuild_flip_uv_flags, FLAG_NBUILDBASE_TEXCOORD_FLIP_U))
	{
		NVec2AddTo(porigin,puaxis);
		NVec2ScaleBy(puaxis,-1.0f);
	}
	if(ISFLAG_ON(ntxcbuild_flip_uv_flags, FLAG_NBUILDBASE_TEXCOORD_FLIP_V))
	{
		NVec2AddTo(porigin,pvaxis);
		NVec2ScaleBy(pvaxis,-1.0f);
	}
}

void NGetOAARects16Orthogonal(NVEC2s16 *porigin, NVEC2s16 *puaxis, NVEC2s16 *pvaxis, const NRECTs16 *poaarects16,const Nu32 ntxcbuild_flip_uv_flags )
{
	NErrorIf(!porigin||!puaxis||!pvaxis, NERROR_NULL_POINTER);
	// Accept only used FLAGS !
	NErrorIf((ntxcbuild_flip_uv_flags&MASK_NBUILDBASE_TEXCOORD_FLIP_UV)!=ntxcbuild_flip_uv_flags, NERROR_INCONSISTENT_FLAGS );

	// Identity Orthogonal ?	
	if(!poaarects16)
	{
		porigin->x	= 0;
		porigin->y	= 0;

		puaxis->x	= 1;
		puaxis->y	= 0;

		pvaxis->x	= 0;
		pvaxis->y	= 1;
	}
	else
	{
		NErrorIf(poaarects16->left == poaarects16->right, NERROR_INCONSISTENT_VALUES);
		NErrorIf(poaarects16->top == poaarects16->bottom, NERROR_INCONSISTENT_VALUES);

		// Is Oriented Axis Aligned Rect ?
		// ( x represents O + vector U + vector V ) 
		//		Axis Aligned Base		CCW90 Base					CCW180 Base					CCW270 Base
		// 		O									  O					      vector V				 vector U
		// 		+----> vector U			vector V <----+						 x	  ^						^	 x  
		// 		|									  |							  |						|	  
		// 		|									  |							  |						|	  
		// 		v	 x							 x 	  v				vector U <----+						+----> vector V 
		//   vector V							   vector U						  O						O	  	 

		// Origin of the Orthogonal
		porigin->x = poaarects16->left;
		porigin->y = poaarects16->top;

		// Retrieve the AA origin to compare with the incoming origin and conclude the Oriented Base of the incoming RECT ! ...
		Nf32 aatop		= NMIN(poaarects16->top,poaarects16->bottom);
		Nf32 aaleft		= NMIN(poaarects16->left,poaarects16->right);

		if(aatop == poaarects16->top)
		{
			// Axis Aligned Base !
			if(aaleft == poaarects16->left)
			{
				puaxis->x = poaarects16->right - poaarects16->left;
				puaxis->y = 0;

				pvaxis->x = 0;
				pvaxis->y = poaarects16->bottom - poaarects16->top;
			}
			// CCW90 Base !
			else
			{
				puaxis->x = 0;
				puaxis->y = poaarects16->bottom - poaarects16->top;

				pvaxis->x = poaarects16->right - poaarects16->left;
				pvaxis->y = 0;
			}
		}
		else
		{
			// CCW270 Base !
			if(aaleft == poaarects16->left)
			{
				puaxis->x = 0;
				puaxis->y = poaarects16->bottom - poaarects16->top;

				pvaxis->x = poaarects16->right - poaarects16->left;
				pvaxis->y = 0;
			}
			// CCW180 Base !
			else
			{
				puaxis->x = poaarects16->right - poaarects16->left;
				puaxis->y = 0;

				pvaxis->x = 0;
				pvaxis->y = poaarects16->bottom - poaarects16->top;
			}
		}
	}

	// FLIPS U & V
	if( ISFLAG_ON(ntxcbuild_flip_uv_flags, FLAG_NBUILDBASE_TEXCOORD_FLIP_U) )
	{
		porigin->x	+= puaxis->x;
		porigin->y	+= puaxis->y;
		puaxis->x	= -puaxis->x;
		puaxis->y	= -puaxis->y;
	}
	if( ISFLAG_ON(ntxcbuild_flip_uv_flags, FLAG_NBUILDBASE_TEXCOORD_FLIP_V) )
	{
		porigin->x	+= pvaxis->x;
		porigin->y	+= pvaxis->y;
		pvaxis->x	= -pvaxis->x;
		pvaxis->y	= -pvaxis->y;
	}
}

/*

// ------------------------------------------------------------------------------------------
// Nbool NCheckTexCoordBuildContext_TargetTexCoordSet
// ------------------------------------------------------------------------------------------
// Description :
//	Verify that a TexCoordBuildContext Match with a geometry.
//	This function check context vertex format requirement. 
//	Notes:	right now, N manages only 2f texture coordinates so we are just going to check
//			that the requested TexCoordSet ID is available into the geometry vertex format.
//			TODO: add some others requirement.
// ------------------------------------------------------------------------------------------
// In	:
//			const NTEXCOORD_BUILD_CONTEXT		*pcontext		a valid pointer to a texture coordinates context structure
//			const NGEOMETRY						*pgeom			a valid pointer to a geometry Structure to check with
// Out :
//			NTRUE/NFALSE as a response to the question: Does the geometry possess the requested texture coordinates Set ? 
//
// ------------------------------------------------------------------------------------------
Nbool NCheckTexCoordBuildContext_TargetTexCoordSet(const NTEXCOORD_BUILD_CONTEXT *pcontext, const NGEOMETRY *pgeom )
{
	if( pcontext->TexCoordSetID == 0 && NCHECK_VERTEX_SPECS_TEXCOORD0_2f(NGetVertexSpecs(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS))->Flags) )
	{
		return NTRUE;
	}
	else if( pcontext->TexCoordSetID == 1 && NCHECK_VERTEX_SPECS_TEXCOORD1_2f(NGetVertexSpecs(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS))->Flags) )
	{
		return NTRUE;
	}
	else
	{
		return FALSE;
	}
}
*/

/*
NTEXCOORD_BUILD_CONTEXT* NSetupTexCoordBuildContext(NTEXCOORD_BUILD_CONTEXT *pcontext, const Nu8 target_texcoord_set_id,const NRECTf32 *ptextureusedOAArectf32,const Nu32 flags_24bits)
{
	NErrorIf(!pcontext,NERROR_NULL_POINTER);
	NErrorIf(target_texcoord_set_id > 1, NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA );
	NErrorIf((flags_24bits&0xFF000000)!= 0,NERROR_INCONSISTENT_FLAGS);
	
	pcontext->Flags				= (flags_24bits&0xFFFFFF);
	pcontext->TexCoordSetID		= target_texcoord_set_id;
	if(ptextureusedOAArectf32)
	{
		pcontext->TxUsedOAARectf32	= *ptextureusedOAArectf32;
	}
	else 
	{
		pcontext->TxUsedOAARectf32.left		= 0.0f;
		pcontext->TxUsedOAARectf32.top		= 0.0f;
		pcontext->TxUsedOAARectf32.right	= 1.0f;
		pcontext->TxUsedOAARectf32.bottom	= 1.0f;
	}
	return pcontext;
}

NTEXCOORD_BUILD_CONTEXT* NCreateTexCoordBuildContext(const Nu8 target_texcoord_set_id,const NRECTf32 *ptextureusedOAArectf32,const Nu32 flags_24bits)
{
	return NSetupTexCoordBuildContext(NEW(NTEXCOORD_BUILD_CONTEXT),target_texcoord_set_id,ptextureusedOAArectf32,flags_24bits);
}

void NClearTexCoordBuildContext(NTEXCOORD_BUILD_CONTEXT *pcontext)
{
	Nmem0(pcontext,NTEXCOORD_BUILD_CONTEXT);
}

void NDeleteTexCoordBuildContext(NTEXCOORD_BUILD_CONTEXT *pcontext)
{
	NClearTexCoordBuildContext(pcontext);
	Nfree(pcontext);
}
*/

/*
OLDIEST

// ------------------------------------------------------------------------------------------
// NGetTexCoordBuildContext_Texture2DOrthogonal
// ! TODO: Replace it by:
//			NMATRIX* NGetTexCoordBuildContext_TextureMatrix(NMATRIX *ptm, const NTEXCOORD_BUILD_CONTEXT *pcontext)
//
// ------------------------------------------------------------------------------------------
// Description :
//
// Evaluate the Oriented base describe into a Texture Coordinate Context Structure (NTEXCOORD_BUILD_CONTEXT).
// NTEXCOORD_BUILD_CONTEXT.TxUsedOAARectf32 is an oriented (or non oriented ) Rect.
// Explanation(s):
// Usually a RECT is ... a 'AA RECT'(Axis Aligned Rect)  with Top, Bottom, Left and Right are real Top, Bottom,
// Left and Right Coordinates.  (BOTTOM > TOP and RIGHT > LEFT.) 
// BUT ! When a RECT is used to represent UVs Area this rule could be broken to represents possible ATLAS ELEMENT 90° 
// snapped Orientation into texture.
// Very often, ATLAS elements which are aggregate together into a single texture may have different orientation,
// because in that way they fit better the whole texture surface. 
// So in the same texture there will be pure Axis Aligned ATLAS ELEMENT and "Oriented" Axis Aligned ATLAS ELEMENT.
// To store this information, ATLAS element coordinates are just stored into a relative oriented way.
// That means, TOP value doesn't represent the lowest vertical coordinate relatively to the texture, and the BOTTOM value
// doesn't represent the biggest one. same thing for LEFT and RIGHT coordinates. 
// BUT one thing stills always TRUE: From the relative TOP LEFT Origin of the Texture Rect we will reach the relative Bottom Right
// corner by adding successively 2dvector "u" and 2dvector "v".
//			BOTTOM_RIGHT corner = TOP_LEFT Corner + vector U + vector V
// An Oriented Axis Aligned Rectangle gives us its TOP value into its own base. Same thing for BOTOM, RIGHT and LEFT.
// So let's compare them against Axis Aligned Base to define the Oriented Axis Aligned Rect orientation and by this way,
// its Origin its U vector and its V vector.
//
// So to summarize, this function build an orthogonal from an oriented Rectangle.
//		+ "Origin" of the orthogonal (translation) represents the TOP-LEFT corner point of the rectangle.
//		+ "UAxis" of the orthogonal represents the oriented "width" of the rectangle. 
//		  This vector has a length of "width". Is oriented from "LEFT" to "RIGHT" ("relative" to the Oriented rectangle).
//		+ "VAxis" of the orthogonal represents the oriented "height" of the rectangle. 
//		  This vector has a length of "height". Is oriented from "TOP" to "BOTTOM" ("relative" to the Oriented rectangle).
// ------------------------------------------------------------------------------------------
// In  :
//		NTEXCOORD_BUILD_ORTHOGONAL			*porthogonal			a valid Ptr to a texcoord orthogonal.
//		const NTEXCOORD_BUILD_CONTEXT		*pcontext				a valid Ptr to a texcoord build context
// Out :		
//		return the incoming 'porthogonal' pointer.
// ------------------------------------------------------------------------------------------
NTEXCOORD_BUILD_2DORTHOGONAL* NGetTexCoordBuildContext_Texture2DOrthogonal(NTEXCOORD_BUILD_2DORTHOGONAL *porthogonal, const NTEXCOORD_BUILD_CONTEXT *pcontext)
{
	NErrorIf(!porthogonal, NERROR_NULL_POINTER);
	NErrorIf(!pcontext, NERROR_NULL_POINTER);

	// Is Oriented Axis Aligned Rect ?
	// 
	//		Axis Aligned Base		CCW90 Base					CCW180 Base					CCW270 Base
	// 		O									  O					      vector V				 vector U
	// 		+----> vector U			vector V <----+							  ^						^	   
	// 		|									  |							  |						|	  
	// 		|									  |							  |						|	  
	// 		v									  v				vector U <----+						+----> vector V 
	//   vector V							   vector U						  O						O	  	 

	porthogonal->Translation.x = pcontext->TxUsedOAARectf32.left;
	porthogonal->Translation.y = pcontext->TxUsedOAARectf32.top;

	// Retrieve the AA origin to compare with the incoming origin and conclude the Oriented Base of the incoming RECT ! ...
	Nf32 aatop		= NMIN(pcontext->TxUsedOAARectf32.top,pcontext->TxUsedOAARectf32.bottom);
	Nf32 aaleft		= NMIN(pcontext->TxUsedOAARectf32.left,pcontext->TxUsedOAARectf32.right);

	if(aatop == pcontext->TxUsedOAARectf32.top)
	{
		// Axis Aligned Base !
		if(aaleft == pcontext->TxUsedOAARectf32.left)
		{
			porthogonal->UAxis.x = pcontext->TxUsedOAARectf32.right - pcontext->TxUsedOAARectf32.left;
			porthogonal->UAxis.y = 0.0f;

			porthogonal->VAxis.x = 0.0f;
			porthogonal->VAxis.y = pcontext->TxUsedOAARectf32.bottom - pcontext->TxUsedOAARectf32.top;
		}
		// CCW90 Base !
		else
		{
			porthogonal->UAxis.x = 0.0f;
			porthogonal->UAxis.y = pcontext->TxUsedOAARectf32.bottom - pcontext->TxUsedOAARectf32.top;

			porthogonal->VAxis.x = pcontext->TxUsedOAARectf32.right - pcontext->TxUsedOAARectf32.left;
			porthogonal->VAxis.y = 0.0f;
		}
	}
	else
	{
		// CCW270 Base !
		if(aaleft == pcontext->TxUsedOAARectf32.left)
		{
			porthogonal->UAxis.x = 0.0f;
			porthogonal->UAxis.y = pcontext->TxUsedOAARectf32.bottom - pcontext->TxUsedOAARectf32.top;

			porthogonal->VAxis.x = pcontext->TxUsedOAARectf32.right - pcontext->TxUsedOAARectf32.left;
			porthogonal->VAxis.y = 0.0f;
		}
		// CCW180 Base !
		else
		{
			porthogonal->UAxis.x = pcontext->TxUsedOAARectf32.right - pcontext->TxUsedOAARectf32.left;
			porthogonal->UAxis.y = 0.0f;

			porthogonal->VAxis.x = 0.0f;
			porthogonal->VAxis.y = pcontext->TxUsedOAARectf32.bottom - pcontext->TxUsedOAARectf32.top;
		}
	}

	// FLIP Horizontally
	if( ISFLAG_ON(pcontext->Flags,FLAG_NTEXCOORDS_BUILD_CONTEXT_FLIP_U) )
	{
		NVec2AddTo(&porthogonal->Translation,&porthogonal->UAxis);
		NVec2ScaleBy(&porthogonal->UAxis,-1.0f);
	}

	// FLIP Vertically
	if( ISFLAG_ON(pcontext->Flags,FLAG_NTEXCOORDS_BUILD_CONTEXT_FLIP_V) )
	{
		NVec2AddTo(&porthogonal->Translation,&porthogonal->VAxis);
		NVec2ScaleBy(&porthogonal->VAxis,-1.0f);
	}

	return porthogonal;
}

*/

/*
// ------------------------------------------------------------------------------------------
// NGetTexCoordOrthogonal
// ------------------------------------------------------------------------------------------
// Description :
// Evaluate the Oriented base of an oriented (or non oriented ) Rect.
// Usually a RECT is ... a 'AA RECT'(Axis Aligned Rect)  with Top, Bottom, Left and Right are real Top, Bottom,
// Left and Right Coordinates.  (BOTTOM > TOP and RIGHT > LEFT.) 
// BUT ! When a RECT is used to represent UVs this rule could be broken to represents possible ATLAS ELEMENT 90° 
// snapped Orientation into texture.
// Very often, ATLAS elements which are aggregate together into a single texture may have different orientation,
// because in that way they fit better the whole texture surface. 
// So in the same texture there will be pure Axis Aligned ATLAS ELEMENT and "Oriented" Axis Aligned ATLAS ELEMENT.
// To store this information, ATLAS element coordinates are just stored into a relative oriented way.
// That means, TOP value doesn't represent the lowest vertical coordinate relatively to the texture, and the BOTTOM value
// doesn't represent the biggest one. same thing for LEFT and RIGHT coordinates. 
// BUT one thing stills always TRUE: From the relative TOP LEFT Origin of the Texture Rect we will reach the relative Bottom Right
// corner by adding successively 2dvector "u" and 2dvector "v".
//			BOTTOM_RIGHT corner = TOP_LEFT Corner + vector U + vector V
// An Oriented Axis Aligned Rectangle gives us its TOP value into its own base. Same thing for BOTOM, RIGHT and LEFT.
// So let's compare them against Axis Aligned Base to define the Oriented Axis Aligned Rect orientation and by this way,
// its Origin its U vector and its V vector.
// ------------------------------------------------------------------------------------------
// In  :
//		NVEC2 *porigin			a valid Ptr to write in the Base Origin.
//		NVEC2 *pvect_u			a valid Ptr to write in the Base U vector.
//		NVEC2 *pvect_v			a valid Ptr to write in the Base V vector.
//		NRECTf32 *porientedrect		Incoming OAA Rectangle (it could be AA rectangle too, because AA Rect is a OAA Rect with 
//															an orientation of 0. Actually, purpose of this function is retrieving
//															the orthogonal from a rect that we ignore the orientation. )
// Out :		
//		nothing
// ------------------------------------------------------------------------------------------
void NGetTexCoordOrthogonal(NVEC2 *porigin, NVEC2 *pvect_u, NVEC2 *pvect_v, const NRECTf32 *porientedrect)
{
	NErrorIf(!porigin, NERROR_NULL_POINTER);
	NErrorIf(!pvect_u, NERROR_NULL_POINTER);
	NErrorIf(!pvect_v, NERROR_NULL_POINTER);

	// Is Oriented Axis Aligned Rect ?
	// 
	//		Axis Aligned Base		CCW90 Base					CCW180 Base					CCW270 Base
	// 		O									  O					      vector V				 vector U
	// 		+----> vector U			vector V <----+							  ^						^	   
	// 		|									  |							  |						|	  
	// 		|									  |							  |						|	  
	// 		v									  v				vector U <----+						+----> vector V 
	//   vector V							   vector U						  O						O	  	 

	porigin->x = porientedrect->left;
	porigin->y = porientedrect->top;

	// Retrieve the AA origin to compare with the incoming origin and conclude the Oriented Base of the incoming RECT ! ...
	Nf32 aatop		= NMIN(porientedrect->top,porientedrect->bottom);
	Nf32 aaleft		= NMIN(porientedrect->left,porientedrect->right);

	if(aatop == porientedrect->top)
	{
		// Axis Aligned Base !
		if(aaleft == porientedrect->left)
		{
			pvect_u->x = porientedrect->right - porientedrect->left;
			pvect_u->y = 0.0f;

			pvect_v->x = 0.0f;
			pvect_v->y = porientedrect->bottom - porientedrect->top;
		}
		// CCW90 Base !
		else
		{
			pvect_u->x = 0.0f;
			pvect_u->y = porientedrect->bottom - porientedrect->top;

			pvect_v->x = porientedrect->right - porientedrect->left;
			pvect_v->y = 0.0f;
		}
	}
	else
	{
		// CCW270 Base !
		if(aaleft == porientedrect->left)
		{
			pvect_u->x = 0.0f;
			pvect_u->y = porientedrect->bottom - porientedrect->top;

			pvect_v->x = porientedrect->right - porientedrect->left;
			pvect_v->y = 0.0f;
		}
		// CCW180 Base !
		else
		{
			pvect_u->x = porientedrect->right - porientedrect->left;
			pvect_u->y = 0.0f;

			pvect_v->x = 0.0f;
			pvect_v->y = porientedrect->bottom - porientedrect->top;
		}
	}
}


// ------------------------------------------------------------------------------------------
// NGetEffectiveTexCoordRect
// ------------------------------------------------------------------------------------------
// Description :
//	Calculate and return a Rectangular area.
//	This calculation 
//	The full rectangular texture area is {0,0,1,1} (upper_left corner,lower_right corner).
// ------------------------------------------------------------------------------------------
// In  :
//		NRECTf32			*poneresult			a valid Ptr on NRECTf32 to store the result.
//												Which will be a "one" rectangle with all coordinates between 0 and 1.
//		const NTEXTURE		*ptexture			a valid texture
//
//		const NRECTs16 *psubscreenrect		a rectangular area into screen coord system (pixel)... most often from an ATLAS description. 
//												It represents a portion of the the texture, so, obviously this portion is smaller than the texture size.
//												... It's a "sub" portion of the texture.
//												if NULL, the full rectangular texture area is {0,0,1,1} is used.
//
//		const NRECTf32		*psubonerect		a relative rectangular area into ONE coord system ( 0 < coords < 1 ).
//												It represents a "sub"rectangular area relative from the subscreenrect 
// Out :		
// ------------------------------------------------------------------------------------------
void NGetEffectiveTexCoordRect( NRECTf32 *poneresult, const NTEXTURE *ptexture, const NRECTs16 *psubscreenrect, const NRECTf32 *psubonerect  )
{
	NErrorIf(!ptexture, NERROR_NULL_POINTER);
	NErrorIf(!psubscreenrect && !psubonerect, NERROR_INCONSISTENT_PARAMETERS);

	Nf32		u,v;
	Nf32		ub,vb;

	// UV values describe into screen coord system
	// ... most often from an ATLAS description. 
	// To compute final UVs coordinates a valid texture has to be already bind with the geometry
	if( psubscreenrect )
	{
		poneresult->upper_left.x	= (Nf32)psubscreenrect->upper_left.x / (Nf32)ptexture->size.x; 
		poneresult->upper_left.y	= (Nf32)psubscreenrect->upper_left.y / (Nf32)ptexture->size.y;

		poneresult->lower_right.x	= (Nf32)psubscreenrect->lower_right.x / (Nf32)ptexture->size.x; 
		poneresult->lower_right.y	= (Nf32)psubscreenrect->lower_right.y / (Nf32)ptexture->size.y;
	}
	else
	{
		// Default values
		poneresult->upper_left.x = 0.0f;
		poneresult->upper_left.y = 0.0f;

		poneresult->lower_right.x = 1.0f; 
		poneresult->lower_right.y = 1.0f;
	}

	// UV values describe into floating coord system
	// because user can ask for using 'scruv' (FLAG_USEPARAM_SCRUV) and 'uv' (FLAG_USEPARAM_UV) at the same time
	// final uvs are the result of the right combination of both parameters.
	if( psubonerect )
	{	
		u	= poneresult->upper_left.x;
		v	= poneresult->upper_left.y;
		ub	= poneresult->lower_right.x - poneresult->upper_left.x;
		vb	= poneresult->lower_right.y - poneresult->upper_left.y;

		poneresult->upper_left.x	= u + psubonerect->upper_left.x * ub;
		poneresult->upper_left.y	= v + psubonerect->upper_left.y * vb;

		poneresult->lower_right.x	= u + psubonerect->lower_right.x * ub;
		poneresult->lower_right.y	= v + psubonerect->lower_right.y * vb; 
	}
}


// ------------------------------------------------------------------------------------------
// NGetEffectiveTexCoordAARectSize
// ------------------------------------------------------------------------------------------
// Description :
//	Calculate and the size (width and height) of the Axis Aligned Effective rectangle of a texture.
//	This calculation 
//	The full rectangular texture effective rectangle is {0,0,1,1} (upper_left corner,lower_right corner).
//	... and its Size is 1,1.
//	!!! This function Consider the effective rect as a PURE Axis Aligned Rectangle.
// ------------------------------------------------------------------------------------------
// In  :
//		NVEC2			*psize				a valid NVEC2 ptr to store the result in.
//		const NTEXTURE		*ptexture			a valid texture
//
//		const NRECTs16 *psubscreenrect		a rectangular area into screen coord system (pixel)... most often from an ATLAS description. 
//												It represents a portion of the the texture, so, obviously this portion is smaller than the texture size.
//												... It's a "sub" portion of the texture.
//												if NULL, the full rectangular texture area {0,0,textureWidth,textureHeight} is used.
//
//		const NRECTf32		*puv_rect			a relative rectangular area into ONE coord system ( 0 < coords < 1 ).
//												It represents a "sub"rectangular area relative from the subscreenrect 
// Out :		
// ------------------------------------------------------------------------------------------
void NGetEffectiveTexCoordAARectSize( NVEC2 *psize, const NTEXTURE *ptexture, const NRECTs16 *pscruv_Rect, const NRECTf32 *puv_rect  )
{
	NRECTf32 result;
	NGetEffectiveTexCoordRect( &result, ptexture, pscruv_Rect, puv_rect  );
	psize->x = NABS( result.lower_right.x - result.upper_left.x );
	psize->y = NABS( result.lower_right.y - result.upper_left.y );
}

// ------------------------------------------------------------------------------------------
// NGetEffectiveTexCoordOAARectSize
// ------------------------------------------------------------------------------------------
// Description :
//	Calculate the size (width and height) of the Effective ORIENTED Axis Aligned rectangle of a texture.
//	The full effective rectangular texture rectangle has a Size of 1,1.
//	!!! This function Consider the effective rect as an ORIENTED Axis Aligned Rectangle.
// ------------------------------------------------------------------------------------------
// In  :
//		NVEC2			*psize				a valid NVEC2 ptr to store the result in.
//		const NTEXTURE		*ptexture			a valid texture
//
//		const NRECTs16 *psubscreenrect		a rectangular area into screen coord system (pixel)... most often from an ATLAS description. 
//												It represents a portion of the the texture, so, obviously this portion is smaller than the texture size.
//												... It's a "sub" portion of the texture.
//												if NULL, the full rectangular texture area {0,0,textureWidth,textureHeight} is used.
//
//		const NRECTf32		*puv_rect			a relative rectangular area into ONE coord system ( 0 < coords < 1 ).
//												It represents a "sub"rectangular area relative from the subscreenrect 
// Out :		
// ------------------------------------------------------------------------------------------
void NGetEffectiveTexCoordOAARectSize( NVEC2 *psize, const NTEXTURE *ptexture, const NRECTs16 *pscruv_Rect, const NRECTf32 *puv_rect  )
{
	NRECTf32 result;

	NGetEffectiveTexCoordRect( &result, ptexture, pscruv_Rect, puv_rect  );

	// For a OAARect, Width (size.x) will be the length of vector U and Height (size.y) length of vector V.
	NVEC2 origin,u,v;
	NGetTexCoordOrthogonal(&origin,&u,&v,&result);
	psize->x = NABS(u.x+u.y); // special case of  "length = sqrt(x*x + y*y)" with "x" OR "y" = ZERO !!
	psize->y = NABS(v.x+v.y); // special case of  "length = sqrt(x*x + y*y)" with "x" OR "y" = ZERO !!
}
// ------------------------------------------------------------------------------------------
// NGetEffectiveTexCoordAARectScreenRelativeSize
// ------------------------------------------------------------------------------------------
// Description :
//	Calculate the size (width and height) of the Axis Aligned Effective rectangle of a texture.
//	Calculated Size will be relative to the screen Size into the Normalized screen system.
// Reminder:	Into the Normalized Screen Base ...
//				Normalized ScreenWidth = width/height
//				Normalized ScreenHeight= height/height = 1
// So The Height of this rectangle will be 1 if it is as height as the screen !
//	!!! This function Consider the effective rect as a PURE Axis Aligned Rectangle.
// ------------------------------------------------------------------------------------------
// In  :
//		NVEC2			*psize				a valid NVEC2 ptr to store the result in.
//		const NTEXTURE		*ptexture			a valid texture
//
//		const NRECTs16 *psubscreenrect		a rectangular area into screen coord system (pixel)... most often from an ATLAS description. 
//												It represents a portion of the the texture, so, obviously this portion is smaller than the texture size.
//												... It's a "sub" portion of the texture.
//												if NULL, the full rectangular texture area {0,0,textureWidth,textureHeight} is used.
//
//		const NRECTf32		*puv_rect			a relative rectangular area into ONE coord system ( 0 < coords < 1 ).
//												It represents a "sub"rectangular area relative from the subscreenrect 
// Out :		
// ------------------------------------------------------------------------------------------
void NGetEffectiveTexCoordAARectScreenRelativeSize( NVEC2 *psize, const NTEXTURE *ptexture, const NRECTs16 *pscruv_Rect, const NRECTf32 *puv_rect  )
{
	NGetEffectiveTexCoordAARectSize( psize,ptexture,pscruv_Rect,puv_rect  );
	psize->x*= NCoord_PixToHRel_X(ptexture->size.x);
	psize->y*= NCoord_PixToHRel_Y(ptexture->size.y);
}
// ------------------------------------------------------------------------------------------
// NGetEffectiveTexCoordOAARectSize
// ------------------------------------------------------------------------------------------
// Description :
// Evaluate the Effective Oriented Axis Aligned Rectangle Size of a texture into the Normalized Screen Base.
// Reminder:	Into the Normalized Screen Base ...
//				Normalized ScreenWidth = width/height
//				Normalized ScreenHeight= height/height = 1
// So The Height of this rectangle will be 1 if it is as height as the screen !
//	!!! This function Consider the effective rect as an ORIENTED Axis Aligned Rectangle.
// ------------------------------------------------------------------------------------------
// In  :
//		NVEC2			*psize				a valid NVEC2 ptr to store the result in.
//		const NTEXTURE		*ptexture			a valid texture
//
//		const NRECTs16 *psubscreenrect		a rectangular area into screen coord system (pixel)... most often from an ATLAS description. 
//												It represents a portion of the the texture, so, obviously this portion is smaller than the texture size.
//												... It's a "sub" portion of the texture.
//												if NULL, the full rectangular texture area {0,0,textureWidth,textureHeight} is used.
//
//		const NRECTf32		*puv_rect			a relative rectangular area into ONE coord system ( 0 < coords < 1 ).
//												It represents a "sub"rectangular area relative from the subscreenrect 
// Out :		
// ------------------------------------------------------------------------------------------
void NGetEffectiveTexCoordOAARectScreenRelativeSize( NVEC2 *psize, const NTEXTURE *ptexture, const NRECTs16 *pscruv_Rect, const NRECTf32 *puv_rect  )
{
	NGetEffectiveTexCoordOAARectSize( psize,ptexture,pscruv_Rect,puv_rect  );
	psize->x*= NCoord_PixToHRel_X(ptexture->size.x);
	psize->y*= NCoord_PixToHRel_Y(ptexture->size.y);
}

*/
