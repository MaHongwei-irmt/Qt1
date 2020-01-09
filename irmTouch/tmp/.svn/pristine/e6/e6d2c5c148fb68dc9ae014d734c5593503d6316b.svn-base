//---------------------------------------------------------------------------

#ifndef BoundaryTriH
#define BoundaryTriH
//---------------------------------------------------------------------------

#include "stdint.h"
#include "FrameInfo.h"
#include "ADProcess.h"

#define FLAG_TYPEMASK		(0x03<<0)
#define FLAG_ID_YL			(0<<0)
#define FLAG_ID_XU			(1<<0)
#define FLAG_ID_XD			(2<<0)
#define FLAG_ID_YR			(3<<0)

#define FLAG_INTV_BEGIN_UNCLEAR	(1<<2)
#define FLAG_INTV_END_UNCLEAR	(1<<3)
#define FLAG_INTV_UNCLEAR		(3<<2)
#define FLAG_INTV_MIX			(1<<4)  	//凹坑重叠
#define FLAG_INTV_DETC			(1<<5)

#define MAX_INTERVALS_TRI   (MAX_REV_LEDS*MAX_POINTS)

#pragma (push, 1)
typedef struct Interval_Tri{
	uint8_t Flag;
	uint8_t RevId;
	uint8_t MatchID;        //凹坑匹配后的 ID
	uint8_t MixCnt;			//经过触摸点数，>1 表示改凹坑为复合凹坑
	int16_t Begin;
	int16_t End;
	int16_t OldBegin;
	int16_t OldEnd;
}intv_tri_t;
#pragma (pop)

typedef struct RevIntervals{
	uint8_t Num;
	intv_tri_t *intvs;
}rev_intv_t;

class BoundaryTri
{
private:	// User declarations
		uint8_t MemPool[1024*512];
public:		// User declarations
		intv_tri_t Intervals[MAX_INTERVALS_TRI];		   //凹坑信息
		rev_intv_t RevIntvs[MAX_REV_LEDS];

		uint8_t *AdBuf[MAX_REV_LEDS];                //AD 原始数据
		uint8_t *AxisAd[MAX_REV_LEDS];             //AD 原始数据
		uint8_t *UsualVolt[MAX_REV_LEDS];		   //环境电压
		uint8_t *MaxVolt[MAX_REV_LEDS];			   //最大电压
		uint8_t *NormAd[MAX_REV_LEDS];			   //归一化后的各轴电压
		uint8_t *BadLeds[MAX_REV_LEDS];			//坏灯处理
		uint8_t first_run;					   //是否第一次运行的标志

		uint8_t EAd[MAX_REV_LEDS];			   //归一化后的各轴电压
		uint8_t EAdMax[MAX_REV_LEDS];
		uint8_t EAdMin[MAX_REV_LEDS];

		SigQ_t SigQ[MAX_REV_LEDS];

		uint8_t XNumMaxItv;
		uint8_t YNumMaxItv;

		uint16_t IntervalNum;       //凹坑数量
		uint16_t YLIntvNum;
		uint16_t YRIntvNum;
		uint16_t XUIntvNum;
		uint16_t XDIntvNum;
		intv_tri_t *YLIntvs;
		intv_tri_t *YRIntvs;
		intv_tri_t *XUIntvs;
		intv_tri_t *XDIntvs;


		uint8_t MatchID;

		int Cury;

		__fastcall BoundaryTri(void);

		void __fastcall SignalQuality(void);
		
		void __fastcall InitialUsualVolt(void);
		void __fastcall InitialMaxVolt(void);
		void __fastcall InitialMinVolt(void);
		void __fastcall GetAxisVolt(void);
		uint8_t __fastcall NormalAD(int i, int j);

		void __fastcall GetInterval(void);
		void __fastcall MakeInterval(void);

		void __fastcall Allocate_Buffers(tri_info_t *st);

		void __fastcall Adj_MaxMin(void);
		void __fastcall CheckInterval_Boundary(void);
		uint16_t __fastcall GetInterval_Side(uint16_t RevLeds, uint16_t Off, uint8_t Flag);
		void __fastcall Match_Interval_Side(uint16_t RevLeds, uint16_t Off, intv_tri_t *Intvs, uint16_t Num);
		void __fastcall Match_Interval(void);
		void __fastcall Unclear_Bound_Interval_Expand(intv_tri_t **Intvs, uint8_t Num);

		void __fastcall Match_Bouble_Interval_Side(intv_tri_t *Intv, uint16_t Num, uint16_t StartID);
		void __fastcall Clear_Bound_Interval_Expand(intv_tri_t **Intvs, uint8_t Num);
};

extern BoundaryTri MtBoundTri;
#endif
