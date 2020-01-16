//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------
USEFORM("MAIN.CPP", tc);
USEFORM("CHILDWIN.CPP", MDIChild);
USEFORM("about.cpp", AboutBox);
USEFORM("image.cpp", FormPoints);
USEFORM("FirmwareUpdate.cpp", FormUpdate);
USEFORM("ROM.cpp", FormROM);
USEFORM("Config.cpp", FormConfig);
USEFORM("FormAdd.cpp", FormAddStdParam);
USEFORM("DataBase.cpp", FormViewDataBase);
USEFORM("Calibrate.cpp", FormCalibrate);
USEFORM("CalibPerformance.cpp", FormCalibPerform);
USEFORM("ConfigList.cpp", FormConfigList);
USEFORM("BatchSet.cpp", FormBatchSet);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Application->Initialize();
	Application->CreateForm(__classid(Ttc), &tc);
		Application->CreateForm(__classid(TAboutBox), &AboutBox);
		Application->CreateForm(__classid(TFormPoints), &FormPoints);
		Application->CreateForm(__classid(TFormUpdate), &FormUpdate);
		Application->CreateForm(__classid(TFormROM), &FormROM);
		Application->CreateForm(__classid(TFormConfig), &FormConfig);
		Application->CreateForm(__classid(TFormAddStdParam), &FormAddStdParam);
		Application->CreateForm(__classid(TFormViewDataBase), &FormViewDataBase);
		Application->CreateForm(__classid(TFormCalibrate), &FormCalibrate);
		Application->CreateForm(__classid(TFormCalibPerform), &FormCalibPerform);
		Application->CreateForm(__classid(TFormConfigList), &FormConfigList);
		Application->CreateForm(__classid(TFormBatchSet), &FormBatchSet);
		Application->Run();

	return 0;
}
//---------------------------------------------------------------------
