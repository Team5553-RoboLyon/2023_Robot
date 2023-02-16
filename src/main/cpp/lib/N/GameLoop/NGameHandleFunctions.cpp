#include "../NCStandard.h"
#include "../NCore.h"
#include "../Event/NEvent.h"
#include "NGameHandleFunctions.h"

void WindowProc(const NEVENT* pevent,const Nu32 user_Nu32)
{
/*
	switch(pevent->Type)
	{

		default:
			break;
	}
*/
}
/*
void MoveCameraHandle(NEVENT* pevent,NTIME* ptime)
{
	switch(pevent->type)
	{

		case SDL_QUIT:
			continuer = false;
			break;


		case SDL_MOUSEMOTION:
			pmouseevent =(SDL_MouseMotionEvent*)pevent;

			if(pmouseevent->state & SDL_BUTTON_LMASK)
			{
				fangle += (float)pMouseEvent->xrel;
				fangle2 += (float)pMouseEvent->yrel/10.0f;
				Camera.Pos.y = cos(RAD(fangle2))*fcamdist;
				r = sin(RAD(fangle2))*fcamdist;
				Camera.Pos.x = cos(RAD(fangle))*r;
				Camera.Pos.z = sin(RAD(fangle))*r;
			}
			else if(pmouseevent->state & SDL_BUTTON_RMASK)
			{
				glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
				glGetDoublev( GL_PROJECTION_MATRIX, projection );
				glGetIntegerv( GL_VIEWPORT, viewport );

				winx = (GLdouble)pMouseEvent->x;
				winy = (GLdouble)viewport[3] - (GLdouble)pMouseEvent->y; // Hauteur ecran - y ( sinon y inversé )
				winz = 0;
				gluUnProject( winx, winy, winz, modelview, projection, viewport, &posX[0], &posY[0], &posZ[0]);
				vect.x = (float)posX[0] - Camera.Pos.x;
				vect.y = (float)posY[0] - Camera.Pos.y;
				vect.z = (float)posZ[0] - Camera.Pos.z;

				f = NVec3FastNormalize(&vect);//f = NVec3Normalize(&vect);
				NSetParticleInfluence_RayDirectorVector(pdeflector, &vect, &Camera.Pos);
				NVec3Scale(&vect,&vect,0.01f);
			}
			break;
	}
}
*/