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

typedef	struct							//��ʵ��ṹ��
{
	int32_t x;							//��ʵ������
	int32_t y;
	int32_t width;						//���
	int32_t height;				    //�߶�
	uint8_t ID;							//����ID

	int32_t vx;
	int32_t vy;
	int32_t ax;
	int32_t ay;

	AREA_STA Status;
}point_t;

typedef	struct area_t							//��ṹ��
{
	area_t *NextSon;
	area_t *Next;                      //����ʹ��һά��������ʹ�ö�ά����Traceʹ��һά����

	int32_t x1;					   	   //�㷶Χ����
	int32_t x2;
	int32_t y1;
	int32_t y2;

	AREA_STA Status;				   //���Ƿ���Ч
	AREA_STA MatchSta;				   //���Ƿ���Ч
	uint32_t AA;                       //���
	float proj0;
	float proj1;
	uint8_t No;						   //ԭʼ������
	uint8_t ItvID;                     //Դ���ĸ�����
	uint8_t Flag;
	uint8_t PassCount;                 //ʶ��ʱΪԴ�Եİ�������
	uint8_t MergeCnt;				   //�ϲ��İ�������
}area_t;                               //��Ҫ�ĳ�����ṹ fix me!!!!!!!!!!!!!!!!!!!!!!!!!!!!

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
