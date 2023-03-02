#include "../../NCStandard.h"
#include "../../Geometry/NGeometry.h"
#include "../../Maths/NMatrix3x3.h"
#include "../../NErrorHandling.h"
#include "../../NUsualCoordinates.h"
#include "../../NErrorHandling.h"

#include "../Maths/NUT_MathsMisc.h"

#include "NUT_DrawPencil.h"
#include "NUT_Draw.h"

extern NGEOMETRY	*NUT_pCurrentDrawableShapeGeometry;
extern NMATRIX3x3	NUT_DrawOrthonormal;

void NUT_Draw_Segment(const NVEC3 *pA, const NVEC3 *pB, const NCOLOR *pcolor)
{
	NUTDRAWVERTEX	v;
	//		A ------- B
	v.Color0_4f		= *pcolor;
	v.Position_3f	= *pA;
	NUT_DrawPencil_From(&v);
	v.Position_3f	= *pB;
	NUT_DrawPencil_LineTo(&v);
}
void NUT_Draw_DoubleSegment(const NVEC3 *pA, const NVEC3 *pB, const Nf32 thickness, const NCOLOR *pcolor)
{

	NVEC3			i, j;			// i and j are base vectors. i is AB vector and j an orthogonal vector of i. !!!
	NUTDRAWVERTEX	v0;
	Nf32			li, lj;		// length of vector i and j ( there are not unit vectors )

	//		 + -------- +
	//		 |			|
	//	   A +			+ B
	//		 |			|
	//		 + -------- +


	// Construction vectors i and j
	NVec3Sub(&i, pB, pA);
	li = NVec3ManhattanLength(&i);
	if (li < NF32_EPSILON_VECTOR_LENGTH) // ALMOST NULL VECTOR !!! ( Manhattan length is enough here to decide that this vector is very small !)
		return;

	// Choose the Z Axis of NUT_Wraw_Orthonormal as "BUILD VECTOR" 
	NVec3CrossProduct(&j, &NUT_DrawOrthonormal.ZAxis, &i);
	lj = NVec3ManhattanLength(&j);
	if (lj < NF32_EPSILON_VECTOR_LENGTH)
	{
		// Choose X Axis instead of ZAxis if 'i' and 'ZAxis' are almost collinear.
		NVec3CrossProduct(&j, &NUT_DrawOrthonormal.XAxis, &i);
		lj = NVec3ManhattanLength(&j);
		NErrorIf(lj < NF32_EPSILON_VECTOR_LENGTH, NERROR_NULL_VECTOR); // That means i is collinear with XAxis ... ( it was already colinear with ZAxis ... there is something WRONG !) 
	}
	NVec3ScaleBy(&j, thickness);
	
	v0.Color0_4f = *pcolor;
	NVec3Add(&v0.Position_3f, pA, &j);				NUT_DrawPencil_From(&v0);	// 0
	NVec3AddTo(&v0.Position_3f, &i);				NUT_DrawPencil_LineTo(&v0);	// 1
	NVec3Sub(&v0.Position_3f, pA, &j);				NUT_DrawPencil_From(&v0);	// 2
	NVec3AddTo(&v0.Position_3f, &i);				NUT_DrawPencil_LineTo(&v0);	// 3
}
void NUT_Draw_Stick(const NVEC3 *pA, const NVEC3 *pB, const Nf32 thickness, const NCOLOR *pcolor)
{

}

void NUT_Draw_Arrows(const NVEC3 *pA, const NVEC3 *pB, const NUT_DRAW_ARROWS *parrows)
{
	NErrorIf(parrows == NULL , NERROR_NULL_VECTOR); // a NUT_DRAW_ARROWS structure must be defined !

	NVEC3		i,j;			// i and j are base vectors. i is AB vector and j an orthogonal vector of i. !!!
	NVEC3		vai,vbi,vaj,vbj;// vectors used to draw Arrowheads (Aarrowhead with vai and vaj and Barrowhead with vbi,vbj) 
	NVEC3		vai_s, vbi_s;	// vectors used to Arrowheads  ( vai_s is vai but with a special size computed to draw a specific shape, same for vbi_s regarding vbi )
	NVEC3		c;				// vector used for computing.

	NUTDRAWVERTEX	v0,v,vb,vc;	
	Nf32			li,lj;		// length of vector i and j ( there are not unit vectors )
	Nf32			ai,aj;		// ratio(i,j)  of A arrowhead to multiply (i,j) and obtain vai,vaj with the whish size
	Nf32			bi,bj;		// ratio(i,j)  of B arrowhead to multiply (i,j) and obtain vbi,vbj with the whish size
	Nf32			lvai;		// length  of A arrowhead
	Nf32			lvbi;		// length  of B arrowhead
	Nu32			istooshort = 0;
	//		+		  +
	//	  .	|         | .
	//A +	+ ------- +   + B
	//	  .	|         | .
	//		+		  +
	
	// Color
	v0.Color0_4f = v.Color0_4f = parrows->Color;

	// Construction vectors i and j
	NVec3Sub(&i,pB,pA);
	li = NVec3SquareLength(&i);
	if( li<NF32_EPSILON_VECTOR_LENGTH ) // ALMOST NULL VECTOR !!! ( even if it's a square length, it's really really SMALL !)
	{
		//NErrorIf(1,NERROR_NULL_VECTOR); 
		istooshort = 1;	// to avoid drawing ticks evenif user asks it 
		NVec3Set(&i,1.0f,0.0f,0.0f); // Set i to 1,0,0 to draw arrows even if AB vector (==i) is almost NULL vector 
		li = 1.0f;
	}
	// Choose the Z Axis of NUT_Wraw_Orthonormal as "BUILD VECTOR" 
	NVec3CrossProduct(&j, &NUT_DrawOrthonormal.ZAxis,&i);
	lj = NVec3SquareLength(&j);
	if( lj<NF32_EPSILON_VECTOR_LENGTH )
	{
		// Choose X Axis instead of ZAxis if 'i' and 'ZAxis' are almost collinear.
		NVec3CrossProduct(&j, &NUT_DrawOrthonormal.XAxis,&i);
		lj = NVec3SquareLength(&j);
		NErrorIf( lj<NF32_EPSILON_VECTOR_LENGTH,NERROR_NULL_VECTOR); // That means i is collinear with XAxis ... ( it was already colinear with ZAxis ... there is something WRONG !) 
	}
	
	// Finalize 'li' and 'lj' pre-calculation only if it's necessary (1 or 2 ABSOLUTE ARROW SIZE to draw)!
	if( parrows->Flags&(FLAG_NUT_DRAW_ARROWS_ABSOLUTE_AARROWSIZE|FLAG_NUT_DRAW_ARROWS_ABSOLUTE_BARROWSIZE|FLAG_NUT_DRAW_ARROWS_ABSOLUTE_UNITSIZE) ) // That means if, at least one of these flags is ON ...
	{
		li	=	sqrt(li);	// Because square length is already calculated !!!!
		lj	=	sqrt(lj);	// Because square length is already calculated !!!!
	}

	// "ai" and "aj" are set to ZERO only if there is "NO A SHAPE". ( like this we avoid the creation of one point ... )
	// We don't doing the same in the case of NO B SHAPE, because B SHAPE is drawn the first and the case is handled inside the switch case bellow.
	if ( ((parrows->Flags)&MASK_SHIFTED_NUT_DRAW_ARROWS_SHAPE_A) == FLAGS_NUT_DRAW_ARROWS_ASHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_NONE))
	{
		ai = 0.0f;	lvai = 0.0f;
		aj = 0.0f;
	}
	else
	{
		if (ISFLAG_ON(parrows->Flags, FLAG_NUT_DRAW_ARROWS_ABSOLUTE_AARROWSIZE))
		{
			ai = parrows->AArrowSize/li;			lvai = parrows->AArrowSize;
			aj = 0.5f*parrows->AArrowSize/lj;
		}
		else
		{
			ai = parrows->AArrowSize;				lvai = parrows->AArrowSize * li;
			aj = 0.5f*parrows->AArrowSize;
		}
	}

	if( ISFLAG_ON(parrows->Flags, FLAG_NUT_DRAW_ARROWS_ABSOLUTE_BARROWSIZE) )
	{
		bi	=	parrows->BArrowSize/li;				lvbi = parrows->BArrowSize;
		bj	=	0.5f*parrows->BArrowSize/lj; 
	}
	else
	{
		bi	=	parrows->BArrowSize;				lvbi = parrows->BArrowSize * li;
		bj	=	0.5f*parrows->BArrowSize; 
	}


	// setup vai,vaj and vbi,vbj the building vectors of A and B arrowheads.
	// notice that i and j are NOT normalized vectors ! But, normalization is included, if necessary, inside ai,aj,bi,bj... ( like this :" ai = ??/li " where li is the length of i )
	NVec3Scale(&vai,&i,ai);
	NVec3Scale(&vaj,&j,aj);
	
	NVec3Scale(&vbi,&i,bi);
	NVec3Scale(&vbj,&j,bj);

	// DRAWING BLUEPRINTS:
	// ( 'heads' are drawn 'inside' segment [AB] whatever the head shapes )
	//		+		  +
	//	  .	|         | .
	//A +	+ ------- +   + B
	//	  .	|         | .
	//		+		  +
	
	// DRAWING ORDER:
	//		6		  1
	//	  .	|         | .
	//	7  5/9-------0/4  2
	//	  .	|         | .
	//		8		  3
	

	switch ((parrows->Flags)&MASK_SHIFTED_NUT_DRAW_ARROWS_SHAPE_B)
	{
		case FLAGS_NUT_DRAW_ARROWS_BSHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_NONE):
			// DRAWING ORDER:
			//			 
			//	         
			//	1--------.------0
			//	         
			//			 
			v0.Position_3f = *pB;								NUT_DrawPencil_From(&v0);	// 0
			NVec3Add(&v0.Position_3f, pA, &vai);				NUT_DrawPencil_LineTo(&v0);	// 1
			break;
		case FLAGS_NUT_DRAW_ARROWS_BSHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_SPACE):
			// DRAWING ORDER:
			//			 
			//	         
			//	1--------0
			//	         
			//	
			NVec3Sub(&v0.Position_3f, pB, &vbi);				NUT_DrawPencil_From(&v0);	// 0
			NVec3Add(&v0.Position_3f, pA, &vai);				NUT_DrawPencil_LineTo(&v0);	// 1
			break;
		case FLAGS_NUT_DRAW_ARROWS_BSHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_STICK):
			// DRAWING ORDER:
			//					0
			//	         		|
			//	3--------.------2
			//	         		|
			//					1
			NVec3Add(&v0.Position_3f, pB, &vbj);				NUT_DrawPencil_From(&v0);	// 0
			NVec3Sub(&v0.Position_3f, pB, &vbj);				NUT_DrawPencil_LineTo(&v0);	// 1
			v0.Position_3f = *pB;								NUT_DrawPencil_From(&v0);	// 2
			NVec3Add(&v0.Position_3f, pA, &vai);				NUT_DrawPencil_LineTo(&v0);	// 3
			break;
		case FLAGS_NUT_DRAW_ARROWS_BSHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_SQUARE):
			// DRAWING ORDER:
			//			 1------2
			//	         |		|
			//	6-------0/5		|
			//	         |		|
			//			 4------3
			NVec3Sub(&v0.Position_3f, pB, &vbi);
																NUT_DrawPencil_From(&v0);	// 0
			NVec3Add(&v.Position_3f, &v0.Position_3f, &vbj);	NUT_DrawPencil_LineTo(&v);	// 1		 
			NVec3AddTo(&v.Position_3f, &vbi);					NUT_DrawPencil_LineTo(&v);	// 2	
			NVec3Sub(&v.Position_3f, pB, &vbj);					NUT_DrawPencil_LineTo(&v);	// 3
			NVec3SubFrom(&v.Position_3f, &vbi);					NUT_DrawPencil_LineTo(&v);	// 4
																NUT_DrawPencil_LineTo(&v0);	// 5
			NVec3Add(&v0.Position_3f, pA, &vai);				NUT_DrawPencil_LineTo(&v0);	// 6
			break;
		case FLAGS_NUT_DRAW_ARROWS_BSHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_PAUSE):
			vb.Color0_4f = parrows->Color;
			// DRAWING ORDER:
			//			6--7  3-0/4
			//	        |  |  |  |
			//	11-----5/10|  |  |
			//	        |  |  |  |
			//			9--8  2--1
			NVec3Scale(&vbi_s, &vbi, 1.0f/3.0f);

			NVec3Add(&v0.Position_3f, pB, &vbj);				NUT_DrawPencil_From(&v0);	// 0
			NVec3Sub(&v.Position_3f, pB, &vbj);					NUT_DrawPencil_LineTo(&v);	// 1
			NVec3SubFrom(&v.Position_3f, &vbi_s);				NUT_DrawPencil_LineTo(&v);	// 2
			NVec3Sub(&v.Position_3f, &v0.Position_3f, &vbi_s);	NUT_DrawPencil_LineTo(&v);	// 3
																NUT_DrawPencil_LineTo(&v0);	// 4

			NVec3Sub(&v0.Position_3f, pB, &vbi);				NUT_DrawPencil_From(&v0);	// 5
			NVec3Add(&v.Position_3f, &v0.Position_3f, &vbj);	NUT_DrawPencil_LineTo(&v);	// 6
			NVec3AddTo(&v.Position_3f, &vbi_s);					NUT_DrawPencil_LineTo(&v);	// 7	
			NVec3Sub(&vb.Position_3f, &v0.Position_3f, &vbj);								// . 9
			NVec3Add(&v.Position_3f, &vb.Position_3f, &vbi_s);	NUT_DrawPencil_LineTo(&v);	// 8	
																NUT_DrawPencil_LineTo(&vb);	// 9	
																NUT_DrawPencil_LineTo(&v0);	// 10	
			NVec3Add(&v0.Position_3f, pA, &vai);				NUT_DrawPencil_LineTo(&v0);	// 11
			break;
		case FLAGS_NUT_DRAW_ARROWS_BSHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_HALFSQUARE):
			// DRAWING ORDER:
			//				3-0/4
			//				|  |
			//	 6----------5  |
			//				|  |
			//				2--1
			NVec3Scale(&vbi_s, &vbi, 1.0f / 2.0f);

			NVec3Add(&v0.Position_3f, pB, &vbj);				NUT_DrawPencil_From(&v0);	// 0
			NVec3Sub(&v.Position_3f, pB, &vbj);					NUT_DrawPencil_LineTo(&v);	// 1
			NVec3SubFrom(&v.Position_3f, &vbi_s);				NUT_DrawPencil_LineTo(&v);	// 2
			NVec3Sub(&v.Position_3f, &v0.Position_3f, &vbi_s);	NUT_DrawPencil_LineTo(&v);	// 3
																NUT_DrawPencil_LineTo(&v0);	// 4

			NVec3Sub(&v0.Position_3f, pB, &vbi_s);				NUT_DrawPencil_From(&v0);	// 5
			NVec3Add(&v0.Position_3f, pA, &vai);				NUT_DrawPencil_LineTo(&v0);	// 6
			break;

		case FLAGS_NUT_DRAW_ARROWS_BSHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_FORWARD):
			// DRAWING ORDER:
			//			 1
			//	         |  .
			//	5-------0/4   2
			//	         |  .
			//			 3
			NVec3Sub(&v0.Position_3f, pB, &vbi);
																NUT_DrawPencil_From(&v0);	// 0
			NVec3Add(&v.Position_3f, &v0.Position_3f, &vbj);	NUT_DrawPencil_LineTo(&v);	// 1		 
			v.Position_3f = *pB;								NUT_DrawPencil_LineTo(&v);	// 2	
			NVec3Sub(&v.Position_3f, &v0.Position_3f, &vbj);	NUT_DrawPencil_LineTo(&v);	// 3
																NUT_DrawPencil_LineTo(&v0);	// 4
			NVec3Add(&v0.Position_3f, pA, &vai);				NUT_DrawPencil_LineTo(&v0);	// 5
			break;

		case FLAGS_NUT_DRAW_ARROWS_BSHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_FASTFORWARD):
			vb.Color0_4f = parrows->Color;
			// DRAWING ORDER:
			//			 1	  3
			//	         | .  | .
			//	9-------0/8  2/6  4
			//	         | .  | .
			//			 7	  5
			NVec3Scale(&vbi_s, &vbi, 1.0f / 2.0f);
			NVec3Sub(&v0.Position_3f, pB, &vbi);				NUT_DrawPencil_From(&v0);	// 0
			NVec3Add(&vb.Position_3f,&v0.Position_3f, &vbj);	NUT_DrawPencil_LineTo(&vb);	// 1
			NVec3Sub(&v.Position_3f, pB, &vbi_s);				NUT_DrawPencil_LineTo(&v);	// 2
			NVec3AddTo(&vb.Position_3f,  &vbi_s);				NUT_DrawPencil_LineTo(&vb);	// 3
			vb.Position_3f = *pB;								NUT_DrawPencil_LineTo(&vb);	// 4	
			NVec3Sub(&vb.Position_3f, &v.Position_3f, &vbj);	NUT_DrawPencil_LineTo(&vb);	// 5
																NUT_DrawPencil_LineTo(&v);	// 6			
			NVec3Sub(&vb.Position_3f, &v0.Position_3f, &vbj);	NUT_DrawPencil_LineTo(&vb);	// 7
																NUT_DrawPencil_LineTo(&v0);	// 8
			NVec3Add(&v0.Position_3f, pA, &vai);				NUT_DrawPencil_LineTo(&v0);	// 9
			break;
		case FLAGS_NUT_DRAW_ARROWS_BSHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_FASTBACKWARD):
			vb.Color0_4f = parrows->Color;
			vc.Color0_4f = parrows->Color;
			// DRAWING ORDER:
			//				 1	  3
			//	          .  | .  |	
			//	8 ------0/7 2/5   |
			//	          .  | .  |
			//				 6	  4
			NVec3Scale(&vbi_s, &vbi, 1.0f / 2.0f);
			NVec3Sub(&v0.Position_3f, pB, &vbi);				NUT_DrawPencil_From(&v0);	// 0
			NVec3Add(&v.Position_3f,&v0.Position_3f, &vbi_s);
			NVec3Add(&vb.Position_3f, &v.Position_3f, &vbj);	NUT_DrawPencil_LineTo(&vb);	// 1
																NUT_DrawPencil_LineTo(&v);	// 2
			NVec3AddTo(&vb.Position_3f,  &vbi_s);				NUT_DrawPencil_LineTo(&vb);	// 3

			NVec3Sub(&vc.Position_3f, &v.Position_3f, &vbj);	// (6)
			NVec3Add(&vb.Position_3f, &vc.Position_3f, &vbi_s);	NUT_DrawPencil_LineTo(&vb);	// 4
																NUT_DrawPencil_LineTo(&v);	// 5																							
																NUT_DrawPencil_LineTo(&vc);	// 6
																NUT_DrawPencil_LineTo(&v0);	// 7
			NVec3Add(&v0.Position_3f, pA, &vai);				NUT_DrawPencil_LineTo(&v0);	// 9
			break;
		case FLAGS_NUT_DRAW_ARROWS_BSHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_DIAMOND):
			// DRAWING ORDER:
			//				 1
			//	           .   .
			//	5--------0/4     2
			//	           .   .
			//				 3
			NVec3Scale(&vbi_s, &vbi, 1.0f / 2.0f);
			NVec3Sub(&v0.Position_3f, pB, &vbi);				NUT_DrawPencil_From(&v0);	// 0
			NVec3Add(&v.Position_3f, &v0.Position_3f, &vbi_s);	
			NVec3AddTo(&v.Position_3f, &vbj);					NUT_DrawPencil_LineTo(&v);	// 1	
			v.Position_3f = *pB;								NUT_DrawPencil_LineTo(&v);	// 2	
			NVec3SubFrom(&v.Position_3f,  &vbi_s);				
			NVec3SubFrom(&v.Position_3f, &vbj);					NUT_DrawPencil_LineTo(&v);	// 3	
																NUT_DrawPencil_LineTo(&v0);	// 4
			NVec3Add(&v0.Position_3f, pA, &vai);				NUT_DrawPencil_LineTo(&v0);	// 5
			break;
		case FLAGS_NUT_DRAW_ARROWS_BSHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_CROSS):
			// DRAWING ORDER:
				//			 3     0
				//	          .   .
				//	5-----------4   
				//	          .   .
				//			 1     2
			NVec3Scale(&vbi_s, &vbi, 0.5f);

			NVec3Add(&v0.Position_3f, pB, &vbj);				NUT_DrawPencil_From(&v0);	// 0					
			NVec3Sub(&v.Position_3f, pB, &vbi);
			NVec3SubFrom(&v.Position_3f, &vbj);					NUT_DrawPencil_LineTo(&v);	// 1
			NVec3AddTo(&v.Position_3f, &vbi);					NUT_DrawPencil_From(&v);	// 2
			NVec3Sub(&v.Position_3f, &v0.Position_3f, &vbi);	NUT_DrawPencil_LineTo(&v);	// 3					

			NVec3Sub(&v0.Position_3f, pB, &vbi_s);				NUT_DrawPencil_From(&v0);	// 4					
			NVec3Add(&v0.Position_3f, pA, &vai);				NUT_DrawPencil_LineTo(&v0);	// 5		
			break;
		case FLAGS_NUT_DRAW_ARROWS_BSHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_CIRCLE):
			// DRAWING ORDER:
			//			    . . .
			//	          .	      .
			//	16------0/15	   .	
			//	          .		  .
			//			    . . .
			NVec3Scale(&vbi_s, &vbi,0.5f);

			NVec3Sub(&c, pB, &vbi_s);
			NVec3Sub(&v0.Position_3f, pB, &vbi);				NUT_DrawPencil_From(&v0);	// 0
			for (Nu32 angle = NANGLE180; angle < (NANGLELUT_SIZE+ NANGLE180); angle += CONSTANT_NUT_DRAW_ARROWS_CIRCLE_ANGLESTEP)// == 256 that means 16 segments circle
			{
				NFastCosSin(angle, &ai, &aj);
				v.Position_3f.x = c.x + vbi_s.x * ai + vbj.x * aj;
				v.Position_3f.y = c.y + vbi_s.y * ai + vbj.y * aj;
				v.Position_3f.z = c.z + vbi_s.z * ai + vbj.z * aj;
																NUT_DrawPencil_LineTo(&v);	// ... 14
			}
																NUT_DrawPencil_LineTo(&v0);	// 15
			NVec3Add(&v0.Position_3f, pA, &vai);				NUT_DrawPencil_LineTo(&v0);	// 16
			break;
		case FLAGS_NUT_DRAW_ARROWS_BSHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_CIRCLESHIFTED):
			// DRAWING ORDER:
			//			    . . .
			//	          .	      .
			//	16------0/15	   .	
			//	          .		  .
			//			    . . .
			NVec3Scale(&vbi_s, &vbi, 0.5f);

			c = *pB;
			NVec3Sub(&v0.Position_3f, pB, &vbi_s);				NUT_DrawPencil_From(&v0);	// 0
			for (Nu32 angle = NANGLE180; angle < (NANGLELUT_SIZE + NANGLE180); angle += CONSTANT_NUT_DRAW_ARROWS_CIRCLE_ANGLESTEP)// == 256 that means 16 segments circle
			{
				NFastCosSin(angle, &ai, &aj);
				v.Position_3f.x = c.x + vbi_s.x * ai + vbj.x * aj;
				v.Position_3f.y = c.y + vbi_s.y * ai + vbj.y * aj;
				v.Position_3f.z = c.z + vbi_s.z * ai + vbj.z * aj;
				NUT_DrawPencil_LineTo(&v);	// ... 14
			}
			NUT_DrawPencil_LineTo(&v0);	// 15
			NVec3Add(&v0.Position_3f, pA, &vai);				NUT_DrawPencil_LineTo(&v0);	// 16
			break;


		case FLAGS_NUT_DRAW_ARROWS_BSHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_BACKWARD):
			// DRAWING ORDER:
			//				 1
			//	           . |
			//	4-------0/3  |
			//	           . |
			//				 2
			NVec3Sub(&v0.Position_3f, pB, &vbi);
																NUT_DrawPencil_From(&v0);	// 0
			NVec3Add(&v.Position_3f, &v0.Position_3f, &vbi);		
			NVec3AddTo(&v.Position_3f, &vbj);					NUT_DrawPencil_LineTo(&v);	// 1
			NVec3Sub(&v.Position_3f, pB, &vbj);					NUT_DrawPencil_LineTo(&v);	// 2
																NUT_DrawPencil_LineTo(&v0);	// 3	
			NVec3Add(&v0.Position_3f, pA, &vai);				NUT_DrawPencil_LineTo(&v0);	// 4
			break;
	}


	// ! Assume that V0 is already setup by the A SHAPE DRAWING PROCESS !
	switch ((parrows->Flags)&MASK_SHIFTED_NUT_DRAW_ARROWS_SHAPE_A)
	{
		case FLAGS_NUT_DRAW_ARROWS_ASHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_NONE):
			// Nothing to do thanks to the previous test "if ( ((parrows->Flags)&MASK_SHIFTED_NUT_DRAW_ARROWS_SHAPE_A) == FLAGS_NUT_DRAW_ARROWS_ASHAPE_NONE)" made some lines earlier, ai and aj are NULL
			// ... so the B SHAPE already drawing made the JOB !
			break;
		case FLAGS_NUT_DRAW_ARROWS_ASHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_SPACE):
			// Nothing to do.
			// ... the B SHAPE already drawing made the JOB !
			break;
		case FLAGS_NUT_DRAW_ARROWS_ASHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_STICK):
			// DRAWING ORDER:
			//	3
			//	|
			//	1---0
			//	|
			//	2
			v0.Position_3f = *pA;								NUT_DrawPencil_LineTo(&v0);	// 1
			NVec3Sub(&v.Position_3f, &v0.Position_3f, &vaj);	NUT_DrawPencil_LineTo(&v);	// 2
			NVec3Add(&v.Position_3f, &v0.Position_3f, &vaj);	NUT_DrawPencil_LineTo(&v);	// 3
		case FLAGS_NUT_DRAW_ARROWS_ASHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_SQUARE):
			// DRAWING ORDER:
			//	2-------1
			//	|		|
			//	|	   0/5
			//	|		|
			//	3-------4
			
			NVec3Add(&v.Position_3f, &v0.Position_3f, &vaj);	NUT_DrawPencil_LineTo(&v);	// 1
			NVec3Add(&v.Position_3f, pA, &vaj);					NUT_DrawPencil_LineTo(&v);	// 2		 
			NVec3Sub(&v.Position_3f, pA, &vaj);					NUT_DrawPencil_LineTo(&v);	// 3	
			NVec3Sub(&v.Position_3f, &v0.Position_3f,&vaj);		NUT_DrawPencil_LineTo(&v);	// 4
																NUT_DrawPencil_LineTo(&v0);	// 5
			break;
		case FLAGS_NUT_DRAW_ARROWS_ASHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_PAUSE):
			vb.Color0_4f = parrows->Color;
			vc.Color0_4f = parrows->Color;

			// DRAWING ORDER:
			//	8--7  2-1/5
			//	|  |  |  | 
			//	+  |  |	 +---
			//	|  |  |  | 
			// 5/9-6  3--4
			NVec3Scale(&vai_s, &vai, 1.0f / 3.0f);

			NVec3Add(&vc.Position_3f, &v0.Position_3f, &vaj);	NUT_DrawPencil_LineTo(&vc);	// 1
			NVec3Sub(&v.Position_3f, &vc.Position_3f, &vai_s);	NUT_DrawPencil_LineTo(&v);	// 2
			NVec3Sub(&vb.Position_3f, &v0.Position_3f, &vaj);	// (4)
			NVec3Sub(&v.Position_3f, &vb.Position_3f, &vai_s);	NUT_DrawPencil_LineTo(&v);	// 3
																NUT_DrawPencil_LineTo(&vb);	// 4
																NUT_DrawPencil_LineTo(&vc);	// 5


			NVec3Sub(&v0.Position_3f, pA, &vaj);				NUT_DrawPencil_From(&v0);	// 5
			NVec3Add(&v.Position_3f, &v0.Position_3f, &vai_s);	NUT_DrawPencil_LineTo(&v);	// 6
			NVec3Add(&vb.Position_3f, pA, &vaj);				//(8)
			NVec3Add(&v.Position_3f, &vb.Position_3f, &vai_s);	NUT_DrawPencil_LineTo(&v);	// 7
																NUT_DrawPencil_LineTo(&vb);	// 8
																NUT_DrawPencil_LineTo(&v0);	// 9
			break;
		case FLAGS_NUT_DRAW_ARROWS_ASHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_HALFSQUARE):
			NVec3Scale(&vai_s, &vai, 0.5f);
			// DRAWING ORDER:
			//	3---2
			//	|	|
			//	|  1/6---
			//	|	|
			//	4---5
			NVec3Add(&v0.Position_3f, pA, &vai_s);				NUT_DrawPencil_LineTo(&v0);	// 1
			NVec3Add(&v.Position_3f, &v0.Position_3f, &vaj);	NUT_DrawPencil_LineTo(&v);	// 1
			NVec3Add(&v.Position_3f, pA, &vaj);					NUT_DrawPencil_LineTo(&v);	// 2		 
			NVec3Sub(&v.Position_3f, pA, &vaj);					NUT_DrawPencil_LineTo(&v);	// 3	
			NVec3Sub(&v.Position_3f, &v0.Position_3f, &vaj);	NUT_DrawPencil_LineTo(&v);	// 4
																NUT_DrawPencil_LineTo(&v0);	// 5
			break;
		case FLAGS_NUT_DRAW_ARROWS_ASHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_FORWARD):
			// DRAWING ORDER:
			//	 1
			//	 |  .
			//	 |  0/3----
			//	 |  .
			//	 2
			NVec3Add(&v.Position_3f, pA, &vaj);					NUT_DrawPencil_LineTo(&v);	// 1
			NVec3Sub(&v.Position_3f, pA, &vaj);					NUT_DrawPencil_LineTo(&v);	// 2		 
																NUT_DrawPencil_LineTo(&v0);	// 3
			break;
		case FLAGS_NUT_DRAW_ARROWS_ASHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_FASTFORWARD):
			vb.Color0_4f = parrows->Color;
			// DRAWING ORDER:
			//			 3	  1
			//	         | .  | .
			//			 |	 2/5  7-----
			//	         | .  | .
			//			 4	  6
			NVec3Scale(&vai_s, &vai, 1.0f / 2.0f);

			NVec3Add(&vb.Position_3f, pA, &vai_s);				//(2/5)
			NVec3Add(&v.Position_3f, &vb.Position_3f, &vaj);	NUT_DrawPencil_LineTo(&v);	// 1
																NUT_DrawPencil_LineTo(&vb);	// 2
			NVec3Add(&v.Position_3f, pA, &vaj);					NUT_DrawPencil_LineTo(&v);	// 3
			NVec3Sub(&v.Position_3f, pA, &vaj);					NUT_DrawPencil_LineTo(&v);	// 4
																NUT_DrawPencil_LineTo(&vb);	// 5	
			NVec3Sub(&v.Position_3f, &vb.Position_3f, &vaj);	NUT_DrawPencil_LineTo(&v);	// 6
																NUT_DrawPencil_LineTo(&v0);	// 7
			break;
		case FLAGS_NUT_DRAW_ARROWS_ASHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_FASTBACKWARD):
			vb.Color0_4f = parrows->Color;
			vc.Color0_4f = parrows->Color;
			// DRAWING ORDER:
			//		 3	  1
			//	  .  | .  |	
			//	4   2/6   8-------
			//	  .  | .  |
			//		 5	  7
			NVec3Scale(&vai_s, &vai, 1.0f / 2.0f);
	
			NVec3Add(&vb.Position_3f, pA, &vai_s);				//(2/6)
			NVec3Add(&v.Position_3f, &v0.Position_3f, &vaj);	NUT_DrawPencil_LineTo(&v);	// 1
																NUT_DrawPencil_LineTo(&vb);	// 2
			NVec3Add(&v.Position_3f,&vb.Position_3f, &vaj);		NUT_DrawPencil_LineTo(&v);	// 3
			v.Position_3f = *pA;								NUT_DrawPencil_LineTo(&v);	// 4
			NVec3Sub(&v.Position_3f, &vb.Position_3f, &vaj);	NUT_DrawPencil_LineTo(&v);	// 5
																NUT_DrawPencil_LineTo(&vb);	// 6
			NVec3Sub(&v.Position_3f, &v0.Position_3f, &vaj);	NUT_DrawPencil_LineTo(&v);	// 7
																NUT_DrawPencil_LineTo(&v0);	// 8
			break;

		case FLAGS_NUT_DRAW_ARROWS_ASHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_DIAMOND):
			vb.Color0_4f = parrows->Color;
			// DRAWING ORDER:
			//		 1
			//	   .   .
			//	2       0/4
			//	   .   .
			//		 3
			NVec3Scale(&vai_s, &vai, 0.5f);
			NVec3Add(&v.Position_3f, pA, &vai_s);				
			NVec3Add(&vb.Position_3f, &v.Position_3f, &vaj);	NUT_DrawPencil_LineTo(&vb);	// 1	
			vb.Position_3f = *pA;								NUT_DrawPencil_LineTo(&vb);	// 2	
			NVec3Sub(&vb.Position_3f, &v.Position_3f, &vaj);	NUT_DrawPencil_LineTo(&vb);	// 3	
																NUT_DrawPencil_LineTo(&v0);	// 4		
			break;
		case FLAGS_NUT_DRAW_ARROWS_ASHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_CROSS):
			vb.Color0_4f = parrows->Color;
			// DRAWING ORDER:
			//	 3     5
			//	  .   .
			//		1--0 
			//	  .   .
			//	 4     2
			NVec3Scale(&vai_s, &vai, 0.5f);

			NVec3Add(&v.Position_3f, pA, &vai_s);				NUT_DrawPencil_LineTo(&v);	// 1					
			NVec3Sub(&vb.Position_3f, &v0.Position_3f, &vaj);	NUT_DrawPencil_LineTo(&vb);	// 2
			NVec3Add(&vb.Position_3f, pA,&vaj);					NUT_DrawPencil_LineTo(&vb);	// 3
			NVec3Sub(&v.Position_3f, pA, &vaj);					NUT_DrawPencil_From(&v);	// 4
			NVec3Add(&v.Position_3f, &v0.Position_3f, &vaj);	NUT_DrawPencil_LineTo(&v);	// 5					
			break;
		case FLAGS_NUT_DRAW_ARROWS_ASHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_CIRCLE):
			// DRAWING ORDER:
			//		    . . .
			//	      .	      .
			//	     .		   0/15	
			//	      .		  .
			//		    . . .
			NVec3Scale(&vai_s, &vai, 0.5f);

			NVec3Add(&c, pA, &vai_s);
			for (Nu32 angle = 0; angle < NANGLELUT_SIZE; angle += CONSTANT_NUT_DRAW_ARROWS_CIRCLE_ANGLESTEP)// == 256 that means 16 segments circle
			{
				NFastCosSin(angle, &ai, &aj);
				v.Position_3f.x = c.x + vai_s.x * ai + vaj.x * aj;
				v.Position_3f.y = c.y + vai_s.y * ai + vaj.y * aj;
				v.Position_3f.z = c.z + vai_s.z * ai + vaj.z * aj;
				NUT_DrawPencil_LineTo(&v);	// ... 14
			}
			NUT_DrawPencil_LineTo(&v0);	// 15
			break;
		case FLAGS_NUT_DRAW_ARROWS_ASHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_CIRCLESHIFTED):
			// DRAWING ORDER:
			//		    . . .
			//	      .	      .
			//	     .		   1/16 ----0	
			//	      .		  .
			//		    . . .
			NVec3Scale(&vai_s, &vai, 0.5f);
			NVec3Add(&v0.Position_3f, pA, &vai_s);
			NUT_DrawPencil_LineTo(&v0);		//1
			c = *pA;
			for (Nu32 angle = 0; angle < NANGLELUT_SIZE; angle += CONSTANT_NUT_DRAW_ARROWS_CIRCLE_ANGLESTEP)// == 256 that means 16 segments circle
			{
				NFastCosSin(angle, &ai, &aj);
				v.Position_3f.x = c.x + vai_s.x * ai + vaj.x * aj;
				v.Position_3f.y = c.y + vai_s.y * ai + vaj.y * aj;
				v.Position_3f.z = c.z + vai_s.z * ai + vaj.z * aj;
				NUT_DrawPencil_LineTo(&v);	// ... 15
			}
			NUT_DrawPencil_LineTo(&v0);	// 16
			break;
		case FLAGS_NUT_DRAW_ARROWS_ASHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_BACKWARD):
			// DRAWING ORDER:
			//		 1
			//	   . |
			//	2    4----
			//	   . |
			//		 3
			NVec3Add(&v.Position_3f, &v0.Position_3f, &vaj);	NUT_DrawPencil_LineTo(&v);	// 1
			v.Position_3f=*pA;									NUT_DrawPencil_LineTo(&v);	// 2			
			NVec3Sub(&v.Position_3f, &v0.Position_3f, &vaj);	NUT_DrawPencil_LineTo(&v);	// 3
																NUT_DrawPencil_LineTo(&v0);	// 4
			break;
	}

	// TICKS ?
	if ( ((parrows->Flags >> SHIFT_NUT_DRAW_ARROWS_TICK)&MASK_NUT_DRAW_ARROWS_TICK) == (BITS_NUT_DRAW_ARROWS_TICK_NONE | istooshort) )
		return;

	NVEC3f32	vti, vtja,vtjb;		// vectors used to draw ticks
	Nf32		ti, tj;				// ratio(i, j)  to multiply(i, j) and obtain vti, vtja and vtjb with the whish size
	Nf32		lvti;				// length of vti vector

	Nu32		nbticks,tick0,tick;
		
	// use v and vc and c (already allocated on stack) to draw each tick as a segment [v,vc] with vc = v + c
		
	// A) Define the length of ONE UNIT ( the segment between 2 ticks )
	// -----------------------------------------------------------------
	if (ISFLAG_ON(parrows->Flags, FLAG_NUT_DRAW_ARROWS_ABSOLUTE_UNITSIZE))
	{
		ti		= parrows->Unit/li;		// Operation NVec3Scale(&vti,&i,ti) is going to give us the vti vector which is colinear with i and with a length of "parrows->Unit"	(li is the length of i)
		lvti	= parrows->Unit;
		// Notes:
		// if (ISFLAG_ON(parrows->Flags, FLAG_NUT_DRAW_ARROWS_FIRST_TICK_ON_A)) has no influence here, because UNITSIZE is absolute ! 
		// So, no matter the position of the first tick, "ti"( the used length of ONE UNIT ) remains the same.
	}
	else
	{
		//FLAG_NUT_DRAW_ARROWS_ABSOLUTE_UNITSIZE is OFF so, UNITSIZE is supposed to be relative ! 
		// relative ... ? Relative to the length of the segment used to spread ticks on.
		// 
		// So there are 2 possibilities:
		//		Ticks are spread on the whole segment AB (default case )	............... FLAG_NUT_DRAW_ARROWS_SPREAD_TICKS_IN_BETWEEN is OFF
		//		Ticks are spread on the segment which is in between the  Arrowheads........ FLAG_NUT_DRAW_ARROWS_SPREAD_TICKS_IN_BETWEEN is ON
		//		UnitSize is going to be calculated against the right reference size
		if (ISFLAG_ON(parrows->Flags, FLAG_NUT_DRAW_ARROWS_SPREAD_TICKS_IN_BETWEEN))
		{
			// Reference size is smaller than the whole AB segment, is the part in between Arrowheads.
			// the part between the 2 arrowheads has a length of  (li - lvai - lvbi) , with		li		length of i
			//																					lvai	length of vai
			//																					lvbi	length of vbi
			ti = parrows->Unit*(li - lvai - lvbi)/li;		// Operation NVec3Scale(&vti,&i,ti) is going to give us the vti vector which is colinear with i and with a length of "parrows->Unit * (li-lvai-lvbi)"	(li is the length of i which is NOT a unit vector)
			lvti = parrows->Unit*(li - lvai - lvbi);
		}
		else
		{
			// Reference size is the whole AB segment
			ti = parrows->Unit;			// Operation NVec3Scale(&vti,&i,ti) is going to give us the vti vector which is colinear with i and with a length of "parrows->Unit * li"	(li is the length of i)
			lvti = parrows->Unit*li;
		}
	}

	NErrorIf(ti <= 0.0f || lvti <= 0.0f, NERROR_UNEXPECTED_RESULT);
	//if (ti <= 0.0f)
	//	return;

	NVec3Scale(&vti, &i, ti);


	// B)  Define the base length of ONE TICK
	// ---------------------------------------
	// Retrieve the 2bits number (0,1,2 or 3 ) representing the length of ticks.
	// 1) retrieve the value: n = ( parrows->Flags >> SHIFT_NUT_DRAW_ARROWS_TICK_SIZE ) & MASK_NUT_DRAW_ARROWS_TICK_SIZE
	// 2) use it as a shifting on the first bit to obtain a power of two  p = (2^n)
	// 3) transform p in a 32 bits float number and use it to define a simple fraction: f = 1.0f/(Nf32)p
	// 4) use f as a ratio of the unitsize to define tick j size. tj = f*unitsize
	// So, in a compact form:
	// tj = parrows->Unit  / ( (Nf32)(1 << ((parrows->Flags >> SHIFT_NUT_DRAW_ARROWS_TICK_SIZE)&MASK_NUT_DRAW_ARROWS_TICK_SIZE)) * lj );... lj is here to retrieve the unit vector from i ( i is not unit at this stage ! )

	if(ISFLAG_ON(parrows->Flags, FLAG_NUT_DRAW_ARROWS_TICK_SIZE_RELATIVE_TO_UNIT))
		tj = lvti / ((Nf32)(1 << ((parrows->Flags&MASK_SHIFTED_NUT_DRAW_ARROWS_TICK_SIZE) >> (SHIFT_NUT_DRAW_ARROWS_TICK_SIZE)))*lj);	// lj is here to retrieve the unit vector from j ( j is not unit at this stage ! )
	else																																
		tj = lvbi / ((Nf32)(1 << ((parrows->Flags&MASK_SHIFTED_NUT_DRAW_ARROWS_TICK_SIZE) >> (SHIFT_NUT_DRAW_ARROWS_TICK_SIZE)))*lj);	// relative to B arrowhead size. 
																																			
	switch (parrows->Flags&MASK_SHIFTED_NUT_DRAW_ARROWS_TICK)
	{
		case FLAGS_NUT_DRAW_ARROWS_TICK(BITS_NUT_DRAW_ARROWS_TICK_RIGHT):
			vtja.x = vtja.y = vtja.z = 0.0f;
			NVec3Scale(&vtjb, &j, -tj);
			break;

		case FLAGS_NUT_DRAW_ARROWS_TICK(BITS_NUT_DRAW_ARROWS_TICK_LEFT):
			vtja.x = vtja.y = vtja.z = 0.0f;
			NVec3Scale(&vtjb, &j, tj);
			break;

		case FLAGS_NUT_DRAW_ARROWS_TICK(BITS_NUT_DRAW_ARROWS_TICK_CENTERED):
			NVec3Scale(&vtja, &j, -tj*0.5f);
			NVec3Scale(&vtjb, &j,  tj*0.5f);
			break;
		default:
			NErrorIf(1, NERROR_INVALID_CASE);
			break;
	}

	// Define step to respect on screen tick density
	NErrorIf(!GET_NUT_DRAW_ARROWS_TICK_DENSITY_MAX(parrows->Flags), NERROR_NULL_VALUE);
	Nf32 f = (Nf32)GET_NUT_DRAW_ARROWS_TICK_DENSITY_MAX(parrows->Flags);
	Nu32 step = (Nu32)ceilf(1.0f / (lvti*f));

	// C) Define the origin (c) of TICKS drawing and the TICKS spread
	// ---------------------------------------------------------------


	// spread Ticks middle of "drawing segment" to both direction A side and B Side
	if (ISFLAG_ON(parrows->Flags, FLAG_NUT_DRAW_ARROWS_SPREAD_TICKS_FROM_CENTER))
	{
		if (ISFLAG_ON(parrows->Flags, FLAG_NUT_DRAW_ARROWS_SPREAD_TICKS_IN_BETWEEN))
		{
			//tick0 = 0;								// The first tick is supposed to be drawn.( the first tick is at the middle of "in between segment" )
			f = (li - lvai - lvbi);						// use the length of the "in between segment" as reference length.
			NVec3Scale(&vai_s, &i, f / (2.0f*li));		// compute "vai_s" a vector colinear with i ( so with vai too ) and with a length of f/2 (remember... 'i' has a length of li ! it's not a vector unit. )
			NVec3Add(&c, pA, &vai_s);					// ticks spread starts from the middle of the "drawing segment"
				
			f = (li - lvai - lvbi) / 2.0f*lvti;

			if (f < 1.0f) // ... if 'f' is negative that means STRANGE config. and if 'f' is between 0 and 1, that means nbticks will be 0 thanks to the floor(f) function !!!
				return;
			else
				nbticks = (Nu32)floor(f);
		}
		else
		{
			//tick0 = 0;								// The first tick is supposed to be drawn. ( the first tick is at the middle of AB segment )
			NVec3Scale(&vai_s, &i, 0.5f);				// compute "vai_s" a vector colinear with i ( so with vai too ) and with a length of li/2 (remember... 'i' has a length of li ! it's not a vector unit. )
			NVec3Add(&c, pA, &vai_s);					// ticks spread starts from the middle of the "AB segment"
				
			f = (li/2.0f - NMAX(lvbi,lvai)) / lvti;		// NMAX(li - lvbi) because we don't want to draw ticks "inside" A or B Arrowhead, and we need to have the same number of ticks on each side of the middle, ...
														// ... so we keep the largest Arrowhead size to compute this number of ticks
														// ...and '/lvti' to figure out the number of unit segments are "inside" this half drawing length.
			if (f < 1.0f)								// because, if f is negative that means STANGE config. and if f is between 0 and 1, that means nbticks will be 0 !!!
				return;
			else
				nbticks = (Nu32)floor(f);
		}

	
		// Draw All Ticks from drawing segment middle !
		for (tick = 0; tick <= nbticks; tick += step)
		{
			// from center to pB
			v.Position_3f.x = c.x + (Nf32)tick * vti.x + vtja.x;
			v.Position_3f.y = c.y + (Nf32)tick * vti.y + vtja.y;
			v.Position_3f.z = c.z + (Nf32)tick * vti.z + vtja.z;
			NUT_DrawPencil_From(&v);

			v.Position_3f.x = c.x + (Nf32)tick * vti.x + vtjb.x;
			v.Position_3f.y = c.y + (Nf32)tick * vti.y + vtjb.y;
			v.Position_3f.z = c.z + (Nf32)tick * vti.z + vtjb.z;
			NUT_DrawPencil_LineTo(&v);

			// from center to pA
			v.Position_3f.x = c.x - (Nf32)tick * vti.x + vtja.x;
			v.Position_3f.y = c.y - (Nf32)tick * vti.y + vtja.y;
			v.Position_3f.z = c.z - (Nf32)tick * vti.z + vtja.z;
			NUT_DrawPencil_From(&v);

			v.Position_3f.x = c.x - (Nf32)tick * vti.x + vtjb.x;
			v.Position_3f.y = c.y - (Nf32)tick * vti.y + vtjb.y;
			v.Position_3f.z = c.z - (Nf32)tick * vti.z + vtjb.z;
			NUT_DrawPencil_LineTo(&v);

		}
	}
	// Draw Ticks from A side to B side
	else
	{
		if (ISFLAG_ON(parrows->Flags, FLAG_NUT_DRAW_ARROWS_SPREAD_TICKS_IN_BETWEEN))
		{
			NVec3Add(&c, pA, &vai);		// ticks spread starts "after" the A Arrowhead
			tick0 = 0;					// The first tick is going to be drawn.

			f = (li - lvai - lvbi) / lvti;

			if (f < 1.0f) // ... if 'f' is negative that means STRANGE config. and if 'f' is between 0 and 1, that means nbticks will be 0 thanks to the floor(f) function !!!
				return;
			else
				nbticks = (Nu32)floor(f);
		}
		else
		{
			c = *pA;					// ticks spread startsat A point
			f = ceilf(lvai / lvti);	// evenif spread is calculated to start at A, Engine avoids to draw the ticks which are "inside" the A ArrowHead.
										//			.
			if (f == 0.0f)				//			.
				tick0 = 0;				//			.
			else if (f < 0.0f)			//			.
				return;					//			.
			else						//			.
				tick0 = (Nu32)f;		// 	So, tick0 may be > 0. In that way all the ticks with an id < tick0 are not going to be drawn.

			f = (li - lvbi) / lvti;		// (li - lvbi) because we don't want to draw ticks "inside" B Arrowhead... and '/lvti' to figure out the number of unit segments are "inside" the drawing length.
			if (f < 1.0f)				// because, if f is negative that means STANGE config. and if f is between 0 and 1, that means nbticks will be 0 !!!
				return;
			else
				nbticks = (Nu32)floor(f);
		}
			


		// Draw All Ticks from A Side  to B Side!
		for (tick = tick0; tick <= nbticks; tick+=step)
		{
			v.Position_3f.x = c.x + (Nf32)tick * vti.x + vtja.x;
			v.Position_3f.y = c.y + (Nf32)tick * vti.y + vtja.y;
			v.Position_3f.z = c.z + (Nf32)tick * vti.z + vtja.z;
			NUT_DrawPencil_From(&v);

			v.Position_3f.x = c.x + (Nf32)tick * vti.x + vtjb.x;
			v.Position_3f.y = c.y + (Nf32)tick * vti.y + vtjb.y;
			v.Position_3f.z = c.z + (Nf32)tick * vti.z + vtjb.z;
			NUT_DrawPencil_LineTo(&v);
		}
	}
	
}
/*
void NUT_Draw_Axis(const NVEC3 *pA, const NVEC3 *pB, const NUT_DRAW_AXIS *pparam )
{

	NVEC3			i, j;
	Nf32			li, lj;
	NUTDRAWVERTEX	v;
	Nu32			nbgrad;
	Nf32			length, lengtharrows;
	Nf32			gradstart = 0.0f;

	// Construction vectors i and j
	NVec3Sub(&i, pB, pA);
	li = NVec3SquareLength(&i);
	if (li < NF32_EPSILON_VECTOR_LENGTH) // ALMOST NULL VECTOR !!! ( even if it's a square length, it's really really SMALL !)
	{
		NErrorIf(1, NERROR_NULL_VECTOR);
		NVec3Set(&i, 1.0f, 0.0f, 0.0f); // Set i to 1,0,0 to draw arrows even if AB vector (==i) is almost NULL vector 
		li = 1.0f;
	}
	// Choose the Z Axis of NUT_Wraw_Orthonormal as "BUILD VECTOR" 
	NVec3CrossProduct(&j, &NUT_DrawOrthonormal.ZAxis, &i);
	lj = NVec3SquareLength(&j);
	if (lj < NF32_EPSILON_VECTOR_LENGTH)
	{
		// Choose X Axis instead of ZAxis if 'i' and 'ZAxis' are almost collinear.
		NVec3CrossProduct(&j, &NUT_DrawOrthonormal.XAxis, &i);
		lj = NVec3SquareLength(&j);
		NErrorIf(lj < NF32_EPSILON_VECTOR_LENGTH, NERROR_NULL_VECTOR); // That means i is collinear with XAxis ... ( it was already colinear with ZAxis ... there is something WRONG !) 
	}

	length = sqrt(li);

	if (pparam)
	{
		li = pparam->Unit / length;									// Because square length of i is already calculated !!!!
		lj = pparam->GraduationExtend * pparam->Unit / sqrt(lj);	// Because square length of j is already calculated !!!!
	}
	else
	{
		li = 1.0f / length;											// Because square length of i is already calculated !!!!
		lj = DEFAULT_NUT_DRAW_AXIS_GRADUATIONEXTEND / sqrt(lj);			// Because square length of j is already calculated !!!!
	}
	
	// DRAW AXIS MAIN ARROW 
	if (pparam && ISFLAG_ON(pparam->Flags, FLAG_NUT_DRAW_AXIS_USEPARAM_ARROWPARAM))
	{
		NUT_Draw_Arrows(pA, pB, &pparam->ArrowParam);

		v.Color0_4f = pparam->ArrowParam.Color; // ... for the graduations along the axis
		
		lengtharrows = 0.0f; gradstart = 0.0f;
		if ((pparam->ArrowParam.Flags&MASK_SHIFTED_NUT_DRAW_ARROWS_SHAPE_A)!= FLAGS_NUT_DRAW_ARROWS_ASHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_NONE))
		{
			if( ISFLAG_ON(pparam->ArrowParam.Flags,FLAG_NUT_DRAW_ARROWS_ABSOLUTE_AARROWSIZE) )
				gradstart = ceilf(pparam->ArrowParam.AArrowSize);
			else	
				gradstart = ceilf(pparam->ArrowParam.AArrowSize*length);
		}
		
		if ((pparam->ArrowParam.Flags&MASK_SHIFTED_NUT_DRAW_ARROWS_SHAPE_B) != FLAGS_NUT_DRAW_ARROWS_BSHAPE(BITS_NUT_DRAW_ARROWS_SHAPE_NONE))
		{
			if (ISFLAG_ON(pparam->ArrowParam.Flags, FLAG_NUT_DRAW_ARROWS_ABSOLUTE_BARROWSIZE))
				lengtharrows = pparam->ArrowParam.BArrowSize;
			else
				lengtharrows = pparam->ArrowParam.BArrowSize*length;
		}

		length = length - lengtharrows - gradstart;
		
		if (length > 0.0f)
			nbgrad = (Nu32)floorf(length / pparam->Unit);
		else
			nbgrad = 0;
	}
	else
	{
		NUT_DRAW_ARROWS	default_arrow;
		NSetColorf(&default_arrow.Color, NCOLOR_PRESET3F_WHITE, 1.0f);
		default_arrow.AArrowSize = 0.0f;	// is not going to be drawn
		default_arrow.BArrowSize = DEFAUL_NUT_DRAW_AXIS_ARROW_SIZE;
		NUT_SetDrawArrows_ShapeA(&default_arrow, BITS_NUT_DRAW_ARROWS_SHAPE_NONE);
		NUT_SetDrawArrows_ShapeB(&default_arrow, BITS_NUT_DRAW_ARROWS_SHAPE_FORWARD);

		NUT_Draw_Arrows(pA, pB, &default_arrow);

		NSetColorf(&v.Color0_4f, NCOLOR_PRESET3F_WHITE, 1.0f); // ... for the graduations along the axis

		nbgrad = (Nu32)floorf( length *(1.0f - DEFAUL_NUT_DRAW_AXIS_ARROW_SIZE) );
	}


	// Graduation sticks
	for (Nu32 s = 0; s < nbgrad; s++)
	{
		v.Position_3f.x = pA->x + (s + gradstart) * li * i.x + lj * j.x;
		v.Position_3f.y = pA->y + (s + gradstart) * li * i.y + lj * j.y;
		v.Position_3f.z = pA->z + (s + gradstart) * li * i.z + lj * j.z;
		NUT_DrawPencil_From(&v);

		v.Position_3f.x = pA->x + (s + gradstart) * li * i.x;// -lj * j.x;
		v.Position_3f.y = pA->y + (s + gradstart) * li * i.y;// -lj * j.y;
		v.Position_3f.z = pA->z + (s + gradstart) * li * i.z;// -lj * j.z;
		NUT_DrawPencil_LineTo(&v);
	}

}
*/


