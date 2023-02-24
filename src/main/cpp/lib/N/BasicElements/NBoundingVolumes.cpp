#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/GL/Ngl.h"
#include "../Maths/NVec3f32.h"
#include "lib/N/Geometry/NGeometry.h"
#include "../NCamera.h"
#include "../Containers/NArray.h"
#include "../NBoundingVolumes.h"
#include "lib/N/NErrorHandling.h"

// IMPORTANT NOTES:
// Here we have a collection of several functions to easily calculate/update Bounding Volume.
// FOR RENDERABLE BOUNDING VOLUMES
// Renderable Bounding Volume Update has to be performed by user when its necessary.
// ... for example at the end of the "Update and extract" function link with the Renderable
// ... or in some specific cases inside game code.

// ------------------------------------------------------------------------------------------
// NGetAABBBoundingSphere
// ------------------------------------------------------------------------------------------
// Description :
// ------------------------------------------------------------------------------------------
// In  :
// Out :
//
// ------------------------------------------------------------------------------------------
