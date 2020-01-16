//---------------------------------------------------------------------------

#ifndef DataBaseH
#define DataBaseH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TFormViewDataBase : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *StringGrid1;
	TPopupMenu *PopupMenuDB;
	TMenuItem *N1;
	TMenuItem *N2;
	TMenuItem *N3;
	TMenuItem *N4;
	TMenuItem *N5;
	TMenuItem *N6;
	TMenuItem *N7;
	TMenuItem *N8;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall StringGrid1DrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
	void __fastcall FormDeactivate(TObject *Sender);
	void __fastcall N1Click(TObject *Sender);
	void __fastcall N4Click(TObject *Sender);
	void __fastcall StringGrid1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormViewDataBase(TComponent* Owner);
	void __fastcall ViewDataBase(void);
	void __fastcall AddRecord(uint8_t *Data, uint32_t Index);

	String FileName;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormViewDataBase *FormViewDataBase;
//---------------------------------------------------------------------------
#endif
