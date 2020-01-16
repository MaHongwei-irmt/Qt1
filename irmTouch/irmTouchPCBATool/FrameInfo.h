// ---------------------------------------------------------------------------

#ifndef FrameInfoH
#define FrameInfoH

#include "config.h"
#include "ADProcess.h"

// const uint16_t MAX_POINTS = 32;
// const uint16_t MAX_CLC_POINTS = MAX_POINTS + 2;
//
// const uint16_t MAX_AREAS = (MAX_CLC_POINTS * MAX_CLC_POINTS);
// const uint16_t MAX_INTERVALS = MAX_CLC_POINTS*2;

const uint16_t MAX_POINTS = 40;
const uint16_t MAX_CLC_POINTS = MAX_POINTS + 1;

const uint16_t MAX_AREAS = (MAX_CLC_POINTS * MAX_CLC_POINTS);
const uint16_t MAX_INTERVALS = MAX_CLC_POINTS * 2;

typedef enum ScreenType {
	ST_Normal, ST_Tri
} screen_type_t;

typedef struct SCREEN_INFO {
	uint16_t XLeds;
	uint16_t YLeds;
	uint16_t TotalLeds;

	uint8_t XAxis;
	uint8_t YAxis;
	uint8_t TotalAxis;

	int16_t XOffsets[MAX_AXIS];
	int16_t YOffsets[MAX_AXIS];
	int16_t Offsets[MAX_AXIS * 2];

	int16_t TotalXOffsets[MAX_AXIS];
	int16_t TotalYOffsets[MAX_AXIS];
	int16_t TotalOffsets[MAX_AXIS * 2];

	// float XTan[MAX_AXIS];
	// float YTan[MAX_AXIS];
	// float Tan[MAX_AXIS*2];
	//
	// float XCoTan[MAX_AXIS];
	// float YCoTan[MAX_AXIS];
	float CoTan[MAX_AXIS * 2];
	//
	// float TotalXTan[MAX_AXIS];
	// float TotalYTan[MAX_AXIS];
	// float TotalTan[MAX_AXIS*2];
	//
	// float TotalXCoTan[MAX_AXIS];
	// float TotalYCoTan[MAX_AXIS];
	// float TotalCoTan[MAX_AXIS*2];
	//
	// float TotalXCos[MAX_AXIS];
	// float TotalYCos[MAX_AXIS];
	// float TotalCos[MAX_AXIS*2];
	//
	// float TotalXSin[MAX_AXIS];
	// float TotalYSin[MAX_AXIS];
	// float TotalSin[MAX_AXIS*2];

	// float XOffLen[MAX_AXIS];
	// float YOffLen[MAX_AXIS];
	// float OffLen[MAX_AXIS*2];
	//
	// uint16_t XAxisId[MAX_AXIS];
	// uint16_t YAxisId[MAX_AXIS];
	// uint16_t AxisId[MAX_AXIS*2];
	//
	// uint16_t XStartLed[MAX_AXIS];
	// uint16_t YStartLed[MAX_AXIS];
	// uint16_t StartLed[MAX_AXIS*2];
	//
	// uint16_t XStopLed[MAX_AXIS];
	// uint16_t YStopLed[MAX_AXIS];
	// uint16_t StopLed[MAX_AXIS*2];

	int16_t AuxXOffsets[MAX_AXIS];
	int16_t AuxYOffsets[MAX_AXIS];
	int16_t AuxOffsets[MAX_AXIS * 2];

	// float AuxXTan[MAX_AXIS];
	// float AuxYTan[MAX_AXIS];
	// float AuxTan[MAX_AXIS*2];
	//
	// float AuxXCoTan[MAX_AXIS];
	// float AuxYCoTan[MAX_AXIS];
	// float AuxCoTan[MAX_AXIS*2];
	//
	// float AuxXOffLen[MAX_AXIS];
	// float AuxYOffLen[MAX_AXIS];
	// float AuxOffLen[MAX_AXIS*2];

	uint16_t Signals;

	uint16_t CornerSignals;
	uint16_t XCornerAxis;
	uint16_t YCornerAxis;

} info_t;

typedef struct SCREEN_INFO_TRI {
	uint8_t XBoards;
	uint8_t YBoards;

	uint16_t XEmitLeds;
	uint16_t YEmitLeds;
	uint16_t EmitLeds;
	uint16_t XRevLeds;
	uint16_t YRevLeds;
	uint16_t RevLeds;

	uint16_t Signals;
	uint16_t XSignals;
	uint16_t YSignals;

	PCB_CONFIG PCBs[MAX_BOARDS];

	uint16_t RevPos[MAX_REV_LEDS];
	uint16_t EmitStart[MAX_REV_LEDS];
	uint16_t EmitEnd[MAX_REV_LEDS];

	float XTan[MAX_EMIT_LEDS];
	float YTan[MAX_EMIT_LEDS];

	float XCoTan[MAX_EMIT_LEDS];
	float YCoTan[MAX_EMIT_LEDS];

	float MarginLeft;
	float MarginRight;
	float MarginUp;
	float MarginDown;

	float XLength;
	float YLength;
	float XTouchLen; // 发射灯
	float YTouchLen;

	uint16_t XLedOff;
	uint16_t XLedSpace;

	uint16_t YLedOff;
	uint16_t YLedSpace;
} tri_info_t;

#define XLEDS		(FrameInfo.Info.XLeds)
#if XAXIS_MODE == 0
#define YLEDS		(FrameInfo.Info.YLeds)
#else
#define YLEDS		(XLEDS*9/16)	//juest for debug 16:9
#endif
#define TOTAL_LEDS	(FrameInfo.Info.TotalLeds)

#define X_CORNER_AXIS		(FrameInfo.Info.XCornerAxis)
#define Y_CORNER_AXIS		(FrameInfo.Info.YCornerAxis)
#define CORNER_AXIS			(X_CORNER_AXIS + Y_CORNER_AXIS)

#define XAXI_LEFT_REV   	(ALL_AXIS + 0)
#define XAXI_RIGHT_REV   	(ALL_AXIS + X_CORNER_AXIS/2-1)
#define XAXI_LEFT_EMIT   	(ALL_AXIS + X_CORNER_AXIS/2)
#define XAXI_RIGHT_EMIT   	(ALL_AXIS + X_CORNER_AXIS -1)
#define YAXI_DOWN_REV   	(ALL_AXIS + X_CORNER_AXIS)
#define YAXI_UP_REV   		(ALL_AXIS + X_CORNER_AXIS + Y_CORNER_AXIS/2-1)
#define YAXI_DOWN_EMIT   	(ALL_AXIS + X_CORNER_AXIS + Y_CORNER_AXIS/2)
#define YAXI_UP_EMIT   		(ALL_AXIS + X_CORNER_AXIS + Y_CORNER_AXIS-1)

#define XAXIS		(FrameInfo.Info.XAxis)
#define YAXIS		(FrameInfo.Info.YAxis)
#define TOTAL_AXIS	(FrameInfo.Info.TotalAxis)

#define MIX_XAXIS		(XAXIS*2 - 1)
#define MIX_YAXIS		(YAXIS*2 - 1)
#define MIX_TOTAL_AXIS	(MIX_XAXIS + MIX_YAXIS)

#define MAX_X_OFF		(FrameInfo.Info.XOffsets[XAXIS-1])
#define MAX_Y_OFF		(FrameInfo.Info.YOffsets[YAXIS-1])
// --------------------------------------------------------------------------------------
#define LED_SPACE		9.00f		//毫米
#define X_MARGIN_LEFT	7.00
#define X_MARGIN_RIGHT	7.00
#define X_MARGIN_TOTAL	(X_MARGIN_LEFT + X_MARGIN_RIGHT)

#define Y_MARGIN_UP    	7.00
#define Y_MARGIN_DOWN   7.00
#define Y_MARGIN_TOTAL	(Y_MARGIN_UP + Y_MARGIN_DOWN)

#define REC_VALID_WIDTH		2.888345
#define EMIT_VALID_WIDTH	2.317517
#define VALID_WIDTH		((REC_VALID_WIDTH + EMIT_VALID_WIDTH)/2)
#define	SCALE				64							 //处理时的坐标放大倍数
#define VALID_SCALE			(SCALE * VALID_WIDTH/LED_SPACE)
#define INVALID_SCALE       (SCALE - VALID_SCALE)

#define LED_WIDTH_SCALE		32
#define LED_SPACE_SCALE		(SCALE - LED_SCALE)
#define MAX_X_SCALE		(XLEDS*SCALE)

#if XAXIS_MODE == 0
#define MAX_Y_SCALE		(YLEDS*SCALE)
#else
#define MAX_Y_SCALE		(XLEDS*9*SCALE/16)
#endif

#define X_MARGIN_LEFT_SCALE		(X_MARGIN_LEFT * SCALE / LED_SPACE)
#define X_MARGIN_RIGHT_SCALE	(X_MARGIN_RIGHT * SCALE / LED_SPACE)
#define Y_MARGIN_UP_SCALE		(Y_MARGIN_UP * SCALE / LED_SPACE)
#define Y_MARGIN_DOWN_SCALE 	(Y_MARGIN_DOWN * SCALE / LED_SPACE)

#define X_MARGIN_SCALE	(X_MARGIN_LEN * SCALE / LED_SPACE)
#define Y_MARGIN_SCALE  (Y_MARGIN_LEN * SCALE / LED_SPACE)

#define MAX_X_COOD		(MAX_X_SCALE + X_MARGIN_RIGHT_SCALE)
#define MAX_Y_COOD		(MAX_Y_SCALE + Y_MARGIN_UP_SCALE)

#define MIN_X_COOD		(- X_MARGIN_LEFT_SCALE)
#define MIN_Y_COOD		(- Y_MARGIN_DOWN_SCALE)

#define MAX_X_SCALE_TRI     ((XLEDS - 0.5) * SCALE / 11.0)      //fix me !!!!!!!!!!!!!!
#define MAX_Y_SCALE_TRI     ((YLEDS - 0.5) * SCALE / 11.0)
#define MAX_X_COOD_TRI		(MAX_X_SCALE_TRI + X_MARGIN_RIGHT_SCALE)
#define MAX_Y_COOD_TRI		(MAX_Y_SCALE_TRI + Y_MARGIN_UP_SCALE)

#define MIN_X_COOD_TRI		(- X_MARGIN_LEFT_SCALE)
#define MIN_Y_COOD_TRI		(- Y_MARGIN_DOWN_SCALE)

class TouchFrame {
private: // User declarations

public:
	__fastcall TouchFrame(void);

	void __fastcall InitialFrameInfo(void);
	void __fastcall UpdateFrameInfo(DebugHead_t *dh, uint16_t CornerSignals);
	void __fastcall UpdateFrameAuxInfo(DebugHeadV2_t *dh,
		uint16_t CornerSignals);

	info_t Info;

	screen_type_t Type;
};

extern TouchFrame FrameInfo;
#endif
