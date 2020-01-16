//---------------------------------------------------------------------------


#pragma hdrstop

#include "BoundaryTri.h"

//---------------------------------------------------------------------------

BoundaryTri MtBoundTri;

__fastcall BoundaryTri::BoundaryTri(void)
{
	first_run = 0;
}

//环境光
void __fastcall BoundaryTri::InitialUsualVolt(void)
{
	memcpy(UsualVolt[0], AdBuf[0], FrameInfo.InfoTri.Signals);
}

//最大值
void __fastcall BoundaryTri::InitialMaxVolt(void)
{
	memcpy(MaxVolt[0], AdBuf[0], FrameInfo.InfoTri.Signals);
}

//最小值
void __fastcall BoundaryTri::InitialMinVolt(void)
{
	for(uint16_t i=0; i<FrameInfo.InfoTri.Signals; i+=1){
		UsualVolt[0][i] = MaxVolt[0][i]>>2;         //that is 1/4
	}
}

//真实值
void __fastcall BoundaryTri::GetAxisVolt(void)
{
	memcpy(AxisAd[0], AdBuf[0], FrameInfo.InfoTri.Signals);
}

uint8_t __fastcall BoundaryTri::NormalAD(int i, int j)
{
	uint8_t Normal;
	int iTemp;

	if(UsualVolt[i][j] > AxisAd[i][j])
	{
		UsualVolt[i][j] = AxisAd[i][j];
	}

//	if(MaxVolt[i][j] < UsualVolt[i][j] + 10)
//	{
//		Normal = 0;
//	}
//	else
	if(MaxVolt[i][j] <= AxisAd[i][j])  //((MaxVolt[i][j] > AxisAd[i][j] && MaxVolt[i][j] - AxisAd[i][j] < (MaxVolt[i][j]>>2)) ||
	{
		Normal = 255;
	}
	else if(MaxVolt[i][j] > UsualVolt[i][j])
	{
		iTemp = ((AxisAd[i][j]-UsualVolt[i][j])*256)/(MaxVolt[i][j] - UsualVolt[i][j]);

		if(iTemp > 255) Normal = 255;
		else if(iTemp <0) Normal = 0;
		else Normal = iTemp;
	}
	else                                  //MaxVolt[i][j]<UsualVolt[i][j]
	{
		Normal = 255;
	}

	NormAd[i][j] = Normal;

	return Normal;
}

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

void __fastcall BoundaryTri::Allocate_Buffers(tri_info_t *st)
{
	int BufPos = 0;

	memset(MemPool, 0, sizeof(MemPool));

	for(int i=0; i<st->RevLeds; i+=1)
	{
		AdBuf[i] = MemPool + BufPos;
		AxisAd[i] = MemPool + st->Signals + BufPos;
		UsualVolt[i] = MemPool + st->Signals*2 + BufPos;
		MaxVolt[i] = MemPool + st->Signals*3 + BufPos;
		NormAd[i] = MemPool + st->Signals*4 + BufPos;
		BadLeds[i] = MemPool + st->Signals*5 + BufPos;

		BufPos += st->EmitEnd[i] - st->EmitStart[i];
	}
}

//---------------------------------------------------------------------------
void __fastcall BoundaryTri::SignalQuality(void)
{
	int i,j;
	double Variance;
	double Average;
	double AdSum;
	tri_info_t *fi = &FrameInfo.InfoTri;

	for(i=0; i<fi->RevLeds; i+=1)
	{
		uint16_t Emits = fi->EmitEnd[i] - fi->EmitStart[i];

		if(Emits == 0) continue;
        //-------------------------------------------------
		//Maxim AD Value
		//-------------------------------------------------
		Average = 0;
		for(uint16_t j=0; j<Emits; j+=1)
		{
			Average += MaxVolt[i][j];
		}

		SigQ[i].EMax = Average/Emits;

		//-------------------------------------------------
		//Current AD Value
		//-------------------------------------------------
		Average = 0;
		for(uint16_t j=0; j<Emits; j+=1)
		{
			Average += AxisAd[i][j];
		}

		Average = Average/Emits;

		//-----------------------------------
		Variance = 0;
		for(uint16_t j=0; j<Emits; j+=1)
		{
			double ftemp = (double)AxisAd[i][j] - Average;
			Variance += ftemp * ftemp;
		}

		Variance = Variance/Emits;

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
}

//---------------------------------------------------------------------------
void __fastcall BoundaryTri::Adj_MaxMin(void)
{
	uint32_t Len;
	tri_info_t *fi = &FrameInfo.InfoTri;

	for(int i=0; i<fi->RevLeds; i+=1)
	{
		uint16_t Emits = fi->EmitEnd[i] - fi->EmitStart[i];

		for(uint16_t j=0; j<Emits; j+=1){             //Adj Max voltage
			if(AxisAd[i][j] > MaxVolt[i][j]){
				if(AxisAd[i][j] < EAdMax[i]){
					MaxVolt[i][j] = AxisAd[i][j];
				}else{
					MaxVolt[i][j] ++;
				}
			}else if(AxisAd[i][j] < MaxVolt[i][j] ){       //&& MtTrace.RealPoints == 0
				if(AxisAd[i][j] > EAd[i]){
					MaxVolt[i][j] = AxisAd[i][j];
				}else if(AxisAd[i][j] > EAdMin[i]){
					//if(MtTrace.RealPoints == 0)
					MaxVolt[i][j] --;
				}else if(AxisAd[i][j] > UsualVolt[i][j] + 20){
					//if(MtTrace.RealPoints == 0)
					MaxVolt[i][j] --;
				}
			}
#if 0
			//Adj Min voltage
			if(AxisAd[i][j] <= UsualVolt[i][j]){
				UsualVolt[i][j] = AxisAd[i][j];
			}else if(MaxVolt[i][j] > AxisAd[i][j] + 20){
				if(UsualVolt[i][j] > MaxVolt[i][j]/4)
					UsualVolt[i][j] --;
			}else if(UsualVolt[i][j] > MaxVolt[i][j]/4){
				UsualVolt[i][j] --;
			}else if(UsualVolt[i][j] < MaxVolt[i][j]/4){
				UsualVolt[i][j] ++;
			}
#endif
		}
	}
}

//获取凹坑
#define HIGH_NORMAL_THRES	220
#define DOWN_THRES	140
#define UP_THRES	140
#define LOW_NORMAL_THRES	100

uint16_t __fastcall BoundaryTri::GetInterval_Side(uint16_t RevLeds, uint16_t Off, uint8_t Flag)
{
	tri_info_t *fi = &FrameInfo.InfoTri;
	uint16_t Intvs = 0;
	uint8_t Normal;
	uint8_t LastNormal;
	uint8_t Down;
	uint16_t Begin, End;

	for(uint16_t i=0; i<RevLeds; i+=1)
	{
		uint16_t Start = fi->EmitStart[i+Off];
		uint16_t Stop = fi->EmitEnd[i+Off];

		Down = 0;
		LastNormal = 255;

		RevIntvs[i+Off].Num = 0;
		RevIntvs[i+Off].intvs = &Intervals[IntervalNum];

		if(NormalAD(i+Off, 0) < DOWN_THRES){
			Flag |= FLAG_INTV_BEGIN_UNCLEAR;
		}

		for(uint16_t j=Start; j<Stop; j+=1)
		{
			Normal = NormalAD(i+Off, j-Start);

			if(!Down && Normal < DOWN_THRES)			//记录下降沿
			{
				Down = 1;

				if(LastNormal < HIGH_NORMAL_THRES){
					Begin = (j-1) * SCALE + InterpolationTable[255 - LastNormal]/2;
				}else{
					Begin = j * SCALE + InterpolationTable[Normal]*LED_WIDTH_SCALE/SCALE;
				}
			}
			else if(Down && Normal > UP_THRES)
			{
				Down = 0;

				if(Normal > HIGH_NORMAL_THRES){
					if(LastNormal > LOW_NORMAL_THRES)    {
						End = (j-1) * SCALE + InterpolationTable[255 - LastNormal]/2;
					} else {
						End = j * SCALE;
					}
				}else{
					End = j * SCALE + InterpolationTable[255 - Normal]/2;
				}

				if(End > Begin){
					Intervals[IntervalNum].RevId = i;
					Intervals[IntervalNum].Flag |= Flag;
					Intervals[IntervalNum].MatchID = 0;
					Intervals[IntervalNum].Begin = Begin;
					Intervals[IntervalNum].End = End;
					Intervals[IntervalNum].OldBegin = Begin;
					Intervals[IntervalNum].OldEnd = End;

					IntervalNum += 1;
					Intvs += 1;
					RevIntvs[i+Off].Num += 1;

					Flag &= ~FLAG_INTV_BEGIN_UNCLEAR;
				}
			}

			if((IntervalNum >= MAX_INTERVALS_TRI))
			{
				break;
			}

			LastNormal = Normal;
		}

		if(Down)
		{
			End = (Stop-1) * SCALE + InterpolationTable[255-Normal]/2;

			if(End > Begin)
			{
				Intervals[IntervalNum].RevId = i;
				Intervals[IntervalNum].MatchID = 0;
				Intervals[IntervalNum].Flag = Flag | FLAG_INTV_END_UNCLEAR;
				Intervals[IntervalNum].Begin = Begin;
				Intervals[IntervalNum].End = End;
				Intervals[IntervalNum].OldBegin = Begin;
				Intervals[IntervalNum].OldEnd = End;

				IntervalNum += 1;
				Intvs += 1;
				RevIntvs[i+Off].Num += 1;
			}
		}
	}

	return Intvs;
}

void __fastcall BoundaryTri::GetInterval(void)
{
	tri_info_t *fi = &FrameInfo.InfoTri;
	uint16_t Intvs = 0;

	IntervalNum = 0;
	memset(Intervals, 0, sizeof(Intervals));	//for debug

	YLIntvs = &Intervals[Intvs];
	Intvs += YLIntvNum = GetInterval_Side(fi->YRevLeds, 0, FLAG_ID_YL);

	XUIntvs = &Intervals[Intvs];
	Intvs += XUIntvNum = GetInterval_Side(fi->XRevLeds, fi->YRevLeds, FLAG_ID_XU);

	XDIntvs = &Intervals[Intvs];
	Intvs += XDIntvNum = GetInterval_Side(fi->XRevLeds, fi->YRevLeds + fi->XRevLeds, FLAG_ID_XD);

	YRIntvs = &Intervals[Intvs];
	Intvs += YRIntvNum = GetInterval_Side(fi->YRevLeds, fi->YRevLeds + fi->XRevLeds*2, FLAG_ID_YR);

	if(IntervalNum > 4)
	{
		Match_Interval();
	}
}

void __fastcall BoundaryTri::Clear_Bound_Interval_Expand(intv_tri_t **Intvs, uint8_t Num)
{
	uint32_t Width = 0;
	uint8_t Valids = 0;

	for(uint8_t i=0; i<Num; i++)
	{
		if((Intvs[i]->Flag & (FLAG_INTV_UNCLEAR | FLAG_INTV_MIX)) == 0)
		{
			Width += (Intvs[i]->End - Intvs[i]->Begin);
			Valids += 1;
		}
	}

	if(Valids)
	{
		Width = Width/Valids;

		for(uint8_t i=0; i<Num; i++)
		{
			if((Intvs[i]->Flag & (FLAG_INTV_UNCLEAR | FLAG_INTV_MIX)) == 0)
			{
				int Mid =  (Intvs[i]->Begin + Intvs[i]->End)/2;

				Intvs[i]->Begin = Mid - Width/2;
				Intvs[i]->End = Mid + Width/2;
			}
		}
	}
}

void __fastcall BoundaryTri::Unclear_Bound_Interval_Expand(intv_tri_t **Intvs, uint8_t Num)
{
	int Width = 0;
	uint8_t Valids = 0;

	for(uint8_t i=0; i<Num; i++)
	{
		if((Intvs[i]->Flag & FLAG_INTV_UNCLEAR) == 0)      //fix me !!!!!!!!!!!!! FLAG_INTV_MIX
		{
			Width += (Intvs[i]->End - Intvs[i]->Begin);
			Valids += 1;
		}
	}

	if(Valids)
	{
		Width = Width/Valids;

		for(uint8_t i=0; i<Num; i++)
		{
			if(Intvs[i]->End - Intvs[i]->Begin > Width - SCALE/2)       //不能缩小 (边缘不确定的凹坑必然比实际的要小)
				continue;

			if(Intvs[i]->Flag & FLAG_INTV_BEGIN_UNCLEAR)
			{
				Intvs[i]->Begin = Intvs[i]->End - Width;
			}
			else if(Intvs[i]->Flag & FLAG_INTV_END_UNCLEAR)
			{
				Intvs[i]->End = Intvs[i]->Begin + Width;
			}
		}
	}
}

void __fastcall BoundaryTri::Match_Bouble_Interval_Side(intv_tri_t *Intv, uint16_t Num, uint16_t StartID)
{
	uint16_t  MatchID = StartID;
	intv_tri_t *MatchedIntvs[2];

	for(uint16_t i=0; i<Num; i+=1)
	{
		if(Intv[i].MatchID == 0){
			uint8_t RevIdToMatch = Intv[i].RevId + 1;
			int WidthToMatch = Intv[i].End - Intv[i].Begin;
			int CenterToMatch = (Intv[i].End + Intv[i].Begin)/2;
			bool Matched = false;

			for(uint8_t j=i+1; j<Num; j+=1)
			{
				if(Intv[j].RevId > RevIdToMatch)
					break;

				if(Intv[j].RevId == RevIdToMatch && Intv[j].MatchID == 0)
				{
					//width
					int Width = Intv[j].End - Intv[j].Begin;
					int Center = (Intv[j].End + Intv[j].Begin)/2;

					if(Center > CenterToMatch + SCALE)
						continue;

					if((Intv[i].Flag & FLAG_INTV_UNCLEAR) || (Intv[j].Flag & FLAG_INTV_UNCLEAR)){
						if(Width > WidthToMatch) {

						}
					} else {
						Width -= WidthToMatch;
					}

					if(-SCALE*3/2 < Width && Width < SCALE*3/2)    //理论宽度误差不能超过灯间距
					{
						//matched !!
						Intv[j].MatchID = MatchID;
						Intv[j].MixCnt = 1;

						MatchedIntvs[0] = &Intv[i];
						MatchedIntvs[1] = &Intv[j];

						Clear_Bound_Interval_Expand(MatchedIntvs, 2);
						Unclear_Bound_Interval_Expand(MatchedIntvs, 2);

						Matched = true;
						break;
					}
				}
			}

			if(!Matched){
            	Intv[i].Flag |= FLAG_INTV_MIX;
			}

            Intv[i].MatchID = MatchID;
			Intv[i].MixCnt = 1;
			MatchID += 1;
		}
	}
}

void __fastcall BoundaryTri::Match_Interval_Side(uint16_t RevLeds, uint16_t Off, intv_tri_t *Intv, uint16_t Num)
{
	uint8_t MatchID = 1;
	int WidthToMatch;
	int CenterToMatch;
	uint16_t Matchs;
	intv_tri_t *MatchedIntvs[20];

	for(uint16_t i=0; i<Num; i+=1)
	{
		if(Intv[i].MatchID == 0){
			WidthToMatch = Intv[i].End - Intv[i].Begin;
			CenterToMatch = (Intv[i].End + Intv[i].Begin)/2;
			bool Matched = false;

			for(int j=Num-1; j>i; j-=1) {
				Matched = false;
				Matchs = 0;

				if(Intv[j].RevId == Intv[i].RevId)
					break;

				if(Intv[j].MatchID == 0)
				{
					int Width = Intv[j].End - Intv[j].Begin;
					int Center = (Intv[j].End + Intv[j].Begin)/2;

					if(Center > CenterToMatch + SCALE)
						continue;
						
					if((Intv[i].Flag & FLAG_INTV_UNCLEAR) || (Intv[j].Flag & FLAG_INTV_UNCLEAR)){
						Width = 0;
					} else {
						Width -= WidthToMatch;
					}

					if(-SCALE*3/2 < Width && Width < SCALE*3/2)    //理论宽度误差不能超过灯间距
					{
						int IDStart  = Intv[i].RevId;
						int IDEnd  = Intv[j].RevId;

						Matched = true;
						MatchedIntvs[Matchs ++] = &Intv[j];

						for(int k = IDStart + 1; k<IDEnd; k+=1)
						{
							int Intp = CenterToMatch + (Center - CenterToMatch)* (k-IDStart)/(IDEnd - IDStart);
							rev_intv_t *ritv = &RevIntvs[k + Off];       //使用id快速搜索

							Matched = false;

							for(int l=0; l<ritv->Num; l+=1)
							{
								if(ritv->intvs[l].MatchID == 0 || (ritv->intvs[l].Flag & FLAG_INTV_MIX))
								{
									int W = ritv->intvs[l].End - ritv->intvs[l].Begin;
									int C = (ritv->intvs[l].End + ritv->intvs[l].Begin)/2;

									C -= Intp;

									if(ritv->intvs[l].Begin < Intp && Intp < ritv->intvs[l].End)
									{
										Matched = true;

										if((ritv->intvs[l].Flag & FLAG_INTV_UNCLEAR) == 0)
										{
											
										}

										W -= WidthToMatch;

										if(-SCALE*3/2 < W)
										{
											MatchedIntvs[Matchs ++] = &ritv->intvs[l];

											if(W > SCALE*3/2)		//超宽    fix me!!!!!!!!!! 超宽判断 不依据  FLAG_INTV_UNCLEAR
											{
												ritv->intvs[i].Flag |= FLAG_INTV_MIX;
											}
										}
										break;
									}
								}
							}

							if(! Matched) {
								break;
							}
						}
					}
				}

				if(Matched)	//more than 2
				{
					MatchedIntvs[Matchs ++] = &Intv[i];       //myself

					if(Matchs > 2)
					{
						for(int k=0; k<Matchs; k+=1)
						{
							if(MatchedIntvs[k]->Flag & FLAG_INTV_MIX){
								MatchedIntvs[k]->MatchID = 255;
								MatchedIntvs[k]->MixCnt += 1;    //mixed number
							} else {
								MatchedIntvs[k]->MatchID = MatchID;
								MatchedIntvs[k]->MixCnt = 1;
							}
						}

						Clear_Bound_Interval_Expand(MatchedIntvs, Matchs);
						Unclear_Bound_Interval_Expand(MatchedIntvs, Matchs);

						MatchID += 1;
					}

					break;
				}
			}

//			if(!Matched){
//				Intv[i].Flag |= FLAG_INTV_MIX;
//				Intv[i].MatchID = 255;
//				Intv[i].MixCnt = 1;
//			} else if(Matchs > 1){
//				Intv[i].MatchID = MatchID;
//				Intv[i].MixCnt = 1;
//				MatchID += 1;
//			} else {
//				//double match
//			}
		}
	}

	Match_Bouble_Interval_Side(Intv, Num, MatchID);
}

void __fastcall BoundaryTri::Match_Interval(void)
{
	tri_info_t *fi = &FrameInfo.InfoTri;
	uint16_t RevOff = 0, IntvOff = 0;

	Match_Interval_Side(fi->YRevLeds, RevOff, &Intervals[IntvOff], YLIntvNum);

	RevOff += fi->YRevLeds;
	IntvOff += YLIntvNum;
	Match_Interval_Side(fi->XRevLeds, RevOff, &Intervals[IntvOff], XUIntvNum);

	RevOff += fi->XRevLeds;
	IntvOff += XUIntvNum;
	Match_Interval_Side(fi->XRevLeds, RevOff, &Intervals[IntvOff], XDIntvNum);

	RevOff += fi->XRevLeds;
	IntvOff += XDIntvNum;
	Match_Interval_Side(fi->YRevLeds, RevOff, &Intervals[IntvOff], YRIntvNum);
}

//---------------------------------------------------------------------------
//Function:
//Param:
//Return:
//Note: 计算所有凹坑坐标，并扩展默认凹坑
//---------------------------------------------------------------------------
void __fastcall BoundaryTri::MakeInterval(void)
{
	uint16_t j;
	int i,temp;

	if(first_run<10)									 //如果程序第一次运行，给MaxVolt数组赋初值，使其更新MaxVolt
	{
		InitialMaxVolt();
		InitialMinVolt();

		first_run ++;

		SignalQuality();
		return;
	}

	GetInterval();

	if(IntervalNum < 4 )	 //&& MtTrace.RealPoints == 0	  //如果x y轴主轴没有凹坑 更新最大值
	{
		SignalQuality();
		Adj_MaxMin();
	}
	
	//Adj_MaxMin();

	//AddDefaultInterval();
	//IntervalShift(false);
}


#pragma package(smart_init)
