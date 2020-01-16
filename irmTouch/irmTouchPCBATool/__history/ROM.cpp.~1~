//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ROM.h"
#include "stdio.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormROM *FormROM;

UPDATE_CTRL  UpdateCtrl;
//---------------------------------------------------------------------------
__fastcall TFormROM::TFormROM(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TFormROM::ShowRomData(TStringGrid *StringGrid, unsigned char *Data, unsigned len, unsigned StartAdd)
{
	int i;
	String Msg;
	unsigned short *RomBuf;

	RomBuf = (unsigned short *)Data;

	StringGrid->RowCount = (len+15)/16+1;

	StringGrid->Cells[0][0] = "Address";

	for(i=0; i<16; i+=2)
	{
		StringGrid->Cells[i/2+1][0] = Msg.sprintf(L"%.2X", i);
	}

	for(unsigned j=0; j<(len+15)/16; j+=1)
	{
		StringGrid->Cells[0][j+1] = Msg.sprintf(L"%.4X", j*16+StartAdd);

		for(i=1; i<9; i+=1)
		{
			StringGrid->Cells[i][j+1] = Msg.sprintf(L"%.4X", RomBuf[j*8+i-1]);
		}
	}
}
void __fastcall TFormROM::FormShow(TObject *Sender)
{
	ShowRomData(StringGrid1, UpdateCtrl.MainFlash + UpdateCtrl.AppStartAdd, UpdateCtrl.AppSize, UpdateCtrl.AppStartAdd);
	//ShowRomData(StringGrid2, UpdateCtrl.EEPROM, EEPROM_SIZE, 0);
	//ShowRomData(StringGrid3, UpdateCtrl.Config, CONFIG_SIZE, 0x300000);
}
//---------------------------------------------------------------------------
