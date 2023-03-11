#include "lib/N/NCStandard.h"
#include "lib/N/Geometry/NGeometry.h"
#include "../../NErrorHandling.h"
#include "lib/N/Maths/NMatrix3x3.h"
#include "NUT_DrawPencil.h"
#include "NUT_Draw.h"

extern NGEOMETRY			*NUT_pCurrentDrawableShapeGeometry;
extern NGEOMETRY			NUT_DrawableShapeGeometry[]; // 0 means 3D, 1 means ORTHO
extern NMATRIX3x3			NUT_DrawOrthonormal;


/*
static inline void _draw_ellipse_simple(const NUT_DRAW_ELLIPSE *pellipse, const NVEC3 *pi, const NVEC3 *pj )
{
	Nu32			angle;
	NVEC3		icos,jsin;
	Nf32			fcos,fsin;
	NUTDRAWVERTEX	v0,vb;

	v0.Color0_4f = vb.Color0_4f = pellipse->Color;
	

	NVec3Add(&v0.Position_3f,&pellipse->Center,pi);
	NUT_DrawPencil_From(&v0);
	for(angle=0;angle<NANGLELUT_SIZE;angle+=pellipse->Accuracy)
	{
		NFastCosSin(angle,&fcos,&fsin);
		NVec3Scale(&icos,pi,fcos);
		NVec3Scale(&jsin,pj,fsin);
		vb.Position_3f.x = pellipse->Center.x + icos.x + jsin.x;
		vb.Position_3f.y = pellipse->Center.y + icos.y + jsin.y;
		vb.Position_3f.z = pellipse->Center.z + icos.z + jsin.z;
		NUT_DrawPencil_LineTo(&vb);
	}
	NUT_DrawPencil_LineTo(&v0);
}
static inline void _draw_ellipse_dashed(const NUT_DRAW_ELLIPSE *pellipse, const NVEC3 *pi, const NVEC3 *pj )
{
	Nu32				angle;
	NVEC3			icos,jsin;
	Nf32				fcos,fsin;
	NUTDRAWVERTEX		v0,vb;
	Nu32				dash = 0;

	v0.Color0_4f = vb.Color0_4f = pellipse->Color;

	NVec3Add(&v0.Position_3f,&pellipse->Center,pi);
	NUT_DrawPencil_From(&v0);
	for(angle=0;angle<NANGLELUT_SIZE;angle+=pellipse->Accuracy)
	{
		NFastCosSin(angle,&fcos,&fsin);
		NVec3Scale(&icos,pi,fcos);
		NVec3Scale(&jsin,pj,fsin);
		vb.Position_3f.x = pellipse->Center.x + icos.x + jsin.x;
		vb.Position_3f.y = pellipse->Center.y + icos.y + jsin.y;
		vb.Position_3f.z = pellipse->Center.z + icos.z + jsin.z;

		dash ^= 1;
		if(dash)
			NUT_DrawPencil_LineTo(&vb);
		else
			NUT_DrawPencil_From(&vb);
	}
	NUT_DrawPencil_LineTo(&v0);
}
static inline void _draw_markings(const NUT_DRAW_ELLIPSE *pellipse, const NVEC3 *pi, const NVEC3 *pj )
{
	Nf32			fout = 1.0f + (Nf32)pellipse->MarkingOutSize/255;
	Nf32			fin	 = 1.0f - (Nf32)pellipse->MarkingInSize/255;
	Nu32			angle;
	NVEC3		i0,j0,i1,j1;
	NVEC3		i0cos,j0sin;
	NVEC3		i1cos,j1sin;
	Nf32			fcos,fsin;
	NUTDRAWVERTEX	v0,v1;
	
	v0.Color0_4f = v1.Color0_4f = pellipse->Color;

	// bases
	NVec3Scale(&i0,pi,fin);	NVec3Scale(&j0,pj,fin);
	NVec3Scale(&i1,pi,fout);	NVec3Scale(&j1,pj,fout);

	for(angle=0;angle<NANGLELUT_SIZE;angle+=pellipse->MarkingsAccuracy)
	{
		NFastCosSin(angle,&fcos,&fsin);

		NVec3Scale(&i0cos,&i0,fcos);
		NVec3Scale(&j0sin,&j0,fsin);
		v0.Position_3f.x = pellipse->Center.x + i0cos.x + j0sin.x;
		v0.Position_3f.y = pellipse->Center.y + i0cos.y + j0sin.y;
		v0.Position_3f.z = pellipse->Center.z + i0cos.z + j0sin.z;
		NVec3Scale(&i1cos,&i1,fcos);
		NVec3Scale(&j1sin,&j1,fsin);
		v1.Position_3f.x = pellipse->Center.x + i1cos.x + j1sin.x;
		v1.Position_3f.y = pellipse->Center.y + i1cos.y + j1sin.y;
		v1.Position_3f.z = pellipse->Center.z + i1cos.z + j1sin.z;
		NUT_DrawPencil_From(&v0);
		NUT_DrawPencil_LineTo(&v1);
	}
}

*/

void NUT_Draw_Ellipse(const NUT_DRAW_ELLIPSE *pellipse)
{
	NVEC3			i,j,i0,j0;
	NUTDRAWVERTEX		v0,vb;
	NVEC3			icos,jsin;
	Nf32				fcos,fsin;
	Nf32				angledeg;
	Nf32				anglestep;
	Nu32				a;
	Nu16				nb;

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + COMMON PART of all possible Ellipse Drawing procedures
	// +
	// + --- + I,J 
	NErrorIf((pellipse->Extents.x < 0.0f)||(pellipse->Extents.y < 0.0f), NERROR_INVALID_EXTENT );
	NErrorIf(!pellipse->SliceNb, NERROR_NULL_VALUE );
	NErrorIf(ISFLAG_ON(pellipse->Flags,FLAG_NUT_DRAW_ELLIPSE_MARKINGS) && !pellipse->MarkingNb , NERROR_INCONSISTENT_PARAMETERS );
	NErrorIf(ISFLAG_ON(pellipse->Flags,FLAG_NUT_DRAW_ELLIPSE_MARKINGS) && !pellipse->MarkingInSize && !pellipse->MarkingOutSize , NERROR_INCONSISTENT_PARAMETERS );
	NErrorIf(ISFLAG_ON(pellipse->Flags,FLAG_NUT_DRAW_ELLIPSE_OPEN) && pellipse->FromAngle==pellipse->ToAngle, NERROR_INCONSISTENT_PARAMETERS );
	
	NVec3Scale(&i,&NUT_DrawOrthonormal.Side, pellipse->Extents.x);
	NVec3Scale(&j,&NUT_DrawOrthonormal.Up,	pellipse->Extents.y);
	
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + Drawing Procs (Vertex Creation)
	// +
	// Calling several different loops according with the Ellipse Flags
	// This way to avoid having tests inside the drawing main loop !
	v0.Color0_4f = vb.Color0_4f = pellipse->Color;
	if( ISFLAG_OFF(pellipse->Flags,FLAG_NUT_DRAW_ELLIPSE_MARKINGS_ONLY) )
	{
		// + --- + From,To,Step
		if( ISFLAG_ON(pellipse->Flags,FLAG_NUT_DRAW_ELLIPSE_OPEN) )
		{
			angledeg = pellipse->FromAngle;
			// Angular sector
			anglestep = pellipse->ToAngle - angledeg;
			if( anglestep <= 0.0f ) // that means FromAngle > ToAngle !
				anglestep += 360.0f;

			anglestep/=(Nf32)pellipse->SliceNb;		
			nb = pellipse->SliceNb;// pellipse->SliceNb + 1 are going to be created, but the first one outside the loop
		}
		else
		{
			angledeg = 0.0f;
			anglestep = 360.0f/(Nf32)pellipse->SliceNb;
			nb = pellipse->SliceNb-1; // pellipse->SliceNb are going to be created, but the first one outside the loop
		}
		
		// First Vertex (Available for DASHED and SIMPLE Ellipse)
		NFastCosSin(NDEGtoANGLE(angledeg),&fcos,&fsin);
		NVec3Scale(&icos,&i,fcos);
		NVec3Scale(&jsin,&j,fsin);
		v0.Position_3f.x = pellipse->Center.x + icos.x + jsin.x;
		v0.Position_3f.y = pellipse->Center.y + icos.y + jsin.y;
		v0.Position_3f.z = pellipse->Center.z + icos.z + jsin.z;
		NUT_DrawPencil_From(&v0);
		angledeg += anglestep;
		
		if(  ISFLAG_ON(pellipse->Flags,FLAG_NUT_DRAW_ELLIPSE_DASHLINE) )
		{
			//_draw_ellipse_dashed( pellipse, &i, &j );
			Nu32 dash = 0;

			for(a=nb;a!=0;a--,angledeg+=anglestep)
			{
				NFastCosSin(NDEGtoANGLE(angledeg),&fcos,&fsin);
				NVec3Scale(&icos,&i,fcos);
				NVec3Scale(&jsin,&j,fsin);
				vb.Position_3f.x = pellipse->Center.x + icos.x + jsin.x;
				vb.Position_3f.y = pellipse->Center.y + icos.y + jsin.y;
				vb.Position_3f.z = pellipse->Center.z + icos.z + jsin.z;

				dash ^= 1;
				if(dash)
					NUT_DrawPencil_LineTo(&vb);
				else
					NUT_DrawPencil_From(&vb);
			}

			if( ISFLAG_OFF(pellipse->Flags,FLAG_NUT_DRAW_ELLIPSE_OPEN) )
				NUT_DrawPencil_LineTo(&v0);
		}
		else
		{
			//_draw_ellipse_simple( pellipse, &i, &j );
			for(a=nb;a!=0;a--,angledeg+=anglestep)
			{
				NFastCosSin(NDEGtoANGLE(angledeg),&fcos,&fsin);
				NVec3Scale(&icos,&i,fcos);
				NVec3Scale(&jsin,&j,fsin);
				vb.Position_3f.x = pellipse->Center.x + icos.x + jsin.x;
				vb.Position_3f.y = pellipse->Center.y + icos.y + jsin.y;
				vb.Position_3f.z = pellipse->Center.z + icos.z + jsin.z;
				NUT_DrawPencil_LineTo(&vb);
			}
			if( ISFLAG_OFF(pellipse->Flags,FLAG_NUT_DRAW_ELLIPSE_OPEN) )
				NUT_DrawPencil_LineTo(&v0);
		}
	}

	if( ISFLAG_ON(pellipse->Flags,FLAG_NUT_DRAW_ELLIPSE_MARKINGS) )
	{
		//_draw_markings( pellipse, &i, &j );
		Nf32			fout = 1.0f + (Nf32)pellipse->MarkingOutSize/255;
		Nf32			fin	 = 1.0f - (Nf32)pellipse->MarkingInSize/255;
		NVEC3		i1,j1;
		NVEC3		i1cos,j1sin;

		// bases
		NVec3Scale(&i0,&i,fin);	NVec3Scale(&j0,&j,fin);
		NVec3Scale(&i1,&i,fout);	NVec3Scale(&j1,&j,fout);
		
		// + --- + From,To,Step
		if( ISFLAG_ON(pellipse->Flags,FLAG_NUT_DRAW_ELLIPSE_OPEN) )
		{
			angledeg = pellipse->FromAngle;
			// Angular sector
			anglestep = pellipse->ToAngle - angledeg;
			if( anglestep <= 0.0f ) // that means FromAngle > ToAngle !
				anglestep += 360.0f;

			anglestep/=(Nf32)(pellipse->MarkingNb-1);		
			nb = pellipse->MarkingNb;
		}
		else
		{
			angledeg = 0.0f;
			anglestep = 360.0f/(Nf32)pellipse->MarkingNb;
			nb = pellipse->MarkingNb;
		}

		for(a=nb;a!=0;a--,angledeg+=anglestep)
		{
			NFastCosSin(NDEGtoANGLE(angledeg),&fcos,&fsin);

			NVec3Scale(&icos,&i0,fcos);
			NVec3Scale(&jsin,&j0,fsin);
			v0.Position_3f.x = pellipse->Center.x + icos.x + jsin.x;
			v0.Position_3f.y = pellipse->Center.y + icos.y + jsin.y;
			v0.Position_3f.z = pellipse->Center.z + icos.z + jsin.z;
			NVec3Scale(&i1cos,&i1,fcos);
			NVec3Scale(&j1sin,&j1,fsin);
			vb.Position_3f.x = pellipse->Center.x + i1cos.x + j1sin.x;
			vb.Position_3f.y = pellipse->Center.y + i1cos.y + j1sin.y;
			vb.Position_3f.z = pellipse->Center.z + i1cos.z + j1sin.z;
			NUT_DrawPencil_From(&v0);
			NUT_DrawPencil_LineTo(&vb);
		}

	}
}

