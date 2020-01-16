//----------------------------------------------------------------------------
#ifndef MainH
#define MainH

//----------------------------------------------------------------------------
#include "ChildWin.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Messages.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <StdCtrls.hpp>
#include <Menus.hpp>
#include <Controls.hpp>
#include <Forms.hpp>
#include <Graphics.hpp>
#include <Classes.hpp>
#include <SysUtils.hpp>
#include <Windows.hpp>
#include <System.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <StdActns.hpp>
#include <ToolWin.hpp>
//----------------------------------------------------------------------------
#include "fstream.h"
#include "iostream.h"
#include "stdint.h"
#include "USBTalk.h"

#include "FrameInfo.h"
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

#define MAX_DEVS	3
const uint16_t VID = 0x23c9;   //0x23c9;//
const uint16_t PID = 0x5666;   //0x5666;//

const int ImagePlay = 12;
const int ImagePause = 11;
const int ImageStop = 7;

const int ImageRecordOn = 2;
const int ImageRecordOff = 1;

const uint32_t DEBUG_FIFO_MASK = 0x3F;
const uint32_t DEBUG_FIFO_LEN = DEBUG_FIFO_MASK + 1;
const uint32_t DEBUG_FRAME_SIZE = 64*1024;

class TMainForm : public TForm
{
__published:
	TOpenDialog *OpenDialog;
        TStatusBar *StatusBar;
        TActionList *ActionList1;
        TEditCut *EditCut1;
        TEditCopy *EditCopy1;
        TEditPaste *EditPaste1;
        TAction *FileNew1;
        TAction *FileSave1;
        TAction *FileExit1;
        TAction *FileOpen1;
        TAction *FileSaveAs1;
        TWindowCascade *WindowCascade1;
        TWindowTileHorizontal *WindowTileHorizontal1;
        TWindowArrange *WindowArrangeAll1;
        TWindowMinimizeAll *WindowMinimizeAll1;
        TAction *HelpAbout1;
        TWindowClose *FileClose1;
        TWindowTileVertical *WindowTileVertical1;
        TToolBar *ToolBar2;
        TToolButton *ToolButton1;
        TToolButton *ToolButton8;
        TToolButton *ToolButton10;
        TToolButton *ToolButton11;
        TImageList *ImageList1;
	TTimer *TimerFps;
	TToolButton *ToolButton13;
	TTimer *TimerReplay;
	TToolButton *ToolButtonShowImage;
	TToolButton *ToolButtonRecord;
	TToolButton *ToolButtonPrev;
	TToolButton *ToolButtonPlayPause;
	TToolButton *ToolButtonStop;
	TToolButton *ToolButton17;
	TToolButton *ToolButton18;
	TToolButton *ToolButtonNext;
	TToolButton *ToolButtonBackward;
	TToolButton *ToolButtonForward;
	TToolButton *ToolButtonSpdUp;
	TToolButton *ToolButtonSpdDown;
	TTimer *TimerReDraw;
	TToolButton *ToolButton12;
	TToolButton *ToolButton2;
	TToolButton *ToolButton3;
	TToolButton *ToolButton4;
	TToolButton *ToolButton5;
	TToolButton *ToolButton6;
	TToolButton *ToolButton7;
	TToolButton *ToolButtonAD;
	TToolButton *ToolButtonInterval;
	TToolButton *ToolButtonPoint;
	TToolButton *ToolButton16;
	TToolButton *ToolButton19;
	TToolButton *ToolButton20;
	TTimer *TimerData;
	TPopupMenu *PopupMenu1;
	TMenuItem *SigOrg;
	TMenuItem *SigMax;
	TMenuItem *SigMin;
	TMenuItem *SigNormal;
	TPopupMenu *PopupMenu2;
	TMenuItem *LogVer1;
	TMenuItem *LogVer2;
	TMenuItem *SigCorner;
	TToolButton *ToolButton9;
	TMenuItem *SigCheck;
        void __fastcall FileOpen1Execute(TObject *Sender);
	void __fastcall ti(TObject *Sender);
	void __fastcall TimerFpsTimer(TObject *Sender);
	void __fastcall TimerReplayTimer(TObject *Sender);
	void __fastcall ToolButtonRecordClick(TObject *Sender);
	void __fastcall ToolButtonPlayPauseClick(TObject *Sender);
	void __fastcall ToolButtonStopClick(TObject *Sender);
	void __fastcall ToolButtonPrevClick(TObject *Sender);
	void __fastcall ToolButtonNextClick(TObject *Sender);
	void __fastcall ToolButtonBackwardClick(TObject *Sender);
	void __fastcall ToolButtonForwardClick(TObject *Sender);
	void __fastcall ToolButtonSpdUpClick(TObject *Sender);
	void __fastcall ToolButtonSpdDownClick(TObject *Sender);
	void __fastcall TimerReDrawTimer(TObject *Sender);
	void __fastcall ToolButtonShowImageClick(TObject *Sender);
	void __fastcall ToolButton2Click(TObject *Sender);
	void __fastcall ToolButton3Click(TObject *Sender);
	void __fastcall ToolButton4Click(TObject *Sender);
	void __fastcall ToolButton5Click(TObject *Sender);
	void __fastcall ToolButton7Click(TObject *Sender);
	void __fastcall ToolButton16Click(TObject *Sender);
	void __fastcall ToolButton19Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ChangeAppReportMode(TObject *Sender);
	void __fastcall TimerDataTimer(TObject *Sender);
	void __fastcall SigOrgClick(TObject *Sender);
	void __fastcall LogVer1Click(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, char &Key);
	void __fastcall ToolButton9Click(TObject *Sender);
	void __fastcall HandleException(TObject*Sender,Exception*E);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
private:
	//TMDIChild *__fastcall CreateMDIChild(String Name);

	ifstream LogReplay;
	int ReplayFrames;
	bool OnReplay;
public:

	TMDIChild *__fastcall CreateMDIChild(String Name);


	virtual __fastcall TMainForm(TComponent *Owner);
	TMDIChild *__fastcall FindChild(int AxiId);
	void __fastcall LoadLogFile(String FileName);
	void __fastcall StopPlay(void);
	void __fastcall PlayOneFrame(void);
	void __fastcall StartPausePlay(void);
	int __fastcall RollOnFrames(int Num);
	int __fastcall RollBackFrames(int Num);
	void __fastcall ADRecordButtonClick(void);

	void __fastcall Bootloader_Cmd(uint8_t *Data, uint16_t Size);
	void __fastcall Debug_Data(uint8_t *Data, uint16_t Size);
	void __fastcall DeviceConnected(void);
	void __fastcall DeviceDisConnected(void);
	//void __fastcall ProcessMessage(TMsg  &message,   bool &handled);
	//void   __fastcall   WndProc(TMessage   &Message);
	bool   __fastcall   AppHookFunc(TMessage   &Message);
	bool   __fastcall   IsOurUSBDevice(int   iMsgLParam);
	void __fastcall OnDebugData(const void *Data, uint16_t Size);
	void __fastcall LogDebugData(FILE *fp, uint8_t *Data, int Len);

	void __fastcall App_Cmd(uint8_t *Data, uint16_t Size);
	void __fastcall CreatChild(int Childs);
	void __fastcall App_Cmd_Hid(uint8_t *Data, uint16_t Size);
	bool __fastcall Update_Device_Connection(void);

	uint8_t FiFoData[DEBUG_FIFO_LEN][DEBUG_FRAME_SIZE];
	uint32_t FiFoDataLen[DEBUG_FIFO_LEN];
	uint32_t FiFoInPtr;
	uint32_t FiFoOutPtr;
	uint32_t FrameLost;
	uint32_t FrameLen;
	int MTime, CTime, ScanT, ClcTimes;
	float Fps;
	int Frames;

	USBTalk *HIDSendThread;
	String LogFileVersion;
	String LogVersion;

	uint8_t AppReply[16*1024];
	uint16_t AppReplyLen;

	FILE *fplogframe;
};
//----------------------------------------------------------------------------
extern TMainForm *MainForm;
extern TMDIChild *__fastcall MDIChildCreate(void);

//----------------------------------------------------------------------------
#endif    
