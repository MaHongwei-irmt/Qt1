//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ChildWin.h"
#include "Boundary.h"

#include "main.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
//---------------------------------------------------------------------

const TColor SerialCl[]=
{
	clGray,
	clTeal,
	clWebTomato,
	clRed,
	clBlue,
//	clRed,
//	clDkGray,
//	clGreen,
//	clFuchsia,
//	clBlack,
//	clBlue,
//	clWebTomato,
//	clWebSalmon,
	(TColor)0x9314FF,
	(TColor)0x908070,
	(TColor)0xFF901E,
	(TColor)0x228B22,
//	0xB48246,
//	0xEEEEAF,
//	0x808000,
//	0xE16941,
//	0x800080,
//	0xCD5A6A,
//	0x3C14DC,//clCrimson,

};

__fastcall TMDIChild::TMDIChild(TComponent *Owner)
	: TForm(Owner)
{
}
//--------------------------------------------------------------------- 
void __fastcall TMDIChild::FormClose(TObject *Sender, TCloseAction &Action)
{
	Action = caFree;
}

//---------------------------------------------------------------------
void __fastcall TMDIChild::UpdateChart(uint8_t Type, uint8_t *Data, uint16_t Len, int16_t Offset, interval_t &intv, SigQ_t *SigQ)
{
	String Msg = Caption;
	char *p = strchr(AnsiString(Msg).c_str(), ' ');

	if(p != NULL)
	{
		Msg = Msg.SubString(0, (int)(p- AnsiString(Msg).c_str()));
	}

	Msg.cat_printf(L" 均值:%.0f/%.0f 方差:%.0f 偏移:%d, %d - ", SigQ->EAd, SigQ->EMax, SigQ->DAd, Offset, intv.Num);

	for(int i=0; i<intv.Num; i+=1)
	{
		Msg.cat_printf(L"(%d, %d) ", intv.Section[i].Begin, intv.Section[i].End);
	}

	Caption = Msg;

	if(Chart1->SeriesCount() < 4)
	{
		for(int i=Chart1->SeriesCount(); i<4; i+=1)
		{
			TChartSeries * pTS;
			pTS = new TLineSeries(Chart1);
			Chart1->AddSeries(pTS);
		}
	}

	for(int i=0; i<Chart1->SeriesCount(); i+=1){
		Chart1->Series[i]->Clear();
	}


	for(uint16_t i=0; i<Len; i+=1)
	{
		Chart1->Series[Type]->AddXY(i+1, Data[i], i+1);
		Chart1->Series[2]->AddXY(i+1, Data[i+Len], i+1);
		Chart1->Series[3]->AddXY(i+1, Data[i+Len*2], i+1);
	}

	Chart1->Update();
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void __fastcall TMDIChild::UpdateChart(uint8_t *Data, uint16_t Len, int16_t Offset, interval_t &intv, SigQ_t *SigQ)
{
	String Msg = Caption;
	char *p = strchr(AnsiString(Msg).c_str(), ' ');

	if(p != NULL)
	{
		Msg = Msg.SubString(0, (int)(p - AnsiString(Msg).c_str()));
	}

	Msg.cat_printf(L" 均值:%.0f/%.0f 方差:%.0f 偏移:%d, %d - ", SigQ->EAd, SigQ->EMax, SigQ->DAd, Offset, intv.Num);

	for(int i=0; i<intv.Num; i+=1)
	{
		Msg.cat_printf(L"(%d, %d) ", intv.Section[i].Begin, intv.Section[i].End);
	}

	Caption = Msg;

	if(Chart1->SeriesCount() < 6)
	{
		for(int i=Chart1->SeriesCount(); i<6; i+=1)
		{
			TChartSeries * pTS;

			if(i == 5) {
			   TBarSeries  *pTS_ = new TBarSeries(Chart1);
			   pTS = pTS_;
			   pTS_->CustomBarWidth = pTS_->BarWidth;
			   pTS_->BarWidthPercent = 95;
			   //pTS_->Marks->Visible = false;
			   pTS_->Marks->Style = smsValue;
			   pTS_->BarPen->Color = clRed;
			} else {
			   TLineSeries *pTS_ = new TLineSeries(Chart1);
			   pTS = pTS_;
			}
		   //	pTS->
			Chart1->AddSeries(pTS);
		}
	}

	for(int i=0; i<Chart1->SeriesCount(); i+=1){
		Chart1->Series[i]->Clear();
	}

//	int margin = Len / 80;
//
//	if(margin < 1) margin = 1;
//
//	for(int i=0; i<margin; i+=1){
//		Chart1->Series[3]->AddNull();
//	}

	for(uint16_t i=0; i<Len; i+=1)
	{
		if(MainForm->SigOrg->Checked == true)
		{
			if((Offset >= 0 && i+Offset<Len) ||
			   (Offset < 0 && i+Offset>=0) ||
			   ADProc.DebugDataVer == REPORT_MODE_VERSION1 ) {

			   //if(Data[i] > 40){
					Chart1->Series[3]->AddXY(i+1, Data[i], i+1, clBlue);
			   //} else {
			   //		Chart1->Series[3]->AddXY(i+1, Data[i], i+1, clRed);
			   //}
			} else {
				Chart1->Series[3]->AddNull();
			}
		}

		if(MainForm->SigCheck->Checked == true)
		{
			if((Offset >= 0 && i+Offset<Len) ||
			   (Offset < 0 && i+Offset>=0) ||
			   ADProc.DebugDataVer == REPORT_MODE_VERSION1 ) {
					if(Data[i] < 60 || Data[i] > 210) {
						Chart1->Series[5]->AddXY(i+1, Data[i], i+1, clRed);//, clBlue);
					} else {
						Chart1->Series[5]->AddNull();
					}
			} else {
				Chart1->Series[5]->AddNull();
			}
		}

		if(MainForm->SigCorner->Checked == true)
		{
			if((Offset >= 0 && i+Offset<Len) ||
			   (Offset < 0 && i+Offset>=0) ||
			   ADProc.DebugDataVer == REPORT_MODE_VERSION1)
				Chart1->Series[4]->AddNull();//Chart1->Series[4]->AddXY(i+1, 0, i+1, clWebTomato);
			else
				Chart1->Series[4]->AddXY(i+1, Data[i], i+1, clWebTomato);
		}

		if(MainForm->SigMin->Checked == true)
			Chart1->Series[1]->AddXY(i+1, Data[i+Len], i+1, clGray);

		if(MainForm->SigMax->Checked == true)
			Chart1->Series[2]->AddXY(i+1, Data[i+Len*2], i+1, clRed);

		if(MainForm->SigNormal->Checked == true)
			Chart1->Series[0]->AddXY(i+1, Data[i+Len*3], i+1, clTeal);
	}

//	for(int i=0; i<margin; i+=1){
//		Chart1->Series[3]->AddNull();
//	}

	Chart1->Update();
}

void __fastcall TMDIChild::UpdateChart(uint8_t *Data, tri_info_t *st, uint16_t Off, uint16_t Revs, uint16_t Len)
{
	String Msg = Caption;
	char *p = strchr(AnsiString(Msg).c_str(), ' ');

	if(p != NULL)
	{
		Msg = Msg.SubString(0, (int)(p - AnsiString(Msg).c_str()));
	}

	for(uint16_t i=0; i<Revs; i+=1)
	{
		Msg.cat_printf(L"   %d(%d):%d-%d", i+1, st->RevPos[Off+i]+1, st->EmitStart[Off+i]+1, st->EmitEnd[Off+i]);
	}

	Caption = Msg;

	if(Chart1->SeriesCount()< 4)
	{
		for(int i=Chart1->SeriesCount(); i<4; i+=1)
		{
			TChartSeries * pTS;
			pTS = new TLineSeries(Chart1);
			Chart1->AddSeries(pTS);
		}
	}

	for(int i=0; i<Chart1->SeriesCount(); i+=1){
		Chart1->Series[i]->Clear();
	}

	int Pos = 0;

	for(uint16_t i=0; i<Revs; i+=1)
	{
		TColor Col = clBlue;
		if((i&0x01) == 0)
		{
			Col = clMaroon;
		}

		for(int j=st->EmitStart[Off+i]; j<st->EmitEnd[Off+i]; j+=1)
		{
			String Lab;

			Lab.sprintf(L"%d-%d", i+1 , j+1);

			if(MainForm->SigOrg->Checked == true){
				Chart1->Series[3]->AddXY(Pos+1, Data[Pos], Lab, Col);              //Data[Pos]
			}

			if(MainForm->SigMin->Checked == true){
				Chart1->Series[2]->AddXY(Pos+1, Data[Pos+Len], Lab, clGray);             //clGray
			}

			if(MainForm->SigMax->Checked == true){
				Chart1->Series[1]->AddXY(Pos+1, Data[Pos+Len*2], Lab, clFuchsia);
			}

			if(MainForm->SigNormal->Checked == true){
				Chart1->Series[0]->AddXY(Pos+1, Data[Pos+Len*3], Lab, clTeal);
			}

			Pos += 1;
		}
	}

	Chart1->Update();
}




