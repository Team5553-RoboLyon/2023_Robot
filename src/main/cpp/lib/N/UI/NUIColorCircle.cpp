#include "../NCStandard.h"
#include "../NErrorHandling.h"
#include "../NCoordinateSystemConversions.h"
#include "../Event/NEventTouch.h"
#include "NUI.h"
#include "NUIColorCircle.h"

#include "../Utilities/NUT_Shape.h"

Nchar* NUIColorCircle_SetupDisplayableInfos_RGBAString(Nchar *pdest, const Nchar *pcaption, const Nu8 red, const Nu8 green, const Nu8 blue, const Nu8 alpha )
{
	if(pcaption)
		sprintf(pdest,"%sR:%d, V:%d, B:%d, A:%d",pcaption,red,green,blue,alpha);
	else
		sprintf(pdest,"R:%d, V:%d, B:%d, A:%d",red,green,blue,alpha);
	return pdest;
}
Nchar* NUIColorCircle_SetupDisplayableInfos_RGBString(Nchar *pdest, const Nchar *pcaption, const Nu8 red, const Nu8 green, const Nu8 blue )
{
	if(pcaption)
		sprintf(pdest,"%sR:%d, V:%d, B:%d",pcaption,red,green,blue);
	else
		sprintf(pdest,"R:%d, V:%d, B:%d",red,green,blue);
	return pdest;
}
static inline void _printcurrentcolorvalues(NUI_COLORCIRCLE *pcc)
{
	Nchar	fullcolorcircle_caption[CONSTANT_NUI_COLORCIRCLE_INFOS_STRING_SIZEMAX + CONSTANT_NUI_NAME_SIZEMAX];
	Nu32	color8;
	if(	 ISFLAG_ON(pcc->UI.Flags,FLAG_NUI_COLORCIRCLE_COLOR_INFOS_PRINTING)  )
	{
		// start with a right initialized empty string ...
		fullcolorcircle_caption[0]=0;
		// Nu8 Printing infos ... always !
		NCOLOR_To_RGBA8(&color8,&pcc->Color);
		if( ISFLAG_ON(pcc->UI.Flags,FLAG_NUI_COLORCIRCLE_RGBA ) )
			NUIColorCircle_SetupDisplayableInfos_RGBAString(fullcolorcircle_caption,pcc->UI.pName,RGBA8_R(color8),RGBA8_G(color8),RGBA8_B(color8),RGBA8_A(color8));
		else
			NUIColorCircle_SetupDisplayableInfos_RGBString(fullcolorcircle_caption,pcc->UI.pName,RGBA8_R(color8),RGBA8_G(color8),RGBA8_B(color8));
		NBuildUITextMeshFromString((NUI*)pcc, fullcolorcircle_caption);
	}
}

static inline void _update_colorcircle_vertexcolor( NUI_COLORCIRCLE *pc )
{
	NVERTEX_V3C4	*pvertex;
	Nu16			i,j;
	Nf32			t;
	const NCOLOR	colorsextant[7]={
										{ 1.0f, 0.0f, 0.0f, 1.0f },
										{ 1.0f, 1.0f, 0.0f, 1.0f },
										{ 0.0f, 1.0f, 0.0f, 1.0f },
										{ 0.0f, 1.0f, 1.0f, 1.0f },
										{ 0.0f, 0.0f, 1.0f, 1.0f },
										{ 1.0f, 0.0f, 1.0f, 1.0f },
										{ 1.0f, 0.0f, 0.0f, 1.0f }	// == colorsextant[0] but like this for a simple code
									};

	// Color Circle: Update Vertex Color
	//	According to the current Brightness value
	pvertex = &((NVERTEX_V3C4*)pc->pCircle_Geometry->Mesh.VertexArray.pFirst)[pc->Circle_FirstVertex];
	for(i=0;i<6;i++) 
	{
		for(j=0;j<pc->Circle_AdditionalSextantSubdivision+1;j++,pvertex++)
		{
			t = (Nf32)j/(Nf32)(pc->Circle_AdditionalSextantSubdivision+1);

			pvertex->Color0_4f.red		= pc->HSV.Brightness * ( colorsextant[i].red +  (colorsextant[i+1].red - colorsextant[i].red)*t );
			pvertex->Color0_4f.green	= pc->HSV.Brightness * ( colorsextant[i].green +  (colorsextant[i+1].green - colorsextant[i].green)*t );
			pvertex->Color0_4f.blue		= pc->HSV.Brightness * ( colorsextant[i].blue +  (colorsextant[i+1].blue - colorsextant[i].blue)*t );
			pvertex->Color0_4f.alpha	= 1.0f;
		}
	}
	pvertex = &((NVERTEX_V3C4*)pc->pCircle_Geometry->Mesh.VertexArray.pFirst)[pc->Circle_CenterVertex];
	pvertex->Color0_4f.red		= pc->HSV.Brightness;
	pvertex->Color0_4f.green	= pc->HSV.Brightness;
	pvertex->Color0_4f.blue		= pc->HSV.Brightness;
	pvertex->Color0_4f.alpha	= 1.0f;
}

static inline void _update_alpha_arc_vertexcolor( NUI_COLORCIRCLE *pc )
{
	NCOLOR			backcolor;
	NVERTEX_V3C4	*pvertex;
	Nu32			i;
	Nf32			tcol,tcolstep;

	NErrorIf(NISODD(pc->ArcAlpha_VertexSize),NERROR_ODD_NUMBER ); // !!! It has to be EVEN !

	if( ISFLAG_ON(pc->UI.Flags,FLAG_NUI_COLORCIRCLE_RGBA ) )
	{
		// Of course the 4 backcolor setup lines bellow are just made for a best reliability, nothing mathematically correct !
		// i mean, multiply by Brightness is just to keep a visible difference between the 100% alpha and 0% alpha which is 
		// here simulated by ramping color from backcolor !!!
		backcolor.red	= CONSTANT_NUI_COLORCIRCLE_ALPHAARC_BACK_COLOR_RED*pc->HSV.Brightness;
		backcolor.green = CONSTANT_NUI_COLORCIRCLE_ALPHAARC_BACK_COLOR_GREEN*pc->HSV.Brightness;
		backcolor.blue	= CONSTANT_NUI_COLORCIRCLE_ALPHAARC_BACK_COLOR_BLUE*pc->HSV.Brightness;
		backcolor.alpha = CONSTANT_NUI_COLORCIRCLE_ALPHAARC_BACK_COLOR_ALPHA;
		
		pvertex = &((NVERTEX_V3C4*)pc->pCircle_Geometry->Mesh.VertexArray.pFirst)[pc->ArcAlpha_FirstVertex];
		tcol = 0.0f;
		tcolstep = 2.0f/pc->ArcAlpha_VertexSize;
		for(i=pc->ArcAlpha_VertexSize/2;i!=0;i--,tcol+=tcolstep)
		{
			NColorLerp( &pvertex->Color0_4f,&backcolor,&pc->Color,tcol );
			pvertex++;
			NColorLerp( &pvertex->Color0_4f,&backcolor,&pc->Color,tcol );
			pvertex++;
		}
	}
	else
	{
		pvertex = &((NVERTEX_V3C4*)pc->pCircle_Geometry->Mesh.VertexArray.pFirst)[pc->ArcAlpha_FirstVertex];
		for(i=pc->ArcAlpha_VertexSize;i!=0;i--,pvertex++)
		{
			pvertex->Color0_4f = pc->Color;
		}
	}
}


static inline void _place_hue_saturation_selector( NUI_COLORCIRCLE *pc )
{
	Nu32			i;
	Nf32			cos,sin;
	NVERTEX_V3C4	*pvertex;
	NVEC2		newpos;

	NFastCosSin( (Nu32)(pc->HSV.Hue * (Nf32)NANGLELUT_SIZE),&cos,&sin );
	newpos.x = pc->ColorRadius*pc->HSV.Saturation*cos;
	newpos.y = pc->ColorRadius*pc->HSV.Saturation*sin;

	pvertex = &((NVERTEX_V3C4*)pc->pCircle_Geometry->Mesh.VertexArray.pFirst)[pc->SelectorColor_FirstVertex];
	for(i=pc->SelectorColor_VertexSize;i!=0;i--,pvertex++)
	{
		pvertex->Position_3f.x = pvertex->Position_3f.x - pc->SelectorColor_Position.x + newpos.x;
		pvertex->Position_3f.y = pvertex->Position_3f.y - pc->SelectorColor_Position.y + newpos.y;
		// pvertex->z += pc->SelectorColor_Position.z;
	}
	pc->SelectorColor_Position.x = newpos.x;
	pc->SelectorColor_Position.y = newpos.y;
}

static inline void _place_brightness_selector( NUI_COLORCIRCLE *pc )
{
	Nu32			i;
	Nf32			cos,sin;
	NVERTEX_V3C4	*pvertex;
	NVEC2		newpos;

	NFastCosSin( NDEGtoANGLE(CONSTANT_NUI_COLORCIRCLE_BRIGHTNESSMIN_ANGLEDEG - pc->HSV.Brightness * CONSTANT_NUI_COLORCIRCLE_BRIGHTNESSRANGE_ANGLEDEG ),&cos,&sin );
	newpos.x = cos*(pc->SpacingRadius+pc->ArcsRadius)*0.5f;
	newpos.y = sin*(pc->SpacingRadius+pc->ArcsRadius)*0.5f;

	pvertex = &((NVERTEX_V3C4*)pc->pCircle_Geometry->Mesh.VertexArray.pFirst)[pc->SelectorBright_FirstVertex];
	for(i=pc->SelectorBright_VertexSize;i!=0;i--,pvertex++)
	{
		pvertex->Position_3f.x = pvertex->Position_3f.x - pc->SelectorBright_Position.x + newpos.x;
		pvertex->Position_3f.y = pvertex->Position_3f.y - pc->SelectorBright_Position.y + newpos.y;
		// pvertex->z += pc->SelectorColor_Position.z;
	}
	pc->SelectorBright_Position.x = newpos.x;
	pc->SelectorBright_Position.y = newpos.y;
}

static inline void _place_alpha_selector( NUI_COLORCIRCLE *pc )
{
	Nu32			i;
	Nf32			cos,sin;
	NVERTEX_V3C4	*pvertex;
	NVEC2		newpos;

	NFastCosSin( NDEGtoANGLE(CONSTANT_NUI_COLORCIRCLE_ALPHAMIN_ANGLEDEG + pc->Color.alpha * CONSTANT_NUI_COLORCIRCLE_ALPHARANGE_ANGLEDEG),&cos,&sin );
	newpos.x = cos*(pc->SpacingRadius+pc->ArcsRadius)*0.5f;
	newpos.y = sin*(pc->SpacingRadius+pc->ArcsRadius)*0.5f;

	pvertex = &((NVERTEX_V3C4*)pc->pCircle_Geometry->Mesh.VertexArray.pFirst)[pc->SelectorAlpha_FirstVertex];
	for(i=pc->SelectorAlpha_VertexSize;i!=0;i--,pvertex++)
	{
		pvertex->Position_3f.x = pvertex->Position_3f.x - pc->SelectorAlpha_Position.x + newpos.x;
		pvertex->Position_3f.y = pvertex->Position_3f.y - pc->SelectorAlpha_Position.y + newpos.y;
		// pvertex->z += pc->SelectorColor_Position.z;
	}
	pc->SelectorAlpha_Position.x = newpos.x;
	pc->SelectorAlpha_Position.y = newpos.y;
}
static inline void _colorcircle_end_command(NUI *pui)
{
	NEVENT nevent;

	memset(&nevent,0,sizeof(NEVENT));
	if( ISFLAG_ON(pui->Flags,FLAG_NUI_COLORCIRCLE_NU8_COMMAND_FORMAT) )
	{
		NFillupEventHead(nevent,NCOMMAND_COLORCIRCLE_RGBA8,0,NEVENT_RECIPIENT_CAST_UI,pui);
		NCOLOR_To_RGBA8( &nevent.ColorCircle_RGBA8.Color,&((NUI_COLORCIRCLE*)pui)->Color );
	}
	else
	{
		NFillupEventHead(nevent,NCOMMAND_COLORCIRCLE_RGBA32,0,NEVENT_RECIPIENT_CAST_UI,pui);
		nevent.ColorCircle_RGBA32.Color = ((NUI_COLORCIRCLE*)pui)->Color;
	}
	NSendUICommandEvent(&nevent);
}

static inline void _colorcircle_inprogress_command(NUI *pui)
{
	NEVENT nevent;

	if( ISFLAG_ON(pui->Flags,FLAG_NUI_COLORCIRCLE_INPROGRESS_COMMAND) )
	{
		memset(&nevent,0,sizeof(NEVENT));
		if( ISFLAG_ON(pui->Flags,FLAG_NUI_COLORCIRCLE_NU8_COMMAND_FORMAT) )
		{

			NFillupEventHead(nevent,NCOMMAND_COLORCIRCLE_RGBA8,0,NEVENT_RECIPIENT_CAST_UI,pui);
			NCOLOR_To_RGBA8( &nevent.ColorCircle_RGBA8.Color,&((NUI_COLORCIRCLE*)pui)->Color );
		}
		else
		{
			NFillupEventHead(nevent,NCOMMAND_COLORCIRCLE_RGBA32,0,NEVENT_RECIPIENT_CAST_UI,pui);
			nevent.ColorCircle_RGBA32.Color = ((NUI_COLORCIRCLE*)pui)->Color;
		}
		NSendUICommandEvent(&nevent);
	}
}
void NUIColorCircle_EventToState_Handle(NUI *pui, const NEVENT *pevent)
{

	//	Check the recipient to be sure This UI is the recipient of the Event !!!
	//	BECAUSE it could be intercepting a child Event (which is weird for a ColorCircle but anyway ...) 
	//  in that case function returns immediately ...
	if( (NUI*)pevent->Head.pRecipient != pui )
		return;

	Nf32					sinmin,sinmax;
	Nf32					angle;
	Nf32					dist,distx,disty;
	NVEC2				mouse;
	NUI_COLORCIRCLE			*pcc;

	switch(pevent->Type)
	{
		case NTOUCH_START:
			switch( pui->CurrentState )
			{
				case NUI_STATE_COLORCIRCLE_REST:
					pcc = (NUI_COLORCIRCLE*)pui;
					
					// Normalize pointer coordinates
// 					mouse.x = NCoord_PixToHRel_X(pevent->Touch.Position.x);
// 					mouse.y = NCoord_PixToHRel_Y(-pevent->Touch.Position.y);
					
					NCoord_PixToHRel( &mouse, NGetEventTouchPositionPtr(pevent,0) );

					// Get and Store Transformed Center !
					NMulVector3ByMatrixO( &pcc->TransformedCenter_Position,
											(NMATRIX*)&pui->pRenderable->pTransformHNode->WorldMatrix,
											&((NVEC3*)NGetFirstArrayPtr(&pcc->pCircle_Geometry->Mesh.VertexArray))[pcc->Circle_CenterVertex]);
					
					// And test what user wants to update
					distx = mouse.x - pcc->TransformedCenter_Position.x;
					disty = mouse.y - pcc->TransformedCenter_Position.y;
					dist  = NFastSqrt( distx*distx + disty*disty );
					
					// Touch position is inside the Full Color Circle !
					// ( Full means Arcs included )
					if( dist <= pcc->ArcsRadius )
					{
						// Inside "Out Circle Crown" !!!
						if(dist >= pcc->SpacingRadius) 
						{
							//disty /= dist;//pcc->ArcsRadius;	// normalized disty against ArcsRadius...
																// This way we can compare dist with sin value
							//disty = NMIN(NMAX(-1.0f,disty/((pcc->ArcsRadius+pcc->SpacingRadius)*0.5f)),1.0f);
							disty = NMIN(NMAX(-1.0f,disty/dist),1.0f);
							// Here, we have to decide what arc does user select ...  
							// On the Left: Brightness
							if(distx < 0.0f )
							{
								// BRIGHTNESS ARC SELECTED !!!
								// We have to transform Touch position into Brightness Value ...

								sinmin = NFastSin(NDEGtoANGLE(CONSTANT_NUI_COLORCIRCLE_BRIGHTNESSMIN_PLUS_MARGIN_ANGLEDEG));
								sinmax = NFastSin(NDEGtoANGLE(CONSTANT_NUI_COLORCIRCLE_BRIGHTNESSMAX_PLUS_MARGIN_ANGLEDEG));
								if( (disty >= sinmin)&&(disty<=sinmax) )
								{

									angle = 180.0f-NRADtoDEG(asin(disty));
									if(angle<0.0f)
										angle+=360.0f;
									
									NUISetColorCircleColorHSV_V(pcc, NMIN( NMAX( -(angle - CONSTANT_NUI_COLORCIRCLE_BRIGHTNESSMIN_ANGLEDEG)/CONSTANT_NUI_COLORCIRCLE_BRIGHTNESSRANGE_ANGLEDEG,0.0f),1.0f ) );
									_colorcircle_inprogress_command(pui);
									// Update current STATE

									pui->CurrentState = NUI_STATE_COLORCIRCLE_BRIGHTNESS_UPDATE;
									break;
								}

							}
							// On the Right: Alpha
							else //(distx >= 0.0f )
							{
								if( ISFLAG_ON(pui->Flags,FLAG_NUIDC_COLORCIRCLE_RGBA) )
								{
									// ALPHA ARC SELECTED !!!
									// We have to transform Touch position into ALPHA Value ...
									sinmin = NFastSin(NDEGtoANGLE(CONSTANT_NUI_COLORCIRCLE_ALPHAMIN_PLUS_MARGIN_ANGLEDEG));
									sinmax = NFastSin(NDEGtoANGLE(CONSTANT_NUI_COLORCIRCLE_ALPHAMAX_PLUS_MARGIN_ANGLEDEG));
									if( (disty >= sinmin)&&(disty<=sinmax) )
									{
										angle = NRADtoDEG(asin(disty))- CONSTANT_NUI_COLORCIRCLE_ALPHAMIN_ANGLEDEG;
										if(angle<0.0f)
											angle+=360.0f;

										NUISetColorCircleColorAlpha(pcc, NMIN( NMAX(angle/CONSTANT_NUI_COLORCIRCLE_ALPHARANGE_ANGLEDEG,0.0f),1.0f ) );
										_colorcircle_inprogress_command(pui);

										// Update current STATE
										pui->CurrentState = NUI_STATE_COLORCIRCLE_ALPHA_UPDATE;
										break;
									}
								}
							}
						}
						else // (dist <= pcc->SpacingRadius) 
						{
							// Inside ColorCircle
							// Hue and Saturation calculations ...
							if( dist <= pcc->ColorRadius ) 
							{
								// MANUAL HSV AND NCOLOR UPDATE !
								//Hue
								distx = NMIN(NMAX(-1.0f,distx/dist),1.0f);
								
								angle = acos(distx);
								if(disty<=0.0f)
								{
									angle = NF32_2PI-angle;
								}

								pcc->HSV.Hue = angle/NF32_2PI;
								
								// Saturation
								pcc->HSV.Saturation = NMIN(1.0f,dist/pcc->ColorRadius);

								// Manual updating of NCOLOR member
								NHSV_To_NCOLOR(&pcc->Color,&pcc->HSV,-1);
								_colorcircle_inprogress_command(pui);

								// Update current STATE
								pui->CurrentState = NUI_STATE_COLORCIRCLE_HUE_SATURATION_UPDATE;
								break;
							}
						}
					}
					// if we arrive here ...
					break;

				default:
					break;
			}
			break;
		
		case NTOUCH_MOVE:
			switch( pui->CurrentState )
			{
				case NUI_STATE_COLORCIRCLE_HUE_SATURATION_WAIT:
					pcc = (NUI_COLORCIRCLE*)pui;
					// Normalize pointer coordinates
// 					mouse.x = NCoord_PixToHRel_X(pevent->Touch.Position.x);
// 					mouse.y = NCoord_PixToHRel_Y(-pevent->Touch.Position.y);
					NCoord_PixToHRel( &mouse, NGetEventTouchPositionPtr(pevent,0) );

					// Get and Store Transformed Center !
					NMulVector3ByMatrixO( &pcc->TransformedCenter_Position,
						(NMATRIX*)&pui->pRenderable->pTransformHNode->WorldMatrix,
						&((NVEC3*)NGetFirstArrayPtr(&pcc->pCircle_Geometry->Mesh.VertexArray))[pcc->Circle_CenterVertex]);

					// And check where user touch is  ...
					distx = mouse.x - pcc->TransformedCenter_Position.x;
					disty = mouse.y - pcc->TransformedCenter_Position.y;
					dist  = NFastSqrt( distx*distx + disty*disty );
					// MANUAL HSV AND NCOLOR UPDATE !
					//Hue
					distx = NMIN(NMAX(-1.0f,distx/dist),1.0f);

					angle = acos(distx);
					if(disty<0.0f)
					{
						angle = NF32_2PI-angle;
					}

					pcc->HSV.Hue = angle/NF32_2PI;

					// Saturation
					pcc->HSV.Saturation = NMIN(1.0f,dist/pcc->ColorRadius);

					// Manual updating of NCOLOR member
					NHSV_To_NCOLOR(&pcc->Color,&pcc->HSV,-1);
					_colorcircle_inprogress_command(pui);

					// Update current STATE
					pui->CurrentState = NUI_STATE_COLORCIRCLE_HUE_SATURATION_UPDATE;
					break;
				
				case NUI_STATE_COLORCIRCLE_BRIGHTNESS_WAIT:
					pcc = (NUI_COLORCIRCLE*)pui;
					// Normalize pointer coordinates
// 					mouse.x = NCoord_PixToHRel_X(pevent->Touch.Position.x);
// 					mouse.y = NCoord_PixToHRel_Y(-pevent->Touch.Position.y);
					NCoord_PixToHRel( &mouse, NGetEventTouchPositionPtr(pevent,0) );

					// Get and Store Transformed Center !
					NMulVector3ByMatrixO( &pcc->TransformedCenter_Position,
						(NMATRIX*)&pui->pRenderable->pTransformHNode->WorldMatrix,
						&((NVEC3*)NGetFirstArrayPtr(&pcc->pCircle_Geometry->Mesh.VertexArray))[pcc->Circle_CenterVertex]);

					// And check where user touch is  ...
					distx = mouse.x - pcc->TransformedCenter_Position.x;
					disty = mouse.y - pcc->TransformedCenter_Position.y;
					dist  = NFastSqrt( distx*distx + disty*disty );
					
					disty = NMIN(NMAX(-1.0f,disty/dist),1.0f);
					sinmin = NFastSin(NDEGtoANGLE(CONSTANT_NUI_COLORCIRCLE_BRIGHTNESSMIN_ANGLEDEG));
					sinmax = NFastSin(NDEGtoANGLE(CONSTANT_NUI_COLORCIRCLE_BRIGHTNESSMAX_ANGLEDEG));
					if(disty < sinmin)
					{
						disty = sinmin;
					}
					else if(disty > sinmax)
					{
						disty = sinmax;
					}

					// Final calculation
					angle = 180.0f-NRADtoDEG(asin(disty));
					if(angle<0.0f)
						angle+=360.0f;
					NUISetColorCircleColorHSV_V(pcc, NMIN( NMAX( -(angle - CONSTANT_NUI_COLORCIRCLE_BRIGHTNESSMIN_ANGLEDEG)/CONSTANT_NUI_COLORCIRCLE_BRIGHTNESSRANGE_ANGLEDEG,0.0f),1.0f ) );
					_colorcircle_inprogress_command(pui);

					// Update current STATE
					pui->CurrentState = NUI_STATE_COLORCIRCLE_BRIGHTNESS_UPDATE;
					break;
				
				case NUI_STATE_COLORCIRCLE_ALPHA_WAIT:
					// Notes:
					// Is not necessary to perform the Test "if( ISFLAG_ON(pui->Flags,FLAG_NUIDC_COLORCIRCLE_RGBA) )" 
					// Because the only way to be here, is to previously pass this test when the TOUCH_START event occurred.
					// So if we are here, FLAG_NUIDC_COLORCIRCLE_RGBA is set to ON ! 
					pcc = (NUI_COLORCIRCLE*)pui;
					// Normalize pointer coordinates
// 					mouse.x = NCoord_PixToHRel_X(pevent->Touch.Position.x);
// 					mouse.y = NCoord_PixToHRel_Y(-pevent->Touch.Position.y);
					NCoord_PixToHRel( &mouse, NGetEventTouchPositionPtr(pevent,0) );

					// Get and Store Transformed Center !
					NMulVector3ByMatrixO( &pcc->TransformedCenter_Position,
						(NMATRIX*)&pui->pRenderable->pTransformHNode->WorldMatrix,
						&((NVEC3*)NGetFirstArrayPtr(&pcc->pCircle_Geometry->Mesh.VertexArray))[pcc->Circle_CenterVertex]);

					// And check where user touch is  ...
					distx = mouse.x - pcc->TransformedCenter_Position.x;
					disty = mouse.y - pcc->TransformedCenter_Position.y;
					dist  = NFastSqrt( distx*distx + disty*disty );

					disty = NMIN(NMAX(-1.0f,disty/dist),1.0f);
					sinmin = NFastSin(NDEGtoANGLE(CONSTANT_NUI_COLORCIRCLE_BRIGHTNESSMIN_ANGLEDEG));
					sinmax = NFastSin(NDEGtoANGLE(CONSTANT_NUI_COLORCIRCLE_BRIGHTNESSMAX_ANGLEDEG));
					if(disty < sinmin)
					{
						disty = sinmin;
					}
					else if(disty > sinmax)
					{
						disty = sinmax;
					}

					// Final calculation
					angle = NRADtoDEG(asin(disty))- CONSTANT_NUI_COLORCIRCLE_ALPHAMIN_ANGLEDEG;
					if(angle<0.0f)
						angle+=360.0f;

					NUISetColorCircleColorAlpha(pcc, NMIN( NMAX(angle/CONSTANT_NUI_COLORCIRCLE_ALPHARANGE_ANGLEDEG,0.0f),1.0f ) );
					_colorcircle_inprogress_command(pui);

					// Update current STATE
					pui->CurrentState = NUI_STATE_COLORCIRCLE_ALPHA_UPDATE;

					break;

				default:
					break;
			}
			break;

		case NTOUCH_END:
			switch( pui->CurrentState )
			{
				case NUI_STATE_COLORCIRCLE_HUE_SATURATION_WAIT:
					//pcc = (NUI_COLORCIRCLE*)pui;
					// Here Send message if necessary
					_colorcircle_end_command(pui);
					pui->CurrentState = NUI_STATE_COLORCIRCLE_END;
					break;

				case NUI_STATE_COLORCIRCLE_BRIGHTNESS_WAIT:
					//pcc = (NUI_COLORCIRCLE*)pui;
					// Here Send message if necessary
					_colorcircle_end_command(pui);
					pui->CurrentState = NUI_STATE_COLORCIRCLE_END;
					break;

				case NUI_STATE_COLORCIRCLE_ALPHA_WAIT:
					//pcc = (NUI_COLORCIRCLE*)pui;
					// Here Send message if necessary
					_colorcircle_end_command(pui);
					pui->CurrentState = NUI_STATE_COLORCIRCLE_END;
					break;

				default:
					break;
			}
			break;

		case NTOUCH_CANCEL:
			pui->CurrentState = NUI_STATE_COLORCIRCLE_REST; // or NUI_STATE_COLORCIRCLE_END ???
			break;

		default:
			break;
	}
}

static void NUIColorCircle_States_Handle(NUI *pui, const NTIME	*ptime)
{
	switch( pui->CurrentState )
	{
// 		case NUI_STATE_COLORCIRCLE_REST:
// 			break;

		case NUI_STATE_COLORCIRCLE_HUE_SATURATION_WAIT:
			break;
		case NUI_STATE_COLORCIRCLE_BRIGHTNESS_WAIT:
			break;
		case NUI_STATE_COLORCIRCLE_ALPHA_WAIT:
			break;

		case NUI_STATE_COLORCIRCLE_HUE_SATURATION_UPDATE:
			_place_hue_saturation_selector((NUI_COLORCIRCLE*)pui);
			_update_alpha_arc_vertexcolor((NUI_COLORCIRCLE*)pui);
			_printcurrentcolorvalues((NUI_COLORCIRCLE *)pui);
			pui->CurrentState = NUI_STATE_COLORCIRCLE_HUE_SATURATION_WAIT;
			break;

		case NUI_STATE_COLORCIRCLE_BRIGHTNESS_UPDATE:
			_place_brightness_selector((NUI_COLORCIRCLE*)pui);
			_update_colorcircle_vertexcolor((NUI_COLORCIRCLE*)pui);
			_update_alpha_arc_vertexcolor((NUI_COLORCIRCLE*)pui);
			_printcurrentcolorvalues((NUI_COLORCIRCLE *)pui);
			pui->CurrentState = NUI_STATE_COLORCIRCLE_BRIGHTNESS_WAIT;
			break;

		case NUI_STATE_COLORCIRCLE_ALPHA_UPDATE:
			_place_alpha_selector((NUI_COLORCIRCLE*)pui);
			_printcurrentcolorvalues((NUI_COLORCIRCLE *)pui);
			pui->CurrentState = NUI_STATE_COLORCIRCLE_ALPHA_WAIT;
			break;

		case NUI_STATE_COLORCIRCLE_END:
			pui->CurrentState = NUI_STATE_COLORCIRCLE_REST;
			break;
	
		default:
			break;
	}
}

NUI_COLORCIRCLE*	NCreateUIColorCircle(NUI *parent, NUI_COLORCIRCLE_DESC *pcdesc, const Nu32 user32)
{
	return NSetupUIColorCircle(parent,NEW(NUI_COLORCIRCLE),pcdesc,user32);
}

NUI_COLORCIRCLE *NSetupUIColorCircle(NUI *parent, NUI_COLORCIRCLE *pc, NUI_COLORCIRCLE_DESC *pcdesc, const Nu32 user32)
{
/*
	const NCOLOR	colorsextant[7]={
										{ 1.0f, 0.0f, 0.0f, 1.0f },
										{ 1.0f, 1.0f, 0.0f, 1.0f },
										{ 0.0f, 1.0f, 0.0f, 1.0f },
										{ 0.0f, 1.0f, 1.0f, 1.0f },
										{ 0.0f, 0.0f, 1.0f, 1.0f },
										{ 1.0f, 0.0f, 1.0f, 1.0f },
										{ 1.0f, 0.0f, 0.0f, 1.0f }	// == colorsextant[0] but like this for a simple code
									};
*/
				
	NErrorIf( ISFLAG_ON( pcdesc->UIDesc.Flags_Style,FLAG_NUIDS_COLORCIRCLE_COLOR_INFOS_PRINTING )&& ISFLAG_OFF( pcdesc->UIDesc.Flags_Style,FLAG_NUIDS_TEXT_DISPLAY_DEVICE ),NERROR_UI_FRAME_TEXT_DISPLAY_DEVICE_MISSING );

	NPushUISetUpProcess( (NUI*)pc, (NUI_DESC*)pcdesc );
	
	
	// + -------------------------------------------------------------------------------------------------------------------------
	// + FIRST STEPS ...
	memset(pc,0,sizeof(NUI_COLORCIRCLE));

	// + -------------------------------------------------------------------------------------------------------------------------
	// + CALL PARENT STRUCTURE SETUP
	//		Here, parent structure is NUI.
	
#ifdef _DEBUG
	if( ! NSetupUI(parent,(NUI*)pc,(NUI_DESC*)pcdesc,user32) )
	{
		// NPopUISetUpProcess triggered EVENT sending from NSetupUI but it's a bug !
		// And unfortunately, it seem's user delete the UI during event  processing (NUI_SETUP or/and NUI_ENABLE) ...
		// This should never happen ! 
		// (Maybe it's not exactly that but certainly something like that to have a NULL pointer returned by NSetupUI)
		NErrorIf(1,NERROR_UI_SETUPPROCESS_STACK_CORRUPTED); 
	}
#else
	NSetupUI(parent,(NUI*)pc,(NUI_DESC*)pcdesc,user32);
#endif	
	// + -------------------------------------------------------------------------------------------------------------------------
	// + RE-SETUP System data & functions 
	//		Overwrite some parameters
	pc->UI.CurrentState			= NUI_STATE_COLORCIRCLE_REST; 
	pc->UI.UIState_Proc			= NUIColorCircle_States_Handle;
	pc->UI.UIEventToState_Proc	= NUIColorCircle_EventToState_Handle;
	pc->UI.ClearUI_Proc			= NClearUIColorCircle;

	// Specific Default Touch Mode ( different than Simple UI )
	if( ISFLAG_OFF(pcdesc->UIDesc.Flags_Style, FLAG_NUIDS_USEPARAM_LISTEN_TOUCH_MODE) )
		NSetUIListenTouchMode((NUI*)pc, NUI_LISTEN_TOUCH_MODE_BASIC);

	NErrorIf(pc->UI.TouchCountMax != 1,NERROR_UI_UNAUTHORIZED_TOUCHCOUNTMAX ); // Not an Error, but to Warn User ColorCircle Manage Only 1 Touch !
	pc->UI.TouchCountMax		= 1;

	// A complete Color Circle include a color disc and 2 half open rings around the disc.
	// Each element radius follows simple proportional rules define like this:
	//	<---------><--><--------------------------------------->	PRESET_... defines
	//	
	//	               <---------------------------------------+	r0
	//			   <-------------------------------------------+	r1
	//	<------------------------------------------------------+	r2

	pc->ColorRadius		= DEFAULT_NUI_COLORCIRCLE_COLOR_RSIZE*0.5f*pcdesc->ColorCircleDesc.Placement.Size.x;
	pc->SpacingRadius	= (DEFAULT_NUI_COLORCIRCLE_COLOR_RSIZE+DEFAULT_NUI_COLORCIRCLE_SPACING_RSIZE)*0.5f*pcdesc->ColorCircleDesc.Placement.Size.x;					
	pc->ArcsRadius		= 0.5f*pcdesc->ColorCircleDesc.Placement.Size.x;						
	pc->Circle_AdditionalSextantSubdivision = DEFAULT_NUI_COLORCIRCLE_ADDITIONNAL_SEXTANT_SUBDIVISION;

	if( ISFLAG_ON(pcdesc->UIDesc.Flags_Style, FLAG_NUIDS_USEPARAM_COLORCIRCLE_INITIALCOLOR ) )
	{
		pc->Color = pcdesc->InitialColor;
	}
	else
	{
		NSetColorf(&pc->Color,	DEFAULT_NUI_COLORCIRCLE_COLOR_RED,
								DEFAULT_NUI_COLORCIRCLE_COLOR_GREEN,
								DEFAULT_NUI_COLORCIRCLE_COLOR_BLUE,
								DEFAULT_NUI_COLORCIRCLE_COLOR_ALPHA);
	}
	NCOLOR_To_NHSV( &pc->HSV, &pc->Color );


	// + -------------------------------------------------------------------------------------------------------------------------
	// + SETUP COLOR CIRCLE VERTEX INDEX
	//  
	pc->Circle_CenterVertex			= 0;				
	pc->Circle_FirstVertex			= 1;				
	pc->Circle_VertexSize			= (Nu16)NUT_GetDiscVertexNumber(DEFAULT_NUI_COLORCIRCLE_SLICENUMBER, NFALSE )-1; // -1 because of the disc center vertex				
	
	pc->ArcAlpha_FirstVertex		= (Nu16)NUT_GetDiscVertexNumber(DEFAULT_NUI_COLORCIRCLE_SLICENUMBER, NFALSE );
	pc->ArcAlpha_VertexSize			= (Nu16)NUT_GetRingVertexNumber(DEFAULT_NUI_COLORCIRCLE_SLICENUMBER/2, NTRUE );
	
	pc->SelectorColor_FirstVertex	= (Nu16)(NUT_GetDiscVertexNumber(DEFAULT_NUI_COLORCIRCLE_SLICENUMBER, NFALSE )+NUT_GetRingVertexNumber(DEFAULT_NUI_COLORCIRCLE_SLICENUMBER/2, NTRUE )*2); // Here we dont have a ref index on ArCBrightness even if it exists !
	pc->SelectorColor_VertexSize	= (Nu16)NUT_GetRingVertexNumber( DEFAULT_NUI_COLORCIRCLE_SELECTOR_SLICENB, NFALSE );

	pc->SelectorBright_FirstVertex	= pc->SelectorColor_FirstVertex + pc->SelectorColor_VertexSize;
	pc->SelectorBright_VertexSize	= (Nu16)NUT_GetRingVertexNumber( DEFAULT_NUI_COLORCIRCLE_SELECTOR_SLICENB, NFALSE );

	// These 2 last ones are going to be used ONLY if Alpha is updatable. Otherwise they are going to still unused !
	pc->SelectorAlpha_FirstVertex	= pc->SelectorBright_FirstVertex + pc->SelectorBright_VertexSize;
	pc->SelectorAlpha_VertexSize	= (Nu16)NUT_GetRingVertexNumber( DEFAULT_NUI_COLORCIRCLE_SELECTOR_SLICENB, NFALSE );

	// + -------------------------------------------------------------------------------------------------------------------------
	// + CREATE THE COLOR CIRCLE GEOMETRY !
	//	
	pc->pCircle_Geometry	= NCreateUIGeometry((NUI*)pc,&pcdesc->ColorCircleDesc );

	// + -------------------------------------------------------------------------------------------------------------------------
	// INTIAL COLORCIRCLE SET UP
	// Flags Style
	if(ISFLAG_ON(pcdesc->UIDesc.Flags_Style,FLAG_NUIDS_COLORCIRCLE_COLOR_INFOS_PRINTING))
	{
		NErrorIf(ISFLAG_OFF(pcdesc->UIDesc.Flags_Style,FLAG_NUIDS_TEXT_DISPLAY_DEVICE ), NERROR_FLAG_INCONSISTENCY );
		FLAG_ON(pc->UI.Flags,FLAG_NUI_COLORCIRCLE_COLOR_INFOS_PRINTING);
	}
	else
	{
		FLAG_OFF(pc->UI.Flags,FLAG_NUI_COLORCIRCLE_COLOR_INFOS_PRINTING);
	}

	// Flags Core
	if(ISFLAG_ON(pcdesc->UIDesc.Flags_Core,FLAG_NUIDC_COLORCIRCLE_INPROGRESS_COMMAND))
		FLAG_ON(pc->UI.Flags,FLAG_NUI_COLORCIRCLE_INPROGRESS_COMMAND);
	else
		FLAG_OFF(pc->UI.Flags,FLAG_NUI_COLORCIRCLE_INPROGRESS_COMMAND);

	if(ISFLAG_ON(pcdesc->UIDesc.Flags_Core,FLAG_NUIDC_COLORCIRCLE_RGBA))
		FLAG_ON(pc->UI.Flags,FLAG_NUI_COLORCIRCLE_RGBA);
	else
		FLAG_OFF(pc->UI.Flags,FLAG_NUI_COLORCIRCLE_RGBA);

	if(ISFLAG_ON(pcdesc->UIDesc.Flags_Core,FLAG_NUIDC_COLORCIRCLE_NU8_COMMAND_FORMAT))
		FLAG_ON(pc->UI.Flags,FLAG_NUI_COLORCIRCLE_NU8_COMMAND_FORMAT);
	else
		FLAG_OFF(pc->UI.Flags,FLAG_NUI_COLORCIRCLE_NU8_COMMAND_FORMAT);

	// + -------------------------------------------------------------------------------------------------------------------------
	// + SETUP ALL VERTEX COLOR, PLACE THE 3 SELECTORS and 'PRINT' initial color
	//			Selectors have to be place at the right places and Alpha Arc has to use the current Color !
	_update_alpha_arc_vertexcolor( pc );
	_update_colorcircle_vertexcolor( pc );
	_update_alpha_arc_vertexcolor(pc);
	
	_place_hue_saturation_selector(pc);
	_place_brightness_selector(pc);
	_place_alpha_selector(pc);
	
	_printcurrentcolorvalues(pc);

	
	return (NUI_COLORCIRCLE*)NPopUISetUpProcess( (NUI*)pc, (NUI_DESC*)pcdesc );
}


void	NClearUIColorCircle(NUI *puicc)
{
	// b)Clear UI Section
	NClearUI_Basics(puicc);
}



void NUIColorCircle_SetColor(NUI_COLORCIRCLE *pc, const NCOLOR *pcolor, const Nbool send_command_message, const Nbool _if_color_updated_only)
{

	NEVENT	nevent;
	Nbool	bchange = NFALSE;

	// ++++++++++++++++++++++++++++++++++++
	// + Is Incoming Color a New One ?
	if(!pcolor) // means RGBA:0,0,0,0 ...
	{
		if( pc->Color.red	!= 0.0f ||
			pc->Color.green != 0.0f ||
			pc->Color.blue	!= 0.0f ||
			pc->Color.alpha != 0.0f )
		{
			pc->Color = *pcolor;
			NCOLOR_To_NHSV(&pc->HSV,&pc->Color);
			bchange = NTRUE;
		}
	}
	else
	{
		if( pc->Color.red	!= pcolor->red	 ||
			pc->Color.green != pcolor->green ||
			pc->Color.blue	!= pcolor->blue  ||
			pc->Color.alpha != pcolor->alpha )
		{
			pc->Color = *pcolor;
			NCOLOR_To_NHSV(&pc->HSV,&pc->Color);
			bchange = NTRUE;
		}
	}

	if(bchange)
	{
		_place_hue_saturation_selector(pc);
		_update_alpha_arc_vertexcolor(pc);
		_place_brightness_selector(pc);
		_update_colorcircle_vertexcolor(pc);
		_update_alpha_arc_vertexcolor(pc);
		_place_alpha_selector(pc);
		_printcurrentcolorvalues(pc);
	}

	if(!_if_color_updated_only)
		bchange = NTRUE;

	if( bchange && send_command_message )
	{
		memset(&nevent,0,sizeof(NEVENT));
		if( ISFLAG_ON(pc->UI.Flags,FLAG_NUI_COLORCIRCLE_NU8_COMMAND_FORMAT) )
		{
			NFillupEventHead(nevent,NCOMMAND_COLORCIRCLE_RGBA8,0,NEVENT_RECIPIENT_CAST_UI,pc);
			NCOLOR_To_RGBA8( &nevent.ColorCircle_RGBA8.Color,pcolor );
		}
		else
		{
			NFillupEventHead(nevent,NCOMMAND_COLORCIRCLE_RGBA32,0,NEVENT_RECIPIENT_CAST_UI,pc);
			nevent.ColorCircle_RGBA32.Color = *pcolor;
		}
		NSendUICommandEvent(&nevent);
	}
}


NGEOMETRY *NUICreateColorCircleCustomGeometry(const Nf32 main_diameter,const Nbool updatable_alpha)
{
	NGEOMETRY_DESC		geomdesc;
	NGEOMETRY			*pgeom_circle;
	NUT_DISC_DESC		disc_desc;
	NUT_RING_DESC		ring_desc;
	Nf32				radius;
	Nu32				selectornb;
	NCOLOR				colorrange[NRANGE_4COLORS];
	// + -------------------------------------------------------------------------------------------------------------------------
	// + COLOR CIRCLE AND TWO ARCS GEOMETRY
	//
	NErrorIf( main_diameter < CONSTANT_NUI_COLORCIRCLE_SIZEMIN, NERROR_INVALID_PARAMETER );

	if(updatable_alpha)
		selectornb = 3;
	else
		selectornb = 2;

	NFillupGeometryDesc(&geomdesc,	NGEOMETRY_FORMAT_COLORFLATTRIANGLE_TU0_V3C4_TRIANGLE, 0,
									NUT_GetDiscTriangleNumber( DEFAULT_NUI_COLORCIRCLE_SLICENUMBER ) + 2*NUT_GetRingTriangleNumber( DEFAULT_NUI_COLORCIRCLE_SLICENUMBER/2 )+NUT_GetRingTriangleNumber( DEFAULT_NUI_COLORCIRCLE_SELECTOR_SLICENB )*selectornb,
									NUT_GetDiscVertexNumber( DEFAULT_NUI_COLORCIRCLE_SLICENUMBER,NTRUE ) + 2*NUT_GetRingVertexNumber( DEFAULT_NUI_COLORCIRCLE_SLICENUMBER/2,NTRUE )+NUT_GetRingVertexNumber( DEFAULT_NUI_COLORCIRCLE_SELECTOR_SLICENB,NFALSE )*selectornb );
/*
	NSetUpGeometryDesc_Triangle(	&geomdesc,
									NGetMostSuitableStateSet(NULL,0),
									NUT_GetDiscTriangleNumber( DEFAULT_NUI_COLORCIRCLE_SLICENUMBER ) + 2*NUT_GetRingTriangleNumber( DEFAULT_NUI_COLORCIRCLE_SLICENUMBER/2 )+NUT_GetRingTriangleNumber( DEFAULT_NUI_COLORCIRCLE_SELECTOR_SLICENB )*selectornb,
									NUT_GetDiscVertexNumber( DEFAULT_NUI_COLORCIRCLE_SLICENUMBER,NTRUE ) + 2*NUT_GetRingVertexNumber( DEFAULT_NUI_COLORCIRCLE_SLICENUMBER/2,NTRUE )+NUT_GetRingVertexNumber( DEFAULT_NUI_COLORCIRCLE_SELECTOR_SLICENB,NFALSE )*selectornb,
									NULL );
*/
	pgeom_circle = NCreateGeometry(&geomdesc);
	


	// + -------------------------------------------------------------------------------------------------------------------------
	// + INSERT COLOR CIRCLE INTO GEOMETRY
	//  
	Nmem0(&disc_desc,NUT_DISC_DESC);
	FLAG_ON(disc_desc.CoreDesc.Flags,FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE);
	FLAG_ON(disc_desc.CoreDesc.Flags,FLAG_NUTSCD_USEPARAM_SIZE);
	FLAG_ON(disc_desc.CoreDesc.Flags,FLAG_NUTSCD_USEPARAM_CONSTRUCTION_ORIGIN);

	disc_desc.CoreDesc.Construction_Plane	= _PLANE_XY; 
	disc_desc.CoreDesc.Size.x	= main_diameter*DEFAULT_NUI_COLORCIRCLE_COLOR_RSIZE;
	disc_desc.CoreDesc.Size.y	= main_diameter*DEFAULT_NUI_COLORCIRCLE_COLOR_RSIZE;
	
	disc_desc.SliceNb	= DEFAULT_NUI_COLORCIRCLE_SLICENUMBER;
	// Define figure building position: "Origin"
	// !!! Its not the disc center because figure is supposed to be built from their top left corner.
	// !!! ... and Construction Origin is supposes to be the place of this top left corner.
	// !!! Here, the entire figure is the color circle with it's 2 ARCs ...
	// !!! So the disc as to be push to the right and the bottom to later insert the 2 Arcs and have the entire figure
	// !!! at the right place regarding its top-left corner. 
	// !!! We ARE using A variable called "radius" but here it's more a portion of a radius to be crystal clear...
	radius = (1.0f - DEFAULT_NUI_COLORCIRCLE_COLOR_RSIZE)*main_diameter*0.5f;
	NVec3Set(&disc_desc.CoreDesc.ConstructionOrigin,radius,-radius,0);

	// Define figure Anchor point
	// 	NVec3Set(&disc_desc.Anchor,ANCHOR_MIDWIDTH,0,0);
	// 	FLAG_ON(disc_desc.flags,FLAG_NUTSCD_USEPARAM_ANCHOR);
	//printf("\n ring::Circle_CenterVertex = %d",pgeom_circle->Mesh.VertexArray.Size);
	NUT_InsertDisc(pgeom_circle,&disc_desc);

	// + -------------------------------------------------------------------------------------------------------------------------
	// + INSERT ARC ALPHA INTO GEOMETRY
	//  
	Nmem0(&ring_desc,NUT_RING_DESC );
	FLAG_ON(ring_desc.CoreDesc.Flags,FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE);
	FLAG_ON(ring_desc.CoreDesc.Flags,FLAG_NUTSCD_USEPARAM_SIZE);
	FLAG_ON(ring_desc.CoreDesc.Flags,FLAG_RING_DESC_USEPARAM_SMALLSIZERATIO);
	FLAG_ON(ring_desc.CoreDesc.Flags,FLAG_RING_DESC_USEPARAM_OPENRING_ANGLES);
	
	FLAG_ON(ring_desc.CoreDesc.ColorBuild.Flags,FLAG_NBUILDBASE_COLOR_USEPARAMS_COLORRANGE);
	
	//ring_desc.CoreDesc.ConstructionOrigin = 0,0,0 We don't want to change that.
	ring_desc.CoreDesc.Construction_Plane			= _PLANE_XY; // or NUT_PLANE_XZ, or NUT_PLANE_YZ
	ring_desc.CoreDesc.Size.x						= main_diameter;//1.0f;
	ring_desc.CoreDesc.Size.y						= main_diameter;//1.0f;
	ring_desc.SmallSizeRatio.x						= (DEFAULT_NUI_COLORCIRCLE_COLOR_RSIZE+DEFAULT_NUI_COLORCIRCLE_SPACING_RSIZE);
	ring_desc.SmallSizeRatio.y						= (DEFAULT_NUI_COLORCIRCLE_COLOR_RSIZE+DEFAULT_NUI_COLORCIRCLE_SPACING_RSIZE);
	ring_desc.SliceNb								= DEFAULT_NUI_COLORCIRCLE_SLICENUMBER/2;
	ring_desc.OpenRing_FromAngle					= CONSTANT_NUI_COLORCIRCLE_ALPHAMIN_ANGLEDEG;//280.0f;
	ring_desc.OpenRing_ToAngle						= CONSTANT_NUI_COLORCIRCLE_ALPHAMAX_ANGLEDEG;//80.0f;

	ring_desc.CoreDesc.ColorBuild.pColorRange		= colorrange;
	ring_desc.CoreDesc.ColorBuild.ColorRangeSize	= NRANGE_4COLORS;

	// Define Colors
	if(updatable_alpha)
	{
		NSetColorf( &colorrange[0], CONSTANT_NUI_COLORCIRCLE_ALPHAARC_BACK_COLOR_RED,CONSTANT_NUI_COLORCIRCLE_ALPHAARC_BACK_COLOR_GREEN,CONSTANT_NUI_COLORCIRCLE_ALPHAARC_BACK_COLOR_BLUE,CONSTANT_NUI_COLORCIRCLE_ALPHAARC_BACK_COLOR_ALPHA );
		NSetColorf( &colorrange[1], CONSTANT_NUI_COLORCIRCLE_ALPHAARC_BACK_COLOR_RED,CONSTANT_NUI_COLORCIRCLE_ALPHAARC_BACK_COLOR_GREEN,CONSTANT_NUI_COLORCIRCLE_ALPHAARC_BACK_COLOR_BLUE,CONSTANT_NUI_COLORCIRCLE_ALPHAARC_BACK_COLOR_ALPHA );
	}
	else
	{
		NSetColorf( &colorrange[0], 1,1,1,1 );
		NSetColorf( &colorrange[1], 1,1,1,1 );
	}
	NSetColorf( &colorrange[2], 1,1,1,1 );
	NSetColorf( &colorrange[3], 1,1,1,1 );

	//printf("\n ring::ArcAlpha_FirstVertex = %d",pgeom_circle->Mesh.VertexArray.Size);
	NUT_InsertRing(pgeom_circle,&ring_desc);

	// + -------------------------------------------------------------------------------------------------------------------------
	// + INSERT ARC BRIGHTNESS INTO GEOMETRY
	//  
	Nmem0(&ring_desc,NUT_RING_DESC );
	FLAG_ON(ring_desc.CoreDesc.Flags,FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE);
	FLAG_ON(ring_desc.CoreDesc.Flags,FLAG_NUTSCD_USEPARAM_SIZE);
	FLAG_ON(ring_desc.CoreDesc.Flags,FLAG_RING_DESC_USEPARAM_SMALLSIZERATIO);
	FLAG_ON(ring_desc.CoreDesc.Flags,FLAG_RING_DESC_USEPARAM_OPENRING_ANGLES);
	FLAG_ON(ring_desc.CoreDesc.Flags,FLAG_RING_DESC_VERTEX_TRIGO_INVERSE_BUILDING);

	FLAG_ON(ring_desc.CoreDesc.ColorBuild.Flags,FLAG_NBUILDBASE_COLOR_USEPARAMS_COLORRANGE);
	//ring_desc.CoreDesc.ConstructionOrigin = 0,0,0 We don't want to change that.
	ring_desc.CoreDesc.Construction_Plane			= _PLANE_XY; // or NUT_PLANE_XZ, or NUT_PLANE_YZ
	ring_desc.CoreDesc.Size.x						= main_diameter;
	ring_desc.CoreDesc.Size.y						= main_diameter;
	ring_desc.SmallSizeRatio.x						= (DEFAULT_NUI_COLORCIRCLE_COLOR_RSIZE+DEFAULT_NUI_COLORCIRCLE_SPACING_RSIZE);
	ring_desc.SmallSizeRatio.y						= (DEFAULT_NUI_COLORCIRCLE_COLOR_RSIZE+DEFAULT_NUI_COLORCIRCLE_SPACING_RSIZE);
	ring_desc.SliceNb								= DEFAULT_NUI_COLORCIRCLE_SLICENUMBER/2;
	ring_desc.OpenRing_FromAngle					= CONSTANT_NUI_COLORCIRCLE_BRIGHTNESSMIN_ANGLEDEG;//260.0f;
	ring_desc.OpenRing_ToAngle						= CONSTANT_NUI_COLORCIRCLE_BRIGHTNESSMAX_ANGLEDEG;//100.0f;
	ring_desc.CoreDesc.ColorBuild.pColorRange		= colorrange;
	ring_desc.CoreDesc.ColorBuild.ColorRangeSize	= 2;

	// Define Colors
	NSetColorf( &colorrange[0], 0,0,0,1 );
	NSetColorf( &colorrange[1], 1,1,1,1 );

	NUT_InsertRing(pgeom_circle,&ring_desc);

	// + -------------------------------------------------------------------------------------------------------------------------
	// + SETUP THE 3 SELECTORS
	//		( Color, Brightness and Alpha ).
	//
	Nmem0(&ring_desc,NUT_RING_DESC );
	FLAG_ON(ring_desc.CoreDesc.Flags,FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE);
	FLAG_ON(ring_desc.CoreDesc.Flags,FLAG_NUTSCD_USEPARAM_SIZE);
	FLAG_ON(ring_desc.CoreDesc.Flags,FLAG_RING_DESC_USEPARAM_SMALLSIZERATIO);
	FLAG_ON(ring_desc.CoreDesc.Flags,FLAG_NUTSCD_USEPARAM_ANCHOR);
	FLAG_ON(ring_desc.CoreDesc.Flags,FLAG_NUTSCD_USEPARAM_CONSTRUCTION_ORIGIN);

	FLAG_ON(ring_desc.CoreDesc.ColorBuild.Flags,FLAG_NBUILDBASE_COLOR_USEPARAMS_COLORRANGE);
	ring_desc.CoreDesc.Construction_Plane		= _PLANE_XY; // or NUT_PLANE_XZ, or NUT_PLANE_YZ
	ring_desc.CoreDesc.Size.x					= DEFAULT_NUI_COLORCIRCLE_SELECTOR_SIZERATIO*main_diameter;
	ring_desc.CoreDesc.Size.y					= DEFAULT_NUI_COLORCIRCLE_SELECTOR_SIZERATIO*main_diameter;
	ring_desc.SliceNb							= DEFAULT_NUI_COLORCIRCLE_SELECTOR_SLICENB;
	ring_desc.SmallSizeRatio.x					= DEFAULT_NUI_COLORCIRCLE_SELECTOR_SMALLSIZERATIO;
	ring_desc.SmallSizeRatio.y					= DEFAULT_NUI_COLORCIRCLE_SELECTOR_SMALLSIZERATIO;
	ring_desc.CoreDesc.ColorBuild.pColorRange	= colorrange;
	ring_desc.CoreDesc.ColorBuild.ColorRangeSize= 1;

	
	// All the Selectors are going to be created  anchored by their center at the center of the entire figure.
	// Center of the entire figure is (radius,-radius,0) from the top left corner of reference.
	// So ...
	NVec3Set(&ring_desc.CoreDesc.Anchor, ANCHOR_MIDWIDTH, ANCHOR_MIDHEIGHT,ANCHOR_BACK );
	radius = main_diameter*0.5f;
	NVec3Set(&ring_desc.CoreDesc.ConstructionOrigin,radius,-radius,0);

	NSetColorf( &colorrange[0], 1,1,1,1 );
	NSetColorf( &colorrange[1], 0.5f,0.5f,0.5f,1 );
	
//	printf("\n ring::selector?_FirstVertex = %d",pgeom_circle->Mesh.VertexArray.Size);
	NUT_InsertRing(pgeom_circle,&ring_desc);	
//	printf("\n ring::selector?_FirstVertex = %d",pgeom_circle->Mesh.VertexArray.Size);
	NUT_InsertRing(pgeom_circle,&ring_desc);	
	if( updatable_alpha )
	{
//		printf("\n ring::selector?_FirstVertex = %d",pgeom_circle->Mesh.VertexArray.Size);
		NUT_InsertRing(pgeom_circle,&ring_desc);	
	}

	return pgeom_circle;
}
