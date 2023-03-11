#include "lib/N/NCStandard.h"
#include "lib/N/Geometry/NGeometry.h"
#include "../../NErrorHandling.h"
#include "lib/N/NUsualCoordinates.h"

#include "lib/N/Maths/NMatrix3x3.h"
#include "lib/N/Maths/NUT_MathsMisc.h"

#include "NUT_DrawPencil.h"
#include "NUT_Draw.h"


void NUT_Draw_Spring(const NUT_DRAW_SPRING *pspring)
{
	NVEC3		i,j,halfj,up,down,dir;
	NUTDRAWVERTEX	v;
	Nu32			s;
	Nf32			li;

	v.Color0_4f	= pspring->Color;

	//	      /\    /\    /\			|
	// A +---/  \  /  \  /  \---+ B		|- Thickness
	//	         \/    \/				|
	//		   ------	   ------ 	
	//	 ------ 	 ------	
	//
	//	 Begin Coil1 Coil2 	End






	// Construction vectors i and j
	NVec3Sub(&i,&pspring->B,&pspring->A);
	li = NVec3SquareLength(&i);
	if( li<NF32_EPSILON_VECTOR_LENGTH ) // ALMOST NULL VECTOR !!! ( even if it's a square length, it's really really SMALL !)
	{
		// i is going to stay as small as it be ! (we don't care !)
		NVec3Set(&j,_WORLD_YAXIS_3f);
	}
	else
	{
		if(ISFLAG_ON(pspring->Flags,FLAG_NUT_DRAW_SPRING_USEPARAM_BUILDVECTOR))
		{
			// A user defined Vector should not be close to the NULL VECTOR !!!
			NErrorIf( NVec3SquareLength(&pspring->BuildVector)<NF32_EPSILON_VECTOR_LENGTH,NERROR_NULL_VECTOR);
			NVec3CrossProduct(&j,&pspring->BuildVector,&i);
		}
		else
		{
// 			NUT_HughesMollerChooseUp(&dir,&i);
// 			NVec3CrossProduct(&j,&dir,&i);

			// That means User let the function choose the right BUILD VECTOR ( it should be WORLD Z Axis to have the arrow build into a "vertical" plane)
			NVec3Set(&dir,_WORLD_ZAXIS_3f);
			NVec3CrossProduct(&j,&dir,&i);
			if(NVec3SquareLength(&j)<NF32_EPSILON_VECTOR_LENGTH)
			{
				// Choose X Axis instead of ZAxis if 'i' and 'ZAxis' are almost collinear.
				NVec3Set(&dir,_WORLD_XAXIS_3f);
				NVec3CrossProduct(&j,&dir,&i);
				NErrorIf( NVec3SquareLength(&j)<NF32_EPSILON_VECTOR_LENGTH,NERROR_NULL_VECTOR); // That means i is collinear with XAxis ... ( it was already colinear with ZAxis ... there is something WRONG !) 
			}


		}
	}

	// Notes:
	// Each coil need 2 "segments" + 2 more "segments" for the beginning and 2 more "segments" for End
	NVec3ScaleBy( &i,1.0f/(Nf32)(pspring->SpringCoil*2 + 4) );// +4 for beginning(+2) and End(+2)
	NVec3ScaleBy( &j, pspring->Thickness/NVec3Length(&j) );
	NVec3Scale( &halfj,&j,0.5f);
	
	// Start
	v.Position_3f = pspring->A;		
	NUT_DrawPencil_From(&v);
	
	NVec3AddTo(&v.Position_3f,&i);		
	NUT_DrawPencil_LineTo(&v);
	
	v.Position_3f.x += i.x + halfj.x;
	v.Position_3f.y += i.y + halfj.y;
	v.Position_3f.z += i.z + halfj.z;
	NUT_DrawPencil_LineTo(&v);

	// Coils
	NVec3Add(&up,&i,&j);
	NVec3Sub(&down,&i,&j);
	for( s=pspring->SpringCoil;s!=0;s-- )
	{
		NVec3AddTo(&v.Position_3f,&down);
		NUT_DrawPencil_LineTo(&v);
		NVec3AddTo(&v.Position_3f,&up);	
		NUT_DrawPencil_LineTo(&v);
	}
	// End
	v.Position_3f.x += i.x - halfj.x;
	v.Position_3f.y += i.y - halfj.y;
	v.Position_3f.z += i.z - halfj.z;
	NUT_DrawPencil_LineTo(&v);
	
	v.Position_3f = pspring->B;
	NUT_DrawPencil_LineTo(&v);
}

