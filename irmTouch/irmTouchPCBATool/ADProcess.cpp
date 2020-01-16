// ---------------------------------------------------------------------------

#pragma hdrstop

#include <vcl.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "time.h"

#include "ADProcess.h"
#include "FrameInfo.h"

#include "Boundary.h"

#include "Image.h"
#include "main.h"

#include "Calibrate.h"

// ---------------------------------------------------------------------------
ADProcess ADProc;
extern TFormPoints * PtFrm;

__fastcall ADProcess::ADProcess(void) {
	memset(&LastDebugHead, 0, sizeof(LastDebugHead));
	memset(&LastDebugHeadV2, 0, sizeof(LastDebugHeadV2));

	LastCornerSignals = CornerSignals = 0;

	Initial_Algorithm();
}

void __fastcall ADProcess::Initial_Algorithm(void) {
	MtBound.first_run = 0;

	Logfp = NULL;
	LastLogSta = false;
}

void __fastcall ADProcess::LogADDataV4(FILE *fp, void *Head, uint8_t *Data,
	bool ClcCheckSum) {
	softscan_ctrl_t ssc;
	DebugHeadV2_t *HeadV2 = (DebugHeadV2_t*)Head;
	uint32_t HeadLen = 11 + (HeadV2->XAxis + HeadV2->YAxis) * 2;
	uint8_t *Ptr = Data;
	// uint32_t ssclen;
	// char verstr[128];
	//
	// rewind(fp);
	// fread(verstr, sizeof(char), sizeof(LogFileMagicStringV4), fp);
	// if((strncmp(verstr, LogFileMagicStringV4, sizeof(LogFileMagicStringV4)) != 0) {
	// rewind(fp);
	// fprintf(fp, "%s\n", LogFileMagicStringV4.c_str());
	// }
	//
	// fseek(fp, 0, SEEK_END);

	fprintf(fp, "%d ", HeadV2->Type);
	fprintf(fp, "%d ", HeadV2->FrameNum);
	fprintf(fp, "%d ", HeadV2->Fps);
	fprintf(fp, "%d ", HeadV2->ScanTime);
	fprintf(fp, "%d ", HeadV2->XAxis);
	fprintf(fp, "%d ", HeadV2->YAxis);
	fprintf(fp, "%d ", HeadV2->XLeds);
	fprintf(fp, "%d\n", HeadV2->YLeds);

	for (int i = 0; i < HeadV2->XAxis + HeadV2->YAxis; i += 1) {
		fprintf(fp, "%d ", HeadV2->Offsets[i]);
	}

	if ((HeadV2->Type & 0xF0) == REPORT_MODE_VERSION4) {
		fprintf(fp, "\n");

		for (int i = HeadV2->XAxis + HeadV2->YAxis;
		i < (HeadV2->XAxis + HeadV2->YAxis) * 2; i += 1) {
			fprintf(fp, "%d ", HeadV2->Offsets[i]);
		}

		HeadLen = 11 + (HeadV2->XAxis + HeadV2->YAxis) * 4;
	}

	fprintf(fp, "\n");

	// ssc
	ssc.Flag = (*Ptr++);
	((uint8_t*)&ssc.xStart)[0] = (*Ptr++);
	((uint8_t*)&ssc.xStart)[1] = (*Ptr++);
	((uint8_t*)&ssc.xEnd)[0] = (*Ptr++);
	((uint8_t*)&ssc.xEnd)[1] = (*Ptr++);
	((uint8_t*)&ssc.yStart)[0] = (*Ptr++);
	((uint8_t*)&ssc.yStart)[1] = (*Ptr++);
	((uint8_t*)&ssc.yEnd)[0] = (*Ptr++);
	((uint8_t*)&ssc.yEnd)[1] = (*Ptr++);
	ssc.xAxis = (*Ptr++);
	ssc.yAxis = (*Ptr++);

	for (int i = 0; i < ssc.xAxis + ssc.yAxis; i += 1) {

		ssc.AxiIds[i] = (*Ptr++);
	}

	fprintf(fp, "%d ", ssc.Flag);
	fprintf(fp, "%d %d ", ssc.xStart, ssc.xEnd);
	fprintf(fp, "%d %d ", ssc.yStart, ssc.yEnd);
	fprintf(fp, "%d %d ", ssc.xAxis, ssc.yAxis);

	for (int i = 0; i < ssc.xAxis + ssc.yAxis; i += 1) {
		fprintf(fp, "%d ", ssc.AxiIds[i]);
	}

	fprintf(fp, "\n");

	// ssclen = 11 + ssc.xAxis + ssc.yAxis;

	for (int i = 0; i < ssc.xAxis; i += 1) {
		for (int j = ssc.xStart; j <= ssc.xEnd; j += 1) {
			fprintf(fp, "%u ", (*Ptr++) & 0xFF);
		}

		fprintf(fp, "\n");
	}

	for (int i = 0; i < ssc.yAxis; i += 1) {
		for (int j = ssc.yStart; j <= ssc.yEnd; j += 1) {
			fprintf(fp, "%u ", (*Ptr++) & 0xFF);
		}

		fprintf(fp, "\n");
	}

	if (ClcCheckSum) {
		uint8_t CheckSum = 0;

		for (uint32_t i = 0; i < HeadLen; i += 1) {
			CheckSum += ((uint8_t *)HeadV2)[i];
		}

		for (int i = 0; i < HeadV2->XAxis * HeadV2->XLeds +
			HeadV2->YAxis * HeadV2->YLeds; i += 1) {
			CheckSum += Data[i];
		}

		fprintf(fp, "%d\n", CheckSum & 0x0FF); // CheckSum
	}
	else {
		fprintf(fp, "%d\n", (*Ptr++) & 0x0FF); // CheckSum
	}
}

void __fastcall ADProcess::LogADData(FILE *fp, void *Head, uint8_t *Data,
	bool ClcCheckSum) {
	DebugHeadV2_t *HeadV2 = (DebugHeadV2_t*)Head;
	uint32_t HeadLen = 11 + (HeadV2->XAxis + HeadV2->YAxis) * 2;
	uint8_t *Ptr = Data;

	fprintf(fp, "%d ", HeadV2->Type);
	fprintf(fp, "%d ", HeadV2->FrameNum);
	fprintf(fp, "%d ", HeadV2->Fps);
	fprintf(fp, "%d ", HeadV2->ScanTime);
	fprintf(fp, "%d ", HeadV2->XAxis);
	fprintf(fp, "%d ", HeadV2->YAxis);
	fprintf(fp, "%d ", HeadV2->XLeds);
	fprintf(fp, "%d\n", HeadV2->YLeds);

	for (int i = 0; i < HeadV2->XAxis + HeadV2->YAxis; i += 1) {
		fprintf(fp, "%d ", HeadV2->Offsets[i]);
	}

	if ((HeadV2->Type & 0xF0) == REPORT_MODE_VERSION2) {
		fprintf(fp, "\n");

		for (int i = HeadV2->XAxis + HeadV2->YAxis;
		i < (HeadV2->XAxis + HeadV2->YAxis) * 2; i += 1) {
			fprintf(fp, "%d ", HeadV2->Offsets[i]);
		}

		HeadLen = 11 + (HeadV2->XAxis + HeadV2->YAxis) * 4;
	}

	fprintf(fp, "\n");

	for (int i = 0; i < HeadV2->XAxis; i += 1) {
		for (int j = 0; j < HeadV2->XLeds; j += 1) {
			fprintf(fp, "%u ", (*Ptr++) & 0xFF);
		}

		fprintf(fp, "\n");
	}

	for (int i = 0; i < HeadV2->YAxis; i += 1) {
		for (int j = 0; j < HeadV2->YLeds; j += 1) {
			fprintf(fp, "%u ", (*Ptr++) & 0x0FF);
		}

		fprintf(fp, "\n");
	}

	if (ClcCheckSum) {
		uint8_t CheckSum = 0;

		for (uint32_t i = 0; i < HeadLen; i += 1) {
			CheckSum += ((uint8_t *)HeadV2)[i];
		}

		for (int i = 0; i < HeadV2->XAxis * HeadV2->XLeds +
			HeadV2->YAxis * HeadV2->YLeds; i += 1) {
			CheckSum += Data[i];
		}

		fprintf(fp, "%d\n", CheckSum & 0x0FF); // CheckSum
	}
	else {
		fprintf(fp, "%d\n", (*Ptr++) & 0x0FF); // CheckSum
	}
}

void __fastcall ADProcess::LogADData_V5_Cur(FILE *fp, void *Head, uint8_t **Buf)
{
	DebugHeadV2_t *HeadV2 = (DebugHeadV2_t*)Head;
	uint32_t HeadLen = 11 + (HeadV2->XAxis + HeadV2->YAxis) * 2;
	// info_t *it = &FrameInfo.Info;
	uint8_t CheckSum = 0;

	fprintf(fp, "%d ", HeadV2->Type);
	fprintf(fp, "%d ", HeadV2->FrameNum);
	fprintf(fp, "%d ", HeadV2->Fps);
	fprintf(fp, "%d ", HeadV2->ScanTime);
	fprintf(fp, "%d ", HeadV2->XAxis);
	fprintf(fp, "%d ", HeadV2->YAxis);
	fprintf(fp, "%d ", HeadV2->XLeds);
	fprintf(fp, "%d\n", HeadV2->YLeds);

	for (int i = 0; i < HeadV2->XAxis + HeadV2->YAxis; i += 1) {
		fprintf(fp, "%d ", HeadV2->Offsets[i]);
	}

	fprintf(fp, "\n");

	for (int i = HeadV2->XAxis + HeadV2->YAxis;
	i < (HeadV2->XAxis + HeadV2->YAxis) * 2; i += 1) {
		fprintf(fp, "%d ", HeadV2->Offsets[i]);
	}

	HeadLen = 11 + (HeadV2->XAxis + HeadV2->YAxis) * 4;

	fprintf(fp, "\n");

	for (int i = 0; i < HeadV2->XAxis; i += 1) {
		for (int j = 0; j < HeadV2->XLeds; j += 1) {
			fprintf(fp, "%u ", Buf[i][j] & 0xFF);
			CheckSum += Buf[i][j];
		}

		fprintf(fp, "\n");
	}

	for (uint32_t i = 0; i < HeadLen; i += 1) {
		CheckSum += ((uint8_t *)HeadV2)[i];
	}

	// full scan data
	for (int i = 0; i < HeadV2->YAxis; i += 1) {
		for (int j = 0; j < HeadV2->YLeds; j += 1) {
			fprintf(fp, "%u ", Buf[i + XAXIS][j] & 0x0FF);
			CheckSum += Buf[i + XAXIS][j];
		}

		fprintf(fp, "\n");
	}

	// if(HeadV2->YLeds != 0) {
	// x corner
	for (int i = 0; i < X_CORNER_AXIS; i += 1) {
		for (int j = 0; j < MAX_X_OFF; j += 1) {
			fprintf(fp, "%u ", Buf[i + XAXIS + YAXIS][j] & 0x0FF);
			CheckSum += Buf[i + XAXIS + YAXIS][j];
		}

		fprintf(fp, "\n");
	}

	// y corner
	for (int i = 0; i < Y_CORNER_AXIS; i += 1) {
		for (int j = 0; j < MAX_Y_OFF; j += 1) {
			fprintf(fp, "%u ",
				Buf[i + XAXIS + YAXIS + X_CORNER_AXIS][j] & 0x0FF);
			CheckSum += Buf[i + XAXIS + YAXIS + X_CORNER_AXIS][j];
		}

		fprintf(fp, "\n");
	}

	// } else {
	// for(int i=0; i<12; i+=1) {
	// for(int j=0; j<MAX_X_OFF; j+=1) {
	// fprintf(fp, "%u ", Buf[i+XAXIS+YAXIS][j] & 0x0FF);
	// CheckSum += Buf[i+XAXIS+YAXIS][j];
	// }
	//
	// fprintf(fp, "\n");
	// }
	// }

	fprintf(fp, "%d\n", CheckSum & 0x0FF); // CheckSum
}

void __fastcall ADProcess::LogADData_V5(FILE *fp, void *Head, uint8_t *Data) {
	DebugHeadV2_t *HeadV2 = (DebugHeadV2_t*)Head;
	// uint32_t HeadLen = 11 + (HeadV2->XAxis + HeadV2->YAxis) * 2;
	uint8_t *Ptr = Data;
	// info_t *it = &FrameInfo.Info;

	fprintf(fp, "%d ", HeadV2->Type);
	fprintf(fp, "%d ", HeadV2->FrameNum);
	fprintf(fp, "%d ", HeadV2->Fps);
	fprintf(fp, "%d ", HeadV2->ScanTime);
	fprintf(fp, "%d ", HeadV2->XAxis);
	fprintf(fp, "%d ", HeadV2->YAxis);
	fprintf(fp, "%d ", HeadV2->XLeds);
	fprintf(fp, "%d\n", HeadV2->YLeds);

	for (int i = 0; i < HeadV2->XAxis + HeadV2->YAxis; i += 1) {
		fprintf(fp, "%d ", HeadV2->Offsets[i]);
	}

	fprintf(fp, "\n");

	for (int i = HeadV2->XAxis + HeadV2->YAxis;
	i < (HeadV2->XAxis + HeadV2->YAxis) * 2; i += 1) {
		fprintf(fp, "%d ", HeadV2->Offsets[i]);
	}

	// HeadLen = 11 + (HeadV2->XAxis + HeadV2->YAxis) * 4;

	fprintf(fp, "\n");

	for (int i = 0; i < HeadV2->XAxis; i += 1) {
		for (int j = 0; j < HeadV2->XLeds; j += 1) {
			fprintf(fp, "%u ", (*Ptr++) & 0xFF);
		}

		fprintf(fp, "\n");
	}

	// if(HeadV2->YLeds != 0) {
	// full scan data
	for (int i = 0; i < HeadV2->YAxis; i += 1) {
		for (int j = 0; j < HeadV2->YLeds; j += 1) {
			fprintf(fp, "%u ", (*Ptr++) & 0x0FF);
		}

		fprintf(fp, "\n");
	}

	// x corner
	for (int i = 0; i < X_CORNER_AXIS; i += 1) {
		for (int j = 0; j < MAX_X_OFF; j += 1) {
			fprintf(fp, "%u ", (*Ptr++) & 0x0FF);
		}

		fprintf(fp, "\n");
	}

	// y corner
	for (int i = 0; i < Y_CORNER_AXIS; i += 1) {
		for (int j = 0; j < MAX_Y_OFF; j += 1) {
			fprintf(fp, "%u ", (*Ptr++) & 0x0FF);
		}

		fprintf(fp, "\n");
	}
	// } else {
	// //x corner
	// for(int i=0; i<12; i+=1) {
	// for(int j=0; j<MAX_X_OFF; j+=1) {
	// fprintf(fp, "%u ", (*Ptr++) & 0x0FF);
	// }
	//
	// fprintf(fp, "\n");
	// }
	// }

	fprintf(fp, "%d\n", (*Ptr++) & 0x0FF); // CheckSum
}

void __fastcall ADProcess::LogDebugADData(FILE *fp, DebugHead_t *Head,
	uint8_t *Data) {
	DebugHeadV2_t *HeadV2 = (DebugHeadV2_t*)Head;
	// uint32_t HeadLen = 11 + (Head->XAxis + Head->YAxis) * 2;
	uint8_t *Ptr = Data;

	if (!fp)
		return;

	fprintf(fp, "\nHead Type: %d, ", Head->Type);
	fprintf(fp, "FrameNum: %d, ", Head->FrameNum);
	fprintf(fp, "Fps: %d, ", Head->Fps);
	fprintf(fp, "ScanTime: %d, ", Head->ScanTime);
	fprintf(fp, "XAxis: %d, ", Head->XAxis);
	fprintf(fp, "YAxis: %d, ", Head->YAxis);
	fprintf(fp, "XLeds: %d, ", Head->XLeds);
	fprintf(fp, "YLeds: %d\n", Head->YLeds);

	fprintf(fp, "\nOffsets:\n");
	for (int i = 0; i < Head->XAxis + Head->YAxis; i += 1) {
		fprintf(fp, "%d ", Head->Offsets[i]);
	}

	if ((Head->Type & 0xF0) == REPORT_MODE_VERSION2) {
		fprintf(fp, "\n");
		fprintf(fp, "\nAUX Offsets:\n");

		for (int i = Head->XAxis + Head->YAxis;
		i < (Head->XAxis + Head->YAxis) * 2; i += 1) {
			fprintf(fp, "%d ", HeadV2->Offsets[i]);
		}

		// HeadLen = 11 + (Head->XAxis + Head->YAxis) * 4;
	}

	fprintf(fp, "\n\n");

	for (int i = 0; i < Head->XAxis; i += 1) {
		fprintf(fp, "XAxis %d Led Signals:\n", i + 1);
		for (int j = 0; j < Head->XLeds; j += 1) {
			fprintf(fp, "%.2X ", (*Ptr++) & 0xFF);
		}

		fprintf(fp, "\n\n");
	}

	for (int i = 0; i < Head->YAxis; i += 1) {
		fprintf(fp, "YAxis %d Led Signals:\n", i + 1);
		for (int j = 0; j < Head->YLeds; j += 1) {
			fprintf(fp, "%.2X ", (*Ptr++) & 0x0FF);
		}

		fprintf(fp, "\n\n");
	}

	fprintf(fp,
		"\n--------------------------------- Frame End! -------------------------------------\n\n");
}

void __fastcall ADProcess::CloseLogFile(void) {
	if (Logfp != NULL) {
		fclose(Logfp);
	}

	LastLogSta = false;
}

void __fastcall ADProcess::LogData(void *DebugHead, uint8_t *Data) {
	uint8_t LogType = ((DebugHead_t*)DebugHead)->Type & 0xF0;

	if (MainForm->ToolButtonRecord->Down != LastLogSta) {
		if (LastLogSta == false) // open
		{
			time_t ltime;
			time(&ltime);

			if ((Logfp =
				fopen(AnsiString(ExtractFilePath(Application->ExeName) +
				String(ltime) + ".txt").c_str(), "w")) == NULL) {
				ShowMessage(L"log 文件创建失败！");
				return;
			}

			if (LogType == REPORT_MODE_VERSION1) {
				DebugHead_t Head = *((DebugHead_t*)DebugHead);

				// LogFile Magic and Version
				fprintf(Logfp, "%s\n",
					AnsiString(LogFileMagicStringV1).c_str());

				// LogMaxData
				Head.Type = 0x4;
				LogADData(Logfp, &Head, MtBound.MaxVolt[0], true);

				// LogEnvData
				Head.Type = 0x1;
				LogADData(Logfp, &Head, MtBound.UsualVolt[0], true);
			}
			else if (LogType == REPORT_MODE_VERSION2) {
				DebugHeadV2_t HeadV2 = *((DebugHeadV2_t*)DebugHead);

				// LogFile Magic and Version
				fprintf(Logfp, "%s\n",
					AnsiString(LogFileMagicStringV4).c_str());

				// LogMaxData
				HeadV2.Type = 0x4 | REPORT_MODE_VERSION2;
				LogADData(Logfp, (DebugHead_t *)&HeadV2,
					MtBound.MaxVolt[0], true);

				// LogEnvData
				HeadV2.Type = 0x1 | REPORT_MODE_VERSION2;
				LogADData(Logfp, (DebugHead_t *)&HeadV2,
					MtBound.UsualVolt[0], true);

				// Logad
				HeadV2.Type = 0x0 | REPORT_MODE_VERSION2;
				LogADData(Logfp, (DebugHead_t *)&HeadV2,
					MtBound.AxisAd[0], true);
			}
			else if (LogType == REPORT_MODE_VERSION4) {
				DebugHeadV2_t HeadV2 = *((DebugHeadV2_t*)DebugHead);

				// LogFile Magic and Version
				fprintf(Logfp, "%s\n",
					AnsiString(LogFileMagicStringV4).c_str());

				// LogMaxData
				HeadV2.Type = 0x4 | REPORT_MODE_VERSION2;
				LogADData(Logfp, (DebugHead_t *)&HeadV2,
					MtBound.MaxVolt[0], true);

				// LogEnvData
				HeadV2.Type = 0x1 | REPORT_MODE_VERSION2;
				LogADData(Logfp, (DebugHead_t *)&HeadV2,
					MtBound.UsualVolt[0], true);

				// Logad
				HeadV2.Type = 0x0 | REPORT_MODE_VERSION2;
				LogADData(Logfp, (DebugHead_t *)&HeadV2,
					MtBound.AxisAd[0], true);
			}
			else if (LogType == REPORT_MODE_VERSION5) {
				DebugHeadV2_t HeadV2 = *((DebugHeadV2_t*)DebugHead);

				// LogFile Magic and Version
				fprintf(Logfp, "%s\n",
					AnsiString(LogFileMagicStringV5).c_str());

				// LogMaxData
				HeadV2.Type = 0x4 | REPORT_MODE_VERSION5;
				LogADData_V5_Cur(Logfp, (DebugHead_t *)&HeadV2,
					MtBound.MaxVolt);

				// LogEnvData
				HeadV2.Type = 0x1 | REPORT_MODE_VERSION5;
				LogADData_V5_Cur(Logfp, (DebugHead_t *)&HeadV2,
					MtBound.UsualVolt);

				// Logad
				HeadV2.Type = 0x0 | REPORT_MODE_VERSION5;
				LogADData_V5_Cur(Logfp, (DebugHead_t *)&HeadV2, MtBound.AxisAd);
			}
		}
		else { // close
			fclose(Logfp);
			Logfp = NULL;
		}
	}

	if (MainForm->ToolButtonRecord->Down == true) {
		if (Logfp != NULL) {
			// Log Current Data
			if (LogType == REPORT_MODE_VERSION1 ||
				LogType == REPORT_MODE_VERSION2) {
				LogADData(Logfp, DebugHead, Data, false);
			}
			else if (LogType == REPORT_MODE_VERSION4) {
				LogADDataV4(Logfp, DebugHead, Data, false);
			}
			else if (LogType == REPORT_MODE_VERSION5) {
				LogADData_V5(Logfp, DebugHead, Data);
			}
		}
	}

	LastLogSta = MainForm->ToolButtonRecord->Down;
}

void __fastcall ADProcess::ParamUpdate(DebugHead_t *dh) {
	if (dh->XAxis != LastDebugHead.XAxis || dh->YAxis != LastDebugHead.YAxis) {
		goto __ParamUpdate_Update;
	}

	if (dh->XLeds != LastDebugHead.XLeds || dh->YLeds != LastDebugHead.YLeds) {
		goto __ParamUpdate_Update;
	}

	for (int i = 0; i < dh->XAxis + dh->YAxis; i += 1) {
		if (dh->Offsets[i] != LastDebugHead.Offsets[i]) {
			goto __ParamUpdate_Update;
		}
	}

	return;

__ParamUpdate_Update:

	uint32_t HeadLen = 11;

	HeadLen += (dh->XAxis + dh->YAxis)*2;
	memcpy(&LastDebugHead, dh, HeadLen);

	FrameInfo.UpdateFrameInfo(dh, CornerSignals);

	// initial
	Initial_Algorithm();
}

void __fastcall ADProcess::ParamUpdateV2(DebugHeadV2_t *dh) {
	if (dh->XAxis != LastDebugHeadV2.XAxis ||
		dh->YAxis != LastDebugHeadV2.YAxis) {
		goto __ParamUpdate_Update_V2;
	}

	if (dh->XLeds != LastDebugHeadV2.XLeds ||
		dh->YLeds != LastDebugHeadV2.YLeds) {
		goto __ParamUpdate_Update_V2;
	}

	for (int i = 0; i < (dh->XAxis + dh->YAxis) * 2; i += 1) {
		if (dh->Offsets[i] != LastDebugHeadV2.Offsets[i]) {
			goto __ParamUpdate_Update_V2;
		}
	}

	if (CornerSignals != LastCornerSignals) {
		goto __ParamUpdate_Update_V2;
	}

	return;

__ParamUpdate_Update_V2:

	uint32_t HeadLen = 11;

	HeadLen += (dh->XAxis + dh->YAxis)*4;
	memcpy(&LastDebugHeadV2, dh, HeadLen);

	LastCornerSignals = CornerSignals;

	FrameInfo.UpdateFrameAuxInfo(dh, CornerSignals);
	// FrameInfo.UpdateFrameAuxInfo(dh);

	// initial
	Initial_Algorithm();
}

// -------------------------------------------------------------------------------------
void __fastcall ADProcess::ProcessFrame(uint8_t *Data, uint32_t Len) {
	uint32_t FrameLen, i;
	uint8_t CheckSum = 0;
	uint8_t *ActualData;
	uint32_t PayLoad;
	DebugHead_t *dh = (DebugHead_t*)Data;
	uint32_t HeadLen;
	uint8_t Type;

	// if(Data[0] == 0x0A)
	// {
	// point_t Point[MAX_POINTS];
	// uint8_t *Ptr = Data+1;
	//
	// for(int i=0; i<6 && i<MAX_POINTS; i+=1)
	// {
	// switch(*Ptr++)
	// {
	// case 0x07:
	// Point[i].Status = AREA_MOVE;
	// break;
	//
	// case 0x04:
	// Point[i].Status = AREA_UPED;
	// break;
	//
	// default :
	// Point[i].Status = AREA_DEAD;
	// break;
	// }
	//
	// Point[i].ID = *Ptr++;
	//
	// Point[i].x = ((uint16_t *)Ptr)[0];	Ptr+=2;
	// Point[i].y = ((uint16_t *)Ptr)[0];   Ptr+=2;
	// Point[i].width = ((uint16_t *)Ptr)[0];   Ptr+=2;
	// Point[i].height = ((uint16_t *)Ptr)[0];   Ptr+=2;
	// }
	//
	// FormPoints->DrawPoints(6, Point);
	//
	// return;
	// }

	FILE *fp = MainForm->fplogframe;

	// check sum
	for (i = 0; i < Len - 1; i += 1) {
		CheckSum += Data[i];
	}

	if (fp) {
		fprintf(fp, "\nFrame Length: %d\n", Len);
		fprintf(fp, "CheckSum: 0x%.2X\n", CheckSum);
	}

	if (Data[Len - 1] != CheckSum) {
		// CheckSum Error;
		MainForm->StatusBar->SimpleText =
			L"校验和错误!" + String((uint32_t)Data[Len - 1]) + L":" +
			String((uint32_t)CheckSum);
		if (fp) {
			fprintf(fp, "CheckSum Error!\n");
		}
		return;
	}

	if (fp)
		fprintf(fp, "CheckSum OK!\n");

	MainForm->Frames += 1;

	DebugDataVer = Data[0] & 0xF0;
	Type = Data[0] & 0x0F;

	if (DebugDataVer == REPORT_MODE_VERSION1) {
		HeadLen = 11 + (dh->XAxis + dh->YAxis) * 2;
		ParamUpdate(dh);

		if (fp) {
			fprintf(fp, "Debug head Version2 (0x%.2X)\n", DebugDataVer);
		}
	}
	else if (DebugDataVer == REPORT_MODE_VERSION2 ||
		DebugDataVer == REPORT_MODE_VERSION4 ||
		DebugDataVer == REPORT_MODE_VERSION5) {
		HeadLen = 11 + (dh->XAxis + dh->YAxis) * 4;
		ParamUpdateV2((DebugHeadV2_t *)Data);
	}
	else
		return;

	ActualData = Data + HeadLen;
	PayLoad = Len - 1 - HeadLen;

	// log data
	MainForm->CTime = dh->ScanTime;
	if (MainForm->MTime < MainForm->CTime)
		MainForm->MTime = MainForm->CTime;

	if (DebugDataVer == REPORT_MODE_VERSION1 ||
		DebugDataVer == REPORT_MODE_VERSION2 ||
		DebugDataVer == REPORT_MODE_VERSION4 ||
		DebugDataVer == REPORT_MODE_VERSION5) {
		if ((Data[0] & 0x0F) == 0 || (Data[0] & 0x0F) == 1 ||
			(Data[0] & 0x0F) == 4) // AD data
		{
			LogData(dh, ActualData);
			LogDebugADData(fp, dh, ActualData);
		}
		else if ((Data[0] & 0x0F) == 3) {
			// LogRealPoints(dh, ActualData);
		}

		switch (Type) {
		case 0: // ad value
			CornerSignals = Len - 1 - HeadLen - dh->XAxis * dh->XLeds -
				dh->YAxis * dh->YLeds;
			ProcessAD(DebugDataVer, Type, ActualData, PayLoad);
			break;

		case 1: // usual voltage initial
			CornerSignals = Len - 1 - HeadLen - dh->XAxis * dh->XLeds -
				dh->YAxis * dh->YLeds;
			memcpy(MtBound.UsualVolt[0], ActualData, XLEDS*XAXIS + YLEDS*YAXIS);
			if (DebugDataVer == REPORT_MODE_VERSION5) {
				ActualData += XLEDS * XAXIS + YLEDS * YAXIS;
				for (int i = 0; i < 4; i += 1) {
					memcpy(MtBound.UsualVolt[i + XAXIS + YAXIS], ActualData,
						MAX_X_OFF);
					ActualData += MAX_X_OFF;
				}

				for (int i = 0; i < 4; i += 1) {
					memcpy(MtBound.UsualVolt[i + XAXIS + YAXIS + 4], ActualData,
						MAX_Y_OFF);
					ActualData += MAX_Y_OFF;
				}
			}
			break;

		case 3: // real points
			// MtIdentify.AreaNum = (dh->FrameNum >> 4) & 0x0F;
			// MtTrace.RealPoints = dh->FrameNum & 0x0F;
			ProcessPoint(Type, ActualData, PayLoad);
			break;

		case 4:
			// if(FrameInfo.Info.YLeds != 0) {
			CornerSignals = Len - 1 - HeadLen - dh->XAxis * dh->XLeds -
				dh->YAxis * dh->YLeds;
			memcpy(MtBound.MaxVolt[0], ActualData, XLEDS*XAXIS + YLEDS*YAXIS);
			memcpy(MtBound.AxisAd[0], ActualData, XLEDS*XAXIS + YLEDS*YAXIS);

			if (DebugDataVer == REPORT_MODE_VERSION5) {
				ActualData += XLEDS * XAXIS + YLEDS * YAXIS;
				for (int i = 0; i < X_CORNER_AXIS; i += 1) {
					memcpy(MtBound.MaxVolt[i + XAXIS + YAXIS], ActualData,
						MAX_X_OFF);
					memcpy(MtBound.AxisAd[i + XAXIS + YAXIS], ActualData,
						MAX_X_OFF);
					ActualData += MAX_X_OFF;
				}

				for (int i = 0; i < Y_CORNER_AXIS; i += 1) {
					memcpy(MtBound.MaxVolt[i + XAXIS + YAXIS + X_CORNER_AXIS],
						ActualData, MAX_Y_OFF);
					memcpy(MtBound.AxisAd[i + XAXIS + YAXIS + X_CORNER_AXIS],
						ActualData, MAX_Y_OFF);
					ActualData += MAX_Y_OFF;
				}
			}
			// } else {
			// memcpy(MtBound.MaxVolt[0], ActualData, XLEDS*XAXIS);
			// memcpy(MtBound.AxisAd[0], ActualData, XLEDS*XAXIS);
			//
			// if(DebugDataVer == REPORT_MODE_VERSION5) {
			// ActualData += XLEDS*XAXIS;
			// for(int i=0; i<12; i+=1) {
			// memcpy(MtBound.MaxVolt[i + XAXIS+YAXIS], ActualData, MAX_X_OFF);
			// memcpy(MtBound.AxisAd[i + XAXIS+YAXIS], ActualData, MAX_X_OFF);
			// ActualData += MAX_X_OFF;
			// }
			// }
			// }

			// memcpy(MtBound.AdBuf[0], ActualData, XLEDS);
			// memset(MtBound.AxisAd[0], 0, PayLoad);
			MtBound.SignalQuality(DebugDataVer);
			MtBound.first_run = 100;
			break;
		}
	}
}

void __fastcall ADProcess::Update_Debug_Scan_Data(uint8_t *Data, uint32_t Len) {
	softscan_ctrl_t ssc;
	uint32_t ptr = 0;

	memcpy(MtBound.AdBuf[0], Data, XAXIS*XLEDS + YAXIS*YLEDS);

	ptr = XAXIS * XLEDS + YAXIS * YLEDS;

	for (int i = 0; i < CORNER_AXIS; i += 1) {
		int start, signals, id;
		if (i < X_CORNER_AXIS) {
			start = 0;
			signals = MAX_X_OFF;
			id = i + XAXIS + YAXIS;
		}
		else {
			start = 0;
			signals = MAX_Y_OFF;
			id = i + XAXIS + YAXIS;
		}

		start = 0;
		memcpy(&MtBound.AdBuf[id][start], Data + ptr, signals);
		ptr += signals;
	}
}

void __fastcall ADProcess::ProcessAD(uint8_t ver, uint8_t EnvFlag,
	uint8_t *Data, uint32_t Len) {
	if (ver == REPORT_MODE_VERSION2) {
		memcpy(MtBound.AdBuf[0], Data, Len);
	}
	else if (ver == REPORT_MODE_VERSION5) {
		Update_Debug_Scan_Data(Data, Len);
	}
	else {
		return;
	}

	if (EnvFlag) { // initial
		MtBound.InitialUsualVolt(ver);
	}
	else {
		MtBound.GetAxisVolt(ver);
		MtBound.MakeInterval(ver);
	}
}

void __fastcall ADProcess::ProcessPoint(uint8_t EnvFlag, uint8_t *Data,
	uint32_t Len) {
	point_t Point[MAX_POINTS];
	uint8_t Points = Data[0];
	uint8_t *Ptr = Data + 1;

	memset(Point, 0, sizeof(Point));

	for (int i = 0; i < Points && i < MAX_POINTS; i += 1) {
		Point[i].x = ((uint16_t*)Ptr)[0];
		Ptr += 2;
		Point[i].y = ((uint16_t*)Ptr)[0];
		Ptr += 2;
		Point[i].width = ((uint16_t*)Ptr)[0];
		Ptr += 2;
		Point[i].height = ((uint16_t*)Ptr)[0];
		Ptr += 2;
		Point[i].ID = *Ptr++;
		Point[i].Status = (AREA_STA) * Ptr++;

		// FillTraceBuf(&Point[i]);
	}

	// fill tracebuf

	FormPoints->DrawPoints(Points, Point);
	// FormCalibrate->ProcessPoints(Points, Point);
	// LogRealPoint(Point, Points);

	for (int i = 0; i < Points && i < MAX_POINTS; i += 1) {
		Point[i].x = Point[i].x * MAX_X_SCALE / 32768;
		Point[i].y = Point[i].y * MAX_Y_SCALE / 32768;

		Point[i].width = Point[i].width * MAX_X_SCALE / 32768;
		Point[i].height = Point[i].height * MAX_Y_SCALE / 32768;
	}
}

#pragma package(smart_init)
