//---------------------------------------------------------------------------


#pragma hdrstop
#include <string.h>

#include "adprocess.h"
#include "Boundary.h"
#include "FrameInfo.h"
#include "Main.h"

//---------------------------------------------------------------------------
Boundary MtBound;

//获取凹坑
#define HIGH_NORMAL_THRES	220
#define DOWN_THRES	140
#define UP_THRES	140
#define LOW_NORMAL_THRES	100
#define BOTTOM_NORMAL_THRES	40

__fastcall Boundary::Boundary(void)
{
	OldAreaNum = 0;
	first_run = 0;
}

//环境光
void __fastcall Boundary::InitialUsualVolt(int SigVersion)
{
	memcpy(UsualVolt[0], AdBuf[0], FrameInfo.Info.Signals);

	if(SigVersion == REPORT_MODE_VERSION5) {
	memcpy(UsualVolt[TOTAL_AXIS], AdBuf[TOTAL_AXIS], XLEDS * X_CORNER_AXIS + YLEDS * Y_CORNER_AXIS);
	}
}

//最大值
void __fastcall Boundary::InitialMaxVolt(int SigVersion)
{
	memcpy(MaxVolt[0], AdBuf[0], FrameInfo.Info.Signals);

	if(SigVersion == REPORT_MODE_VERSION5) {
	memcpy(MaxVolt[TOTAL_AXIS], AdBuf[TOTAL_AXIS], XLEDS * X_CORNER_AXIS + YLEDS * Y_CORNER_AXIS);
	}
}

//最小值
void __fastcall Boundary::InitialMinVolt(int SigVersion)
{
	for(uint16_t i=0; i<FrameInfo.Info.Signals; i+=1){
		UsualVolt[0][i] = MaxVolt[0][i]>>2;         //that is 1/4
	}

	if(SigVersion == REPORT_MODE_VERSION5) {
		for(uint16_t i=0; i<XLEDS * X_CORNER_AXIS + YLEDS * Y_CORNER_AXIS; i+=1){
			UsualVolt[TOTAL_AXIS][i] = MaxVolt[TOTAL_AXIS][i]>>2;         //that is 1/4
		}
	}
}

//真实值
void __fastcall Boundary::GetAxisVolt(int SigVersion)
{
	memcpy(AxisAd[0], AdBuf[0], FrameInfo.Info.Signals);

	if(SigVersion == REPORT_MODE_VERSION5) {
		memcpy(AxisAd[TOTAL_AXIS], AdBuf[TOTAL_AXIS], XLEDS * X_CORNER_AXIS + YLEDS * Y_CORNER_AXIS);
	}
}

void __fastcall Boundary::Normal_CornerSignal(void)
{
	info_t *st = &FrameInfo.Info;

	for(int i=0; i<X_CORNER_AXIS; i+=1){
		for(int j=0; j<st->XOffsets[XAXIS-1]; j+=1) {
			NormalAD(i+TOTAL_AXIS, j);
		}
	}

	for(int i=0; i<X_CORNER_AXIS; i+=1){
		for(int j=0; j<st->YOffsets[YAXIS-1]; j+=1) {
			NormalAD(i+TOTAL_AXIS+X_CORNER_AXIS, j);
		}
	}
}

uint8_t __fastcall Boundary::NormalAD(int i, int j)
{
	uint8_t Normal;
	int iTemp;

	if(UsualVolt[i][j] > AxisAd[i][j])
	{
		UsualVolt[i][j] = AxisAd[i][j];
	}
//	else if(UsualVolt[i][j] < EAd[i]/2)
//	{
//    	UsualVolt[i][j] += 1;
//	}

	if(MaxVolt[i][j] < UsualVolt[i][j] + 10) 	//badled
	{
		Normal = 0;
	}
	else if(MaxVolt[i][j] <= AxisAd[i][j])  //((MaxVolt[i][j] > AxisAd[i][j] && MaxVolt[i][j] - AxisAd[i][j] < (MaxVolt[i][j]>>2)) ||
	{
		//MaxVolt[i][j] = AxisAd[i][j];
		Normal = 255;
//		if(AxisAd[i][j] > EAdMax[i])
//		{
//			MaxVolt[i][j] ++;
//		}
//		else if(AxisAd[i][j] > EAdMin[i])
//		{
//			MaxVolt[i][j] = AxisAd[i][j];
//		}
	}
	else if(MaxVolt[i][j] > UsualVolt[i][j])
	{
		iTemp = ((AxisAd[i][j]-UsualVolt[i][j])*256)/(MaxVolt[i][j] - UsualVolt[i][j]);

		if(iTemp > 255) Normal = 255;
		else if(iTemp <1) Normal = 1;
		else Normal = iTemp;

		
	}
	else                                  //MaxVolt[i][j]<UsualVolt[i][j]
	{
		//MaxVolt[i][j]=AxisAd[i][j];
		Normal = 255;
	}

//	if(Normal > DOWN_THRES)
//	{
//		if(AxisAd[i][j] < MaxVolt[i][j])
//			MaxVolt[i][j]--;
//		else
//			MaxVolt[i][j] =AxisAd[i][j];
//	}

	NormAd[i][j] = Normal;
	return Normal;
}

//const uint8_t NormalTable[256] = {
//	  0,   0,   0,   0,   0,   0,   0,   0,
//	  0,   0,   0,   0,   0,   0,   0,   0,
//	  0,   0,   0,   0,   0,   0,   0,   0,
//	  0,   0,   0,   0,   0,   0,   0,   0,
//	  0,   0,   0,   0,   0,   1,   1,   1,
//	  1,   1,   1,   1,   1,   1,   1,   1,
//	  1,   1,   1,   1,   2,   2,   2,   2,
//	  2,   2,   2,   2,   2,   2,   2,   2,
//	  3,   3,   3,   3,   3,   3,   3,   3,
//	  3,   4,   4,   4,   4,   4,   4,   4,
//	  4,   4,   5,   5,   5,   5,   5,   5,
//	  5,   5,   6,   6,   6,   6,   6,   6,
//	  6,   7,   7,   7,   7,   7,   7,   7,
//	  8,   8,   8,   8,   8,   8,   9,   9,
//	  9,   9,   9,   9,  10,  10,  10,  10,
//	 10,  11,  11,  11,  11,  11,  11,  12,
//	 12,  12,  12,  12,  13,  13,  13,  13,
//	 13,  14,  14,  14,  14,  14,  15,  15,
//	 15,  15,  16,  16,  16,  16,  16,  17,
//	 17,  17,  17,  18,  18,  18,  18,  19,
//	 19,  19,  19,  20,  20,  20,  20,  21,
//	 21,  21,  21,  22,  22,  22,  22,  23,
//	 23,  23,  23,  24,  24,  24,  24,  25,
//	 25,  25,  26,  26,  26,  26,  27,  27,
//	 27,  28,  28,  28,  28,  29,  29,  29,
//	 30,  30,  30,  31,  31,  31,  31,  32,
//	 32,  32,  33,  33,  33,  34,  34,  34,
//	 35,  35,  35,  36,  36,  36,  37,  37,
//	 37,  38,  38,  38,  39,  39,  39,  40,
//	 40,  40,  41,  41,  41,  42,  42,  43,
//	 43,  43,  44,  44,  44,  45,  45,  45,
//	 46,  46,  47,  47,  47,  48,  48,  49,
//};

#if SCALE == 64
const uint8_t InterpolationTable[256] = {
	  0,   0,   0,   0,   1,   1,   1,   1,
	  2,   2,   2,   2,   3,   3,   3,   3,
	  4,   4,   4,   4,   5,   5,   5,   5,
	  6,   6,   6,   6,   7,   7,   7,   7,
	  8,   8,   8,   8,   9,   9,   9,   9,
	 10,  10,  10,  10,  11,  11,  11,  11,
	 12,  12,  12,  12,  13,  13,  13,  13,
	 14,  14,  14,  14,  15,  15,  15,  15,
	 16,  16,  16,  16,  17,  17,  17,  17,
	 18,  18,  18,  18,  19,  19,  19,  19,
	 20,  20,  20,  20,  21,  21,  21,  21,
	 22,  22,  22,  22,  23,  23,  23,  23,
	 24,  24,  24,  24,  25,  25,  25,  25,
	 26,  26,  26,  26,  27,  27,  27,  27,
	 28,  28,  28,  28,  29,  29,  29,  29,
	 30,  30,  30,  30,  31,  31,  31,  31,
	 32,  32,  32,  32,  33,  33,  33,  33,
	 34,  34,  34,  34,  35,  35,  35,  35,
	 36,  36,  36,  36,  37,  37,  37,  37,
	 38,  38,  38,  38,  39,  39,  39,  39,
	 40,  40,  40,  40,  41,  41,  41,  41,
	 42,  42,  42,  42,  43,  43,  43,  43,
	 44,  44,  44,  44,  45,  45,  45,  45,
	 46,  46,  46,  46,  47,  47,  47,  47,
	 48,  48,  48,  48,  49,  49,  49,  49,
	 50,  50,  50,  50,  51,  51,  51,  51,
	 52,  52,  52,  52,  53,  53,  53,  53,
	 54,  54,  54,  54,  55,  55,  55,  55,
	 56,  56,  56,  56,  57,  57,  57,  57,
	 58,  58,  58,  58,  59,  59,  59,  59,
	 60,  60,  60,  60,  61,  61,  61,  61,
	 62,  62,  62,  62,  63,  63,  63,  64,
};
#elif SCALE == 32
const uint8_t InterpolationTable[256] = {
	  0>>1,   0>>1,   0>>1,   0>>1,   1>>1,   1>>1,   1>>1,   1>>1,
	  2>>1,   2>>1,   2>>1,   2>>1,   3>>1,   3>>1,   3>>1,   3>>1,
	  4>>1,   4>>1,   4>>1,   4>>1,   5>>1,   5>>1,   5>>1,   5>>1,
	  6>>1,   6>>1,   6>>1,   6>>1,   7>>1,   7>>1,   7>>1,   7>>1,
	  8>>1,   8>>1,   8>>1,   8>>1,   9>>1,   9>>1,   9>>1,   9>>1,
	 10>>1,  10>>1,  10>>1,  10>>1,  11>>1,  11>>1,  11>>1,  11>>1,
	 12>>1,  12>>1,  12>>1,  12>>1,  13>>1,  13>>1,  13>>1,  13>>1,
	 14>>1,  14>>1,  14>>1,  14>>1,  15>>1,  15>>1,  15>>1,  15>>1,
	 16>>1,  16>>1,  16>>1,  16>>1,  17>>1,  17>>1,  17>>1,  17>>1,
	 18>>1,  18>>1,  18>>1,  18>>1,  19>>1,  19>>1,  19>>1,  19>>1,
	 20>>1,  20>>1,  20>>1,  20>>1,  21>>1,  21>>1,  21>>1,  21>>1,
	 22>>1,  22>>1,  22>>1,  22>>1,  23>>1,  23>>1,  23>>1,  23>>1,
	 24>>1,  24>>1,  24>>1,  24>>1,  25>>1,  25>>1,  25>>1,  25>>1,
	 26>>1,  26>>1,  26>>1,  26>>1,  27>>1,  27>>1,  27>>1,  27>>1,
	 28>>1,  28>>1,  28>>1,  28>>1,  29>>1,  29>>1,  29>>1,  29>>1,
	 30>>1,  30>>1,  30>>1,  30>>1,  31>>1,  31>>1,  31>>1,  31>>1,
	 32>>1,  32>>1,  32>>1,  32>>1,  33>>1,  33>>1,  33>>1,  33>>1,
	 34>>1,  34>>1,  34>>1,  34>>1,  35>>1,  35>>1,  35>>1,  35>>1,
	 36>>1,  36>>1,  36>>1,  36>>1,  37>>1,  37>>1,  37>>1,  37>>1,
	 38>>1,  38>>1,  38>>1,  38>>1,  39>>1,  39>>1,  39>>1,  39>>1,
	 40>>1,  40>>1,  40>>1,  40>>1,  41>>1,  41>>1,  41>>1,  41>>1,
	 42>>1,  42>>1,  42>>1,  42>>1,  43>>1,  43>>1,  43>>1,  43>>1,
	 44>>1,  44>>1,  44>>1,  44>>1,  45>>1,  45>>1,  45>>1,  45>>1,
	 46>>1,  46>>1,  46>>1,  46>>1,  47>>1,  47>>1,  47>>1,  47>>1,
	 48>>1,  48>>1,  48>>1,  48>>1,  49>>1,  49>>1,  49>>1,  49>>1,
	 50>>1,  50>>1,  50>>1,  50>>1,  51>>1,  51>>1,  51>>1,  51>>1,
	 52>>1,  52>>1,  52>>1,  52>>1,  53>>1,  53>>1,  53>>1,  53>>1,
	 54>>1,  54>>1,  54>>1,  54>>1,  55>>1,  55>>1,  55>>1,  55>>1,
	 56>>1,  56>>1,  56>>1,  56>>1,  57>>1,  57>>1,  57>>1,  57>>1,
	 58>>1,  58>>1,  58>>1,  58>>1,  59>>1,  59>>1,  59>>1,  59>>1,
	 60>>1,  60>>1,  60>>1,  60>>1,  61>>1,  61>>1,  61>>1,  61>>1,
	 62>>1,  62>>1,  62>>1,  62>>1,  63>>1,  63>>1,  63>>1,  64>>1,
};
#elif SCALE == 16
const uint8_t InterpolationTable[256] = {
	  0>>2,   0>>2,   0>>2,   0>>2,   1>>2,   1>>2,   1>>2,   1>>2,
	  2>>2,   2>>2,   2>>2,   2>>2,   3>>2,   3>>2,   3>>2,   3>>2,
	  4>>2,   4>>2,   4>>2,   4>>2,   5>>2,   5>>2,   5>>2,   5>>2,
	  6>>2,   6>>2,   6>>2,   6>>2,   7>>2,   7>>2,   7>>2,   7>>2,
	  8>>2,   8>>2,   8>>2,   8>>2,   9>>2,   9>>2,   9>>2,   9>>2,
	 10>>2,  10>>2,  10>>2,  10>>2,  11>>2,  11>>2,  11>>2,  11>>2,
	 12>>2,  12>>2,  12>>2,  12>>2,  13>>2,  13>>2,  13>>2,  13>>2,
	 14>>2,  14>>2,  14>>2,  14>>2,  15>>2,  15>>2,  15>>2,  15>>2,
	 16>>2,  16>>2,  16>>2,  16>>2,  17>>2,  17>>2,  17>>2,  17>>2,
	 18>>2,  18>>2,  18>>2,  18>>2,  19>>2,  19>>2,  19>>2,  19>>2,
	 20>>2,  20>>2,  20>>2,  20>>2,  21>>2,  21>>2,  21>>2,  21>>2,
	 22>>2,  22>>2,  22>>2,  22>>2,  23>>2,  23>>2,  23>>2,  23>>2,
	 24>>2,  24>>2,  24>>2,  24>>2,  25>>2,  25>>2,  25>>2,  25>>2,
	 26>>2,  26>>2,  26>>2,  26>>2,  27>>2,  27>>2,  27>>2,  27>>2,
	 28>>2,  28>>2,  28>>2,  28>>2,  29>>2,  29>>2,  29>>2,  29>>2,
	 30>>2,  30>>2,  30>>2,  30>>2,  31>>2,  31>>2,  31>>2,  31>>2,
	 32>>2,  32>>2,  32>>2,  32>>2,  33>>2,  33>>2,  33>>2,  33>>2,
	 34>>2,  34>>2,  34>>2,  34>>2,  35>>2,  35>>2,  35>>2,  35>>2,
	 36>>2,  36>>2,  36>>2,  36>>2,  37>>2,  37>>2,  37>>2,  37>>2,
	 38>>2,  38>>2,  38>>2,  38>>2,  39>>2,  39>>2,  39>>2,  39>>2,
	 40>>2,  40>>2,  40>>2,  40>>2,  41>>2,  41>>2,  41>>2,  41>>2,
	 42>>2,  42>>2,  42>>2,  42>>2,  43>>2,  43>>2,  43>>2,  43>>2,
	 44>>2,  44>>2,  44>>2,  44>>2,  45>>2,  45>>2,  45>>2,  45>>2,
	 46>>2,  46>>2,  46>>2,  46>>2,  47>>2,  47>>2,  47>>2,  47>>2,
	 48>>2,  48>>2,  48>>2,  48>>2,  49>>2,  49>>2,  49>>2,  49>>2,
	 50>>2,  50>>2,  50>>2,  50>>2,  51>>2,  51>>2,  51>>2,  51>>2,
	 52>>2,  52>>2,  52>>2,  52>>2,  53>>2,  53>>2,  53>>2,  53>>2,
	 54>>2,  54>>2,  54>>2,  54>>2,  55>>2,  55>>2,  55>>2,  55>>2,
	 56>>2,  56>>2,  56>>2,  56>>2,  57>>2,  57>>2,  57>>2,  57>>2,
	 58>>2,  58>>2,  58>>2,  58>>2,  59>>2,  59>>2,  59>>2,  59>>2,
	 60>>2,  60>>2,  60>>2,  60>>2,  61>>2,  61>>2,  61>>2,  61>>2,
	 62>>2,  62>>2,  62>>2,  62>>2,  63>>2,  63>>2,  63>>2,  64>>2,
};
#endif

void __fastcall Boundary::GetInterval(void)
{
	int start;
	int stop;
	int i;
	uint16_t j;
	info_t *st = &FrameInfo.Info;
	uint8_t Normal;
	uint8_t Down;

	TotalIntvs = 0;
	memset(Intervals, 0, sizeof(Intervals));

	for(i = 0; i<TOTAL_AXIS; i++)
	{
		Down = 0;
		Intervals[i].Num = 0;									//凹坑数量归零

		if(i < XAXIS)                                           //fix me 查表
		{
			start = 0 - st->XOffsets[i];
			stop = XLEDS - st->XOffsets[i];

			if(start < 0) start = 0;
			if(stop > XLEDS) stop = XLEDS;
		}
		else
		{
			if(st->YLeds == 0) { 
				break;
			}

			start = 0 - st->YOffsets[i-XAXIS];
			stop = YLEDS - st->YOffsets[i-XAXIS];

			if(start < 0) start = 0;
			if(stop > YLEDS) stop = YLEDS;
		}

		for(j=start; j<stop; j++)			 //获得y轴凹坑
		{
//			if(MaxVolt[i][j] < UsualVolt[i][j] + 20)
//			{
//				if(StartBadLed == -1)
//				{
//					StartBadLed = j;
//					BadLeds = 1;
//				}
//				else            //连续坏灯
//				{
//					BadLeds += 1;
//				}
//
//				continue;
//			}
			
			Normal = NormalAD(i, j);

			if(!Down && Normal < DOWN_THRES)			//记录下降沿
			{
				Down = 1;
////Ag 1
//				if(LastNormal < HIGH_NORMAL_THRES)
//				{
//					Intervals[i].Section[Intervals[i].Num].Begin = j * SCALE - InterpolationTable[255-LastNormal];
//				}
//				else
				{
					Intervals[i].Section[Intervals[i].Num].Begin = j * SCALE + InterpolationTable[Normal];
				}


////Ag2
//				if(Normal > LOW_NORMAL_THRES && LastNormal>HIGH_NORMAL_THRES)
//				{
//					Intervals[i].Section[Intervals[i].Num].Begin = j * SCALE + InterpolationTable[Normal]/2;
//				}
//				else
//				{
//					Intervals[i].Section[Intervals[i].Num].Begin = (j-1) * SCALE +  InterpolationTable[LastNormal]/2;
//				}
//
//				if(Intervals[i].Section[Intervals[i].Num].Begin < LastEnd)
//					Intervals[i].Section[Intervals[i].Num].Begin = LastEnd;

//Ag3
//				if(LastNormal < HIGH_NORMAL_THRES)
//				{
//					Intervals[i].Section[Intervals[i].Num].Begin = (j-1) * SCALE +  InterpolationTable[LastNormal]/2;
//				}
//				else if(Normal > LOW_NORMAL_THRES)
//				{
//					Intervals[i].Section[Intervals[i].Num].Begin = j * SCALE + InterpolationTable[Normal]/2;
//				}
//				else
//				{
//					Intervals[i].Section[Intervals[i].Num].Begin = j * SCALE;
//				}
//
//				if(Intervals[i].Section[Intervals[i].Num].Begin < LastEnd)
//					Intervals[i].Section[Intervals[i].Num].Begin = LastEnd;
			}
			else if(Down && Normal > UP_THRES)
			{
				Down = 0;

//				if(Normal > HIGH_NORMAL_THRES)
//				{
//					Intervals[i].Section[Intervals[i].Num].End = j * SCALE - InterpolationTable[LastNormal];
//				}
//				else
				{
					Intervals[i].Section[Intervals[i].Num].End = j * SCALE + InterpolationTable[255 - Normal];
				}

				if(Intervals[i].Section[Intervals[i].Num].End > Intervals[i].Section[Intervals[i].Num].Begin)
				{
					Intervals[i].Num += 1;
					TotalIntvs += 1;
				}

//				if(LastNormal > LOW_NORMAL_THRES && Normal > HIGH_NORMAL_THRES)
//				{
//					Intervals[i].Section[Intervals[i].Num].End = j * SCALE - InterpolationTable[LastNormal]/2;
//				}
//				else
//				{
//					Intervals[i].Section[Intervals[i].Num].End = (j+1) * SCALE - InterpolationTable[Normal]/2;
//				}
//
//				LastEnd = Intervals[i].Section[Intervals[i].Num].End;
//				Intervals[i].Num += 1;


//				if(Normal < HIGH_NORMAL_THRES)
//				{
//					Intervals[i].Section[Intervals[i].Num].End = (j+1) * SCALE - InterpolationTable[Normal]/2;
//				}
//				else if(LastNormal > LOW_NORMAL_THRES)
//				{
//					Intervals[i].Section[Intervals[i].Num].End = j * SCALE - InterpolationTable[LastNormal]/2;
//				}
//				else
//				{
//					Intervals[i].Section[Intervals[i].Num].End = j * SCALE;
//				}
//
//				LastEnd = Intervals[i].Section[Intervals[i].Num].End;
//				Intervals[i].Num += 1;
			}

			if((Intervals[i].Num >= MAX_INTERVALS) || ((i==0 || i==XAXIS ) && Intervals[i].Num >= MAX_CLC_POINTS))
			{
				break;
			}

			//LastNormal = Normal;
		}

		if(Down)
		{
			Intervals[i].Section[Intervals[i].Num].End = stop * SCALE - InterpolationTable[Normal];
			//Intervals[i].Section[Intervals[i].Num].End = (stop) * SCALE - InterpolationTable[Normal]/2;

			if(Intervals[i].Section[Intervals[i].Num].End > Intervals[i].Section[Intervals[i].Num].Begin)
			{
				Intervals[i].Num += 1;
				TotalIntvs += 1;
			}
		}
	 }

	 if (st->YLeds == 0) {
		 if(Intervals[0].Num != 0)
		 {
			 Intervals[XAXIS].Num = 1;
			 Intervals[XAXIS].Section[0].Begin = 0;
			 Intervals[XAXIS].Section[0].End = MAX_Y_SCALE - 1;
		 }
		 else
		 {
			 Intervals[XAXIS].Num = 0;
		 }

		 for(int i=1; i<YAXIS; i+=1)
		 {
			 Intervals[XAXIS+i].Num = 0;
		 }

		 uint32_t MaxIntvNum = 0;

		 for(int i=0; i<XAXIS; i+=1)
		 {
			if((uint32_t)(Intervals[i].Num) > MaxIntvNum)
			{
				MaxIntvNum = Intervals[i].Num;
			}
		 }

		 Cury = 0;

		 if(MaxIntvNum > 1 && Intervals[0].Num == 1)     //扫描得到的x轴凹坑处于合并状态
		 {
			//X轴 拆分
			int xm1, xm2, ym1, ym2;
			float *CoTan; //*Tan,
			//int xm;
			int height;

			//Tan = FrameInfo.Info.Tan;
			CoTan = FrameInfo.Info.CoTan;

			for(int i=XAXIS-1; i>0; i-=2)
			{
				if(Intervals[i].Num + Intervals[i-1].Num == 3)
				{
					//xm = (Intervals[0].Section[0].Begin + Intervals[0].Section[0].End)/2;

					if(Intervals[i].Num == 1)
					{
						xm1 = ((Intervals[i].Section[0].Begin + Intervals[i].Section[0].End)/2 +
								   (Intervals[i-1].Section[0].Begin + Intervals[i-1].Section[0].End)/2 ) / 2;

						xm2 = ((Intervals[i].Section[0].Begin + Intervals[i].Section[0].End)/2 +
								   (Intervals[i-1].Section[1].Begin + Intervals[i-1].Section[1].End)/2 ) / 2;

						ym1 = (Intervals[i-1].Section[0].Begin + Intervals[i-1].Section[0].End)/2;
						ym1 = MAX_Y_COOD - (xm1 - ym1)*CoTan[i-1];

						ym2 = (Intervals[i-1].Section[1].Begin + Intervals[i-1].Section[1].End)/2;
						ym2 = MAX_Y_COOD - (xm2 - ym2)*CoTan[i-1];
					}
					else
					{
						xm1 = ((Intervals[i].Section[0].Begin + Intervals[i].Section[0].End)/2 +
								   (Intervals[i-1].Section[0].Begin + Intervals[i-1].Section[0].End)/2 ) / 2;

						xm2 = ((Intervals[i].Section[1].Begin + Intervals[i].Section[1].End)/2 +
								   (Intervals[i-1].Section[0].Begin + Intervals[i-1].Section[0].End)/2 ) / 2;

						ym1 = (Intervals[i].Section[0].Begin + Intervals[i].Section[0].End)/2;
						ym1 = MAX_Y_COOD - (xm1 - ym1)*CoTan[i];

						ym2 = (Intervals[i].Section[1].Begin + Intervals[i].Section[1].End)/2;
						ym2 = MAX_Y_COOD - (xm2 - ym2)*CoTan[i];
					}
				}
				else if(Intervals[i].Num + Intervals[i-1].Num == 4)
				{
					xm1 = ((Intervals[i].Section[0].Begin + Intervals[i].Section[0].End)/2 +
								   (Intervals[i-1].Section[1].Begin + Intervals[i-1].Section[1].End)/2 ) / 2;

					xm2 = ((Intervals[i].Section[1].Begin + Intervals[i].Section[1].End)/2 +
							   (Intervals[i-1].Section[0].Begin + Intervals[i-1].Section[0].End)/2 ) / 2;

					ym1 = (Intervals[i].Section[0].Begin + Intervals[i].Section[0].End)/2;
					ym1 = MAX_Y_COOD - (xm1 - ym1)*CoTan[i];

					ym2 = (Intervals[i].Section[1].Begin + Intervals[i].Section[1].End)/2;
					ym2 = MAX_Y_COOD - (xm2 - ym2)*CoTan[i];
				}
				else
				{
					continue;
				}

				if(ym1 > ym2)
				{
					ym1 = ym1 ^ ym2;
					ym2 = ym1 ^ ym2;
					ym1 = ym1 ^ ym2;
				}

				Intervals[XAXIS].Num = 2;

				height = (ym1+ym2)/2 - ym1;
				if(height > ym1 - MIN_Y_COOD)
				{
					height = ym1 - MIN_Y_COOD;
				}

				Intervals[XAXIS].Section[0].End = ym1+height;
				Intervals[XAXIS].Section[0].Begin = ym1-height;


				height = (ym2 - (ym1+ym2)/2);
				if(height > MAX_Y_COOD - ym2)
				{
					height = MAX_Y_COOD - ym2;
				}
				Intervals[XAXIS].Section[1].Begin = ym2-height;
				Intervals[XAXIS].Section[1].End = ym2+height;

				break;
			}
		 }
		 else if(MaxIntvNum == 1)		//单点
		 {
			int xm, x, y, height;
			int Max = 0, Min = 0x7FFFFFFF;
			float *CoTan;            //*Tan,
			int Num = 0;

			//Tan = ScreenInfo.Tan;
			CoTan = FrameInfo.Info.CoTan;

			xm = (Intervals[0].Section[0].Begin + Intervals[0].Section[0].End)/2;
			height = Intervals[0].Section[0].End - Intervals[0].Section[0].Begin;
			height /= 2;

			//缩短Y轴
			for(int i=XAXIS-1; i>0; i-=1)
			{
				if(Intervals[i].Num == 1)
				{
					x = (Intervals[i].Section[0].Begin + Intervals[i].Section[0].End)/2;
					y = MAX_Y_COOD - (xm - x) * CoTan[i];

					if(y < MIN_Y_COOD || y > MAX_Y_COOD)
						continue;

					Num += 1;
					Cury += y;

					if(y>Max) Max = y;
					if(y<Min) Min = y;
				}
			}

			if(Num > 2)
			{
				if(Max > 0)	{Cury -= Max; Num--;}
				if(Min < 0x7FFFFFFF) {Cury -= Min; Num--;}
			}

			if(Num > 0)
			{
				Cury /= Num;

				Intervals[XAXIS].Num = 1;

				Intervals[XAXIS].Section[0].End = Cury + height;
				Intervals[XAXIS].Section[0].Begin = Cury - height;
			}
		 }
	 }

	 //Find_Max_Num_Interval();
	 //Interval_Match_Main();
}

void __fastcall Boundary::Expand_Interval(void)
{
	uint16_t i, j;
	interval_t *itv;

	for(i=0; i<TOTAL_AXIS; i+=1)
	{
		itv = &Intervals[i];

		for(j=0; j<itv->Num; j+=1)
		{
			if(itv->Section[j].End - itv->Section[j].Begin < 2*SCALE)
			{
				itv->Section[j].End += SCALE/2;
				itv->Section[j].Begin -= SCALE/2;
				
				if(j+1<itv->Num && itv->Section[j].End > itv->Section[j+1].Begin)
				{
					itv->Section[j].End -= SCALE/2;
				}
				
				if(j>0 && itv->Section[j].Begin < itv->Section[j-1].End)
				{
					itv->Section[j].Begin += SCALE/2;
				}
					
				if(i == 0)	//Main x
				{
					if(itv->Section[j].Begin < 0)
					{
						itv->Section[j].Begin = 0;
					}

					if(itv->Section[j].End > MAX_X_SCALE)
					{
						itv->Section[j].End = MAX_X_SCALE;
					}
				}
				else if(i == XAXIS) //Main y
				{
					if(itv->Section[j].Begin < 0)
					{
						itv->Section[j].Begin = 0;
					}
					
					if(itv->Section[j].End > MAX_Y_SCALE)
					{
						itv->Section[j].End = MAX_Y_SCALE;
					}
				}
			}
		}
	}
}

//扩展添加默认凹坑
void __fastcall Boundary::AddDefaultInterval(void)
{
	uint16_t Num;
	int Start, End, SExp, EExp, Offset, Leds;
	info_t *st = &FrameInfo.Info;

	for(int i=0; i<TOTAL_AXIS; i+=1)
	{
		if(i<XAXIS && st->XOffsets[i] == 0)
			continue;
		else if(i>=XAXIS && st->YOffsets[i-XAXIS] == 0)
			continue;

		if((Num = Intervals[i].Num) == 0)	continue;

		if(i<XAXIS)
		{
			Offset = st->XOffsets[i];
			Leds = XLEDS;
		}
		else
		{
			Offset = st->YOffsets[i - XAXIS];
			Leds = YLEDS;
		}

		if(Offset < 0)
		{
			Start = -Offset * SCALE + SCALE;
			End = Leds * SCALE - SCALE;
			SExp = 0;
			EExp =  (Leds - Offset) * SCALE;
		}
		else
		{
			Start = 1 * SCALE;
			End = (Leds - Offset) * SCALE - SCALE;
			SExp = -Offset * SCALE;
			EExp =  Leds * SCALE;
		}

		if(Intervals[i].Section[0].Begin < Start)                   //扩展
		{
			Intervals[i].Section[0].Begin = SExp;
		}
		else                                                        //添加
		{
			for(int k=Intervals[i].Num; k>0; k-=1)
			{
				Intervals[i].Section[k] = Intervals[i].Section[k-1];
			}
			Intervals[i].Section[0].Begin = SExp;
			Intervals[i].Section[0].End = Start - 1 * SCALE;
			Intervals[i].Num += 1;
			Num += 1;
		}

		if(Intervals[i].Section[Num - 1].End > End)           //扩展
		{
			Intervals[i].Section[Num - 1].End = EExp;
		}                                                      //添加
		else
		{
			Intervals[i].Section[Intervals[i].Num].Begin = End + 1 * SCALE;
			Intervals[i].Section[Intervals[i].Num].End = EExp;
			Intervals[i].Num += 1;
		}
	}
}

void __fastcall Boundary::Allocate_Buffers(void)
{
	uint32_t ValidADs;
	uint32_t Off;

	memset(MemPool, 0, sizeof(MemPool));
	ValidADs = XLEDS * XAXIS + YLEDS * YAXIS;

	for(int i=0; i<TOTAL_AXIS; i+=1)
	{
		if(i<XAXIS)
			Off = i*XLEDS;
		else
			Off = XLEDS * XAXIS + (i-XAXIS)*YLEDS;

		AdBuf[i] = MemPool + Off;
		AxisAd[i] = MemPool + ValidADs + Off;
		UsualVolt[i] = MemPool + ValidADs*2 + Off;
		MaxVolt[i] = MemPool + ValidADs*3 + Off;
		NormAd[i] = MemPool + ValidADs*4 + Off;
		BadLeds[i] = MemPool + ValidADs*5 + Off;
	}


	//corner signals
	uint8_t *CornerPool = MemPool + ValidADs*6;
	ValidADs =  XLEDS * X_CORNER_AXIS  +  YLEDS * Y_CORNER_AXIS;   //more than enough

	for(int i=0; i<CORNER_AXIS; i+=1) {
		if(i<X_CORNER_AXIS)  {
			Off = i*XLEDS;
		} else {
			Off = XLEDS * X_CORNER_AXIS + (i - X_CORNER_AXIS)*YLEDS;
		}

		AdBuf[i+TOTAL_AXIS] = CornerPool + Off;
		AxisAd[i+TOTAL_AXIS] = CornerPool + ValidADs + Off;
		UsualVolt[i+TOTAL_AXIS] = CornerPool + ValidADs*2 + Off;
		MaxVolt[i+TOTAL_AXIS] = CornerPool + ValidADs*3 + Off;
		NormAd[i+TOTAL_AXIS] = CornerPool + ValidADs*4 + Off;
		BadLeds[i+TOTAL_AXIS] = CornerPool + ValidADs*5 + Off;
	}
}

void __fastcall Boundary::GetAuxInterval(void)  //同时添加了默认凹坑
{
	int start;
	int stop;
	int i;
	uint16_t j;
	info_t *st = &FrameInfo.Info;
	uint8_t Normal;
	uint8_t LastNormal;
	uint8_t Down;
	//int16_t LastEnd;
	int offset,tmp,mLEDS;    //MarkStart,MarkEnd,

	for(i = 0; i<TOTAL_AXIS; i++)
	{
		//LastEnd = 0;
		Down = 0;
		LastNormal = 255;
		AuxIntervals[i].Num = 0;
		//MarkStart = 0;
		//MarkEnd = 0;

		if( i == 0 || i == XAXIS)
			continue;									//凹坑数量归零

		if(i >= XAXIS && st->YLeds == 0) {
        	break;
		}

		offset = st->Offsets[i];

		if(i < XAXIS)                                           //fix me 查表
			mLEDS = XLEDS;
		else
			mLEDS = YLEDS;

		if (offset < 0)
		{
			start = 0;
			stop = -offset;
			offset = - offset;
		}
		else
		{
			start = mLEDS - offset;
			stop = mLEDS;
		}

		for(j=start; j<stop; j++)			 //获得凹坑
		{
			Normal = NormalAD(i, j);

			if(!Down && Normal < DOWN_THRES)			//记录下降沿
			{
				Down = 1;
////Ag 1
				if (j == start)
				{
					if(start == 0) tmp = start - offset;
					else tmp = 0;
					AuxIntervals[i].Section[AuxIntervals[i].Num].Begin = tmp * SCALE;
					//MarkStart = 1;
				}
				else if(LastNormal < HIGH_NORMAL_THRES)
				{
					AuxIntervals[i].Section[AuxIntervals[i].Num].Begin = j * SCALE - InterpolationTable[255-LastNormal];
				}
				else
				{
					AuxIntervals[i].Section[AuxIntervals[i].Num].Begin = j * SCALE + InterpolationTable[Normal];
				}
			}
			else if(Down && Normal > UP_THRES)
			{
				Down = 0;

				if(Normal > HIGH_NORMAL_THRES)
				{
					AuxIntervals[i].Section[AuxIntervals[i].Num].End = j * SCALE - InterpolationTable[LastNormal];
				}
				else
				{
					AuxIntervals[i].Section[AuxIntervals[i].Num].End = j * SCALE + InterpolationTable[255 - Normal];
				}

				if(AuxIntervals[i].Section[AuxIntervals[i].Num].End > AuxIntervals[i].Section[AuxIntervals[i].Num].Begin)
				{
					AuxIntervals[i].Num += 1;
				}

			}

			if((AuxIntervals[i].Num >= MAX_INTERVALS) || ((i==0 || i==XAXIS ) && AuxIntervals[i].Num >= MAX_CLC_POINTS))
			{
				break;
			}

			LastNormal = Normal;
		}

		if(Down)
		{
			if(stop == mLEDS) tmp = stop + offset;
			else tmp = mLEDS;
			AuxIntervals[i].Section[AuxIntervals[i].Num].End = tmp * SCALE - InterpolationTable[Normal];
			//MarkEnd = 1;
			if(AuxIntervals[i].Section[AuxIntervals[i].Num].End > AuxIntervals[i].Section[AuxIntervals[i].Num].Begin)
			{
				AuxIntervals[i].Num += 1;
			}
		}

//		if(AuxIntervals[i].Num >0)       //add default intervals
//		{
//			 if(MarkStart == 0)
//			 {
//				for(int k = AuxIntervals[i].Num; k>0; k-=1)
//				{
//					AuxIntervals[i].Section[k] = AuxIntervals[i].Section[k-1];
//				}
//				if(start == 0) tmp = start - offset;
//				else tmp = 0;
//				AuxIntervals[i].Section[0].Begin = tmp * SCALE;
//				AuxIntervals[i].Section[0].End = start* SCALE;
//				AuxIntervals[i].Num += 1;
//			 }
//			 if (MarkEnd == 0)
//			 {
//				if(stop == mLEDS) tmp = stop + offset;
//				else tmp = mLEDS;
//				AuxIntervals[i].Section[AuxIntervals[i].Num].Begin = stop * SCALE;
//				AuxIntervals[i].Section[AuxIntervals[i].Num].End = tmp * SCALE;
//				AuxIntervals[i].Num += 1;
//			 }
//		 }
	 }


}

void __fastcall Boundary::SignalQuality(int SigVersion)
{
	int i,j;
	double Variance;
	double Average;
	double AdSum;
	info_t *it = &FrameInfo.Info;
	int Axis = TOTAL_AXIS;

	if(SigVersion == REPORT_MODE_VERSION5) {
		Axis += X_CORNER_AXIS + Y_CORNER_AXIS;
	}

	for(i=0; i<Axis; i+=1)
	{
		int16_t Off = -it->Offsets[i];
		int16_t Start = Off;
		int16_t End;
		int16_t Leds;

		if(i<XAXIS){
			Leds = XLEDS;
			End = Leds + Off;
		} else if(i<XAXIS + YAXIS) {
			Leds = YLEDS;
			End = Leds + Off;
		} else if(i<XAXIS + YAXIS + X_CORNER_AXIS) {
			Leds = XLEDS;
			Start = 0;
			End = it->Offsets[XAXIS-1];
			Off = 0;
		} else {
			Leds = YLEDS;
			Start = 0;
			End = it->Offsets[XAXIS+YAXIS-1];
			Off = 0;
		}

		if(Start < 0) Start = 0;
		if(End > Leds) End = Leds;

		if(End-Start == 0) continue;
        //-------------------------------------------------
		//Maxim AD Value
		//-------------------------------------------------
		Average = 0;
		for(int j=Start; j<End; j+=1)
		{
			Average += MaxVolt[i][j];
		}

		SigQ[i].EMax = Average/(End-Start);

		//-------------------------------------------------
		//Current AD Value
		//-------------------------------------------------
		Average = 0;
		for(int j=Start; j<End; j+=1)
		{
			Average += AxisAd[i][j];
		}

		Average = Average/(End-Start);

		//-----------------------------------
		Variance = 0;

		for(int j=Start; j<End; j+=1)
		{
			double ftemp = (double)AxisAd[i][j] - Average;
			Variance += ftemp * ftemp;
		}

		Variance = Variance/(End-Start);

		SigQ[i].EAd = Average;
		SigQ[i].DAd = Variance;

		EAd[i] = Average;

		if(Average + Average/4 > 255)
		{
			EAdMax[i] = 255;
		}
		else
		{
			EAdMax[i] = Average + Average/4;
		}

		EAdMin[i] = Average - Average/4;
	}

//	for(i=0; i<st->YAxis; i+=1)
//	{
//		AdSum = 0;
//		for(int j=0; j<st->YLeds; j+=1)
//		{
//			AdSum += AxisAd[i+st->XAxis][j];
//		}
//
//		EAd[i+st->XAxis] = AdSum/st->YLeds;
//	}
}

void __fastcall Boundary::Adj_WithOutTouch(void)
{

}

void __fastcall Boundary::Adj_MaxMin(int SigVersion)
{
	uint32_t Len;
	int Axis = TOTAL_AXIS;

	if(SigVersion == REPORT_MODE_VERSION5) {
		Axis += (X_CORNER_AXIS + Y_CORNER_AXIS);
	}

	for(int i=0; i<Axis; i+=1) {
		int16_t Start;
		int16_t End;
		int16_t Leds;

		if(i >= XAXIS && FrameInfo.Info.YLeds == 0) {
			break;
		}
		
		if(i<XAXIS)
			Leds = XLEDS;
		else if(i<TOTAL_AXIS)
			Leds = YLEDS;
		else if(i<TOTAL_AXIS + X_CORNER_AXIS)
			Leds = XLEDS;
		else
        	Leds = YLEDS;

		//End = Leds + Off;

		//if(Start < 0) Start = 0;
		//if(End > Leds) End = Leds;

		for(int j=0; j<Leds; j+=1) {
			//Adj Max voltage
			if(AxisAd[i][j] > MaxVolt[i][j]) {
				//if(AxisAd[i][j] < EAdMax[i])
				{
					MaxVolt[i][j] = AxisAd[i][j];
				}
				//else
				{
				//	MaxVolt[i][j] ++;
				}
			} else if(AxisAd[i][j] < MaxVolt[i][j] ) {
				if(AxisAd[i][j] > EAd[i]) {
					MaxVolt[i][j] = AxisAd[i][j];
				} else if (AxisAd[i][j] > EAdMin[i]) {
					MaxVolt[i][j] --;
				} else if (AxisAd[i][j] > UsualVolt[i][j] + 20) {
					MaxVolt[i][j] --;
				}
			}
		}
	}
}

//---------------------------------------------------------------------------
//Function:
//Param:
//Return:
//Note:
//---------------------------------------------------------------------------
bool __fastcall Boundary::IsEmit(uint16_t LedPos, uint8_t EAd, uint8_t PCBPos)
{
	int EmitLed, RevLed;
	int BadCount = 0;
	int ValidCount = 0;
	info_t *st = &FrameInfo.Info;

	EmitLed = LedPos;       //switch emit only fix me !!!!!!!!!!!!

	if(PCBPos == PCB_POS_X)
	{
		for(int j=0; j<XAXIS; j+=1)
		{
			RevLed = EmitLed - st->XOffsets[j];
			if(RevLed<0 || RevLed >= st->XLeds)
				continue;

			ValidCount += 1;

			if(MtBound.AxisAd[j][RevLed] < EAd/2)
			{
				BadCount += 1;
			}
		}

		if(BadCount == ValidCount)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		for(int j=0; j<YAXIS; j+=1)
		{
			RevLed = EmitLed - st->YOffsets[j];
			if(RevLed<0 || RevLed >= st->YLeds)
				continue;

			ValidCount += 1;

			if(MtBound.AxisAd[j+XAXIS][RevLed] < EAd/2)
			{
				BadCount += 1;
			}
		}

		if(BadCount == ValidCount)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

//---------------------------------------------------------------------------
//Function:
//Param:
//Return:
//Note:
//---------------------------------------------------------------------------
bool __fastcall Boundary::IsRev(uint16_t LedPos, uint8_t EAd, uint8_t PCBPos)
{
	int EmitLed, RevLed;
	int BadCount = 0;
	int ValidCount = 0;
	info_t *st = &FrameInfo.Info;

	if(PCBPos == PCB_POS_X)
	{
		for(int j=0; j<XAXIS; j+=1)
		{
			RevLed = LedPos;       //switch emit only!!!!!!! fix me !!!!!!!!!!!!
			EmitLed = RevLed + st->XOffsets[j];
			if(EmitLed<0 || EmitLed >= st->XLeds)
				continue;

			ValidCount += 1;

			if(MtBound.AxisAd[j][RevLed] < EAd/2)
			{
				BadCount += 1;
			}
		}

		if(BadCount == ValidCount)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		for(int j=0; j<YAXIS; j+=1)
		{
			RevLed = LedPos;       //switch emit only fix me !!!!!!!!!!!!
			EmitLed = RevLed + st->YOffsets[j];
			if(EmitLed<0 || EmitLed >= st->YLeds)
				continue;

			ValidCount += 1;

			if(MtBound.AxisAd[j+XAXIS][RevLed] < EAd/2)
			{
				BadCount += 1;
			}
		}

		if(BadCount == ValidCount)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

//---------------------------------------------------------------------------
//Function:
//Param:
//Return:
//Note:
//---------------------------------------------------------------------------
void __fastcall Boundary::BadLed_Check2(void)
{
	int i, j;
	info_t *st = &FrameInfo.Info;

	BadLedNum = 0;

	for(i=0; i<XLEDS; i+=1)
	{
		if(MaxVolt[0][i] < UsualVolt[0][i] + 20) {
			//check emit
			for(j=0; j<XAXIS; j+=1){
				int pos = i - st->XOffsets[j];

				if(pos<0 || pos>= XLEDS)	continue;

				if(MaxVolt[j][pos] > UsualVolt[j][pos] + 20){
					break;
				}
			}

			if(j >= XAXIS){	//	bad emit led
				BadLedNo[BadLedNum ++] = BADLED_EX | i;
				if(BadLedNum >= MAX_BADLEDS)	return;
			}

			for(j=0; j<XAXIS; j+=1){
				int pos = i + st->XOffsets[j];

				if(pos<0 || pos>= XLEDS)	continue;

				if(MaxVolt[j][pos] > UsualVolt[j][pos] + 20){
					break;
				}
			}

			if(j >= XAXIS){	//	bad emit led
				BadLedNo[BadLedNum ++] = BADLED_RX | i;
				if(BadLedNum >= MAX_BADLEDS)	return;
			}
		}
	}

	for(i=0; i<YLEDS; i+=1)
	{
		if(MaxVolt[XAXIS][i] < UsualVolt[XAXIS][i] + 20) {
			//check emit
			for(j=0; j<YAXIS; j+=1){
				int pos = i - st->YOffsets[j];

				if(pos<0 || pos>= YLEDS)	continue;

				if(MaxVolt[j+XAXIS][pos] > UsualVolt[j+XAXIS][pos] + 20){
					break;
				}
			}

			if(j >= YAXIS){	//	bad emit led
				BadLedNo[BadLedNum ++] = BADLED_EY | i;
				if(BadLedNum >= MAX_BADLEDS)	return;
			}

			//check rev
			for(j=0; j<YAXIS; j+=1){
				int pos = i + st->YOffsets[j];

				if(pos<0 || pos>= YLEDS)	continue;

				if(MaxVolt[j+XAXIS][pos] > UsualVolt[j+XAXIS][pos] + 20){
					break;
				}
			}

			if(j >= YAXIS){	//	bad emit led
				BadLedNo[BadLedNum ++] = BADLED_RY | i;
				if(BadLedNum >= MAX_BADLEDS)	return;
			}
		}
	}
}

//---------------------------------------------------------------------------
//Function:
//Param:
//Return:
//Note:
//---------------------------------------------------------------------------
bool __fastcall Boundary::BadLed_Check(void)
{
	uint32_t Len;
	int i, j;
	bool ret = false;
	info_t *st = &FrameInfo.Info;

	Len = XAXIS*XLEDS + YAXIS*YLEDS;

	memset(BadLeds[0], 0x00, Len);

	for(i=0; i<XLEDS; i+=1)
	{
		if(AxisAd[0][i] < EAd[0]/3 || AxisAd[0][i]<20 || MaxVolt[0][i] < EAd[0]/2)
		{
			if(IsEmit(i, EAd[0], PCB_POS_X))
			{
				for(j=0; j<XAXIS; j+=1)
				{
					int LedPos = i - st->XOffsets[j];
					if(LedPos >= 0 && LedPos < XLEDS)
						BadLeds[j][LedPos] = 1;
				}

				ret = true;
			}
			else if(IsRev(i, EAd[0], PCB_POS_X))
			{
				for(j=0; j<XAXIS; j+=1)
				{
					BadLeds[j][i] = 1;
				}

				ret = true;
			}
		}
	}

	for(i=0; i<YLEDS; i+=1)
	{
		if(AxisAd[XAXIS][i] < EAd[XAXIS]/3 || AxisAd[XAXIS][i]<20 || MaxVolt[XAXIS][i] < EAd[XAXIS]/2)
		{
			if(IsEmit(i, EAd[XAXIS], PCB_POS_Y))
			{
				for(j=0; j<YAXIS; j+=1)
				{
					int LedPos = i - st->YOffsets[j];
					if(LedPos >= 0 && LedPos < YLEDS)
						BadLeds[j+XAXIS][LedPos] = 1;
				}

				ret = true;
			}
			else if(IsRev(i, EAd[XAXIS], PCB_POS_Y))
			{
            	for(j=0; j<YAXIS; j+=1)
				{
					BadLeds[j+XAXIS][i] = 1;
				}

				ret = true;
			}
		}
	}

	return ret;
}

bool intv_is_same(interval_t *a, interval_t *b)
{
	if(a->Num != b->Num) return false;

	for(int i=0; i<a->Num; i+=1){
		if((a->Section[i].Begin != b->Section[i].Begin) ||
		   (a->Section[i].End != b->Section[i].End) ) {
			return false;
		}
	}

	return true;
}

bool __fastcall Boundary::Signal_Is_Initialized(void)
{
	return  first_run >= 10;
}

//---------------------------------------------------------------------------
//Function:
//Param:
//Return:
//Note: 计算所有凹坑坐标，并扩展默认凹坑
//---------------------------------------------------------------------------
void __fastcall Boundary::MakeInterval(int SigVersion)
{
	uint16_t j;
	int i,temp;

	if(first_run<10)									 //如果程序第一次运行，给MaxVolt数组赋初值，使其更新MaxVolt
	{
		InitialMaxVolt(SigVersion);
		InitialMinVolt(SigVersion);

		first_run ++;

		SignalQuality(SigVersion);
		return;
	}

	GetInterval();
	GetAuxInterval();

	if((Intervals[0].Num == 0 && Intervals[XAXIS].Num == 0)) {
		Adj_MaxMin(SigVersion);
	}
//	else

	if(SigVersion == REPORT_MODE_VERSION5) {
		Normal_CornerSignal();
	}
}
void __fastcall Boundary::CombineInterval(void){	memcpy(&TotalIntervals[0], &Intervals[0], sizeof(interval_t) * XAXIS);	memcpy(&TotalIntervals[XAXIS], &AuxIntervals[1], sizeof(interval_t) * (XAXIS-1));	memcpy(&TotalIntervals[XAXIS*2-1], &Intervals[XAXIS], sizeof(interval_t) * YAXIS);	memcpy(&TotalIntervals[XAXIS*2-1 + YAXIS], &AuxIntervals[XAXIS+1], sizeof(interval_t) * (YAXIS-1));}
#pragma package(smart_init)
