//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Devices.h"
#include <setupapi.h>

#include "..\\hid\\inc\\Hidsdi++.h";

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

const WORD MY_VID = 0x23c9;
const WORD MY_PID = 0x5666;

BOOL __fastcall TDeviceView::DeviceOpen(HANDLE &handle, WORD wVID, WORD wPID)
{
	//HANDLE HidHandle;
	BOOL bRet = FALSE;
	GUID hidGuid;
	HDEVINFO hardwareDeviceInfo;
	SP_INTERFACE_DEVICE_DATA deviceInfoData;
	PSP_INTERFACE_DEVICE_DETAIL_DATA functionClassDeviceData = NULL;
	ULONG predictedLength = 0;
	ULONG requiredLength = 0;

	CloseHandle(handle);
	handle = INVALID_HANDLE_VALUE;
	
	deviceInfoData.cbSize = sizeof(SP_INTERFACE_DEVICE_DATA);
	
	HidD_GetHidGuid(&hidGuid);
	hardwareDeviceInfo = SetupDiGetClassDevs(&hidGuid, NULL, NULL, (DIGCF_PRESENT|DIGCF_DEVICEINTERFACE));

	for (int i=0; i<128; i++)
	{
		if (!SetupDiEnumDeviceInterfaces(hardwareDeviceInfo, 0, &hidGuid, i, &deviceInfoData)) continue;
			
		SetupDiGetDeviceInterfaceDetail(hardwareDeviceInfo, &deviceInfoData, NULL, 0, &requiredLength, NULL);
		
		predictedLength = requiredLength;
		
		functionClassDeviceData = (PSP_INTERFACE_DEVICE_DETAIL_DATA)malloc(predictedLength);
		
		if (!functionClassDeviceData) 
			continue;
		
		functionClassDeviceData->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);

		if (!SetupDiGetDeviceInterfaceDetail (hardwareDeviceInfo, &deviceInfoData, functionClassDeviceData, predictedLength, &requiredLength, NULL)) 
			break;
		
		handle = CreateFile(functionClassDeviceData->DevicePath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		
		if (handle != INVALID_HANDLE_VALUE)
		{
			HIDD_ATTRIBUTES attri;
			
			HidD_GetAttributes(handle, &attri);

			String Msg = "";

			Msg.sprintf("VID: 0x%.4X  PID: 0x%.4X  Ver: 0x%.4X", attri.VendorID, attri.ProductID, attri.VersionNumber);

			if ((attri.VendorID == wVID) && (attri.ProductID == wPID))
			{
				bRet = TRUE;
				Msg += " - This is my usb hid device!";
				TreeViewUSB->Items->Add(0, Msg);

				//return bRet;
			}

			TreeViewUSB->Items->Add(0, Msg);

			CloseHandle(handle);
			handle = INVALID_HANDLE_VALUE;
		}
	}
	
	SetupDiDestroyDeviceInfoList(hardwareDeviceInfo);
	
	return bRet;
}

TDeviceView *DeviceView;

//---------------------------------------------------------------------------
__fastcall TDeviceView::TDeviceView(TComponent* Owner)
	: TForm(Owner)
{


}
//---------------------------------------------------------------------------
void __fastcall TDeviceView::FormShow(TObject *Sender)
{
	TTreeNode *Node;

	TreeViewUSB->Items->Clear();
	DeviceOpen(handle, MY_VID, MY_PID);
}
//---------------------------------------------------------------------------
