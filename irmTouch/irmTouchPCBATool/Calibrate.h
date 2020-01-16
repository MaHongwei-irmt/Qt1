// ---------------------------------------------------------------------------

#ifndef CalibrateH
#define CalibrateH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "stdint.h"
#include "Config.h"
#include "image.h"

const int CALIB_POINT_FIFO_MASK = 0x0FF;
const int CALIB_POINT_FIFO_LEN = CALIB_POINT_FIFO_MASK + 1;

const int CALIB_INFO_VALID = (1 << 0);

typedef struct CALIB_POINTS {
	uint8_t Points;

	struct {
		int x;
		int y;
		uint8_t ID;
		uint8_t Sta;
	}

	pt[10];
} CalibPoints_t;

typedef struct Calib_Points_Info {
	int x0, y0; // 校准前屏幕点的实际坐标    0~32767 与屏幕分辨率无关
	int x1, y1; // 校准前触摸屏上报的坐标	约1秒钟 滑动平均值
} Calib_Points_Info_t;

typedef struct Calib_Info {
	uint32_t Flag;
	uint8_t xPoints;
	uint8_t yPoints;
	uint8_t TotalPoints;

	Calib_Points_Info_t Calib[6 * 6 + 5 * 5]; // 最大校准点数
} Calib_Info_t;

// ---------------------------------------------------------------------------
class TFormCalibrate : public TForm {
__published: // IDE-managed Components
	TImage *ImageCalibrate;
	TTimer *Timer1;

	void __fastcall FormResize(TObject *Sender);
	void __fastcall ImageCalibrateDblClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, char &Key);
	void __fastcall Timer1Timer(TObject *Sender);

private: // User declarations
public: // User declarations

	Calib_Info_t CalibInfo;
	int PtNo;
	int Percent;
	int PtID;

	enum CalibSta_ {
		WAIT_NEW_POINT = 0, SKIP_BEGIN, CLC_POS
	} CalibSta;

	CalibPoints_t PtFifo[CALIB_POINT_FIFO_LEN];
	uint16_t InPtr;
	uint16_t OutPtr;
	uint16_t xPxiMargin;
	uint16_t yPxiMargin;

	bool PointMode;

	void __fastcall CalibrateFSM(void);
	__fastcall TFormCalibrate(TComponent* Owner);
	void __fastcall UpdatePoint(int No, int Pecent);
	void __fastcall ResizeBitMap(void);
	void __fastcall DrawBack(void);
	void __fastcall ProcessPoints(int Points, point_t(&pt)[]);
};

// ---------------------------------------------------------------------------
extern PACKAGE TFormCalibrate *FormCalibrate;
// ---------------------------------------------------------------------------
#endif
