#include "../../NCStandard.h"
#include "../../Geometry/NGeometry.h"
#include "../../NErrorHandling.h"
#include "../../Maths/NMatrix3x3.h"
#include "NUT_Draw.h"
#include "NUT_DrawPencil.h"

extern NMATRIX3x3				NUT_DrawOrthonormal;

void NUT_Draw_Slider( const NUT_SLIDER *pslider, const NVEC3 *pposition, const NCOLOR *pcolor, const NUT_DRAWTEXT *ptxtparam )
{
	NUTDRAWVERTEX	v0,v1,v2;
	NVEC3		o,i,j,i1,j1;
	Nf32			f;

	v0.Color0_4f = v1.Color0_4f = v2.Color0_4f= *pcolor;
	// i represent the Slider direction in the Drawing Base. 
	i.x = NUT_DrawOrthonormal.Side.x * pslider->VDir.x + NUT_DrawOrthonormal.Up.x * pslider->VDir.y;
	i.y = NUT_DrawOrthonormal.Side.y * pslider->VDir.x + NUT_DrawOrthonormal.Up.y * pslider->VDir.y;
	i.z = NUT_DrawOrthonormal.Side.z * pslider->VDir.x + NUT_DrawOrthonormal.Up.z * pslider->VDir.y;

	// j an orthogonal vector to u in the Drawing Base XY plane
	// (simply apply i(ab) orthogonal to j(-b a) with i and j coplanar, but do this in R3)
	j.x = -NUT_DrawOrthonormal.Side.x * pslider->VDir.y + NUT_DrawOrthonormal.Up.x * pslider->VDir.x;
	j.y = -NUT_DrawOrthonormal.Side.y * pslider->VDir.y + NUT_DrawOrthonormal.Up.y * pslider->VDir.x;
	j.z = -NUT_DrawOrthonormal.Side.z * pslider->VDir.y + NUT_DrawOrthonormal.Up.z * pslider->VDir.x;

	// 2 possible ways:
	//		a/ user gives a 3D position which is the origin the 3D orthonormal to draw slider from, then use it and place the slider base from this point.
	//		b/ user doesn't give anything (NULL pointer actually), then assume the 3D orthonormal origin is {0,0,0} from slider base
	//		   to the WORLD Othogonal Base (simply add a z=0)
	o.x = NUT_DrawOrthonormal.Side.x * pslider->Base.x + NUT_DrawOrthonormal.Up.x * pslider->Base.y;
	o.y = NUT_DrawOrthonormal.Side.y * pslider->Base.x + NUT_DrawOrthonormal.Up.y * pslider->Base.y;
	o.z = NUT_DrawOrthonormal.Side.z * pslider->Base.x + NUT_DrawOrthonormal.Up.z * pslider->Base.y;

	if(pposition)
	{
		o.x += pposition->x;
		o.y += pposition->y;
		o.z += pposition->z;
	}
	//													^j
	//	Slider Scheme									|
	//		 |						|				|	|
	//		o|----------------------|---------------|	+--> i
	//		 |						|				|
	//
	//
	// -------------------------------------------------------
	// Draw the MAIN AXIS
	v0.Position_3f		= o;
	v1.Position_3f.x	= o.x + i.x*pslider->Size.x; //|
	v1.Position_3f.y	= o.y + i.y*pslider->Size.x; //|-pslider->Size.x represents the 'length' of the slider
	v1.Position_3f.z	= o.z + i.z*pslider->Size.x; //| 
	NUT_DrawPencil_Line(&v0,&v1);
	
	// -------------------------------------------------------
	// Setup ticks drawing
	// f represents the 'half thickness' of the slider
	f = pslider->Size.y*0.5f; 
	// scale j by f
	j1.x = f*j.x;
	j1.y = f*j.y;
	j1.z = f*j.z;

	// -------------------------------------------------------
	// Draw the Origin or "Min" Tick
	v0.Position_3f.x += j1.x;
	v0.Position_3f.y += j1.y;
	v0.Position_3f.z += j1.z;
	
	v2.Position_3f	  = o;
	v2.Position_3f.x -= j1.x;
	v2.Position_3f.y -= j1.y;
	v2.Position_3f.z -= j1.z;
	NUT_DrawPencil_Line(&v0,&v2);
	
	// -------------------------------------------------------
	// // Draw the "Max" Tick
	v0.Position_3f	  = v1.Position_3f;
	v0.Position_3f.x += j1.x;
	v0.Position_3f.y += j1.y;
	v0.Position_3f.z += j1.z;

	v2.Position_3f	  = v1.Position_3f;
	v2.Position_3f.x -= j1.x;
	v2.Position_3f.y -= j1.y;
	v2.Position_3f.z -= j1.z;
	NUT_DrawPencil_Line(&v0,&v2);

	// -------------------------------------------------------
	// Draw the "Handle" Tick
	v1.Position_3f.x  = o.x + i.x*pslider->Magnitude; //|
	v1.Position_3f.y  = o.y + i.y*pslider->Magnitude; //|-pslider->Magnitude represents the 'position' of the slider handle
	v1.Position_3f.z  = o.z + i.z*pslider->Magnitude; //|

	v0.Position_3f	  = v1.Position_3f;
	v0.Position_3f.x += j1.x;
	v0.Position_3f.y += j1.y;
	v0.Position_3f.z += j1.z;

	v2.Position_3f    = v1.Position_3f;
	v2.Position_3f.x -= j1.x;
	v2.Position_3f.y -= j1.y;
	v2.Position_3f.z -= j1.z;
	NUT_DrawPencil_Line(&v0,&v2);

	// -------------------------------------------------------
	// Intermediate Steps ?
	if(pslider->InterSteps && ISFLAG_ON(pslider->Flags,FLAG_NUT_SLIDER_SHOW_TICKS))
	{
		v0.Position_3f	  = o;
		v0.Position_3f.x += j1.x;
		v0.Position_3f.y += j1.y;
		v0.Position_3f.z += j1.z;

		v1.Position_3f	  = o;
		// 		v1.Position_3f.x -= j1.x;
		// 		v1.Position_3f.y -= j1.y;
		// 		v1.Position_3f.z -= j1.z;
		
		f	= pslider->Size.x/(Nf32)(pslider->InterSteps+1);
		i1.x = f*i.x;
		i1.y = f*i.y;
		i1.z = f*i.z;

		for(Nu16 s=0;s<pslider->InterSteps;s++)
		{
			v0.Position_3f.x += i1.x;
			v0.Position_3f.y += i1.y;
			v0.Position_3f.z += i1.z;
			v1.Position_3f.x += i1.x;
			v1.Position_3f.y += i1.y;
			v1.Position_3f.z += i1.z;
			NUT_DrawPencil_Line(&v0,&v1);
		}
	}

	// -------------------------------------------------------
	// Text the current value ?
	if( ISFLAG_ON(pslider->Flags,FLAG_NUT_SLIDER_SHOW_OUTPUTVALUE) )
	{
		Nchar	txt[64];
		// Recalculate out going value from current slider magnitude
		f = pslider->Min + ( pslider->Max - pslider->Min )*pslider->Magnitude/pslider->Size.x;
		sprintf(txt,STRING_NUT_SLIDER_PRINT_VALUE,f);	
		// Text position
		// build v, an orthogonal vector to u in the Drawing Base XY plane, take it's opposite (-v) and scale it by the text size + a Space relative to text size  
		// ... and add it to the right extremity of the slider handle tick ( v2 actually )
		NVEC3	v;
		v.x = v2.Position_3f.x - j.x*ptxtparam->Size*(1.0f + CONSTANT_NUT_DRAW_SLIDER_TEXT_SHIFT);
		v.y = v2.Position_3f.y - j.y*ptxtparam->Size*(1.0f + CONSTANT_NUT_DRAW_SLIDER_TEXT_SHIFT);
		v.z = v2.Position_3f.z - j.z*ptxtparam->Size*(1.0f + CONSTANT_NUT_DRAW_SLIDER_TEXT_SHIFT);

		if( ISFLAG_ON(pslider->Flags,FLAG_NUT_SLIDER_ORIENT_OUTPUTVALUE) )
		{
			// change text base ...
			NMATRIX3x3 base;
			base = NUT_DrawOrthonormal;
			NUT_DrawOrthonormal.Up		= i;
			NUT_DrawOrthonormal.Side	= j;NVec3ScaleBy(&NUT_DrawOrthonormal.Side,-1.0f); // because i becomes up, side becomes -j to stay in a RH direct base 
			//NUT_DrawOrthonormal.Forward	= the same;
			if (ptxtparam)
				NUT_Draw_Text(txt, &v, ptxtparam);
			else
			{
				NUT_DRAWTEXT txtparam;
				txtparam.Color = *pcolor;
				txtparam.Size = pslider->Size.y*0.5f;
				NUT_Draw_Text(txt, &v, &txtparam);
			}
			NUT_DrawOrthonormal = base; // restore NUT_DrawOrthonormal
		}
		else
		{
			if (ptxtparam)
				NUT_Draw_Text(txt, &v, ptxtparam);
			else
			{
				NUT_DRAWTEXT txtparam;
				txtparam.Color = *pcolor;
				txtparam.Size = pslider->Size.y*0.5f;
				NUT_Draw_Text(txt, &v, &txtparam);
			}
		}
	}
}

void NUT_Draw_XYSlider( const NUT_SLIDER *pslider, const NVEC3 *pposition, const NCOLOR *pcolor, const NUT_DRAWTEXT *ptxtparam )
{
	NUTDRAWVERTEX	v0,v1,v2;
	NVEC3		o,i,j,i1,j1;	
	Nf32			f;

	v0.Color0_4f = v1.Color0_4f = v2.Color0_4f= *pcolor;
	// i, represent the Slider direction in the WORLD Base. Slider direction is a unit vector
	i.x = pslider->VDir.x;
	i.y = pslider->VDir.y;
	i.z = 0.0f;

	// j, an orthogonal vector to u in the WORLD Base. Uses to build ticks and handle and text positioning
	j.x = -i.y;
	j.y =  i.x;
	j.z =  0.0f;

	// o represents the "slider origin" in the WORLD Base. Slider Origin is not base origin !
	// Slider origin is the point where slider value is equal to it's minimum.
	// Slider Origin has its own coordinates in the XY plane of the used orthonormal base.
	
	// 2 possible ways:
	//		a/ user gives a 3D position which is the origin the 3D orthonormal to draw slider from, then use it and place the slider origin from this point.
	//		b/ user doesn't give anything (NULL pointer actually), then assume the 3D orthonormal origin is {0,0,0} from slider internal origin 
	//		   to the WORLD Base (simply add a z=0)
	o.x = pslider->Base.x;
	o.y = pslider->Base.y;
	o.z = 0.0f;

	if(pposition)
	{
		o.x += pposition->x;
		o.y += pposition->y;
		o.z += pposition->z;
	}

	//													^j
	//	Slider Scheme									|
	//		 |						|				|	|
	//		o|----------------------|---------------|	+--> i
	//		 |						|				|
	//
	//
	// -------------------------------------------------------
	// Draw the MAIN AXIS
	v0.Position_3f		= o;
	v1.Position_3f.x	= o.x + i.x*pslider->Size.x; //|
	v1.Position_3f.y	= o.y + i.y*pslider->Size.x; //|-pslider->Size.x represents the 'length' of the slider
	v1.Position_3f.z	= o.z;						 //| (i.z = 0.0f, so 'i.z*pslider->Size.x = 0')
	NUT_DrawPencil_Line(&v0,&v1);

	// -------------------------------------------------------
	// Setup ticks drawing
	// f represents the 'half thickness' of the slider
	f = pslider->Size.y*0.5f; 
	// scale j by f
	j1.x = j.x*f;
	j1.y = j.y*f;
	j1.z = 0.0f; // because j.z = 0.0f so j.z*f=0;

	// -------------------------------------------------------
	// Draw the Origin or "Min" Tick
	v0.Position_3f.x += j1.x;
	v0.Position_3f.y += j1.y;
	// v0.Position_3f.z += j1.z; ....j1.z = 0.0f so v0.Position_3f.z + j1.z = v0.Position_3f.z !!

	v2.Position_3f	  = o;
	v2.Position_3f.x -= j1.x;
	v2.Position_3f.y -= j1.y;
	// v2.Position_3f.z -= j1.z; ....j1.z = 0.0f  so v2.Position_3f.z - j1.z = v2.Position_3f.z !!
	NUT_DrawPencil_Line(&v0,&v2);

	// -------------------------------------------------------
	// // Draw the "Max" Tick
	v0.Position_3f	  = v1.Position_3f;
	v0.Position_3f.x += j1.x;
	v0.Position_3f.y += j1.y;
	// v0.Position_3f.z += j1.z; ....j1.z = 0.0f so v0.Position_3f.z + j1.z = v0.Position_3f.z !!

	v2.Position_3f	  = v1.Position_3f;
	v2.Position_3f.x -= j1.x;
	v2.Position_3f.y -= j1.y;
	// v2.Position_3f.z -= j1.z; ....j1.z = 0.0f  so v2.Position_3f.z - j1.z = v2.Position_3f.z !!
	NUT_DrawPencil_Line(&v0,&v2);

	// -------------------------------------------------------
	// Draw the "Handle" Tick
	v1.Position_3f.x  = o.x + i.x*pslider->Magnitude; //|
	v1.Position_3f.y  = o.y + i.y*pslider->Magnitude; //|-pslider->Magnitude represents the 'position' of the slider handle
	v1.Position_3f.z  = o.z;						  //| (i.z = 0.0f, so 'i.z*pslider->Magnitude = 0')

	v0.Position_3f	  = v1.Position_3f;
	v0.Position_3f.x += j1.x;
	v0.Position_3f.y += j1.y;
	// v0.Position_3f.z += j1.z; ....j1.z = 0.0f so v0.Position_3f.z + j1.z = v0.Position_3f.z !!

	v2.Position_3f    = v1.Position_3f;
	v2.Position_3f.x -= j1.x;
	v2.Position_3f.y -= j1.y;
	// v2.Position_3f.z -= j1.z; ....j1.z = 0.0f  so v2.Position_3f.z - j1.z = v2.Position_3f.z !!
	NUT_DrawPencil_Line(&v0,&v2);

	// -------------------------------------------------------
	// Intermediate Steps ?
	if(pslider->InterSteps && ISFLAG_ON(pslider->Flags,FLAG_NUT_SLIDER_SHOW_TICKS))
	{
		v0.Position_3f	  = o;
		v0.Position_3f.x += j1.x;
		v0.Position_3f.y += j1.y;
		// v0.Position_3f.z += j1.z; ....j1.z = 0.0f so v0.Position_3f.z + j1.z = v0.Position_3f.z !!

		v1.Position_3f	  = o;
		// 		v1.Position_3f.x -= j1.x;
		// 		v1.Position_3f.y -= j1.y;
		// 		v1.Position_3f.z -= j1.z;

		f	= pslider->Size.x/(Nf32)(pslider->InterSteps+1);
		i1.x = i.x * f;
		i1.y = i.y * f;
		i1.z = 0.0f;//i.z = 0; ... i.z*f = 0
		

		for(Nu16 s=0;s<pslider->InterSteps;s++)
		{
			v0.Position_3f.x += i1.x;
			v0.Position_3f.y += i1.y;
			//v0.Position_3f.z += i1.z;
			v1.Position_3f.x += i1.x;
			v1.Position_3f.y += i1.y;
			//v1.Position_3f.z += i1.z;
			NUT_DrawPencil_Line(&v0,&v1);
		}
	}
	// -------------------------------------------------------
	// Text the current value ?
	if( ISFLAG_ON(pslider->Flags,FLAG_NUT_SLIDER_SHOW_OUTPUTVALUE) )
	{
		Nchar	txt[64];
		// Recalculate out going value from current slider magnitude
		f = pslider->Min + ( pslider->Max - pslider->Min )*pslider->Magnitude/pslider->Size.x;
		sprintf(txt,STRING_NUT_SLIDER_PRINT_VALUE,f);
		
		// Text position
		// build v, an orthogonal vector to u in the Drawing Base XY plane, take it's opposite (-v) and scale it by the text size + a Space relative to text size  
		// ... and add it to the right extremity of the slider handle tick ( v2 actually )
		NVEC3	v;
		v.x = v2.Position_3f.x - j.x*ptxtparam->Size*(1.0f + CONSTANT_NUT_DRAW_SLIDER_TEXT_SHIFT);
		v.y = v2.Position_3f.y - j.y*ptxtparam->Size*(1.0f + CONSTANT_NUT_DRAW_SLIDER_TEXT_SHIFT);
		v.z = v2.Position_3f.z;

		if( ISFLAG_ON(pslider->Flags,FLAG_NUT_SLIDER_ORIENT_OUTPUTVALUE) )
		{
			// change text base ...
			NMATRIX3x3 base;
			base = NUT_DrawOrthonormal;
			NUT_DrawOrthonormal.Up		= i;
			NUT_DrawOrthonormal.Side	= j;NVec3ScaleBy(&NUT_DrawOrthonormal.Side,-1.0f); // because i becomes up, side becomes -j to stay in a RH direct base 
			//NUT_DrawOrthonormal.Forward	= the same;
			
			if(ptxtparam)
				NUT_Draw_Text(txt,&v,ptxtparam);
			else
			{
				NUT_DRAWTEXT txtparam;
				txtparam.Color	= *pcolor;
				txtparam.Size = pslider->Size.y*0.5f;
				NUT_Draw_Text(txt, &v, &txtparam);
			}
			NUT_DrawOrthonormal = base; // restore NUT_DrawOrthonormal
		}
		else
		{
			if (ptxtparam)
				NUT_Draw_Text(txt, &v, ptxtparam);
			else
			{
				NUT_DRAWTEXT txtparam;
				txtparam.Color = *pcolor;
				txtparam.Size = pslider->Size.y*0.5f;
				NUT_Draw_Text(txt, &v, &txtparam);
			}
		}
	}
}

