// ----------------------------------------------------------------------------
#ifndef AboutH
#define AboutH
// ----------------------------------------------------------------------------
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <StdCtrls.hpp>
#include <Controls.hpp>
#include <Forms.hpp>
#include <Graphics.hpp>
#include <Classes.hpp>
#include <Windows.hpp>
#include <System.hpp>

// ----------------------------------------------------------------------------
class TAboutBox : public TForm {
__published:
	TPanel *Panel1;
	TImage *ProgramIcon;
	TLabel *ProductName;
	TLabel *Version;
	TLabel *Copyright;
	TButton *Button1;
	TEdit *Edit1;
	TLabel *Label1;
	TLabel *Label2;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);

private:
public:
	virtual __fastcall TAboutBox(TComponent *Owner);
	bool __fastcall CheckLic(void);

	uint32_t Registered;
	uint32_t UnRegistered;

	String CID;
	String Lic;
};

// ----------------------------------------------------------------------------
extern TAboutBox *AboutBox;
// ----------------------------------------------------------------------------
#endif
