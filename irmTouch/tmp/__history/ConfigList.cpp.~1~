//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ConfigList.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormConfigList *FormConfigList;
//---------------------------------------------------------------------------
__fastcall TFormConfigList::TFormConfigList(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormConfigList::FormShow(TObject *Sender)
{
	ListBox1->Clear();

	if(ini)
	{
		ini->ReadSections(ListBox1->Items);

		for(int i=0; i<ListBox1->Count; i+=1)
		{
			ListBox1->Selected[i] = false;
		}

		SelCount = 0;
		Sels = "";
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormConfigList::Button1Click(TObject *Sender)
{
	Sels = "";
	SelCount  = ListBox1->SelCount;
	
	for(int i=0; i<ListBox1->Count; i+=1)
	{
		if(ListBox1->Selected[i] == true)
		{
			Sels += ListBox1->Items->Strings[i];
			Sels += "\n";
		}
	}

	this->Close();
}
//---------------------------------------------------------------------------
