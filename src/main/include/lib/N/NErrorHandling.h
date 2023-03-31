/*
#ifndef __NERRORHANDLING_H
#define __NERRORHANDLING_H
*/
//#include "NType.h"
#include <assert.h>
/*
#ifdef __cplusplus
extern "C"
{
#endif
*/
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

	// There are 2 ways to manage error in N.
	// -----------------------------------------------------------------------------------------------------------
	// 1)	NErrorIf() function.
	//		This function works like an Assert, but the program ends if the test is true ( opposite of assert ).
	//		This function is effective in debug mode only, and is totally ignored in release mode. 		
	//		Use NErrorIf() to check for errors that should never occur. 
	//
#ifdef _DEBUG
#define NErrorIf(tst,error_code)	assert(!(tst))
#endif

#ifndef _DEBUG
#define NErrorIf(tst,error_code)	
#endif
	// 2)	specific error code return by some functions. They are negative numbers or NULL pointer.
	//		Use/Check for errors that might occur. Notice that there are only a few number of function that return error code.
	//		Each case is unique and documented by function.

// Error codes
// -----------
#define NERROR_GENERIC																	-1

#define NERROR_SYSTEM_CLEARGRADIENT_COLOR_OUTOFRANGE									-2
#define NERROR_SYSTEM_CLEARGRADIENT_UNEXPECTED_ARRAY									-3

#define NERROR_SYSTEM_NENGINEVERSION_CONFLICT											-4
#define NERROR_SYSTEM_INVALID_CONSTANT													-5
#define NERROR_SYSTEM_MEMORY_CORRUPTION													-6
#define NERROR_SYSTEM_DATASTRUCTURE_CORRUPTION											-7
#define NERROR_SYSTEM_GURU_MEDITATION													-8	// Something bad happens due to a BIG mistake...for fun, AMIGA ... you remember ?
#define NERROR_SYSTEM_USELESS_CALL														-9	// Engine specific, an useless function call happens ... check and optimize !!!	
#define NERROR_SYSTEM_TODO																-10	// Engine Specific. Temporary Error, something has to be finished ... 	
#define NERROR_SYSTEM_CHECK																-11	// Engine Specific. Temporary Error, something has to be check ... (it's done, but N wants a human check on the case ... to validate the algorithm )	
#define NERROR_SYSTEM_RESERVED_VALUE													-12	// User try to use a system reserved value to setup a variable or a structure member. 

// -----------------------------------------------------------------------------------------
#define	NERROR_NULL_MALLOC																-20
#define	NERROR_NULL_REALLOC																-21
#define	NERROR_NULL_POINTER																-22
#define NERROR_NULL_VALUE																-23	// A no NULL value is requested, and its not the case.
#define NERROR_NULL_VECTOR																-24
#define NERROR_NULL_SIZE																-25
#define NERROR_NON_NULL_POINTER															-26
#define NERROR_NON_NULL_VALUE															-27
#define	NERROR_VALUE_CLOSE_TO_ZERO														-28
// -----------------------------------------------------------------------------------------
#define NERROR_ADDRESS_OUTOFRANGE														-50 // |_same meaning !
#define NERROR_POINTER_OUTOFRANGE														-50 // | 
#define NERROR_VALUE_OUTOFRANGE															-51
#define NERROR_4BITS_VALUE_OUTOFRANGE													-52
#define NERROR_NU8_OUTOFRANGE															-53
#define NERROR_NS8_OUTOFRANGE															-54
#define NERROR_NU16_OUTOFRANGE															-55
#define NERROR_NS32_OUTOFRANGE															-56
#define NERROR_ENUM_OUTOFRANGE															-57
#define NERROR_NU32_NEGATIVE_VALUE_OUTOFRANGE											-58	// User try to assign a negative value to a Nu32 
// -----------------------------------------------------------------------------------------
#define NERROR_STACK_OVERFLOW															-75
#define NERROR_MEMORY_UNEXPECTED_REALLOCATION_WILL_OCCUR								-76	// a memory reallocation (by a array capacity increase for example) is going to happen and it's not suppose to be the case !!!
// -----------------------------------------------------------------------------------------
#define NERROR_INVALID_DESCRIPTION														-100
#define NERROR_INVALID_PARAMETER														-101
#define NERROR_INVALID_RESULT															-102
#define NERROR_INVALID_CASE                                                             -103 // Case doesn't exist.
#define NERROR_INCONSISTENT_PARAMETERS													-104
#define NERROR_INCONSISTENT_VALUES														-105
#define NERROR_INCONSISTENT_FLAGS														-106
#define NERROR_INCONSISTENT_REQUEST														-107
#define NERROR_UNEXPECTED_RESULT														-108
// -----------------------------------------------------------------------------------------
#define NERROR_UNAUTHORIZED_ACTION														-200
#define NERROR_UNAUTHORIZED_REQUEST														-201
#define NERROR_UNAUTHORIZED_VALUE														-202
#define NERROR_UNAUTHORIZED_CASE														-203 // Case exist, but is not authorized here !
#define NERROR_UNAUTHORIZED_FLAG														-204 // Flag exist, but is not authorized here !
// -----------------------------------------------------------------------------------------
#define NERROR_ALREADY_DONE																-250
#define NERROR_IN_USE																	-251
// -----------------------------------------------------------------------------------------
//#define NERROR_BEYOND_TYPE_LIMITS														-300
//#define NERROR_BEYOND_Nbool_LIMITS														-301
//#define NERROR_BEYOND_Nu8_LIMITS														-302
//#define NERROR_BEYOND_Nchar_LIMITS														-303
//#define NERROR_BEYOND_Nu16_LIMITS														-304
//#define NERROR_BEYOND_Ns16_LIMITS														-305
// -----------------------------------------------------------------------------------------
#define NERROR_NVOID_VALUE																-400	// A no NVOID value is requested, and its not the case.
#define NERROR_ODD_NUMBER																-401	// An Even number is requested, and it's not the case.
#define NERROR_WRONG_VALUE																-402	// A value is not what is suppose to be
#define NERROR_VALUES_HAVE_TO_BE_DIFFERENT												-403
#define NERROR_INDEX_OUTOFRANGE															-404
#define NERROR_WRONG_SIZE																-405
#define NERROR_WRONG_ADDRESS															-406
// -----------------------------------------------------------------------------------------
#define NERROR_WRONG_STRING																-500	// A string of characters is not what is suppose to be
#define NERROR_STRING_TOO_LONG															-501	// A string of characters is too long !
#define NERROR_STRING_TOO_SHORT															-502	// A string of characters is too short !
#define NERROR_STRING_NULL																-503	// A string of characters is requested and its not the case !
#define NERROR_STRING_SIZEMAX_HAS_TO_BE_EQUAL_OR_GREATER_THAN_1							-504

// -----------------------------------------------------------------------------------------
#define NERROR_MEMORY_CHECK_FAILED														-600
#define NERROR_CLEAR_NOT_ALLOWED_STILL_LINKED_DATA										-601

// -----------------------------------------------------------------------------------------
#define NERROR_TRY_TO_DELETE_REFERENCED_TRANSFORM_NODE									-650

// -----------------------------------------------------------------------------------------
#define NERROR_COLORPALETTE_IS_NOT_RGBA													-670
#define NERROR_COLORPALETTE_IS_NOT_COLOR4F												-671

// -----------------------------------------------------------------------------------------
#define NERROR_GAMESTATE_UNAUTHORIZED_STATE_USE											-700
#define NERROR_GAMESTATE_STATE_ALREADY_ACTIVE											-701
#define NERROR_GAMESTATE_USELESS_PARENT													-702
#define NERROR_GAMESTATE_MACHINE_ACTIVE													-703
#define NERROR_GAMESTATE_CYCLIC_HIERARCHY												-704
#define NERROR_GAMESTATE_INVALID_NON_ROOT_TOUCHCOUNTMAX									-705
#define NERROR_GAMESTATE_IS_NOT_ROOT													-706
#define NERROR_GAMESTATE_MACHINE_LISTENED_TOUCH_ARRAY_MUST_BE_EMPTY						-707
// -----------------------------------------------------------------------------------------
#define NERROR_RINGBUFFER_OVERFLOW														-725
#define NERROR_RINGBUFFER_IS_EMPTY														-726
#define NERROR_RINGBUFFER_INDEX_OUTOFRANGE												-727
#define NERROR_RINGBUFFER_SIZE_OUTOFRANGE												-728

// -----------------------------------------------------------------------------------------
#define	NERROR_ARRAY_UNCLEARED															-800
#define NERROR_ARRAY_INDEX_BEYOND_LIMITS												-801
#define NERROR_ARRAY_WRONG_SIZE															-802
#define NERROR_ARRAY_WRONG_CAPACITY														-803
#define NERROR_ARRAY_NOT_EMPTY															-804
#define NERROR_ARRAY_IS_NOT_EMPTY														-804
#define NERROR_ARRAY_ATTEMPT_TO_CLEAR_USED_ELEMENT										-805
#define NERROR_ARRAY_ELEMENT_NOT_FOUND													-806
#define NERROR_ARRAY_ELEMENT_ALREADY_EXIST												-807
#define NERROR_ARRAY_ADRESS_BEYOND_LIMITS												-808 
#define NERROR_ARRAY_WRONG_ELEMENT_SIZE													-809
#define NERROR_ARRAY_REALLOCATION_FAILURE												-810
#define NERROR_ARRAY_PTR_BEYOND_LIMITS													-811
#define NERROR_ARRAY_CORRUPTED															-812
#define NERROR_ARRAY_CORRUPTED_PTR														-813
#define NERROR_ARRAY_IS_EMPTY															-814
#define NERROR_ARRAY_NULL_ADDRESS														-815
// -----------------------------------------------------------------------------------------
#define NERROR_HASHMAP_WRONG_BUCKETCOUNT												-825
#define NERROR_HASHMAP_INVALID_KEY														-826
#define NERROR_HASHMAP_INDEX_BEYOND_LIMITS												-827

// -----------------------------------------------------------------------------------------
#define NERROR_NXNODELIST_NOT_EMPTY														-850
// -----------------------------------------------------------------------------------------
#define NERROR_NNODE_INVALID_INDEX														-900
#define NERROR_NNODE_INVALID_NODE														-901
#define NERROR_NNODE_ORPHAN																-902
#define NERROR_NNODELIST_NOT_EMPTY														-903
#define NERROR_NODE_IS_NOT_ORPHAN														-904
#define NERROR_NODE_IS_ORPHAN															-905
#define NERROR_NODE_POINTER_INCONSISTENCY												-906

// -----------------------------------------------------------------------------------------
#define NERROR_NHIERARCHY_NODE_INVALID_HIERARCHY										-950
// -----------------------------------------------------------------------------------------
#define NERROR_POOL_ENABLE_XNODELIST_MISSING											-1001
#define NERROR_POOL_UNAUTHORIZED_CLEAR_REQUEST											-1002
#define NERROR_POOLTAG_MISSING															-1003
#define NERROR_POOLTAG_LIST_HAS_TO_BE_EMPTY												-1004
// -----------------------------------------------------------------------------------------
#define NERROR_PHYSICSTRUCTURE_CLEARREQUEST_UNAUTHORIZED_WITH_EXISTING_COLLISION_REF	-1100
#define NERROR_PHYSICSTRUCTURE_INVALID_NUMBER_OF_CONSTRAINTS							-1101
#define NERROR_PHYSICSTRUCTURE_INVALID_NUMBER_OF_JOINTS									-1102

#define NERROR_STRUCTURE_SKIN_VALID_GEOMETRY_REQUESTED									-1150
#define NERROR_STRUCTURE_SKIN_VALID_RENDERABLE_REQUESTED								-1151
#define NERROR_STRUCTURE_SKIN_UNKNOWN_GEOMETRY											-1152
#define NERROR_STRUCTURE_SKIN_UNAUTHORIZED_STRUCTURE_ELEMENT_TYPE						-1153
#define NERROR_STRUCTURE_SKIN_NOT_ENOUGH_REF_ELEMENT_TO_BUILD_REQUESTED_SKIN			-1154
#define NERROR_STRUCTURE_SKIN_UNEXPECTED_VALUE											-1155

// -----------------------------------------------------------------------------------------
#define NERROR_RENDERABLE_GEOMETRY_ARRAY_SETUP_FAILED									-1200
#define NERROR_RENDERABLE_UPDATE_FCT_INVALID											-1201
#define NERROR_RENDERABLE_UPDATE_FCT_ID_INVALID											-1202
#define NERROR_RENDERABLE_UPDATE_FCT_ALREADY_EXISTS										-1203
#define NERROR_RENDERABLE_UPDATE_FCT_LUT_CORRUPTED										-1204
#define NERROR_RENDERABLE_EXTRACT_FCT_INVALID											-1205
#define NERROR_RENDERABLE_EXTRACT_FCT_ID_INVALID										-1206
#define NERROR_RENDERABLE_EXTRACT_FCT_ALREADY_EXISTS									-1207
#define NERROR_RENDERABLE_EXTRACT_FCT_LUT_CORRUPTED										-1208

#define NERROR_RENDERABLE_EXTRACTSET_INVALID											-1209
#define NERROR_RENDERABLE_EXTRACTSET_ALREADY_EXISTS										-1210
#define NERROR_RENDERABLE_EXTRACTSET_UNAUTHORIZED_FLAGS_USING							-1211
// -----------------------------------------------------------------------------------------
#define NERROR_TEXT_FONT_MISSING														-1300

// -----------------------------------------------------------------------------------------
#define NERROR_SPLINE_NOT_ENOUGH_SPLINEKNOTS											-1400
#define NERROR_SPLINE_WRONG_ACCURACY													-1401
#define NERROR_SPLINE_INVALID_SPLINEKNOT												-1402
// + ---------------------------------------------------------------------------------------
// + EVENT
//#define NERROR_EVENT_IS_NOT_AUTHORIZED_TO_BE_QUEUED												-1450
//#define NERROR_EVENT_PRIVATE_EVENT_CAN_NOT_BE_PUBLIC											-1451
// #define NERROR_EVENT_IS_NOT_UI																	-1452
// #define NERROR_EVENT_IS_NOT_GS																	-1453
#define NERROR_EVENT_HAS_NOT_RECIPIENT															-1454
#define NERROR_EVENT_INCONSISTENT_RECIPIENT														-1455
#define	NERROR_EVENT_INCONSISTENT_TYPE															-1456
#define	NERROR_EVENT_TOUCH_CANCEL_MUST_BE_DISPATCHED											-1457
#define NERROR_EVENT_DISPATCH_UNAUTHORIZED														-1458
#define NERROR_EVENT_UNAUTHORIZED_MAIN_TYPE														-1459
#define NERROR_EVENT_UI_RECIPIENT_MUST_BE_ACTIVE												-1460
// + ---------------------------------------------------------------------------------------
// + UI
#define NERROR_UI_STYLE_MISSING																	-1500
#define NERROR_UI_FRAME_CLEAR_PROC_MISSING														-1501
#define NERROR_UI_QUAD_FRAME_BACKGROUND_IS_NECESSARY											-1502	// a UI needs to have a Simple Quad mesh as background, and doesn't have...
#define NERROR_UI_QUAD_FRAME_ICON_IS_NECESSARY													-1503
#define NERROR_UI_FRAME_CUSTOM_PICKING_GEOMETRYMESH_TRIANGLE_ONLY								-1504	// a UI picking mesh has to use triangle as geometry primitives !

#define NERROR_UI_OVERLOADED_GEOMETRY_CAPACITY													-1505
#define	NERROR_UI_OVERSIZED_GEOMETRY_CAPACITY													-1506
#define NERROR_UI_ATLAS_ELEMENT_MISSING															-1507
#define NERROR_UI_SHOULD_BE_ACTIVE_UIROOT														-1508
#define NERROR_UI_CYCLIC_HIERARCHY																-1509
#define NERROR_UI_ATTEMPT_TOMODIFY_ACTIVE_FRAME_HIERARCHY										-1510
#define NERROR_UIGEOMETRYDESC_TEXTURE_REQUESTED													-1511
#define NERROR_UIGEOMETRYDESC_CUSTOM_GEOMETRY_REQUESTED											-1512
#define NERROR_UI_PLACEMENTDESC_HAS_TO_BE_FULLY_DEFINED											-1513
#define NERROR_UI_ROOTFRAME_REQUESTED															-1514
#define NERROR_UI_UNAUTHORIZED_NUIRESULT														-1515
#define NERROR_UI_ROOT_SHOULD_HAVE_CHILD_WITH_FOCUS												-1516
#define NERROR_UIGEOMETRYDESC_SIZE_REQUESTED													-1517
#define NERROR_UI_FRAME_CUSTOM_PICKING_GEOMETRYMESH_REQUESTED									-1518
#define NERROR_UI_FRAME_BACKGROUND_GEOMETRY_REQUESTED											-1519
#define NERROR_UI_FRAME_ICON_GEOMETRY_REQUESTED													-1520
#define NERROR_UI_FRAME_NAME_TOO_LONG															-1521
#define NERROR_UI_FRAME_NAME_MISSING															-1522
#define NERROR_UI_WRONG_TRANSFORMED_VERTEX_ARRAY_SIZE											-1523
#define NERROR_UI_FRAME_WRONG_TEXT_UPDATE_AND_EXTRACT_FCT										-1524
#define NERROR_UI_FRAME_TEXT_DISPLAY_DEVICE_MISSING												-1525
#define NERROR_UI_UNKNOWN_SLIDINGLAYER_CONSTRAINT_CONTEXT										-1526
#define NERROR_UI_TEXCOORD_CONTEXT_HAS_TO_BE_FULLY_DEFINED										-1527
#define NERROR_UI_NAME_IS_LOCKED																-1528
#define NERROR_UI_SHOULD_BE_UIROOT																-1529
#define NERROR_UI_SHOULD_BE_UNACTIVE															-1530
#define NERROR_UI_IS_UNDER_CLEAR_PROCESS														-1531

#define NERROR_UI_TOUCH_KILL_EVENT_IGNORED														-1532	
#define NERROR_UI_HIDE_EVENT_IGNORED															-1533	
#define NERROR_UI_SHOW_EVENT_IGNORED															-1534	
#define NERROR_UI_DISABLE_EVENT_IGNORED															-1535
#define NERROR_UI_ENABLE_EVENT_IGNORED															-1536
#define NERROR_UI_NESTED_CLEAR																	-1537
#define NERROR_UI_NESTED_DELETE																	-1538
#define NERROR_UI_NESTED_ENABLE																	-1539
#define NERROR_UI_NESTED_DISABLE																-1540
#define NERROR_UI_NESTED_SHOW																	-1541
#define NERROR_UI_NESTED_HIDE																	-1542
#define NERROR_UI_ENABLE_UI_PARENT_MUST_BE_ENABLE												-1543
#define NERROR_UI_PARENT_OF_VISIBLE_UI_MUST_BE_VISIBLE												-1544
#define NERROR_UI_SHOWED_MUST_HAVE_HIDDEN_RENDER_UPDATE_FCT										-1545										
#define NERROR_UI_SETUPPROCESS_STACK_CORRUPTED													-1546		
#define NERROR_UI_COLOR_UPDATE_IS_OFF															-1547
#define NERROR_UI_UNAUTHORIZED_TOUCHCOUNTMAX													-1548	
#define	NERROR_UI_INCOMPATIBLE_STATES															-1549
// + ---------------------------------------------------------------------------------------
// + NTIMER
#define NERROR_TIMER_WRONG_ID																	-1580
#define NERROR_TIMER_UNKNOWN_ID																	-1581
// + ---------------------------------------------------------------------------------------
// + FILE
#define NERROR_FILE_FSEEK_ERROR																	-1600
#define NERROR_FILE_ELEMENT_SIZE_IS_NULL														-1601
#define NERROR_FILE_ELEMENT_COUNT_IS_NULL														-1602
#define NERROR_FILE_READ_ERROR																	-1603
#define NERROR_FILE_WRITE_ERROR																	-1604
#define NERROR_FILE_OPENING_ERROR																-1605
#define NERROR_FILE_UNKNOWN_SIGNATURE															-1606
#define NERROR_FILE_UNKNOWN_VERSIONTAG															-1607
#define NERROR_FILE_UNEXPECTED_READ_VALUE														-1608
#define NERROR_FILE_INCONSISTENT_FLAGS															-1609
#define NERROR_FILE_INVALID_EXTENSION															-1610
#define NERROR_FILE_INCONSISTENT_FILENAME														-1611
#define NERROR_FILE_FILENAME_MUST_BE_ROOT_RELATIVE												-1612
#define NERROR_FILE_FILENAME_MUST_BE_ABSOLUT													-1613
#define NERROR_UNAUTHORIZED_DRIVE_ROOT_FILENAME													-1614
#define NERROR_FILESYSTEM_UNDEFINED_PATH_MODE													-1615
#define NERROR_FILE_ALREADY_DETECTED_ERROR														-1616
// -----------------------------------------------------------------------------------------
#define NERROR_TOUCH_ALREADY_LISTEN																-1650
#define NERROR_TOUCH_INCONSISTENT_PHASE															-1651
#define NERROR_TOUCH_LISTENER_HAS_TO_BE_CLEAN_UP												-1652
#define NERROR_TOUCH_LISTENER_IS_ALREADY_CLEAN													-1653
#define NERROR_TOUCH_LISTENER_HAS_TO_BE_NULL													-1654

#define NERROR_TOUCH_EMUL_HAS_TO_BE_UNUSED														-1655
// -----------------------------------------------------------------------------------------
#define NERROR_CLEAR_SURFACE_NULL																-1700

// -----------------------------------------------------------------------------------------
#define NERROR_ACCUMULATOR_MULTIPLE_RESULTS														-1801
#define NERROR_ACCUMULATOR_INVALID																-1803
#define NERROR_ACCUMULATOR_NOT_FOUND															-1804
#define NERROR_ACCUMULATOR_INCONSISTENCY														-1805
#define NERROR_STATESET_UNAUTHORIZED_FLAGS_USING												-1806
// + ---------------------------------------------------------------------------------------
// + GEOMETRY
#define NERROR_GEOMETRY_WRONG_PRIMITIVE_ARRAY_SIZE												-1900
#define NERROR_GEOMETRY_TOTAL_CLONES_NUMBER_HAS_TO_BE_GREATER_THAN_ONE							-1901
#define NERROR_GEOMETRY_NOT_ENOUGH_PRIMITIVE													-1902
#define NERROR_GEOMETRY_WRONG_COLOR_ARRAY_SIZE													-1903
#define NERROR_GEOMETRY_WRONG_UVUNIT_NUMBER														-1904
#define NERROR_GEOMETRY_OUTOFRANGE_UVSETID														-1905
#define NERROR_GEOMETRY_MISSING_UVSET															-1906
#define	NERROR_GEOMETRY_MISSING_TEXTURE															-1907
#define NERROR_GEOMETRY_UNKNOWN_ID																-1908
#define NERROR_GEOMETRY_NOT_ENOUGH_UVSET														-1909
#define NERROR_GEOMETRY_DESCRIPTION_FLAGS_NOT_SUPPORTED											-1910
#define NERROR_GEOMETRY_WRONG_TEXTURE_NUMBER													-1911
#define NERROR_GEOMETRY_NOT_ENOUGH_TEXUNIT														-1912
#define NERROR_GEOMETRY_INCONSISTENT_PARAMETER													-1913
//#define NERROR_GEOMETRY_NOT_ENOUGH_TRIANGLE														-903
//#define NERROR_GEOMETRY_NOT_ENOUGH_QUAD															-904
#define NERROR_GEOMETRY_NOT_ENOUGH_VERTEX														-1914
#define NERROR_GEOMETRY_UNKNOWN_RENDER_FUNCTION													-1915
//#define NERROR_GEOMETRY_NOT_ENOUGH_UV															-906
//#define NERROR_GEOMETRY_NOT_ENOUGH_COLOR														-907
#define NERROR_GEOMETRY_OUTOFRANGE_TEXUNIT														-1916
#define NERROR_GEOMETRY_INCOMPATIBLE_TYPE														-1917
#define NERROR_GEOMETRY_NOT_UNLINKED															-1918	
#define NERROR_GEOMETRY_WRONG_LINKCOUNT															-1919
#define NERROR_GEOMETRY_VERTEX_UVS_SIZE_INCONSISTENCY											-1920
#define NERROR_GEOMETRY_STRUCTURE_CORRUPTED														-1921
#define NERROR_GEOMETRY_COMPONENT_NOT_INCLUDED													-1922
#define NERROR_GEOMETRY_NULL_UVSETSIZE															-1923
#define NERROR_GEOMETRY_NO_UVSET																-1924
#define NERROR_GEOMETRY_NO_TEXTUREUNIT															-1925
#define NERROR_GEOMETRY_VERTEX_UVS_CAPACITY_INCONSISTENCY										-1926
#define NERROR_GEOMETRY_LINKCOUNT_OVERLOADED													-1927
#define NERROR_GEOMETRY_WRONG_VERTEX_FORMAT														-1928
#define NERROR_GEOMETRY_MAPS_NEEDS_ONE_TEXTUREUNIT_AT_LEAST										-1929
#define NERROR_GEOMETRY_MAPS_ALREADY_CLEARED													-1930
#define NERROR_GEOMETRY_TEXTUREUNIT_OUTOFRANGE													-1931
#define NERROR_GEOMETRY_OUTOFRANGE_RENDERFCTID													-1932
#define NERROR_GEOMETRY_WRONG_VERTEX_STRUCTURESIZE												-1933
#define NERROR_GEOMETRY_WRONG_PRIMITIVE_STRUCTURESIZE											-1934

#define NERROR_GEOMETRY_UNSUPPORTED_GEOMETRY_FORMAT												-1935
#define NERROR_GEOMETRY_UNSUPPORTED_PRIMITIVE_FORMAT											-1936
#define NERROR_GEOMETRY_UNSUPPORTED_VERTEX_FORMAT												-1937
#define NERROR_GEOMETRY_UNSUPPORTED_APPEARANCE_FORMAT											-1938
#define NERROR_GEOMETRY_UNSUPPORTED_TEXTUREUNIT_FORMAT											-1939
#define NERROR_GEOMETRY_UNSUPPORTED_TEXTUREUNIT_NUMBER											-1940
#define NERROR_GEOMETRY_UNSUPPORTED_BLEND_FORMAT												-1941
#define NERROR_GEOMETRY_UNSUPPORTED_MATERIAL_FORMAT												-1942

#define NERROR_GEOMETRY_APPEARANCE_INCOMPATIBLE_TYPE											-1943
#define NERROR_GEOMETRY_TEXTUREUNIT_INCOMPATIBLE_TYPE											-1944
#define NERROR_GEOMETRY_UNSUPPORTED_MESH_DESCRIPTION											-1945
#define NERROR_GEOMETRY_UNSUPPORTED_APPEARANCE_DESCRIPTION										-1946
#define NERROR_GEOMETRY_DESCRIPTION_STRUCTURE_INCONSISTENCY										-1947
#define NERROR_GEOMETRY_INCONSISTENT_VERTEX_STRIDES												-1948
#define NERROR_GEOMETRY_UNEXPECTED_VERTEX_FORMAT												-1949
#define NERROR_GEOMETRY_UNEXPECTED_PRIMITIVE_FORMAT												-1940
#define NERROR_GEOMETRY_UNEXPECTED_APPEARANCE_FORMAT											-1951
#define NERROR_GEOMETRY_UNEXPECTED_TEXTUREUNIT_FORMAT											-1952
#define NERROR_GEOMETRY_UNEXPECTED_TEXTUREUNIT_NUMBER											-1953
#define NERROR_GEOMETRY_UNEXPECTED_VERTEX_DATA													-1954
#define NERROR_GEOMETRY_UNEXPECTED_TEXTURE														-1955
#define	NERROR_GEOMETRY_APPEARANCE_NOT_INCLUDED													-1956	
#define NERROR_GEOMETRY_MATERIAL_NOT_SUPPORTED													-1957
#define NERROR_GEOMETRY_BLENDPARAMS_NOT_SUPPORTED												-1958
#define NERROR_GEOMETRY_TEXTURE_NOT_SUPPORTED													-1959
#define NERROR_GEOMETRY_OUTOFRANGE_VERTEXID														-1960	
#define NERROR_GEOMETRY_OUTOFRANGE_PRIMITIVEID													-1961
#define NERROR_GEOMETRY_TEXCOORD0_2f_NOT_SUPPORTED												-1962
#define NERROR_GEOMETRY_TEXCOORD1_2f_NOT_SUPPORTED												-1963
#define NERROR_GEOMETRY_COLOR0_4f_NOT_SUPPORTED													-1964
#define NERROR_GEOMETRY_COLOR1_4f_NOT_SUPPORTED													-1965
#define NERROR_GEOMETRY_UNEXPECTED_VERTEX_STRUCTURE_SIZE										-1966
#define NERROR_GEOMETRY_UNEXPECTED_PRIMITIVE_STRUCTURE_SIZE										-1967

#define NERROR_PRIMITIVE_DEGENERATED_TRIANGLE													-1990
#define NERROR_PRIMITIVE_INCONSISTENT_WINDING_ORDER												-1991
// + ---------------------------------------------------------------------------------------	
// + MATHS
#define NERROR_DIVIDE_BY_ZERO																	-2000


#define NERROR_MATRIX_NULL_W																	-2050
#define NERROR_MATRIX_NULL_Z																	-2051
#define NERROR_MATRIX_IN_OUT_MATRIX_POINTERS_HAS_TO_BE_DIFFERENT								-2052
#define NERROR_MATRIX_IN_OUT_VECTOR4_POINTERS_HAS_TO_BE_DIFFERENT								-2053
#define NERROR_MATRIX_IN_OUT_VECTOR3_POINTERS_HAS_TO_BE_DIFFERENT								-2054
#define NERROR_MATRIX_IN_OUT_VECTOR2_POINTERS_HAS_TO_BE_DIFFERENT								-2055

// + ---------------------------------------------------------------------------------------
// + IMAGE
#define NERROR_IMAGE_INVALID_IMAGEID															-2100 
#define NERROR_IMAGE_NULL_WIDTH																	-2101
#define NERROR_IMAGE_NULL_HEIGHT																-2102
#define NERROR_IMAGE_NULL_BPP																	-2103
#define NERROR_IMAGE_NULL_COLORMAP_DEPTH														-2104
#define NERROR_TGA_INCONSISTENT_COLORMAP_DEPTH													-2105
#define NERROR_IMAGE_INVALID_BPP																-2106
#define NERROR_IMAGE_INVALID_COLORMAP_SIZE														-2107
#define NERROR_IMAGE_INVALID_WIDTH																-2108
#define NERROR_IMAGE_INVALID_HEIGHT																-2109
#define NERROR_IMAGE_INVALID_CHANNEL															-2110
#define NERROR_IMAGE_SURFACE_ALREADY_ALLOCATED													-2111
#define NERROR_IMAGE_CONVERT_TARGETFORMAT_EQUAL_SOURCEFORMAT									-2112

// + ---------------------------------------------------------------------------------------
// + TGA FILE
#define NERROR_TGA_UNKNOWN_TYPE																	-2200
#define NERROR_TGA_UNKNOWN_BPP																	-2201
#define NERROR_TGA_UNKNOWN_COLORMAP_DEPTH														-2202
#define NERROR_TGA_COLORMAP_MISSING																-2203
#define NERROR_TGA_IMAGEDATA_MISSING															-2204
#define NERROR_TGA_COLORMAP_SIZE_OUTOFRANGE														-2205
#define NERROR_TGA_COLORMAP_SIZE_NULL															-2206
#define NERROR_TGA_COLORMAP_TYPE_SHOULD_HAVE_8BPP												-2207
#define NERROR_TGA_INCONSISTENT_TYPE_AND_BPP													-2208
#define NERROR_TGA_GRAYSCALE_SHOULDNT_HAVE_COLORMAP												-2209
#define NERROR_TGA_RGB16_SHOULDNT_HAVE_COLORMAP													-2210
#define NERROR_TGA_RGB24_SHOULDNT_HAVE_COLORMAP													-2211
#define NERROR_TGA_RGB32_SHOULDNT_HAVE_COLORMAP													-2212

#define NERROR_TGA_COLORMAP_LENGTH_SHOULD_BE_NULL												-2213
#define NERROR_TGA_COLORMAP_DEPTH_SHOULD_BE_NULL												-2214	
#define NERROR_TGA_TYPE0																		-2215
#define NERROR_TGA_LOADING_ERROR																-2216
// + ---------------------------------------------------------------------------------------
// + TEXTURE
#define NERROR_TEXTURE_UNSUPPORTED_IMAGEID														-2300
#define NERROR_TEXTURE_INVALID_LIST																-2301
#define NERROR_TEXTURE_BANK_TEXTURE_NAME_DOESNT_EXIST											-2302

// + ---------------------------------------------------------------------------------------
// + ATLAS ELEMENT
#define NERROR_ATLAS_ELEMENT_NAME_DOESNT_EXIST													-2350	

// + ---------------------------------------------------------------------------------------
// + MATRIX
#define NERROR_MATRIX_INVALID_VALUE																-2500

// + ---------------------------------------------------------------------------------------
// + FLAGs
#define NERROR_FLAG_INCONSISTENCY																-2600

// + ---------------------------------------------------------------------------------------
// + MISC
#define NERROR_COLLINEAR_VECTORS																-2700
#define NERROR_NON_UNIT_VECTOR																	-2701
#define NERROR_INVALID_EXTENT																	-2702

// + ---------------------------------------------------------------------------------------
// + MISC
#define NERROR_NLIBRARY_KEYNAME_AND_PATHFILESYSTEM_INCONSISTENCY								-3000
#define NERROR_NLIBRARY_NON_UNIQUE_ENTRY														-3001
#define NERROR_NUT_UIDESK_PANEL_IS_OPEN															-3500
#define NERROR_NUT_UIDESK_PANEL_IS_CLOSE														-3501
#define NERROR_NUT_UIDESK_PANEL_UI_MUST_HAVE_THE_SAME_THN_HIERARCHY								-3502
//#define NERROR_NUT_UIDESK_PANEL_REQUIRES_CONSISTENT_SIZE										-3503
#define NERROR_RECT_IS_OAARECT																	-3503
#define NERROR_FASTRAND_SIZE_OUTOFRANGE															-3504
#define NERROR_BATCHLOAD_TEXTURES_SYNTAX_ERROR													-3505
#define NERROR_NUT_Nf32SET_BUILDER_ID_OUTOFRANGE												-3508
#define NERROR_NUT_Nf32SET_BUILDER_IS_NOT_COMPOSITE												-3512
#define NERROR_NUT_Nf32SET_BUILDER_IS_COMPOSITE													-3513

#define NERROR_NUT_Nf32SET_SEQUENCE_STRIDE_OVERLAPING											-3514
#define NERROR_NUT_Nf32SET_BUILDER_OPERAND_OUTOFRANGE											-3515

#define NERROR_NUT_Nf32SET_FILTER_IS_COMPOSITE													-3516
#define NERROR_NUT_Nf32SET_FILTER_IS_NOT_COMPOSITE												-3517
#define NERROR_NUT_Nf32SET_FILTER_ID_OUTOFRANGE													-3518
#define NERROR_NUT_Nf32SET_FILTER_REFERENCE_FOR_SPREADOUT_OUTOFRANGE							-3519

#define NERROR_NUT_Nf32SET_PROCESS_ID_OUTOFRANGE												-3520
#define NERROR_NUT_Nf32SET_SEQUENCE_SIZE_TOO_SHORT												-3521

// + ---------------------------------------------------------------------------------------
// + N AUDIO
#define NERROR_AUDIO_OPEN_DEVICE_FAIL															-3700
#define NERROR_AUDIO_CONTEXT_CREATION_FAIL														-3701
#define NERROR_AUDIO_CONTEXT_FAIL																-3702
#define NERROR_AUDIO_FILE_IS_NOT_A_RIFF_FILE													-3703
#define NERROR_AUDIO_RIFF_FILE_IS_NOT_A_WAVE													-3704
#define NERROR_AUDIO_RIFF_WAVE_FILE_UNEXPECTED_FORMAT											-3705
#define NERROR_AUDIO_RIFF_WAVE_FILE_CORRUPTED													-3706
#define NERROR_AUDIO_RIFF_WAVE_FILE_UNEXPECTED_DATA_ID											-3707
#define NERROR_AUDIO_RIFF_WAVE_FILE_UNEXPECTED_DATA_SIZE										-3708
#define NERROR_AUDIO_RIFF_WAVE_FILE_IS_NOT_PCM													-3709
#define NERROR_AUDIO_WAVE_UNEXPECTED_BPS														-3710
#define NERROR_AUDIO_WAVE_UNEXPECTED_CHANNEL_NUMBER												-3711
#define NERROR_AUDIOBUFFER_STILL_IN_USE															-3712
#define NERROR_AUDIOBUFFER_INVALID_NAME															-3713
#define NERROR_AUDIOBUFFER_INVALID_REQUESTED_BUFFER_NB											-3714
#define NERROR_AUDIOBUFFER_UNEXPECTED_CASE														-3715
#define NERROR_AUDIOSOURCE_NO_CONTEXT															-3716
#define NERROR_AUDIOSOURCE_OUT_OF_MEMORY														-3717
#define NERROR_AUDIOSOURCE_OUT_OF_RESOURCES														-3718
#define NERROR_AUDIOSOURCE_UNEXPECTED_CASE														-3719
#define	NERROR_AUDIOSOURCE_INVALID_NAME															-3720
#define NERROR_AUDIO																			-3721
// + ---------------------------------------------------------------------------------------
// + NUT LIBRARIES
#define NERROR_UNKNOWN_BLENDPRESET																-4000											

// + ---------------------------------------------------------------------------------------
// + Functions Specific
#define NERROR_NCOPYVERTEX_PREFER_USING_EQUALITY_INSTEAD										-10000 // 'NCopyVertex' function tells an advice: prefer using 'Vertex_dst = Vertex_src', because the 2 have the same format


#define NERROR_XTRI_ALREADY_USED																-20001
#define NERROR_XTRI_NO_ADJACENT_XTRI															-20002
#define NERROR_XTRI_ALREADY_LOCKED																-20003
#define NERROR_XTRI_UNLOCKED																	-20004

#define NERROR_XQUAD_ALREADY_IN_AREA															-20010
#define NERROR_XQUAD_ALREADY_IN_RPOLY															-20011
#define NERROR_XQUAD_ALREADY_LOCKED																-20012
#define NERROR_XQUAD_UNLOCKED																	-20013
#define NERROR_XQUAD_EDGE_IS_DIAGONAL															-20014
#define NERROR_XQUAD_NOT_IN_AREA																-20015
#define NERROR_XQUAD_NOT_IN_RPOLY																-20016
#define NERROR_XQUAD_GRIDCOORDS_NON_NULL														-20017
#define	NERROR_XMESH_INCONSISTENCY																-20018
#define NERROR_XMESH_RPOLY_PROCESSING_QUEUE_NOT_EMPTY											-20019
#define NERROR_XMESH_RPOLY_PROCESSING_RPOLY_NOT_EMPTY											-20020
#define NERROR_XMESH_RPOLY_PROCESSING_RPOLY_EMPTY												-20021
#define NERROR_XMESH_XQUAD_ALREADY_INSERTED_IN													-20022
#define NERROR_XMESH_RPOLY_TEXCOORD_PROCESSING_INCONSISTENCY									-20023

/*
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NERRORHANDLING_H 
*/
