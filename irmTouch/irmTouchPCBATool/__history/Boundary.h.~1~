//---------------------------------------------------------------------------

#ifndef BoundaryH
#define BoundaryH
#include <stdint.h>
//---------------------------------------------------------------------------
#include "FrameInfo.h"
#include "ADProcess.h"

#define INTERVALLIMIT 2*MAX_POINTS1


#pragma (push, 1)
typedef struct
{
	int16_t Begin;
	int16_t End;
	//int32_t MatchMain;
}section_t;

typedef struct							 //凹坑结构体
{
	uint8_t Num;                     //该轴凹坑数量
	section_t Section[MAX_INTERVALS+2];			//凹坑坐标			//凹坑坐标
}interval_t;

#pragma (pop)

#define PCB_TYPE_REV	1
#define PCB_TYPE_EMIT	2

#define PCB_POS_X	1
#define PCB_POS_Y	2

#define BADLED_EX	(0<<14)
#define BADLED_EY	(1<<14)
#define BADLED_RX	(2<<14)
#define BADLED_RY	(3<<14)
#define BADLED_FLAG_MASK   (3<<14)
#define BADLED_NO_MASK   (0x3FFF)
#define MAX_BADLEDS	100

#define MAX_LEDS    2000

#define CORNER_LEDS     	(3)
#define MAX_CORNER_AXIS     (CORNER_LEDS * 8)
class Boundary
{
private:	// User declarations
		uint8_t MemPool[1024*1024];
public:		// User declarations
		interval_t Intervals[MAX_AXIS*2 + MAX_CORNER_AXIS];		   	//凹坑信息
		interval_t AuxIntervals[MAX_AXIS*2 + MAX_CORNER_AXIS];
		interval_t TotalIntervals[MAX_AXIS*4 + MAX_CORNER_AXIS];	//Intervals + AuxIntervals
		interval_t MixIntvs[MAX_AXIS*4 + MAX_CORNER_AXIS];			//Intervals + AuxIntervals
		interval_t PreciseIntvs[10];				//Intervals + AuxIntervals
		interval_t CornerIntvs[MAX_CORNER_AXIS];
		interval_t AdjacentIntvs[MAX_AXIS*2];		//Intervals + AuxIntervals
		uint8_t *AdBuf[MAX_AXIS*2 + MAX_CORNER_AXIS];               //AD 原始数据
		uint8_t *AxisAd[MAX_AXIS*2 + MAX_CORNER_AXIS];             	//AD 原始数据
		uint8_t *UsualVolt[MAX_AXIS*2 + MAX_CORNER_AXIS];		   	//环境电压
		uint8_t *MaxVolt[MAX_AXIS*2 + MAX_CORNER_AXIS];			   	//最大电压
		uint8_t *NormAd[MAX_AXIS*2 + MAX_CORNER_AXIS];			   	//归一化后的各轴电压
		uint8_t *BadLeds[MAX_AXIS*2 + MAX_CORNER_AXIS];				//坏灯处理
		uint8_t OldAreaNum;					   		//上一帧触摸点数量
		uint8_t first_run;					   		//是否第一次运行的标志
		interval_t DynamicBound[2];            		//确定动态扫描的边界

		uint8_t EAd[MAX_AXIS*2 + MAX_CORNER_AXIS];			   		//归一化后的各轴电压
		uint8_t EAdMax[MAX_AXIS*2 + MAX_CORNER_AXIS];
		uint8_t EAdMin[MAX_AXIS*2 + MAX_CORNER_AXIS];

		SigQ_t SigQ[MAX_AXIS*2 + MAX_CORNER_AXIS];

		uint8_t XNumMaxItv;
		uint8_t YNumMaxItv;
		//float E_Ad[MAX_AXIS*2];			   //归一化后的各轴电压
		//float D_Ad[MAX_AXIS*2];			   //归一化后的各轴电压

		int Cury;

		uint16_t BadLedNo[MAX_BADLEDS];
		uint16_t BadLedNum;
		uint32_t TotalIntvs;
		uint32_t TotalCornerIntvs;


		__fastcall Boundary(void);

		void __fastcall GetTotalInterval_old(void);

		void __fastcall GetInterval(void);
		void __fastcall AddDefaultInterval(void);
		void __fastcall MakeInterval(int SigVersion);
		void __fastcall Normalize(void);

		void __fastcall GetAxisVolt(int SigVersion);
		void __fastcall InitialUsualVolt(int SigVersion);
		void __fastcall InitialMaxVolt(int SigVersion);
		void __fastcall InitialMinVolt(int SigVersion);

		void __fastcall Allocate_Buffers(void);

		uint8_t __fastcall NormalAD(int i, int j);
		void __fastcall GetAuxInterval(void);
		void __fastcall GetDynamicBound(void);
		void __fastcall Expand_Interval(void);
		void __fastcall SignalQuality(int Axis);
		void __fastcall Adj_WithOutTouch(void);
		void __fastcall Adj_MaxMin(int Axis);
		void __fastcall Normal_CornerSignal(void);

		//void __fastcall Find_Max_Num_Interval(void);
		void __fastcall Interval_Match_Main(void);
		bool __fastcall IsRev(uint16_t LedPos, uint8_t EAd, uint8_t PCBPos);
		bool __fastcall IsEmit(uint16_t LedPos, uint8_t EAd, uint8_t PCBPos);

		int __fastcall AdjacentAxi_Shield_Check(uint16_t CurRev, uint8_t CurAxi);
		void __fastcall MergeMixInterval(void);

		bool __fastcall BadLed_Check(void);
		void __fastcall BadLed_Check2(void);
		bool __fastcall Signal_Is_Initialized(void);
		
		#ifdef _MATCH_
		signals_t sigUp, sigLeft, sigBottom, sigRight;//记录每条边上所有接收灯获取的凹坑信号
		void __fastcall AllocateSignals(void);
		void __fastcall Interval2Signal(void);
		#define INTE_WIDTH_THRESH 256 //同一触点形成的凹坑的宽度之差的阈值(若两凹坑宽度差超过该值则判断两凹坑对应的不是同一触点，以灯的编号*SCALE为单位)
		#define COMLEN_FACTOR 3 //类似于原detect_c.c中的F参数
		#endif

		void __fastcall CombineInterval(void);
		void __fastcall GetMixInterval(void);
		
		void __fastcall GetAdjecentInterval(void);
		void __fastcall GetPreciseInterval(void);
			//new
		void __fastcall GetTotalSignalPit(void);     //NEW
        void __fastcall FindSignalPit(uint8_t* norm, uint8_t normindex, uint8_t pitnum, int lednum);
    	void __fastcall CopyPit(int lednum, uint8_t start, uint8_t end, uint8_t startsignal, uint8_t endsignal, uint8_t condition, uint8_t pitnum);
        void __fastcall Boundary::FindBoundaryX(void);
		void __fastcall Boundary::FindBoundaryY(void);
    	void __fastcall Boundary::CopySideLine(uint8_t sidelinenum, unsigned int recx, unsigned int recy, unsigned int trmx, unsigned int trmy, bool isvalid);



		//function declarations for new D2 algorithm
		void __fastcall Boundary::getInterval1(void);
		void __fastcall Boundary::scanAdjacentMainX1(uint8_t axisnum, uint8_t axisindexsmall, uint8_t axisindexlarge, int16_t startled, int16_t endled);
		void __fastcall Boundary::scanAdjacentNegativeX1(uint8_t axisnum, uint8_t axisindexsmall, uint8_t axisindexlarge, int16_t startled, int16_t endled, uint8_t rightcorner);
		void __fastcall Boundary::scanAdjacentPositiveX1(uint8_t axisnum, uint8_t axisindexsmall, uint8_t axisindexlarge, int16_t startled, int16_t endled, uint8_t leftcorner);
		void __fastcall Boundary::scanAdjacentMainY1(uint8_t axisnum, uint8_t axisindexsmall, uint8_t axisindexlarge, int16_t startled, int16_t endled);
		void __fastcall Boundary::scanAdjacentNegativeY1(uint8_t axisnum, uint8_t axisindexsmall, uint8_t axisindexlarge, int16_t startled, int16_t endled, uint8_t upcorner);
		void __fastcall Boundary::scanAdjacentPositiveY1(uint8_t axisnum, uint8_t axisindexsmall, uint8_t axisindexlarge, int16_t startled, int16_t endled, uint8_t downcorner);
		void __fastcall Boundary::copyIntervalXF1(uint8_t axisnum, int16_t fallingedgerecx, int16_t offsetf);
		void __fastcall Boundary::copyIntervalXR1(uint8_t axisnum, int16_t risingedgerecx, int16_t offsetr);
		void __fastcall Boundary::copyIntervalYF1(uint8_t axisnum, int16_t fallingedgerecy, int16_t offsetf); 
		void __fastcall Boundary::copyIntervalYR1(uint8_t axisnum, int16_t risingedgerecy, int16_t offsetr);
		void __fastcall Boundary::copyBlockedLineX1(uint8_t axisnum, int16_t recx, int16_t offset);
		void __fastcall Boundary::copyBlockedLineY1(uint8_t axisnum, int16_t recy, int16_t offset);
		//dynamic scan
		void __fastcall Boundary::findSignalPitX(uint16_t pitspace);
		void __fastcall Boundary::findSignalPitY(uint16_t pitspace);
		void __fastcall Boundary::calculateTouchArea(uint16_t expandsize);
		void __fastcall Boundary::calculateScanTable(void);
		void __fastcall Boundary::initTangents1(void);

};

extern Boundary MtBound;
#endif
