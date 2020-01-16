//---------------------------------------------------------------------------


#pragma hdrstop

#include "FrameInfo.h"

#include "Boundary.h"

#include "Main.h"

#include <math.h>

TouchFrame FrameInfo;

//---------------------------------------------------------------------------
__fastcall TouchFrame::TouchFrame(void)
{
	InitialFrameInfo();
	Type = ST_Normal;
}

void __fastcall TouchFrame::InitialFrameInfo(void)
{
	info_t *it = &Info;

	memset(it, 0, sizeof(info_t));

	it->XLeds = 1;
	it->YLeds = 1;
	it->TotalLeds = 2;
	it->XAxis = 1;
	it->YAxis = 1;
	it->TotalAxis = 2;

	it->XCornerAxis = 0;
	it->YCornerAxis = 0;

	MtBound.Allocate_Buffers();
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
void __fastcall TouchFrame::UpdateFrameInfo(DebugHead_t *dh, uint16_t CornerSignals)
{
	float XLen, YLen;
	float Tan, CoTan, OffLen;
	info_t *it = &Info;

	memset(it, 0, sizeof(info_t));

	it->XLeds = dh->XLeds;
	it->YLeds = dh->YLeds;
	it->TotalLeds = dh->XLeds + dh->YLeds;

	it->XAxis = dh->XAxis;
	it->YAxis = dh->YAxis;
	it->TotalAxis = dh->XAxis + dh->YAxis;

	XLen = X_MARGIN_TOTAL + it->XLeds * LED_SPACE;		//fix me!!!!!!!!!  LED_SPACE

	if(it->YLeds == 0) {
		YLen = Y_MARGIN_TOTAL + it->XLeds * LED_SPACE * 9 / 16;		//fix me!!!!!!!!! 16:9
	} else {
		YLen = Y_MARGIN_TOTAL + it->YLeds * LED_SPACE;		//fix me!!!!!!!!!
	}

	//MtBound.Allocate_Buffers();

	for(int i=0; i<dh->XAxis+dh->YAxis; i+=1)
	{
		if(i<dh->XAxis)
		{
			it->XOffsets[i] = dh->Offsets[i];
			OffLen = (float)it->XOffsets[i]*LED_SPACE;

			Tan = OffLen/YLen;
			if(it->XOffsets[i] != 0)
				CoTan = YLen/OffLen;
			else
				CoTan = 0;

			//it->XTan[i] = Tan;
			//it->XCoTan[i] = CoTan;
			//it->XOffLen[i] = OffLen;
		}
		else
		{
			it->YOffsets[i-dh->XAxis] = dh->Offsets[i];
			OffLen = (float)it->YOffsets[i-dh->XAxis]*LED_SPACE;

			Tan = OffLen/XLen;
			if(it->YOffsets[i-dh->XAxis] != 0)
				CoTan = XLen/OffLen;
			else
				CoTan = 0;

			//it->YTan[i-dh->XAxis] = Tan;
			//it->YCoTan[i-dh->XAxis] = CoTan;
			//it->YOffLen[i-dh->XAxis] = OffLen;
		}

		it->Offsets[i] = dh->Offsets[i];
		//it->Tan[i] = Tan;
		it->CoTan[i] = CoTan;
		//it->OffLen[i] = OffLen;
	}

	it->Signals = it->XAxis*it->XLeds + it->YAxis*it->YLeds;

	if(it->YLeds > 0) {
		if(MAX_X_OFF + MAX_Y_OFF > 0) {
			it->XCornerAxis = it->YCornerAxis = CornerSignals/(MAX_X_OFF + MAX_Y_OFF);
		} else {
            it->XCornerAxis = it->YCornerAxis = 0;
		}
	} else {
		if(MAX_X_OFF > 0) {
			it->XCornerAxis = CornerSignals/MAX_X_OFF;
		} else {
			it->XCornerAxis = 0;
		}

        it->YCornerAxis = 0;
	}

	MtBound.Allocate_Buffers();

	it->CornerSignals = CornerSignals;
	Type = ST_Normal;
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
void __fastcall TouchFrame::UpdateFrameAuxInfo(DebugHeadV2_t *dh, uint16_t CornerSignals)
{
	int16_t *Offsets = &(dh->Offsets[dh->XAxis + dh->YAxis]);
	float XLen, YLen;
	float Tan, CoTan, OffLen;
	info_t *it = &Info;

	UpdateFrameInfo((DebugHead_t *)dh, CornerSignals);                 //Main Axis

	XLen = X_MARGIN_TOTAL + it->XLeds * LED_SPACE;		//fix me!!!!!!!!!  LED_SPACE
	YLen = Y_MARGIN_TOTAL + it->YLeds * LED_SPACE;		//fix me!!!!!!!!!

	for(int i=0; i<dh->XAxis+dh->YAxis; i+=1)
	{
		if(i<dh->XAxis)
		{
			it->AuxXOffsets[i] = Offsets[i];
			OffLen = (float)it->AuxXOffsets[i]*LED_SPACE;

			Tan = OffLen/YLen;
			if(it->AuxXOffsets[i] != 0)
				CoTan = YLen/OffLen;
			else
				CoTan = 0;

			//it->AuxXTan[i] = Tan;
			//it->AuxXCoTan[i] = CoTan;
			//it->AuxXOffLen[i] = OffLen;
		}
		else
		{
			it->AuxYOffsets[i-dh->XAxis] = Offsets[i];
			OffLen = (float)it->AuxYOffsets[i - dh->XAxis]*LED_SPACE;

			Tan = OffLen/XLen;
			if(it->AuxYOffsets[i - dh->XAxis] != 0)
				CoTan = XLen/OffLen;
			else
				CoTan = 0;

			//it->AuxYTan[i-dh->XAxis] = Tan;
			//it->AuxYCoTan[i-dh->XAxis] = CoTan;
			//it->AuxYOffLen[i-dh->XAxis] = OffLen;
		}

		it->AuxOffsets[i] = Offsets[i];
		//it->AuxTan[i] = Tan;
		//it->AuxCoTan[i] = CoTan;
		//it->AuxOffLen[i] = OffLen;
	}

	it->Signals = it->XAxis*it->XLeds + it->YAxis*it->YLeds;

	//make total offset info
//	for(int i=0; i<it->XAxis; i+=1){
//		if(i==0){
//			it->TotalXTan[0] = it->XTan[0];
//			it->TotalXCoTan[0] = it->XCoTan[0];
//			it->TotalXCos[0] = 1/sqrt(1+it->XTan[0]*it->XTan[0]);
//			if(it->XOffsets[i]) {
//				it->TotalXSin[0] = 1/sqrt(1+it->XCoTan[0]*it->XCoTan[0]);
//			} else {
//				it->TotalXSin[0] = 0;
//			}
//
//			it->XStartLed[0] = 0;
//			it->XStopLed[0] = it->XLeds;
//			it->XAxisId[0] = 0;
//			it->TotalXOffsets[0] = it->XOffsets[0];
//		} else {
//
//			it->XAxisId[i*2-1] = i;
//			it->XAxisId[i*2] = i;
//
//			if(it->XOffsets[i] < 0) {
//				it->TotalXTan[i*2-1] = it->AuxXTan[i];
//				it->TotalXCoTan[i*2-1] = it->AuxXCoTan[i];
//				it->TotalXCos[i*2-1] = 1/sqrt(1+it->AuxXTan[i]*it->AuxXTan[i]);
//				it->TotalXSin[i*2-1] = 1/sqrt(1+it->AuxXCoTan[i]*it->AuxXCoTan[i]);
//				it->XStartLed[i*2-1] = 0;
//				it->XStopLed[i*2-1] = -1 * it->XOffsets[i];
//				it->TotalXOffsets[i*2-1] = it->AuxXOffsets[i];
//
//				it->TotalXTan[i*2] = it->XTan[i];
//				it->TotalXCoTan[i*2] = it->XCoTan[i];
//				it->TotalXCos[i*2] = 1/sqrt(1+it->XTan[i]*it->XTan[i]);
//				it->TotalXSin[i*2] = 1/sqrt(1+it->XCoTan[i]*it->XCoTan[i]);
//				it->XStartLed[i*2] = -1 * it->XOffsets[i];
//				it->XStopLed[i*2] = it->XLeds;
//				it->TotalXOffsets[i*2] = it->XOffsets[i];
//			} else {
//				it->TotalXTan[i*2-1] = it->XTan[i];
//				it->TotalXCoTan[i*2-1] = it->XCoTan[i];
//				it->TotalXCos[i*2-1] = 1/sqrt(1+it->XTan[i]*it->XTan[i]);
//				it->TotalXSin[i*2-1] = 1/sqrt(1+it->XCoTan[i]*it->XCoTan[i]);
//				it->XStartLed[i*2-1] = 0;
//				it->XStopLed[i*2-1] = it->XLeds - it->XOffsets[i];
//				it->TotalXOffsets[i*2-1] = it->XOffsets[i];
//
//				it->TotalXTan[i*2] = it->AuxXTan[i];
//				it->TotalXCoTan[i*2] = it->AuxXCoTan[i];
//				it->TotalXCos[i*2] = 1/sqrt(1+it->AuxXTan[i]*it->AuxXTan[i]);
//				it->TotalXSin[i*2] = 1/sqrt(1+it->AuxXCoTan[i]*it->AuxXCoTan[i]);
//				it->XStartLed[i*2] = it->XLeds - it->XOffsets[i];
//				it->XStopLed[i*2] = it->XLeds;
//				it->TotalXOffsets[i*2] = it->AuxXOffsets[i];
//			}
//		}
//	}
//
//	for(int i=0; i<it->YAxis; i+=1){
//		if(i==0){
//			it->TotalYTan[0] = it->YTan[0];
//			it->TotalYCoTan[0] = it->YCoTan[0];
//			it->TotalYCos[0] = 1/sqrt(1+it->YTan[0]*it->YTan[0]);
//			if(it->YOffsets[i]) {
//				it->TotalYSin[0] = 1/sqrt(1+it->YCoTan[0]*it->YCoTan[0]);
//			} else {
//				it->TotalYSin[0] = 0;
//			}
//			it->YStartLed[0] = 0;
//			it->YStopLed[0] = it->YLeds;
//			it->YAxisId[i] = it->XAxis;
//			it->TotalYOffsets[0] = it->YOffsets[0];
//		} else {
//			it->YAxisId[i*2-1] = it->XAxis + i;
//			it->YAxisId[i*2] = it->XAxis + i;
//
//			if(it->YOffsets[i] < 0) {
//				it->TotalYTan[i*2-1] = it->AuxYTan[i];
//				it->TotalYCoTan[i*2-1] = it->AuxYCoTan[i];
//				it->TotalYCos[i*2-1] = 1/sqrt(1+it->AuxYTan[i]*it->AuxYTan[i]);
//				it->TotalYSin[i*2-1] = 1/sqrt(1+it->AuxYCoTan[i]*it->AuxYCoTan[i]);
//				it->YStartLed[i*2-1] = 0;
//				it->YStopLed[i*2-1] = -1 * it->YOffsets[i];
//				it->TotalYOffsets[i*2-1] = it->AuxYOffsets[i];
//
//				it->TotalYTan[i*2] = it->YTan[i];
//				it->TotalYCoTan[i*2] = it->YCoTan[i];
//				it->TotalYCos[i*2] = 1/sqrt(1+it->YTan[i]*it->YTan[i]);
//				it->TotalYSin[i*2] = 1/sqrt(1+it->YCoTan[i]*it->YCoTan[i]);
//				it->YStartLed[i*2] = -1 * it->YOffsets[i];
//				it->YStopLed[i*2] = it->YLeds;
//				it->TotalYOffsets[i*2] = it->YOffsets[i];
//			} else {
//				it->TotalYTan[i*2-1] = it->YTan[i];
//				it->TotalYCoTan[i*2-1] = it->YCoTan[i];
//				it->TotalYCos[i*2-1] = 1/sqrt(1+it->YTan[i]*it->YTan[i]);
//				it->TotalYSin[i*2-1] = 1/sqrt(1+it->YCoTan[i]*it->YCoTan[i]);
//				it->YStartLed[i*2-1] = 0;
//				it->YStopLed[i*2-1] = it->YLeds - it->YOffsets[i];
//				it->TotalYOffsets[i*2-1] = it->YOffsets[i];
//
//				it->TotalYTan[i*2] = it->AuxYTan[i];
//				it->TotalYCoTan[i*2] = it->AuxYCoTan[i];
//				it->TotalYCos[i*2] = 1/sqrt(1+it->AuxYTan[i]*it->AuxYTan[i]);
//				it->TotalYSin[i*2] = 1/sqrt(1+it->AuxYCoTan[i]*it->AuxYCoTan[i]);
//				it->YStartLed[i*2] = it->YLeds - it->YOffsets[i];
//				it->YStopLed[i*2] = it->YLeds;
//				it->TotalYOffsets[i*2] = it->AuxYOffsets[i];
//			}
//		}
//	}
//
//	memcpy(&it->TotalTan[0], &it->TotalXTan[0], sizeof(it->TotalTan[0]) * (it->XAxis*2-1));
//	memcpy(&it->TotalCoTan[0], &it->TotalXCoTan[0], sizeof(it->TotalCoTan[0]) * (it->XAxis*2-1));
//	memcpy(&it->TotalCos[0], &it->TotalXCos[0], sizeof(it->TotalCos[0]) * (it->XAxis*2-1));
//	memcpy(&it->TotalSin[0], &it->TotalXSin[0], sizeof(it->TotalSin[0]) * (it->XAxis*2-1));
//	memcpy(&it->StartLed[0], &it->XStartLed[0], sizeof(it->StartLed[0]) * (it->XAxis*2-1));
//	memcpy(&it->StopLed[0], &it->XStopLed[0], sizeof(it->StopLed[0]) * (it->XAxis*2-1));
//	memcpy(&it->AxisId[0], &it->XAxisId[0], sizeof(it->AxisId[0]) * (it->XAxis*2-1));
//
//	memcpy(&it->TotalTan[it->XAxis*2-1], &it->TotalYTan[0], sizeof(it->TotalTan[0]) * (it->YAxis*2-1));
//	memcpy(&it->TotalCoTan[it->XAxis*2-1], &it->TotalYCoTan[0], sizeof(it->TotalCoTan[0]) * (it->YAxis*2-1));
//	memcpy(&it->TotalCos[it->XAxis*2-1], &it->TotalYCos[0], sizeof(it->TotalCos[0]) * (it->YAxis*2-1));
//	memcpy(&it->TotalSin[it->XAxis*2-1], &it->TotalYSin[0], sizeof(it->TotalSin[0]) * (it->YAxis*2-1));
//	memcpy(&it->StartLed[it->XAxis*2-1], &it->YStartLed[0], sizeof(it->StartLed[0]) * (it->YAxis*2-1));
//	memcpy(&it->StopLed[it->XAxis*2-1], &it->YStopLed[0], sizeof(it->StopLed[0]) * (it->YAxis*2-1));
//	memcpy(&it->AxisId[it->XAxis*2-1], &it->YAxisId[0], sizeof(it->AxisId[0]) * (it->YAxis*2-1));
//
//	Type = ST_Normal;
}

void ExportInfo(tri_info_t *ti)
{
	FILE *fp;

	if((fp = fopen("d:\\info.txt", "w")) == NULL){
		ShowMessage(L"can not export info!\n");
		return ;
	}

	fprintf(fp, "------------------ x ---------------\n");
	fprintf(fp, "EmitLeds:%d \n", ti->XEmitLeds);
	fprintf(fp, "RevLeds:%d \n", ti->XRevLeds);
	fprintf(fp, "Boards:%d \n", ti->XBoards);
	fprintf(fp, "LedSpace:%f mm\n", ti->XLedSpace/1000.0f);
	fprintf(fp, "MarginLeftUp:%f \n", ti->MarginLeft);
	fprintf(fp, "MarginRightDown:%f \n", ti->MarginRight);
	fprintf(fp, "Length:%f \n", ti->XLength);
	fprintf(fp, "Distance:%f \n", ti->YLength);

	fprintf(fp, "RevPos EmitStart EmitEnd\n");
	for(int i=ti->YRevLeds; i<ti->XRevLeds + ti->YRevLeds; i+=1)
	{
		fprintf(fp, "%d, %d, %d\n", ti->RevPos[i], ti->EmitStart[i], ti->EmitEnd[i]);
	}

	fprintf(fp, "Off:%d\n", ti->XLedOff);

	fprintf(fp, "Tan:\n");
	for(int i=0; i<ti->XLedOff*2; i+=1)
	{
		fprintf(fp, "%f ", ti->XTan[i]);
	}

	fprintf(fp, "\nCoTan:\n");
	for(int i=0; i<ti->XLedOff*2; i+=1)
	{
		fprintf(fp, "%f ", ti->XCoTan[i]);
	}


	fprintf(fp, "\n");


	fprintf(fp, "------------------ y ---------------\n");
	fprintf(fp, "EmitLeds:%d \n", ti->YEmitLeds);
	fprintf(fp, "RevLeds:%d \n", ti->YRevLeds);
	fprintf(fp, "Boards:%d \n", ti->YBoards);
	fprintf(fp, "LedSpace:%f mm\n", ti->YLedSpace/1000.0f);
	fprintf(fp, "MarginLeftUp:%f \n", ti->MarginUp);
	fprintf(fp, "MarginRightDown:%f \n", ti->MarginDown);
	fprintf(fp, "Length:%f \n", ti->YLength);
	fprintf(fp, "Distance:%f \n", ti->XLength);

	fprintf(fp, "RevPos EmitStart EmitEnd\n");
	for(int i=0; i<ti->YRevLeds; i+=1)
	{
		fprintf(fp, "%d, %d, %d\n", ti->RevPos[i], ti->EmitStart[i], ti->EmitEnd[i]);
	}

	fprintf(fp, "Off:%d\n", ti->YLedOff);

	fprintf(fp, "Tan:\n");
	for(int i=0; i<ti->YLedOff*2; i+=1)
	{
		fprintf(fp, "%f ", ti->YTan[i]);
	}

	fprintf(fp, "\nCoTan:\n");
	for(int i=0; i<ti->YLedOff*2; i+=1)
	{
		fprintf(fp, "%f ", ti->YCoTan[i]);
	}


	fprintf(fp, "\n");



	fclose(fp);
}

#pragma package(smart_init)
