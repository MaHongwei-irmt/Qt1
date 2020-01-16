//---------------------------------------------------------------------------

#ifndef DevicesH
#define DevicesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TDeviceView : public TForm
{
__published:	// IDE-managed Components
	TTreeView *TreeViewUSB;
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	HANDLE handle;
	uint8_t USBRev[2];

	__fastcall TDeviceView(TComponent* Owner);
	BOOL __fastcall DeviceOpen(HANDLE &handle, WORD wVID, WORD wPID);
};
//---------------------------------------------------------------------------
extern PACKAGE TDeviceView *DeviceView;
//---------------------------------------------------------------------------
#endif
