#include "../NCStandard.h"
#include "../NType.h"
#include "../GL/Ngl.h"
#include "../Maths/NVec3f32.h"
#include "../NCamera.h"
#include "../NErrorHandling.h"
#include "NFrustum_Culling.h"

extern NCAMERA		NCamera;
static inline NFRUSTUM_CULLING_RESULT  _nearfarfrustum(const NVEC3 *ppos, const Nf32 fradius, Nf32 *preturn_spherecamcoord_z)
{
	Nf32 az;

	// ZAxis Tests:
	*preturn_spherecamcoord_z = az  = NVec3DotProduct(ppos,&NCamera.ZAxis); // !!! TODO:::: MAybe -Z Check !

	if (az > NCamera.fZFar + fradius || az < NCamera.fZNear - fradius)
		return NFRUSTUM_CULLING_OUTSIDE;

	if (az > NCamera.fZFar - fradius || az < NCamera.fZNear + fradius)
		return NFRUSTUM_CULLING_INTERSECT;

	return NFRUSTUM_CULLING_INSIDE;

}

static inline NFRUSTUM_CULLING_RESULT _3dfrustum(const NVEC3 *ppos, const Nf32 fradius, Nf32 *preturn_spherecamcoord_z)
{
	Nf32 ax,ay,az,d;

	// ZAxis Tests:
	*preturn_spherecamcoord_z = az  = NVec3DotProduct(ppos,&NCamera.ZAxis); // !!! TODO:::: MAybe -Z Check !

	if (az > NCamera.fZFar + fradius || az < NCamera.fZNear - fradius)
		return NFRUSTUM_CULLING_OUTSIDE;

/*
	if (az > NCamera.fZFar - fradius || az < NCamera.fZNear + fradius)
		result = NFRUSTUM_CULLING_INTERSECT;
*/

	// YAxis Tests:
	ay = NVec3DotProduct(ppos,&NCamera.YAxis);
	d = fradius * NCamera.PreCompute_SphereFactorY;
	az *= NCamera.PreCompute_Tangente;
	if ( ay > az + d || ay < -az - d )
		return NFRUSTUM_CULLING_OUTSIDE;

/*
	if ( ay > az-d || ay < -az+d )
		result = NFRUSTUM_CULLING_INTERSECT;
*/

	// XAxis Tests:
	ax = NVec3DotProduct(ppos,&NCamera.XAxis);
	az *= NCamera.fAspectRatio;
	d = fradius * NCamera.PreCompute_SphereFactorX;
	if (ax > az+d || ax < -az-d)
		return NFRUSTUM_CULLING_OUTSIDE;

/*
	if (ax > az-d || ax < -az+d)
		result = NFRUSTUM_CULLING_INTERSECT;
*/

	return NFRUSTUM_CULLING_INSIDE;
}
// ------------------------------------------------------------------------------------------
// NFrustumCulling_BSphere_vs_NearFarFrustum
// ------------------------------------------------------------------------------------------
// Description :
// ------------------------------------------------------------------------------------------
// In  :
//		
// Out :
//		
// ------------------------------------------------------------------------------------------
NFRUSTUM_CULLING_RESULT	NFrustumCulling_BSphere_vs_NearFarFrustum(const NBOUNDINGSPHERE *psphere, const NMATRIX *pmatrix, Nf32 *preturn_spherecamcoord_z)
{
	NVEC3				v;

	return NFRUSTUM_CULLING_INSIDE;
	// TODO: Test and Debug this function
	if( pmatrix )
	{
		NMulVector3ByMatrixO(&v,pmatrix,&psphere->Center);
		NVec3SubFrom(&v,&NCamera.Pos);
	}
	else
	{
		NVec3Sub(&v,&psphere->Center,&NCamera.Pos);
	}
	return _nearfarfrustum(&v,psphere->fRadius,preturn_spherecamcoord_z);
}

// ------------------------------------------------------------------------------------------
// NFrustumCulling_BSphere_vs_3DFrustum
// ------------------------------------------------------------------------------------------
// Description :
// ------------------------------------------------------------------------------------------
// In  :
//		
// Out :
//		
// ------------------------------------------------------------------------------------------
NFRUSTUM_CULLING_RESULT	NFrustumCulling_BSphere_vs_3DFrustum(const NBOUNDINGSPHERE *psphere, const NMATRIX *pmatrix, Nf32 *preturn_spherecamcoord_z)
{
	NVEC3				v;

	// TODO: Test and Debug this function
	if( pmatrix )
	{
		NMulVector3ByMatrixO(&v,pmatrix,&psphere->Center);
		NVec3SubFrom(&v,&NCamera.Pos);
	}
	else
	{
		NVec3Sub(&v,&psphere->Center,&NCamera.Pos);
	}
	return _3dfrustum(&v, psphere->fRadius, preturn_spherecamcoord_z);
}


// ------------------------------------------------------------------------------------------
// NCulling_Renderable_vs_3DFrustum
// ------------------------------------------------------------------------------------------
// Description :
// ------------------------------------------------------------------------------------------
// In  :
//		
// Out :
//		
// ------------------------------------------------------------------------------------------
NFRUSTUM_CULLING_RESULT	NRenderableCulling_vs_3DFrustum(const NRENDERABLE *prenderable, Nf32 *preturn_spherecamcoord_z)
{
	NVEC3				v;

	// TODO: Test and Debug this function
	if( prenderable->pTransformHNode )
	{
		NMulVector3ByMatrixO(&v,&prenderable->pTransformHNode->WorldMatrix,&prenderable->BoundingSphere.Center);
		NVec3SubFrom(&v,&NCamera.Pos);
	}
	else
	{
		NVec3Sub(&v,&prenderable->BoundingSphere.Center,&NCamera.Pos);
	}
	return _3dfrustum(&v, prenderable->BoundingSphere.fRadius, preturn_spherecamcoord_z);
}
// ------------------------------------------------------------------------------------------
// NRenderableCulling_vs_NearFarFrustum
// ------------------------------------------------------------------------------------------
// Description :
// ------------------------------------------------------------------------------------------
// In  :
//		
// Out :
//		
// ------------------------------------------------------------------------------------------
NFRUSTUM_CULLING_RESULT	NRenderableCulling_vs_NearFarFrustum(const NRENDERABLE *prenderable, Nf32 *preturn_spherecamcoord_z)
{
	NVEC3				v;

	return NFRUSTUM_CULLING_INSIDE;

	// TODO: Test and Debug this function
	if( prenderable->pTransformHNode )
	{
		NMulVector3ByMatrixO(&v,&prenderable->pTransformHNode->WorldMatrix,&prenderable->BoundingSphere.Center);
		NVec3SubFrom(&v,&NCamera.Pos);
	}
	else
	{
		NVec3Sub(&v,&prenderable->BoundingSphere.Center,&NCamera.Pos);
	}
	return _nearfarfrustum(&v,prenderable->BoundingSphere.fRadius,preturn_spherecamcoord_z);
}
/*
class FrustumR{

public:
	static enum {OUTSIDE, INTERSECT, INSIDE};

	Vec3 cc; // camera position
	Vec3 X,Y,Z;// the camera referential
	float nearD, farD, width, height;

	// NEW: these are the variables required to test spheres
	float sphereFactorX, sphereFactorY;

	FrustumR::FrustumR();
	FrustumR::~FrustumR();

	void setCamInternals(float angle, float ratio, float nearD, float farD);
	void setCamDef(Vec3 &p, Vec3 &l, Vec3 &u);
	int pointInFrustum(Vec3 &p);

	// NEW: function to test spheres
	int sphereInFrustum(Vec3 ¢er, float radius);
};

#define HALF_ANG2RAD 3.14159265358979323846/360.0

void FrustumR::setCamInternals(float angle, float ratio, float nearD, float farD)
{

	// half of the the horizontal field of view
	float angleX;
	// store the information
	this->ratio = ratio;
	this->nearD = nearD;
	this->farD = farD;

	angle *= HALF_ANG2RAD;
	// compute width and height of the near and far plane sections
	tang = tan(angle);
	sphereFactorY = 1.0/cos(angle);

	// compute half of the the horizontal field of view and sphereFactorX
	float anglex = atan(tang*ratio);
	sphereFactorX = 1.0/cos(anglex);
}

int FrustumR::sphereInFrustum(Vec3 &p, float radius)
{

	float d;
	float az,ax,ay;
	int result = INSIDE;

	Vec3 v = p-camPos;

	az = v.innerProduct(-Z);
	if (az > farD + radius || az < nearD-radius)
		return(OUTSIDE);

	if (az > farD - radius || az < nearD+radius)
		result = INTERSECT;

	ay = v.innerProduct(Y);
	d = sphereFactorY * radius;
	az *= tang;
	if (ay > az+d || ay < -az-d)
		return(OUTSIDE);

	if (ay > az-d || ay < -az+d)
		result = INTERSECT;

	ax = v.innerProduct(X);
	az *= ratio;
	d = sphereFactorX * radius;
	if (ax > az+d || ax < -az-d)
		return(OUTSIDE);

	if (ax > az-d || ax < -az+d)
		result = INTERSECT;

	return(result);
}
*/
