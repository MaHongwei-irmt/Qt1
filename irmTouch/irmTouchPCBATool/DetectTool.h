//---------------------------------------------------------------------------

#ifndef DetectToolH
#define DetectToolH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>

// ---------------------------------------------------------------------------
class TDetectTool : public TForm {
__published: // IDE-managed Components
	TRichEdit *ctlInfoShow;
	TTimer *Timer1;
	TPanel *Panel1;
	TRichEdit *RichEdit1;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDblClick(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);

private: // User declarations

public: // User declarations
	__fastcall TDetectTool(TComponent* Owner);

	int mainActive;
	int debug;
};

// ---------------------------------------------------------------------------
extern PACKAGE TDetectTool *DetectTool;
// ---------------------------------------------------------------------------
#endif
