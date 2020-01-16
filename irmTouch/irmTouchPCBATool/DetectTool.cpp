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
#include "math.h"

#include "cstdlib.h"
 #include <math.h>


#include "Config.h"
#include "stdio.h"
#include "math.h"
#include "stdint.h"

#include "main.h"
#include "firmwareUpdate.h"
#include "rom.h"

#include "FormAdd.h"

#include <inifiles.hpp>

#include "untBeatCode.h"
#include "hex.h"
#include "GenTable.h"

#include "main.h"
#include "DataBase.h"

#include "Calibrate.h"
#include "CalibPerformance.h"

#include "ConfigList.h"
#include "classes.hpp"

#include "BatchSet.h"

#include <math.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDetectTool *DetectTool;
//---------------------------------------------------------------------------
__fastcall TDetectTool::TDetectTool(TComponent* Owner)
	: TForm(Owner)
{
	mainActive = false;
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
void __fastcall TDetectTool::FormDblClick(TObject *Sender) {

	POINT p;

	GetCursorPos(&p);

	if (p.x > 1000 && p.y > 800 && debug) {
		MainForm->WindowState = wsNormal;
	}

}

// ---------------------------------------------------------------------------

void __fastcall TDetectTool::Timer1Timer(TObject *Sender) {

	if (MainForm->WindowState != wsMinimized) {

	}
	else if (DetectTool->ctlInfoShow->Visible) {
		SendMessage(DetectTool->ctlInfoShow->Handle, WM_VSCROLL,
			SB_BOTTOM, 0); // SB_ENDSCROLL       , 0);//SB_LINEDOWN,   0);
	}

	String Msg;
	POINT p;
	GetCursorPos(&p);
	Msg.sprintf(L"IRMTouch Factory Tools %d-%d  %d", p.x, p.y, debug);
	DetectTool->ctlInfoShow->Lines->Add(Msg);
}
// ---------------------------------------------------------------------------

void __fastcall TDetectTool::FormActivate(TObject *Sender) {
	if (MainForm->WindowState != wsMinimized) {
		MainForm->SetFocus();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDetectTool::FormCreate(TObject *Sender) {

	AnsiString thestring;
	TIniFile *pIniFile = new TIniFile(ExtractFilePath(Application->ExeName) +
		"para.ini");
	thestring = pIniFile->ReadString("debug", "debug", "");
	debug = atoi(thestring.c_str());
	delete pIniFile;
}
// ---------------------------------------------------------------------------
