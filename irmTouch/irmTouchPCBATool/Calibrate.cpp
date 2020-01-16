// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Calibrate.h"
#include "math.h"

#include "Main.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormCalibrate *FormCalibrate;

static Graphics::TBitmap *SrcBitmap = NULL;

// ---------------------------------------------------------------------------
__fastcall TFormCalibrate::TFormCalibrate(TComponent* Owner) : TForm(Owner) {
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalibrate::ResizeBitMap(void) {
	if (SrcBitmap == NULL) {
		SrcBitmap = new Graphics::TBitmap;
		SrcBitmap->PixelFormat = pf24bit;
	}

	if (SrcBitmap->Width != ImageCalibrate->Width ||
		SrcBitmap->Height != ImageCalibrate->Height) {
		SrcBitmap->Width = ImageCalibrate->Width;
		SrcBitmap->Height = ImageCalibrate->Height;

		SrcBitmap->Canvas->Brush->Color = clWhite;
		SrcBitmap->Canvas->FillRect(Rect(0, 0, SrcBitmap->Width,
			SrcBitmap->Height));
	}
}

void __fastcall TFormCalibrate::DrawBack(void) {
	Calib_Info_t *ci = &CalibInfo;
	int xPxis = ImageCalibrate->Width;
	int yPxis = ImageCalibrate->Height;
	int x, y;

	ResizeBitMap();

	SrcBitmap->Canvas->Brush->Color = clWhite;
	SrcBitmap->Canvas->FillRect(Rect(0, 0, xPxis, yPxis));

	SrcBitmap->Canvas->Pen->Color = clGray;
	SrcBitmap->Canvas->Pen->Width = 1;
	SrcBitmap->Canvas->Pen->Style = psSolid;

	for (uint8_t i = 0; i < ci->TotalPoints; i += 1) {
		if (i < PtNo) {
			SrcBitmap->Canvas->Pen->Color = clLtGray;
		}
		else {
			SrcBitmap->Canvas->Pen->Color = clBlack;
		}

		x = ci->Calib[i].x0 * xPxis / 32768;
		y = ci->Calib[i].y0 * yPxis / 32768;

		SrcBitmap->Canvas->MoveTo(x - 15, y);
		SrcBitmap->Canvas->LineTo(x + 15, y);

		SrcBitmap->Canvas->MoveTo(x, y - 15);
		SrcBitmap->Canvas->LineTo(x, y + 15);

		SrcBitmap->Canvas->Brush->Style = bsClear;
		SrcBitmap->Canvas->Ellipse(x - 10, y - 10, x + 10, y + 10);
	}

	SrcBitmap->Canvas->Font->Color = clBlue;
	SrcBitmap->Canvas->TextOutW(xPxis / 2 - 100, 5, L"按ESC键或者两指触摸超过3秒退出校准!");
	SrcBitmap->Canvas->TextOutW(xPxis / 2 - 140, 20,
		L"Press 'ESC' key or dual-touch more than 3 seconds to Exit!");

}

// ---------------------------------------------------------------------------
void __fastcall TFormCalibrate::FormResize(TObject *Sender) {
	DrawBack();

	UpdatePoint(0, 0);
}

void __fastcall TFormCalibrate::UpdatePoint(int No, int Percent) {
	Calib_Info_t *ci = &CalibInfo;
	Calib_Points_Info_t *cp;
	int xPxis = ImageCalibrate->Width;
	int yPxis = ImageCalibrate->Height;
	float sx, sy, ex, ey;

	if (this->Active == false)
		return;
	if (No < 0 || No >= ci->TotalPoints)
		return;

	cp = &ci->Calib[No];

	DrawBack();

	// int y = No/cp->xPoints;
	// int x = No%cp->xPoints;
	//
	// y = yStartPxis + y*yStepPxis;
	// x = xStartPxis + x*xStepPxis;

	int x = cp->x0 * xPxis / 32768;
	int y = cp->y0 * yPxis / 32768;

	SrcBitmap->Canvas->Brush->Color = clWhite;
	SrcBitmap->Canvas->FillRect(Rect(x - 35, y - 35, x + 60, y + 35));

	if (Percent != 0) {
		ex = x;
		ey = y - 30;
		sx = x + 30 * cos(-2 * 3.1415926 * float(Percent) / 100.0 +
			3.1415926 / 2);
		sy = y - 30 * sin(-2 * 3.1415926 * float(Percent) / 100.0 +
			3.1415926 / 2);

		SrcBitmap->Canvas->Brush->Style = bsSolid;
		SrcBitmap->Canvas->Brush->Color = clLime;

		SrcBitmap->Canvas->Pen->Color = clLime;
		SrcBitmap->Canvas->Pen->Width = 3;
		SrcBitmap->Canvas->Pie(x - 20, y - 20, x + 20, y + 20, sx, sy, ex, ey);
	}

	SrcBitmap->Canvas->Pen->Color = clRed;
	SrcBitmap->Canvas->Pen->Width = 2;
	SrcBitmap->Canvas->Pen->Style = psSolid;

	SrcBitmap->Canvas->MoveTo(x - 30, y);
	SrcBitmap->Canvas->LineTo(x + 30, y);

	SrcBitmap->Canvas->MoveTo(x, y - 30);
	SrcBitmap->Canvas->LineTo(x, y + 30);

	if (Percent != 0) {
		SrcBitmap->Canvas->MoveTo(x, y);
		SrcBitmap->Canvas->LineTo(sx, sy);
	}

	SrcBitmap->Canvas->Brush->Style = bsClear;
	SrcBitmap->Canvas->Ellipse(x - 20, y - 20, x + 20, y + 20);

#if CALIB_DEBUG == 1
	// calib info
	String Msg;

	x += 35;

	Msg.sprintf("x0: %d", cp->x0*xPxis / 32768);
	SrcBitmap->Canvas->TextOutA(x, y - 25, Msg);

	Msg.sprintf("y0: %d", cp->y0*yPxis / 32768);
	SrcBitmap->Canvas->TextOutA(x, y - 15, Msg);

	Msg.sprintf("x1: %d", cp->x1); // *xPxis/32768
	SrcBitmap->Canvas->TextOutA(x, y, Msg);

	Msg.sprintf("y1: %d", cp->y1); // *yPxis/32768
	SrcBitmap->Canvas->TextOutA(x, y + 10, Msg);

#endif

	ImageCalibrate->Picture->Assign(SrcBitmap);
	ImageCalibrate->Refresh();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalibrate::ImageCalibrateDblClick(TObject *Sender) {
	// this->Close();
	//
	// if(SrcBitmap)
	// {
	// delete SrcBitmap;
	// SrcBitmap = NULL;
	// }
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalibrate::FormShow(TObject *Sender) {
	int Width = Screen->Width;
	int Height = Screen->Height;
	Calib_Info_t *ci = &CalibInfo;

	PtNo = 0;
	Percent = 0;
	PtID = 0;

	InPtr = OutPtr = 0;
	ci->Flag = 0;

	int xStart = xPxiMargin * 32768 / Width;
	int yStart = yPxiMargin * 32768 / Height;
	int xStep = (32768 - xStart * 2) / (ci->xPoints - 1);
	int yStep = (32768 - yStart * 2) / (ci->yPoints - 1);
	int x, y;

	if (ci->xPoints < 2)
		ci->xPoints = 2;
	else if (ci->xPoints > 6)
		ci->xPoints = 6;

	if (ci->yPoints < 2)
		ci->yPoints = 2;
	else if (ci->yPoints > 6)
		ci->yPoints = 6;

	ci->TotalPoints = 0;

	for (uint8_t i = 0; i < ci->xPoints; i += 1) {
		x = xStart + i * xStep;

		for (uint8_t j = 0; j < ci->yPoints; j += 1) {
			y = yStart + j * yStep;

			ci->Calib[ci->TotalPoints].x0 = x;
			ci->Calib[ci->TotalPoints].y0 = y;

			ci->TotalPoints += 1;
		}

		if (i < ci->xPoints - 1 && FormConfig->CheckBoxCalibAuxPoint->Checked ==
			true) {
			x += (xStep / 2);

			for (uint8_t j = 0; j < ci->yPoints - 1; j += 1) {
				y = yStart + j * yStep + yStep / 2;

				ci->Calib[ci->TotalPoints].x0 = x;
				ci->Calib[ci->TotalPoints].y0 = y;

				ci->TotalPoints += 1;
			}
		}
	}

	DrawBack();

	UpdatePoint(0, 0);
}

void __fastcall TFormCalibrate::ProcessPoints(int Points, point_t(&pt)[]) {
	//
	CalibPoints_t *cp = &PtFifo[InPtr];

	cp->Points = 0;

	for (int i = 0; i < Points; i += 1) {
		if (pt[i].Status == AREA_MOVE) {
			cp->pt[cp->Points].x = pt[i].x;
			cp->pt[cp->Points].y = pt[i].y;
			cp->pt[cp->Points].ID = pt[i].ID;
			cp->pt[cp->Points].Sta = pt[i].Status;

			cp->Points += 1;

			if (cp->Points >= 10)
				break;
		}
	}

	InPtr = (InPtr + 1) & CALIB_POINT_FIFO_MASK;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalibrate::FormKeyPress(TObject *Sender, char &Key) {
	if (Key == 27) {
		this->Close();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCalibrate::CalibrateFSM(void) {
	static int NoDataCnt = 0;
	static int skips = 0;
	static int DoublePointsTimes = 0;

	if (InPtr == OutPtr) {
		if (NoDataCnt >= 3) {
			Percent = 0;
			PtID = 0;

			UpdatePoint(PtNo, 0);

			// if(PointMode == false)
			{
				uint8_t Mode[2] = {REPORT_MODE_POINT, 0xAA};

				MainForm->HIDSendThread->SubmitCmd(APP_CMD_SW_REPORT, Mode, 2);
			}
		}
		else {
			NoDataCnt += 1;
		}
	}
	else {
		CalibPoints_t *cp;
		Calib_Info_t *ci = &CalibInfo;

		PointMode = true;
		NoDataCnt = 0;

		while (InPtr != OutPtr) {
			cp = &PtFifo[OutPtr];
			OutPtr = (OutPtr + 1) & CALIB_POINT_FIFO_MASK;

			if (cp->Points == 2) {
				DoublePointsTimes += 1;

				if (DoublePointsTimes > 100) {
					DoublePointsTimes = 0;
					this->Close();
				}
			}
			else {
				DoublePointsTimes = 0;
			}

			if (cp->Points != 1)
				continue; // single point

			switch (CalibSta) {
			case WAIT_NEW_POINT:
				if (cp->pt[0].Sta == AREA_MOVE && cp->pt[0].ID != PtID) {
					PtID = cp->pt[0].ID;
					CalibSta = SKIP_BEGIN;
					skips = 0;
					Percent = 0;
				}
				break;

			case SKIP_BEGIN:
				if (cp->pt[0].Sta == AREA_MOVE && cp->pt[0].ID == PtID) {
					skips += 1;

					if (skips >= 5) {
						ci->Calib[PtNo].x1 = cp->pt[0].x;
						ci->Calib[PtNo].y1 = cp->pt[0].y;

						CalibSta = CLC_POS;
						skips = 0;
					}
				}
				else {
					CalibSta = WAIT_NEW_POINT;
					Percent = 0;
				}
				break;

			case CLC_POS:
				if (cp->pt[0].Sta == AREA_MOVE && cp->pt[0].ID == PtID) {
					ci->Calib[PtNo].x1 = (ci->Calib[PtNo].x1 + cp->pt[0].x) / 2;
					ci->Calib[PtNo].y1 = (ci->Calib[PtNo].y1 + cp->pt[0].y) / 2;
				}
				else {
					CalibSta = WAIT_NEW_POINT;
					Percent = 0;
				}
				break;

			}
		}

		if (CalibSta == CLC_POS && cp->Points == 1) {
			Percent += 10;

			if (Percent >= 100) {
				Percent = 0;
				PtNo += 1;
				CalibSta = WAIT_NEW_POINT;

				if (PtNo >= ci->TotalPoints) {
					// close dialog
					ci->Flag |= CALIB_INFO_VALID;
					this->Close();
				}
			}
		}

		UpdatePoint(PtNo, Percent);
	}
}

void __fastcall TFormCalibrate::Timer1Timer(TObject *Sender) {
	if (this->Active == false) {
		return;
	}
	else {
		CalibrateFSM();
	}
}
// ---------------------------------------------------------------------------
