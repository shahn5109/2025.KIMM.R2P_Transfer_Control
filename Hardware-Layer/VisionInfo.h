#ifndef VISION_INFO_H
#define VISION_INFO_H

/**
 * VisionInfo.h : This file defines object type of System.
 *            
 *
 */
//===================================================================================
//===================================================================================
#define _INSP_CAM_DCF		"RS-170"
//===================================================================================
#define _VGA_MODE	false
#define _DEV_MODE	true
//===================================================================================
/** System Error Define */
//const	int	ERR_SYSTEM_SUCCESS					= 1;		// 己傍 贸府 Return Value
//const	int	ERR_SYSTEM_CANNOT_FIND_OBJECT		= 0;
//const	int	ERR_SYSTEM_INITIAL_OK				= 1;		// 己傍 贸府 Return Value
//const	int	ERR_SYSTEM_INITIAL_ERROR			= 0;
//===================================================================================
#define _SYSTEM_LOG		0
#define _ERROR_LOG		1
#define _MOTION_LOG		2
#define _THREAD_LOG		3
//===================================================================================
#define _DRAW_RECT_R	255
#define _DRAW_RECT_G	160
#define _DRAW_RECT_B	160

#define _DRAW_TEXT_R	150
#define _DRAW_TEXT_G	150
#define _DRAW_TEXT_B	150

#define _ROI_RECT_R1	179
#define _ROI_RECT_G1	179
#define _ROI_RECT_B1	255

#define _ROI_RECT_R2	179
#define _ROI_RECT_G2	179
#define _ROI_RECT_B2	255
//--------------------------------------------------------------------
enum CAMERA_SEL_IDX		{ eTOP_CAM_20X = 0, eTOP_CAM_2X, eBTM_CAM_20X, eBTM_CAM_2X };
enum CAMERA_IDX			{ eTOP_CAM = 0, eBTM_CAM};
enum MARK_IDX			{ eALIGN_1ST_MARK = 0, eALIGN_2ND_MARK};
enum MODELGROUP_IDX		{ ePLATE_1 = 0, ePLATE_2,  eROLL, ePHOTOMASK, eSYNC, ePLATE_3};
enum MARKUSED_IDX		{ eALIGN_MARK = 0, eMASK_MARK};
enum LENS_IDX			{ e2X_LENS = 0, e20X_LENS};

#define _MAX_FIND_MODEL		26+2

enum FIND_MODEL_IDX 
{
	mdTOP_20X_STAEG1_START = 0, 	mdTOP_20X_STAEG1_END, 
	mdTOP_2X_STAEG1_START = 2,		mdTOP_2X_STAEG1_END,
	mdTOP_20X_STAEG2_START = 4,	 	mdTOP_20X_STAEG2_END, 
	mdTOP_2X_STAEG2_START = 6,		mdTOP_2X_STAEG2_END,
	mdBTM_20X_ROLL_START = 8,		mdBTM_20X_ROLL_END, 
	mdBTM_2X_ROLL_START = 10,		mdBTM_2X_ROLL_END,
	mdTOP_20X_MASK = 12,			mdTOP_2X_MASK, 
	mdBTM_20X_MASK = 14,			mdBTM_2X_MASK,
	mdTOP_20X_DEF_START = 16, 		mdTOP_20X_DEF_END, 
	mdTOP_2X_DEF_START = 18,		mdTOP_2X_DEF_END,
	mdTMP_MASK = 20,
	mdTOP_20X_STAEG3_START = 21,	mdTOP_20X_STAEG3_END, 
	mdTOP_2X_STAEG3_START = 23,		mdTOP_2X_STAEG3_END,
	mdTOP_COMP_START = 25,			mdBTM_COMP_START
};

//===================================================================================
//	Image Size包访 郴侩  
//--------------------------------------------------------------------

#define _INSP_IMAGE_SIZE_X		1352
#define _INSP_IMAGE_SIZE_Y		1030
#define _INSP_CAM_SIZE_X		1352
#define _INSP_CAM_SIZE_Y		1030

#define _WHITE_REF_				0 
#define _BLACK_REF_				1
//===================================================================================
#define _MAX_VIEW_				6
#define _MAX_CAM_				6
#define _MAX_TMPVIEW_			6
//===================================================================================
#define _INSP_CAM_GRAB_CONTINUE		0
#define _INSP_CAM_GRAB				1

#define _ZOOM_NORMAL			-1.0
#define _ZOOM_HALF				-2.0
#define _ZOOM_QUARTZ			-4.0

#define _DISP_ZOOM_FACTOR		-2.0
#define _DISP_ROI_FACTOR		2.0
#endif
