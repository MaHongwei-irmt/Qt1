//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "About.h"

#include "stdio.h"
#include "registry.hpp"

#include "untBeatCode.h"

#include "pbKeyBlob.h"

#include<winsock2.h>
#include<Iphlpapi.h>

//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
#pragma link "iphlpapi.lib"

TAboutBox *AboutBox;

#define IDS_REG_KEY "IRMTEST"
#define SLEN 128
#define BUFFER_SIZE 1024

#define  IDENTIFY_BUFFER_SIZE  512

#define  IDE_ATAPI_IDENTIFY  0xA0
#define  IDE_ATA_IDENTIFY    0xEC
#define  DFP_RECEIVE_DRIVE_DATA   0x0007c088

#pragma pack(1)
//获取硬盘序列号需要的数据结构
typedef struct _IDSECTOR
{
	USHORT  wGenConfig;
    USHORT  wNumCyls;
	USHORT  wReserved;
    USHORT  wNumHeads;
    USHORT  wBytesPerTrack;
    USHORT  wBytesPerSector;
    USHORT  wSectorsPerTrack;
	USHORT  wVendorUnique[3];
    CHAR    sSerialNumber[20];
    USHORT  wBufferType;
    USHORT  wBufferSize;
    USHORT  wECCSize;
	CHAR    sFirmwareRev[8];
    CHAR    sModelNumber[40];
    USHORT  wMoreVendorUnique;
    USHORT  wDoubleWordIO;
    USHORT  wCapabilities;
	USHORT  wReserved1;
    USHORT  wPIOTiming;
    USHORT  wDMATiming;
    USHORT  wBS;
    USHORT  wNumCurrentCyls;
	USHORT  wNumCurrentHeads;
    USHORT  wNumCurrentSectorsPerTrack;
    ULONG   ulCurrentSectorCapacity;
    USHORT  wMultSectorStuff;
	ULONG   ulTotalAddressableSectors;
	USHORT  wSingleWordDMA;
    USHORT  wMultiWordDMA;
    BYTE    bReserved[128];
}IDSECTOR, *PIDSECTOR;

typedef struct _DRIVERSTATUS
{
    BYTE  bDriverError;
    BYTE  bIDEStatus;
    BYTE  bReserved[2];
	DWORD  dwReserved[2];
} DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS;

typedef struct _SENDCMDOUTPARAMS
{
	DWORD         cBufferSize;
    DRIVERSTATUS  DriverStatus;
    BYTE          bBuffer[1];
} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS;

typedef struct _IDEREGS
{
	BYTE bFeaturesReg;
	BYTE bSectorCountReg;
	BYTE bSectorNumberReg;
	BYTE bCylLowReg;
	BYTE bCylHighReg;
	BYTE bDriveHeadReg;
	BYTE bCommandReg;
	BYTE bReserved;
} IDEREGS, *PIDEREGS, *LPIDEREGS;

typedef struct _SENDCMDINPARAMS
{
	DWORD     cBufferSize;
	IDEREGS   irDriveRegs;
	BYTE bDriveNumber;
	BYTE bReserved[3];
	DWORD     dwReserved[4];
	BYTE      bBuffer[1];
} SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS;

#pragma pack()

//--------------------------------------------------------------------- 
__fastcall TAboutBox::TAboutBox(TComponent *Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------
//-------------------------读取硬盘序列号
bool __fastcall CrnGetHDSerialNumber(LPSTR lpBuf)
{
	HANDLE hDevice;

	strcpy(lpBuf, "");

	if(Win32Platform == VER_PLATFORM_WIN32_NT)
		// 获取第一个硬盘，如果有多个，更改PhysicalDrive?就可以了。
		hDevice = CreateFile(L"\\\\.\\PhysicalDrive1",
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
                0,
				0);
	else
		hDevice = CreateFile(L"\\\\.\\SMARTVSD", 0, 0, NULL, CREATE_NEW, 0, 0);

	if(hDevice == INVALID_HANDLE_VALUE)
	{
		//ShowMessage(GetLastError());
		return false;
	}

	SENDCMDINPARAMS scip = { 0 };
    char szBuffer[sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE] = { 0 };
    DWORD dwBytesRet = 0;
	bool bSuccess = false;

    try
    {
        scip.cBufferSize = IDENTIFY_BUFFER_SIZE;

        scip.irDriveRegs.bSectorCountReg = 1;
        scip.irDriveRegs.bSectorNumberReg = 1;
        scip.irDriveRegs.bDriveHeadReg = IDE_ATAPI_IDENTIFY;
		scip.irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;

        if(DeviceIoControl(hDevice,
                DFP_RECEIVE_DRIVE_DATA,
				&scip,
                sizeof(SENDCMDINPARAMS) - 1,
                szBuffer,
                sizeof(szBuffer),
				&dwBytesRet,
                NULL))
        {
            PSENDCMDOUTPARAMS pOut = (PSENDCMDOUTPARAMS)szBuffer;
			PIDSECTOR pIdSec = (PIDSECTOR)(pOut->bBuffer);
            //ChangeByteOrder(pIdSec->sSerialNumber, sizeof(pIdSec->sSerialNumber));
            strncpy(lpBuf, pIdSec->sSerialNumber, 20);   //20->dwBytesRet 会出错
            bSuccess = true;
		}
    }
    __finally
    {
		CloseHandle(hDevice);
    }

    return bSuccess;
}
//--------------------
//获取CUP序列号
String   __fastcall  CPU_ID(void)
{
	unsigned long s1, s2;
	unsigned char vendor_id[]="------------";
	char sel = '1';
	String ID, VernderID, MyCpuID, CPUID1, CPUID2;

	switch(sel)
	{
		case '1':
				__asm
				{
                xor eax,eax
                cpuid
                mov dword ptr vendor_id,ebx
				mov dword ptr vendor_id[+4],edx
                mov dword ptr vendor_id[+8],ecx
               }
				VernderID.sprintf(L"%s-",vendor_id);
			   __asm
			   {
				mov eax,01h
				xor edx,edx
				cpuid
				mov s1,edx
				mov s2,eax
				}
				CPUID1.sprintf(L"%08X%08X",s1,s2);
				__asm
				{
					mov eax,03h
					xor ecx,ecx
					xor edx,edx
					cpuid
					mov s1,edx
					mov s2,ecx
				}
				CPUID2.sprintf(L"%08X%08X",s1,s2);
				break;

		case '2':
			{
				asm
				{
					mov ecx,119h
					rdmsr
					or eax,00200000h
					wrmsr
				}
			}
			//ShowMessage("CPU id is disabled.");
			break;
	}

	MyCpuID = CPUID1;
	ID = MyCpuID;

	return ID;
}

//获得MAc地址
String __fastcall::GetMac(void)
{
	String Msg = "";
	String Mac = "";

	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter;
	ULONG ulOutBufLen;
	pAdapterInfo = (PIP_ADAPTER_INFO)malloc(sizeof(IP_ADAPTER_INFO));
	ulOutBufLen = sizeof(IP_ADAPTER_INFO);

	// 第一次调用GetAdapterInfo获取ulOutBufLen大小
	if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen);
	}

	if ((GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
	{
		pAdapter = pAdapterInfo;

		while (pAdapter)
		{
			Mac.cat_printf(L"%02X%02X%02X%02X%02X%02X",
							pAdapter->Address[0],
							pAdapter->Address[1],
							pAdapter->Address[2],
							pAdapter->Address[3],
							pAdapter->Address[4],
							pAdapter->Address[5]);

			Msg.cat_printf(L"Adapter Name: \t%s\n", pAdapter->AdapterName);
			Msg.cat_printf(L"Adapter Desc: \t%s\n", pAdapter->Description);
			Msg.cat_printf(L"MAC Addr: \t%02X-%02X-%02X-%02X-%02X-%02X\n",
							pAdapter->Address[0],
							pAdapter->Address[1],
							pAdapter->Address[2],
							pAdapter->Address[3],
							pAdapter->Address[4],
							pAdapter->Address[5]);
			Msg.cat_printf(L"IP Address: \t%s\n", pAdapter->IpAddressList.IpAddress.String);
			Msg.cat_printf(L"IP Mask: \t%s\n", pAdapter->IpAddressList.IpMask.String);
			Msg.cat_printf(L"Gateway: \t%s\n\n", pAdapter->GatewayList.IpAddress.String);

			pAdapter = pAdapter->Next;
		}
	}
	else
	{
		Msg.cat_printf(L"Call to GetAdaptersInfo failed.\n");
	}

	free(pAdapterInfo);

	//ShowMessage(Msg);
	return (Mac);
}

//--------------------从硬件信息生成机器码-----------
String __fastcall GenCID(void)
{
	const uint8_t CodeChar[] = "RHX9T4FACMYNPQLJS2573KWVE";
	uint8_t Base = sizeof(CodeChar) - 1;
	uint8_t Code[8];
	uint8_t EnBuf[8];
	uint8_t Buf[8];
	unsigned long long Lic;
	String hcode = "";
	char sz[255];

	ZeroMemory(sz,255);
	CrnGetHDSerialNumber(sz);

	hcode += String(sz).Trim(); 	//硬盘序列号
	hcode += CPU_ID().Trim(); 		//CPU序列号
	hcode += GetMac().Trim();		//Mac地址

	if(hcode.Length() < 8)
	{
		return "";
	}

	//ShowMessage(hcode);
	
	memset(Code, 0xFF, 8);

	for(int i=0; i<hcode.Length(); i+=7)
	{
		memset(Buf, 0xFF, 8);
		Buf[7] = 0xAA<<i;

		if(hcode.Length()-i < 7)
		{
			memcpy(Buf, hcode.c_str()+i, hcode.Length()-i);
		}
		else
		{
			memcpy(Buf, hcode.c_str()+i, 7);
		}

		Des_Go(EnBuf, Buf, 8, des_key, sizeof(des_key), ENCRYPT);

		for(int j=0; j<8; j+=1)
		{
			Code[j] ^= EnBuf[j];
		}
	}

	//Code
	memcpy(&Lic, Code, 8);

	String Msg = "";
    uint8_t Num;

	for(int i=0; i<16; i+=1)
	{
		if(i>0 && (i&0x03) == 0)
		{
			Msg += "-";
		}
		
		Num = Lic % Base;
		Lic /= Base;
		Msg.cat_printf(L"%c", CodeChar[Num]);
	}

	return Msg;
}

//------------------ 从注册表中读取注册码 ------------
String __fastcall GetLic(void)
{
	TRegistry *tReg = new TRegistry;
	String Regkeyname = "SoftWare\\IRMTouch\\HID_Tools\\";
	String Lic = "";

	if(tReg)
	{
		tReg->RootKey = HKEY_LOCAL_MACHINE;
		if(tReg->KeyExists(Regkeyname) && tReg->OpenKey(Regkeyname, false))
		{
			Lic = tReg->ReadString("LIC");
			tReg->CloseKey();
		}

		delete tReg;
	}

	return Lic;
}

void __fastcall WriteLic(String Lic)
{
	TRegistry *tReg = new TRegistry;
	String Regkeyname = "SoftWare\\IRMTouch\\HID_Tools\\";

	if(Lic.Length() == 0)
		return;

	if(tReg)
	{
		tReg->RootKey = HKEY_LOCAL_MACHINE;
		if(tReg->OpenKey(Regkeyname, true))
		{
			tReg->WriteString("LIC", Lic);
			tReg->CloseKey();
		}

		delete tReg;
	}
}

//---------------------------------------------------------------------------
bool __fastcall CheckSign(String Data, uint8_t *Sign)
{
	HCRYPTPROV hProv;   				// CSP的句柄
	HCRYPTHASH hHash;   				// 散列的句柄
	HCRYPTKEY hPublicKey;				// 公共密钥的句柄　　　
	const int BUFFER=4096;				// 缓冲区大小常数　
	unsigned char pBuffer[BUFFER];		// 存放读文件内容的缓冲区　　　
	unsigned char pSignature[128];		// 上一段得到的签名的缓冲区　　　
	unsigned long dSignatureLen = 128;		// 上一段得到的签名的长度　　　
	int dReadLen;
	int FileLen;

	if(!CryptAcquireContext(&hProv, L"IRMT", NULL, PROV_RSA_FULL, 0))	// 连接名为IRMT的CSP，接受它的句柄放入hProv
	{
		if (GetLastError() == (DWORD)NTE_BAD_KEYSET)   						//不存在的话就创建
		{
			if(!CryptAcquireContext(&hProv, L"IRMT", NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))
			{
				//ShowMessage("IRMT密钥容器创建失败！\n错误号:" + String(GetLastError()));
				return false;
			}
		}
		else
		{
			//ShowMessage("无法连接CSP！\n错误号:" + String(GetLastError()));
			return false;
		}
	}

	if(!CryptImportKey(hProv, pbKeyBlobIRM, sizeof(pbKeyBlobIRM), 0, 0, &hPublicKey))
	{
		//ShowMessage("无法导入公钥！\n错误号:" + String(GetLastError()));
		return false;
	}


	if(!CryptCreateHash(hProv,CALG_MD5,0,0,&hHash)) // 创建一个散列对象，得到它的句柄放入hHash
	{
		//ShowMessage("无法创建散列对象！\n错误号:" + String(GetLastError()));
		return false;
	}
	
	memcpy(pSignature, Sign, 128);

	if(!CryptHashData(hHash, AnsiString(Data).c_str(), Data.Length(),0))// 根据文件的内容计算散列值
	{

	}

	bool sta = false;

	if(!CryptVerifySignature(hHash, pSignature, dSignatureLen, hPublicKey,NULL,0))
	{
		if(GetLastError() == (DWORD)NTE_BAD_SIGNATURE)
		{
		}

		AboutBox->Registered = 0xAFD35597;
		AboutBox->UnRegistered = 0xAFD34597;
	}
	else
	{
		//ShowMessage("文件没被修改");

		AboutBox->Registered = 0xACFAD597;
		AboutBox->UnRegistered = 0x2347AEDB ^ AboutBox->Registered;
		sta = true;
	}

	if (!CryptDestroyKey(hPublicKey))
	{
		//ShowMessage("释放密钥失败！\n错误号:" + String(GetLastError()));
	}

	if (!CryptReleaseContext(hProv,0))
	{
		//ShowMessage("释放CSP失败！\n错误号:" + String(GetLastError()));
	}

	return sta;
}


bool __fastcall TAboutBox::CheckLic(void)
{
	String Msg = "";
	uint8_t Sign[128];
	String Data = "IRMTouch HID Tools ";
	const char LicChar[16] = "!XT^CY%QLS5&3KW#";

	CID = GenCID();
	Lic = GetLic();

	if(Lic.Length() != 0)
	{
		Data += CID;

		for(int i=0; i<Lic.Length()/2 && i<128; i+=1)
		{
			for(uint8_t j=0; j<16; j+=1)
			{
				if(LicChar[j] == (Lic.c_str())[i*2])
				{
					Sign[i] = j;
					break;
				}
			}

			for(uint8_t j=0; j<16; j+=1)
			{
				if(LicChar[j] == (Lic.c_str())[i*2+1])
				{
					Sign[i] |= j<<4;
					break;
				}
			}
		}

		Msg.sprintf(L"%d位数字签名:\n", sizeof(Sign)*8);
		for(int i=0; i<sizeof(Sign); i+=1)
		{
			Msg.cat_sprintf(L"%.2X ", Sign[i]);
			if((i&0x0F) == 0x0F)
			{
				Msg += "\n";
			}
		}

		//ShowMessage(Msg);
		
		return CheckSign(Data, Sign);
	}

	return false;
}

void __fastcall TAboutBox::FormShow(TObject *Sender)
{
	String Cap = "";
	
	Edit1->Text = "";

	if(CheckLic())
	{
		Button1->Visible = false;
		Edit1->Visible = false;

		this->Height = 130;
		Cap = "About";
	}
	else
	{
		this->Height = 160;
		Cap += "CID: " + CID;
	}

	this->Caption = Cap;
	
}
//---------------------------------------------------------------------------

void __fastcall TAboutBox::Button1Click(TObject *Sender)
{
	//write registry
	WriteLic(Edit1->Text);

	ShowMessage("注册码将在下次重启软件后生效!");

	this->Close();
}
//---------------------------------------------------------------------------

