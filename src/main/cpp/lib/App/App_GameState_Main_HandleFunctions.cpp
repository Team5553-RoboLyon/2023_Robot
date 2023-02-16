#include "lib/N/NCStandard.h"
#include "lib/N/GameLoop/NGameStates.h"
#include "lib/N/GameLoop/NGameStatesUID.h"
#include "lib/N/Event/NEventTouch.h"
#include "lib/N/NStructure.h"
#include "lib/N/NCoordinateSystemConversions.h"
#include "lib/N/NEditor.h"
#include "lib/N/NCore.h"
#include "lib/N/NMath.h"
#include "lib/NL/NLProject.h"
#include "lib/NL/MotionControl/Trajectory/NLTrajectory.h"
#include "lib/N/Utilities/NUT_Slider.h"
#include "lib/N/Utilities/Draw/NUT_Draw.h"
#include "lib/N/Utilities/SplineTools/NUT_Editor_SplinesPack_Tools.h"
#include "lib/N/Utilities/NUT_3DMouse.h"
#include "lib/NL/NL2DOrthogonalCoordinateSystem.h"
#include "lib/NL/NL2DOrthogonalCoordinateSystem_MotionProfileFlags.h"
#include "lib/NL/NL2DOrthogonalCoordinateSystemToolData.h"
#include "lib/NL/NLGfx.h"
#include "lib/NL/MotionControl/NLPathWorkbench.h"

#include "lib/NL/MotionControl/NLKinTweak.h"
#include "lib/NL/Simulation/VirtualRobot/NLVirtualRobot.h"
#include "lib/NL/Simulation/NLRobot.h"
#include "lib/App/App_TestAndDebug_Tools.h"
#include "lib/App/App_GameState_Main_HandleFunctions.h"

extern NUT_SPACK_TOOLDATA SplinesPack;
extern NLMOTIONCONTROLER MotionControler;

extern NLPROJECT Project;

/*
extern NLDRIVETRAINSPECS				DriveTrainSpecifications;
extern NARRAY							WorkbenchArray;
extern NLPATH_WORKBENCH					*pCurrentWorkbench;
*/
/*
extern NLPATH							Path;
extern NLTRAJECTORY						Trajectory;
extern NLTRAJECTORY_BASIC_PACK		TrajectoryStateSPack;
extern NLTRAJECTORY_PACK			TrajectoryStatePack;
extern NLPATH_BUILDER_POLYGONAL_CHAIN	PathBuilderPolygonalChain;
extern NLPATH_BUILDER					*pCurrentPathBuilder;
*/

extern NL2DOCS_TOOLDATA OrthogonalCoordinateSystemToolData;
extern NL2DOCS OrthogonalCoordinateSystem_B;
extern NL2DOCS OrthogonalCoordinateSystem_A;
extern NLGFX Gfx;

extern NARRAY WayPointsArray;

extern APP_TEST_TOOLDATA TestToolData;

// Simulation
extern NLVIRTUAL_ROBOT VirtualRobot;
// extern NLROBOT						Robot;

// extern NL2DOCS	PathVelocityLimitsOcs;

NSPLINE Spline;
NUT_SLIDER CameraSlider;

#define STATE_EDITOR_CAMERA_NULL 0
#define STATE_EDITOR_CAMERA_TOUCH 1
#define STATE_EDITOR_CAMERA_EDIT_Z 2

void App_GameState_Main_StartHandle(const NSTATEUID_ENUM previous_uid, const Nu32 UserNu32)
{
}
void App_GameState_Main_EndHandle(const NSTATEUID_ENUM next_uid, const Nu32 UserNu32)
{
}

void App_GameState_Main_TimeHandle(const NTIME *ptime, const Nu32 user_Nu32)
{
	Nchar infos[64];
	NCOLOR color = {NCOLOR_PRESET3F_BLUE_TURQUOISE, 1};
	static Nf32 time;
	Nu32 i;
	NLPATH *ppath;
	NLPATH_POINT *pp;
	NVEC3 pos;
	NMATRIX3x3 mx;

	// build current path and trajectory
	if (Project.m_pCurrentWorkbench)
		Project.m_pCurrentWorkbench->build();

	//	Trajectory.buildTrajectoryPack(&TrajectoryStateSPack); // build un pack d'�tats  'simples'   |_�videment, sur le 'vrai' robot seul un de ces deux packs est utile.
	//	Trajectory.buildTrajectoryPack(&TrajectoryStatePack);  // build un pack d'�tats  'complets'  |

	// DRAW
	NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_3D);
	NUT_SetDrawConstructionPlane(_PLANE_XY);

	NLPATH_WORKBENCH *pwb = (NLPATH_WORKBENCH *)Project.m_workbenchArray.pFirst;
	for (i = 0; i < Project.m_workbenchArray.Size; i++, pwb++)
	{
		if (pwb == Project.m_pCurrentWorkbench)
		{
			pwb->m_pPath->draw(Project.m_driveTrainSpecifications.m_axleTrack);
			pwb->getTrajectory()->drawLimitedUserVStagesArray(0.1f, 0);
			pwb->getTrajectory()->drawLimitedKeysArray(Gfx.useGfx(NLTRAJECTORY_COLORPICKPACK_drawKeys, NLGFX_TXTSZ_3D_S, NLGFX_SZ_3D_M));

			// Draw Robot at Start/End
			if ((ppath = pwb->getPath()) && (ppath->m_geometry.m_pathPointsArray.Size > 1))
			{
				// Start
				pp = (NLPATH_POINT *)ppath->m_geometry.m_pathPointsArray.pFirst;
				pos.x = pp->p.x;
				pos.y = pp->p.y;
				pos.z = 0.0f;
				mx.XAxis.x = pp->u.x;
				mx.YAxis.x = -pp->u.y;
				mx.ZAxis.x = 0.0f;
				mx.XAxis.y = pp->u.y;
				mx.YAxis.y = pp->u.x;
				mx.ZAxis.y = 0.0f;
				mx.XAxis.z = 0.0f;
				mx.YAxis.z = 0.0f;
				mx.ZAxis.z = 1.0f;

				VirtualRobot.drawRobotShape(&ppath->m_matrix, &mx, &pos, &color);

				// End
				pp = (NLPATH_POINT *)NGetLastArrayPtr(&ppath->m_geometry.m_pathPointsArray);
				pos.x = pp->p.x;
				pos.y = pp->p.y;
				pos.z = 0.0f;
				mx.XAxis.x = pp->u.x;
				mx.YAxis.x = -pp->u.y;
				mx.ZAxis.x = 0.0f;
				mx.XAxis.y = pp->u.y;
				mx.YAxis.y = pp->u.x;
				mx.ZAxis.y = 0.0f;
				mx.XAxis.z = 0.0f;
				mx.YAxis.z = 0.0f;
				mx.ZAxis.z = 1.0f;

				VirtualRobot.drawRobotShape(&ppath->m_matrix, &mx, &pos, &color);

				NUT_SetDrawConstructionPlane(_PLANE_XY);
			}

			pwb->m_pPathBuilder->draw();
		}
		else
		{
			pwb->m_pPath->draw(0.0f);
		}
	}

	Project.m_playground.drawFieldTrueEdges(&color);
	/*
		if (PathBuilderPolygonalChain.m_geometry.m_primitivesArray.Size)
		{
			NVEC3		pos;
			NMATRIX3x3	mx;
			NLPATH_POINT* pp =  (NLPATH_POINT*)PathBuilderPolygonalChain.m_geometry.m_pathPointsArray.pFirst;
			pos.x = pp->p.x;
			pos.y = pp->p.y;
			pos.z = 0.0f;
			mx.XAxis.x =  pp->u.y;	mx.YAxis.x = pp->u.x;	mx.ZAxis.x = 0.0f;
			mx.XAxis.y = -pp->u.x;	mx.YAxis.y = pp->u.y;	mx.ZAxis.y = 0.0f;
			mx.XAxis.z =  0.0f;		mx.YAxis.z = 0.0f;		mx.ZAxis.z = 1.0f;
			NUT_SetDrawConstructionPlaneMatrix3x3(&mx);
			VirtualRobot.drawRobotShape(&pos,&mx,&color);
			NUT_SetDrawConstructionPlane(_PLANE_XY);
			PathBuilderPolygonalChain.draw();
		}

		Path.draw(&DriveTrainSpecifications);

		NCOLOR colcircles	= { NCOLOR_PRESET3F_BLUE_PRUSSIAN,1.0f };
		NCOLOR colpair		= { NCOLOR_PRESET3F_RED_MADDER,1.0f };
		NCOLOR colalpha		= { NCOLOR_PRESET3F_ORANGE,1.0f };
		NCOLOR colbeta		= { NCOLOR_PRESET3F_BRONZE,1.0f };

		for (Nu32 i = 0; i < Path.getSectionsNumber(); i++)
			Path.drawSection(i, &colcircles,&colpair,&colalpha,&colbeta);

	*/
	if (Project.m_pCurrentWorkbench)
	{
		NLDRIVETRAINSPECS *pdtspec = Project.m_pCurrentWorkbench->m_pDriveTrainSpecifications;
		NLPATH *ppth = Project.m_pCurrentWorkbench->m_pPath;
		NLTRAJECTORY *ptrj = Project.m_pCurrentWorkbench->m_pTrajectory;

		// Tra�age des Layers ( OCS: Orthogonal Coordinate System )
		NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC);
		NL2DOCS **ppocs = (NL2DOCS **)OrthogonalCoordinateSystemToolData.m_ArrayOfOcsPtr.pFirst;
		for (Nu32 i = 0; i < OrthogonalCoordinateSystemToolData.m_ArrayOfOcsPtr.Size; i++, ppocs++)
		{
			// Preparation de l'OCS
			// L'OCS est-il configur� pour repr�senter les donn�es par rapport au temps ?
			if (ISFLAG_ON((*ppocs)->m_Flags, FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_FT))
			{
				(*ppocs)->setUnitCount((Nu32)ceil(2.0f * Project.m_pCurrentWorkbench->m_pPath->m_geometry.m_ds / pdtspec->m_limits.getVelocityMax()) + 1, (Nu32)ceil(pdtspec->m_limits.getVelocityMax()) + 1);
			}
			else // ... ou par rapport � la distance parcourue ?
			{
				(*ppocs)->setUnitCount((Nu32)ceil(ppth->m_geometry.m_ds) + 1, (Nu32)ceil(pdtspec->m_limits.getVelocityMax()) + 1);
			}

			// Subscale pour la representation de l'Acceleration
			(*ppocs)->setSubScale(NL2DOCS_SUBSCALE_0, 1.0f, pdtspec->m_limits.getVelocityMax() / pdtspec->m_limits.getAccelerationMax());
			// Subscale pour la representation du Jerk
			(*ppocs)->setSubScale(NL2DOCS_SUBSCALE_1, 1.0f, pdtspec->m_limits.getVelocityMax() / pdtspec->m_limits.getJerkMax());

			if (ISFLAG_ON((*ppocs)->m_Flags, FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_INFO_MAIN))
			{
				sprintf(infos, "Length:%.3f m  Time:%.3f s  Average Speed: %.3f m/s", ptrj->getDs(), /*ptrj->getPath()->m_geometry.m_ds*/ ptrj->getDt(), ptrj->getDs() / ptrj->getDt());
				(*ppocs)->drawBaseLine(infos);
			}

			if (!FLAGS_TEST((*ppocs)->m_Flags, MASK_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_JAV, 0))
			{
				// ptrj->drawKinTArray(*ppocs, NLTRAJECTORY_COLORPICKPACK_drawKinsArray);
				// ptrj->drawChunksArray(*ppocs, NLTRAJECTORY_COLORPICKPACK_drawKinsArray);
				// ptrj->drawUserVStagesArray(*ppocs,0);
				ptrj->drawMotionProfile(*ppocs, NLTRAJECTORY_COLORPICKPACK_drawKinsArray);
			}

			if (FLAGS_TEST((*ppocs)->m_Flags, FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_VEL_CHECK | FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_FT, FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_VEL_CHECK | FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_FT))
				ptrj->drawReverseVelocityCheck(*ppocs, NLTRAJECTORY_COLORPICKPACK_drawReverseVelocityCheck);

			if (ISFLAG_ON((*ppocs)->m_Flags, FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_KEYPOINT_ID))
			{
				ptrj->drawKnodes(*ppocs, NLTRAJECTORY_COLORPICKPACK_drawKnodes); // temporaire !!! devrait avoir son propre flag de tracage !!!
				ptrj->drawMotionProfilePathKeyPointId(*ppocs);
			}
			if (ISFLAG_ON((*ppocs)->m_Flags, FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_VSTAGE))
				ptrj->drawVStages(*ppocs, NLTRAJECTORY_COLORPICKPACK_drawVStages);

			(*ppocs)->draw(DEFAULT_NL2DOCS_COLOR_PICKPACK);
		}
	}

	// SIMULATION
	if (VirtualRobot.IsOn())
	{
		VirtualRobot.update(Project.m_simulationDt);

		NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_3D);
		// Robot.draw();			// Draw the ghost ( position et orient. estim�e par odom�trie )

		VirtualRobot.draw(); // Draw the real one
	}

	//	Trajectory.drawKinsArray(&OrthogonalCoordinateSystem_A);
	//	Trajectory.drawKnodeArray(&MotionProfileOcs);
	//	Trajectory.drawVelocityStages(&OrthogonalCoordinateSystem_A);
	// MotionControler.draw();

	// OCS
	// NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC); est d�j� ON, normalement.
	//	MotionProfileOcs.draw();

	NCallCurrentToolTimeHandle(ptime);
	// Affichage Slider Zoom Camera
	if (user_Nu32 == STATE_EDITOR_CAMERA_EDIT_Z)
	{
		NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC);
		NUT_Draw_SliderOnScreenPlane(&CameraSlider, &color, NULL);
	}
}

Nu32 _Camera_EventHandle(const NEVENT *pevent, const NEDITOR_RESULT current_tool_result, Nu32 user_Nu32)
{
	NVEC3 pos, prevpos;
	NVEC3 vect;
	NVEC2f32 v2f32;
	NVEC2f32 sliderxtend;
	NVEC2f32 sliderbase;
	NVEC2f32 sliderdir;

	NTOUCH *ptouch;
	static NCAMERA basecamera;

	// Event was initially build up for the game state from where this function is called.
	// check if this game state manage only one Touch ... if not ... CRASH ! because this Camera Event Handle
	// is not able to manage more than ONE SINGLE TOUCH !!!
	NErrorIf(((NGAMESTATE *)pevent->Head.pRecipient)->Listener.LTouchCountMax != 1, NERROR_SYSTEM_CHECK);

	if (current_tool_result != NEDITOR_RESULT_TOOL_ON) // To manage camera only if current tool does nothing !
	{
		// NEW Camera PAN (Multi Touch !!! )
		switch (pevent->Type)
		{
		case NTOUCH_START:
			// Of course, incoming "pevent" comes from current Game state !!!
			NErrorIf(pevent->Touch.RecipientCast != NEVENT_RECIPIENT_CAST_GAMESTATE, NERROR_INCONSISTENT_VALUES);
			if (NIsListenerFirstTouchStarted(&((NGAMESTATE *)pevent->Touch.pRecipient)->Listener))
			{
				NCoord_PixToHRel(&v2f32, NGetEventTouchPositionPtr(pevent, 0));
				if (v2f32.x > NCoord_NormToHRel_X(0.98f))
				{
					user_Nu32 = STATE_EDITOR_CAMERA_EDIT_Z;
					sliderxtend.x = 0.95f;
					sliderxtend.y = 0.025f;
					sliderdir.x = 0.0f;
					sliderdir.y = -1.0f;
					sliderbase.x = NCoord_NormToHRel_X(0.985f);
					sliderbase.y = NCoord_NormToHRel_Y(-0.025f);

					NUT_SetupSlider(&CameraSlider, 1.0f, 41.0f, &sliderdir, &sliderxtend, 1, 8, &sliderbase);
					NUT_EnableSliderRelativeHandle(&CameraSlider);
					NUT_HideSliderOutputValue(&CameraSlider);
					// NUT_SetSliderLockedDownRectf(&Slider, NCoord_NormToHRel_X(0.05f), NCoord_NormToHRel_X(0.95f), NCoord_NormToHRel_Y(-0.95f), NCoord_NormToHRel_Y(-0.05f), NTRUE);
					NUT_PlaceSliderOnScreenPlane(&CameraSlider, NGetCamera()->Pos.z, NGetEventTouchPositionPtr(pevent, 0));
				}
				else
				{
					basecamera = *NGetCamera();
					user_Nu32 = STATE_EDITOR_CAMERA_TOUCH;
				}
			}
			break;

		case NTOUCH_MOVE:
			ptouch = NGetFastestTouchPtr(&pevent->Touch.TouchSequence);
			if (user_Nu32 == STATE_EDITOR_CAMERA_EDIT_Z)
			{
				NSetCameraPositionZf(NUT_UpdateSliderOnScreenPlane(&CameraSlider, NGetEventTouchPositionPtr(pevent, 0)));
			}
			else if (user_Nu32 == STATE_EDITOR_CAMERA_TOUCH)
			{
				NUT_Get3DMouseEx(&pos, &basecamera, &ptouch->Position, basecamera.Pos.z);
				NUT_Get3DMouseEx(&prevpos, &basecamera, &ptouch->PreviousPosition, basecamera.Pos.z);
				NVec3Sub(&vect, &prevpos, &pos);

				NMoveCameraPosition(&vect);
				NMoveCameraTargetPosition(&vect);
			}
			else // Do things exactly like for NTOUCH_START to get in on fly !
			{
				// pcamera = NGetCamera();
				basecamera = *NGetCamera();
				user_Nu32 = STATE_EDITOR_CAMERA_TOUCH;
			}
			break;

		case NTOUCH_END:
			// Of course, incoming "pevent" comes from current Game state !!!
			NErrorIf(pevent->Touch.RecipientCast != NEVENT_RECIPIENT_CAST_GAMESTATE, NERROR_INCONSISTENT_VALUES);
			if (NIsListenerLastTouchEnded(&((NGAMESTATE *)pevent->Touch.pRecipient)->Listener))
			{
				if (user_Nu32 == STATE_EDITOR_CAMERA_EDIT_Z)
				{
					NUT_ClearSlider(&CameraSlider);
					user_Nu32 = STATE_EDITOR_CAMERA_NULL;
				}
			}
			break;

		case NTOUCH_CANCEL:
			// Of course, incoming "pevent" comes from current Game state !!!
			NErrorIf(pevent->Touch.RecipientCast != NEVENT_RECIPIENT_CAST_GAMESTATE, NERROR_INCONSISTENT_VALUES);
			if (NIsListenerLastTouchEnded(&((NGAMESTATE *)pevent->Touch.pRecipient)->Listener))
			{
				if (user_Nu32 == STATE_EDITOR_CAMERA_EDIT_Z)
				{
					NUT_ClearSlider(&CameraSlider);
					user_Nu32 = STATE_EDITOR_CAMERA_NULL;
				}
			}
			break;

		default:
			break;
		}
	}
	else
	{
		user_Nu32 = STATE_EDITOR_CAMERA_NULL;
	}

	return user_Nu32;
}

void App_GameState_Main_EventHandle(const NEVENT *pevent, const Nu32 user_Nu32)
{
	//	if (ISFLAG_OFF(MotionControler.m_drawFlags, FLAG_NLMOTIONCONTROLER_DRAW_ROBOT_SIMULATION))
	//	{
	NSetGameState_User_Nu32(STATE_MAIN, _Camera_EventHandle(pevent, NCallCurrentTool(pevent, NULL), user_Nu32));
	//	}
}
