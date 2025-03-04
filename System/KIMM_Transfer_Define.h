#ifndef _KIMM_TRANSFER_VER1_SYSTEM_INFO_H
#define _KIMM_TRANSFER_VER1_SYSTEM_INFO_H

//////////////////////////////////////////////////////////////////////
// End-Userb Define
// +-8mm -> +-2.6µµ
//////////////////////////////////////////////////////////////////////

	#define _MAX_LOADCELL_NO	50000
	#define _MAX_SYNC_NO	20000

	#define _Max_LoadCell_Kg	100.0
	#define _MIT_MAX_Volt		10.0

	#define  _AD_LOADCELL_F			2
	#define  _AD_LOADCELL_R			3

	#define _LIN_ENC1		0
	#define _LIN_ENC2		1
	#define _IL_CONT12		2
	#define _IL_CONT34		3

//A/D ScaleFactor => 0.000244140625 1/power(2,12)
//-----------------------------------------------------------------------------------
//	I/O Define
//----------------------------------------------------------------------------------- 
	const int DEF_IOMONITOR_MAX_INPUT_PAGE		= 4;
	const int DEF_IOMONITOR_MAX_OUTPUT_PAGE		= 4;

	const int DEF_IOMONITOR_MAX_IO_PER_PAGE		= 16;
	const int DEF_IOMONITOR_MAX_TITLE_PER_PAGE	= 2;

	const int DEF_IOMONITOR_MAX_IO_INPUT		= 48;
	const int DEF_IOMONITOR_MAX_IO_OUTPUT		= 48;

	#define _INOUT_MAX_NO						52
	#define _ACS_INOUT_START_NO					0

	#define _ACS_INOUT_MC4U_IO_PORT_ADDR	0
	#define _ACS_INOUT_MC4U_START_NO		0
	#define _ACS_INOUT_MC4U_END_NO			7

	#define _ACS_INOUT_UDilt1_IO_PORT_ADDR	2
	#define _ACS_INOUT_UDilt1_START_NO		8
	#define _ACS_INOUT_UDilt1_END_NO		11

	#define _ACS_INOUT_UDilt2_IO_PORT_ADDR	3
	#define _ACS_INOUT_UDilt2_START_NO		12
	#define _ACS_INOUT_UDilt2_END_NO		15

	#define _ACS_INOUT_DIO1_IO_PORT_ADDR	0
	#define _ACS_INOUT_DIO1_START_NO		16
	#define _ACS_INOUT_DIO1_END_NO			31

	#define _ACS_INOUT_DIO2_IO_PORT_ADDR	1
	#define _ACS_INOUT_DIO2_START_NO		32
	#define _ACS_INOUT_DIO2_END_NO			47
	//----------------------------------------------------------------------------------- 
	#define _INPUT_Upper_Vision_Y_Org			0
	#define _INPUT_Lower_Vision_X_Org			1
	#define _INPUT_Lower_Vision_Y_Org			2
	#define _INPUT_Lower_Vision_Z_Org			3
	#define _INPUT_Lower_Vision_Y_LN
	#define _INPUT_Lower_Vision_Y_LP			5
	#define _INPUT_Lower_Vision_Z_LN			6
	#define _INPUT_Lower_Vision_Z_LP			7

	#define _INPUT_Upper_Vision_Z_Org	8
	#define _INPUT_9	9
	#define _INPUT_AlignStage_Y1_Org	10
	#define _INPUT_AlignStage_Y2_Org	11

	#define _INPUT_Impress_Z1_Org			12
	#define _INPUT_Impress_Z2_Org			13
	#define _INPUT_14			14
	#define _INPUT_15			15

	#define _INPUT_Air1_On			16
	#define _INPUT_Air2_On			17
	#define _INPUT_SW_Vac1			18
	#define _INPUT_SW_Vac2			19
	#define _INPUT_SW_Vac3			20
	#define _INPUT_SW_Vac4			21
	#define _INPUT_SW_Vac5			22
	#define _INPUT_SEN_Vac1			23
	#define _INPUT_SEN_Vac2			24
	#define _INPUT_SEN_Vac3			25
	#define _INPUT_SEN_Vac4			26
	#define _INPUT_SEN_Vac5			27
	#define _INPUT_Reserve028		28
	#define _INPUT_Err_LoadCell_1	29
	#define _INPUT_Err_LoadCell_2	30
	#define _INPUT_31				31
	#define _INPUT_SW_Foot			32
	#define _INPUT_33				33
	#define _INPUT_HeaterPower		34
	#define _INPUT_MotorPower		35
	#define _INPUT_Emg				36
	#define _INPUT_Reserve037		37
	#define _INPUT_Reserve038		38
	#define _INPUT_Reserve039		39
	#define _INPUT_Reserve040		40
	#define _INPUT_Reserve041		41
	#define _INPUT_Reserve042		42
	#define _INPUT_Reserve043		43
	#define _INPUT_Reserve044		44
	#define _INPUT_Reserve045		45
	#define _INPUT_Reserve046		46
	#define _INPUT_Reserve047		47
	//-----------------------------------------------------------------------------------
	#define _OUTPUT_MotorPower		0
	#define _OUTPUT_Reserve001					1
	#define _OUTPUT_Reserve002			2
	#define _OUTPUT_Reserve003			3
	#define _OUTPUT_Reserve004			4
	#define _OUTPUT_Reserve005			5
	#define _OUTPUT_Reserve006			6
	#define _OUTPUT_Reserve007			7

	#define _OUTPUT_Reserve008			8
	#define _OUTPUT_Reserve009			9
	#define _OUTPUT_Reserve010			10
	#define _OUTPUT_Reserve011			11

	#define _OUTPUT_Reserve012			12
	#define _OUTPUT_Reserve013			13
	#define _OUTPUT_Reserve014			14
	#define _OUTPUT_Reserve015			15

	#define _OUTPUT_Reserve016			16
	#define _OUTPUT_Reserve017			17
	#define _OUTPUT_SWLamp_Vac1			18
	#define _OUTPUT_SWLamp_Vac2			19
	#define _OUTPUT_SWLamp_Vac3			20
	#define _OUTPUT_SWLamp_Vac4			21
	#define _OUTPUT_SWLamp_Vac5			22
	#define _OUTPUT_SolOn_Vac1			23
	#define _OUTPUT_SolOn_Vac2			24
	#define _OUTPUT_SolOn_Vac3			25
	#define _OUTPUT_SolOn_Vac4			26
	#define _OUTPUT_SolOn_Vac5			27
	#define _OUTPUT_SolOff_Vac1			28
	#define _OUTPUT_SolOff_Vac2			29
	#define _OUTPUT_SolOff_Vac3			30
	#define _OUTPUT_SolOff_Vac4			31
	#define _OUTPUT_SolOff_Vac5			32
	#define _OUTPUT_Door				33
	#define _OUTPUT_TowerRamp_R			34
	#define _OUTPUT_TowerRamp_G			35
	#define _OUTPUT_TowerRamp_Y			36
	#define _OUTPUT_Buzzer1				37
	#define _OUTPUT_Buzzer2				38
	#define _OUTPUT_Buzzer3				39
	#define _OUTPUT_SubRamp1			40
	#define _OUTPUT_SubRamp2			41
	#define _OUTPUT_LoadCell_Reset		42
	#define _OUTPUT_Reserve043			43
	#define _OUTPUT_Reserve044			44
	#define _OUTPUT_Reserve045			45
	#define _OUTPUT_Reserve046			46
	#define _OUTPUT_Reserve047			47

	//-----------------------------------------------------------------------------------

	#define _ROLL_MAX_STEP_			320400.0 //1310720.0 //2621440.0 => res : 2086.0761041, 4172.1534
	#define _PAT_ROLL_MAX_STEP_		314150.0 //816814.0
	
	///-----------------------------------------------------------------------------------
	// Roll Winder
	#define _ENC_ROLL_DIAM_			48.0
	#define _ENC_PULSE_				20000.0
	//------------------------------------------------------------------------------------

	#define _MAX_AXIS				16
	#define _ACS_MAX_AXIS			16

	#define _X_Z_CHECK_POS				0.0 //300.0

	#define _ACS_AXIS_STAGE_X			0
	#define _ACS_AXIS_ENC_1	 			1
	#define _ACS_AXIS_IMP_ROLL 			2
	#define _ACS_AXIS_3					3

	#define _ACS_AXIS_UPPER_CAM_Y		4
	#define _ACS_AXIS_LOWER_CAM_X		5
	#define _ACS_AXIS_LOWER_CAM_Y		6
	#define _ACS_AXIS_LOWER_CAM_Z		7
	
	#define _ACS_AXIS_UPPER_CAM_Z		8
	#define _ACS_AXIS_9					9
	#define _ACS_AXIS_ALIGN_Y1			10
	#define _ACS_AXIS_ALIGN_Y2			11
	
	#define _ACS_AXIS_IMP_ROLL_Z1		12
	#define _ACS_AXIS_IMP_ROLL_Z2		13
	#define _ACS_AXIS_14				14
	#define _ACS_AXIS_15				15

//////////////////////////////////////////////////////////////////////
	#define _L_LEN_POS					0
	#define _R_LEN_POS					1

// 	//-------------------------------------------------
	#define _STAGE_Z_CONTROL_BUFFER_NO	25
	#define _STAGE_DATA_BUFFER_NO		26
	#define _STAGE_CAL_BUFFER_NO		27
	#define _STAGE_SYNC_BUFFER_NO		28
	#define _STAGE_SYNC_STOP_BUFFER_NO	29
	#define _STAGE_SYNCMODE_SET_BUFFER_NO		23
	#define _STAGE_SYNCMODE_RESET_BUFFER_NO		29
// 	#define _STAGE_RX_ORG_CHECK_NO		1100
// 
// 	#define _DOC_LX_ORG_PLC_NO			12
// 	#define _DOC_LX_ORG_CHECK_NO		1200
// 	//-------------------------------------------------

//-------------------------------------------------
	
#endif
