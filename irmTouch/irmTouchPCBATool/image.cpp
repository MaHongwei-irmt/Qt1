// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <stdint.h>

#include "image.h"
#include "Main.h"
#include "math.h"
#include "FrameInfo.h"
#include "About.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFormPoints * FormPoints;

static Graphics::TBitmap *SrcBitmap = NULL;

const TColor PointCl[] = {clBlack, clRed, clLime, clBlue, clFuchsia, clAqua,
	// clYellow,
	clDkGray, };

const TColor PointCl_[] = {clBlack, clBlack, clBlack, clBlack, clBlack, clBlack,
	// clYellow,
	clBlack, };

// ---------------------------------------------------------------------------
__fastcall TFormPoints::TFormPoints(TComponent* Owner) : TForm(Owner) {

}

int XOff_ = 0;
int Ratio_ = 1;

#define ToXPxis(a)	((a)*Ratio_*TouchAreaWidth/MAX_X_SCALE)
#define XOffsetPix()(XOff_ * SCALE * Ratio_ * TouchAreaWidth/MAX_X_SCALE)
#define ToYPxis(a)	((a)*TouchAreaHeight/MAX_Y_SCALE)

// #define Flip_Y(y) (Image1->Height - (y))
#define Flip_Y(y) (y)

void __fastcall TFormPoints::DrawPoints(int Points, point_t(&pt)[]) {
	int x1, x2, y1, y2, Width, Height;
	String Msg;
	int xMgLeft, xMgRight, yMgUp, yMgDown;
	int TouchAreaWidth;
	int TouchAreaHeight;

	if (SrcBitmap == NULL) {
		SrcBitmap = new Graphics::TBitmap;
		SrcBitmap->PixelFormat = pf24bit;
	}

	if (SrcBitmap->Width != Image1->Width ||
		SrcBitmap->Height != Image1->Height) {
		SrcBitmap->Width = Image1->Width;
		SrcBitmap->Height = Image1->Height;

		Canvas->Brush->Color = clWhite;
		Canvas->FillRect(Rect(0, 0, SrcBitmap->Width, SrcBitmap->Height));
	}

	if (ReDraw->Checked) {
		Canvas->Brush->Color = clWhite;
		Canvas->FillRect(Rect(0, 0, SrcBitmap->Width, SrcBitmap->Height));
	}

	xMgLeft = SrcBitmap->Width * X_MARGIN_LEFT_SCALE /
		(MAX_X_COOD - MIN_X_COOD);
	xMgRight = SrcBitmap->Width * X_MARGIN_RIGHT_SCALE /
		(MAX_X_COOD - MIN_X_COOD);
	yMgUp = SrcBitmap->Width * Y_MARGIN_UP_SCALE / (MAX_Y_COOD - MIN_Y_COOD);
	yMgDown = SrcBitmap->Width * Y_MARGIN_DOWN_SCALE /
		(MAX_Y_COOD - MIN_Y_COOD);

	Canvas->Pen->Width = 1;
	Canvas->Pen->Style = psSolid;

	Canvas->Pen->Color = clRed;
	Canvas->MoveTo(xMgLeft, yMgDown);
	Canvas->LineTo(xMgLeft, SrcBitmap->Height - yMgUp);
	Canvas->LineTo(SrcBitmap->Width - xMgRight, SrcBitmap->Height - yMgUp);
	Canvas->LineTo(SrcBitmap->Width - xMgRight, yMgDown);
	Canvas->LineTo(xMgLeft, yMgDown);

	TouchAreaWidth = Image1->Width - xMgLeft - xMgRight;
	TouchAreaHeight = Image1->Height - yMgUp - yMgDown;

	Canvas->Pen->Color = clBlack;

	Msg.printf(L"PointsImage %d - ", Points);
	Msg.cat_printf(L" Zoom:%d, Xoff: %d leds", Ratio_, XOff_);
	this->Caption = Msg;

	for (int i = 0; i < Points; i += 1) {
		if (FixSize1->Checked == true) {
			Width = 8 * 32768 / TouchAreaWidth / Ratio_;
			Height = 8 * 32768 / TouchAreaHeight;
		}
		else {
			Width = pt[i].width;
			Height = pt[i].height;
		}

		if (pt[i].Status == AREA_MOVE || pt[i].Status == AREA_UPED) {
			if (pt[i].x < 0)
				pt[i].x = 0;

			x1 = pt[i].x - Width / 2;
			x2 = pt[i].x + Width / 2;
			y1 = pt[i].y - Height / 2;
			y2 = pt[i].y + Height / 2;

			x1 = x1 * TouchAreaWidth / 32768 + xMgLeft;
			x2 = x2 * TouchAreaWidth / 32768 + xMgLeft;

			x1 = x1 * Ratio_ +
				XOff_ * SCALE * TouchAreaWidth * Ratio_ / MAX_X_SCALE;
			x2 = x2 * Ratio_ +
				XOff_ * SCALE * TouchAreaWidth * Ratio_ / MAX_X_SCALE;

			y1 = y1 * TouchAreaHeight / 32768 + yMgDown;
			y2 = y2 * TouchAreaHeight / 32768 + yMgDown;

			Msg.cat_printf(L"%.3d:(%4d,%4d)", pt[i].ID, (x1 + x2) / 2,
				(y1 + y2) / 2);

			if (DrawLine->Checked == true) {
				int ox, oy;

				if (LastPoint[i].Status == AREA_MOVE) {
					ox = LastPoint[i].x;
					oy = LastPoint[i].y;

					ox = ox * TouchAreaWidth / 32768 + xMgLeft;
					oy = oy * TouchAreaHeight / 32768 + yMgDown;

					Canvas->Pen->Color =
						PointCl[pt[i].ID % (sizeof(PointCl) / sizeof(TColor))];
					Canvas->MoveTo(ox, oy);
				}
			}

			if (DrawLine->Checked == false) {
				if (pt[i].Status == AREA_MOVE) {
					Canvas->Brush->Color =
						PointCl[pt[i].ID % (sizeof(PointCl) / sizeof(TColor))];
					Canvas->Ellipse(x1, Flip_Y(y1), x2, Flip_Y(y2));
				}
				else {
					Canvas->Brush->Color = clWhite;

					// if(FixSize1->Checked == true)
					Canvas->Rectangle(x1, Flip_Y(y1), x2, Flip_Y(y2));
					// ((x1+x2)/2-10, (y1+y2)/2-10, (x1+x2)/2+10, (y1+y2)/2+10);
				}
			}
			else {
				if (LastPoint[i].Status == AREA_MOVE &&
					(pt[i].Status == AREA_MOVE || pt[i].Status == AREA_UPED)) {
					int nx, ny;

					nx = pt[i].x * TouchAreaWidth / 32768 + xMgLeft;
					ny = pt[i].y * TouchAreaHeight / 32768 + yMgDown;

					if (FixSize1->Checked == true) {
						Canvas->Pen->Width = 1;
					}
					else {
						Canvas->Pen->Width = 2;
					}

					Canvas->LineTo(nx, ny);
				}
			}

			LastPoint[i] = pt[i];

			if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0) {
				break;
			}

			Canvas->Pen->Width = 1;
		}
	}

	return;
}

#define LED_WIDYH_PIXS	10
#define REVERSE_Y(y) (ImgHeight - (y))
#define REVERSE_X(x) (ImgWidth - (x))

// ---------------------------------------------------------------------------
void __fastcall TFormPoints::Image1Click(TObject *Sender) {
	SrcBitmap->Canvas->Brush->Color = clWhite;
	SrcBitmap->Canvas->FillRect(Rect(0, 0, SrcBitmap->Width,
		SrcBitmap->Height));

	Image1->Picture->Assign(SrcBitmap);
	Image1->Refresh();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPoints::MenuItemClick(TObject *Sender) {
	((TMenuItem*)Sender)->Checked = !((TMenuItem*)Sender)->Checked;
}

void __fastcall TFormPoints::StayOnTop1Click(TObject *Sender) {
	StayOnTop1->Checked = !StayOnTop1->Checked;

	if (StayOnTop1->Checked == true) {
		FormStyle = Vcl::Forms::fsStayOnTop;
	}
	else {
		FormStyle = Vcl::Forms::fsNormal;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPoints::FormKeyPress(TObject *Sender, char &Key) {
	if (Key == 'n' || Key == 'N') {
		MainForm->PlayOneFrame();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPoints::HighLightClick(TObject *Sender) {
	MenuItemClick(Sender);
}
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

int MouseIsDown = 0;
int MouseX;
int XoffDown;

void __fastcall TFormPoints::FormMouseWheelDown(TObject *Sender,
	TShiftState Shift, TPoint &MousePos, bool &Handled) {
	if (Ratio_ > 1) {
		Ratio_ -= 1;
		String Msg;
		Msg.printf(L"Zoom:%d, Xoff: %d leds", Ratio_, XOff_);
		this->Caption = Msg;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormPoints::FormMouseWheelUp(TObject *Sender,
	TShiftState Shift, TPoint &MousePos, bool &Handled) {
	if (Ratio_ < 20) {
		Ratio_ += 1;
		String Msg;
		Msg.printf(L"Zoom:%d, Xoff: %d leds", Ratio_, XOff_);
		this->Caption = Msg;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormPoints::Image1MouseDown(TObject *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y) {
	MouseIsDown = 1;
	MouseX = X;
	XoffDown = XOff_;
}
// ---------------------------------------------------------------------------

void __fastcall TFormPoints::Image1MouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y) {
	if (MouseIsDown) {
		XOff_ = XoffDown + (X - MouseX);

		if (XOff_ > 0) {
			XOff_ = 0;
		}

		String Msg;
		Msg.printf(L"Zoom:%d, Xoff: %d leds", Ratio_, XOff_);
		this->Caption = Msg;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormPoints::Image1MouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y) {
	MouseIsDown = 0;
}
// ---------------------------------------------------------------------------

void __fastcall TFormPoints::FormShow(TObject *Sender) {
	point_t pt[7];

	for (int i = 0; i < 7; i += 1) {
		pt[i].x = 1000 * i;
		pt[i].y = 1000 * i;
		pt[i].width = 1000;
		pt[i].height = 1000;
		pt[i].ID = i;
	}

	DrawPoints(7, pt);
}
// ---------------------------------------------------------------------------
