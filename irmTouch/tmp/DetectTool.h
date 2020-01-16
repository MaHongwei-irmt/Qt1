//---------------------------------------------------------------------------

#ifndef DetectToolH
#define DetectToolH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

// ---------------------------------------------------------------------------
class TDetectTool : public TForm {
__published: // IDE-managed Components
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormDblClick(TObject *Sender);

private: // User declarations
	WORD key[4];
    int keyPress;

public: // User declarations
	__fastcall TDetectTool(TComponent* Owner);

	int mainActive;
};

// ---------------------------------------------------------------------------
extern PACKAGE TDetectTool *DetectTool;
// ---------------------------------------------------------------------------
#endif
