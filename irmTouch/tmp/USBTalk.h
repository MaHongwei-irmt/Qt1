// ---------------------------------------------------------------------------

#ifndef USBTalkH
#define USBTalkH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include ".\hid\inc\Hidsdi++.h"
#include ".\hid\inc\Hidpi++.h"
#include ".\winusb\include\winusb.h"

#include "FirmwareUpdate.h"

#include <vector>

#include <setupapi.h>

using namespace std;
// ---------------------------------------------------------------------------

typedef enum {
	BUSY = 0, IDLE
} SEND_STA;

const int REPORT_FIFO_MASK = 0xFFF;
const int REPORT_FIFO_LEN = REPORT_FIFO_MASK + 1;

#define BOOT_FLAG_WIN7		(1<<0)
#define BOOT_FLAG_RO		(1<<1)
#define BOOT_FLAG_WO		(1<<2)

#define MAX_REPORT_PACKET_SIZE  	2048

#define MAX_CANDIDATE_HIDDEV		16
#define HIDDEV_REPORT_IN_ID			1
#define HIDDEV_REPORT_OUT_ID		2
#define HIDDEV_REPORT_PACKET_SIZE	63

typedef struct HIDDevice {
	HANDLE Handle;
	OVERLAPPED Overlapped;
	HIDP_CAPS Caps;
	// HIDD_CONFIGURATION Config;
	HANDLE Event;
	uint8_t Reports[REPORT_FIFO_LEN][MAX_REPORT_PACKET_SIZE];
	int ReportInPtr;
	int ReportOutPtr;
} irmtouch_t;

typedef struct CmdFifo {
	uint8_t CmdReport[64];
	uint8_t CmdReportLen;
	uint8_t CmdTrys;
	bool read;
} CmdFifo_t;

struct IrmDevInfo {
	String devPath;
	BOOL used;
};

class USBTalk : public TThread {
private:
protected:
	void __fastcall Execute();

public:
	__fastcall USBTalk(bool CreateSuspended);
	bool __fastcall SubmitTask(uint8_t Cmd, uint32_t Len, uint8_t *Data,
		PROGRAM_HEAD *Head);
	bool __fastcall SendUpdateFrame(void);

	BOOL EnumerateHidDevice(USHORT vid, USHORT pid);
	BOOL EnumerateHidDevice2(WORD wVID, WORD wPID);
	BOOL RegisterHidDeviceNotify(void);
	BOOL ReadHid(irmtouch_t *Dev, uint8_t *Buf, uint8_t BufLen);
	BOOL WriteHid(irmtouch_t *Dev, uint8_t *Data, uint8_t Len,
		uint32_t &Written);
	BOOL FindHid(void);
	void ReadyForWR(irmtouch_t *Dev);
	void __fastcall SendCmd(void);
	void __fastcall SubmitCmd(uint8_t Cmd, uint8_t *Data, uint8_t Len);
	void __fastcall GetDeviceSta(void);
	void __fastcall DeviceRun(void);
	BOOL FindIRMTouchDevice(void);
	int32_t WriteFMsg(uint8_t *buf, uint32_t &len, uint16_t value);
	int32_t ReadFMsg(uint8_t *buf, uint32_t &len, uint16_t value);

	uint32_t StartAdd;
	uint32_t EndAdd;
	uint32_t Len;
	uint32_t SendLen;
	uint8_t Cmd;
	PROGRAM_HEAD *Head;
	uint8_t *DataBuf;
	bool Cancle;
	SEND_STA Sta;

	irmtouch_t DebugHidDev;

	BOOL Connected;
	BOOL Registered;
	HDEVNOTIFY HidDevNotify;

	bool Exit;

	CmdFifo_t CmdFifo[REPORT_FIFO_LEN];
	int CmdFifoInPtr;
	int CmdFifoOutPtr;
	String EnumerateMsg;

	uint8_t Key[16];
	uint8_t ReportInId;
	uint8_t ReportOutId;
	uint8_t ReportPacketSize;

	USHORT usPid, usVid;

	vector<IrmDevInfo>m_DevInfo;

	String m_devPath;
	HANDLE m_devHandle;
	WINUSB_INTERFACE_HANDLE m_usbHandle;
	UCHAR m_UsbSpeed;
	USB_INTERFACE_DESCRIPTOR m_intDesc;
	USB_DEVICE_DESCRIPTOR m_DevDesc;

	bool IsHidDevice;
	uint8_t buckrev[16 * 1024];

	BOOT_PARAM BootParam;

	FILE *fplogframe;

	FILE *fpDebugMsg;
	SYSTEMTIME stLocalDebug;
};
// ---------------------------------------------------------------------------
#endif
