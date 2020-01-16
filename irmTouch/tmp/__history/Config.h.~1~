//---------------------------------------------------------------------------

#ifndef ConfigH
#define ConfigH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <inifiles.hpp>

#include "firmwareupdate.h"
#include "rom.h"
#include <Grids.hpp>
#include <stdio.h>
#include "time.h"
#include <Dialogs.hpp>
#include <stdlib.h>
//---------------------------------------------------------------------------
//#define XAXIS_MODE	0

#define MAX_AXIS	31
//#define MAX_POINTS	64
#define MAX_BOARDS	60
#define MAX_EMIT_LEDS	512
#define MAX_REV_LEDS	(MAX_BOARDS*8)

#define SECRET_INITIAL_ON		1

#define HALF_SCAN	0
#define CALIB_DEBUG	0

#define FEATURE_STD				(1<<0)
#define FEATURE_SMOOTH_ON		(1<<1)
#define FEATURE_INTERPLATION_ON	(1<<2)
#define FEATURE_MAC				(1<<3)
#define FEATURE_REVERSE_X		(1<<4)
#define FEATURE_REVERSE_Y		(1<<5)
#define FEATURE_EXCHANGE_XY		(1<<6)
#define FEATURE_USART_ON		(1<<7)
#define FEATURE_SMALL_POINT_ON	(1<<8)

#define USB_MAX_STR_DESC_SIZ	64 

typedef uint32_t(*Get_SPI_Data_Func)(uint16_t Addr, uint8_t Led, uint16_t Pow);

const uint32_t RecordSize = 256;
extern int ANTI_AXIS;

#pragma   pack(push,1)
typedef struct PCB_CONFIG_ {
	uint8_t Flag;
	uint8_t Gain;
	uint8_t EmitLeds;
	uint8_t RevLeds;
	uint8_t RevPos[8];
}PCB_CONFIG;

typedef struct AXI_PARAM_ {
	int TotalLeds;
	int JumpLeds;
	int ScanGap;
	int BoradLeds[MAX_BOARDS];
	int Boards;
	float Dis;
	float Margin;
	float MarginRightDown;
	float Len;
	int Axis;
	int MiniOff;		//偏移整灯数
	float RequestDegOff[MAX_AXIS];
	float ActualDegOff[MAX_AXIS];
	int LedOff[MAX_AXIS];
	float Tan[MAX_AXIS];
	float LenOff[MAX_AXIS];
	short Gain[MAX_AXIS];
	uint16_t EmitPower;

	float AuxReqDegOff[MAX_AXIS];
	float AuxActuDegOff[MAX_AXIS];
	int AuxLedOff[MAX_AXIS];
	float AuxTan[MAX_AXIS];
	float AuxLenOff[MAX_AXIS];

	int MixAxis;
	int MixLedOff[MAX_AXIS * 2];
	float MixTan[MAX_AXIS * 2];
	float MixCos[MAX_AXIS * 2];
	uint16_t MixAxisId[MAX_AXIS * 2];
	uint16_t MixStartLed[MAX_AXIS * 2];
	uint16_t MixEndLed[MAX_AXIS * 2];


	Get_SPI_Data_Func Get_Emit_Data;
	Get_SPI_Data_Func Get_Rev_Data;
	//Get_SPI_Data_Func Get_Rev_Data;

	int16_t MaxOff;

	bool SkipFirstAxi;
	bool DualSpiData;
	bool ShutdownEmit;
	bool SPI8Bits;
	bool GPIOGain;
	bool HalfScan;
	bool ReverseXScan;
	bool ReverseYScan;
	bool SwitchEmit;
	bool GPIOSwitch;
	bool NotD7;
	bool WaitRxPowerOn;
	bool CutUnused;
}AXI_PARAM;

typedef struct SCAN_TABLE_HEAD_ {
	uint8_t Flags;

	uint8_t XAxis;
	uint8_t YAxis;
	uint8_t AllAxis;

	uint16_t XLeds;
	uint16_t YLeds;

	uint8_t XBoards;
	uint8_t YBoards;

	const int16_t *XOffsets;
	const int16_t *YOffsets;
	const int16_t *Offsets;

	const float *XTan;
	const float *YTan;
	const float *Tan;

	const float *XCoTan;
	const float *YCoTan;
	const float *CoTan;

	const float *XOffLen;
	const float *YOffLen;
	const float *OffLen;

	const uint16_t *RevTab;
	const uint16_t *EmitTab;
	uint16_t RevTabLen;
	uint16_t EmitTabLen;

	const int16_t *AuxXOffsets;
	const int16_t *AuxYOffsets;
	const int16_t *AuxOffsets;

	const float *AuxXTan;
	const float *AuxYTan;
	const float *AuxTan;

	const float *AuxXCoTan;
	const float *AuxYCoTan;
	const float *AuxCoTan;

	const float *AuxXOffLen;
	const float *AuxYOffLen;
	const float *AuxOffLen;

	const uint32_t *GainTab;
	uint16_t GainTabLen;

	float MarginLeft;
	float MarginRight;
	float MarginUp;
	float MarginDown;

	uint8_t XEmitVer;
	uint8_t XRevVer;
	uint8_t XBoardLed;
	uint8_t XPowerGroup;
	uint8_t XJumpLeds;
	uint8_t XPow;
	uint16_t XMaxDeg;
	uint16_t XLedSpace;
	uint8_t XCuts[12];
	uint8_t XReScan;
	const uint8_t *XGainSet;

	uint8_t YEmitVer;
	uint8_t YRevVer;
	uint8_t YBoardLed;
	uint8_t YPowerGroup;
	uint8_t YJumpLeds;
	uint8_t YPow;
	uint16_t YMaxDeg;
	uint16_t YLedSpace;
	uint8_t YCuts[12];
	uint8_t YReScan;
	const uint8_t *YGainSet;

	uint32_t Feature;

	uint16_t xJumps;
	uint16_t yJumps;
	const uint16_t *JumpLedsTable;
	uint16_t JumpLedsTableLen;

	const uint16_t *ReorderAdMap;
	uint16_t ReorderAdMapLen;

	//Mix Scan Param
	uint32_t Maigc1;

	uint8_t MixXAxis;
	uint8_t MixYAxis;
	uint8_t MixAllAxis;

	const int16_t *MixXLedOff;
	const int16_t *MixYLedOff;
	const int16_t *MixLedOff;

	const float *MixXTan;
	const float *MixYTan;
	const float *MixTan;

	const float *MixXCos;
	const float *MixYCos;
	const float *MixCos;

	const uint8_t *MixXAxisId;
	const uint8_t *MixYAxisId;
	const uint8_t *MixAxisId;

	const uint16_t *MixXStartLed;
	const uint16_t *MixYStartLed;
	const uint16_t *MixStartLed;

	const uint16_t *MixXEndLed;
	const uint16_t *MixYEndLed;
	const uint16_t *MixEndLed;

	uint8_t GlareLever;
	uint16_t HIDToolsVer;

	const uint16_t *V15EmitGroupTable;
	uint16_t V15EmitGroupTableLen;

	const uint32_t *GPIOA;    	//for gpio scaning
	uint32_t GPIOALen;
	const uint32_t *GPIOB;
	uint32_t GPIOBLen;
	const uint32_t *GPIOC;
	uint32_t GPIOCLen;
	const uint32_t *GPIOD;
	uint32_t GPIODLen;

	uint16_t XScanGap;
	uint16_t YScanGap;

	//soft scaning
	int16_t MaxXOff;
	int16_t MaxYOff;
	const float *ScXTan;		//from 0 to MaxOff
	const float *ScXCoTan;
	const float *ScXSin;
	const float *ScXCos;
	const float *ScYTan;
	const float *ScYCoTan;
	const float *ScYSin;
	const float *ScYCos;

	uint8_t XDivMethod;
	uint8_t YDivMethod;
	uint8_t XCutUnUsed;
	uint8_t YCutUnUsed;
	uint8_t BoardLed[64];
}SCAN_TABLE_HEAD;

typedef struct SCAN_TABLE_HEAD_TRI_ {
	uint32_t Flags;

	uint8_t XBoards;
	uint8_t YBoards;

	uint16_t XEmitLeds;
	uint16_t YEmitLeds;
	uint16_t EmitLeds;
	uint16_t XRevLeds;
	uint16_t YRevLeds;
	uint16_t RevLeds;

	uint16_t Signals;
	uint32_t FrameTime;

	PCB_CONFIG PCBs[31];

	const uint16_t *RevPos;
	const uint16_t *EmitStart;
	const uint16_t *EmitEnd;

	const float *XTan;
	const float *YTan;

	const float *XCoTan;
	const float *YCoTan;

	const uint16_t *TimerArrTable;
	const uint32_t *GpioSwitchTable;
	const uint32_t *GainTimerArrTable;
	uint16_t TimerArrTableLen;
	uint16_t GpioSwitchTableLen;
	uint16_t GainTimerArrTableLen;

	float MarginLeft;
	float MarginRight;
	float MarginUp;
	float MarginDown;

	float XLength;
	float YLength;

	uint16_t XDegOff;
	uint16_t XLedOff;
	uint16_t XLedSpace;

	uint16_t YDegOff;
	uint16_t YLedOff;
	uint16_t YLedSpace;

	uint16_t TOn;
	uint16_t TOff;
	uint16_t TWait;

	uint32_t Feature;

	const uint16_t *ReorderAdMap;
	uint16_t ReorderAdMapLen;
}SCAN_TABLE_HEAD_TRI;

typedef struct RevRange {
	uint16_t RevNo;
	uint16_t EmitStart;
	uint16_t EmitEnd;
}RevRange_t;

typedef struct PARAM_ {
	uint8_t Frames;
	uint8_t LowFrames;
	uint8_t TOn;
	uint8_t TOff;
	uint8_t AdTrig;
	uint8_t Gain1[15];
	uint8_t Gain2[15];
	uint8_t EmitPow[15];
	uint8_t TimeOut;
	uint32_t Feature;
	uint8_t TRise;
	uint8_t Click;
	uint8_t tracepoints;
	uint8_t gesturestart;
	uint8_t gestureend;
	uint8_t Gain1_60[15];
	uint8_t Gain2_60[15];
	uint8_t EmitPow_60[15];
    uint16_t worktime;
}RunTimeParam_t;

typedef struct CALIBRATION_POINT {
	int x, y;      //校准前的实际区域中心坐标
	float k[6];  //校准系数
}Calibration_Points_t;

typedef struct CALIBRATION_ {
	uint32_t Flag;
	uint8_t xPoints;
	uint8_t yPoints;
	Calibration_Points_t Calib[5 * 5];
}Calibration_t;
#pragma   pack(pop)

#define PI 3.1415926536

extern AXI_PARAM	XParam, YParam;

class TFormConfig : public TForm
{
__published:	// IDE-managed Components
	TPageControl * PageControl1;
	TTabSheet *TabSheetScan;
	TStatusBar *StatusBar1;
	TLabel *Label31;
	TButton *ButtonDownTab;
	TGroupBox *GroupBox2;
	TLabel *Label3;
	TLabel *Label5;
	TLabel *Label10;
	TLabel *Label20;
	TLabel *Label21;
	TLabel *Label22;
	TLabel *Label23;
	TLabel *Label24;
	TLabel *Label28;
	TLabel *Label37;
	TLabel *Label39;
	TEdit *EditYLeds;
	TEdit *EditYDist;
	TEdit *EditYMargin;
	TComboBox *ComboBoxYAxis;
	TEdit *EditYoffset;
	TButton *Button3;
	TComboBox *ComboBoxYBoards;
	TEdit *EditYtail3;
	TEdit *EditYtail2;
	TEdit *EditYtail1;
	TComboBox *ComboBoxYPower;
	TButton *Button4;
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TLabel *Label4;
	TLabel *Label6;
	TLabel *Label15;
	TLabel *Label16;
	TLabel *Label17;
	TLabel *Label18;
	TLabel *Label19;
	TLabel *Label25;
	TLabel *Label36;
	TLabel *Label38;
	TEdit *EditXLeds;
	TEdit *EditXoffset;
	TButton *Button2;
	TComboBox *ComboBoxXAxis;
	TEdit *EditXDist;
	TEdit *EditXMargin;
	TComboBox *ComboBoxXBoards;
	TEdit *EditXtail3;
	TEdit *EditXtail2;
	TEdit *EditXtail1;
	TComboBox *ComboBoxXPower;
	TTabSheet *TabSheetParam;
	TLabel *Label41;
	TEdit *EditFrame;
	TLabel *Label42;
	TEdit *EditTOn;
	TLabel *Label43;
	TEdit *EditTOff;
	TLabel *Label44;
	TEdit *EditADTrig;
	TLabel *Label46;
	TEdit *EditLowFrame;
	TTabSheet *TabSheet3;
	TTabSheet *TabSheet4;
	TLabel *LabelBoot;
	TLabel *LabelHWVer;
	TLabel *LabelFirmwareVer;
	TLabel *LabelFwSta;
	TLabel *LabelAppRunSta;
	TLabel *LabelResetReson;
	TLabel *LabelFlashSize;
	TLabel *Label54;
	TLabel *Label55;
	TButton *ButtonGetSta;
	TTimer *TimerDownTab;
	TLabel *Label56;
	TLabel *Label57;
	TLabel *Label58;
	TLabel *Label59;
	TLabel *Label60;
	TStringGrid *StringGrid1;
	TButton *ButtonDownParam;
	TButton *ButtonParamRead;
	TButton *ButtonSwitch;
	TComboBox *ComboBoxXBoardLeds;
	TComboBox *ComboBoxYBoardLeds;
	TLabel *LabelRstTimes;
	TLabel *LabelLastRunTime;
	TLabel *LabelRunTime;
	TLabel *Label50;
	TEdit *EditLowFrameTimeout;
	TLabel *Label51;
	TButton *Button9;
	TGroupBox *GroupBox3;
	TGroupBox *GroupBox4;
	TEdit *EditHardwareVer;
	TLabel *Label62;
	TLabel *Label63;
	TEdit *EditSoftwareVer;
	TButton *ButtonFlashProtect;
	TEdit *EditDate;
	TLabel *Label61;
	TLabel *Label64;
	TButton *ButtonGetSta2;
	TEdit *EditTime;
	TLabel *Label65;
	TShape *Shape1;
	TLabel *Label67;
	TEdit *EditCode;
	TEdit *EditType;
	TLabel *Label52;
	TEdit *EditSerialNum;
	TLabel *Label66;
	TButton *Button10;
	TLabel *LabelOS;
	TLabel *Label49;
	TLabel *Label68;
	TShape *ShapeRO;
	TShape *ShapeWO;
	TLabel *Label47;
	TShape *Shape2;
	TLabel *Label48;
	TEdit *Edit75;
	TLabel *Label53;
	TEdit *Edit76;
	TComboBox *ComboBoxEmitXVer;
	TComboBox *ComboBoxRevXVer;
	TComboBox *ComboBoxEmitYVer;
	TComboBox *ComboBoxRevYVer;
	TLabel *Label72;
	TEdit *EditCPUID;
	TLabel *Label71;
	TEdit *EditCustomer;
	TButton *ButtonWriteSerialNum;
	TButton *ButtonSecretInitial;
	TButton *ButtonReadSerialNum;
	TButton *Button13;
	TButton *ButtonGetCPUID;
	TButton *Button15;
	TLabel *Label73;
	TLabel *Label74;
	TComboBox *ComboBoxStdParam;
	TButton *Button1;
	TButton *Button6;
	TEdit *EditBootVer;
	TLabel *Label75;
	TButton *Button7;
	TButton *Button11;
	TGroupBox *GroupBox5;
	TCheckBox *CheckBoxFeatureMac;
	TLabel *Label45;
	TEdit *EditTRise;
	TLabel *Label78;
	TGroupBox *GroupBox6;
	TComboBox *ComboBoxStdScreen;
	TButton *Button5;
	TButton *Button8;
	TGroupBox *GroupBox7;
	TCheckBox *CheckBoxSkipFirstAxis;
	TCheckBox *CheckBoxSwitchEmit;
	TCheckBox *CheckBoxDualSPIData;
	TCheckBox *CheckBoxShutDownEmit;
	TCheckBox *CheckBox8BitSPI;
	TCheckBox *CheckBoxGPIOGain;
	TCheckBox *CheckBoxReverseXScan;
	TCheckBox *CheckBoxReverseYScan;
	TCheckBox *CheckBoxFeatureReverseX;
	TCheckBox *CheckBoxFeatureReverseY;
	TCheckBox *CheckBoxFeatureExchangeXY;
	TCheckBox *CheckBoxFeatureUSARTOn;
	TGroupBox *GroupBox8;
	TButton *Button12;
	TEdit *EditXMiniOff;
	TLabel *Label40;
	TEdit *EditYMiniOff;
	TLabel *Label79;
	TButton *Button14;
	TEdit *EditXJump;
	TLabel *Label80;
	TLabel *Label81;
	TEdit *EditYJump;
	TLabel *Label26;
	TLabel *Label82;
	TLabel *Label83;
	TLabel *Label27;
	TEdit *EditXtail4;
	TLabel *Label85;
	TLabel *Label86;
	TEdit *EditXReScan;
	TLabel *Label87;
	TEdit *EditYtail4;
	TLabel *Label29;
	TLabel *Label30;
	TLabel *Label88;
	TLabel *Label89;
	TEdit *EditYReScan;
	TLabel *Label91;
	TLabel *Label92;
	TLabel *Label93;
	TComboBox *ComboBoxXCut1;
	TComboBox *ComboBoxXCut2;
	TComboBox *ComboBoxXCut3;
	TComboBox *ComboBoxXCut4;
	TComboBox *ComboBoxYCut1;
	TComboBox *ComboBoxYCut2;
	TComboBox *ComboBoxYCut3;
	TComboBox *ComboBoxYCut4;
	TCheckBox *CheckBoxSkipX;
	TCheckBox *CheckBoxFeatureSmallPointOn;
	TEdit *EditXMarginRight;
	TLabel *Label94;
	TLabel *Label95;
	TEdit *EditYMarginDown;
	TLabel *Label96;
	TLabel *Label97;
	TCheckBox *CheckBoxDoubleFirst;
	TButton *ButtonGetScanParam;
	TTabSheet *TabSheet1;
	TTrackBar *TrackBarClick;
	TTrackBar *TrackBarDoubleClick;
	TLabel *Label102;
	TLabel *Label103;
	TCheckBox *CheckBoxFeatureSmooth;
	TCheckBox *CheckBoxFeatureInterplation;
	TGroupBox *GroupBox10;
	TLabel *Label98;
	TLabel *Label99;
	TLabel *Label100;
	TLabel *Label101;
	TEdit *Edit91;
	TTrackBar *TrackBar1;
	TTrackBar *TrackBar2;
	TGroupBox *GroupBox9;
	TCheckBox *CheckBox1;
	TCheckBox *CheckBox2;
	TCheckBox *CheckBox3;
	TCheckBox *CheckBox4;
	TCheckBox *CheckBox5;
	TCheckBox *CheckBox6;
	TButton *Button16;
	TButton *Button17;
	TButton *Button20;
	TGroupBox *GroupBox11;
	TButton *Button18;
	TButton *Button19;
	TLabel *Label104;
	TComboBox *ComboBoxClibXPoints;
	TComboBox *ComboBoxClibYPoints;
	TLabel *Label105;
	TCheckBox *CheckBoxCalibAuxPoint;
	TLabel *Label106;
	TLabel *Label107;
	TEdit *EditxPxiMargin;
	TEdit *EdityPxiMargin;
	TButton *Button21;
	TButton *Button22;
	TCheckBox *CheckBoxAntiGlare;
	TCheckBox *CheckBoxCPLDMagicConfig;
	TButton *Button23;
	TButton *Button24;
	TButton *Button25;
	TOpenDialog *OpenDialogConfig;
	TSaveDialog *SaveDialogConfig;
	TButton *Button26;
	TCheckBox *CheckBox7;
	TCheckBox *CheckBoxFeatureFourceMouse;
	TButton *Button27;
	TButton *ButtonForceWROff;
	TEdit *Edit92;
	TEdit *Edit93;
	TEdit *Edit94;
	TLabel *Label108;
	TLabel *Label109;
	TLabel *Label110;
	TButton *Button28;
	TCheckBox *CheckBoxSwEmitRev;
	TCheckBox *CheckBox8;
	TCheckBox *CheckBoxFeatureBoardPoint;
	TCheckBox *CheckBoxFeatureRev;
	TButton *Button29;
	TCheckBox *CheckBoxFeatureGainAdj;
	TCheckBox *CheckBoxFeatureEdgeAdsorption;
	TButton *Button30;
	TCheckBox *CheckBoxFeatureGPIOGesture;
	TCheckBox *CheckBoxFeatureDisableRightClick;
	TCheckBox *CheckBoxGPIOSwitch;
	TCheckBox *CheckBoxSkipLastY;
	TPageControl *PageControl2;
	TTabSheet *TabSheet2;
	TLabel *Label2;
	TEdit *Edit2;
	TEdit *Edit3;
	TEdit *Edit4;
	TEdit *Edit5;
	TEdit *Edit6;
	TEdit *Edit7;
	TEdit *Edit8;
	TEdit *Edit9;
	TEdit *Edit10;
	TEdit *Edit11;
	TEdit *Edit12;
	TEdit *Edit13;
	TEdit *Edit14;
	TEdit *Edit15;
	TEdit *Edit16;
	TEdit *Edit17;
	TEdit *Edit18;
	TEdit *Edit19;
	TEdit *Edit20;
	TEdit *Edit21;
	TEdit *Edit22;
	TEdit *Edit23;
	TEdit *Edit24;
	TEdit *Edit25;
	TEdit *Edit26;
	TEdit *Edit27;
	TEdit *Edit28;
	TEdit *Edit29;
	TEdit *Edit30;
	TEdit *Edit31;
	TEdit *Edit32;
	TEdit *Edit33;
	TEdit *Edit34;
	TComboBox *ComboBox1;
	TComboBox *ComboBox2;
	TComboBox *ComboBox3;
	TComboBox *ComboBox4;
	TComboBox *ComboBox5;
	TComboBox *ComboBox6;
	TComboBox *ComboBox7;
	TComboBox *ComboBox8;
	TComboBox *ComboBox9;
	TComboBox *ComboBox10;
	TComboBox *ComboBox11;
	TComboBox *ComboBox12;
	TComboBox *ComboBox13;
	TComboBox *ComboBox14;
	TComboBox *ComboBox15;
	TComboBox *ComboBox16;
	TComboBox *ComboBox17;
	TComboBox *ComboBox18;
	TComboBox *ComboBox19;
	TComboBox *ComboBox20;
	TComboBox *ComboBox21;
	TComboBox *ComboBox22;
	TEdit *Edit1;
	TEdit *Edit35;
	TEdit *Edit69;
	TEdit *Edit70;
	TEdit *Edit71;
	TEdit *Edit72;
	TEdit *Edit73;
	TEdit *Edit74;
	TEdit *Edit77;
	TEdit *Edit78;
	TEdit *Edit79;
	TLabel *Label7;
	TLabel *Label9;
	TLabel *Label8;
	TLabel *Label76;
	TLabel *Label32;
	TLabel *Label33;
	TTabSheet *TabSheet5;
	TLabel *Label111;
	TEdit *Edit95;
	TEdit *Edit96;
	TEdit *Edit97;
	TEdit *Edit98;
	TEdit *Edit99;
	TEdit *Edit100;
	TEdit *Edit101;
	TEdit *Edit102;
	TEdit *Edit103;
	TEdit *Edit104;
	TEdit *Edit106;
	TEdit *Edit107;
	TEdit *Edit108;
	TEdit *Edit109;
	TEdit *Edit110;
	TEdit *Edit111;
	TEdit *Edit112;
	TEdit *Edit113;
	TEdit *Edit114;
	TEdit *Edit115;
	TEdit *Edit117;
	TEdit *Edit118;
	TEdit *Edit119;
	TEdit *Edit120;
	TEdit *Edit121;
	TEdit *Edit122;
	TEdit *Edit123;
	TEdit *Edit124;
	TEdit *Edit125;
	TEdit *Edit126;
	TComboBox *ComboBox45;
	TComboBox *ComboBox46;
	TComboBox *ComboBox47;
	TComboBox *ComboBox48;
	TComboBox *ComboBox49;
	TComboBox *ComboBox50;
	TComboBox *ComboBox51;
	TComboBox *ComboBox52;
	TComboBox *ComboBox53;
	TComboBox *ComboBox54;
	TComboBox *ComboBox56;
	TComboBox *ComboBox57;
	TComboBox *ComboBox58;
	TComboBox *ComboBox59;
	TComboBox *ComboBox60;
	TComboBox *ComboBox61;
	TComboBox *ComboBox62;
	TComboBox *ComboBox63;
	TComboBox *ComboBox64;
	TComboBox *ComboBox65;
	TEdit *Edit128;
	TEdit *Edit129;
	TEdit *Edit130;
	TEdit *Edit131;
	TEdit *Edit132;
	TEdit *Edit133;
	TEdit *Edit134;
	TEdit *Edit135;
	TEdit *Edit136;
	TEdit *Edit137;
	TLabel *Label112;
	TLabel *Label113;
	TLabel *Label114;
	TLabel *Label115;
	TLabel *Label116;
	TLabel *Label117;
	TTabSheet *TabSheet6;
	TLabel *Label118;
	TEdit *Edit105;
	TEdit *Edit116;
	TEdit *Edit127;
	TEdit *Edit138;
	TEdit *Edit139;
	TEdit *Edit140;
	TEdit *Edit141;
	TEdit *Edit142;
	TEdit *Edit143;
	TEdit *Edit144;
	TEdit *Edit145;
	TEdit *Edit146;
	TEdit *Edit147;
	TEdit *Edit148;
	TEdit *Edit149;
	TEdit *Edit150;
	TEdit *Edit151;
	TEdit *Edit152;
	TEdit *Edit153;
	TEdit *Edit154;
	TEdit *Edit155;
	TEdit *Edit156;
	TEdit *Edit157;
	TEdit *Edit158;
	TEdit *Edit159;
	TEdit *Edit160;
	TEdit *Edit161;
	TEdit *Edit162;
	TEdit *Edit163;
	TEdit *Edit164;
	TComboBox *ComboBox55;
	TComboBox *ComboBox66;
	TComboBox *ComboBox67;
	TComboBox *ComboBox68;
	TComboBox *ComboBox69;
	TComboBox *ComboBox70;
	TComboBox *ComboBox71;
	TComboBox *ComboBox72;
	TComboBox *ComboBox73;
	TComboBox *ComboBox74;
	TComboBox *ComboBox75;
	TComboBox *ComboBox76;
	TComboBox *ComboBox77;
	TComboBox *ComboBox78;
	TComboBox *ComboBox79;
	TComboBox *ComboBox80;
	TComboBox *ComboBox81;
	TComboBox *ComboBox82;
	TComboBox *ComboBox83;
	TComboBox *ComboBox84;
	TEdit *Edit165;
	TEdit *Edit166;
	TEdit *Edit167;
	TEdit *Edit168;
	TEdit *Edit169;
	TEdit *Edit170;
	TEdit *Edit171;
	TEdit *Edit172;
	TEdit *Edit173;
	TEdit *Edit174;
	TLabel *Label119;
	TLabel *Label120;
	TLabel *Label121;
	TLabel *Label122;
	TLabel *Label123;
	TLabel *Label124;
	TPageControl *PageControl3;
	TTabSheet *TabSheet7;
	TLabel *Label11;
	TLabel *Label12;
	TLabel *Label13;
	TLabel *Label14;
	TLabel *Label34;
	TLabel *Label35;
	TLabel *Label77;
	TEdit *Edit36;
	TEdit *Edit37;
	TEdit *Edit38;
	TEdit *Edit39;
	TEdit *Edit40;
	TEdit *Edit41;
	TEdit *Edit42;
	TEdit *Edit43;
	TEdit *Edit44;
	TEdit *Edit45;
	TEdit *Edit46;
	TEdit *Edit47;
	TEdit *Edit48;
	TEdit *Edit49;
	TEdit *Edit50;
	TEdit *Edit51;
	TEdit *Edit52;
	TEdit *Edit53;
	TEdit *Edit54;
	TEdit *Edit55;
	TEdit *Edit56;
	TEdit *Edit57;
	TEdit *Edit58;
	TEdit *Edit59;
	TEdit *Edit60;
	TEdit *Edit61;
	TEdit *Edit62;
	TEdit *Edit63;
	TEdit *Edit64;
	TEdit *Edit65;
	TEdit *Edit66;
	TEdit *Edit67;
	TEdit *Edit68;
	TComboBox *ComboBox23;
	TComboBox *ComboBox24;
	TComboBox *ComboBox25;
	TComboBox *ComboBox26;
	TComboBox *ComboBox27;
	TComboBox *ComboBox28;
	TComboBox *ComboBox29;
	TComboBox *ComboBox30;
	TComboBox *ComboBox31;
	TComboBox *ComboBox32;
	TComboBox *ComboBox33;
	TComboBox *ComboBox34;
	TComboBox *ComboBox35;
	TComboBox *ComboBox36;
	TComboBox *ComboBox37;
	TComboBox *ComboBox38;
	TComboBox *ComboBox39;
	TComboBox *ComboBox40;
	TComboBox *ComboBox41;
	TComboBox *ComboBox42;
	TComboBox *ComboBox43;
	TComboBox *ComboBox44;
	TEdit *Edit80;
	TEdit *Edit81;
	TEdit *Edit82;
	TEdit *Edit83;
	TEdit *Edit84;
	TEdit *Edit85;
	TEdit *Edit86;
	TEdit *Edit87;
	TEdit *Edit88;
	TEdit *Edit89;
	TEdit *Edit90;
	TTabSheet *TabSheet8;
	TLabel *Label125;
	TLabel *Label126;
	TLabel *Label127;
	TLabel *Label128;
	TLabel *Label129;
	TLabel *Label130;
	TLabel *Label131;
	TEdit *Edit175;
	TEdit *Edit176;
	TEdit *Edit177;
	TEdit *Edit178;
	TEdit *Edit179;
	TEdit *Edit180;
	TEdit *Edit181;
	TEdit *Edit182;
	TEdit *Edit183;
	TEdit *Edit184;
	TEdit *Edit185;
	TEdit *Edit186;
	TEdit *Edit187;
	TEdit *Edit188;
	TEdit *Edit189;
	TEdit *Edit190;
	TEdit *Edit191;
	TEdit *Edit192;
	TEdit *Edit193;
	TEdit *Edit194;
	TEdit *Edit195;
	TEdit *Edit196;
	TEdit *Edit197;
	TEdit *Edit198;
	TEdit *Edit199;
	TEdit *Edit200;
	TEdit *Edit201;
	TEdit *Edit202;
	TEdit *Edit203;
	TEdit *Edit204;
	TComboBox *ComboBox85;
	TComboBox *ComboBox86;
	TComboBox *ComboBox87;
	TComboBox *ComboBox88;
	TComboBox *ComboBox89;
	TComboBox *ComboBox90;
	TComboBox *ComboBox91;
	TComboBox *ComboBox92;
	TComboBox *ComboBox93;
	TComboBox *ComboBox94;
	TComboBox *ComboBox95;
	TComboBox *ComboBox96;
	TComboBox *ComboBox97;
	TComboBox *ComboBox98;
	TComboBox *ComboBox99;
	TComboBox *ComboBox100;
	TComboBox *ComboBox101;
	TComboBox *ComboBox102;
	TComboBox *ComboBox103;
	TComboBox *ComboBox104;
	TEdit *Edit205;
	TEdit *Edit206;
	TEdit *Edit207;
	TEdit *Edit208;
	TEdit *Edit209;
	TEdit *Edit210;
	TEdit *Edit211;
	TEdit *Edit212;
	TEdit *Edit213;
	TEdit *Edit214;
	TTabSheet *TabSheet9;
	TLabel *Label132;
	TLabel *Label133;
	TLabel *Label134;
	TLabel *Label135;
	TLabel *Label136;
	TLabel *Label137;
	TLabel *Label138;
	TEdit *Edit215;
	TEdit *Edit216;
	TEdit *Edit217;
	TEdit *Edit218;
	TEdit *Edit219;
	TEdit *Edit220;
	TEdit *Edit221;
	TEdit *Edit222;
	TEdit *Edit223;
	TEdit *Edit224;
	TEdit *Edit225;
	TEdit *Edit226;
	TEdit *Edit227;
	TEdit *Edit228;
	TEdit *Edit229;
	TEdit *Edit230;
	TEdit *Edit231;
	TEdit *Edit232;
	TEdit *Edit233;
	TEdit *Edit234;
	TEdit *Edit235;
	TEdit *Edit236;
	TEdit *Edit237;
	TEdit *Edit238;
	TEdit *Edit239;
	TEdit *Edit240;
	TEdit *Edit241;
	TEdit *Edit242;
	TEdit *Edit243;
	TEdit *Edit244;
	TComboBox *ComboBox105;
	TComboBox *ComboBox106;
	TComboBox *ComboBox107;
	TComboBox *ComboBox108;
	TComboBox *ComboBox109;
	TComboBox *ComboBox110;
	TComboBox *ComboBox111;
	TComboBox *ComboBox112;
	TComboBox *ComboBox113;
	TComboBox *ComboBox114;
	TComboBox *ComboBox115;
	TComboBox *ComboBox116;
	TComboBox *ComboBox117;
	TComboBox *ComboBox118;
	TComboBox *ComboBox119;
	TComboBox *ComboBox120;
	TComboBox *ComboBox121;
	TComboBox *ComboBox122;
	TComboBox *ComboBox123;
	TComboBox *ComboBox124;
	TEdit *Edit245;
	TEdit *Edit246;
	TEdit *Edit247;
	TEdit *Edit248;
	TEdit *Edit249;
	TEdit *Edit250;
	TEdit *Edit251;
	TEdit *Edit252;
	TEdit *Edit253;
	TEdit *Edit254;
	TLabel *Label69;
	TLabel *Label84;
	TLabel *Label139;
	TLabel *Label140;
	TLabel *Label141;
	TLabel *Label142;
	TLabel *Label70;
	TLabel *Label90;
	TLabel *Label143;
	TLabel *Label144;
	TLabel *Label145;
	TLabel *Label146;
	TCheckBox *CheckBoxFeatureUnclickMove;
	TCheckBox *CheckBoxFeatureClickup;
	TCheckBox *CheckBox9;
	TCheckBox *CheckBox10;
	TCheckBox *CheckBox11;
	TCheckBox *CheckBox12;
	TCheckBox *CheckBox13;
	TCheckBox *CheckBox14;
	TCheckBox *CheckBox15;
	TCheckBox *CheckBox16;
	TCheckBox *CheckBox18;
	TCheckBox *CheckBoxReorderMap;
	TComboBox *ComboBoxUartReportType;
	TComboBox *ComboBoxGlareLever;
	TLabel *Label173;
	TLabel *Label174;
	TLabel *Label175;
	TLabel *Label176;
	TLabel *Label177;
	TEdit *EditGestureStart;
	TEdit *EditGestureEnd;
	TLabel *Label178;
	TEdit *EditTracePoints;
	TLabel *Label179;
	TEdit *Edit255;
	TCheckBox *CheckBoxNotD7;
	TCheckBox *CheckBoxWaitRxPowerOn;
	TLabel *Label180;
	TLabel *Label181;
	TEdit *EditXScanGap;
	TLabel *Label182;
	TLabel *Label183;
	TEdit *EditYScanGap;
	TLabel *Label184;
	TComboBox *ComboBoxXDivMethod;
	TComboBox *ComboBoxYDivMethod;
	TEdit *EditXBoardLed;
	TCheckBox *CheckBox17;
	TEdit *EditYBoardLed;
	TCheckBox *CheckBox19;
	TCheckBox *CheckBoxSupport64Boards;
	TCheckBox *CheckBoxFeatureAutoGainSub;
	TEdit *EditDispVID;
	TEdit *EditDispManuInfo;
	TEdit *EditDispPID;
	TEdit *EditDispProInfo;
	TLabel *Label185;
	TLabel *Label186;
	TLabel *Label187;
	TLabel *Label188;
	TCheckBox *CheckBoxD2CurHigh;
	TCheckBox *CheckBoxFeatureAutoGainUniform;
	TCheckBox *CheckBoxWorkTime;
	TEdit *EditWorkTime;
	TLabel *Label189;
	TButton *Button41;
	void __fastcall ButtonDownTabClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ComboBoxXAxisChange(TObject *Sender);
	void __fastcall ComboBoxYAxisChange(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall ClcParam(TList *Edits, AXI_PARAM *Ap);
	void __fastcall ClcAuxParam(TList *Edits, AXI_PARAM *Ap);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall ComboBoxXBoardsChange(TObject *Sender);
	void __fastcall ComboBoxYBoardsChange(TObject *Sender);
	void __fastcall TimerDownTabTimer(TObject *Sender);
	void __fastcall StringGrid1DrawCell(TObject *Sender, int ACol, int ARow,
		TRect &Rect, TGridDrawState State);
	void __fastcall ButtonDownParamClick(TObject *Sender);
	void __fastcall Button8Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall ComboBoxStdScreenChange(TObject *Sender);
	void __fastcall ButtonSwitchClick(TObject *Sender);
	void __fastcall ButtonGetStaClick(TObject *Sender);
	void __fastcall Button10Click(TObject *Sender);
	void __fastcall ButtonFlashProtectClick(TObject *Sender);
	void __fastcall ButtonGetCPUIDClick(TObject *Sender);
	void __fastcall Button15Click(TObject *Sender);
	void __fastcall Button9Click(TObject *Sender);
	void __fastcall Button13Click(TObject *Sender);
	void __fastcall ButtonWriteSerialNumClick(TObject *Sender);
	void __fastcall ComboBoxStdParamChange(TObject *Sender);
	void __fastcall ButtonReadSerialNumClick(TObject *Sender);
	void __fastcall ButtonSecretInitialClick(TObject *Sender);
	void __fastcall Button7Click(TObject *Sender);
	void __fastcall Button11Click(TObject *Sender);
	void __fastcall ButtonParamReadClick(TObject *Sender);
	void __fastcall Button12Click(TObject *Sender);
	void __fastcall ButtonGetScanParamClick(TObject *Sender);
	void __fastcall Button19Click(TObject *Sender);
	void __fastcall Button18Click(TObject *Sender);
	void __fastcall Button21Click(TObject *Sender);
	void __fastcall Button22Click(TObject *Sender);
	void __fastcall Button16Click(TObject *Sender);
	void __fastcall Button17Click(TObject *Sender);
	void __fastcall Button20Click(TObject *Sender);
	void __fastcall Button23Click(TObject *Sender);
	void __fastcall Button24Click(TObject *Sender);
	void __fastcall Button25Click(TObject *Sender);
	void __fastcall Button26Click(TObject *Sender);
	void __fastcall Button27Click(TObject *Sender);
	void __fastcall ButtonForceWROffClick(TObject *Sender);
	String __fastcall GetCPUType(void);
	void __fastcall Button30Click(TObject *Sender);
	void __fastcall CheckBoxFeatureUSARTOnClick(TObject *Sender);

	void __fastcall EditXoffsetChange(TObject *Sender);
	void __fastcall EditYoffsetChange(TObject *Sender);
	void __fastcall ComboBoxYDivMethodChange(TObject *Sender);
	void __fastcall ComboBoxXDivMethodChange(TObject *Sender);
	void __fastcall CheckBox17Click(TObject *Sender);
	void __fastcall CheckBox19Click(TObject *Sender);
	void __fastcall EditXBoardLedChange(TObject *Sender);
	void __fastcall EditYBoardLedChange(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall EditXScanGapChange(TObject *Sender);
	void __fastcall CheckBoxFeatureGainAdjClick(TObject *Sender);
	void __fastcall CheckBoxWorkTimeClick(TObject *Sender);
private:	// User declarations

	TList *XParamEdit;
	TList *YParamEdit;
	TList *FeatureCheckBox;
	TList *FeatureCheckBoxUser;
	TList *ScanFeatureCheckBox;
	TList *XCutBoards;
	TList *YCutBoards;

	int16_t *XOffsets;
	int16_t *YOffsets;
	int16_t *Offsets;

	float *XTan;
	float *YTan;
	float *Tan;

	float *XCoTan;
	float *YCoTan;
	float *CoTan;

	float *XOffLen;
	float *YOffLen;
	float *OffLen;

public:		// User declarations
	__fastcall TFormConfig(TComponent* Owner);
	void __fastcall AddStdConfig(TIniFile *ini, String Section);
	void __fastcall DispSta(void);
	void __fastcall DispCPUID(uint8_t *Data, uint16_t Size);
	void __fastcall DevConnect(void);
	void __fastcall DevDisConnect(void);
	bool __fastcall CheckParam(void);
	long __fastcall CheckCPUID(FILE *fp);
	long __fastcall CheckSerialNum(FILE *fp);
	void __fastcall ShowShipInfo(uint8_t *Buf, uint16_t Len);
	void __fastcall ShowParam(uint8_t *Buf, uint16_t Len);
	void __fastcall ShowUserParam(uint8_t *Buf, uint16_t Len);
	void __fastcall ShowScanParam(uint8_t *Buf, uint16_t Len);
	void __fastcall DisableButtons(void);
	void __fastcall EnableButtons(void);

	bool __fastcall WriteDataBase(void);

	void __fastcall SaveParamConfig(TIniFile *ini, String Section);
	void __fastcall SaveScanTableConfig(TIniFile *ini, String Section);
	void __fastcall SaveScanTableConfigTri(TIniFile *ini, String Section);
	bool __fastcall Gen_DMATable_Rule_Check(uint32_t Flags);

	bool __fastcall GenScanTable(void);
	int __fastcall GetMaxNum(uint8_t Num);

	void __fastcall ShowAutoGainResult(uint8_t *Buf, uint16_t Len);
	bool __fastcall GenScanTableTri(void);
	void __fastcall ShowScanParam_Tri(uint8_t *Buf, uint16_t Len);
	void __fastcall ClcStdParam(TList *Edits, AXI_PARAM *Ap);
	void __fastcall ClcMixParam(TList *Edits, AXI_PARAM *Ap);
	int __fastcall ToBoardLeds(uint8_t *srt, int* BoardLed);
	void __fastcall ShowManuProInfo(uint8_t *Buf, uint16_t Len);

	bool UserReq;
	uint8_t Table[256 * 1024];
	uint32_t TableLen;
	SCAN_TABLE_HEAD TabHead;
	SCAN_TABLE_HEAD ScanParam;
	SCAN_TABLE_HEAD_TRI TabHeadTri;
	SCAN_TABLE_HEAD_TRI ScanParamTri;

	PROGRAM_HEAD Head;
	RunTimeParam_t Param;
	RunTimeParam_t UserParam;

	uint8_t ParamBuf[2 * 1024];
	uint8_t SecretBuf[16 * 1024];
	BOOT_PARAM BootParam;
	uint8_t CPUID[12];
	time_t ShipTime;

	uint8_t SerialBuf[2048];

	Calibration_t CalibParam;

    uint8_t buf[1024*1024];
};

extern PACKAGE TFormConfig *FormConfig;
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
#endif
