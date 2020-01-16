// ---------------------------------------------------------------------------

#ifndef FormAddH
#define FormAddH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

// ---------------------------------------------------------------------------
class TFormAddStdParam : public TForm {
__published: // IDE-managed Components
	TButton *Button1;
	TButton *Button2;
	TEdit *EditName;
	TLabel *Label1;

	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);

private: // User declarations
public: // User declarations
	__fastcall TFormAddStdParam(TComponent* Owner);
};

// ---------------------------------------------------------------------------
extern PACKAGE TFormAddStdParam *FormAddStdParam;
// ---------------------------------------------------------------------------
#endif
