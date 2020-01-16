//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormAdd.h"
#include "config.h"
#include <inifiles.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAddStdParam *FormAddStdParam;
//---------------------------------------------------------------------------
__fastcall TFormAddStdParam::TFormAddStdParam(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormAddStdParam::Button1Click(TObject *Sender)
{
	TIniFile *ini = new TIniFile(ExtractFilePath(Application->ExeName)+"screen.ini");

	if(ini)
	{
		if(ini->SectionExists(EditName->Text) == true)
		{
			if(MessageBox(NULL, L"配置已经存在，需要更新吗?", L"确认更新", MB_OKCANCEL | MB_ICONWARNING) == IDCANCEL)
			{
				delete ini;
				return;
			}
		}

		FormConfig->AddStdConfig(ini, EditName->Text);
		delete ini;
		Close();
	}	
}
//---------------------------------------------------------------------------
void __fastcall TFormAddStdParam::Button2Click(TObject *Sender)
{
	this->Close();	
}
//---------------------------------------------------------------------------
