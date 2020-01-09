//---------------------------------------------------------------------------

#ifndef FirmwareUpdateH
#define FirmwareUpdateH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------

#define BOOT_FLAG_WIN7		(1<<0)
#define BOOT_FLAG_RO		(1<<1)
#define BOOT_FLAG_WO		(1<<2)

#pragma   pack(push,1)
typedef struct BootParam_ {
	uint8_t	BootLoaderVersion;
	uint8_t ResetType;
	uint16_t WDTRstTimes;
	uint16_t LastRstTime;
	uint16_t RunTime;
	uint16_t HardwareVer;
	uint16_t SoftwareVer;
	uint8_t AppSta;
	uint8_t FlashSize;
	uint8_t RunSta;
	uint8_t Flags;
}BOOT_PARAM;

typedef struct FlashParam_ {
	uint32_t FlashSize;
	uint32_t BootSize;
	uint32_t ParamStartAdd;
	uint32_t ParamSize;
	uint32_t SerialStartAdd;
	uint32_t SerialSize;
	uint32_t ReserveStartAdd;
	uint32_t ReserveSize;
	uint32_t SecretStartAdd;
	uint32_t SecretSize;
	uint32_t ScanTabStartAdd;
	uint32_t ScanTabSize;
	uint32_t AppStartAdd;
	uint32_t AppSize;
	uint32_t EndAreaSize;

	uint32_t UserParamStartAdd;
	uint32_t UserParamSize;
	uint32_t CalibrateStartAdd;
	uint32_t CalibrateSize;

}FLASH_PARAM;

typedef struct FileVersion_ {
	uint8_t	BootLoaderVersion;
	uint16_t HardwareVer;
	uint16_t SoftwareVer;
	uint8_t FlashSize;
}FILE_VERSION;

typedef struct PROGRAM_HEAD_ {
	uint8_t MemType;
	uint8_t Mode;
	uint32_t StartAdd;
	uint8_t Len;
}PROGRAM_HEAD;
#pragma   pack(pop)

class TFormUpdate : public TForm
{
__published:	// IDE-managed Components
	TGroupBox * GroupBox1;
	TEdit *EditFirmware;
	TButton *Button1;
	TLabel *Label237;
	TLabel *Label238;
	TLabel *Label239;
	TLabel *Label240;
	TLabel *Label257;
	TLabel *Label258;
	TLabel *Label259;
	TLabel *Label275;
	TLabel *Label276;
	TOpenDialog *OpenDialog;
	TGroupBox *GroupBox2;
	TStatusBar *StatusBar1;
	TLabel *LabelBoot;
	TLabel *LabelHWVer;
	TLabel *LabelFirmwareVer;
	TLabel *LabelFwSta;
	TLabel *LabelAppRunSta;
	TLabel *LabelResetReson;
	TLabel *Label1;
	TLabel *Label3;
	TButton *ButtonUpdate;
	TButton *Button3;
	TButton *ButtonGetSta;
	TLabel *Label2;
	TButton *ButtonSwMode;
	TCheckBox *CheckBox1;
	TTimer *TimerUpdate;
	TCheckBox *CheckBoxEncode;
	TTimer *Timer1;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall ButtonUpdateClick(TObject *Sender);
	void __fastcall ButtonGetStaClick(TObject *Sender);
	void __fastcall TimerUpdateTimer(TObject *Sender);
	void __fastcall ButtonSwModeClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
private:	// User declarations

public:		// User declarations
	__fastcall TFormUpdate(TComponent* Owner);
	bool __fastcall InterpretFile(String FileName);
	bool __fastcall CheckFile(String FileName);
	void __fastcall DispSta(void);
	uint32_t __fastcall CRC32(uint32_t *Data, uint32_t Len);
	void __fastcall DevConnect(void);
	void __fastcall DevDisConnect(void);
	void __fastcall UpdateFlashParam(uint8_t BootVersion, uint8_t FlashSize);
	void __fastcall MakeKey(uint8_t *Key);

	BOOT_PARAM BootParam;
	FLASH_PARAM FlashParam;
	PROGRAM_HEAD Head;
	FILE_VERSION FileVersion;

    bool IsAPUFirmware;
	bool AppRunStaUpdated;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormUpdate *FormUpdate;

//bootloader command
const uint8_t BOOT_CMD_GET_STA = 0x10;
const uint8_t BOOT_CMD_SW_MODE = 0x11;

const uint8_t BOOT_CMD_DOWN_VIDPID = 0x12;

const uint8_t BOOT_CMD_WO_ON = 0x14;
const uint8_t BOOT_CMD_WO_OFF = 0x15;
const uint8_t BOOT_CMD_CPUID = 0x16;
const uint8_t BOOT_CMD_READ_SERIAL = 0x17;
const uint8_t BOOT_CMD_READ_PARAM = 0x18;
const uint8_t BOOT_CMD_READ_USER_PARAM = 0x19;

//application command
const uint8_t APP_CMD_GET_STA = 0x20;
const uint8_t APP_CMD_SW_REPORT = 0x21;
const uint8_t APP_CMD_READ_SCAN_PARAM = 0x22;

const uint8_t APP_CMD_SET_CALIB_COFF = 0x24;
const uint8_t APP_CMD_GET_AUTO_GAIN = 0x25;
const uint8_t APP_CMD_READ_SCAN_PARAM_TRI = 0x26;

const uint8_t APP_CMD_GET_MANUPRO_INFO = 0x27;

const uint8_t RUM_MODE_APP = 1;
const uint8_t RUM_MODE_BOOT = 2;

#define REPORT_MODE_NORMAL		0x00
#define REPORT_MODE_AD			0x01
#define REPORT_MODE_INTERVAL	0x02
#define REPORT_MODE_POINT		0x03

#define RUN_STA_MODE_MASK	(3<<0)
#define RUN_MODE_APP	1
#define RUN_MODE_BOOT	2

#define RUN_STA_PARAM_ERR	(1<<2)
#define RUN_STA_SCAN_ERR	(1<<3)
#define RUN_STA_SECRET_ERR	(1<<4)
#define RUN_STA_MEM_ERR		(1<<5)
#define RUN_STA_PWR_ERR		(1<<6)

#define APP_OK			1
#define APP_EMPTY		2
#define APP_CORRUPT		3

#define RESET_COLD		0
#define RESET_IWDT		1
#define RESET_SOFT		2
#define RESET_LPOW		3
#define RESET_UNKONWN	4

//---------------------------------------------------------------------------
#endif
