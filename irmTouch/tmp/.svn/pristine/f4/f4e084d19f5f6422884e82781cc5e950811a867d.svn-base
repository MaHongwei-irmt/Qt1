//---------------------------------------------------------------------------


#pragma hdrstop

#include "hex.h"
#include "rom.h"
#include "stdlib.h"
#include "untBeatCode.h"
#include "FirmwareUpdate.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

HEXFile hexfile;

//---------------------------------------------------------------------------
//handle hex file
//---------------------------------------------------------------------------
__fastcall HEXFile::HEXFile(void)
{

}

int __fastcall HEXFile::GetLine(FILE *fp, unsigned char *Buf, int BufLen, int FileLen)
{
	int LineLen = 0;
	unsigned char ch;
	char FileData[8];
	static char Data[8];
	static int BufPtr = 7;

	if(ftell(fp) == 0)
	{
    	fread(FileData, 1, 8, fp);
		Des_Go(Data, FileData, 8, des_key, sizeof(des_key), DECRYPT);
		BufPtr = 0;
	}

	if(ftell(fp) < FileLen - 128)
	{
		for(LineLen=0; LineLen<BufLen; LineLen+=1)
		{
			if(BufPtr >= 7)
			{
				fread(FileData, 1, 8, fp);
				Des_Go(Data, FileData, 8, des_key, sizeof(des_key), DECRYPT);
				BufPtr = 0;
			}

			ch = Data[BufPtr++];

			if(ch != '\n' && ch != '\r')
			{
				Buf[LineLen] = ch;
			}
			else if (ch == '\n')
			{
				break;
			}
		}
	}

	return LineLen;
}

int __fastcall HEXFile::GetLine(FILE *fp,unsigned char *Buf,int BufLen)
{
	int LineLen = 0;
	unsigned char ch;

	if(!feof(fp))
	{
		for(LineLen=0; LineLen<BufLen; LineLen+=1)
		{
			ch = fgetc(fp);
			if(ch != '\n')
			{
				Buf[LineLen] = ch;
			}
			else
			{
				break;
			}
		}
	}

	return LineLen;
}

//---------------------------------------------------------------------------
long __fastcall HEXFile::HexToInt(unsigned char *Hex,int Len)
{
	int i;
	long Data=0;

	for(i=0; i<Len; i+=1)
	{
		Data *= 16;

		if(Hex[i] >= '0' && Hex[i]<= '9')
		{
			Data += (int)(Hex[i] - '0');
		}
		else if(Hex[i] >= 'A' && Hex[i]<= 'F')
		{
			Data += (int)(Hex[i] - 'A' + 10);
		}
		else if(Hex[i] >= 'a' && Hex[i]<= 'f')
		{
			Data += (int)(Hex[i] - 'a' + 10);
		}
		else
		{
			return -1;
		}
	}

	return Data;
}

int __fastcall HEXFile::GetFileData(String File)
{
	FILE *fp;
	unsigned char LineBuf[1024];
	long FileDataLen = 0;
	long LineDataLen;
	long StartAdd;
	long OffsetAdd = 0;
	int  LineFormat;
	long i;
	unsigned char Data;
	unsigned char CheckSum;
	int Error = 0;
	int len = MAIN_SIZE;
	unsigned char *Buf = UpdateCtrl.MainFlash;
	int FileSize;

	String Msg;

	UpdateCtrl.Valid = 0;


	for(i=0; i<MAIN_SIZE/2; i+=1)
	{
		((short *)UpdateCtrl.MainFlash)[i] = rand();
	}

	//memset(UpdateCtrl.MainFlash, 0xFF, MAIN_SIZE);

	FormUpdate->Label237->Caption = "文件大小：0KB";

	if(ExtractFileExt(File) == ".bin")
	{
		if((fp = fopen(AnsiString(File).c_str(),"rb"))==NULL)
		{
			Msg = "文件：" + File + "\n打开失败!";
			Application->MessageBox(Msg.c_str(), L"ERROR", MB_OK|MB_ICONERROR);
			return -1;
		}
	}
	else
	{
		if((fp = fopen(AnsiString(File).c_str(),"rt"))==NULL)
		{
			Msg = "文件：" + File + "\n打开失败!";
			Application->MessageBox(Msg.c_str(), L"ERROR", MB_OK|MB_ICONERROR);
			return -1;
		}
	}

	fseek(fp, 0, SEEK_END);
	FileSize = UpdateCtrl.FileSize = ftell(fp);

	rewind(fp);

	//memset(Buf, 0xFF, len);
	//while(GetLine(fp, LineBuf, 1024)>9)

	UpdateCtrl.Valid = MODE_MAIN_FLASH;
	UpdateCtrl.StartAdd = 0;
	UpdateCtrl.ROMStartAdd = FLASH_BASE_ADD + MAIN_SIZE;
	UpdateCtrl.ROMEndAdd = 0;

	while(1)
	{
		if(ExtractFileExt(File) == ".bin")
		{
			if(GetLine(fp, LineBuf, 1024, FileSize) <= 9)
			{
				break;
			}
		}
		else
		{
			if(GetLine(fp, LineBuf, 1024) <= 9)
			{
				break;
			}
		}

		//Line += 1;

		if(LineBuf[0] != ':')
		{
			Msg = "File:\n"+File+"\n 无效的HEX文件格式!\n";
			Application->MessageBox(Msg.c_str(), L"ERROR", MB_OK|MB_ICONERROR);
			Error = 1;

			break;
		}
		else
		{
			LineDataLen = HexToInt(LineBuf+1,2);
			StartAdd = HexToInt(LineBuf+3,4);
			LineFormat = HexToInt(LineBuf+7,2);

			if(LineFormat == 0)
			{
				StartAdd += OffsetAdd;

//				if(UpdateCtrl.ROMStartAdd == 0)
//				{
//					UpdateCtrl.ROMStartAdd = StartAdd + FLASH_BASE_ADD;
//				}

				if(UpdateCtrl.ROMStartAdd > (uint32_t)StartAdd + FLASH_BASE_ADD)
				{
					UpdateCtrl.ROMStartAdd = StartAdd + FLASH_BASE_ADD;
				}

				if(StartAdd + LineDataLen <= len)
				{

					if(StartAdd + LineDataLen > FileDataLen)
					{
						FileDataLen = StartAdd + LineDataLen;
					}

					CheckSum = 0;
					for(i = 0; i<4; i+=1)
					{
						Data = (unsigned char)HexToInt(LineBuf+1+i*2,2);
						CheckSum += Data;
					}
					
					for(i = 0; i<LineDataLen; i+=1)
					{
						Data = (unsigned char)HexToInt(LineBuf+9+i*2,2);
						*(Buf + i + StartAdd) = Data;
						CheckSum += Data;
					}

					if(UpdateCtrl.ROMEndAdd < StartAdd + LineDataLen + FLASH_BASE_ADD)
					{
						UpdateCtrl.ROMEndAdd = StartAdd + LineDataLen + FLASH_BASE_ADD;
					}

					CheckSum += (char)HexToInt(LineBuf+9+LineDataLen*2,2);

					if((CheckSum&0xFF) != 0)
					{
						Msg = "校验和出错，文件可能已经损坏!\n";
						Application->MessageBox(Msg.c_str(), L"ERROR", MB_OK|MB_ICONERROR);
						Error = 1;
						break;
					}
				}
				else
				{
					Msg = "地址超出范围!\n";
					Application->MessageBox(Msg.c_str(), L"ERROR", MB_OK|MB_ICONERROR);
					Error = 1;

					break;
				}
			}
			else if(LineFormat == 01)		//end-of-file record
			{
				break;
			}
			else if(LineFormat == 02)  		//extended segment address record
			{
				break;
			}
			else if(LineFormat == 04)		//extended linear address record
			{
				OffsetAdd = HexToInt(LineBuf+9,4);
				OffsetAdd <<= 16;

				if((uint32_t)OffsetAdd >= FLASH_BASE_ADD)
				{
					OffsetAdd -= FLASH_BASE_ADD;
				}
			}
		}
	}

	Msg.sprintf(L"文件大小：%.3fKB", (float)UpdateCtrl.FileSize/1000.0);
	FormUpdate->Label237->Caption = Msg;

	Msg.sprintf(L"起始地址：0x%8X", UpdateCtrl.ROMStartAdd);
	FormUpdate->Label239->Caption = Msg;

	Msg.sprintf(L"终止地址：0x%8X", UpdateCtrl.ROMEndAdd);
	FormUpdate->Label240->Caption = Msg;

	Msg.sprintf(L"固件大小：%.3fKB", (float)(UpdateCtrl.ROMEndAdd - UpdateCtrl.ROMStartAdd)/1000.0);
	FormUpdate->Label257->Caption = Msg;

	UpdateCtrl.AppStartAdd = UpdateCtrl.ROMStartAdd - FLASH_BASE_ADD;
	UpdateCtrl.AppEndAdd = UpdateCtrl.ROMEndAdd - FLASH_BASE_ADD;
	UpdateCtrl.AppSize = UpdateCtrl.ROMEndAdd - UpdateCtrl.ROMStartAdd;
	UpdateCtrl.FlashSize = (UpdateCtrl.AppEndAdd + FormUpdate->FlashParam.EndAreaSize)/1024;
	
	fclose(fp);

	if(Error == 1)
	{
		return -1;
	}
	else
	{
		return FileDataLen;
	}
}
