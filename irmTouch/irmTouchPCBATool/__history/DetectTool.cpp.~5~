//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DetectTool.h"

#include "Main.h"
#include "About.h"
#include "childwin.h"
#include "stdio.h"
#include "time.h"

#include "ADProcess.h"

#include "Boundary.h"

#include "image.h"
#include "iostream.h"
#include "FirmwareUpdate.h"
#include "config.h"
#include <fstream.h>
#include <Dbt.h>
#include <objbase.h>
#include <initguid.h>

#include "Devices.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDetectTool *DetectTool;
//---------------------------------------------------------------------------
__fastcall TDetectTool::TDetectTool(TComponent* Owner)
	: TForm(Owner)
{
	memset(key, 0, sizeof(key));
	mainActive = false;
	keyPress = false;
}

//---------------------------------------------------------------------------
void __fastcall TDetectTool::FormClose(TObject *Sender, TCloseAction &Action) {

	if (!MainForm->HIDSendThread->Suspended &&
		MainForm->HIDSendThread->Sta == IDLE) {
		// sending something?  fix me !!!!!!!!
	}

	MainForm->HIDSendThread->Exit = true;

	Application->UnhookMainWindow(MainForm->AppHookFunc);

	if (MainForm->fplogframe)
		fclose(MainForm->fplogframe);

	MainForm->fplogframe = NULL;

	if (mainActive) {
		mainActive = false;
		MainForm->Close();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDetectTool::FormKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift)

{
	keyPress = true;

	if (key[0] == 0) {
		key[0] = Key;
	}
	else if (key[1] == 0) {
		key[1] = Key;
	}
	else if (key[2] == 0) {
		key[2] = Key;
	}
	else if (key[3] == 0) {
		key[3] = Key;
	}

	if (key[0] == 37 && key[1] == 38 && key[2] == 39 && key[3] == 40) {

		memset(key, 0, sizeof(key));

		if (!mainActive) {
			Close();
		}
		else {

			try {
				MainForm->Show();
			}
			catch (Exception &exception) {

				 Application->ShowException(&exception);
			}

		}

	}
	else if (key[0] != 37) {
		memset(key, 0, sizeof(key));
	}

}

// ---------------------------------------------------------------------------

void __fastcall TDetectTool::FormKeyUp(TObject *Sender, WORD &Key,
	TShiftState Shift)

{
	keyPress = false;
}

// ---------------------------------------------------------------------------
void __fastcall TDetectTool::FormDblClick(TObject *Sender) {
	if (keyPress) {
		if (!mainActive) {
			Close();
		}
		else {

			try {
				MainForm->Show();
			}
			catch (Exception &exception) {

				 Application->ShowException(&exception);
			}

		}
	}
}
// ---------------------------------------------------------------------------
