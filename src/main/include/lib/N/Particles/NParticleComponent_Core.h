#ifndef __NPARTICLECOMPONENT_CORE_H
#define __NPARTICLECOMPONENT_CORE_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NParticleComponent_Core.h							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NType.h"
#include "../Core/NSafeConversion.h"
#include "../Geometry/NGeometry.h"
#include "../Geometry/Components/NGeometryAppearance.h"
#include "../Render/Renderable/NRenderable_UpdateAndExtract_LUT.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

//
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + CONSTANT ans DEFAULT VALUES
																														// Std	 : NPARTICLE
																														// Adv	 : NADVANCEDPARTICLE
																														// H.Std : NHOMINGPARTICLE
																														// H.Adv : NADVANCEDHOMINGPARTICLE
																														//
																														// | Std | Adv |H.Std|H.Adv|     |     |
// 4 vertex by particle and AT maximum NU16_MAX indexed vertex (OPENGL ES limitation)									//  ----- ----- ----- ----- ----- -----			
#define CONSTANT_NPARTICLE_NB_MAX													NU16_MAX/4							// |  O  |  O  |  O	 |  O  |     |     |		
#define CONSTANT_NPARTICLE_LOWLEVEL_EXTRACTSET										NEXTRACTSET_SECTION_STANDARD		// |  O  |  O  |  O	 |  O  |     |     |	// SINGLE INSTANCE + SINGLE GEOMETRY in SECTION STANDARD	
																														//  ----- ----- ----- ----- ----- -----			
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_CORE_RENDERABLE_BOUNDINGSPHERE_CENTER_3F	0.0f,0.0f,0.0f						// |  O  |  O  |  O	 |  O  |     |     |	
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_CORE_RENDERABLE_BOUNDINGSPHERE_RADIUS	1.0f								// |  O  |  O  |  O	 |  O  |     |     |	
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_CORE_PARTICLESNBMAX						100									// |  O  |  O  |  O	 |  O  |     |     |	
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_CORE_BIRTHRATE							0									// |  O  |  O  |  O	 |  O  |     |     |
																														// |  O  |  O  |  O	 |  O  |     |     |
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_CORE_GEOMETRY_SPECS_ID					NGEOMETRY_FORMAT_TEXTRIANGLE_TUT1_V3C4T2_TRIANGLE
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_CORE_USER_STATESET						0
#define DEFAULT_NPARTICLE_EMITTERCOMPONENT_CORE_HIGHLEVEL_EXTRACTSET				FLAG_NEXTRACTSET_3D
// CORE FLAGS
																									// | Std | Adv |H.Std|H.Adv|     |     |
																									//  ----- ----- ----- ----- ----- -----			
#define FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_AUTO_EMISSION							BIT_0			// |  O  |  O  |  O	 |  O  |     |     |
// #define FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_xxx									BIT_1			// |	 |	   |   	 |     |     |     |
// #define FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_xxx									BIT_...			// |	 |	   |   	 |     |     |     |
// #define FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_xxx									BIT_15			// |	 |	   |   	 |     |     |     |

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + DESCRIPTION FLAGS
//
//																									// | Std | Adv |H.Std|H.Adv|     |     |
//																									//  ----- ----- ----- ----- ----- -----	
#define FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_AUTO_EMISSION						BIT_0			// |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_BOUNDINGSPHERE				BIT_1			// |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_RENDERSETTINGS_ID				BIT_2			// |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_USER_STATESET					BIT_3			// |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_HIGHLEVEL_EXTRACTSET			BIT_4			// |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_APPERANCERAW					BIT_5			// |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_PARTICLESNBMAX				BIT_6			// |  O  |  O  |  O	 |  O  |     |     |	
#define FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_INITIALPARTICLESNB			BIT_7			// |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_BIRTH_RATE					BIT_8			// |  O  |  O  |  O	 |  O  |     |     |
#define FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_THN							BIT_9			// |  O  |  O  |  O	 |  O  |     |     |
// #define FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_xxx							BIT_...			// |	 |	   |   	 |     |     |     |
// #define FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_xxx							BIT_15			// |	 |	   |   	 |     |     |     |
#define FLAGS_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_ALL							0x1FF

// ***************************************************************************************
// **								 Structures											**
// ***************************************************************************************

typedef	struct 
{
	Nu16								Flags;
	Nu16								Available_Nu16;
	NBOUNDINGSPHERE						BoundingSphere;		// Particles Emitter Renderable BoundingSphere 
	Nu16								ParticlesNbMax;		// Maximum number of particles	
	Nu16								InitialParticlesNb;	// Initial Number of particles
	Nf32								BirthRate;			// Delta time between 2 particles "automatic" creation
	NEXTRACTSET							HighLevelExtractSet;
	NTRANSFORM_HNODE					*pTransformHNode;

	NGEOMETRY_FORMAT_ENUM				GeometryFormat;
	NSTATESET							UserStateSet;
	NAPPEARANCE_RAW						AppearanceRaw;

}NPARTICLE_EMITTERCOMPONENT_CORE_DESC;

typedef	struct 
{
	NRENDERABLE				*pRenderable;
	Nu16					Flags;				// CORE Flags
	Nu16					ParticleStructSize;	// In BYTES. This is the "SizeOf()" of one particle of this emitter.
	void					*pParticlesList;	// Particles list
	Nu16					ParticlesNbMax;		// Maximum number of particles	
	Nu16					ParticlesNb;		// Current number of particles	
	Nf32					ParticleNbToAwake;	// Number of particle to create (between 0 and 1, between 2 update loops )
	Nf32					BirthRate;			// Delta time between 2 particles "automatic" creation
}NPARTICLE_EMITTERCOMPONENT_CORE;

// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
// Setup Particle Emitter Component Desc Parameters
inline void	NSetupParticleEmitterComponent_Core_Desc_HighLevelExtractSet(NPARTICLE_EMITTERCOMPONENT_CORE_DESC *pdesc, const NEXTRACTSET hxset){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_HIGHLEVEL_EXTRACTSET);pdesc->HighLevelExtractSet = (NEXTRACTSET)hxset;}

inline void	NSetupParticleEmitterComponent_Core_Desc_ActivateAutoEmission(NPARTICLE_EMITTERCOMPONENT_CORE_DESC *pdesc){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_AUTO_EMISSION);}
inline void	NSetupParticleEmitterComponent_Core_Desc_BoundingSphere(NPARTICLE_EMITTERCOMPONENT_CORE_DESC *pdesc,const NBOUNDINGSPHERE *psh){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_BOUNDINGSPHERE);pdesc->BoundingSphere = *(NBOUNDINGSPHERE*)psh;}
inline void	NSetupParticleEmitterComponent_Core_Desc_BoundingSpheref(NPARTICLE_EMITTERCOMPONENT_CORE_DESC *pdesc,const Nf32 radius, const Nf32 centerx,const Nf32 centery,const Nf32 centerz){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_BOUNDINGSPHERE); NVec3Set( &pdesc->BoundingSphere.Center,centerx,centery,centerz );pdesc->BoundingSphere.fRadius = radius;}
inline void	NSetupParticleEmitterComponent_Core_Desc_ParticleNumbers(NPARTICLE_EMITTERCOMPONENT_CORE_DESC *pdesc,const Nu16 particlesnbmax,const Nu16 initialparticlesnb){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_PARTICLESNBMAX);FLAG_ON( pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_INITIALPARTICLESNB);pdesc->ParticlesNbMax = particlesnbmax;pdesc->InitialParticlesNb = initialparticlesnb;}
inline void	NSetupParticleEmitterComponent_Core_Desc_BirthRate(NPARTICLE_EMITTERCOMPONENT_CORE_DESC *pdesc,const Nf32 birthrate){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_BIRTH_RATE);pdesc->BirthRate = birthrate;}
inline void	NSetupParticleEmitterComponent_Core_Desc_TransformHNode(NPARTICLE_EMITTERCOMPONENT_CORE_DESC *pdesc,const NTRANSFORM_HNODE *pthn){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_THN);pdesc->pTransformHNode = (NTRANSFORM_HNODE*)pthn;}

inline void	NSetupParticleEmitterComponent_Core_Desc_Texture(NPARTICLE_EMITTERCOMPONENT_CORE_DESC *pdesc,const Nu8 tuid, const NTEXTURE *ptexture){NErrorIf(!ptexture, NERROR_NULL_POINTER);FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_APPERANCERAW);NSetAppearance_Raw_Texture(&pdesc->AppearanceRaw,tuid,ptexture);}
inline void	NSetupParticleEmitterComponent_Core_Desc_BlendAndMaterial(NPARTICLE_EMITTERCOMPONENT_CORE_DESC *pdesc,const NBLEND *pblend, const NMATERIAL *pmaterial){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_APPERANCERAW);NSetAppearance_Raw_BlendAndMaterial(&pdesc->AppearanceRaw,pblend,pmaterial);}
//inline void	NSetupParticleEmitterComponent_Core_Desc_StateSetSpecFlags(NPARTICLE_EMITTERCOMPONENT_CORE_DESC *pdesc,const Nu32 statesetspecflags){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_STATESET_SPECFLAGS);pdesc->StateSet_SpecFlags = statesetspecflags;}
//inline void	NSetupParticleEmitterComponent_Core_Desc_Texture(NPARTICLE_EMITTERCOMPONENT_CORE_DESC *pdesc,const NTEXTURE *ptexture){FLAG_ON(pdesc->Flags, FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_TEXTURE);pdesc->pTexture = (NTEXTURE*)ptexture;}

// Misc
inline Nu16	NGetParticleEmitterComponent_Core_CurrentParticleNumber(NPARTICLE_EMITTERCOMPONENT_CORE *pcore){return pcore->ParticlesNb;}

// ***************************************************************************************
// **				LOW LEVEL:PRIVATE Inline Setup & Clear Functions					**
// ***************************************************************************************
inline void NApplyBirthRate(NPARTICLE_EMITTERCOMPONENT_CORE *pemittercore, const Nf32 fdelay)
{
	pemittercore->ParticleNbToAwake += fdelay*pemittercore->BirthRate;
	Nf32 fnewparticlenb = floorf(pemittercore->ParticleNbToAwake);
	pemittercore->ParticleNbToAwake -= fnewparticlenb; // We keep the rest for the next update loop.
	pemittercore->ParticlesNb = NMIN( pemittercore->ParticlesNb + (Nu16)fnewparticlenb, pemittercore->ParticlesNbMax );
}

inline void NSetupParticlesEmitterComponent_Core( NPARTICLE_EMITTERCOMPONENT_CORE *pemittercomponent,const NPARTICLE_EMITTERCOMPONENT_CORE_DESC *pdesc, const Nu32 sizeofparticle, const NRENDERABLE_UPDATE_FCT pupdate_handle )
{
	NErrorIf(!sizeofparticle,	NERROR_NULL_VALUE);
	NErrorIf(!pupdate_handle,	NERROR_NULL_POINTER);
	NErrorIf(!pdesc,			NERROR_NULL_POINTER);
	
	// NEXTRACTSET						HighLevelExtractSet;
	NEXTRACTSET highlevel_extractset;
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_HIGHLEVEL_EXTRACTSET) )
	{
		highlevel_extractset = pdesc->HighLevelExtractSet;
	}
	else
	{
		highlevel_extractset = DEFAULT_NPARTICLE_EMITTERCOMPONENT_CORE_HIGHLEVEL_EXTRACTSET;
	}
	NErrorIf( NGET_EXTRACTSET_LOWLEVEL(highlevel_extractset),NERROR_RENDERABLE_EXTRACTSET_UNAUTHORIZED_FLAGS_USING );

	// NGEOMETRY_FORMAT_ENUM	GeometryFormat;
	NGEOMETRY_FORMAT_ENUM	rsid;
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_RENDERSETTINGS_ID) )
	{
		rsid = pdesc->GeometryFormat;
	}
	else
	{
		rsid = DEFAULT_NPARTICLE_EMITTERCOMPONENT_CORE_GEOMETRY_SPECS_ID;
	}
	// NSTATESET						UserStateSet;
	NSTATESET user_stateset_flags;
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_USER_STATESET) )
	{
		user_stateset_flags = pdesc->UserStateSet;
	}
	else
	{
		user_stateset_flags = DEFAULT_NPARTICLE_EMITTERCOMPONENT_CORE_USER_STATESET;
	}
	
	// Geometry Checks:
	//		MESH:			NVERTEX_V3C4T2 and NPRIMITIVE_TRIANGLE  only.
	//		APPEARANCE:		At least 1 TextureUnit (obviously !)
	//						No other constraints ...
	NErrorIf( NGET_GEOMETRY_IDS_PRIMITIVE_FORMAT( NGetGeometrySpecs(rsid)->IDS )!= NPRIMITIVE_FORMAT_TRIANGLE	, NERROR_GEOMETRY_UNSUPPORTED_PRIMITIVE_FORMAT );
	NErrorIf( NGET_GEOMETRY_IDS_VERTEX_FORMAT( NGetGeometrySpecs(rsid)->IDS )	!= NVERTEX_FORMAT_V3C4T2		, NERROR_GEOMETRY_UNSUPPORTED_VERTEX_FORMAT );
	NErrorIf( NGetGeometrySpecs(rsid)->UsedTextureUnitNumber < 1, NERROR_GEOMETRY_UNSUPPORTED_TEXTUREUNIT_NUMBER );
	
	//Nf32	ParticleNbToAwake;
	pemittercomponent->ParticleNbToAwake = 0.0f;
	
	//Nu32	Flags;
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_AUTO_EMISSION) )
	{
		FLAG_ON(pemittercomponent->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_AUTO_EMISSION); 
	}
	else
	{
		FLAG_OFF(pemittercomponent->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_AUTO_EMISSION); 
	}
	//Nf32	BirthRate;
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_BIRTH_RATE) )
	{
		pemittercomponent->BirthRate = pdesc->BirthRate;
	}
	else
	{
		pemittercomponent->BirthRate = DEFAULT_NPARTICLE_EMITTERCOMPONENT_CORE_BIRTHRATE;
	}
	//Nu16	ParticlesNbMax;
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_PARTICLESNBMAX) )
	{
		pemittercomponent->ParticlesNbMax = NMIN(pdesc->ParticlesNbMax,CONSTANT_NPARTICLE_NB_MAX);
	}
	else
	{
		pemittercomponent->ParticlesNbMax = NMIN( DEFAULT_NPARTICLE_EMITTERCOMPONENT_CORE_PARTICLESNBMAX,CONSTANT_NPARTICLE_NB_MAX );
	}
	//Nu16	ParticlesNb;
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_INITIALPARTICLESNB) )
	{
		pemittercomponent->ParticlesNb = NMIN(pdesc->InitialParticlesNb,pemittercomponent->ParticlesNbMax);
	}
	else
	{
		pemittercomponent->ParticlesNb = 0;
	}

	//void	*pParticlesList;
	pemittercomponent->pParticlesList	= Nmalloc(pemittercomponent->ParticlesNbMax * sizeofparticle);
	memset(pemittercomponent->pParticlesList,0,pemittercomponent->ParticlesNbMax * sizeofparticle );
	
	// Nu16 ParticleStructSize
	// Useful for external process that would like parse the particle list without knowing the exact type of particle emitter.
	pemittercomponent->ParticleStructSize = _SafeNu32ToNu16(sizeofparticle);
	// ----------------------------------------------------------------------------------------
	// RENDERABLE CREATION
	NRENDERABLE_DESC	rdesc;
	memset(&rdesc,0,sizeof(NRENDERABLE_DESC));

	// RENDERABLE Bounding Sphere
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_BOUNDINGSPHERE) )
	{
		rdesc.BoundingSphere = pdesc->BoundingSphere;
	}
	else
	{
		rdesc.BoundingSphere.fRadius	= DEFAULT_NPARTICLE_EMITTERCOMPONENT_CORE_RENDERABLE_BOUNDINGSPHERE_RADIUS;
		NVec3Set(&rdesc.BoundingSphere.Center,DEFAULT_NPARTICLE_EMITTERCOMPONENT_CORE_RENDERABLE_BOUNDINGSPHERE_CENTER_3F);
	}
	
	// RENDERABLE Transform HNode
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_THN) )
	{
		rdesc.pTransformHNode = pdesc->pTransformHNode;
	}
	else
	{
		rdesc.pTransformHNode = NULL;
	}

	rdesc.IncludedGeometryPreallocatedCapacity	= 1;
	rdesc.Update_FCT							= pupdate_handle;
	rdesc.Extract_FCT							= NGetRenderableExtractFunctionByExtractSet(NMAKE_NEXTRACTSET_2P(CONSTANT_NPARTICLE_LOWLEVEL_EXTRACTSET,highlevel_extractset));
	pemittercomponent->pRenderable				= NCreateRenderable(&rdesc);
	// ----------------------------------------------------------------------------------------
	// UNIQUE GEOMETRY CREATION
	NGEOMETRY			*pgeom;
	NGEOMETRY_DESC		geomdesc;

	// CREATE AND INSERT The "unique" geometry inside
	NFillupGeometryDescEx( &geomdesc,	rsid, user_stateset_flags,
										pemittercomponent->ParticlesNbMax*2, pemittercomponent->ParticlesNbMax*2,
										pemittercomponent->ParticlesNbMax*4, pemittercomponent->ParticlesNbMax*4);
	pgeom = NGeometryEmplaceBack( pemittercomponent->pRenderable, &geomdesc );
	if( ISFLAG_ON(pdesc->Flags,FLAG_NPARTICLE_EMITTERCOMPONENT_CORE_DESC_USE_APPERANCERAW) )
	{
		NSetGeometryAppearance(pgeom,&pdesc->AppearanceRaw);
	}
	
	
	// Particle data PreComputing  
// 	NPARTICLEVERTEX	*pvertex	= (NPARTICLEVERTEX*)NGetFirstMeshVertexPtr(&pgeom->Mesh);
// 	NTRIANGLE	*ptri			= NGetFirstMeshTrianglePtr(&pgeom->Mesh);
	NPARTICLEVERTEX	*pvertex	= (NPARTICLEVERTEX*)pgeom->Mesh.VertexArray.pFirst;
	NTRIANGLE	*ptri			= (NTRIANGLE*)pgeom->Mesh.PrimitiveArray.pFirst;
	Nu16 index = 0;
	for(Nu16 i=pemittercomponent->ParticlesNbMax;i!=0;i--,index+=4)
	{
		// Initialize the 2 Primitives ( 2 triangles of the particle Quad )
		ptri->i0 = index;
		ptri->i1 = index + 1;
		ptri->i2 = index + 2;
		ptri++;
		ptri->i0 = index;
		ptri->i1 = index + 2;
		ptri->i2 = index + 3;
		ptri++;

		// Initialize  the 4 UVs ( for the 4 vertex of the particle Quad )
		pvertex->TexCoord0_2f.x = 0.0f;
		pvertex->TexCoord0_2f.y = 0.0f;
		pvertex++;

		pvertex->TexCoord0_2f.x = 1.0f;
		pvertex->TexCoord0_2f.y = 0.0f;
		pvertex++;
		
		pvertex->TexCoord0_2f.x = 1.0f;
		pvertex->TexCoord0_2f.y = 1.0f;
		pvertex++;

		pvertex->TexCoord0_2f.x = 0.0f;
		pvertex->TexCoord0_2f.y = 1.0f;
		pvertex++;

		// Vertex color still BLACK (full of 0 )
	}
}
inline void NClearParticlesEmitterComponent_Core( NPARTICLE_EMITTERCOMPONENT_CORE *pemittercomponent )
{
	NDeleteRenderable(pemittercomponent->pRenderable);
	Nfree(pemittercomponent->pParticlesList);
	Nmem0(pemittercomponent,NPARTICLE_EMITTERCOMPONENT_CORE);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif  // __NPARTICLECOMPONENT_CORE_H 

