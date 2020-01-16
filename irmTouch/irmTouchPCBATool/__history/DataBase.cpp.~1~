//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DataBase.h"
#include "config.h"

#include <stdio.h>
#include <time.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormViewDataBase *FormViewDataBase;
//---------------------------------------------------------------------------
__fastcall TFormViewDataBase::TFormViewDataBase(TComponent* Owner)
	: TForm(Owner)
{
}

void __fastcall TFormViewDataBase::AddRecord(uint8_t *Data, uint32_t Index)
{
	String Msg = "";
	tm *TimeDate;
	char *Ptr = Data;
	time_t ShipTime;

	StringGrid1->Cells[0][Index+1] = String(Index +1);

	//CPU ID
	for(int i=0; i<12; i+=1)
	{
		Msg.cat_printf(L"%.2X", (*Ptr++)&0x0FF);
	}

	StringGrid1->Cells[8][Index+1] = Msg;

	//Time
	memcpy(&ShipTime, Ptr, sizeof(ShipTime)); Ptr += sizeof(ShipTime);

	TimeDate = localtime(&ShipTime);
	Msg.printf(L"%d-%d-%d", TimeDate->tm_year+1900, TimeDate->tm_mon+1, TimeDate->tm_mday);
	StringGrid1->Cells[1][Index+1] = Msg;

	Msg.printf(L"%.2d:%.2d:%.2d", TimeDate->tm_hour, TimeDate->tm_min, TimeDate->tm_sec);
	StringGrid1->Cells[2][Index+1] = Msg;

	//Serial number
	StringGrid1->Cells[4][Index+1] = String(Ptr);
	Ptr += strlen(Ptr) + 1;

	//Multi-Touch Screen Type
	StringGrid1->Cells[3][Index+1] = String(Ptr);
	Ptr += strlen(Ptr) + 1;

	//shipping code
	StringGrid1->Cells[10][Index+1] = String(Ptr);
	Ptr += strlen(Ptr) + 1;

	//Customer information
	StringGrid1->Cells[11][Index+1] = String(Ptr);
	Ptr += strlen(Ptr) + 1;

	//hardware version
	StringGrid1->Cells[5][Index+1] = String(Ptr);
	Ptr += strlen(Ptr) + 1;

	//softwate version
	StringGrid1->Cells[7][Index+1] = String(Ptr);
	Ptr += strlen(Ptr) + 1;

	//Bootloader version
	StringGrid1->Cells[6][Index+1] = String(Ptr);
	Ptr += strlen(Ptr) + 1;

	//CPU Type
	if(String(Ptr).Length() > 0)
	{
		StringGrid1->Cells[9][Index+1] = String(Ptr);
		//Ptr += strlen(Ptr) + 1;
	}
	else
	{
		if(StringGrid1->Cells[6][Index+1] == "10.6")
			StringGrid1->Cells[9][Index+1] = "F405RGT6";
		else if(StringGrid1->Cells[5][Index+1] == "5.40" || StringGrid1->Cells[5][Index+1] == "6.50")
			StringGrid1->Cells[9][Index+1] = "F103RCT6";
		else
			StringGrid1->Cells[9][Index+1] = "";
	}
	
}

//---------------------------------------------------------------------------
void __fastcall TFormViewDataBase::ViewDataBase(void)
{
	//
	FILE *fp;

	StringGrid1->ColCount = 12;

	StringGrid1->Cells[0][0] = "";
	StringGrid1->Cells[1][0] = "发货日期";
	StringGrid1->Cells[2][0] = "发货时间";
	StringGrid1->Cells[3][0] = "型号";
	StringGrid1->Cells[4][0] = "序列号";
	StringGrid1->Cells[5][0] = "硬件版本";
	StringGrid1->Cells[6][0] = "Boot版本";
	StringGrid1->Cells[7][0] = "固件版本";
	StringGrid1->Cells[8][0] = "CPU ID";
	StringGrid1->Cells[9][0] = "CPU 型号";
	StringGrid1->Cells[10][0] = "发货代码";
	StringGrid1->Cells[11][0] = "客户";

	StringGrid1->ColWidths[0] = 40;
	StringGrid1->ColWidths[1] = 80;
	StringGrid1->ColWidths[2] = 70;
	StringGrid1->ColWidths[3] = 70;
	StringGrid1->ColWidths[4] = 140;
	StringGrid1->ColWidths[5] = 70;
	StringGrid1->ColWidths[6] = 70;
	StringGrid1->ColWidths[7] = 70;
	StringGrid1->ColWidths[8] = 220;
	StringGrid1->ColWidths[9] = 100;
	StringGrid1->ColWidths[10] = 100;
	StringGrid1->ColWidths[11] = 200;

	if((fp = fopen(AnsiString(FileName).c_str(), "rb")) != NULL)
	{
		long FileLen;
		int Records;
		long len;
		int i;
		uint8_t Buf[RecordSize];
		typedef time_t time_t_[2];
		time_t_ *Times;

		fseek(fp, 0, SEEK_END);
		FileLen = ftell(fp);

		rewind(fp);

		StringGrid1->RowCount = FileLen/RecordSize + 2;
		this->Caption = "数据库浏览 - " + String(FileLen/RecordSize);

		Records = FileLen / RecordSize;

		Times = new time_t_[Records];

		if(StringGrid1->RowCount < 10) StringGrid1->RowCount = 10;

		if(Times != NULL)
		{
			for(i=0; i<Records; i+=1)
			{
				fseek(fp, i*RecordSize+12, SEEK_SET);
				fread(Times[i], 1, sizeof(time_t), fp);
				Times[i][1] = i;
			}

			time_t tTemp;
			bool changed;

			for(int i=0; i<Records-1; i+=1)
			{
				changed = false;

				for(int j=0; j<Records-1; j+=1)
				{
					if(Times[j][0]>Times[j+1][0])
					{
						changed = true;

						tTemp = Times[j][0];
						Times[j][0] = Times[j+1][0];
						Times[j+1][0] = tTemp;

						tTemp = Times[j][1];
						Times[j][1] = Times[j+1][1];
						Times[j+1][1] = tTemp;
					}
				}

				if(changed == false)
				{
					break;
				}
			}

			for(int i=0; i<Records; i+=1)
			{
				fseek(fp, Times[i][1]*RecordSize, SEEK_SET);

				fread(Buf, 1, sizeof(Buf), fp);
				AddRecord(Buf, i);
			}

			delete[] Times;
		}
		else
		{
			ShowMessage(L"分配内存失败，无法按时间排序！");

			Records = 0;

			while(FileLen)
			{
				len = fread(Buf, 1, sizeof(Buf), fp);

				AddRecord(Buf, Records);
	
				Records += 1;
				FileLen -= len;
			}
		}

		fclose(fp);
	}
}
void __fastcall TFormViewDataBase::FormShow(TObject *Sender)
{
	ViewDataBase();
}
//---------------------------------------------------------------------------
void __fastcall TFormViewDataBase::StringGrid1DrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
	((TStringGrid*)Sender)->Canvas->Brush->Style = bsSolid;
	((TStringGrid*)Sender)->Canvas->FillRect(Rect); // 使用当前的brush填充矩形，覆盖目前cell中的内容显示 6
	((TStringGrid*)Sender)->Canvas->Brush->Style = bsClear;

	//StringGrid1->Canvas->FillRect(Rect); 			// 使用当前的brush填充矩形，覆盖目前cell中的内容显示 6

	DrawText(((TStringGrid*)Sender)->Canvas->Handle,
	((TStringGrid*)Sender)->Cells[ACol][ARow].c_str(),
	-1,
	(RECT*)&Rect,
	DT_SINGLELINE | DT_VCENTER | DT_CENTER);
}
//---------------------------------------------------------------------------
void __fastcall TFormViewDataBase::FormDeactivate(TObject *Sender)
{
	this->Close();	
}
//---------------------------------------------------------------------------
void __fastcall TFormViewDataBase::N1Click(TObject *Sender)
{
	FILE *fp;

	if((fp = fopen("test.csv", "w")) == NULL)
	{
		ShowMessage(L"无法创建输出文件！");
		return;
	}

	for(int i=0; i<StringGrid1->RowCount; i+=1)
	{
//		fprintf(fp, "%s,", StringGrid1->Cells[0][i].c_str());
//		fprintf(fp, "%s,", StringGrid1->Cells[1][i].c_str());
//		fprintf(fp, "%s,", StringGrid1->Cells[3][i].c_str());
//		fprintf(fp, "%s\n", StringGrid1->Cells[11][i].c_str());

		for(int j=0; j<StringGrid1->ColCount; j+=1){
			if(j<StringGrid1->ColCount-1)
				fprintf(fp, "\"%s\",", AnsiString(StringGrid1->Cells[j][i]).c_str());
			else
				fprintf(fp, "\"%s\"\n", AnsiString(StringGrid1->Cells[j][i]).c_str());
		}
	}

	fclose(fp);

	ShowMessage(L"共导出 " + String(StringGrid1->RowCount) + L" 条记录！");
}
//---------------------------------------------------------------------------

void __fastcall TFormViewDataBase::N4Click(TObject *Sender)
{
	///
	FILE *fp;
	String Customer;
	int Records = 0;

	if(StringGrid1->Row < 1)
	{
		ShowMessage(L"请选择客户!");
		return;
	}

	Customer = StringGrid1->Cells[11][StringGrid1->Row];

	String FileName = Customer + ".csv";

	if((fp = fopen(AnsiString(FileName).c_str(), "w")) == NULL)
	{
		ShowMessage(L"无法创建输出文件！");
		return;
	}

	for(int j=0; j<StringGrid1->ColCount; j+=1)
	{
		if(j<StringGrid1->ColCount-1)
			fprintf(fp, "\"%s\",", StringGrid1->Cells[j][0].c_str());
		else
			fprintf(fp, "\"%s\"\n", StringGrid1->Cells[StringGrid1->ColCount-1][0].c_str());
	}

	for(int i=0; i<StringGrid1->RowCount; i+=1)
	{
		if(StringGrid1->Cells[StringGrid1->ColCount-1][i] == Customer)
		{
			fprintf(fp, "\"%s\",", String(Records+1));

			for(int j=1; j<StringGrid1->ColCount; j+=1)
			{
				if(j<StringGrid1->ColCount-1)
					fprintf(fp, "\"%s\",", StringGrid1->Cells[j][i].c_str());
				else
					fprintf(fp, "\"%s\"\n", StringGrid1->Cells[StringGrid1->ColCount-1][i].c_str());
			}

//			{
//				fprintf(fp, "%s,", StringGrid1->Cells[0][i].c_str());
//				fprintf(fp, "%s,", StringGrid1->Cells[1][i].c_str());
//				fprintf(fp, "%s,", StringGrid1->Cells[3][i].c_str());
//				fprintf(fp, "%s\n", StringGrid1->Cells[10][i].c_str());
//			}

			Records += 1;
		}
	}

	fclose(fp);

	ShowMessage(L"导出客户：" + Customer + "\n共" + String(Records) + "条记录。");

}
//---------------------------------------------------------------------------

void __fastcall TFormViewDataBase::StringGrid1Click(TObject *Sender)
{

	//ShowMessage(String(StringGrid1->Col) + "," + String(StringGrid1->Row));

}
//---------------------------------------------------------------------------







