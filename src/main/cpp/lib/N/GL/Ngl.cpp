#include "../NCore.h"
#include "../NViewport.h"
#include "Ngl.h"

Nbool Ngl_Init()
{
	if( ISFLAG_ON(NEngineCoreFlags,FLAG_NENGINE_CORE_GL_INITIALIZED) )
		return 1;

	#ifdef _NWINDOWS
// 	SDL_Init(SDL_INIT_VIDEO);										// SDL Init

	// IOS
	// SDL_SetVideoMode(320, 200, 32, SDL_OPENGL); 			// Aspect Ratio: 1.6
	// SDL_SetVideoMode(480, 320, 32, SDL_OPENGL); 			// Aspect Ratio: 1.5
	// SDL_SetVideoMode(960, 640, 32, SDL_OPENGL); 			// Aspect Ratio: 1.5
	// SDL_SetVideoMode(1136, 640, 32, SDL_OPENGL); 		// Aspect Ratio: 1.775
	// SDL_SetVideoMode(1136, 750, 32, SDL_OPENGL); 		// Aspect Ratio: 1.51466667
	// SDL_SetVideoMode(1334, 750, 32, SDL_OPENGL); 		// Aspect Ratio: 1.77866667
	// SDL_SetVideoMode(2208, 1242, 32, SDL_OPENGL); 		// Aspect Ratio: 1.77777778
	// SDL_SetVideoMode(2436, 1125, 32, SDL_OPENGL); 		// Aspect Ratio: 2.1653333
	
	// ANDROID
//	 SDL_SetVideoMode(1280, 720, 32, SDL_OPENGL); 		// Aspect Ratio: 1.77777778
	 // SDL_SetVideoMode(1280, 800, 32, SDL_OPENGL); 		// Aspect Ratio: 1.6
	 // SDL_SetVideoMode(1920, 1080, 32, SDL_OPENGL); 		// Aspect Ratio: 1.77777778
	 // SDL_SetVideoMode(1920, 1200, 32, SDL_OPENGL); 		// Aspect Ratio: 1.6
	 // SDL_SetVideoMode(2560, 1440, 32, SDL_OPENGL); 		// Aspect Ratio: 1.77777778
															
	// glViewport(0,0,960,640); useless because already done by default by "SDL_SetVideoMode" ( i suppose )

	glClearColor(CONSTANT_DEFAULT_CLEAR_COLOR4F);
	if(  extgl_Initialize() )
	{
		//SDL_Quit();	// SDL Quit 
		return 0;	// No GL extension available ... Bye !
	}
	#endif
	
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	#ifdef _NWINDOWS
	glEnableClientState(GL_INDEX_ARRAY);
	#endif

	// Opengl has been initialized (it should be)
	// So, we can Pre-calculate ViewPort Constants
	NInitializeViewPortConstants();


	// Reset Current StateSet
	NCurrentStateSet = 0;

	// ZBUFFER READ/WRITE ----------------------------------------------------------------
	glDepthMask(GL_FALSE);							// FLAG_OFF( NCurrentStateSet,FLAG_STATESET_ZWRITE );
	glDisable(GL_DEPTH_TEST);						// FLAG_OFF( NCurrentStateSet,FLAG_STATESET_ZREAD  );
	// TEXTURE ---------------------------------------------------------------------------
	glDisable(GL_TEXTURE_2D);						// FLAG_OFF( NCurrentStateSet,FLAG_STATESET_TEXTURES );	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	// BLEND mode ------------------------------------------------------------------------
	glDisable(GL_BLEND);							// FLAG_OFF( NCurrentStateSet,FLAG_STATESET_BLEND );
	// LIGHT mode -------------------------------------------------------------------------
	glDisable(GL_LIGHTING);							// FLAG_OFF( NCurrentStateSet,FLAG_STATESET_LIGHT );

	FLAG_ON(NEngineCoreFlags,FLAG_NENGINE_CORE_GL_INITIALIZED);
	return 1;
}