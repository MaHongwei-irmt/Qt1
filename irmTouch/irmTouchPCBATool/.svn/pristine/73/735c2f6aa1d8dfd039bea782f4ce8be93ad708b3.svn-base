//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------
USEFORM("FirmwareUpdate.cpp", FormUpdate);
USEFORM("FormAdd.cpp", FormAddStdParam);
USEFORM("ROM.cpp", FormROM);
USEFORM("image.cpp", FormPoints);
USEFORM("MAIN.CPP", MainForm);
USEFORM("CalibPerformance.cpp", FormCalibPerform);
USEFORM("about.cpp", AboutBox);
USEFORM("BatchSet.cpp", FormBatchSet);
USEFORM("ConfigList.cpp", FormConfigList);
USEFORM("DataBase.cpp", FormViewDataBase);
USEFORM("Calibrate.cpp", FormCalibrate);
USEFORM("CHILDWIN.CPP", MDIChild);
USEFORM("Config.cpp", FormConfig);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	Application->Initialize();
		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->CreateForm(__classid(TAboutBox), &AboutBox);
		Application->CreateForm(__classid(TFormCalibPerform), &FormCalibPerform);
		Application->CreateForm(__classid(TFormCalibrate), &FormCalibrate);
		Application->CreateForm(__classid(TFormConfig), &FormConfig);
		Application->CreateForm(__classid(TFormConfigList), &FormConfigList);
		Application->CreateForm(__classid(TFormViewDataBase), &FormViewDataBase);
		Application->CreateForm(__classid(TFormUpdate), &FormUpdate);
		Application->CreateForm(__classid(TFormAddStdParam), &FormAddStdParam);
		Application->CreateForm(__classid(TFormPoints), &FormPoints);
		Application->CreateForm(__classid(TFormROM), &FormROM);
		Application->CreateForm(__classid(TFormBatchSet), &FormBatchSet);
		Application->Run();
	return 0;
}
//---------------------------------------------------------------------
