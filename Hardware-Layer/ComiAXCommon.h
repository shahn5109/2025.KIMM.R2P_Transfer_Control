#if !defined(COMIGRAPHDEFINED_H_INCLUDED_)
#define COMIGRAPHDEFINED_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Frame Style ID //
typedef enum {cmFS_STANDARD, cmFS_SUPPLIED, cmFS_USER} TCmFrameStyle;

// Standard Frame ID //
typedef enum {cmSF_NONE, cmSF_FLAT, cmSF_RAISED, cmSF_SUNKEN} TCmStandrdFrameID;

// Gradient Style ID //
typedef enum {cmGS_InBright, cmGS_OutBright, cmGS_BothBright, cmGS_CentBright} TCmGradStyle;

// Line Style ID //
typedef enum {cmLS_Solid, cmLS_Dash, cmLS_Dot, cmLS_DashDot, 
	cmLS_DashDotDot, cmLS_None} TCmLineStyle;

// Plot Style ID //
typedef enum {cmPS_Line, cmPS_Step, cmPS_VBar, cmPS_HBar, cmPS_FillBase} TCmPlotStyle;

// Marker Style ID //
typedef enum {cmMS_Circle, cmMS_Square, cmMS_Diamond, cmMS_Triangle, cmMS_SymbolX,
cmMS_SymbolCross, cmMS_Asterisk, cmMS_None} TCmMarkerStyle;

// Cursor Attribute ID //
typedef enum {cmCA_VISIBLE, cmCA_STYLE, cmCA_LINE_COLOR, cmCA_LINE_WIDTH, cmCA_XAXIS,
	cmCA_YAXIS, cmCA_VALUE_VISIBLE, cmCA_DELTAX_VISIBLE, cmCA_DELTAY_VISIBLE,
	cmCA_NUM_DIGITS, cmCA_TEXT_COLOR, cmCA_TRANSPARENT, cmCA_TEXT_BACK_COLOR
} TCmCursorAttribute;

// Cursor Style ID //
typedef enum {cmCURSOR_XY_LONG, cmCURSOR_XY_SHORT, cmCURSOR_X_ONLY,
cmCURSOR_Y_ONLY} TCmCursorStyle;

// Variable Type ID //
typedef enum {cmVT_CHAR, cmVT_UCHAR, cmVT_SHORT, cmVT_USHORT, cmVT_INT, cmVT_UINT,
cmVT_LONG, cmVT_ULONG, cmVT_FLOAT, cmVT_DOUBLE} TCmVarType;

#endif