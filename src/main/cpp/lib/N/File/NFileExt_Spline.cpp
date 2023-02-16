#include "../NCStandard.h"
#include "../NType.h"
#include "../NSpline.h"
#include "../File/NFile.h"
#include "../File/NFileExt.h"
#include "../NErrorHandling.h"

// ------------------------------------------------------------------------------------------
// NSPLINE* NFileWriteSpline
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
//		pfile				A valid pointer on NFILE
//		pspline				A valid pointer on a "spline" to write
//		write_splineknot	An optional call back function called to write each SplineKnot array element 
//		userdata			a user value pass as the third parameter of the "write_splineknot" function at each call
// Out :
//		Incoming "pspline" pointer on success
//		NULL on Error ( but "pspline" still available )
// ------------------------------------------------------------------------------------------
Nu32 NFileWriteSpline(NFILE *pfile,const NSPLINE *pspline, NARRAY_RWELEMENT_CALLBACK write_splineknot, Nu32 userdata )
{
	NHEADER_SPLINE	header;

	// Write Current version Tag 
	if(NFileWriteVersionTag( pfile, VERSION_NHEADER_SPLINE ) != 1)
		return 0;

	memset(&header,0,sizeof(NHEADER_SPLINE));
	header.Flags			= pspline->Flags;
	header.BezierAccuracy= pspline->BezierAccuracy;
	if( NFileWrite(pfile,&header,sizeof(NHEADER_SPLINE),1) != 1)
		return 0;
	if( !NFileWriteArray(pfile,&pspline->KnotArray,write_splineknot,userdata) )
		return 0;

	return 1;
}


// ------------------------------------------------------------------------------------------
// NSPLINE* NFileReadSpline
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
//		pfile				A valid pointer on NFILE
//		pspline				A valid pointer on a "empty spline" to be filled up by the NFileReadSpline function [pspline should be = NCreateSpline(0,0,NULL,NULL);]	
//		read_splineknot		An optional call back function called to read each SplineKnot array element 
//		userdata			A user value pass as the third parameter of the "read_splineknot" function at each call
// Out :
//		Incoming "pspline" pointer on success
//		NULL on Error.	"pspline" still available and, even if its content is uncertain regarding what it was suppose to read,
//						it still AVAILABLE with the number of spline knot effectively read inside. ( pspline->KnotArray.Size = number of read Spline Knot)
//						IT CAN ALWAYS BE CLEARED WITH "NClearSpline".
// ------------------------------------------------------------------------------------------
Nu32 NFileReadSpline(NFILE *pfile,NSPLINE *pspline, NARRAY_RWELEMENT_CALLBACK read_splineknot, Nu32 userdata )
{
	NErrorIf(!pfile || !pspline, NERROR_NULL_POINTER);
	// The incoming Spline as to be "empty", that means without any SplineKnot.
	// But let "NFileReadArray" check that later for us ...

	NHEADER_SPLINE	header;
	Nu32			version;

	// Check spline version.
	if( NFileReadVersionTag(pfile,&version) != 1 )
		return 0;

	switch( NGETVERSION_MAIN(version) )
	{
		// Current main version (alls versions 0.0.x):
		case NGETVERSION_MAIN( VERSION_NHEADER_SPLINE ):
			memset(&header,0,sizeof(NHEADER_SPLINE));
			if( NFileRead(pfile,&header,sizeof(NHEADER_SPLINE),1) != 1 )
				return 0;
			
			pspline->Flags		= header.Flags;
			pspline->BezierAccuracy	= header.BezierAccuracy;
			if(!NFileReadArray(pfile,&pspline->KnotArray,read_splineknot,userdata))
			{
				// No call to NClearArray because we don't have "_clear_splineknot" call back we need for this specific spline
				return 0;
			}
			return 1;

		// Unknown version:
		default:
			NErrorIf(1, NERROR_FILE_UNKNOWN_VERSIONTAG);
			return 0;
	}

//	return 1;
}
