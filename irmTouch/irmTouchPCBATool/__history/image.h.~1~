//---------------------------------------------------------------------------

#ifndef imageH
#define imageH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------
#include "frameinfo.h"
#include <Menus.hpp>

typedef enum {	AREA_DEAD=0,
				AREA_ALIVE,
				AREA_BORN,
				AREA_IMMORTAL,
				AREA_MATCHED,
				AREA_UNMATCHED,
				AREA_DOWN,
				AREA_UP,
				AREA_UPED,
				AREA_MOVE
		} AREA_STA;

typedef	struct							//真实点结构体
{
	int32_t x;							//真实点坐标
	int32_t y;
	int32_t width;						//宽度
	int32_t height;				    //高度
	uint8_t ID;							//触摸ID

	int32_t vx;
	int32_t vy;
	int32_t ax;
	int32_t ay;

	AREA_STA Status;
}point_t;

typedef	struct area_t							//点结构体
{
	area_t *NextSon;
	area_t *Next;                      //迭代使用一维链表，过滤使用二维链表，Trace使用一维链表

	int32_t x1;					   	   //点范围坐标
	int32_t x2;
	int32_t y1;
	int32_t y2;

	AREA_STA Status;				   //点是否有效
	AREA_STA MatchSta;				   //点是否有效
	uint32_t AA;                       //面积
	float proj0;
	float proj1;
	uint8_t No;						   //原始区域编号
	uint8_t ItvID;                     //源自哪个凹坑
	uint8_t Flag;
	uint8_t PassCount;                 //识别时为源自的凹坑数量
	uint8_t MergeCnt;				   //合并的凹坑数量
}area_t;                               //需要改成链表结构 fix me!!!!!!!!!!!!!!!!!!!!!!!!!!!!

class TFormPoints : public TForm
{
__published:	// IDE-managed Components
	TImage *Image1;
	TPopupMenu *PopupMenu1;
	TMenuItem *FixSize1;
	TMenuItem *StayOnTop1;
	TMenuItem *N1;
	TMenuItem *RealPoint;
	TMenuItem *DrawLine;
	TMenuItem *ReDraw;
	void __fastcall Image1Click(TObject *Sender);
	void __fastcall StayOnTop1Click(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, char &Key);
	void __fastcall MenuItemClick(TObject *Sender);
	void __fastcall HighLightClick(TObject *Sender);
	void __fastcall FormMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall FormMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall Image1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall Image1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall Image1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall FormShow(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TFormPoints(TComponent* Owner);
	void  __fastcall DrawPoints(int Points, point_t (&pt)[]);

	void __fastcall Draw_Initial_Param(void);
	void __fastcall DrawDebugArea(int areas, area_t (&area)[]);

	point_t LastPoint[MAX_POINTS];

	int xMgLeft, xMgRight, yMgUp, yMgDown;
	int TouchAreaWidth;
	int TouchAreaHeight;
	int ImgWidth, ImgHeight;

};
//---------------------------------------------------------------------------
extern PACKAGE TFormPoints *FormPoints;
//---------------------------------------------------------------------------
#endif
