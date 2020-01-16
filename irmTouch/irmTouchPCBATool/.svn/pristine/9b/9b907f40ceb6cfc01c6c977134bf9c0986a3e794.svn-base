//---------------------------------------------------------------------------

#ifndef ADProcessH
#define ADProcessH
//---------------------------------------------------------------------------
#include "Config.h"
#include "stdio.h"

#pragma   pack(push,1)
typedef struct DebugHead{
	uint8_t Type;
	uint8_t FrameNum;
	uint8_t Fps;
	uint16_t ScanTime;
	uint8_t XAxis;
	uint8_t YAxis;
	uint16_t XLeds;
	uint16_t YLeds;
	int16_t Offsets[MAX_AXIS * 2];
}DebugHead_t;

typedef struct DebugHeadV2{
	uint8_t Type;
	uint8_t FrameNum;
	uint8_t Fps;
	uint16_t ScanTime;
	uint8_t XAxis;
	uint8_t YAxis;
	uint16_t XLeds;
	uint16_t YLeds;
	int16_t Offsets[MAX_AXIS * 4];
}DebugHeadV2_t;

typedef struct softscan_ctrl{
	uint8_t Flag;
	int16_t xStart;
	int16_t xEnd;
	int16_t yStart;
	int16_t yEnd;
	uint8_t xAxis;
	uint8_t yAxis;
	uint8_t AxiIds[MAX_AXIS*2];
}softscan_ctrl_t;

typedef struct
{
	float EAd;
	float DAd;
	float EMax;
}SigQ_t;
#pragma   pack(pop)

#define DEBUG_SCAN_FLAG_START		(1<<0)
#define DEBUG_SCAN_FLAG_ALL			(1<<1)
#define DEBUG_SCAN_FLAG_PARTIAL		(1<<2)
#define DEBUG_SCAN_FLAG_CORNER		(1<<3)
#define DEBUG_SCAN_FLAG_DEBUG		(1<<4)
#define DEBUG_SCAN_FLAG_AUTORESCAN	(1<<5)

class ADProcess
{
public :
	__fastcall ADProcess(void);

	void __fastcall LogData(void *DebugHead, uint8_t *Data);
	void __fastcall LogADData(FILE *fp, void *Head, uint8_t *Data, bool ClcCheckSum);
	void __fastcall LogDebugADData(FILE *fp, DebugHead_t *Head, uint8_t *Data);
	void __fastcall LogADDataV4(FILE *fp, void *Head, uint8_t *Data, bool ClcCheckSum);
	void __fastcall LogADData_V5(FILE *fp, void *Head, uint8_t *Data);
	void __fastcall LogADData_V5_Cur(FILE *fp, void *Head, uint8_t **Buf);

	void __fastcall ProcessFrame(uint8_t *Data, uint32_t Len);

	void __fastcall ProcessAD(uint8_t ver, uint8_t EnvFlag, uint8_t *Data, uint32_t Len);
	void __fastcall ProcessPoint(uint8_t EnvFlag, uint8_t *Data, uint32_t Len);
	void __fastcall AlgInitial(void);
	void __fastcall ParamUpdate(DebugHead_t *dh);
	void __fastcall ParamUpdateV2(DebugHeadV2_t *dh);
	void __fastcall Initial_Algorithm(void);
	void __fastcall CloseLogFile(void);

	void __fastcall Update_Partial_Scan_Data(uint8_t *Data, uint32_t Len);
	void __fastcall Update_Corner_Scan_Data(uint8_t *Data, uint32_t Len);
	void __fastcall Update_Debug_Scan_Data(uint8_t *Data, uint32_t Len);

	//void __fastcall FillTraceBuf(point_t &Rpt);

	FILE *Logfp;
	bool LastLogSta;

	DebugHead_t LastDebugHead;
	DebugHead_t *CurDebugHead;
	DebugHeadV2_t LastDebugHeadV2;
	DebugHeadV2_t *CurDebugHeadV2;

	uint8_t DebugDataVer;

	uint16_t LastCornerSignals;
	uint16_t CornerSignals;

};

extern ADProcess ADProc;

#define REPORT_MODE_VERSION_MASK	0xF0
#define REPORT_MODE_VERSION1		0x00
#define REPORT_MODE_VERSION2		0x10
#define REPORT_MODE_VERSION3		0x20
#define REPORT_MODE_VERSION4		0x30
#define REPORT_MODE_VERSION5		0x40
		
#define REPORT_MODE_NORMAL		0x00
#define REPORT_MODE_AD			0x01
#define REPORT_MODE_INTERVAL	0x02
#define REPORT_MODE_POINT		0x03
#define REPORT_MODE_MAX			0x03

const String LogFileMagicStringV1 = "AD Log Data Version 1.0";
const String LogFileMagicStringV2 = "AD Log Data Version 2.0";    //we add aux scan param
const String LogFileMagicStringV4 = "AD Log Data Version 4.0";    //¶¯Ì¬É¨Ãè
const String LogFileMagicStringV5 = "AD Log Data Version 5.0";    //¶¯Ì¬É¨Ãè  ËùÓÐÖá¼°±ß½Ç
const String LogRealPointFileMagicStringV1 = "Real Points Log Data Version 1.0";
#endif
