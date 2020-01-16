// ---------------------------------------------------------------------------

#ifndef ConfigListH
#define ConfigListH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include <inifiles.hpp>

// ---------------------------------------------------------------------------
class TFormConfigList : public TForm {
__published: // IDE-managed Components
	TListBox *ListBox1;
	TButton *Button1;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);

private: // User declarations
public: // User declarations
	__fastcall TFormConfigList(TComponent* Owner);

	TIniFile *ini;
	String Sels;
	int SelCount;
};

// ---------------------------------------------------------------------------
extern PACKAGE TFormConfigList *FormConfigList;
// ---------------------------------------------------------------------------
#endif
