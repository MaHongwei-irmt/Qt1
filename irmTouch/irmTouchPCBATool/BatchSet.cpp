// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BatchSet.h"
#include "Config.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormBatchSet *FormBatchSet;

// ---------------------------------------------------------------------------
__fastcall TFormBatchSet::TFormBatchSet(TComponent* Owner) : TForm(Owner) {
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchSet::ComboBox1Change(TObject *Sender) {
	if (ComboBox1->ItemIndex == 0 || ComboBox1->ItemIndex == 2) {
		ComboBox2->Clear();

		for (int i = 0; i < 8; i += 1) {
			ComboBox2->Items->Add(i);
		}

		if (ComboBox1->ItemIndex == 0) {
			ComboBox2->ItemIndex = 2;
		}
		else {
			ComboBox2->ItemIndex = 7;
		}
	}
	else {
		ComboBox2->Clear();

		for (int i = 0; i < 4; i += 1) {
			ComboBox2->Items->Add(i);
		}

		ComboBox2->ItemIndex = 2;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormBatchSet::Button1Click(TObject *Sender) {
	int StartAdd = _wtoi(Edit1->Text.t_str());
	int Boards = _wtoi(Edit2->Text.t_str());
	int Type = ComboBox1->ItemIndex;
	int Param = ComboBox2->ItemIndex;

	String Msg = "";

	if (StartAdd <= 0 || StartAdd > MAX_BOARDS) {
		Msg.cat_printf(L"起始地址超出范围[1,%d]\n", MAX_BOARDS);
	}

	if (Boards <= 0 || Boards + StartAdd > MAX_BOARDS + 1) {
		Msg.cat_printf(L"设定板数超出范围[1,%d]\n", MAX_BOARDS - StartAdd + 1);
	}

	if (Type < 0) {
		Msg += "选择正确的设定项目\n";
	}

	if (Param < 0) {
		Msg += "选择正确的设定参数\n";
	}

	if (Msg.Length() == 0) {
		Edit1->Text = String(StartAdd);
		Edit2->Text = String(Boards);

		for (int i = 0; i < Boards; i += 1) {
			FormConfig->StringGrid1->Cells[i + StartAdd][Type + 1] =
				String(Param);
		}
	}
	else {
		ShowMessage(Msg);
	}
}
// ---------------------------------------------------------------------------
