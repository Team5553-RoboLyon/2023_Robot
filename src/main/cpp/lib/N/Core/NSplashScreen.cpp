// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NSplashScreen.cpp								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/GL/Ngl.h"
#include "../NTexture.h"
#include "NSplashScreen.h"
#include "../Geometry/Components/NGeometryVertex.h"
#include "../Geometry/Components/NGeometryPrimitive.h"
#include "../Render/Accumulator/NStateSet.h"
// #define NSCREEN_WIDTH		960
// #define NSCREEN_HEIGHT		640
#define NSCREEN_DEPTH		32

void NSplashScreen(const Nchar *psplash_screen_tga)
{
	NErrorIf(!psplash_screen_tga, NERROR_NULL_POINTER);
	NTEXTURE*	ptexture = NCreateTextureFromTga(NULL,psplash_screen_tga);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	GLint viewport[4];								// Where The Viewport Values Will Be Stored
	glGetIntegerv(GL_VIEWPORT, viewport);           // Retrieves The Viewport Values (X, Y, Width, Height)

	Nf32	ratio = (Nf32)viewport[2]/(Nf32)viewport[3];
	glOrtho( 0.0f,	ratio,	-1.0f,	0.0f,	-1.0f,	1.0f);

	// Render image
	// First Vertex
	
	NVERTEX_V3C4T2	vertex[4];
	NTRIANGLE		tri[2];
	
	Nf32 xtd_w = (Nf32)ptexture->size.x/((Nf32)viewport[3]*2.0f); 
	Nf32 xtd_h = (Nf32)ptexture->size.y/((Nf32)viewport[3]*2.0f); 
	
	ratio*= 0.5f;
	vertex[0].Position_3f.x =  ratio - xtd_w;	vertex[0].TexCoord0_2f.x = 0.0f;
	vertex[0].Position_3f.y = -0.5f + xtd_h;	vertex[0].TexCoord0_2f.y = 0.0f;
	vertex[0].Position_3f.z =  0.0f;			
												
	vertex[1].Position_3f.x =  ratio + xtd_w;	vertex[1].TexCoord0_2f.x = 1.0f;
	vertex[1].Position_3f.y = -0.5f + xtd_h;	vertex[1].TexCoord0_2f.y = 0.0f;
	vertex[1].Position_3f.z =  0.0f;			
												
	vertex[2].Position_3f.x =  ratio + xtd_w;	vertex[2].TexCoord0_2f.x = 1.0f;
	vertex[2].Position_3f.y = -0.5f - xtd_h;	vertex[2].TexCoord0_2f.y = 1.0f;
	vertex[2].Position_3f.z =  0.0f;			
												
	vertex[3].Position_3f.x =  ratio - xtd_w;	vertex[3].TexCoord0_2f.x = 0.0f;
	vertex[3].Position_3f.y = -0.5f - xtd_h;	vertex[3].TexCoord0_2f.y = 1.0f;
	vertex[3].Position_3f.z =  0.0f;			

	NSetColorf(&vertex[0].Color0_4f,NCOLOR_PRESET3F_WHITE,1);
	NSetColorf(&vertex[1].Color0_4f,NCOLOR_PRESET3F_WHITE,1);
	NSetColorf(&vertex[2].Color0_4f,NCOLOR_PRESET3F_WHITE,1);
	NSetColorf(&vertex[3].Color0_4f,NCOLOR_PRESET3F_WHITE,1);

	tri[0].i0 = 0;
	tri[0].i1 = 3;
	tri[0].i2 = 1;
	tri[1].i0 = 1;
	tri[1].i1 = 3;
	tri[1].i2 = 2;

	NSTATESET	stateset = FLAG_STATESET_TEXTURES|FLAG_STATESET_BLEND;
	NUpdateCurrentStateSet( stateset );

	glClearColor(1,1,1,1);
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	
#ifdef _NWINDOWS
	glBlendEquation(GL_FUNC_ADD);
#endif
	glBlendFunc( GL_ONE,GL_ONE_MINUS_SRC_ALPHA );

	

	// Attach texture
	glBindTexture(GL_TEXTURE_2D,ptexture->GL_Texture);
	glTexCoordPointer(2,GL_FLOAT,(GLsizei)sizeof(NVERTEX_V3C4T2),(Nf32*)&vertex[0].TexCoord0_2f);
	glVertexPointer(3,GL_FLOAT,(GLsizei)sizeof(NVERTEX_V3C4T2),(Nf32*)&vertex[0].Position_3f );
 	glColorPointer(4,GL_FLOAT,(GLsizei)sizeof(NVERTEX_V3C4T2),(Nf32*)&vertex[0].Color0_4f);

	// OPEN GL DRAW
	// 16 bits Version for IOS and 32 bits one for WINDOWS
	#ifdef _NWINDOWS
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(Nu32*)&tri->i0 );
	#endif
	
	#if defined  _NIOS || defined _NANDROID
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,(Nu16*)&tri->i0);
	#endif

/*
	#ifdef _NWINDOWS
	SDL_GL_SwapBuffers();								// Screen swap
	#endif
*/

	// Restore Previous Matrix Context 
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	NDeleteTexture(ptexture);
}