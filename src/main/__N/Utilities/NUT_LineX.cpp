#include "NUT_X.h"


Nbool NUT_LineXPLane(const NVEC3 *plineA,const NVEC3 *plinedir,const NVEC3 *pplaneP,const NVEC3 *pplanedir,NVEC3 *presult)
{
	Nf32 dot = NVec3DotProduct(pplanedir,plinedir);
	if( NABS(dot) <= NF32_EPSILON_VECTOR_DOTPRODUCT ) // instead of == 0
		return NFALSE;
	else
	{
		if(presult)
		{
			//NVec3Sub(&u,pplaneP,plineA);
			//NVec3Scale(&v, plinedir, NVec3DotProduct(pplanedir,&u)/dot);
			//NVec3Add(presult,plineA,&v);
			
			// ... same thing but with no calls 
			dot = ( (pplaneP->x - plineA->x)*pplanedir->x + (pplaneP->y - plineA->y)*pplanedir->y + (pplaneP->z - plineA->z)*pplanedir->z) / dot;
			presult->x = plineA->x + plinedir->x * dot;
			presult->y = plineA->y + plinedir->y * dot;
			presult->z = plineA->z + plinedir->z * dot;
		}

		return NTRUE;
	}
}

/*
Nbool NUT_LineXPLaneXY(const NVEC3 *plineA,const NVEC3 *plinedir,const NVEC3 *pplaneP,NVEC3 *presult)
{
	//Nf32		dot;
	NVEC3	u,v;
	
	// Plane XY Director Vector = {0,0,1}
	// dot = plinedir->z;
	if( NABS(plinedir->z) <= NF32_EPSILON_VECTOR_DOTPRODUCT ) // instead of == 0
	{
		return NFALSE;
	}
	else
	{
		if(presult)
		{
			Nf32 f = (pplaneP->z - plineA->z)/plinedir->z;		
			presult->x = plineA->x + plinedir->x * f;
			presult->x = plineA->y + plinedir->y * f;
			presult->x = plineA->z + plinedir->z * f;

		}
		return NTRUE;
	}

	return NFALSE;

}
*/



Nf32 NUT_LinePointShortestDistance(const NVEC3 *pline_o,const NVEC3 *pline_dir,const NVEC3 *ppoint)
{
	NErrorIf( !NVec3IsUnit(pline_dir), NERROR_VECTOR_MUST_BE_UNIT); // Not an Error, just a WARNING !!!

//	NVEC3	op,p;	
	NVEC3	oa,pa;
	Nf32		fdot;

	NVec3Sub(&oa,ppoint,pline_o);
	fdot = NVec3DotProduct(pline_dir,&oa);
	
// 	NVec3Scale(&op,pline_dir,fdot);
// 	NVec3Add(&p,&op,pline_o);
// 	NVec3Sub(&pa,ppoint,&p);
//	---> replace by these 3 lines ( same thing but directly )	
	pa.x = ppoint->x - (pline_dir->x*fdot + pline_o->x); 
	pa.y = ppoint->y - (pline_dir->y*fdot + pline_o->y); 
	pa.z = ppoint->z - (pline_dir->z*fdot + pline_o->z); 

	return NVec3Length(&pa);
}

Nf32 NUT_LinePointShortestSquareDistance(const NVEC3 *pline_o,const NVEC3 *pline_dir,const NVEC3 *ppoint)
{
	NErrorIf( !NVec3IsUnit(pline_dir), NERROR_VECTOR_MUST_BE_UNIT); // Not an Error, just a WARNING !!!

//	NVEC3	op,p;	
	NVEC3	oa,pa;
	Nf32		fdot;

	NVec3Sub(&oa,ppoint,pline_o);
	fdot = NVec3DotProduct(pline_dir,&oa);

	// 	NVec3Scale(&op,pline_dir,fdot);
	// 	NVec3Add(&p,&op,pline_o);
	// 	NVec3Sub(&pa,ppoint,&p);
	//	---> replace by these 3 lines ( same thing but directly )	
	pa.x = ppoint->x - (pline_dir->x*fdot + pline_o->x); 
	pa.y = ppoint->y - (pline_dir->y*fdot + pline_o->y); 
	pa.z = ppoint->z - (pline_dir->z*fdot + pline_o->z); 

	return NVec3SquareLength(&pa);
}


Nbool NUT_LineXAABB( const NVEC3 *pline_o,const NVEC3 *pline_dir, const NAABB *paabb  )
{	
/*
	// Put line in box space
	CMatrix MInv = m_M.InvertSimple();
	CVec3 LB1 = MInv * L1;
	CVec3 LB2 = MInv * L2;

	// Get line midpoint and extent
	CVec3 LMid = (LB1 + LB2) * 0.5f; 
	CVec3 L = (LB1 - LMid);
	CVec3 LExt = CVec3( fabs(L.x), fabs(L.y), fabs(L.z) );

	// Use Separating Axis Test
	// Separation vector from box center to line center is LMid, since the line is in box space
	if ( fabs( LMid.x ) > m_Extent.x + LExt.x ) return false;
	if ( fabs( LMid.y ) > m_Extent.y + LExt.y ) return false;
	if ( fabs( LMid.z ) > m_Extent.z + LExt.z ) return false;
	// Crossproducts of line and each axis
	if ( fabs( LMid.y * L.z - LMid.z * L.y)  >  (m_Extent.y * LExt.z + m_Extent.z * LExt.y) ) return false;
	if ( fabs( LMid.x * L.z - LMid.z * L.x)  >  (m_Extent.x * LExt.z + m_Extent.z * LExt.x) ) return false;
	if ( fabs( LMid.x * L.y - LMid.y * L.x)  >  (m_Extent.x * LExt.y + m_Extent.y * LExt.x) ) return false;
	// No separating axis, the line intersects
	return true;

	// For a segment
	bool Meshmerizer::SegmentAABB(const Segment& segment, const AABB& aabb)
	{
		Point BoxExtents, Diff, Dir;
		float fAWdU[3];

		Dir.x = 0.5f * (segment.mP1.x - segment.mP0.x);
		BoxExtents.x = aabb.GetExtents(0);
		Diff.x = (0.5f * (segment.mP1.x + segment.mP0.x)) - aabb.GetCenter(0);
		fAWdU[0] = fabsf(Dir.x);
		if(fabsf(Diff.x)>BoxExtents.x + fAWdU[0])	return false;

		Dir.y = 0.5f * (segment.mP1.y - segment.mP0.y);
		BoxExtents.y = aabb.GetExtents(1);
		Diff.y = (0.5f * (segment.mP1.y + segment.mP0.y)) - aabb.GetCenter(1);
		fAWdU[1] = fabsf(Dir.y);
		if(fabsf(Diff.y)>BoxExtents.y + fAWdU[1])	return false;

		Dir.z = 0.5f * (segment.mP1.z - segment.mP0.z);
		BoxExtents.z = aabb.GetExtents(2);
		Diff.z = (0.5f * (segment.mP1.z + segment.mP0.z)) - aabb.GetCenter(2);
		fAWdU[2] = fabsf(Dir.z);
		if(fabsf(Diff.z)>BoxExtents.z + fAWdU[2])	return false;

		float f;
		f = Dir.y * Diff.z - Dir.z * Diff.y;	if(fabsf(f)>BoxExtents.y*fAWdU[2] + BoxExtents.z*fAWdU[1])	return false;
		f = Dir.z * Diff.x - Dir.x * Diff.z;	if(fabsf(f)>BoxExtents.x*fAWdU[2] + BoxExtents.z*fAWdU[0])	return false;
		f = Dir.x * Diff.y - Dir.y * Diff.x;	if(fabsf(f)>BoxExtents.x*fAWdU[1] + BoxExtents.y*fAWdU[0])	return false;

		return true;
	}

	// For a ray
	bool Meshmerizer::RayAABB(const Ray& ray, const AABB& aabb)
	{
		Point BoxExtents, Diff;

		Diff.x = ray.mOrig.x - aabb.GetCenter(0);
		BoxExtents.x = aabb.GetExtents(0);
		if(fabsf(Diff.x)>BoxExtents.x && Diff.x*ray.mDir.x>=0.0f)	return false;

		Diff.y = ray.mOrig.y - aabb.GetCenter(1);
		BoxExtents.y = aabb.GetExtents(1);
		if(fabsf(Diff.y)>BoxExtents.y && Diff.y*ray.mDir.y>=0.0f)	return false;

		Diff.z = ray.mOrig.z - aabb.GetCenter(2);
		BoxExtents.z = aabb.GetExtents(2);
		if(fabsf(Diff.z)>BoxExtents.z && Diff.z*ray.mDir.z>=0.0f)	return false;

		float fAWdU[3];
		fAWdU[0] = fabsf(ray.mDir.x);
		fAWdU[1] = fabsf(ray.mDir.y);
		fAWdU[2] = fabsf(ray.mDir.z);

		float f;
		f = ray.mDir.y * Diff.z - ray.mDir.z * Diff.y;	if(fabsf(f)>BoxExtents.y*fAWdU[2] + BoxExtents.z*fAWdU[1])	return false;
		f = ray.mDir.z * Diff.x - ray.mDir.x * Diff.z;	if(fabsf(f)>BoxExtents.x*fAWdU[2] + BoxExtents.z*fAWdU[0])	return false;
		f = ray.mDir.x * Diff.y - ray.mDir.y * Diff.x;	if(fabsf(f)>BoxExtents.x*fAWdU[1] + BoxExtents.y*fAWdU[0])	return false;

		return true;
	}
*/
	// A) Put Line in AABB Space:
	//		Nothing to do because AABB is an ALigned Bounding Box Actually !!!!

	// B) THE ALGORYTHM ! 
	NVEC3	aabb_center;
	NVEC3	aabb_extents;
	NVEC3	diff;

	aabb_center.x	= ( paabb->fXMin + paabb->fXMax)*0.5f;
	aabb_extents.x	= ( paabb->fXMax - paabb->fXMin)*0.5f;
	diff.x			= pline_o->x - aabb_center.x; 
	if( NABS(diff.x)>aabb_extents.x && diff.x*pline_dir->x>=0.0f )return NFALSE;

	aabb_center.y	= ( paabb->fYMin + paabb->fYMax)*0.5f;
	aabb_extents.y	= ( paabb->fYMax - paabb->fYMin)*0.5f;
	diff.y			= pline_o->y - aabb_center.y; 
	if( NABS(diff.y)>aabb_extents.y && diff.y*pline_dir->y>=0.0f )return NFALSE;

	aabb_center.z	= ( paabb->fZMin + paabb->fZMax)*0.5f;
	aabb_extents.z	= ( paabb->fZMax - paabb->fZMin)*0.5f;
	diff.z			= pline_o->z - aabb_center.z; 
	if( NABS(diff.z)>aabb_extents.z && diff.z*pline_dir->z>=0.0f )return NFALSE;

	NVEC3	awdu;
	Nf32		f;

	awdu.x = NABS(pline_dir->x);
	awdu.y = NABS(pline_dir->y);
	awdu.z = NABS(pline_dir->z);

	f = pline_dir->y * diff.z - pline_dir->z * diff.y;	if( NABS(f) > aabb_extents.y*awdu.z + aabb_extents.z*awdu.y )return NFALSE;
	f = pline_dir->z * diff.x - pline_dir->x * diff.z;	if( NABS(f) > aabb_extents.x*awdu.z + aabb_extents.z*awdu.x )return NFALSE;
	f = pline_dir->x * diff.y - pline_dir->y * diff.x;	if( NABS(f) > aabb_extents.x*awdu.y + aabb_extents.y*awdu.x )return NFALSE;

	return NTRUE;
}