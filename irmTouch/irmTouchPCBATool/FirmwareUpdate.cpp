// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FirmwareUpdate.h"
#include "hex.h"
#include "rom.h"
#include "main.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormUpdate *FormUpdate;

#include "USBTalk.h"

// ---------------------------------------------------------------------------
__fastcall TFormUpdate::TFormUpdate(TComponent* Owner) : TForm(Owner) {
	memset(&FlashParam, 0, sizeof(FlashParam));
	memset(&BootParam, 0xFF, sizeof(BootParam));
}

const unsigned char TableA[] = {
	0x58, 0x74, 0xC3, 0x99, 0x7A, 0xDD, 0xE9, 0x65, 0x01, 0xE2, 0x5F, 0xF5,
	0xD9, 0x82, 0x44, 0x9F, 0x98, 0x1D, 0xB7, 0xE5, 0x7F, 0x9A, 0xF7, 0x40,
	0xB4, 0xD8, 0xC1, 0x2F, 0xBE, 0xE7, 0x80, 0x41, 0x28, 0x2D, 0xDE, 0xFC,
	0x12, 0x69, 0x9C, 0x00, 0x06, 0xDA, 0x33, 0xC5, 0xC8, 0x54, 0x04, 0xD0,
	0x5D, 0x70, 0x90, 0x6C, 0x36, 0xC6, 0x56, 0x0E, 0x2C, 0xA7, 0x9B, 0xCF,
	0xE3, 0xCA, 0x87, 0x55, 0x20, 0xD3, 0x22, 0x07, 0x09, 0x91, 0x72, 0xAB,
	0x0F, 0xE0, 0x0D, 0xA5, 0x79, 0x7D, 0x83, 0x0B, 0x4E, 0x53, 0x6D, 0x8A,
	0x85, 0x7B, 0xED, 0x37, 0x96, 0xEB, 0x11, 0x5E, 0x61, 0xFE, 0xBF, 0xDB,
	0x23, 0x2E, 0xFA, 0x34, 0x10, 0x7E, 0x02, 0xBC, 0xCD, 0x76, 0x08, 0xB2,
	0xB9, 0xE4, 0x15, 0x6E, 0x5B, 0xB6, 0xC7, 0x05, 0x6F, 0x8D, 0xD4, 0x97,
	0xFB, 0x93, 0x89, 0x17, 0x43, 0x64, 0x3A, 0x4B, 0x48, 0x84, 0xF6, 0x94,
	0xD5, 0x2B, 0x0C, 0x3E, 0xEC, 0xCE, 0x6B, 0xB5, 0xB3, 0x8B, 0x77, 0xB0,
	0x8E, 0xF4, 0xFD, 0xBA, 0xA4, 0xA6, 0xD1, 0x0A, 0x4D, 0xF3, 0x13, 0xB1,
	0xFF, 0xA9, 0x8F, 0x1B, 0x63, 0x27, 0x46, 0x14, 0x03, 0x66, 0xF1, 0x57,
	0x25, 0x1F, 0x47, 0xF8, 0xDF, 0xD7, 0x75, 0x38, 0xC0, 0x68, 0x2A, 0x62,
	0x73, 0xA8, 0x92, 0x8C, 0x19, 0xD6, 0xAD, 0xA2, 0xD2, 0xCC, 0x49, 0xA0,
	0x39, 0x42, 0x52, 0x5A, 0xF9, 0x9D, 0x71, 0x86, 0x51, 0xB8, 0x3F, 0x45,
	0x95, 0xAE, 0x1C, 0xE6, 0xC9, 0x60, 0x3D, 0x32, 0x81, 0xAF, 0xF0, 0xE8,
	0xBD, 0x1A, 0xC4, 0xAC, 0x24, 0xDC, 0xEF, 0x18, 0x3C, 0x4F, 0xCB, 0xA3,
	0x4C, 0x88, 0x7C, 0x26, 0x3B, 0xBB, 0x21, 0x59, 0x4A, 0x31, 0x67, 0xA1,
	0x35, 0x16, 0x78, 0x30, 0xAA, 0xE1, 0x5C, 0x6A, 0xEE, 0xC2, 0xEA, 0x29,
	0x50, 0x1E, 0x9E, 0xF2, };

const unsigned char TableB[] = {
	0x27, 0x08, 0x66, 0xA4, 0x2E, 0x73, 0x28, 0x43, 0x6A, 0x44, 0x97, 0x4F,
	0x86, 0x4A, 0x37, 0x48, 0x64, 0x5A, 0x24, 0x9A, 0xA3, 0x6E, 0xF1, 0x7B,
	0xDF, 0xB8, 0xD9, 0x9F, 0xCE, 0x11, 0xFD, 0xA9, 0x40, 0xEA, 0x42, 0x60,
	0xDC, 0xA8, 0xE7, 0xA1, 0x20, 0xFB, 0xB2, 0x85, 0x38, 0x21, 0x61, 0x1B,
	0xF3, 0xED, 0xD3, 0x2A, 0x63, 0xF0, 0x34, 0x57, 0xAF, 0xC0, 0x7E, 0xE8,
	0xE0, 0xD2, 0x87, 0xCA, 0x17, 0x1F, 0xC1, 0x7C, 0x0E, 0xCB, 0xA2, 0xAA,
	0x80, 0xBE, 0xEC, 0x7F, 0xE4, 0x98, 0x50, 0xE1, 0xFC, 0xC8, 0xC2, 0x51,
	0x2D, 0x3F, 0x36, 0xA7, 0x00, 0xEB, 0xC3, 0x70, 0xF6, 0x30, 0x5B, 0x0A,
	0xD1, 0x5C, 0xB3, 0xA0, 0x7D, 0x07, 0xA5, 0xEE, 0xB1, 0x25, 0xF7, 0x8A,
	0x33, 0x52, 0x6F, 0x74, 0x31, 0xC6, 0x46, 0xB4, 0x01, 0xAE, 0x69, 0x8E,
	0xF2, 0x4C, 0x04, 0x55, 0xE6, 0x4D, 0x65, 0x14, 0x1E, 0xD4, 0x0D, 0x4E,
	0x81, 0x54, 0xC7, 0x3E, 0xE5, 0x7A, 0x53, 0x8D, 0xB7, 0x75, 0x90, 0x9E,
	0x32, 0x45, 0xB6, 0x79, 0x83, 0xCC, 0x58, 0x77, 0x10, 0x03, 0x15, 0x3A,
	0x26, 0xC5, 0xFE, 0x0F, 0xBF, 0xEF, 0xBB, 0xE3, 0x94, 0x4B, 0x95, 0x39,
	0xB5, 0x9D, 0xF4, 0x47, 0xDB, 0xBA, 0xCD, 0xD5, 0x8F, 0x9B, 0x6B, 0x8C,
	0x18, 0x8B, 0x71, 0x12, 0xC9, 0x6C, 0x93, 0xE9, 0x67, 0xD8, 0x1C, 0x5E,
	0xB0, 0x1A, 0xF9, 0x02, 0xDA, 0x2B, 0x35, 0x72, 0x2C, 0xD0, 0x3D, 0xE2,
	0xBD, 0x68, 0x89, 0x3B, 0x2F, 0x96, 0xBC, 0x41, 0x76, 0x84, 0xB9, 0xAD,
	0x19, 0x0C, 0x29, 0x5F, 0xDD, 0x05, 0x22, 0xAC, 0x49, 0xF5, 0x09, 0x3C,
	0x6D, 0x13, 0xCF, 0x1D, 0xD7, 0x06, 0xFA, 0x59, 0x88, 0x56, 0xF8, 0xDE,
	0xD6, 0xA6, 0xFF, 0x99, 0x91, 0x0B, 0x82, 0x16, 0xAB, 0xC4, 0x62, 0x78,
	0x23, 0x92, 0x5D, 0x9C, };

// const unsigned char pbKeyBlob[148] = {
// 0x06, 0x02, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x52, 0x53, 0x41, 0x31, 0x00, 0x04, 0x00, 0x00,
// 0x01, 0x00, 0x01, 0x00, 0x1D, 0x69, 0x05, 0x94, 0x03, 0x77, 0x3E, 0xE6, 0x2F, 0x3C, 0x6B, 0xDF,
// 0xDB, 0x1F, 0x25, 0x1E, 0xCD, 0x91, 0x2D, 0xAC, 0x19, 0xA9, 0x40, 0x82, 0x5C, 0x64, 0x4A, 0x56,
// 0x24, 0x7D, 0xDB, 0xFF, 0x9B, 0xB1, 0x52, 0xB2, 0x1C, 0x57, 0xFD, 0xD0, 0x83, 0xAC, 0x44, 0x6E,
// 0x4B, 0x62, 0xEB, 0x35, 0x0F, 0xF3, 0xAF, 0x93, 0xC9, 0x8B, 0x80, 0x21, 0x82, 0xAA, 0xB8, 0xC6,
// 0x6F, 0x11, 0x31, 0x3E, 0x9E, 0x53, 0x87, 0x09, 0x3E, 0xEE, 0x3B, 0xA4, 0x70, 0x85, 0x1E, 0x56,
// 0xAB, 0xB5, 0x49, 0x60, 0x23, 0xB6, 0x24, 0x23, 0xF8, 0xDE, 0x16, 0xEF, 0x16, 0xF4, 0xB5, 0x0A,
// 0x20, 0xE7, 0x0F, 0xE9, 0xA1, 0xE4, 0x59, 0x77, 0x76, 0xDF, 0x86, 0x8D, 0xE0, 0xE4, 0xC6, 0x9B,
// 0x08, 0x5B, 0x5E, 0x33, 0x76, 0xE0, 0x3E, 0x21, 0xD9, 0x89, 0xD7, 0x67, 0x45, 0xB6, 0xD8, 0x8D,
// 0xB2, 0x58, 0xC8, 0xA1,
// };

const unsigned char pbKeyBlob[148] = {
	0x06, 0x02, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x52, 0x53, 0x41, 0x31,
	0x00, 0x04, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x4D, 0xD7, 0x12, 0x6B,
	0x4E, 0x98, 0x4F, 0x77, 0xA8, 0xE5, 0x53, 0xB1, 0xFC, 0xD5, 0x34, 0x05,
	0x26, 0x87, 0xDC, 0x5E, 0xF2, 0xFF, 0x66, 0xDB, 0xE2, 0x4C, 0xB2, 0xEA,
	0x76, 0xB1, 0x72, 0x32, 0xF8, 0x56, 0xF8, 0xF2, 0xAA, 0x03, 0x98, 0x83,
	0x83, 0x52, 0x4C, 0x85, 0x7B, 0xA9, 0xFE, 0xBA, 0x0D, 0x7F, 0x68, 0x67,
	0x55, 0x99, 0xF1, 0x82, 0xD6, 0x9B, 0x4E, 0x40, 0x63, 0x1F, 0xBC, 0xF1,
	0xA8, 0xFD, 0xE9, 0xC1, 0xE1, 0xCB, 0x94, 0xAB, 0x8A, 0x6C, 0x2B, 0xA9,
	0xE8, 0x09, 0x31, 0xF7, 0xE2, 0x9D, 0x53, 0x03, 0x19, 0x1B, 0xEE, 0x36,
	0xF2, 0x50, 0x70, 0x3C, 0x16, 0x41, 0x78, 0x5C, 0xA1, 0xC5, 0xCF, 0xB2,
	0x71, 0x77, 0xC4, 0x0E, 0xB6, 0x78, 0xA7, 0x28, 0xA7, 0xD1, 0x96, 0xCF,
	0x87, 0xF5, 0x2C, 0xBE, 0x32, 0x4D, 0xFD, 0x90, 0x6A, 0x48, 0x09, 0x2A,
	0x1A, 0x8E, 0x50, 0x85, };

uint32_t __fastcall TFormUpdate::CRC32(uint32_t *Data, uint32_t Len) {
	uint8_t i;

	uint32_t crc = 0xFFFFFFFF;

	for (uint32_t i = 0; i < Len; i++) {
		uint32_t temp = Data[i];

		for (uint32_t j = 0; j < 32; j++) {
			if ((crc ^ temp) & 0x80000000) {
				crc = 0x04C11DB7 ^ (crc << 1);
			}
			else {
				crc <<= 1;
			}

			temp <<= 1;
		}
	}

	return crc;
}

String __fastcall DateConvert(char *Str) {
	String Month = "JanFebMarAprMayJunJulAugSepOctNovDec";
	String Date = String(Str).SubString(8, 4) + "-";
	int i;

	for (i = 0; i < 12; i += 1) {
		if (Month.SubString(i * 3 + 1, 3) == String(Str).SubString(1, 3)) {
			break;
		}
	}

	Date += String(i + 1) + "-";

	Date += String(Str).SubString(5, 2);

	return Date;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormUpdate::InterpretFile(String FileName) {
	String Msg;

	if (hexfile.GetFileData(FileName) <= 0) {
		// ShowMessage("升级文件" + FileName + "无效");
		return false;
	}
	else {
		uint32_t crc = 0;
		String Msg;
		char RomCtrl[65];
		int32_t i;
		uint16_t iTemp;

		if (BootParam.BootLoaderVersion == 16 ||
			BootParam.BootLoaderVersion == 18) {
			memcpy(RomCtrl, &UpdateCtrl.MainFlash[0xE800 - 64], 64);
			RomCtrl[64] = '\0';
		}
		else {
			memcpy(RomCtrl, &UpdateCtrl.MainFlash[UpdateCtrl.AppEndAdd -
				64], 64);
			RomCtrl[64] = '\0';
		}

		if (((uint32_t*)&RomCtrl[60])[0] == 0xAAAAAAAA) { // APU firmware
			IsAPUFirmware = true;
			GroupBox1->Caption = L"升级文件 (协处理器APU)";
		}
		else if (((uint32_t*)&RomCtrl[60])[0] == 0xBBBBBBBB) { // MPU firmware
			IsAPUFirmware = false;
			GroupBox1->Caption = L"升级文件 (主协处理器MPU)";
		}
		else {
			IsAPUFirmware = false;
			GroupBox1->Caption = L"升级文件 (MPU+APU)";
		}

		crc = CRC32((uint32_t*)(UpdateCtrl.MainFlash + UpdateCtrl.AppStartAdd),
			(UpdateCtrl.AppSize) / 4 - 1);

		UpdateCtrl.MainFlash[UpdateCtrl.AppEndAdd - 4] = ((uint8_t*)&crc)[0];
		UpdateCtrl.MainFlash[UpdateCtrl.AppEndAdd - 3] = ((uint8_t*)&crc)[1];
		UpdateCtrl.MainFlash[UpdateCtrl.AppEndAdd - 2] = ((uint8_t*)&crc)[2];
		UpdateCtrl.MainFlash[UpdateCtrl.AppEndAdd - 1] = ((uint8_t*)&crc)[3];

		Msg.sprintf(L"文件大小：%.3fKB", (float)UpdateCtrl.FileSize / 1000.0);
		Label237->Caption = Msg;

		Msg = "编译日期：" + DateConvert(RomCtrl);
		Label258->Caption = Msg;

		Msg.sprintf(L"编译时间：%s", UnicodeString(&RomCtrl[11]));
		Label259->Caption = Msg;

		((uint8_t*)&iTemp)[0] = RomCtrl[52];
		((uint8_t*)&iTemp)[1] = RomCtrl[53];
		// com.FileHWVersion = iTemp;

		Msg.sprintf(L"硬件版本：%d.%d", iTemp / 100, iTemp % 100);
		FileVersion.HardwareVer = iTemp;
		Label276->Caption = Msg;

		((uint8_t*)&iTemp)[0] = RomCtrl[54];
		((uint8_t*)&iTemp)[1] = RomCtrl[55];
		Msg.sprintf(L"固件版本：%d.%d.%d", iTemp / 1000, iTemp / 10 % 100,
			iTemp % 10);
		FileVersion.SoftwareVer = iTemp;
		Label238->Caption = Msg;

		Msg.sprintf(L"起始地址：0x%.8X", UpdateCtrl.ROMStartAdd);
		Label239->Caption = Msg;

		Msg.sprintf(L"终止地址：0x%.8X", UpdateCtrl.ROMEndAdd);
		Label240->Caption = Msg;

		Msg.sprintf(L"固件大小：%.3fKB",
			(float)(UpdateCtrl.ROMEndAdd - UpdateCtrl.ROMStartAdd) / 1000.0);
		Label257->Caption = Msg;

		// Msg.sprintf("CRC32：0x%.8X", crc);
		// Label275->Caption = Msg;

		Msg.sprintf(L"Flash大小：%dKB", UpdateCtrl.FlashSize);
		if (UpdateCtrl.FlashSize == 64)
			Msg.cat_printf(L" (8)");
		else if (UpdateCtrl.FlashSize == 128)
			Msg.cat_printf(L" (B)");
		else if (UpdateCtrl.FlashSize == 256)
			Msg.cat_printf(L" (C)");
		else if (UpdateCtrl.FlashSize == 512)
			Msg.cat_printf(L" (E)");
		else if (UpdateCtrl.FlashSize == 1024)
			Msg.cat_printf(L" (G)");
		else
			Msg.cat_printf(L" (N/A)");

		FileVersion.FlashSize = UpdateCtrl.FlashSize / 32;
		Label275->Caption = Msg;

		// memcpy(Buf, &UpdateCtrl.MainFlash[APP_END_ADD-64], 19);
		StatusBar1->SimpleText = L"文件打开成功！";
	}

	return true;
}

void __fastcall EncodeBuf(unsigned char *Out, unsigned char *In,
	unsigned int Len) {
	unsigned int i;

	for (i = 0; i < Len; i += 1) {
		*(Out + i) = TableA[*(In + i)];
	}
}

void __fastcall DecodeBuf(unsigned char *Out, unsigned char *In,
	unsigned int Len) {
	unsigned int i;

	for (i = 0; i < Len; i += 1) {
		*(Out + i) = TableB[*(In + i)];
	}
}

// ---------------------------------------------------------------------------
bool __fastcall CheckFileSign(String FileName) {
	HCRYPTPROV hProv; // CSP的句柄
	HCRYPTHASH hHash; // 散列的句柄
	HCRYPTKEY hPublicKey; // 公共密钥的句柄
	const int BUFFER = 4096; // 缓冲区大小常数
	unsigned char pBuffer[BUFFER]; // 存放读文件内容的缓冲区
	FILE *sourceFile; // 一个文件流
	unsigned char pSignature[128]; // 上一段得到的签名的缓冲区
	unsigned long dSignatureLen = 128; // 上一段得到的签名的长度
	int dReadLen;
	int FileLen;

	if (!CryptAcquireContext(&hProv, L"JYEST", NULL, PROV_RSA_FULL, 0))
		// 连接名为JYEST的CSP，接受它的句柄放入hProv
	{
		if (GetLastError() == (DWORD)NTE_BAD_KEYSET) // 不存在的话就创建
		{
			if (!CryptAcquireContext(&hProv, L"JYEST", NULL, PROV_RSA_FULL,
				CRYPT_NEWKEYSET)) {
				// ShowMessage("JYEST密钥容器创建失败！\n错误号:" + String(GetLastError()));
				return false;
			}
		}
		else {
			// ShowMessage("无法连接CSP！\n错误号:" + String(GetLastError()));
			return false;
		}
	}

	if (!CryptImportKey(hProv, pbKeyBlob, sizeof(pbKeyBlob), 0, 0, &hPublicKey))
	{
		// ShowMessage("无法导入公钥！\n错误号:" + String(GetLastError()));
		return false;
	}

	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
		// 创建一个散列对象，得到它的句柄放入hHash
	{
		// ShowMessage("无法创建散列对象！\n错误号:" + String(GetLastError()));
		return false;
	}

	if ((sourceFile = fopen(AnsiString(FileName).c_str(), "rb")) == NULL) {
		// ShowMessage("无法打开输入文件:\n" + FileName);
		return false;
	}

	fseek(sourceFile, -128, SEEK_END);
	FileLen = ftell(sourceFile);
	fread(pSignature, 1, 128, sourceFile);
	rewind(sourceFile);

	DecodeBuf(pSignature, pSignature, 128);

	do {
		if (FileLen < BUFFER) {
			dReadLen = fread(pBuffer, 1, FileLen, sourceFile);
			// sourceFile->Read(pBuffer,BUFFER);
		}
		else {
			dReadLen = fread(pBuffer, 1, BUFFER, sourceFile);
			// sourceFile->Read(pBuffer,BUFFER);
		}

		FileLen -= dReadLen;
		if (dReadLen <= 0) {
			break;
		}

		if (!CryptHashData(hHash, pBuffer, dReadLen, 0)) // 根据文件的内容计算散列值
		{
			// 错误处理
		}
	}
	while (!(dReadLen < BUFFER));

	fclose(sourceFile);

	bool sta = false;

	if (!CryptVerifySignature(hHash, pSignature, dSignatureLen, hPublicKey,
		NULL, 0)) {
		String Msg;

		Msg.sprintf(L"RSA 校验错误！\n错误码:0x%x\n", GetLastError());

		switch (GetLastError()) {
		case ERROR_INVALID_HANDLE:
			Msg.cat_printf(L"ERROR_INVALID_HANDLE");
			break;

		case ERROR_INVALID_PARAMETER:
			Msg.cat_printf(L"ERROR_INVALID_PARAMETER");
			break;

		case NTE_BAD_FLAGS:
			Msg.cat_printf(L"NTE_BAD_FLAGS");
			break;

		case NTE_BAD_HASH:
			Msg.cat_printf(L"NTE_BAD_HASH");
			break;

		case NTE_BAD_KEY:
			Msg.cat_printf(L"NTE_BAD_KEY");
			break;

		case NTE_BAD_SIGNATURE:
			Msg.cat_printf(L"NTE_BAD_SIGNATURE");
			break;

		case NTE_BAD_UID:
			Msg.cat_printf(L"NTE_BAD_UID");
			break;

		case NTE_NO_MEMORY:
			Msg.cat_printf(L"NTE_NO_MEMORY");
			break;

		default:
			Msg.cat_printf(L"Unknown!");
			break;
		}

		ShowMessage(Msg);

		if (GetLastError() == (DWORD)NTE_BAD_SIGNATURE) {

		}
	}
	else {
		// ShowMessage("文件没被修改");
		sta = true;
	}

	if (!CryptDestroyKey(hPublicKey)) {
		// ShowMessage("释放密钥失败！\n错误号:" + String(GetLastError()));
	}

	if (!CryptReleaseContext(hProv, 0)) {
		// ShowMessage("释放CSP失败！\n错误号:" + String(GetLastError()));
	}

	return sta;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormUpdate::CheckFile(String FileName) {
	// check rsa
	if (ExtractFileExt(FileName) == ".bin") {
		if (!CheckFileSign(FileName)) {
			// ShowMessage("文件已经被破坏！");
			return false;
		}
	}

	//
	if (InterpretFile(FileName) == false) {
		return false;
	}

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormUpdate::Button1Click(TObject *Sender) {
	String FileName;

	OpenDialog->Filter = "升级文件(*.bin)|*.bin|Intel-HEX File(*.hex)|*.hex";

#ifdef _DEBUG
	OpenDialog->FilterIndex = 2;
#else
	OpenDialog->FilterIndex = 1;
#endif

	if (OpenDialog->Execute()) {
		FileName = OpenDialog->FileName;

		EditFirmware->Text = FileName;

		if (!CheckFile(FileName)) {
			ShowMessage(L"升级文件无效:\n" + FileName);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormUpdate::Button3Click(TObject *Sender) {
	// #ifdef _DEBUG
	// #if FACTORY == 0
	// FormROM->Show();
	// #endif
	// #endif
}

void __fastcall TFormUpdate::MakeKey(uint8_t *Key) {
	const unsigned char des_key_[] = {
		0x55, 0xd4, 0xac, 0xdb, 0x17, 0x37, 0x14, 0x36, 0xae, 0x78, 0x6c, 0x0d,
		0x38, 0x93, 0x27, 0xb6};

	uint32_t CpuId[3];

	memcpy(&CpuId, FormConfig->CPUID, sizeof(CpuId));

	if (CpuId[0] != (uint32_t)0xFFFFFFFF && CpuId[0] != 0) // invalid cpu id
	{
		for (int i = 0; i < 16; i += 1) {
			Key[i] = CpuId[0] & 0x3F; // 6bit

			if (Key[i] < 0x10) {
				Key[i] |= 0x40;
			}

			CpuId[0] = CpuId[1] << (32 - 6) | CpuId[0] >> 6;
			CpuId[1] = CpuId[2] << (32 - 6) | CpuId[1] >> 6;
			CpuId[2] >>= 6;
		}
	}
	else {
		memcpy(Key, des_key_, 16);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormUpdate::ButtonUpdateClick(TObject *Sender) {
	String Msg = "";

	if (!CheckFile(this->EditFirmware->Text)) {
		ShowMessage(L"升级文件无效:\n"+this->EditFirmware->Text);

		return;
	}

	if (MainForm->HIDSendThread->Connected == TRUE) {

		if (BootParam.AppSta != APP_EMPTY) {
			Msg = "";

			if ((BootParam.Flags & BOOT_FLAG_WO) != 0) {
				ShowMessage(L"写保护处于打开状态，无法升级固件！\n请先关闭写保护！");

				return;
			}

			if (FileVersion.FlashSize != BootParam.FlashSize) {
				Msg.cat_printf(L"文件大小与触摸屏Flash大小不匹配!\n");
			}

			if (FileVersion.HardwareVer != BootParam.HardwareVer) {
				Msg.cat_printf(L"文件所需硬件版本与触摸屏硬件版本不匹配!\n");
			}

			if (FileVersion.SoftwareVer < BootParam.SoftwareVer) {
				Msg.cat_printf(L"文件的版本比触摸屏旧!\n");
			}

			if (Msg.Length() != 0) {
				Msg.cat_printf(L"\n忽略以上警告,强制升级?");
				if (MessageBox(NULL, Msg.t_str(), L"强制升级?",
					MB_OKCANCEL | MB_ICONWARNING) == IDCANCEL)
					return;
			}
		}

		Head.MemType = TYPE_APP;
		Head.Mode = MODE_PROGRAM;
		Head.StartAdd = UpdateCtrl.AppStartAdd;

		if (CheckBoxEncode->Checked) {
			Head.Mode |= MODE_ENCODE;

			MakeKey(MainForm->HIDSendThread->Key);
		}

		if (MainForm->HIDSendThread->SubmitTask(TYPE_APP, UpdateCtrl.AppSize,
			UpdateCtrl.MainFlash + UpdateCtrl.AppStartAdd, &Head)) {
			Button1->Enabled = false;
			ButtonGetSta->Enabled = false;
			ButtonUpdate->Enabled = false;
			ButtonSwMode->Enabled = false;

			TimerUpdate->Enabled = true;
		}
	}
	else {
		ShowMessage(L"设备不存在！");
	}
}

void __fastcall TFormUpdate::UpdateFlashParam(uint8_t BootVersion,
	uint8_t FlashSize) {
	FLASH_PARAM *fp = &FlashParam;

	memset(fp, 0, sizeof(FLASH_PARAM));

	if (BootVersion == 16 || BootVersion == 18) {
		fp->FlashSize = 62 * 1024;
		fp->BootSize = 16 * 1024;

		fp->UserParamStartAdd = fp->BootSize;
		fp->UserParamSize = 2 * 1024;

		fp->CalibrateStartAdd = fp->UserParamStartAdd + fp->UserParamSize;
		fp->CalibrateSize = 2 * 1024;

		fp->AppStartAdd = fp->CalibrateStartAdd + fp->CalibrateSize;
		fp->AppSize = fp->FlashSize - fp->BootSize - 8 * 1024;

		fp->ParamStartAdd = fp->FlashSize - 1024;
		fp->ParamSize = 1024;

		fp->ScanTabStartAdd = fp->ParamStartAdd - 1024;
		fp->ScanTabSize = 1024;

		fp->SecretStartAdd = fp->ScanTabStartAdd - 1024;
		fp->SecretSize = 1024;

		fp->SerialStartAdd = fp->SecretStartAdd - 1024;
		fp->SerialSize = 1024;

		fp->EndAreaSize = 6 * 1024;
	}
	else if (BootVersion < 25 || BootVersion == 26) // stm32f103 16K scan table
	{
		fp->FlashSize = FlashSize * 32 * 1024;
		fp->BootSize = 16 * 1024;

		fp->AppStartAdd = fp->BootSize;
		fp->AppSize = fp->FlashSize - fp->BootSize - 22 * 1024;

		fp->ParamStartAdd = fp->FlashSize - 2 * 1024;
		fp->ParamSize = 2 * 1024;

		fp->ScanTabStartAdd = fp->ParamStartAdd - 16 * 1024;
		fp->ScanTabSize = 16 * 1024;

		fp->SecretStartAdd = fp->ScanTabStartAdd - 2 * 1024;
		fp->SecretSize = 2 * 1024;

		fp->SerialStartAdd = fp->SecretStartAdd - 2 * 1024;
		fp->SerialSize = 2 * 1024;

		fp->EndAreaSize = 22 * 1024;
	}
	else if (BootVersion < 50) // stm32f103 32k scan table
	{
		fp->FlashSize = FlashSize * 32 * 1024;
		fp->BootSize = 16 * 1024;

		fp->UserParamStartAdd = fp->BootSize;
		fp->UserParamSize = 2 * 1024;

		fp->CalibrateStartAdd = fp->UserParamStartAdd + fp->UserParamSize;
		fp->CalibrateSize = 2 * 1024;

		fp->AppStartAdd = fp->CalibrateStartAdd + fp->CalibrateSize;
		fp->AppSize = fp->FlashSize - fp->BootSize - 4 * 1024 - 38 * 1024;

		fp->ParamStartAdd = fp->FlashSize - 2 * 1024;
		fp->ParamSize = 2 * 1024;

		fp->ScanTabStartAdd = fp->ParamStartAdd - 32 * 1024;
		fp->ScanTabSize = 32 * 1024;

		fp->SecretStartAdd = fp->ScanTabStartAdd - 2 * 1024;
		fp->SecretSize = 2 * 1024;

		fp->SerialStartAdd = fp->SecretStartAdd - 2 * 1024;
		fp->SerialSize = 2 * 1024;

		fp->EndAreaSize = 38 * 1024;
	}
	else if (BootVersion < 60) // stm32f20x
	{

	}
	else if (BootVersion < 71) // stm32f30x
	{
		fp->FlashSize = FlashSize * 32 * 1024;
		fp->BootSize = 16 * 1024;

		fp->UserParamStartAdd = fp->BootSize;
		fp->UserParamSize = 2 * 1024;

		fp->CalibrateStartAdd = fp->UserParamStartAdd + fp->UserParamSize;
		fp->CalibrateSize = 2 * 1024;

		fp->AppStartAdd = fp->CalibrateStartAdd + fp->CalibrateSize;

		fp->ParamSize = 2 * 1024;
		fp->ParamStartAdd = fp->FlashSize - fp->ParamSize;

		if (BootVersion >= 69) { // D serial
			fp->ScanTabSize = 96 * 1024;
		}
		else {
			fp->ScanTabSize = 32 * 1024;
		}
		fp->ScanTabStartAdd = fp->ParamStartAdd - fp->ScanTabSize;

		fp->SecretSize = 2 * 1024;
		fp->SecretStartAdd = fp->ScanTabStartAdd - fp->SecretSize;

		fp->SerialSize = 2 * 1024;
		fp->SerialStartAdd = fp->SecretStartAdd - fp->SerialSize;

		fp->EndAreaSize = fp->ScanTabSize + fp->ParamSize + fp->SecretSize +
			fp->SerialSize;

		fp->AppSize = fp->FlashSize - fp->BootSize - fp->UserParamSize -
			fp->CalibrateSize - fp->EndAreaSize;
	}
	else if (BootVersion < 80) // stm32f30x 1对多     7.1~7.9
	{
		fp->FlashSize = FlashSize * 32 * 1024;
		fp->BootSize = 16 * 1024;

		fp->UserParamStartAdd = fp->BootSize;
		fp->UserParamSize = 2 * 1024;

		fp->CalibrateStartAdd = fp->UserParamStartAdd + fp->UserParamSize;
		fp->CalibrateSize = 2 * 1024;

		fp->AppStartAdd = fp->CalibrateStartAdd + fp->CalibrateSize;
		fp->AppSize = fp->FlashSize - fp->BootSize - 4 * 1024 - 134 * 1024;

		fp->ParamStartAdd = fp->FlashSize - 2 * 1024;
		fp->ParamSize = 2 * 1024;

		fp->ScanTabStartAdd = fp->ParamStartAdd - 128 * 1024;
		fp->ScanTabSize = 128 * 1024;

		fp->SecretStartAdd = fp->ScanTabStartAdd - 2 * 1024;
		fp->SecretSize = 2 * 1024;

		fp->SerialStartAdd = fp->SecretStartAdd - 2 * 1024;
		fp->SerialSize = 2 * 1024;

		fp->EndAreaSize = 134 * 1024;
	}
	else if (BootVersion < 90) // at32f403    8.0~8.9
	{
		fp->FlashSize = FlashSize * 32 * 1024;
		fp->BootSize = 16 * 1024;

		fp->ParamStartAdd = fp->BootSize;
		fp->ParamSize = 2 * 1024;

		fp->SerialStartAdd = fp->ParamStartAdd + fp->ParamSize;
		fp->SerialSize = 2 * 1024;

		fp->UserParamStartAdd = fp->SerialStartAdd + fp->SerialSize;
		fp->UserParamSize = 2 * 1024;

		fp->CalibrateStartAdd = fp->UserParamStartAdd + fp->UserParamSize;
		fp->CalibrateSize = 2 * 1024;

		fp->SecretStartAdd = fp->CalibrateStartAdd + fp->CalibrateSize;
		fp->SecretSize = 2 * 1024;

		fp->ReserveStartAdd = fp->SecretStartAdd + fp->SecretSize;
		// not exactly
		fp->ReserveSize = 38 * 1024;

		fp->ScanTabStartAdd = fp->ReserveStartAdd + fp->ReserveSize;
		fp->ScanTabSize = 64 * 1024;

		fp->AppStartAdd = fp->ScanTabStartAdd + fp->ScanTabSize;
		fp->AppSize = 128 * 1024;

		fp->EndAreaSize = fp->FlashSize - 256 * 1024;

	}
	else if (BootVersion == 100)
		// stm32f4xx   V10.0        仅出过一个框     TMD还出问题了  2013.3.12
	{
		fp->FlashSize = FlashSize * 32 * 1024;
		fp->BootSize = 16 * 1024;

		fp->ParamStartAdd = fp->BootSize;
		fp->ParamSize = 2 * 1024;

		fp->SerialStartAdd = fp->ParamStartAdd + fp->ParamSize;
		fp->SerialSize = 2 * 1024;

		fp->ReserveStartAdd = fp->SerialStartAdd + fp->SerialSize;
		fp->ReserveSize = 12 * 1024;

		fp->SecretStartAdd = fp->ReserveStartAdd + fp->ReserveSize;
		fp->SecretSize = 16 * 1024;

		fp->ScanTabStartAdd = fp->SecretStartAdd + fp->SecretSize;
		fp->ScanTabSize = 16 * 1024;

		fp->AppStartAdd = fp->ScanTabStartAdd + fp->ScanTabSize;
		fp->AppSize = 64 * 1024;

		fp->EndAreaSize = fp->FlashSize - 128 * 1024;
	}
	else if (BootVersion == 101) {
		fp->FlashSize = FlashSize * 32 * 1024;
		fp->BootSize = 16 * 1024;

		fp->ParamStartAdd = fp->BootSize;
		fp->ParamSize = 2 * 1024;

		fp->SerialStartAdd = fp->ParamStartAdd + fp->ParamSize;
		fp->SerialSize = 2 * 1024;

		fp->ReserveStartAdd = fp->SerialStartAdd + fp->SerialSize;
		fp->ReserveSize = 28 * 1024;

		fp->SecretStartAdd = fp->ReserveStartAdd + fp->ReserveSize;
		fp->SecretSize = 16 * 1024;

		fp->ScanTabStartAdd = fp->SecretStartAdd + fp->SecretSize;
		fp->ScanTabSize = 64 * 1024;

		fp->AppStartAdd = fp->ScanTabStartAdd + fp->ScanTabSize;
		fp->AppSize = 128 * 1024;

		fp->EndAreaSize = fp->FlashSize - 256 * 1024;
	}
	else // >= 10.2
	{
		fp->FlashSize = FlashSize * 32 * 1024;
		fp->BootSize = 16 * 1024;

		fp->ParamStartAdd = fp->BootSize;
		fp->ParamSize = 2 * 1024;

		fp->SerialStartAdd = fp->ParamStartAdd + fp->ParamSize;
		fp->SerialSize = 2 * 1024;

		fp->UserParamStartAdd = fp->BootSize + 16 * 1024;
		fp->UserParamSize = 2 * 1024;

		fp->CalibrateStartAdd = fp->UserParamStartAdd + fp->UserParamSize;
		fp->CalibrateSize = 2 * 1024;

		fp->ReserveStartAdd = fp->SerialStartAdd + fp->SerialSize;
		// not exactly
		fp->ReserveSize = 28 * 1024;

		fp->SecretStartAdd = fp->ReserveStartAdd + fp->ReserveSize;
		fp->SecretSize = 16 * 1024;

		fp->ScanTabStartAdd = fp->SecretStartAdd + fp->SecretSize;

		if (BootVersion == 124) {
			fp->ScanTabSize = 192 * 1024;
		}
		else {
			fp->ScanTabSize = 64 * 1024;
		}

		fp->AppStartAdd = fp->ScanTabStartAdd + fp->ScanTabSize;
		fp->AppSize = 128 * 1024;

		if (BootVersion == 124) {
			fp->EndAreaSize = fp->FlashSize - 256 * 1024 - 128 * 1024;
		}
		else {
			fp->EndAreaSize = fp->FlashSize - 256 * 1024;
		}
	}

	if (fp->FlashSize == 64 * 1024 && BootVersion != 16) {
		fp->AppStartAdd = fp->BootSize;
		fp->AppSize = fp->FlashSize - fp->BootSize - 10 * 1024;

		fp->ParamStartAdd = fp->FlashSize - 2 * 1024;
		fp->ParamSize = 2 * 1024;

		fp->ScanTabStartAdd = fp->ParamStartAdd - 4 * 1024;
		fp->ScanTabSize = 4 * 1024;

		fp->SecretStartAdd = fp->ScanTabStartAdd - 2 * 1024;
		fp->SecretSize = 2 * 1024;

		fp->SerialStartAdd = fp->SecretStartAdd - 2 * 1024;
		fp->SerialSize = 2 * 1024;

		fp->EndAreaSize = 10 * 1024;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormUpdate::DispSta(void) {
	String Msg;
	const String RstTypes[] = {"上电", "看门狗", "指令", "低电压"};

	Msg.sprintf(L"%d.%d", (BootParam.BootLoaderVersion & 0x7F) / 10,
		(BootParam.BootLoaderVersion & 0x7F) % 10);

	if ((BootParam.BootLoaderVersion & 0x7F) < 60) {
		if (BootParam.BootLoaderVersion & 0x80) {
			Msg += "-10";
		}
		else {
			Msg += "-6";
		}
	}
	else if ((BootParam.BootLoaderVersion & 0x7F) < 80) {
		Msg += "-F3";
	}
	else {
		Msg += "-F4";
	}

	LabelBoot->Caption = "Boot版本：" + Msg;

	if (BootParam.HardwareVer != 0xFFFF) {
		// if(BootParam.HardwareVer%100 < 10)
		// {
		Msg.sprintf(L"硬件版本：%d.%d", BootParam.HardwareVer / 100,
			BootParam.HardwareVer % 100);
		// }
		// else
		// {
		// Msg.sprintf("硬件版本：%d.%d", BootParam.HardwareVer/100, BootParam.HardwareVer%100);
		// }
	}
	else {
		Msg.sprintf(L"硬件版本：N/A");
	}
	LabelHWVer->Caption = Msg;

	if (BootParam.SoftwareVer != 0xFFFF) {
		Msg.sprintf(L"固件版本：%d", BootParam.SoftwareVer / 1000);
		Msg.cat_printf(L".%d", BootParam.SoftwareVer % 1000 / 10);
		Msg.cat_printf(L".%d", BootParam.SoftwareVer % 10);
	}
	else {
		Msg.sprintf(L"软件版本：N/A");
	}
	LabelFirmwareVer->Caption = Msg;

	Msg = "固件状态：";
	switch (BootParam.AppSta) {
	case APP_OK:
		Msg += "正常";
		break;

	case APP_EMPTY:
		Msg += "空白";
		break;

	case APP_CORRUPT:
		Msg += "损坏";
		break;

	default:
		Msg += "未知";
		break;
	}
	LabelFwSta->Caption = Msg;

	Msg = "复位原因：";
	if (BootParam.ResetType < 4) {
		Msg += RstTypes[BootParam.ResetType];
	}
	else {
		Msg += "未知";
	}
	LabelResetReson->Caption = Msg;

	if ((BootParam.RunSta & RUN_STA_MODE_MASK) == RUM_MODE_APP) {
		Msg = "正常运行";

		if (BootParam.RunSta & RUN_STA_PWR_ERR) {
			Msg = "供电不足";
		}
		else if (BootParam.RunSta & RUN_STA_PARAM_ERR) {
			Msg = "参数异常";
		}
		else if (BootParam.RunSta & RUN_STA_SCAN_ERR) {
			Msg = "扫描表异常";
		}
		else if (BootParam.RunSta & RUN_STA_MEM_ERR) {
			Msg = "内存不足";
		}
		else if (BootParam.RunSta & RUN_STA_SECRET_ERR) {
			Msg = "解密失败";
		}

		LabelAppRunSta->Caption = "运行状态：" + Msg;
	}
	else if ((BootParam.RunSta & RUN_STA_MODE_MASK) == RUM_MODE_BOOT) {
		LabelAppRunSta->Caption = "运行状态：停止";
	}
	else {
		LabelAppRunSta->Caption = "运行状态：N/A";
	}

	AppRunStaUpdated = true;

	// Msg.sprintf("运行状态：%d/%d/%d", BootParam.WDTRstTimes, BootParam.LastRstTime, BootParam.RunTime);
	// LabelAppRunSta->Caption = Msg;

	Msg.sprintf(L"Flash大小：%d KB", BootParam.FlashSize*32);
	Label1->Caption = Msg;

	Msg.sprintf(L"触摸屏 (%4X:%4X) ", MainForm->HIDSendThread->usVid,
		MainForm->HIDSendThread->usPid);

	if (BootParam.Flags & BOOT_FLAG_WIN7) {
		Msg += "Win7";
	}
	else {
		Msg += "Windows XP";
	}

	if (BootParam.Flags & BOOT_FLAG_RO) {
		Msg += " 读保护";
	}

	if (BootParam.Flags & BOOT_FLAG_WO) {
		Msg += " 写保护";
	}

	GroupBox2->Caption = Msg;

	UpdateFlashParam(BootParam.BootLoaderVersion & 0x7F, BootParam.FlashSize);
}

void __fastcall TFormUpdate::ButtonGetStaClick(TObject *Sender) {
	LabelBoot->Caption = "Boot版本：";
	LabelFwSta->Caption = "固件状态：";
	LabelFirmwareVer->Caption = "固件版本：";
	LabelResetReson->Caption = "复位原因：";
	LabelHWVer->Caption = "硬件版本：";
	LabelAppRunSta->Caption = "运行状态：";
	Label1->Caption = "Flash大小：";

	MainForm->HIDSendThread->GetDeviceSta();
}

void __fastcall TFormUpdate::TimerUpdateTimer(TObject *Sender) {
	if (MainForm->HIDSendThread->Sta == IDLE) {
		Button1->Enabled = true;
		ButtonGetSta->Enabled = true;
		ButtonUpdate->Enabled = true;
		ButtonSwMode->Enabled = true;

		TimerUpdate->Enabled = false;

		StatusBar1->SimpleText = L"升级完成！";

		if (CheckBox1->Checked == true) {
			MainForm->HIDSendThread->DeviceRun();
			// Timer1->Enabled = true;
		}
	}
	else {
		int Percent = 0;
		String Msg = "固件升级中，请稍候...";

		if (MainForm->HIDSendThread->Len) {
			Percent =
				MainForm->HIDSendThread->SendLen * 100 /
				MainForm->HIDSendThread->Len;
		}

		Msg += String(Percent) + "%";

		StatusBar1->SimpleText = Msg;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormUpdate::ButtonSwModeClick(TObject *Sender) {
	MainForm->HIDSendThread->SubmitCmd(BOOT_CMD_SW_MODE, NULL, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TFormUpdate::FormShow(TObject *Sender) {
	if (MainForm->HIDSendThread->Connected == FALSE) {
		ButtonUpdate->Enabled = false;
		ButtonGetSta->Enabled = false;
		ButtonSwMode->Enabled = false;
	}
	else {
		ButtonUpdate->Enabled = true;
		ButtonGetSta->Enabled = true;
		ButtonSwMode->Enabled = true;
	}

	Button3->Visible = true;

	LabelHWVer->Hint = "1.10 = V1.1, 21.5\" PT       \n\
						4.60 = V4.6, 69Leds          \n\
						5.40 = V5.4, 96Leds          \n\
						5.20 = V5.2 21\"-27\" typex  \n\
						6.50 = V6.5, 72Leds 15mm     \n\
						7.00 = V7.0, 72Leds 22mm     \n\
						8.00 = V8.0, 26inch          \n\
						8.10 = R1 21.3inch           \n\
						8.20 = V8.2, 21.5\" PD V1    \n\
						9.00 = typeL PTV9.0          \n\
						10.00 = typeL PDV10.0        \n\
						6.70 = typeL PDV6.7  from V10.0   \n\
						6.80 = typeL PDV6.8  from V10.0   \n\
						12.00 = typex PDV11.0 19\" ~ 21\" \n\
						12.10 = typex PDV11.0 15\" ~ 19\" \n\
						6.90 = typeL PDV6.9 from V6.7     \n\
						13.00 = typeL PDV13.0             \n\
						13.10 = typeL PTV13.0";

	DispSta();
}

// ---------------------------------------------------------------------------
void __fastcall TFormUpdate::DevConnect(void) {
	Caption = "固件升级 - 设备已连接!";

	ButtonUpdate->Enabled = true;
	ButtonGetSta->Enabled = true;
	ButtonSwMode->Enabled = true;
}

void __fastcall TFormUpdate::DevDisConnect(void) {
	Caption = "固件升级 - 设备未连接!";

	ButtonUpdate->Enabled = false;
	ButtonGetSta->Enabled = false;
	ButtonSwMode->Enabled = false;
}

// ---------------------------------------------------------------------------

void __fastcall TFormUpdate::Timer1Timer(TObject *Sender) {
	static int step = 0;
	static int retry = 0;

	switch (step) {
	case 0:
		break; // wait privious command
		// case 1: AppRunStaUpdated = false; MainForm->HIDSendThread->SubmitCmd(BOOT_CMD_GET_STA, NULL, 0); break;
		// case 2: if(AppRunStaUpdated == false) {
		// return;
		// } else  if(retry < 3 || ((BootParam.RunSta & RUN_STA_MODE_MASK) == RUM_MODE_BOOT && retry < 5))  {
		// MainForm->HIDSendThread->SubmitCmd(BOOT_CMD_SW_MODE, NULL, 0);
		// step = 0;
		// retry += 1;
		// }
		// break;
	case 1:
		MainForm->HIDSendThread->SubmitCmd(BOOT_CMD_SW_MODE, NULL, 0);
		break;
	case 2:
		MainForm->HIDSendThread->SubmitCmd(BOOT_CMD_GET_STA, NULL, 0);
		break;
	case 3:
		MainForm->HIDSendThread->SubmitCmd(BOOT_CMD_SW_MODE, NULL, 0);
		break;
	case 4:
		if (BootParam.BootLoaderVersion == 16 ||
			BootParam.BootLoaderVersion == 18) {
			step += 1;
		}
		else {
			MainForm->HIDSendThread->SubmitCmd(BOOT_CMD_GET_STA, NULL, 0);
		}
		break;
	case 5:
		break; // MainForm->HIDSendThread->SubmitCmd(BOOT_CMD_SW_MODE, NULL, 0); break;
	case 6:
		MainForm->HIDSendThread->SubmitCmd(BOOT_CMD_READ_SERIAL, NULL, 0);
		break;
	case 7:
		MainForm->HIDSendThread->SubmitCmd(BOOT_CMD_CPUID, NULL, 0);
		break;
	case 8:
		MainForm->HIDSendThread->SubmitCmd(BOOT_CMD_READ_PARAM, NULL, 0);
		break;
		// case 4: case 5: case 6: case 7: break; //wait for  BOOT_CMD_READ_PARAM
		// case 4: HIDSendThread->SubmitCmd(BOOT_CMD_READ_USER_PARAM, NULL, 0);  break;
		// case 9: case 10: case 11: case 12: break;
		// case 4: HIDSendThread->SubmitCmd(APP_CMD_READ_SCAN_PARAM, NULL, 0);  break;
	default:
		step = 0;
		retry = 0;
		Timer1->Enabled = false;
		break;
	}

	step += 1;
}
// ---------------------------------------------------------------------------
