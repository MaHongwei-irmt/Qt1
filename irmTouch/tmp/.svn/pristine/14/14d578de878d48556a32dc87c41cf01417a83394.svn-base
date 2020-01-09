//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CalibPerformance.h"
#include "Calibrate.h"
#include "Config.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormCalibPerform *FormCalibPerform;

static Graphics::TBitmap   *SrcBitmap = NULL;
//---------------------------------------------------------------------------
__fastcall TFormCalibPerform::TFormCalibPerform(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormCalibPerform::ResizeBitMap(void)
{
	if(SrcBitmap == NULL)
	{
		SrcBitmap = new  Graphics::TBitmap;
		SrcBitmap->PixelFormat = pf24bit;
	}

	if(SrcBitmap->Width != Image->Width || SrcBitmap->Height != Image->Height)
	{
		SrcBitmap->Width = Image->Width;
		SrcBitmap->Height = Image->Height;

		SrcBitmap->Canvas->Brush->Color = clWhite;
		SrcBitmap->Canvas->FillRect(Rect(0,0,SrcBitmap->Width,SrcBitmap->Height));
	}
}


void __fastcall TFormCalibPerform::FormResize(TObject *Sender)
{
	Calib_Info_t *ci = &FormCalibrate->CalibInfo;
	Calibration_t *cp = &FormConfig->CalibParam;
	float *k = cp->Calib[0].k;
	Calib_Points_Info_t *pt = ci->Calib;
	int xPxis = Image->Width;
	int yPxis = Image->Height;
	int x,y;

	ResizeBitMap();

	SrcBitmap->Canvas->Brush->Color = clWhite;
	SrcBitmap->Canvas->FillRect(Rect(0, 0, xPxis, yPxis));

	for(int i=0; i<ci->TotalPoints; i+=1)
	{
		//reference point
		SrcBitmap->Canvas->Pen->Color = clBlack;
		x = pt[i].x0 * xPxis / 32768;
		y = pt[i].y0 * yPxis / 32768;

		SrcBitmap->Canvas->MoveTo(x-15 , y);
		SrcBitmap->Canvas->LineTo(x+15 , y);

		SrcBitmap->Canvas->MoveTo(x , y-15);
		SrcBitmap->Canvas->LineTo(x , y+15);

		SrcBitmap->Canvas->Brush->Style = bsClear;
		SrcBitmap->Canvas->Ellipse(x-10, y-10, x+10, y+10);

		//sampling point
		SrcBitmap->Canvas->Pen->Color = clRed;
		x = pt[i].x1 * xPxis / 32768;
		y = pt[i].y1 * yPxis / 32768;

		SrcBitmap->Canvas->MoveTo(x-15 , y);
		SrcBitmap->Canvas->LineTo(x+15 , y);

		SrcBitmap->Canvas->MoveTo(x , y-15);
		SrcBitmap->Canvas->LineTo(x , y+15);

		SrcBitmap->Canvas->Brush->Style = bsClear;
		SrcBitmap->Canvas->Ellipse(x-10, y-10, x+10, y+10);

		//calibrated point
		SrcBitmap->Canvas->Pen->Color = clLime;
		x = k[0] * pt[i].x1 + k[1] * pt[i].y1 + k[2] + 0.5;
		y = k[3] * pt[i].x1 + k[4] * pt[i].y1 + k[5] + 0.5;
		x = x * xPxis / 32768;
		y = y * yPxis / 32768;

		SrcBitmap->Canvas->MoveTo(x-15 , y);
		SrcBitmap->Canvas->LineTo(x+15 , y);

		SrcBitmap->Canvas->MoveTo(x , y-15);
		SrcBitmap->Canvas->LineTo(x , y+15);

		SrcBitmap->Canvas->Brush->Style = bsClear;
		SrcBitmap->Canvas->Ellipse(x-10, y-10, x+10, y+10);
	}

	Image->Picture->Assign(SrcBitmap);
	Image->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TFormCalibPerform::FormShow(TObject *Sender)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------
