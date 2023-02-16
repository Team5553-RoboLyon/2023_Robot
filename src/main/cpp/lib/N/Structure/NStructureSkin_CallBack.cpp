
#include "../NCStandard.h"
#include "../NType.h"
#include "../Containers/NNode.h"
#include "../NStructure.h"
#include "../NCamera.h"

/*
#ifdef _DEBUG
#include "../Geometry/NGeometryMisc.h"
#include "../Utilities/Maths/NUT_MathsMisc.h"
#include "../Utilities/Draw/NUT_Draw.h"
#endif
*/

extern NCAMERA				NCamera;
//#include "..\..\SN\Snks_GameEditor.h"

//	|     |     |     |     
//	|     |     |     |     
//	|     |     |     |     
//	+---  +---  +---  +--- 
//	0     1     2    n-1    
//	|     |     |     |     
//	|     |     |     |     

//	Reminder: the last joint used by the skin, which is the second joint used by the last vertebra 
//	is used, even if there are no vertex builded directly from it.

void	NUpdateSkin_Rope_Quads_DoubleRows(NSKIN* pskin)
{
	NVEC3		/*vect,*/vb;
	NVEC3		*pvprev;	
	NSKINVERTEX		*pvertex;
	NCONSTRAINT		*pconstraint;
	Nu32			i;

	Nf32			bulgeshifting;
//	Nf32			*pfsidea,*pfsideb;
	NSKIN_ROPE_VERTEBRA	*pvertebra;

	pvertex = pskin->Rope.pFirstVertex;
	pconstraint = (NCONSTRAINT*)pskin->Rope.pFirst;
	pvprev = &pconstraint->AB;
	pvertebra = (NSKIN_ROPE_VERTEBRA*)pskin->Rope.VertebrasArray.pFirst;
	for(i=pskin->Rope.RangeSize;i!=0;i--,pvprev = &pconstraint->AB,pconstraint++,pvertebra++)
	{
		// Calculate the bulgeshifting amount
		if(pvertebra->Bulge == 0.0f)
		{
			bulgeshifting = 0.0f; 
		}
		else
		{
			vb.x = pconstraint->AB.y;
			vb.y = -pconstraint->AB.x;
			vb.z = 0.0f;
			bulgeshifting = pvertebra->Bulge*NVec3DotProduct(&vb,pvprev);// /NVec3FastLength(&vprev); is not necessary because vprev ( which is constraint.AB) is already normalized into structure update !!!!
		}

		// Calculate and store the Vertebra's Top vertex position
		NVec3Set(	&pvertex->Position_3f,	pconstraint->AB.y,-pconstraint->AB.x,pconstraint->AB.z);
		NVec3ScaleBy(&pvertex->Position_3f,	pvertebra->SideA*( 1.0f + bulgeshifting ) );
		NVec3AddTo(	&pvertex->Position_3f,	&pconstraint->pjA->CurrentPosition);
		pvertex++;

		// Calculate and store the Vertebra's Center vertex position
		pvertex->Position_3f = pconstraint->pjA->CurrentPosition;
		pvertex++;

		// Calculate and store the Vertebra's Bottom vertex position
		NVec3Set(	&pvertex->Position_3f,	-pconstraint->AB.y,pconstraint->AB.x,pconstraint->AB.z);
		NVec3ScaleBy(&pvertex->Position_3f,	pvertebra->SideB*( 1.0f - bulgeshifting ) );
		NVec3AddTo(	&pvertex->Position_3f,	&pconstraint->pjA->CurrentPosition);
		pvertex ++;
	}
/*
	// ... And the last Vertebra (tail's end)
	// --------------------------------------
	// Presets:
	pconstraint --; // because at the end of the 'for' loop, constraint was incremented one more time
					// and we need to re-use the previous constraint to work because this last joint doesn't
					// have a constraint where it is joint A ...

	pvector		= &pconstraint->pjB->CurrentPosition;
	vect		= pconstraint->AB;

	// Bulge shifting for the last
	// We are going to re-use the previous and already calculated bulge shifting.
	
	// Calculate and store the Vertebra's Top vertex position
	NVec3Set(	&pvertex->Position_3f,	vect.y,-vect.x,vect.z);
	NVec3ScaleBy(&pvertex->Position_3f,	pvertebra->SideA*( 1.0f + bulgeshifting ) );
	NVec3AddTo(	&pvertex->Position_3f,	pvector);
	pvertex++;
	// Calculate and store the Vertebra's Center vertex position
	pvertex->Position_3f = *pvector;
	pvertex++;
	// Calculate and store the Vertebra's Bottom vertex position
	NVec3Set(	&pvertex->Position_3f,	-vect.y,vect.x,vect.z);
	NVec3ScaleBy(&pvertex->Position_3f,	pvertebra->SideB*( 1.0f - bulgeshifting ) );
	NVec3AddTo(	&pvertex->Position_3f,	pvector);
*/
}

void	NUpdateSkin_Rope_Quads_OneRow(NSKIN* pskin)
{
	NVEC3		vb,vprev;
	NSKINVERTEX		*pvertex;
	NCONSTRAINT		*pconstraint;
	Nu32			i;
	Nf32			bulgeshifting;
	//	Nf32			*pfsidea,*pfsideb;
	NSKIN_ROPE_VERTEBRA	*pvertebra;

	// In that case, pRef_Start and Ref_Size represent SEGMENTS like SPRINGS or CONSTRAINTS.
	pvertex = pskin->Rope.pFirstVertex;
	pconstraint = (NCONSTRAINT *)pskin->Rope.pFirst;

/*
	pfsidea = (Nf32*)NGetFirstArrayPtr(pskin->Rope.pSideAVertebra);
	pfsideb = (Nf32*)NGetFirstArrayPtr(pskin->Rope.pSideBVertebra);
*/
	vprev = pconstraint->AB;
	pvertebra = (NSKIN_ROPE_VERTEBRA*)pskin->Rope.VertebrasArray.pFirst;
	for(i=pskin->Rope.RangeSize;i!=0;i--,vprev = pconstraint->AB,pvertex++,pconstraint++,pvertebra++)
	{
		// Calculate the bulgeshifting amount
		if(pvertebra->Bulge == 0.0f)
		{
			bulgeshifting = 0.0f;
		}
		else
		{
			vb.x = pconstraint->AB.y;
			vb.y = -pconstraint->AB.x;
			vb.z = 0.0f;
			bulgeshifting = pvertebra->Bulge*NVec3DotProduct(&vb,&vprev)/NVec3FastLength(&vprev);
		}
	
		// Calculate and store the Vertebra's Top vertex position
		NVec3Set(	&pvertex->Position_3f,	pconstraint->AB.y,-pconstraint->AB.x,pconstraint->AB.z);
		NVec3ScaleBy(&pvertex->Position_3f,	pvertebra->SideA*( 1.0f + bulgeshifting ) );
		NVec3AddTo(	&pvertex->Position_3f,	&pconstraint->pjA->CurrentPosition);
		pvertex++;

		// Calculate and store the Vertebra's Bottom vertex position
		NVec3Set(&pvertex->Position_3f,		-pconstraint->AB.y,pconstraint->AB.x,pconstraint->AB.z);
		NVec3ScaleBy(&pvertex->Position_3f,	pvertebra->SideB*( 1.0f - bulgeshifting ) );
		NVec3AddTo(&pvertex->Position_3f,	&pconstraint->pjA->CurrentPosition);
	}
/*
	// ... And the last Vertebra (tail's end)
	// Presets:
	pconstraint --; // because at the end of the 'for' loop, pspring was incremented one more time

	pvector = &pconstraint->pjB->CurrentPosition;
	vect = pconstraint->AB;
	
	// Bulge shifting for the last
	// We are going to re-use the previous and already calculated bulge shifting.
	// Calculate and store the Vertebra's Top vertex position
	NVec3Set(	&pvertex->Position_3f,	vect.y,-vect.x,vect.z);
	NVec3ScaleBy(&pvertex->Position_3f,	pvertebra->SideA*( 1.0f + bulgeshifting ) );
	NVec3AddTo(	&pvertex->Position_3f,	pvector);
	pvertex++;
	// Calculate and store the Vertebra's Bottom vertex position
	NVec3Set(&pvertex->Position_3f,-vect.y,vect.x,vect.z);
	NVec3ScaleBy(&pvertex->Position_3f,pvertebra->SideB*( 1.0f - bulgeshifting ) );
	NVec3AddTo(&pvertex->Position_3f,pvector);
*/
}

void	NUpdateSkin_Rope_Triangles_OneRow(NSKIN* pskin)
{
	NVEC3		vect,vb,vref;
	NVEC3		*pvector;
	NSKINVERTEX		*pvertex;
	NCONSTRAINT		*pconstraint;
	Nu32			i;
	Nf32			bulgeshifting;
	//	Nf32			*pfsidea,*pfsideb;
	NSKIN_ROPE_VERTEBRA	*pvertebra;

	// In that case, pRef_Start and Ref_Size represent SEGMENTS like SPRINGS or CONSTRAINTS.
	pvertex = pskin->Rope.pFirstVertex;
	pconstraint = (NCONSTRAINT*)pskin->Rope.pFirst;
	vref = pconstraint->AB;

/*
	pfsidea = (Nf32*)NGetFirstArrayPtr(pskin->Rope.pSideAVertebra);
	pfsideb = (Nf32*)NGetFirstArrayPtr(pskin->Rope.pSideBVertebra);
*/
	pvertebra = (NSKIN_ROPE_VERTEBRA*)pskin->Rope.VertebrasArray.pFirst;
	for(i=pskin->Rope.RangeSize;i!=0;i--,pvertebra++)
	{
		// Presets:
		pvector = &pconstraint->pjA->CurrentPosition;
		vect = pconstraint->AB;

		// Calculate the bulgeshifting amount
		if(pvertebra->Bulge == 0.0f)
			bulgeshifting = 0.0f;
		else
		{
			vb.x = vect.y;
			vb.y = -vect.x;
			vb.z = 0.0f;
			bulgeshifting = pvertebra->Bulge*NVec3DotProduct(&vb,&vref)/NVec3FastLength(&vref);
		}

		// Calculate and store the Vertebra's Top vertex position
		NVec3Set(&pvertex->Position_3f,vect.y,-vect.x,vect.z);
		NVec3ScaleBy(&pvertex->Position_3f,pvertebra->SideA*( 1.0f + bulgeshifting ) );
		NVec3AddTo(&pvertex->Position_3f,pvector);
		pvertex++;

		// Calculate and store the Vertebra's Bottom vertex position
		NVec3Set(&pvertex->Position_3f,-vect.y,vect.x,vect.z);
		NVec3ScaleBy(&pvertex->Position_3f,pvertebra->SideB*( 1.0f - bulgeshifting ) );
		NVec3AddTo(&pvertex->Position_3f,pvector);
		pvertex++;

		// Calculate and store the JointB vertex position
		pvertex->Position_3f = pconstraint->pjB->CurrentPosition;
		pvertex++;

		vref = pconstraint->AB;
		pconstraint++;
	}
}

void	NUpdateSkin_Rope_Triangles_DoubleRows(NSKIN* pskin)
{
	NVEC3	vect,vb,vref;
	NVEC3	*pvector;
	NSKINVERTEX	*pvertex;
	NCONSTRAINT	*pconstraint;
	Nu32		i;
	Nf32		bulgeshifting;
	//	Nf32			*pfsidea,*pfsideb;
	NSKIN_ROPE_VERTEBRA	*pvertebra;

	pvertex = pskin->Rope.pFirstVertex;
	pconstraint = (NCONSTRAINT*)pskin->Rope.pFirst;
	vref = pconstraint->AB;

/*
	pfsidea = (Nf32*)NGetFirstArrayPtr(pskin->Rope.pSideAVertebra);
	pfsideb = (Nf32*)NGetFirstArrayPtr(pskin->Rope.pSideBVertebra);
*/
	pvertebra = (NSKIN_ROPE_VERTEBRA*)pskin->Rope.VertebrasArray.pFirst;
	for(i=pskin->Rope.RangeSize;i!=0;i--,pvertebra++)
	{
		// Presets:
		pvector = &pconstraint->pjA->CurrentPosition;
		vect = pconstraint->AB;

		// Calculate the bulgeshifting amount
		if(pvertebra->Bulge == 0.0f)
			bulgeshifting = 0.0f;
		else
		{
			vb.x = vect.y;
			vb.y = -vect.x;
			vb.z = 0.0f;
			bulgeshifting = pvertebra->Bulge*NVec3DotProduct(&vb,&vref)/NVec3FastLength(&vref);
		}

		// Calculate and store the Vertebra's Top vertex position
		NVec3Set(	&pvertex->Position_3f,vect.y,-vect.x,vect.z);
		NVec3ScaleBy(&pvertex->Position_3f,pvertebra->SideA*( 1.0f + bulgeshifting ) );
		NVec3AddTo(	&pvertex->Position_3f,pvector);
		pvertex++;
		// Calculate and store the Vertebra's Center vertex position
		pvertex->Position_3f = *pvector;
		pvertex++;

		// Calculate and store the Vertebra's Bottom vertex position
		NVec3Set(&pvertex->Position_3f,-vect.y,vect.x,vect.z);
		NVec3ScaleBy(&pvertex->Position_3f,pvertebra->SideB*( 1.0f - bulgeshifting ) );
		NVec3AddTo(&pvertex->Position_3f,pvector);
		
		pvertex++;
		vref = pconstraint->AB;
		pconstraint++;
	}

	// ... And the last Vertex (tail's end)
	// Presets:
	pconstraint --; // because at the end of the 'for' loop, pspring was incremented one more time
	pvertex->Position_3f = pconstraint->pjB->CurrentPosition;
}

void	NUpdateSkin_Grid_Quads(NSKIN* pskin)
{
	NSKINVERTEX	*pvertex;
	NJOINT		*pjoint;
	Nu32		i;


	// In that case, pRef_Start and Ref_Size represent JOINTS.
	pvertex = pskin->Grid.pFirstVertex;
	pjoint  = (NJOINT*)pskin->Grid.pFirst;

	for(i=pskin->Grid.RangeSize;i!=0;i--,pvertex++,pjoint++)
	{
		pvertex->Position_3f = pjoint->CurrentPosition;
	}
}

void	NUpdateSkin_Grid_FanQuads(NSKIN* pskin)
{
	NUpdateSkin_Grid_Quads(pskin);
}

void	NUpdateSkin_Billboard_Quad(NSKIN* pskin)              
{
	NVEC3	v;
	NVEC3	vcenter;
	NVEC3	vi,vj;
	
	NSKINVERTEX	*pvertex	= pskin->BillBoard.pFirstVertex;
	// AB vector (B-A)
	NVec3Sub(&vi,&pskin->BillBoard.pOrient_B->CurrentPosition,&pskin->BillBoard.pOrient_A->CurrentPosition);
	Nf32 f = NVec3FastLength(&vi);
	
	if(f<=NF32_EPSILON_VECTOR_LENGTH) // that means the vector length is close to 0 ! And this is not good !
	{
		f = 1.0f;
		NVec3Set(&vi,1.0f,0,0);
	}
	else
	{
		f=1.0f/f;
		NVec3Scale(&vi,&vi,f);
	}

	// Get Vj. Notice vi is a unit vector, NCamera.ZAxis is a unit vector , so ... Vj will be a unit vector
	NVec3CrossProduct(&vj,&vi,&NCamera.ZAxis); // Direct Base
	
	//		1 ----- 2		2 ----- 1
	//		|       |		|       |
	//		| - + - |		| - + - |
	//		|       |		|       |
	//		0 ----- 3		3 ----- 0
	// If we consider s = pskin->BillBoard.fScale
	// The basics X,Y coordinates of the BB around its center are:
	//	  -s,+s -- +s,+s 
	//		|        |	
	//		| - + -  |	
	//		|        |	
	//	  -s,-s -- +s,-s
		
	// First of all we need to transform VShift according to the orthonormal (vi,vj),
	vcenter.x = pskin->BillBoard.pCenter->CurrentPosition.x + vi.x * pskin->BillBoard.VShift.x + vj.x * pskin->BillBoard.VShift.y; 
	vcenter.y = pskin->BillBoard.pCenter->CurrentPosition.y + vi.y * pskin->BillBoard.VShift.x + vj.y * pskin->BillBoard.VShift.y; 
	vcenter.z = pskin->BillBoard.pCenter->CurrentPosition.z + pskin->BillBoard.VShift.z; 
																						 
	// prepare vi and vj (scale them before use them)
	NVec3ScaleBy(&vi,pskin->BillBoard.RatioWidth_Height*pskin->BillBoard.fScale);	// W/H * Scale
	NVec3ScaleBy(&vj,pskin->BillBoard.fScale);										// H/H * Scale (= Scale)

	// 0
	NVec3Add(&v,&vcenter,&vi);
	NVec3Sub(&pvertex->Position_3f,&v,&vj);
	// 1
	pvertex++;
	NVec3Add(&pvertex->Position_3f,&v,&vj);
	// 2
	pvertex++;
	NVec3Sub(&v,&vcenter,&vi);//NVec3Sub(&v,&pc->pjA->CurrentPosition,&va);
	NVec3Add(&pvertex->Position_3f,&v,&vj);
	// 3
	pvertex++;
	NVec3Sub(&pvertex->Position_3f,&v,&vj);
/*
#ifdef _DEBUG
	// DEBUG DRAWING
	pvertex	= pskin->BillBoard.pFirstVertex;
	NVEC3	n;
	NMATRIX3x3	m;
	NComputeTriangleNormal(&n,&pvertex[0].Position_3f,&pvertex[1].Position_3f,&pvertex[2].Position_3f);
	NUT_Build3DOrthonormal(&m,&n,NULL,NULL);
	NUT_Draw_3DMatrix3x3(&m,&pvertex[3].Position_3f,25.0f);
#endif
*/

}
