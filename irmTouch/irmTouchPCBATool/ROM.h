// ---------------------------------------------------------------------------

#ifndef ROMH
#define ROMH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>

#define STM32FLASH_SIZE 128

#define FLASH_BASE_ADD		((uint32_t)0x8000000)

// #define FLASH_END_ADD (FLASH_BASE_ADD + FLASH_PAGES * FLASH_PAGE_SIZE)

#define BOOTLOADER_SIZE		(16*1024)
#define MAIN_SIZE 			(1024*1024)

#define TYPE_APP		1
#define TYPE_SERIAL		2
#define TYPE_SECRET		3
#define TYPE_TABLE		4
#define TYPE_PARAM		5
#define TYPE_USER_PARAM	6
#define TYPE_CALIB		7

#define MODE_MAIN_FLASH  	(1<<0)
#define MODE_EEPROM		 	(1<<1)
#define MODE_CONFIG      	(1<<2)
#define MODE_USR         	(1<<3)
#define MODE_PROGRAM	 	(1<<4)
#define MODE_VERIFY		 	(1<<5)
#define MODE_READ		 	(1<<6)
#define MODE_ENCODE		 	(1<<7)

typedef struct UPDATE_CTRL_ {
	uint8_t UpdateMode;
	uint8_t MainFlash[MAIN_SIZE];
	uint8_t Valid;
	uint32_t TotalLen;
	uint32_t Len;
	uint32_t StartAdd;
	uint32_t ROMStartAdd;
	uint32_t ROMEndAdd;
	uint32_t FileSize;

	uint32_t AppStartAdd;
	uint32_t AppEndAdd;
	uint32_t AppSize;
	uint16_t FlashSize;

	uint32_t ReserveStartAdd;
	uint32_t SecretStartAdd;
	uint32_t ScanTableStartAdd;
	uint32_t ParamStartAdd;
} UPDATE_CTRL;

// ---------------------------------------------------------------------------
class TFormROM : public TForm {
__published: // IDE-managed Components
	TStringGrid *StringGrid1;
	TStringGrid *StringGrid3;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TButton *Button1;
	TButton *Button2;
	TStringGrid *StringGrid2;

	void __fastcall FormShow(TObject *Sender);

private: // User declarations
public: // User declarations
	__fastcall TFormROM(TComponent* Owner);
	void __fastcall ShowRomData(TStringGrid *StringGrid, unsigned char *Data,
		unsigned len, unsigned StartAdd);
};

// ---------------------------------------------------------------------------
extern PACKAGE TFormROM *FormROM;
extern UPDATE_CTRL UpdateCtrl;
// ---------------------------------------------------------------------------
#endif
