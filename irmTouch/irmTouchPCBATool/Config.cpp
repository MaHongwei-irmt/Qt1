// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

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

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormConfig *FormConfig;

AXI_PARAM XParam, YParam;

int ANTI_AXIS = 3;

// ---------------------------------------------------------------------------
__fastcall TFormConfig::TFormConfig(TComponent* Owner) : TForm(Owner) {
	memset(&BootParam, 0, sizeof(BootParam));
}
// ---------------------------------------------------------------------------

bool __fastcall TFormConfig::Gen_DMATable_Rule_Check(uint32_t Flags) {
	bool SwEmit = (Flags & 0x02) != 0;
	bool DualSpiData = (Flags & 0x04) != 0;
	bool SPI8Bits = (Flags & 0x10) != 0;
	bool ReverseX = (Flags & 0x40) != 0;
	bool ReverseY = (Flags & 0x80) != 0;
	String Msg = "";

	if (XParam.Get_Rev_Data == NULL || YParam.Get_Rev_Data == NULL) {
		if (!SwEmit) {
			Msg += ("V10.0 只能切换发射灯!\n");
		}

		if (!SPI8Bits) {
			Msg += ("V10.0 为8Bit SPI数据!\n");
		}

		if (DualSpiData) {
			Msg += ("V10.0 为单数据线SPI!\n");
		}

		if (ReverseX || ReverseY) {
			Msg += ("V10.0 不能反向扫描!\n");
		}

		for (int i = 0; i < XParam.Boards - 1; i += 1) {
			if ((XParam.BoradLeds[i] & 0x07) != 0) {
				// Msg.cat_printf("V10.0中间截板必须是8的整数倍！X:(%d,%d)\n", i+1, XParam.BoradLeds[i]);
			}
		}

		for (int i = 0; i < YParam.Boards - 1; i += 1) {
			if ((YParam.BoradLeds[i] & 0x07) != 0) {
				// Msg.cat_printf("V10.0中间截板必须是8的整数倍！Y:(%d,%d)\n", i+1, YParam.BoradLeds[i]);
			}
		}
	}

	if (XParam.Axis != YParam.Axis) {
		Msg.cat_printf(L"X,Y轴数不同(x=%d, y=%d)，目前不支持，请设定为相同值！", XParam.Axis,
			YParam.Axis);
	}

	if (XParam.Boards + YParam.Boards > MAX_BOARDS) {
		Msg.cat_printf(L"X,Y(半周)板子数超过30,过多!");
	}

	if (XParam.Get_Emit_Data == Get_EmitX_Data_FCT) {
		if (!CheckBoxGPIOSwitch->Checked) {
			Msg += "fct 需要使用GPIO切换";
		}
	}

	if (XParam.ScanGap != YParam.ScanGap && YParam.TotalLeds > 0) {
		Msg += "XY扫描间隔需相同";
	}

	if ((XParam.ScanGap != 0 || YParam.ScanGap != 0)
		&& XParam.Get_Emit_Data != Get_Emit_Data_V54_MagicConfig &&
		XParam.Get_Emit_Data != Get_Emit_Data_V54) {
		Msg += "扫描间隔仅用于W系列(v54)";
	}

	if (Msg.Length()) {
		ShowMessage(Msg);
		return false;
	}
	else {
		return true;
	}
}

#define ALIGN_DWORD(a) (((a) + 3) & ~(0x03))

bool __fastcall TFormConfig::GenScanTable(void) {
#if 1
	uint16_t HeadLen;
	uint32_t TabBaseAdd;
	uint32_t Offset;
	uint32_t XAxis, YAxis, AllAxis;
	uint32_t MixXAxis, MixYAxis, MixAllAxis;
	uint32_t XMaxOff, YMaxOff;
	// uint32_t XLeds, YLeds, TotalLeds;
	uint32_t Flag = 0;
	FLASH_PARAM *fp = &FormUpdate->FlashParam;
	uint16_t SkipAxis;

	if (fp->FlashSize < 64 * 1024) {
		ShowMessage(L"请先到状态页获取设备信息！");
		return false;
	}

	Button4Click(NULL);

	StatusBar1->SimpleText = L"";

	XAxis = XParam.Axis;
	YAxis = YParam.Axis;
	AllAxis = (XAxis + YAxis);

	MixXAxis = XParam.MixAxis;
	MixYAxis = YParam.MixAxis;
	MixAllAxis = (MixXAxis + MixYAxis);

	XMaxOff = XParam.MaxOff;
	YMaxOff = YParam.MaxOff;

	if (FormUpdate->BootParam.FlashSize == 0)
		FormUpdate->BootParam.FlashSize = 128 / 32;

	// TabBaseAdd = FLASH_BASE_ADD + FormUpdate->BootParam.FlashSize * 32 * 1024 - PARAM_SIZE - SCAN_TABLE_SIZE;
	TabBaseAdd = FLASH_BASE_ADD + fp->ScanTabStartAdd;

	for (int i = 0; i < ScanFeatureCheckBox->Count; i += 1) {
		if (((TCheckBox*)ScanFeatureCheckBox->Items[i])->Checked == true) {
			Flag |= (1 << i);
		}
	}

	TabHead.Feature = Flag;
	TabHead.Flags = (Flag & 0x0F) | (((uint8_t)CheckBoxDoubleFirst->Checked)
		<< 4) | (((uint8_t)CheckBoxAntiGlare->Checked) << 5) |
		(((uint8_t)CheckBoxCPLDMagicConfig->Checked) << 6) |
		(((uint8_t)CheckBoxReorderMap->Checked) << 7);

	if (!Gen_DMATable_Rule_Check(Flag)) {
		return false;
	}

	TabHead.XAxis = XAxis;
	TabHead.YAxis = YAxis;
	TabHead.AllAxis = XAxis + YAxis;

	TabHead.XLeds = XParam.TotalLeds;
	TabHead.YLeds = YParam.TotalLeds;

	TabHead.XBoards = XParam.Boards;
	TabHead.YBoards = YParam.Boards;

	TabHead.MarginLeft = XParam.Margin;
	TabHead.MarginRight = XParam.MarginRightDown;
	TabHead.MarginUp = YParam.Margin;
	TabHead.MarginDown = YParam.MarginRightDown;

	TabHead.XEmitVer = ComboBoxEmitXVer->ItemIndex;
	TabHead.XRevVer = ComboBoxRevXVer->ItemIndex;
	TabHead.XBoardLed = _wtoi(ComboBoxXBoardLeds->Text.t_str());
	TabHead.XPowerGroup = XParam.MiniOff;
	TabHead.XJumpLeds = XParam.JumpLeds;
	TabHead.XScanGap = XParam.ScanGap;
	TabHead.XPow = XParam.EmitPower;
	TabHead.XMaxDeg = (uint16_t)(_wtof(EditXoffset->Text.t_str()) * 100);
	TabHead.XLedSpace = (uint16_t)(XParam.Dis * 1000);
	TabHead.XReScan = 0;
	TabHead.XDivMethod = ComboBoxXDivMethod->ItemIndex;
	TabHead.XCutUnUsed = CheckBox17->Checked ? 0xAA : 0;

	memset(TabHead.XCuts, 0, sizeof(TabHead.XCuts));
	for (int i = 0, Cuts = 0; i < XParam.Boards && Cuts < sizeof(TabHead.XCuts);
	i += 1) {
		if (XParam.BoradLeds[i] != TabHead.XBoardLed) {
			TabHead.XCuts[Cuts++] = i;
			TabHead.XCuts[Cuts++] = XParam.BoradLeds[i];
		}
	}

	TabHead.YEmitVer = ComboBoxEmitYVer->ItemIndex;
	TabHead.YRevVer = ComboBoxRevYVer->ItemIndex;
	TabHead.YBoardLed = _wtoi(ComboBoxYBoardLeds->Text.c_str()); ;
	TabHead.YPowerGroup = YParam.MiniOff;
	TabHead.YJumpLeds = YParam.JumpLeds;
	TabHead.YScanGap = YParam.ScanGap;
	TabHead.YPow = YParam.EmitPower;
	TabHead.YMaxDeg = (uint16_t)(_wtof(EditYoffset->Text.c_str()) * 100);
	TabHead.YLedSpace = (uint16_t)(YParam.Dis * 1000);
	TabHead.YReScan = 0;
	TabHead.YDivMethod = ComboBoxYDivMethod->ItemIndex;
	TabHead.YCutUnUsed = CheckBox19->Checked ? 0xAA : 0;

	TabHead.GPIOALen = 0;
	TabHead.GPIOBLen = 0;
	TabHead.GPIOCLen = 0;
	TabHead.GPIODLen = 0;

	memset(TabHead.YCuts, 0, sizeof(TabHead.YCuts));
	for (int i = 0, Cuts = 0; i < YParam.Boards && Cuts < sizeof(TabHead.YCuts);
	i += 1) {
		if (YParam.BoradLeds[i] != TabHead.YBoardLed) {
			TabHead.YCuts[Cuts++] = i;
			TabHead.YCuts[Cuts++] = YParam.BoradLeds[i];
		}
	}

	memset(TabHead.BoardLed, 0, sizeof(TabHead.BoardLed));
	if (YParam.Boards + XParam.Boards < sizeof(TabHead.BoardLed)) {
		for (int i = 0; i < YParam.Boards; i += 1) {
			TabHead.BoardLed[i] = YParam.BoradLeds[i];
		}

		for (int i = 0; i < XParam.Boards; i += 1) {
			TabHead.BoardLed[i + YParam.Boards] = XParam.BoradLeds[i];
		}
	}
	else {
		ShowMessage(L"板子数量过多!");
		return false;
	}

	Offset = ALIGN_DWORD(sizeof(SCAN_TABLE_HEAD)); // align to Dwords

	TabHead.XOffsets = (const int16_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 2 * XAxis);
	TabHead.YOffsets = (const int16_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 2 * YAxis);
	TabHead.Offsets = (const int16_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 2 * AllAxis);

	TabHead.XTan = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * XAxis);
	TabHead.YTan = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * YAxis);
	TabHead.Tan = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * AllAxis);

	TabHead.XCoTan = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * XAxis);
	TabHead.YCoTan = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * YAxis);
	TabHead.CoTan = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * AllAxis);

	TabHead.XOffLen = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * XAxis);
	TabHead.YOffLen = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * YAxis);
	TabHead.OffLen = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * AllAxis);

	// ----------------- aux parameters ----------------------
	TabHead.AuxXOffsets = (const int16_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 2 * XAxis);
	TabHead.AuxYOffsets = (const int16_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 2 * YAxis);
	TabHead.AuxOffsets = (const int16_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 2 * AllAxis);

	TabHead.AuxXTan = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * XAxis);
	TabHead.AuxYTan = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * YAxis);
	TabHead.AuxTan = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * AllAxis);

	TabHead.AuxXCoTan = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * XAxis);
	TabHead.AuxYCoTan = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * YAxis);
	TabHead.AuxCoTan = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * AllAxis);

	TabHead.AuxXOffLen = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * XAxis);
	TabHead.AuxYOffLen = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * YAxis);
	TabHead.AuxOffLen = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * AllAxis);

	// ----------------- Mix Scan parameters ----------------------
	TabHead.Maigc1 = 0x13579ABC;

	TabHead.MixXAxis = MixXAxis;
	TabHead.MixYAxis = MixYAxis;
	TabHead.MixAllAxis = MixAllAxis;

	TabHead.MixXLedOff = (const int16_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 2 * MixXAxis);
	TabHead.MixYLedOff = (const int16_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 2 * MixYAxis);
	TabHead.MixLedOff = (const int16_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 2 * MixAllAxis);

	TabHead.MixXTan = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * MixXAxis);
	TabHead.MixYTan = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * MixYAxis);
	TabHead.MixTan = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * MixAllAxis);

	TabHead.MixXCos = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * MixXAxis);
	TabHead.MixYCos = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * MixYAxis);
	TabHead.MixCos = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * MixAllAxis);

	TabHead.MixXAxisId = (const uint8_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + MixXAxis);
	TabHead.MixYAxisId = (const uint8_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + MixYAxis);
	TabHead.MixAxisId = (const uint8_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + MixAllAxis);

	TabHead.MixXStartLed = (const uint16_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 2 * MixXAxis);
	TabHead.MixYStartLed = (const uint16_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 2 * MixYAxis);
	TabHead.MixStartLed = (const uint16_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 2 * MixAllAxis);

	TabHead.MixXEndLed = (const uint16_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 2 * MixXAxis);
	TabHead.MixYEndLed = (const uint16_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 2 * MixYAxis);
	TabHead.MixEndLed = (const uint16_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 2 * MixAllAxis);

	// ----------------- Soft Scan parameters ----------------------
	TabHead.MaxXOff = XMaxOff;
	TabHead.MaxYOff = YMaxOff;

	TabHead.ScXTan = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * (XMaxOff + 1));
	TabHead.ScXCoTan = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * (XMaxOff + 1));
	TabHead.ScXSin = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * (XMaxOff + 1));
	TabHead.ScXCos = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * (XMaxOff + 1));

	TabHead.ScYTan = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * (YMaxOff + 1));
	TabHead.ScYCoTan = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * (YMaxOff + 1));
	TabHead.ScYSin = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * (YMaxOff + 1));
	TabHead.ScYCos = (const float *)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + 4 * (YMaxOff + 1));

	TabHead.HIDToolsVer = 3010;
	// ---------------------------------------
	ANTI_AXIS = _wtoi(ComboBoxGlareLever->Text.t_str());
	TabHead.GlareLever = ANTI_AXIS;

	if (CheckBoxAntiGlare->Checked) {
		SkipAxis = ANTI_AXIS;
	}
	else if (CheckBoxSkipFirstAxis->Checked) {
		SkipAxis = 1;
	}
	else {
		SkipAxis = 0;
	}

	if (CheckBoxSwitchEmit->Checked) // switch Emit
	{
		TabHead.RevTabLen = TabHead.XLeds + TabHead.YLeds;
		TabHead.EmitTabLen = TabHead.XLeds * (XAxis + SkipAxis) +
			TabHead.YLeds * (YAxis + SkipAxis);

		if (CheckBoxShutDownEmit->Checked) {
			TabHead.RevTabLen += 1;
			TabHead.EmitTabLen += (YAxis + SkipAxis);
		}

		if (CheckBoxDoubleFirst->Checked == true) {
			TabHead.RevTabLen += (TabHead.XBoards + TabHead.YBoards);
			TabHead.EmitTabLen +=
				((XAxis + SkipAxis) * TabHead.XBoards + (YAxis + SkipAxis)
				* TabHead.YBoards);
		}

		TabHead.GainTabLen = TabHead.EmitTabLen;

		TabHead.ReorderAdMapLen = TabHead.EmitTabLen * 2;
	}
	else if (CheckBoxSwEmitRev->Checked) // switch emit rev simultaneously
	{
		TabHead.EmitTabLen = TabHead.XLeds * (XAxis + SkipAxis) +
			TabHead.YLeds * (YAxis + SkipAxis);

		if (CheckBoxShutDownEmit->Checked) {
			TabHead.EmitTabLen += (YAxis + SkipAxis);
		}

		if (CheckBoxDoubleFirst->Checked == true) {
			TabHead.EmitTabLen +=
				((XAxis + SkipAxis) * TabHead.XBoards + (YAxis + SkipAxis)
				* TabHead.YBoards);
		}

		if (CheckBoxGPIOSwitch->Checked) {
			TabHead.GainTabLen = 0;
			TabHead.RevTabLen = TabHead.EmitTabLen;
		}
		else {
			TabHead.GainTabLen = TabHead.EmitTabLen;
			TabHead.RevTabLen = 0;
		}

		TabHead.ReorderAdMapLen = TabHead.EmitTabLen * 2;
	}
	else {
		TabHead.EmitTabLen = TabHead.XLeds + TabHead.YLeds;
		TabHead.RevTabLen = TabHead.XLeds * (XAxis + SkipAxis) + TabHead.YLeds *
			(YAxis + SkipAxis);

		if (CheckBoxShutDownEmit->Checked) {
			TabHead.EmitTabLen += 1;
			TabHead.RevTabLen += (YAxis + SkipAxis);
		}

		TabHead.GainTabLen = TabHead.RevTabLen;

		TabHead.ReorderAdMapLen = TabHead.RevTabLen * 2;
	}

	if (CheckBoxGPIOSwitch->Checked) {
		TabHead.RevTabLen *= 4;
		TabHead.EmitTabLen *= 4;
	}
	else if (!CheckBox8BitSPI->Checked) { // 16bits data?
		TabHead.RevTabLen *= 2;
		TabHead.EmitTabLen *= 2;
	}

	// jump invaid leds ---------------------------------------------
	TabHead.JumpLedsTableLen = 0;
	TabHead.xJumps = 0;
	TabHead.yJumps = 0;
	TabHead.V15EmitGroupTableLen = 0;

	bool WaitRxPowerOn = CheckBoxWaitRxPowerOn->Checked;

	if (XParam.Get_Rev_Data == NULL || YParam.Get_Rev_Data == NULL || \
 XParam.Get_Emit_Data == Get_Emit_Data_PTV150) {
		for (int i = 0; i < YParam.Boards; i += 1) {
			if (WaitRxPowerOn) {
				TabHead.JumpLedsTableLen += (YParam.BoradLeds[i] + 7) / 8 * 2;
				TabHead.yJumps += (YParam.BoradLeds[i] + 7) / 8;
			}
			if ((YParam.BoradLeds[i] & 0x07) != 0) {
				TabHead.JumpLedsTableLen +=
					(8 - (YParam.BoradLeds[i] & 0x07)) * 2;
				TabHead.yJumps += (8 - (YParam.BoradLeds[i] & 0x07));
			}
		}

		if (CheckBoxSkipLastY->Checked) {
			TabHead.JumpLedsTableLen += 2;
			TabHead.yJumps += 1;
		}

		for (int i = 0; i < XParam.Boards - 1; i += 1) {
			if (WaitRxPowerOn) {
				TabHead.JumpLedsTableLen += (XParam.BoradLeds[i] + 7) / 8 * 2;
				TabHead.xJumps += (XParam.BoradLeds[i] + 7) / 8;
			}

			if ((XParam.BoradLeds[i] & 0x07) != 0) {
				TabHead.JumpLedsTableLen +=
					(8 - (XParam.BoradLeds[i] & 0x07)) * 2;
				TabHead.xJumps += (8 - (XParam.BoradLeds[i] & 0x07));
			}
		}

		if (WaitRxPowerOn) {
			TabHead.JumpLedsTableLen +=
				(XParam.BoradLeds[XParam.Boards - 1] + 7) / 8 * 2;
			TabHead.xJumps += (XParam.BoradLeds[XParam.Boards - 1] + 7) / 8;
		}

		if (CheckBoxSkipX->Checked == true) // we need jump leds @ the end of x
		{
			if ((XParam.BoradLeds[XParam.Boards - 1] & 0x07) != 0) {
				TabHead.JumpLedsTableLen +=
					(8 - (XParam.BoradLeds[XParam.Boards - 1] & 0x07)) * 2;
				TabHead.xJumps +=
					(8 - (XParam.BoradLeds[XParam.Boards - 1] & 0x07));
			}

			if (XParam.Get_Emit_Data == Get_Emit_Data_PDV1334 ||
				XParam.Get_Emit_Data == Get_Emit_Data_PDV1335) {
				if ((XParam.BoradLeds[XParam.Boards - 1] & 0x07) == 0) {
					TabHead.JumpLedsTableLen += 8 * 2;
					TabHead.xJumps += 8;

					if (WaitRxPowerOn) {
						TabHead.JumpLedsTableLen += 2;
						TabHead.xJumps += 1;
					}
				}
			}
		}

		TabHead.EmitTabLen = (TabHead.XLeds + TabHead.xJumps) *
			(XAxis + SkipAxis) + (TabHead.YLeds + TabHead.yJumps) *
			(YAxis + SkipAxis);

		TabHead.RevTabLen = TabHead.EmitTabLen * 2;

		TabHead.ReorderAdMapLen = TabHead.EmitTabLen * 2;
	}

	if (XParam.Get_Emit_Data == Get_Emit_Data_PTV150) {
		TabHead.V15EmitGroupTableLen = TabHead.EmitTabLen * 6; // 6byte spi
		TabHead.EmitTabLen = TabHead.EmitTabLen * 4; // emit 138 gpio
		TabHead.RevTabLen = 8 * (XAxis + SkipAxis) * 4;
	}
	// else if(XParam.Get_Emit_Data == Get_EmitX_Data_FCT){
	// TabHead.GPIOALen = TabHead.RevTabLen;
	// }

	else if (XParam.Get_Emit_Data == Get_Emit_Data_PDV133) {
		if (WaitRxPowerOn) {
			TabHead.RevTabLen = (XAxis + SkipAxis) * 9 * 4; // for hc4051 abc
		}
		else {
			TabHead.RevTabLen = (XAxis + SkipAxis) * 8 * 4; // for hc4051 abc
		}

		TabHead.GPIOBLen = TabHead.GPIOCLen = TabHead.EmitTabLen * 4;
		// for emit switch
		TabHead.EmitTabLen *= 2;
	}
	else if (XParam.Get_Emit_Data == Get_Emit_Data_PDV1331 ||
		XParam.Get_Emit_Data == Get_Emit_Data_PDV1333 || \
 XParam.Get_Emit_Data == Get_Emit_Data_PDV1334 ||
		XParam.Get_Emit_Data == Get_Emit_Data_PDV1335) {
		if (WaitRxPowerOn) {
			TabHead.RevTabLen = (XAxis + SkipAxis) * 9 * 4; // for hc4051 abc
		}
		else {
			TabHead.RevTabLen = (XAxis + SkipAxis) * 8 * 4;
			// for hc4051 abc     axis * channels
		}

		if (XParam.Get_Emit_Data == Get_Emit_Data_PDV1333) {
			TabHead.GPIOBLen = TabHead.EmitTabLen * 4;
		}
		else {
			TabHead.GPIOBLen = 0;
		}
		TabHead.GPIOCLen = TabHead.EmitTabLen * 4; //
		TabHead.EmitTabLen *= 2;

		if (XParam.Axis > 9 && YParam.Axis > 9)
		{ // main axi only for dynamic scan
			TabHead.EmitTabLen = TabHead.XLeds + TabHead.YLeds;
			TabHead.RevTabLen = 0;
			TabHead.ReorderAdMapLen = 0;

			TabHead.GPIOCLen = TabHead.EmitTabLen * 4; //
			TabHead.EmitTabLen *= 2;
		}
	}

	if (!CheckBoxGPIOGain->Checked) {
		TabHead.GainTabLen = 0;
	}

	if (!CheckBoxReorderMap->Checked) {
		TabHead.ReorderAdMapLen = 0;
	}

	TabHead.GainTabLen *= 4;

	TabHead.RevTab = (const uint16_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + TabHead.RevTabLen);

	TabHead.EmitTab = (const uint16_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + TabHead.EmitTabLen);

	TabHead.GainTab = (const uint32_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + TabHead.GainTabLen);

	TabHead.XGainSet = (const uint8_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + XAxis);

	TabHead.YGainSet = (const uint8_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + YAxis);

	TabHead.JumpLedsTable = (const uint16_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + TabHead.JumpLedsTableLen);

	TabHead.ReorderAdMap = (const uint16_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + TabHead.ReorderAdMapLen);

	TabHead.V15EmitGroupTable = (const uint16_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + TabHead.V15EmitGroupTableLen);

	TabHead.GPIOA = (const uint32_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + TabHead.GPIOALen);

	TabHead.GPIOB = (const uint32_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + TabHead.GPIOBLen);

	TabHead.GPIOC = (const uint32_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + TabHead.GPIOCLen);

	TabHead.GPIOD = (const uint32_t*)(TabBaseAdd + Offset);
	Offset = ALIGN_DWORD(Offset + TabHead.GPIODLen);

	TableLen = Offset;

	if (Offset > fp->ScanTabSize - 4) {
		ShowMessage(L"扫描表太大(V2.3Boot>16KB, V2.5Boot>32KB)，无法写入!");
		return false;
	}

	// memset(Table, 0xFF, sizeof(Table));
	for (int i = 0; i < sizeof(Table) / 2; i += 1) {
		((uint16_t*)Table)[i] = rand();
	}

	memcpy(Table, &TabHead, sizeof(SCAN_TABLE_HEAD));

	if (Gen_DMATable_c(Flag, &TabHead, TabBaseAdd, Table)) {
		ShowMessage(L"扫描表生成失败！");
		return false;
	}

	((uint32_t*)(Table + fp->ScanTabSize - 4))[0] =
		FormUpdate->CRC32((uint32_t*)Table, fp->ScanTabSize / 4 - 1);

	return true;
#else
	return false;
#endif
}

void __fastcall TFormConfig::ButtonDownTabClick(TObject *Sender) {
	FLASH_PARAM *fp = &FormUpdate->FlashParam;

	if (GenScanTable() == false)
		return;

	if (MainForm->HIDSendThread->Connected == FALSE) {
		ShowMessage(L"设备未连接!");
		return;
	}

	Head.MemType = TYPE_TABLE;
	Head.Mode = MODE_PROGRAM;
	Head.StartAdd = fp->ScanTabStartAdd;

	if (MainForm->HIDSendThread->SubmitTask(TYPE_APP, fp->ScanTabSize, Table,
		&Head)) {
		DisableButtons();
		TimerDownTab->Enabled = true;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormConfig::FormShow(TObject *Sender) {
	static bool initial = false;

	if (initial == true)
		return;

	initial = true;

	// gain
	StringGrid1->ColCount = MAX_BOARDS + 1;
	StringGrid1->RowCount = 17;

	StringGrid1->Cells[0][0] = "板地址";
	StringGrid1->Cells[0][1] = "0轴增益1";
	StringGrid1->Cells[0][2] = "0轴增益2";
	StringGrid1->Cells[0][3] = "发射功率";
	StringGrid1->Cells[0][4] = "0轴信号均值";
	StringGrid1->Cells[0][5] = "1轴增益";
	StringGrid1->Cells[0][6] = "2轴增益";
	StringGrid1->Cells[0][7] = "3轴增益";
	StringGrid1->Cells[0][8] = "4轴增益";
	StringGrid1->Cells[0][9] = "5轴增益";
	StringGrid1->Cells[0][10] = "6轴增益";
	StringGrid1->Cells[0][11] = "7轴增益";
	StringGrid1->Cells[0][12] = "8轴增益";
	StringGrid1->Cells[0][13] = "9轴增益";
	StringGrid1->Cells[0][14] = "10轴增益";
	StringGrid1->Cells[0][15] = "11轴增益";
	StringGrid1->Cells[0][16] = "12轴增益";

	StringGrid1->ColWidths[0] = 100;

	for (int i = 1; i < StringGrid1->ColCount; i += 1) {
		StringGrid1->ColWidths[i] = 40;
		StringGrid1->Cells[i][0] = String(i);
	}

	StringGrid1->Options << goEditing;

	XParamEdit = new TList;
	YParamEdit = new TList;
	FeatureCheckBox = new TList;
	FeatureCheckBoxUser = new TList;
	ScanFeatureCheckBox = new TList;
	XCutBoards = new TList;
	YCutBoards = new TList;

	XParamEdit->Add(Edit2);
	XParamEdit->Add(Edit3);
	XParamEdit->Add(Edit4);
	XParamEdit->Add(Edit5);
	XParamEdit->Add(Edit6);
	XParamEdit->Add(Edit7);
	XParamEdit->Add(Edit8);
	XParamEdit->Add(Edit9);
	XParamEdit->Add(Edit10);
	XParamEdit->Add(Edit11);
	XParamEdit->Add(Edit12);
	XParamEdit->Add(Edit95);
	XParamEdit->Add(Edit96);
	XParamEdit->Add(Edit97);
	XParamEdit->Add(Edit98);
	XParamEdit->Add(Edit99);
	XParamEdit->Add(Edit100);
	XParamEdit->Add(Edit101);
	XParamEdit->Add(Edit102);
	XParamEdit->Add(Edit103);
	XParamEdit->Add(Edit104);
	XParamEdit->Add(Edit105);
	XParamEdit->Add(Edit116);
	XParamEdit->Add(Edit127);
	XParamEdit->Add(Edit138);
	XParamEdit->Add(Edit139);
	XParamEdit->Add(Edit140);
	XParamEdit->Add(Edit141);
	XParamEdit->Add(Edit142);
	XParamEdit->Add(Edit143);
	XParamEdit->Add(Edit144);

	XParamEdit->Add(Edit13);
	XParamEdit->Add(Edit14);
	XParamEdit->Add(Edit15);
	XParamEdit->Add(Edit16);
	XParamEdit->Add(Edit17);
	XParamEdit->Add(Edit18);
	XParamEdit->Add(Edit19);
	XParamEdit->Add(Edit20);
	XParamEdit->Add(Edit21);
	XParamEdit->Add(Edit22);
	XParamEdit->Add(Edit23);
	XParamEdit->Add(Edit106);
	XParamEdit->Add(Edit107);
	XParamEdit->Add(Edit108);
	XParamEdit->Add(Edit109);
	XParamEdit->Add(Edit110);
	XParamEdit->Add(Edit111);
	XParamEdit->Add(Edit112);
	XParamEdit->Add(Edit113);
	XParamEdit->Add(Edit114);
	XParamEdit->Add(Edit115);
	XParamEdit->Add(Edit145);
	XParamEdit->Add(Edit146);
	XParamEdit->Add(Edit147);
	XParamEdit->Add(Edit148);
	XParamEdit->Add(Edit149);
	XParamEdit->Add(Edit150);
	XParamEdit->Add(Edit151);
	XParamEdit->Add(Edit152);
	XParamEdit->Add(Edit153);
	XParamEdit->Add(Edit154);

	XParamEdit->Add(Edit24);
	XParamEdit->Add(Edit25);
	XParamEdit->Add(Edit26);
	XParamEdit->Add(Edit27);
	XParamEdit->Add(Edit28);
	XParamEdit->Add(Edit29);
	XParamEdit->Add(Edit30);
	XParamEdit->Add(Edit31);
	XParamEdit->Add(Edit32);
	XParamEdit->Add(Edit33);
	XParamEdit->Add(Edit34);
	XParamEdit->Add(Edit117);
	XParamEdit->Add(Edit118);
	XParamEdit->Add(Edit119);
	XParamEdit->Add(Edit120);
	XParamEdit->Add(Edit121);
	XParamEdit->Add(Edit122);
	XParamEdit->Add(Edit123);
	XParamEdit->Add(Edit124);
	XParamEdit->Add(Edit125);
	XParamEdit->Add(Edit126);
	XParamEdit->Add(Edit155);
	XParamEdit->Add(Edit156);
	XParamEdit->Add(Edit157);
	XParamEdit->Add(Edit158);
	XParamEdit->Add(Edit159);
	XParamEdit->Add(Edit160);
	XParamEdit->Add(Edit161);
	XParamEdit->Add(Edit162);
	XParamEdit->Add(Edit163);
	XParamEdit->Add(Edit164);

	XParamEdit->Add(ComboBox1);
	XParamEdit->Add(ComboBox2);
	XParamEdit->Add(ComboBox3);
	XParamEdit->Add(ComboBox4);
	XParamEdit->Add(ComboBox5);
	XParamEdit->Add(ComboBox6);
	XParamEdit->Add(ComboBox7);
	XParamEdit->Add(ComboBox8);
	XParamEdit->Add(ComboBox9);
	XParamEdit->Add(ComboBox10);
	XParamEdit->Add(ComboBox11);
	XParamEdit->Add(ComboBox45);
	XParamEdit->Add(ComboBox46);
	XParamEdit->Add(ComboBox47);
	XParamEdit->Add(ComboBox48);
	XParamEdit->Add(ComboBox49);
	XParamEdit->Add(ComboBox50);
	XParamEdit->Add(ComboBox51);
	XParamEdit->Add(ComboBox52);
	XParamEdit->Add(ComboBox53);
	XParamEdit->Add(ComboBox54);
	XParamEdit->Add(ComboBox55);
	XParamEdit->Add(ComboBox66);
	XParamEdit->Add(ComboBox67);
	XParamEdit->Add(ComboBox68);
	XParamEdit->Add(ComboBox69);
	XParamEdit->Add(ComboBox70);
	XParamEdit->Add(ComboBox71);
	XParamEdit->Add(ComboBox72);
	XParamEdit->Add(ComboBox73);
	XParamEdit->Add(ComboBox74);

	XParamEdit->Add(ComboBox12);
	XParamEdit->Add(ComboBox13);
	XParamEdit->Add(ComboBox14);
	XParamEdit->Add(ComboBox15);
	XParamEdit->Add(ComboBox16);
	XParamEdit->Add(ComboBox17);
	XParamEdit->Add(ComboBox18);
	XParamEdit->Add(ComboBox19);
	XParamEdit->Add(ComboBox20);
	XParamEdit->Add(ComboBox21);
	XParamEdit->Add(ComboBox22);
	XParamEdit->Add(ComboBox56);
	XParamEdit->Add(ComboBox57);
	XParamEdit->Add(ComboBox58);
	XParamEdit->Add(ComboBox59);
	XParamEdit->Add(ComboBox60);
	XParamEdit->Add(ComboBox61);
	XParamEdit->Add(ComboBox62);
	XParamEdit->Add(ComboBox63);
	XParamEdit->Add(ComboBox64);
	XParamEdit->Add(ComboBox65);
	XParamEdit->Add(ComboBox75);
	XParamEdit->Add(ComboBox76);
	XParamEdit->Add(ComboBox77);
	XParamEdit->Add(ComboBox78);
	XParamEdit->Add(ComboBox79);
	XParamEdit->Add(ComboBox80);
	XParamEdit->Add(ComboBox81);
	XParamEdit->Add(ComboBox82);
	XParamEdit->Add(ComboBox83);
	XParamEdit->Add(ComboBox84);

	XParamEdit->Add(Edit1);
	XParamEdit->Add(Edit35);
	XParamEdit->Add(Edit69);
	XParamEdit->Add(Edit70);
	XParamEdit->Add(Edit71);
	XParamEdit->Add(Edit72);
	XParamEdit->Add(Edit73);
	XParamEdit->Add(Edit74);
	XParamEdit->Add(Edit77);
	XParamEdit->Add(Edit78);
	XParamEdit->Add(Edit79);
	XParamEdit->Add(Edit128);
	XParamEdit->Add(Edit129);
	XParamEdit->Add(Edit130);
	XParamEdit->Add(Edit131);
	XParamEdit->Add(Edit132);
	XParamEdit->Add(Edit133);
	XParamEdit->Add(Edit134);
	XParamEdit->Add(Edit135);
	XParamEdit->Add(Edit136);
	XParamEdit->Add(Edit137);
	XParamEdit->Add(Edit165);
	XParamEdit->Add(Edit166);
	XParamEdit->Add(Edit167);
	XParamEdit->Add(Edit168);
	XParamEdit->Add(Edit169);
	XParamEdit->Add(Edit170);
	XParamEdit->Add(Edit171);
	XParamEdit->Add(Edit172);
	XParamEdit->Add(Edit173);
	XParamEdit->Add(Edit174);

	//
	YParamEdit->Add(Edit36);
	YParamEdit->Add(Edit37);
	YParamEdit->Add(Edit38);
	YParamEdit->Add(Edit39);
	YParamEdit->Add(Edit40);
	YParamEdit->Add(Edit41);
	YParamEdit->Add(Edit42);
	YParamEdit->Add(Edit43);
	YParamEdit->Add(Edit44);
	YParamEdit->Add(Edit45);
	YParamEdit->Add(Edit46);
	YParamEdit->Add(Edit175);
	YParamEdit->Add(Edit176);
	YParamEdit->Add(Edit177);
	YParamEdit->Add(Edit178);
	YParamEdit->Add(Edit179);
	YParamEdit->Add(Edit180);
	YParamEdit->Add(Edit181);
	YParamEdit->Add(Edit182);
	YParamEdit->Add(Edit183);
	YParamEdit->Add(Edit184);
	YParamEdit->Add(Edit215);
	YParamEdit->Add(Edit216);
	YParamEdit->Add(Edit217);
	YParamEdit->Add(Edit218);
	YParamEdit->Add(Edit219);
	YParamEdit->Add(Edit220);
	YParamEdit->Add(Edit221);
	YParamEdit->Add(Edit222);
	YParamEdit->Add(Edit223);
	YParamEdit->Add(Edit224);

	YParamEdit->Add(Edit47);
	YParamEdit->Add(Edit48);
	YParamEdit->Add(Edit49);
	YParamEdit->Add(Edit50);
	YParamEdit->Add(Edit51);
	YParamEdit->Add(Edit52);
	YParamEdit->Add(Edit53);
	YParamEdit->Add(Edit54);
	YParamEdit->Add(Edit55);
	YParamEdit->Add(Edit56);
	YParamEdit->Add(Edit57);
	YParamEdit->Add(Edit185);
	YParamEdit->Add(Edit186);
	YParamEdit->Add(Edit187);
	YParamEdit->Add(Edit188);
	YParamEdit->Add(Edit189);
	YParamEdit->Add(Edit190);
	YParamEdit->Add(Edit191);
	YParamEdit->Add(Edit192);
	YParamEdit->Add(Edit193);
	YParamEdit->Add(Edit194);
	YParamEdit->Add(Edit225);
	YParamEdit->Add(Edit226);
	YParamEdit->Add(Edit227);
	YParamEdit->Add(Edit228);
	YParamEdit->Add(Edit229);
	YParamEdit->Add(Edit230);
	YParamEdit->Add(Edit231);
	YParamEdit->Add(Edit232);
	YParamEdit->Add(Edit233);
	YParamEdit->Add(Edit234);

	YParamEdit->Add(Edit58);
	YParamEdit->Add(Edit59);
	YParamEdit->Add(Edit60);
	YParamEdit->Add(Edit61);
	YParamEdit->Add(Edit62);
	YParamEdit->Add(Edit63);
	YParamEdit->Add(Edit64);
	YParamEdit->Add(Edit65);
	YParamEdit->Add(Edit66);
	YParamEdit->Add(Edit67);
	YParamEdit->Add(Edit68);
	YParamEdit->Add(Edit195);
	YParamEdit->Add(Edit196);
	YParamEdit->Add(Edit197);
	YParamEdit->Add(Edit198);
	YParamEdit->Add(Edit199);
	YParamEdit->Add(Edit200);
	YParamEdit->Add(Edit201);
	YParamEdit->Add(Edit202);
	YParamEdit->Add(Edit203);
	YParamEdit->Add(Edit204);
	YParamEdit->Add(Edit235);
	YParamEdit->Add(Edit236);
	YParamEdit->Add(Edit237);
	YParamEdit->Add(Edit238);
	YParamEdit->Add(Edit239);
	YParamEdit->Add(Edit240);
	YParamEdit->Add(Edit241);
	YParamEdit->Add(Edit242);
	YParamEdit->Add(Edit243);
	YParamEdit->Add(Edit244);

	YParamEdit->Add(ComboBox23);
	YParamEdit->Add(ComboBox24);
	YParamEdit->Add(ComboBox25);
	YParamEdit->Add(ComboBox26);
	YParamEdit->Add(ComboBox27);
	YParamEdit->Add(ComboBox28);
	YParamEdit->Add(ComboBox29);
	YParamEdit->Add(ComboBox30);
	YParamEdit->Add(ComboBox31);
	YParamEdit->Add(ComboBox32);
	YParamEdit->Add(ComboBox33);
	YParamEdit->Add(ComboBox85);
	YParamEdit->Add(ComboBox86);
	YParamEdit->Add(ComboBox87);
	YParamEdit->Add(ComboBox88);
	YParamEdit->Add(ComboBox89);
	YParamEdit->Add(ComboBox90);
	YParamEdit->Add(ComboBox91);
	YParamEdit->Add(ComboBox92);
	YParamEdit->Add(ComboBox93);
	YParamEdit->Add(ComboBox94);
	YParamEdit->Add(ComboBox105);
	YParamEdit->Add(ComboBox106);
	YParamEdit->Add(ComboBox107);
	YParamEdit->Add(ComboBox108);
	YParamEdit->Add(ComboBox109);
	YParamEdit->Add(ComboBox110);
	YParamEdit->Add(ComboBox111);
	YParamEdit->Add(ComboBox112);
	YParamEdit->Add(ComboBox113);
	YParamEdit->Add(ComboBox114);

	YParamEdit->Add(ComboBox34);
	YParamEdit->Add(ComboBox35);
	YParamEdit->Add(ComboBox36);
	YParamEdit->Add(ComboBox37);
	YParamEdit->Add(ComboBox38);
	YParamEdit->Add(ComboBox39);
	YParamEdit->Add(ComboBox40);
	YParamEdit->Add(ComboBox41);
	YParamEdit->Add(ComboBox42);
	YParamEdit->Add(ComboBox43);
	YParamEdit->Add(ComboBox44);
	YParamEdit->Add(ComboBox95);
	YParamEdit->Add(ComboBox96);
	YParamEdit->Add(ComboBox97);
	YParamEdit->Add(ComboBox98);
	YParamEdit->Add(ComboBox99);
	YParamEdit->Add(ComboBox100);
	YParamEdit->Add(ComboBox101);
	YParamEdit->Add(ComboBox102);
	YParamEdit->Add(ComboBox103);
	YParamEdit->Add(ComboBox104);
	YParamEdit->Add(ComboBox115);
	YParamEdit->Add(ComboBox116);
	YParamEdit->Add(ComboBox117);
	YParamEdit->Add(ComboBox118);
	YParamEdit->Add(ComboBox119);
	YParamEdit->Add(ComboBox120);
	YParamEdit->Add(ComboBox121);
	YParamEdit->Add(ComboBox122);
	YParamEdit->Add(ComboBox123);
	YParamEdit->Add(ComboBox124);

	YParamEdit->Add(Edit80);
	YParamEdit->Add(Edit81);
	YParamEdit->Add(Edit82);
	YParamEdit->Add(Edit83);
	YParamEdit->Add(Edit84);
	YParamEdit->Add(Edit85);
	YParamEdit->Add(Edit86);
	YParamEdit->Add(Edit87);
	YParamEdit->Add(Edit88);
	YParamEdit->Add(Edit89);
	YParamEdit->Add(Edit90);
	YParamEdit->Add(Edit205);
	YParamEdit->Add(Edit206);
	YParamEdit->Add(Edit207);
	YParamEdit->Add(Edit208);
	YParamEdit->Add(Edit209);
	YParamEdit->Add(Edit210);
	YParamEdit->Add(Edit211);
	YParamEdit->Add(Edit212);
	YParamEdit->Add(Edit213);
	YParamEdit->Add(Edit214);
	YParamEdit->Add(Edit245);
	YParamEdit->Add(Edit246);
	YParamEdit->Add(Edit247);
	YParamEdit->Add(Edit248);
	YParamEdit->Add(Edit249);
	YParamEdit->Add(Edit250);
	YParamEdit->Add(Edit251);
	YParamEdit->Add(Edit252);
	YParamEdit->Add(Edit253);
	YParamEdit->Add(Edit254);

	FeatureCheckBox->Add(CheckBoxFeatureSmooth);
	FeatureCheckBox->Add(CheckBoxFeatureInterplation);
	FeatureCheckBox->Add(CheckBoxFeatureMac);
	FeatureCheckBox->Add(CheckBoxFeatureReverseX);
	FeatureCheckBox->Add(CheckBoxFeatureReverseY);
	FeatureCheckBox->Add(CheckBoxFeatureExchangeXY);
	FeatureCheckBox->Add(CheckBoxFeatureUSARTOn);
	FeatureCheckBox->Add(CheckBoxFeatureSmallPointOn);
	FeatureCheckBox->Add(CheckBoxFeatureFourceMouse);

	// FeatureCheckBox->Add(CheckBoxFeatureLinkSeeProtocol);
	// FeatureCheckBox->Add(CheckBoxFeatureSvaGesture);
	FeatureCheckBox->Add(CheckBoxFeatureRev);
	FeatureCheckBox->Add(CheckBoxFeatureRev);
	FeatureCheckBox->Add(CheckBoxFeatureRev);
	FeatureCheckBox->Add(CheckBoxFeatureRev);

	FeatureCheckBox->Add(CheckBoxFeatureBoardPoint);
	FeatureCheckBox->Add(CheckBoxFeatureGainAdj);
	FeatureCheckBox->Add(CheckBoxFeatureEdgeAdsorption);
	FeatureCheckBox->Add(CheckBoxFeatureGPIOGesture);
	FeatureCheckBox->Add(CheckBoxFeatureDisableRightClick); // 18
	FeatureCheckBox->Add(CheckBoxFeatureRev); // 19
	FeatureCheckBox->Add(CheckBoxFeatureClickup); // 20
	FeatureCheckBox->Add(CheckBoxFeatureUnclickMove); // 21
	FeatureCheckBox->Add(CheckBoxD2CurHigh); // 22
	FeatureCheckBox->Add(CheckBoxFeatureAutoGainSub); // 23
	FeatureCheckBox->Add(CheckBoxFeatureAutoGainUniform); // 24
	FeatureCheckBox->Add(CheckBoxWorkTime); // 25

	FeatureCheckBoxUser->Add(CheckBox1); // smooth
	FeatureCheckBoxUser->Add(CheckBox1); // Interplation
	FeatureCheckBoxUser->Add(CheckBoxFeatureMac); // 不使用
	FeatureCheckBoxUser->Add(CheckBox2); // ReverseX
	FeatureCheckBoxUser->Add(CheckBox3); // ReverseY
	FeatureCheckBoxUser->Add(CheckBox4); // ExchangeXY
	FeatureCheckBoxUser->Add(CheckBox5); // USARTOn
	FeatureCheckBoxUser->Add(CheckBox6); // SmallPointOn
	FeatureCheckBoxUser->Add(CheckBox7); // FourceMouse
	FeatureCheckBoxUser->Add(CheckBox8); // LinkSeeProtocol
	FeatureCheckBoxUser->Add(CheckBox9); // SvaGesture
	FeatureCheckBoxUser->Add(CheckBox10); // Rev
	FeatureCheckBoxUser->Add(CheckBox10); // Rev
	FeatureCheckBoxUser->Add(CheckBox11); // BoardPoint
	FeatureCheckBoxUser->Add(CheckBox12); // GainAdj
	FeatureCheckBoxUser->Add(CheckBox13); // EdgeAdsorption
	FeatureCheckBoxUser->Add(CheckBox14); // GPIOGesture
	FeatureCheckBoxUser->Add(CheckBox16); // DisableRightClick
	FeatureCheckBoxUser->Add(CheckBox16); // DisableRightClick
	FeatureCheckBoxUser->Add(CheckBox15); // Clickup
	FeatureCheckBoxUser->Add(CheckBox18); // UnclickMove

	ScanFeatureCheckBox->Add(CheckBoxSkipFirstAxis); // 0
	ScanFeatureCheckBox->Add(CheckBoxSwitchEmit); // 1
	ScanFeatureCheckBox->Add(CheckBoxDualSPIData); // 2
	ScanFeatureCheckBox->Add(CheckBoxShutDownEmit); // 3
	ScanFeatureCheckBox->Add(CheckBox8BitSPI); // 4
	ScanFeatureCheckBox->Add(CheckBoxGPIOGain); // 5
	ScanFeatureCheckBox->Add(CheckBoxReverseXScan); // 6
	ScanFeatureCheckBox->Add(CheckBoxReverseYScan); // 7
	ScanFeatureCheckBox->Add(CheckBoxDoubleFirst); // 8
	ScanFeatureCheckBox->Add(CheckBoxAntiGlare); // 9
	ScanFeatureCheckBox->Add(CheckBoxCPLDMagicConfig); // 10
	ScanFeatureCheckBox->Add(CheckBoxSwEmitRev); // 11
	ScanFeatureCheckBox->Add(CheckBoxGPIOSwitch); // 12
	ScanFeatureCheckBox->Add(CheckBoxSkipLastY); // 13
	ScanFeatureCheckBox->Add(CheckBoxSkipX); // 14
	ScanFeatureCheckBox->Add(CheckBoxReorderMap); // 15
	ScanFeatureCheckBox->Add(CheckBoxNotD7); // 16
	ScanFeatureCheckBox->Add(CheckBoxWaitRxPowerOn); // 17
	ScanFeatureCheckBox->Add(CheckBoxSupport64Boards); // 18

	XCutBoards->Add(ComboBoxXCut1);
	XCutBoards->Add(ComboBoxXCut2);
	XCutBoards->Add(ComboBoxXCut3);
	XCutBoards->Add(ComboBoxXCut4);
	XCutBoards->Add(EditXtail1);
	XCutBoards->Add(EditXtail2);
	XCutBoards->Add(EditXtail3);
	XCutBoards->Add(EditXtail4);

	YCutBoards->Add(ComboBoxYCut1);
	YCutBoards->Add(ComboBoxYCut2);
	YCutBoards->Add(ComboBoxYCut3);
	YCutBoards->Add(ComboBoxYCut4);
	YCutBoards->Add(EditYtail1);
	YCutBoards->Add(EditYtail2);
	YCutBoards->Add(EditYtail3);
	YCutBoards->Add(EditYtail4);

	TIniFile *ini = new TIniFile(ExtractFilePath(Application->ExeName) +
		"screen.ini");
	ComboBoxStdScreen->Clear();
	ComboBoxStdParam->Clear();

	ini->ReadSections(ComboBoxStdScreen->Items);
	ini->ReadSections(ComboBoxStdParam->Items);

	ComboBoxStdScreen->ItemIndex = 0;
	ComboBoxStdParam->ItemIndex = 0;
	delete ini;

	ComboBoxStdScreenChange(NULL);
	ComboBoxStdParamChange(NULL);

	ComboBoxXAxisChange(NULL);
	ComboBoxYAxisChange(NULL);
	Button4Click(NULL);

	ComboBoxXDivMethodChange(NULL);
	ComboBoxYDivMethodChange(NULL);
	// Button2Click(NULL);
	// Button3Click(NULL);

#if SECRET_INITIAL_ON == 1
	ButtonSecretInitial->Visible = true;
#else
	ButtonSecretInitial->Visible = false;
#endif

	// TabSheetScan->Visible = false;
	// TabSheetScan->TabVisible = false;
	// TabSheetScan->Enabled = false;
	//
	// TabSheet3->Visible = false;
	// TabSheet3->TabVisible = false;
	// TabSheet3->Enabled = false;

	PageControl1->ActivePage = TabSheetScan; // TabSheetParam;
	PageControl2->ActivePage = TabSheet2;
	PageControl3->ActivePage = TabSheet7;

#ifndef _DEBUG
	TabSheet1->Enabled = false;
	TabSheet1->TabVisible = false;
	TabSheet1->Visible = false;

	TabSheet4->Enabled = false;
	TabSheet4->TabVisible = false;
	TabSheet4->Visible = false;

	// TabSheet5->Enabled = false;
	// TabSheet5->TabVisible = false;
	// TabSheet5->Visible = false;

	TabSheet6->Enabled = false;
	TabSheet6->TabVisible = false;
	TabSheet6->Visible = false;

	// TabSheet8->Enabled = false;
	// TabSheet8->TabVisible = false;
	// TabSheet8->Visible = false;

	TabSheet9->Enabled = false;
	TabSheet9->TabVisible = false;
	TabSheet9->Visible = false;
#endif

	ComboBoxEmitXVer->ShowHint = true;
	ComboBoxEmitXVer->Hint =
		"V4.6: W系列发射板 69灯 \n" "V5.4: T系列发射板/主板 \n" "V6.5: S系列发射板/主板 72灯 15mm\n"
		"V7.0: \n" "X-V1.1: 21.5寸三极管小屏\n" "X-V8.0: 26\"\n" "JY-T6-21-N: \n"
		"V9.0: \n" "X-PD-21.5-R2: \n" "V10.0: TII系列发射板/主板\n"
		"X-V5.2: 21\"~27\"小屏\n" "JY-T6-21-N-R2: \n" "V6.7: SII V1\n"
		"X-V12.0: \n" "X-V12.1: \n" "V6.8 SII: V2\n" "X-V12.2: \n"
		"V6.9: SII V3、V6.71 15mm\n" "V13.0: \n" "15M-X-HS-EMIT: \n"
		"MainV6.9 EmitV6.8: V6.9主板搭配V6.8的发射板\n" "V5.7 四层主板外置带屏蔽罩 定制版 90寸\n";

	DispSta();
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::ComboBoxXAxisChange(TObject *Sender) {
	int Offs = _wtoi(ComboBoxXAxis->Text.c_str());

	for (int i = 0; i < MAX_AXIS; i += 1) {
		if (i < Offs) {
			((TEdit *)(XParamEdit->Items[i]))->Enabled = true;
			((TEdit *)(XParamEdit->Items[i + MAX_AXIS]))->Enabled = true;
			((TEdit *)(XParamEdit->Items[i + MAX_AXIS * 2]))->Enabled = true;

			((TComboBox *)(XParamEdit->Items[i + MAX_AXIS * 3]))
				->Enabled = true;
			((TComboBox *)(XParamEdit->Items[i + MAX_AXIS * 4]))
				->Enabled = true;

			((TEdit *)(XParamEdit->Items[i + MAX_AXIS * 5]))->Enabled = true;
		}
		else {
			((TEdit *)(XParamEdit->Items[i]))->Enabled = false;
			((TEdit *)(XParamEdit->Items[i + MAX_AXIS]))->Enabled = false;
			((TEdit *)(XParamEdit->Items[i + MAX_AXIS * 2]))->Enabled = false;

			((TComboBox *)(XParamEdit->Items[i + MAX_AXIS * 3]))
				->Enabled = false;
			((TComboBox *)(XParamEdit->Items[i + MAX_AXIS * 4]))
				->Enabled = false;

			((TEdit *)(XParamEdit->Items[i + MAX_AXIS * 5]))->Enabled = false;
		}
	}

	if (Offs == 13) {
		ComboBoxXDivMethod->ItemIndex = 4;
	}
	else {
		ComboBoxXDivMethod->ItemIndex = 0;
	}

	// Button2Click(NULL);
	ComboBoxXDivMethodChange(NULL);
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::ComboBoxYAxisChange(TObject *Sender) {
	int Axis = _wtoi(ComboBoxYAxis->Text.c_str());

	for (int i = 0; i < MAX_AXIS; i += 1) {
		if (i < Axis) {
			((TEdit *)(YParamEdit->Items[i]))->Enabled = true;
			((TEdit *)(YParamEdit->Items[i + MAX_AXIS]))->Enabled = true;
			((TEdit *)(YParamEdit->Items[i + MAX_AXIS * 2]))->Enabled = true;

			((TComboBox *)(YParamEdit->Items[i + MAX_AXIS * 3]))
				->Enabled = true;
			((TComboBox *)(YParamEdit->Items[i + MAX_AXIS * 4]))
				->Enabled = true;

			((TEdit *)(YParamEdit->Items[i + MAX_AXIS * 5]))->Enabled = true;
		}
		else {
			((TEdit *)(YParamEdit->Items[i]))->Enabled = false;
			((TEdit *)(YParamEdit->Items[i + MAX_AXIS]))->Enabled = false;
			((TEdit *)(YParamEdit->Items[i + MAX_AXIS * 2]))->Enabled = false;

			((TComboBox *)(YParamEdit->Items[i + MAX_AXIS * 3]))
				->Enabled = false;
			((TComboBox *)(YParamEdit->Items[i + MAX_AXIS * 4]))
				->Enabled = false;

			((TEdit *)(YParamEdit->Items[i + MAX_AXIS * 5]))->Enabled = false;
		}
	}

	if (Axis == 13) {
		ComboBoxYDivMethod->ItemIndex = 4;
	}
	else {
		ComboBoxYDivMethod->ItemIndex = 0;
	}

	// Button3Click(NULL);
	ComboBoxYDivMethodChange(NULL);
}

void __fastcall TFormConfig::ClcStdParam(TList *Edits, AXI_PARAM *Ap) {
	String Msg;
	float Offlen;
	int OffLed;
	float OffDeg;

	for (int i = 0; i < Ap->Axis; i += 1) {
		Msg.sprintf(L"%.2f", Ap->RequestDegOff[i]);
		((TEdit *)(Edits->Items[i]))->Text = Msg;

		Offlen = Ap->Len * tan(Ap->RequestDegOff[i] / 180.0 * PI);
		if (Offlen > 0) {
			Offlen += Ap->Dis / 2;
		}
		else {
			Offlen -= Ap->Dis / 2;
		}
		OffLed = abs(Offlen / Ap->Dis);

		// if(i==1 || i==2){
		// OffLed = 1;
		// } else if(i== 5 || i==6) {
		// OffLed = Ap->LedOff[4] + 1;
		// } else if(i==9 || i==10) {
		// OffLed = Ap->LedOff[8] + 1;
		// }

		// if(i == 0){
		// Offlen = 0;
		// OffLed = 0;
		// } else if(i < 5){
		// OffLed = (i+1)/2;
		// Offlen = Ap->Len *tan(Ap->RequestDegOff[i] / 180.0 * PI);
		// } else {
		// Offlen = Ap->Len *tan(Ap->RequestDegOff[(i-4)*2 + (i&0x01)] / 180.0 * PI);
		// if(Offlen>0){
		// Offlen += Ap->Dis/2;
		// } else{
		// Offlen -= Ap->Dis/2;
		// }
		// OffLed = abs(Offlen/Ap->Dis);
		// }

		// if(i == 0){
		// Offlen = 0;
		// OffLed = 0;
		// } else if(i < 5){
		// OffLed = (i+1)/2;
		// Offlen = Ap->Len *tan(Ap->RequestDegOff[i] / 180.0 * PI);
		// } else {
		// if(((i+1)/2) & 0x01) {
		// Offlen = Ap->Len *tan(Ap->RequestDegOff[(i-4)*2 + (i&0x01)] / 180.0 * PI);
		// if(Offlen>0){
		// Offlen += Ap->Dis/2;
		// } else{
		// Offlen -= Ap->Dis/2;
		// }
		// OffLed = abs(Offlen/Ap->Dis);
		// } else {
		// if(i & 0x01) {
		// OffLed = Ap->LedOff[i-2] -1;
		// } else {
		// OffLed = Ap->LedOff[i-2] +1;
		// }
		//
		// Offlen = OffLed * Ap->Dis;
		// if(OffLed < 0) {
		// OffLed = 0 - OffLed;
		// }
		// //Offlen = Ap->Len *tan(Ap->RequestDegOff[i] / 180.0 * PI);
		// }
		// }

		// if(i == 0){
		// Offlen = 0;
		// OffLed = 0;
		// } else if(i < 3){
		// OffLed = (i+1)/2;
		// Offlen = Ap->Len *tan(Ap->RequestDegOff[i] / 180.0 * PI);
		// } else {
		// if((((i+1)/2) & 0x01) == 0) {
		//
		// Offlen = Ap->Len *tan(Ap->RequestDegOff[(i-2)*2 + (i&0x01)] / 180.0 * PI);
		// if(Offlen>0){
		// Offlen += Ap->Dis/2;
		// } else{
		// Offlen -= Ap->Dis/2;
		// }
		// OffLed = abs(Offlen/Ap->Dis);
		// } else {
		// if(i & 0x01) {
		// OffLed = Ap->LedOff[i-2] -1;
		// } else {
		// OffLed = Ap->LedOff[i-2] +1;
		// }
		//
		// Offlen = OffLed * Ap->Dis;
		// if(OffLed < 0) {
		// OffLed = 0 - OffLed;
		// }
		// //Offlen = Ap->Len *tan(Ap->RequestDegOff[i] / 180.0 * PI);
		// }
		// }

		if (Ap->MiniOff > 1 && OffLed % Ap->MiniOff != 0) {
			if (OffLed % Ap->MiniOff > Ap->MiniOff / 2) {
				OffLed = OffLed + Ap->MiniOff - (OffLed % Ap->MiniOff);
			}
			else {
				OffLed = OffLed - (OffLed % Ap->MiniOff);
			}
		}

		if (Offlen < 0)
			OffLed = 0 - OffLed;

		Ap->LedOff[i] = OffLed;

		Msg.sprintf(L"%d", OffLed);
		((TEdit *)(Edits->Items[i + MAX_AXIS]))->Text = Msg;

		Offlen = OffLed * Ap->Dis;
		Ap->LenOff[i] = Offlen;
		Ap->Tan[i] = Offlen / Ap->Len;
		Ap->ActualDegOff[i] = OffDeg = atan(Offlen / Ap->Len) / PI * 180;

		Msg.sprintf(L"%.2f", OffDeg);
		((TEdit *)(Edits->Items[i + MAX_AXIS * 2]))->Text = Msg;
	}
}

void __fastcall TFormConfig::ClcMixParam(TList *Edits, AXI_PARAM *Ap) {
	// clc mix
	Ap->MixAxis = Ap->Axis * 2 - 1;

	for (int i = 0; i < Ap->Axis; i += 1) {
		if (i == 0) {
			Ap->MixTan[0] = Ap->Tan[0];
			Ap->MixCos[0] = 1 / sqrt(1 + Ap->Tan[0] * Ap->Tan[0]);
			Ap->MixStartLed[0] = 0;
			Ap->MixEndLed[0] = Ap->TotalLeds;
			Ap->MixAxisId[0] = 0;
			Ap->MixLedOff[0] = Ap->LedOff[0];
		}
		else {
			Ap->MixAxisId[i * 2 - 1] = i;
			Ap->MixAxisId[i * 2] = i;

			if (Ap->LedOff[i] < 0) {
				Ap->MixTan[i * 2 - 1] = Ap->AuxTan[i];
				Ap->MixCos[i * 2 - 1] =
					1 / sqrt(1 + Ap->AuxTan[i] * Ap->AuxTan[i]);
				Ap->MixStartLed[i * 2 - 1] = 0;
				Ap->MixEndLed[i * 2 - 1] = -1 * Ap->LedOff[i];
				Ap->MixLedOff[i * 2 - 1] = Ap->AuxLedOff[i];

				Ap->MixTan[i * 2] = Ap->Tan[i];
				Ap->MixCos[i * 2] = 1 / sqrt(1 + Ap->Tan[i] * Ap->Tan[i]);
				Ap->MixStartLed[i * 2] = -1 * Ap->LedOff[i];
				Ap->MixEndLed[i * 2] = Ap->TotalLeds;
				Ap->MixLedOff[i * 2] = Ap->LedOff[i];
			}
			else {
				Ap->MixTan[i * 2 - 1] = Ap->Tan[i];
				Ap->MixCos[i * 2 - 1] = 1 / sqrt(1 + Ap->Tan[i] * Ap->Tan[i]);
				Ap->MixStartLed[i * 2 - 1] = 0;
				Ap->MixEndLed[i * 2 - 1] = Ap->TotalLeds - Ap->LedOff[i];
				Ap->MixLedOff[i * 2 - 1] = Ap->LedOff[i];

				Ap->MixTan[i * 2] = Ap->AuxTan[i];
				Ap->MixCos[i * 2] = 1 / sqrt(1 + Ap->AuxTan[i] * Ap->AuxTan[i]);
				Ap->MixStartLed[i * 2] = Ap->TotalLeds - Ap->LedOff[i];
				Ap->MixEndLed[i * 2] = Ap->TotalLeds;
				Ap->MixLedOff[i * 2] = Ap->AuxLedOff[i];
			}
		}
	}
}

void __fastcall TFormConfig::ClcParam(TList *Edits, AXI_PARAM *Ap) {

	ClcStdParam(Edits, Ap);
	ClcAuxParam(Edits, Ap);
	ClcMixParam(Edits, Ap);

	return;
}

void __fastcall TFormConfig::ClcAuxParam(TList *Edits, AXI_PARAM *Ap) {
	String Msg = "";
	float Offlen;
	int OffLed;
	// float OffDeg;
	// float ReqDeg[MAX_AXIS];

	int off, lastoff;
	for (int i = 0; i < (Ap->Axis + 1) / 2; i += 1) {
		if (i == 0) { // 0
			off = -1 * Ap->LedOff[0] / 2;
			((TEdit*)(Edits->Items[MAX_AXIS * 5]))->Text = String(off);
		}
		else {
			off = (Ap->LedOff[i * 2] - Ap->LedOff[(i - 1) * 2]) / 2;

			if (i >= 3 && off == 0) {
				off += lastoff + 1;
			}
			else {
				off += Ap->LedOff[(i - 1) * 2];
			}

			((TEdit*)(Edits->Items[i * 2 + MAX_AXIS * 5]))->Text =
				String(-1 * off);
			((TEdit*)(Edits->Items[i * 2 - 1 + MAX_AXIS * 5]))->Text =
				String(off);

			lastoff = off;
		}
	}

	for (int i = 0; i < Ap->Axis; i += 1) // fix me!!!  轴偏移角度排序
	{
		// if(i < 2)
		// {
		// Ap->AuxReqDegOff[i] = Ap->RequestDegOff[i]/2;
		// }
		// else
		// {
		// Ap->AuxReqDegOff[i] = (Ap->RequestDegOff[i] + Ap->RequestDegOff[i-2])/2;
		// }

		// Offlen = Ap->Len *tan(Ap->AuxReqDegOff[i] / 180.0 * PI);
		// if(Offlen>0)
		// {
		// Offlen += Ap->Dis/2;
		// }
		// else
		// {
		// Offlen -= Ap->Dis/2;
		// }

		// Ap->AuxLedOff[i] = OffLed = Offlen/Ap->Dis;

		OffLed = _wtoi(((TEdit*)(Edits->Items[i + MAX_AXIS * 5]))
			->Text.c_str());
		Offlen = OffLed * Ap->Dis;
		if (Offlen > 0) {
			Offlen += Ap->Dis / 2;
		}
		else {
			Offlen -= Ap->Dis / 2;
		}

		OffLed = abs(Offlen / Ap->Dis);

		if (Ap->MiniOff > 1 && OffLed % Ap->MiniOff != 0) {
			if (OffLed % Ap->MiniOff > Ap->MiniOff / 2) {
				OffLed = OffLed + Ap->MiniOff - (OffLed % Ap->MiniOff);
			}
			else {
				OffLed = OffLed - (OffLed % Ap->MiniOff);
			}
		}

		if (Offlen < 0)
			OffLed = 0 - OffLed;

		Ap->AuxLedOff[i] = OffLed;

		Offlen = OffLed * Ap->Dis;
		Ap->AuxLenOff[i] = Offlen;
		Ap->AuxTan[i] = Offlen / Ap->Len;

		Ap->AuxActuDegOff[i] = atan(Offlen / Ap->Len) / PI * 180;

		// Msg.cat_printf("%.2f, ", Ap->AuxReqDegOff[i]);
		// Msg.cat_printf("%.2f, ", Ap->AuxActuDegOff[i]);
		// Msg.cat_printf("%d, ", Ap->AuxLedOff[i]);

		// Msg.cat_printf("\n");
		Msg.printf(L"%d", Ap->AuxLedOff[i]);
		((TEdit *)(Edits->Items[i + MAX_AXIS * 5]))->Text = Msg;

	}

	// ShowMessage(Msg);
	// float fTemp;
	// int iTemp;
	//
	// for(int i=1; i<Ap->Axis; i+=2)
	// {
	// fTemp = Ap->AuxReqDegOff[i];
	// Ap->AuxReqDegOff[i] = Ap->AuxReqDegOff[i+1];
	// Ap->AuxReqDegOff[i+1] = fTemp;
	//
	// fTemp = Ap->AuxActuDegOff[i];
	// Ap->AuxActuDegOff[i] = Ap->AuxActuDegOff[i+1];
	// Ap->AuxActuDegOff[i+1] = fTemp;
	//
	// iTemp = Ap->AuxLedOff[i];
	// Ap->AuxLedOff[i] = Ap->AuxLedOff[i+1];
	// Ap->AuxLedOff[i+1] = iTemp;
	//
	// fTemp = Ap->AuxTan[i];
	// Ap->AuxTan[i] = Ap->AuxTan[i+1];
	// Ap->AuxTan[i+1] = fTemp;
	//
	// fTemp = Ap->AuxLenOff[i];
	// Ap->AuxLenOff[i] = Ap->AuxLenOff[i+1];
	// Ap->AuxLenOff[i+1] = fTemp;
	// }

	return;
}

// ---------------------------------------------------------------------------
void __fastcall TFormConfig::Button2Click(TObject *Sender) {
	float MaxXOff = _wtof(EditXoffset->Text.c_str());
	float OffStep;
	int Axis = _wtoi(ComboBoxXAxis->Text.c_str());
	String Msg;

	Msg.sprintf(L"%.2f", MaxXOff);
	// EditXoffset->Text = Msg;

	if (Axis == 1) {
		XParam.RequestDegOff[0] = MaxXOff;
	}
	else {
		OffStep = MaxXOff / (int)(Axis / 2);

		for (int i = 0; i < Axis; i += 1) {
			int iTemp = (i + 1) / 2 * (i & 0x01 ? -1 : 1);

			XParam.RequestDegOff[i] = OffStep * iTemp;
		}
	}

	XParam.Axis = Axis;

	// XParam.Leds = _wtoi(EditXLeds->Text.c_str());
	XParam.Dis = _wtof(EditXDist->Text.c_str());
	XParam.Margin = _wtof(EditXMargin->Text.c_str());
	XParam.MarginRightDown = _wtof(EditXMarginRight->Text.c_str());

	// YParam.Leds = _wtoi(EditYLeds->Text.c_str());
	YParam.Dis = _wtof(EditYDist->Text.c_str());
	YParam.Margin = _wtof(EditYMargin->Text.c_str());
	YParam.MarginRightDown = _wtof(EditYMarginDown->Text.c_str());

	XParam.Len = YParam.Dis * (YParam.TotalLeds) + YParam.Margin +
		YParam.MarginRightDown;

	XParam.MiniOff = _wtoi(EditXMiniOff->Text.c_str());

	ClcParam(XParamEdit, &XParam);
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::Button3Click(TObject *Sender) {
	float MaxYOff = _wtof(EditYoffset->Text.c_str());
	float OffStep;
	int Axis = _wtoi(ComboBoxYAxis->Text.c_str());
	String Msg;

	Msg.sprintf(L"%.2f", MaxYOff);
	// EditYoffset->Text = Msg;

	if (Axis == 1) {
		YParam.RequestDegOff[0] = MaxYOff;
	}
	else {
		OffStep = MaxYOff / (int)(Axis / 2);

		for (int i = 0; i < Axis; i += 1) {
			int iTemp = (i + 1) / 2 * (i & 0x01 ? -1 : 1);

			YParam.RequestDegOff[i] = OffStep * iTemp;
		}
	}

	YParam.Axis = Axis;

	// YParam.Leds = _wtoi(EditYLeds->Text.c_str());
	YParam.Dis = _wtof(EditYDist->Text.c_str());
	YParam.Margin = _wtof(EditYMargin->Text.c_str());
	YParam.MarginRightDown = _wtof(EditYMarginDown->Text.c_str());

	// XParam.Leds = _wtoi(EditXLeds->Text.c_str());
	XParam.Dis = _wtof(EditXDist->Text.c_str());
	XParam.Margin = _wtof(EditXMargin->Text.c_str());
	XParam.MarginRightDown = _wtof(EditXMarginRight->Text.c_str());

	YParam.Len = XParam.Dis * (XParam.TotalLeds) + XParam.Margin +
		XParam.MarginRightDown;

	YParam.MiniOff = _wtoi(EditYMiniOff->Text.c_str());

	ClcParam(YParamEdit, &YParam);
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::Button4Click(TObject *Sender) {
	String Msg;
	float ReqDegs[MAX_AXIS];

	StatusBar1->SimpleText = L"";

	XParam.Axis = _wtoi(ComboBoxXAxis->Text.c_str());
	XParam.Dis = _wtof(EditXDist->Text.c_str());
	XParam.Margin = _wtof(EditXMargin->Text.c_str());
	XParam.MarginRightDown = _wtof(EditXMarginRight->Text.c_str());
	XParam.EmitPower = _wtoi(ComboBoxXPower->Text.c_str()) & 0x07;
	XParam.MiniOff = _wtoi(EditXMiniOff->Text.c_str());
	XParam.JumpLeds = _wtoi(EditXJump->Text.c_str());
	XParam.ScanGap = _wtoi(EditXScanGap->Text.c_str());

	XParam.SkipFirstAxi = CheckBoxSkipFirstAxis->Checked;
	XParam.DualSpiData = CheckBoxDualSPIData->Checked;
	XParam.ShutdownEmit = CheckBoxShutDownEmit->Checked;
	XParam.SPI8Bits = CheckBox8BitSPI->Checked;
	XParam.GPIOGain = CheckBoxGPIOGain->Checked;
	XParam.ReverseXScan = CheckBoxReverseXScan->Checked;
	XParam.ReverseYScan = CheckBoxReverseYScan->Checked;
	XParam.SwitchEmit = CheckBoxSwitchEmit->Checked;
	XParam.GPIOSwitch = CheckBoxGPIOSwitch->Checked;
	XParam.NotD7 = CheckBoxNotD7->Checked;
	XParam.WaitRxPowerOn = CheckBoxWaitRxPowerOn->Checked;

	ComboBoxXBoardsChange(NULL);

	YParam.Axis = _wtoi(ComboBoxYAxis->Text.c_str());
	YParam.Dis = _wtof(EditYDist->Text.c_str());
	YParam.Margin = _wtof(EditYMargin->Text.c_str());
	YParam.MarginRightDown = _wtof(EditYMarginDown->Text.c_str());
	YParam.EmitPower = _wtoi(ComboBoxYPower->Text.c_str()) & 0x07;
	YParam.MiniOff = _wtoi(EditYMiniOff->Text.c_str());
	YParam.JumpLeds = _wtoi(EditYJump->Text.c_str());
	YParam.ScanGap = _wtoi(EditYScanGap->Text.c_str());

	YParam.SkipFirstAxi = CheckBoxSkipFirstAxis->Checked;
	YParam.DualSpiData = CheckBoxDualSPIData->Checked;
	YParam.ShutdownEmit = CheckBoxShutDownEmit->Checked;
	YParam.SPI8Bits = CheckBox8BitSPI->Checked;
	YParam.GPIOGain = CheckBoxGPIOGain->Checked;
	YParam.ReverseXScan = CheckBoxReverseXScan->Checked;
	YParam.ReverseYScan = CheckBoxReverseYScan->Checked;
	YParam.SwitchEmit = CheckBoxSwitchEmit->Checked;
	YParam.GPIOSwitch = CheckBoxGPIOSwitch->Checked;
	YParam.NotD7 = CheckBoxNotD7->Checked;
	YParam.WaitRxPowerOn = CheckBoxWaitRxPowerOn->Checked;

	ComboBoxYBoardsChange(NULL);

	XParam.Len = YParam.Dis * YParam.TotalLeds + YParam.Margin +
		YParam.MarginRightDown;
	YParam.Len = XParam.Dis * XParam.TotalLeds + XParam.Margin +
		XParam.MarginRightDown;

	// Button2Click(NULL);
	// Button3Click(NULL);
	ComboBoxXDivMethodChange(NULL);
	ComboBoxYDivMethodChange(NULL);

	EditXoffsetChange(NULL);
	EditYoffsetChange(NULL);

	for (int i = 0; i < XParam.Axis; i += 1) {
		float fTemp = _wtof(((TEdit*)(XParamEdit->Items[i]))->Text.c_str());
		int Gain1, Gain2;

		Msg.sprintf(L"%.2f", fTemp);
		((TEdit *)(XParamEdit->Items[i + MAX_AXIS]))->Text = Msg;

		XParam.RequestDegOff[i] = fTemp;

		Gain1 = _wtoi(((TComboBox*)XParamEdit->Items[i + MAX_AXIS * 3])
			->Text.c_str());
		Gain2 = _wtoi(((TComboBox*)XParamEdit->Items[i + MAX_AXIS * 4])
			->Text.c_str());

		XParam.Gain[i] = Gain2 << 3 | Gain1;
	}

	ClcParam(XParamEdit, &XParam);

	for (int i = 0; i < YParam.Axis; i += 1) {
		float fTemp = _wtof(((TEdit*)(YParamEdit->Items[i]))->Text.c_str());
		int Gain1, Gain2;

		Msg.sprintf(L"%.2f", fTemp);
		((TEdit *)(YParamEdit->Items[i + MAX_AXIS]))->Text = Msg;

		YParam.RequestDegOff[i] = fTemp;

		Gain1 = _wtoi(((TComboBox*)YParamEdit->Items[i + MAX_AXIS * 3])
			->Text.c_str());
		Gain2 = _wtoi(((TComboBox*)YParamEdit->Items[i + MAX_AXIS * 4])
			->Text.c_str());

		YParam.Gain[i] = Gain2 << 3 | Gain1;
	}

	ClcParam(YParamEdit, &YParam);

	StatusBar1->SimpleText = L"刷新成功！";
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::ComboBoxXBoardsChange(TObject *Sender) {
	AXI_PARAM *ap = &XParam;
	int Cuts = XCutBoards->Count / 2;
	int i;

	ap->Boards = _wtoi(ComboBoxXBoards->Text.c_str());

	if (Sender && (TComboBox*)Sender == ComboBoxXBoards) {
		for (i = 0; i < Cuts; i += 1) {
			((TComboBox *)XCutBoards->Items[i])->Items->Clear();
			((TEdit*)XCutBoards->Items[i + Cuts])->Text =
				ComboBoxXBoardLeds->Text;
		}

		for (i = ap->Boards; i > 0; i -= 1) {
			for (int j = 0; j < Cuts; j += 1) {
				((TComboBox *)XCutBoards->Items[j])->Items->Add(String(i));
			}
		}

		for (i = 0; i < ap->Boards && i < Cuts; i += 1) {
			String Msg = String(ap->Boards - i);

			((TComboBox *)XCutBoards->Items[i])->Enabled = true;
			((TEdit *)XCutBoards->Items[i + Cuts])->Enabled = true;
			((TComboBox*)XCutBoards->Items[i])->ItemIndex =
				((TComboBox*)XCutBoards->Items[i])->Items->IndexOf(Msg);
		}

		for (i = ap->Boards; i < Cuts; i += 1) {
			((TComboBox *)XCutBoards->Items[i])->Enabled = false;
			((TEdit *)XCutBoards->Items[i + Cuts])->Enabled = false;
		}
	}

	ap->CutUnused = CheckBox17->Checked;
	if (!CheckBox17->Checked) {
		for (i = 0; i < ap->Boards; i += 1) {
			ap->BoradLeds[i] = _wtoi(ComboBoxXBoardLeds->Text.c_str());
		}

		for (i = 0; i < Cuts; i += 1) {
			if (((TComboBox*)XCutBoards->Items[i])->Enabled == true) {
				int BoardNo =
					_wtoi(((TComboBox*)XCutBoards->Items[i])->Text.c_str());

				if (BoardNo > 0 && BoardNo <= ap->Boards) {
					ap->BoradLeds[BoardNo - 1] =
						_wtoi(((TEdit*)XCutBoards->Items[i + Cuts])
						->Text.c_str());
				}
			}
		}
	}
	else {
		EditXBoardLedChange(NULL);
	}

	String msg = "";
	for (i = 0; i < ap->Boards; i += 1) {
		msg.cat_printf(L"%d,", ap->BoradLeds[i]);
	}
	EditXBoardLed->Text = msg;

	for (ap->TotalLeds = 0, i = 0; i < ap->Boards; i += 1) {
		ap->TotalLeds += ap->BoradLeds[i];
	}

	ap->JumpLeds = _wtoi(EditXJump->Text.c_str());
	ap->ScanGap = _wtoi(EditXScanGap->Text.c_str());
	ap->TotalLeds -= ap->JumpLeds;

	if (ap->ScanGap != 0) {
		ap->TotalLeds /= (ap->ScanGap + 1);
	}

	EditXLeds->Text = String(ap->TotalLeds);

	if (ComboBoxEmitXVer->Text == "V5.4") {
		if (CheckBoxSupport64Boards->Checked)
			ap->Get_Emit_Data = &Get_Emit_Data_V54_Support_64_Boards;
		else if (CheckBoxCPLDMagicConfig->Checked)
			ap->Get_Emit_Data = &Get_Emit_Data_V54_MagicConfig;
		else
			ap->Get_Emit_Data = &Get_Emit_Data_V54;
	}
	else if (ComboBoxEmitXVer->Text == "V5.6") {
		ap->Get_Emit_Data = &Get_Emit_Data_V56;
	}
	else if (ComboBoxEmitXVer->Text == "V5.7") {
		ap->Get_Emit_Data = &Get_Emit_Data_V57_MagicConfig;
	}
	else if (ComboBoxEmitXVer->Text == "V6.5") {
		if (CheckBoxCPLDMagicConfig->Checked == false)
			ap->Get_Emit_Data = &Get_Emit_Data_V65;
		else
			ap->Get_Emit_Data = &Get_Emit_Data_V65_MagicConfig;
	}
	else if (ComboBoxEmitXVer->Text == "V7.0") {
		ap->Get_Emit_Data = &Get_Emit_Data_V70;
	}
	else if (ComboBoxEmitXVer->Text == "X-V1.1") {
		ap->Get_Emit_Data = &Get_EmitX_Data_TypeX_V11;
	}
	else if (ComboBoxEmitXVer->Text == "X-V8.0") {
		ap->Get_Emit_Data = &Get_EmitX_Data_TypeX_V80;
	}
	else if (ComboBoxEmitXVer->Text == "JY-T6-21-N") {
		ap->Get_Emit_Data = &Get_EmitX_Data_TypeX_JYT6N_VR1;
	}
	else if (ComboBoxEmitXVer->Text == "JY-T6-21-N-R2") {
		ap->Get_Emit_Data = &Get_EmitX_Data_TypeX_JYT6N_VR2;
	}
	else if (ComboBoxEmitXVer->Text == "V9.0") {
		ap->Get_Emit_Data = &Get_Emit_Data_V90;
	}
	else if (ComboBoxEmitXVer->Text == "X-PD-21.5-R2") {
		ap->Get_Emit_Data = &Get_EmitX_Data_TypeX_VR2;
	}
	else if (ComboBoxEmitXVer->Text == "V10.0") {
		if (ComboBoxRevXVer->Text == "PD-V10.0")
			ap->Get_Emit_Data = &Get_Emit_Data_PDV100;
		else
			ap->Get_Emit_Data = &Get_Emit_Data_PTV100;
	}
	else if (ComboBoxEmitXVer->Text == "V15.0") {
		ap->Get_Emit_Data = &Get_Emit_Data_PTV150;
	}
	else if (ComboBoxEmitXVer->Text == "V13.0") {
		if (ComboBoxRevXVer->Text == "PD-V13.0")
			ap->Get_Emit_Data = &Get_Emit_Data_PDV130;
		else
			ap->Get_Emit_Data = &Get_Emit_Data_PTV130;
	}
	else if (ComboBoxEmitXVer->Text == "V13.3") {
		ap->Get_Emit_Data = &Get_Emit_Data_PDV133;
	}
	else if (ComboBoxEmitXVer->Text == "V13.3-1") {
		ap->Get_Emit_Data = &Get_Emit_Data_PDV1331;
	}
	else if (ComboBoxEmitXVer->Text == "V13.3-3") {
		ap->Get_Emit_Data = &Get_Emit_Data_PDV1333;
	}
	else if (ComboBoxEmitXVer->Text == "V13.3-4") {
		ap->Get_Emit_Data = &Get_Emit_Data_PDV1334;
	}
	else if (ComboBoxEmitXVer->Text == "V13.3-5") {
		ap->Get_Emit_Data = &Get_Emit_Data_PDV1335;
	}
	else if (ComboBoxEmitXVer->Text == "V6.7") {
		ap->Get_Emit_Data = &Get_Emit_Data_PDV67;
	}
	else if (ComboBoxEmitXVer->Text == "V6.8") {
		ap->Get_Emit_Data = &Get_Emit_Data_PDV68;
	}
	else if (ComboBoxEmitXVer->Text == "V6.9") {
		ap->Get_Emit_Data = &Get_Emit_Data_PDV69;
	}
	else if (ComboBoxEmitXVer->Text == "MainV6.9 EmitV6.8") {
		ap->Get_Emit_Data = &Get_Emit_Data_PDV69_V68;
	}
	else if (ComboBoxEmitXVer->Text == "MainV6.9-3N EmitV6.9") {
		ap->Get_Emit_Data = &Get_Emit_Data_PDV693_V69;
	}
	else if (ComboBoxEmitXVer->Text == "MainV6.9-3N EmitV6.8") {
		ap->Get_Emit_Data = &Get_Emit_Data_PDV693_V68;
	}
	else if (ComboBoxEmitXVer->Text == "X-V5.2") {
		ap->Get_Emit_Data = &Get_EmitX_Data_TypeX_V52;
	}
	else if (ComboBoxEmitXVer->Text == "X-V12.0") {
		ap->Get_Emit_Data = &Get_EmitX_Data_TypeX_V120;
	}
	else if (ComboBoxEmitXVer->Text == "X-V12.1") {
		ap->Get_Emit_Data = &Get_EmitX_Data_TypeX_V121;
	}
	else if (ComboBoxEmitXVer->Text == "X-V12.2") {
		ap->Get_Emit_Data = &Get_EmitX_Data_TypeX_V122;
	}
	else if (ComboBoxEmitXVer->Text == "15M-X-HS-EMIT") {
		ap->Get_Emit_Data = &Get_EmitX_Data_TypeX_V140;
	}
	else if (ComboBoxEmitXVer->Text == "X-21.5-V14.2" ||
		ComboBoxEmitXVer->Text == "E24-V14.6") {
		ap->Get_Emit_Data = &Get_EmitX_Data_TypeX_V142;
	}
	else if (ComboBoxEmitXVer->Text == "E19-V14.3" ||
		ComboBoxEmitXVer->Text == "E15-V14.4") {
		ap->Get_Emit_Data = &Get_EmitX_Data_TypeX_V143;
	}
	else if (ComboBoxEmitXVer->Text == "E20.1-V14.5") {
		ap->Get_Emit_Data = &Get_EmitX_Data_TypeX_V145;
	}
	else if (ComboBoxEmitXVer->Text == "E12.1-V14.7") {
		ap->Get_Emit_Data = &Get_EmitX_Data_TypeX_V147;
	}
	else if (ComboBoxEmitXVer->Text == "E15-FPC-V14.8") {
		ap->Get_Emit_Data = &Get_EmitX_Data_TypeX_V148;
	}
	else if (ComboBoxEmitXVer->Text == "X-V14.1") {
		ap->Get_Emit_Data = &Get_EmitX_Data_TypeX_V141;
	}
	else if (ComboBoxEmitXVer->Text == "fct") {
		ap->Get_Emit_Data = &Get_EmitX_Data_FCT;
	}
	else {
		if (CheckBoxCPLDMagicConfig->Checked == false) {
			ap->Get_Emit_Data = &Get_Emit_Data_V46;
		}
		else {
			ap->Get_Emit_Data = &Get_Emit_Data_V46_MagicConfig;
		}
	}

	if (ComboBoxRevXVer->Text == "PD-V5.4") {
		if (CheckBoxSupport64Boards->Checked)
			ap->Get_Rev_Data = &Get_Rev_Data_PDV54_Support_64_Boards;
		else if (CheckBoxCPLDMagicConfig->Checked)
			ap->Get_Rev_Data = &Get_Rev_Data_PDV54_MagicConfig;
		else
			ap->Get_Rev_Data = &Get_Rev_Data_PDV54;
	}
	else if (ComboBoxRevXVer->Text == "PD-V5.6") {
		ap->Get_Rev_Data = &Get_Rev_Data_PDV56;
	}
	else if (ComboBoxRevXVer->Text == "PD-V5.7") {
		ap->Get_Rev_Data = &Get_Rev_Data_PDV57_MagicConfig;
	}
	else if (ComboBoxRevXVer->Text == "PT-V5.4") {
		ap->Get_Rev_Data = &Get_Rev_Data_PTV54;
	}
	else if (ComboBoxRevXVer->Text == "PD-V6.5") {
		ap->Get_Rev_Data = &Get_Rev_Data_V65;
	}
	else if (ComboBoxRevXVer->Text == "PD-V7.0") {
		ap->Get_Rev_Data = &Get_Rev_Data_V70;
	}
	else if (ComboBoxRevXVer->Text == "X-PT-V1.1") {
		ap->Get_Rev_Data = &Get_RevX_Data_TypeX_PTV11;
	}
	else if (ComboBoxRevXVer->Text == "X-PT-V8.0") {
		ap->Get_Rev_Data = &Get_RevX_Data_TypeX_PTV80;
	}
	else if (ComboBoxRevXVer->Text == "JY-T6-21-N") {
		ap->Get_Rev_Data = &Get_RevX_Data_TypeX_JYT6N_PDVR1;
	}
	else if (ComboBoxRevXVer->Text == "JY-T6-21-N-R2") {
		ap->Get_Rev_Data = &Get_RevX_Data_TypeX_JYT6N_PDVR2;
	}
	else if (ComboBoxRevXVer->Text == "PT-V9.0") {
		ap->Get_Rev_Data = &Get_Rev_Data_V90;
	}
	else if (ComboBoxRevXVer->Text == "X-PD-21.5-R2") {
		ap->Get_Rev_Data = &Get_RevX_Data_TypeX_PDVR2;
	}
	else if (ComboBoxRevXVer->Text == "PD-V10.0") {
		ap->Get_Rev_Data = NULL;
	}
	else if (ComboBoxRevXVer->Text == "PD-V13.0") {
		ap->Get_Rev_Data = NULL;
	}
	else if (ComboBoxRevXVer->Text == "PD-V13.3") {
		ap->Get_Rev_Data = NULL;
	}
	else if (ComboBoxRevXVer->Text == "PD-V13.3-3") {
		ap->Get_Rev_Data = NULL;
	}
	else if (ComboBoxRevXVer->Text == "PD-V13.3-4") {
		ap->Get_Rev_Data = NULL;
	}
	else if (ComboBoxRevXVer->Text == "PD-V13.3-5") {
		ap->Get_Rev_Data = NULL;
	}
	else if (ComboBoxRevXVer->Text == "PT-V13.0") {
		ap->Get_Rev_Data = &Get_Rev_Data_PTV130;
	}
	else if (ComboBoxRevXVer->Text == "PT-V10.0") {
		ap->Get_Rev_Data = &Get_Rev_Data_PTV100;
	}
	else if (ComboBoxRevXVer->Text == "PT-V15.0") {
		ap->Get_Rev_Data = NULL;
	}
	else if (ComboBoxRevXVer->Text == "PD-V6.7") {
		ap->Get_Rev_Data = NULL;
	}
	else if (ComboBoxRevXVer->Text == "PD-V6.8") {
		ap->Get_Rev_Data = NULL;
	}
	else if (ComboBoxRevXVer->Text == "PD-V6.9") {
		ap->Get_Rev_Data = NULL;
	}
	else if (ComboBoxRevXVer->Text == "X-PD-V12.2") {
		ap->Get_Rev_Data = &Get_RevX_Data_TypeX_PDV122;
	}
	else if (ComboBoxRevXVer->Text == "X-PD-V12.0") {
		ap->Get_Rev_Data = &Get_RevX_Data_TypeX_PDV120;
	}
	else if (ComboBoxRevXVer->Text == "X-PD-V12.1") {
		ap->Get_Rev_Data = &Get_RevX_Data_TypeX_PDV121;
	}
	else if (ComboBoxRevXVer->Text == "X-PD-V5.2") {
		ap->Get_Rev_Data = &Get_RevX_Data_TypeX_PDV52;
	}
	else if (ComboBoxRevXVer->Text == "15M-X-HS-REV") {
		ap->Get_Rev_Data = &Get_RevX_Data_TypeX_PTV140;
	}
	else if (ComboBoxRevXVer->Text == "X-21.5-V14.2") {
		ap->Get_Rev_Data = &Get_RevX_Data_TypeX_PTV142;
	}
	else if (ComboBoxRevXVer->Text == "E24-V14.6") {
		ap->Get_Rev_Data = &Get_RevX_Data_TypeX_PTV146;
	}
	else if (ComboBoxRevXVer->Text == "E19-V14.3" ||
		ComboBoxRevXVer->Text == "E15-V14.4") {
		ap->Get_Rev_Data = &Get_RevX_Data_TypeX_PTV143;
	}
	else if (ComboBoxRevXVer->Text == "E20.1-V14.5") {
		ap->Get_Rev_Data = &Get_RevX_Data_TypeX_PTV145;
	}
	else if (ComboBoxRevXVer->Text == "E12.1-V14.7") {
		ap->Get_Rev_Data = &Get_RevX_Data_TypeX_PTV147;
	}
	else if (ComboBoxRevXVer->Text == "E15-FPC-V14.8") {
		ap->Get_Rev_Data = &Get_RevX_Data_TypeX_PTV148;
	}
	else if (ComboBoxRevXVer->Text == "X-PT-V14.1") {
		ap->Get_Rev_Data = &Get_RevX_Data_TypeX_PTV141;
	}
	else if (ComboBoxRevXVer->Text == "fct") {
		ap->Get_Rev_Data = &Get_RevX_Data_FCT;
	}
	else {
		if (CheckBoxCPLDMagicConfig->Checked == false) {
			ap->Get_Rev_Data = &Get_Rev_Data_V46;
		}
		else {
			ap->Get_Rev_Data = &Get_Rev_Data_V46_MagicConfig;
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::ComboBoxYBoardsChange(TObject *Sender) {
	AXI_PARAM *ap = &YParam;
	int Cuts = YCutBoards->Count / 2;
	int i;

	ap->Boards = _wtoi(ComboBoxYBoards->Text.c_str());

	if (Sender && (TComboBox*)Sender == ComboBoxYBoards) {
		for (i = 0; i < Cuts; i += 1) {
			((TComboBox *)YCutBoards->Items[i])->Items->Clear();
			((TEdit*)YCutBoards->Items[i + Cuts])->Text =
				ComboBoxYBoardLeds->Text;
		}

		for (i = ap->Boards; i > 0; i -= 1) {
			for (int j = 0; j < Cuts; j += 1) {
				((TComboBox *)YCutBoards->Items[j])->Items->Add(String(i));
			}
		}

		for (i = 0; i < ap->Boards && i < Cuts; i += 1) {
			String Msg = String(ap->Boards - i);

			((TComboBox *)YCutBoards->Items[i])->Enabled = true;
			((TEdit *)YCutBoards->Items[i + Cuts])->Enabled = true;
			((TComboBox*)YCutBoards->Items[i])->ItemIndex =
				((TComboBox*)YCutBoards->Items[i])->Items->IndexOf(Msg);
		}

		for (i = ap->Boards; i < Cuts; i += 1) {
			((TComboBox *)YCutBoards->Items[i])->Enabled = false;
			((TEdit *)YCutBoards->Items[i + Cuts])->Enabled = false;
		}
	}

	ap->CutUnused = CheckBox19->Checked;
	if (!CheckBox19->Checked) {
		for (i = 0; i < ap->Boards; i += 1) {
			ap->BoradLeds[i] = _wtoi(ComboBoxYBoardLeds->Text.c_str());
		}

		for (i = 0; i < Cuts; i += 1) {
			if (((TComboBox*)YCutBoards->Items[i])->Enabled == true) {
				int BoardNo =
					_wtoi(((TComboBox*)YCutBoards->Items[i])->Text.c_str());

				if (BoardNo > 0 && BoardNo <= ap->Boards) {
					ap->BoradLeds[BoardNo - 1] =
						_wtoi(((TEdit*)YCutBoards->Items[i + Cuts])
						->Text.c_str());
				}
			}
		}
	}
	else {
		EditYBoardLedChange(NULL);
	}

	String msg = "";
	for (i = 0; i < ap->Boards; i += 1) {
		msg.cat_printf(L"%d,", ap->BoradLeds[i]);
	}
	EditYBoardLed->Text = msg;

	for (ap->TotalLeds = 0, i = 0; i < ap->Boards; i += 1) {
		ap->TotalLeds += ap->BoradLeds[i];
	}

	ap->JumpLeds = _wtoi(EditYJump->Text.c_str());
	ap->ScanGap = _wtoi(EditYScanGap->Text.c_str());
	ap->TotalLeds -= ap->JumpLeds;

	if (ap->ScanGap != 0) {
		ap->TotalLeds /= (ap->ScanGap + 1);
	}

	if (CheckBoxSkipLastY->Checked) {
		ap->TotalLeds -= 1;
	}

	EditYLeds->Text = String(ap->TotalLeds);

	if (ComboBoxEmitYVer->Text == "V5.4") {
		if (CheckBoxSupport64Boards->Checked)
			ap->Get_Emit_Data = &Get_Emit_Data_V54_Support_64_Boards;
		else if (CheckBoxCPLDMagicConfig->Checked)
			ap->Get_Emit_Data = &Get_Emit_Data_V54_MagicConfig;
		else
			ap->Get_Emit_Data = &Get_Emit_Data_V54;
	}
	else if (ComboBoxEmitYVer->Text == "V5.6") {
		ap->Get_Emit_Data = &Get_Emit_Data_V56;
	}
	else if (ComboBoxEmitYVer->Text == "V5.7") {
		ap->Get_Emit_Data = &Get_Emit_Data_V57_MagicConfig;
	}
	else if (ComboBoxEmitYVer->Text == "V6.5") {
		if (CheckBoxCPLDMagicConfig->Checked == false)
			ap->Get_Emit_Data = &Get_Emit_Data_V65;
		else
			ap->Get_Emit_Data = &Get_Emit_Data_V65_MagicConfig;
	}
	else if (ComboBoxEmitYVer->Text == "V7.0") {
		ap->Get_Emit_Data = &Get_Emit_Data_V70;
	}
	else if (ComboBoxEmitYVer->Text == "X-V1.1") {
		ap->Get_Emit_Data = &Get_EmitY_Data_TypeX_V11;
	}
	else if (ComboBoxEmitYVer->Text == "X-V8.0") {
		ap->Get_Emit_Data = &Get_EmitY_Data_TypeX_V80;
	}
	else if (ComboBoxEmitYVer->Text == "JY-T6-21-N") {
		ap->Get_Emit_Data = &Get_EmitY_Data_TypeX_JYT6N_VR1;
	}
	else if (ComboBoxEmitYVer->Text == "JY-T6-21-N-R2") {
		ap->Get_Emit_Data = &Get_EmitY_Data_TypeX_JYT6N_VR2;
	}
	else if (ComboBoxEmitYVer->Text == "V9.0") {
		ap->Get_Emit_Data = &Get_Emit_Data_V90;
	}
	else if (ComboBoxEmitYVer->Text == "X-PD-21.5-R2") {
		ap->Get_Emit_Data = &Get_EmitY_Data_TypeX_VR2;
	}
	else if (ComboBoxEmitYVer->Text == "V10.0") {
		if (ComboBoxRevYVer->Text == "PD-V10.0")
			ap->Get_Emit_Data = &Get_Emit_Data_PDV100;
		else
			ap->Get_Emit_Data = &Get_Emit_Data_PTV100;
	}
	else if (ComboBoxEmitYVer->Text == "V15.0") {
		ap->Get_Emit_Data = &Get_Emit_Data_PTV150;
	}
	else if (ComboBoxEmitYVer->Text == "V13.0") {
		if (ComboBoxRevYVer->Text == "PD-V13.0")
			ap->Get_Emit_Data = &Get_Emit_Data_PDV130;
		else
			ap->Get_Emit_Data = &Get_Emit_Data_PTV130;
	}
	else if (ComboBoxEmitYVer->Text == "V13.3") {
		ap->Get_Emit_Data = &Get_Emit_Data_PDV133;
	}
	else if (ComboBoxEmitYVer->Text == "V13.3-1") {
		ap->Get_Emit_Data = &Get_Emit_Data_PDV1331;
	}
	else if (ComboBoxEmitYVer->Text == "V13.3-3") {
		ap->Get_Emit_Data = &Get_Emit_Data_PDV1333;
	}
	else if (ComboBoxEmitYVer->Text == "V13.3-4") {
		ap->Get_Emit_Data = &Get_Emit_Data_PDV1334;
	}
	else if (ComboBoxEmitYVer->Text == "V13.3-5") {
		ap->Get_Emit_Data = &Get_Emit_Data_PDV1335;
	}
	else if (ComboBoxEmitYVer->Text == "V6.7") {
		ap->Get_Emit_Data = &Get_Emit_Data_PDV67;
	}
	else if (ComboBoxEmitYVer->Text == "V6.8") {
		ap->Get_Emit_Data = &Get_Emit_Data_PDV68;
	}
	else if (ComboBoxEmitYVer->Text == "V6.9") {
		ap->Get_Emit_Data = &Get_Emit_Data_PDV69;
	}
	else if (ComboBoxEmitYVer->Text == "X-V5.2") {
		ap->Get_Emit_Data = &Get_EmitY_Data_TypeX_V52;
	}
	else if (ComboBoxEmitYVer->Text == "X-V12.0") {
		ap->Get_Emit_Data = &Get_EmitY_Data_TypeX_V120;
	}
	else if (ComboBoxEmitYVer->Text == "X-V12.1") {
		ap->Get_Emit_Data = &Get_EmitY_Data_TypeX_V121;
	}
	else if (ComboBoxEmitYVer->Text == "X-V12.2") {
		ap->Get_Emit_Data = &Get_EmitY_Data_TypeX_V122;
	}
	else if (ComboBoxEmitYVer->Text == "15M-X-HS-EMIT") {
		ap->Get_Emit_Data = &Get_EmitY_Data_TypeX_V140;
	}
	else if (ComboBoxEmitYVer->Text == "X-21.5-V14.2") {
		ap->Get_Emit_Data = &Get_EmitY_Data_TypeX_V142;
	}
	else if (ComboBoxEmitYVer->Text == "E24-V14.6") {
		ap->Get_Emit_Data = &Get_EmitY_Data_TypeX_V146;
	}
	else if (ComboBoxEmitYVer->Text == "E19-V14.3" ||
		ComboBoxEmitYVer->Text == "E15-V14.4") {
		ap->Get_Emit_Data = &Get_EmitY_Data_TypeX_V143;
	}
	else if (ComboBoxEmitYVer->Text == "E20.1-V14.5") {
		ap->Get_Emit_Data = &Get_EmitY_Data_TypeX_V145;
	}
	else if (ComboBoxEmitYVer->Text == "E12.1-V14.7") {
		ap->Get_Emit_Data = &Get_EmitY_Data_TypeX_V147;
	}
	else if (ComboBoxEmitYVer->Text == "E15-FPC-V14.8") {
		ap->Get_Emit_Data = &Get_EmitY_Data_TypeX_V148;
	}
	else if (ComboBoxEmitYVer->Text == "X-V14.1") {
		ap->Get_Emit_Data = &Get_EmitY_Data_TypeX_V141;
	}
	else if (ComboBoxEmitYVer->Text == "fct") {
		ap->Get_Emit_Data = &Get_EmitY_Data_FCT;
	}
	else {
		if (CheckBoxCPLDMagicConfig->Checked == false) {
			ap->Get_Emit_Data = &Get_Emit_Data_V46;
		}
		else {
			ap->Get_Emit_Data = &Get_Emit_Data_V46_MagicConfig;
		}
	}

	if (ComboBoxRevYVer->Text == "PD-V5.4") {
		if (CheckBoxSupport64Boards->Checked)
			ap->Get_Rev_Data = &Get_Rev_Data_PDV54_Support_64_Boards;
		else if (CheckBoxCPLDMagicConfig->Checked == false)
			ap->Get_Rev_Data = &Get_Rev_Data_PDV54;
		else
			ap->Get_Rev_Data = &Get_Rev_Data_PDV54_MagicConfig;
	}
	else if (ComboBoxRevYVer->Text == "PD-V5.6") {
		ap->Get_Rev_Data = &Get_Rev_Data_PDV56;
	}
	else if (ComboBoxRevYVer->Text == "PD-V5.7") {
		ap->Get_Rev_Data = &Get_Rev_Data_PDV57_MagicConfig;
	}
	else if (ComboBoxRevYVer->Text == "PT-V5.4") {
		ap->Get_Rev_Data = &Get_Rev_Data_PTV54;
	}
	else if (ComboBoxRevYVer->Text == "PD-V6.5") {
		ap->Get_Rev_Data = &Get_Rev_Data_V65;
	}
	else if (ComboBoxRevYVer->Text == "PD-V7.0") {
		ap->Get_Rev_Data = &Get_Rev_Data_V70;
	}
	else if (ComboBoxRevYVer->Text == "X-PT-V1.1") {
		ap->Get_Rev_Data = &Get_RevY_Data_TypeX_PTV11;
	}
	else if (ComboBoxRevYVer->Text == "X-PT-V8.0") {
		ap->Get_Rev_Data = &Get_RevY_Data_TypeX_PTV80;
	}
	else if (ComboBoxRevYVer->Text == "JY-T6-21-N") {
		ap->Get_Rev_Data = &Get_RevY_Data_TypeX_JYT6N_PDVR1;
	}
	else if (ComboBoxRevYVer->Text == "JY-T6-21-N-R2") {
		ap->Get_Rev_Data = &Get_RevY_Data_TypeX_JYT6N_PDVR2;
	}
	else if (ComboBoxRevYVer->Text == "PT-V9.0") {
		ap->Get_Rev_Data = &Get_Rev_Data_V90;
	}
	else if (ComboBoxRevYVer->Text == "X-PD-21.5-R2") {
		ap->Get_Rev_Data = &Get_RevY_Data_TypeX_PDVR2;
	}
	else if (ComboBoxRevYVer->Text == "PD-V10.0") {
		ap->Get_Rev_Data = NULL;
	}
	else if (ComboBoxRevYVer->Text == "PD-V13.0") {
		ap->Get_Rev_Data = NULL;
	}
	else if (ComboBoxRevYVer->Text == "PT-V13.0") {
		ap->Get_Rev_Data = &Get_Rev_Data_PTV130;
	}
	else if (ComboBoxRevYVer->Text == "PD-V13.3") {
		ap->Get_Rev_Data = NULL;
	}
	else if (ComboBoxRevYVer->Text == "PD-V13.3-3") {
		ap->Get_Rev_Data = NULL;
	}
	else if (ComboBoxRevYVer->Text == "PD-V13.3-4") {
		ap->Get_Rev_Data = NULL;
	}
	else if (ComboBoxRevYVer->Text == "PD-V13.3-5") {
		ap->Get_Rev_Data = NULL;
	}
	else if (ComboBoxRevYVer->Text == "PT-V10.0") {
		ap->Get_Rev_Data = &Get_Rev_Data_PTV100;
	}
	else if (ComboBoxRevYVer->Text == "PT-V15.0") {
		ap->Get_Rev_Data = NULL;
	}
	else if (ComboBoxRevYVer->Text == "PD-V6.7") {
		ap->Get_Rev_Data = NULL;
	}
	else if (ComboBoxRevYVer->Text == "PD-V6.8") {
		ap->Get_Rev_Data = NULL;
	}
	else if (ComboBoxRevYVer->Text == "PD-V6.9") {
		ap->Get_Rev_Data = NULL;
	}
	else if (ComboBoxRevYVer->Text == "X-PD-V5.2") {
		ap->Get_Rev_Data = &Get_RevY_Data_TypeX_PDV52;
	}
	else if (ComboBoxRevYVer->Text == "X-PD-V12.0") {
		ap->Get_Rev_Data = &Get_RevY_Data_TypeX_PDV120;
	}
	else if (ComboBoxRevYVer->Text == "X-PD-V12.1") {
		ap->Get_Rev_Data = &Get_RevY_Data_TypeX_PDV121;
	}
	else if (ComboBoxRevYVer->Text == "X-PD-V12.2") {
		ap->Get_Rev_Data = &Get_RevY_Data_TypeX_PDV122;
	}
	else if (ComboBoxRevYVer->Text == "15M-X-HS-REV") {
		ap->Get_Rev_Data = &Get_RevY_Data_TypeX_PTV140;
	}
	else if (ComboBoxRevYVer->Text == "X-21.5-V14.2" ||
		ComboBoxRevYVer->Text == "E24-V14.6") {
		ap->Get_Rev_Data = &Get_RevY_Data_TypeX_PTV142;
	}
	else if (ComboBoxRevYVer->Text == "E19-V14.3" ||
		ComboBoxRevYVer->Text == "E15-V14.4") {
		ap->Get_Rev_Data = &Get_RevY_Data_TypeX_PTV143;
	}
	else if (ComboBoxRevYVer->Text == "E20.1-V14.5") {
		ap->Get_Rev_Data = &Get_RevY_Data_TypeX_PTV145;
	}
	else if (ComboBoxRevYVer->Text == "E12.1-V14.7") {
		ap->Get_Rev_Data = &Get_RevY_Data_TypeX_PTV147;
	}
	else if (ComboBoxRevYVer->Text == "E15-FPC-V14.8") {
		ap->Get_Rev_Data = &Get_RevY_Data_TypeX_PTV148;
	}
	else if (ComboBoxRevYVer->Text == "X-PT-V14.1") {
		ap->Get_Rev_Data = &Get_RevY_Data_TypeX_PTV141;
	}
	else if (ComboBoxRevYVer->Text == "fct") {
		ap->Get_Rev_Data = &Get_RevY_Data_FCT;
	}
	else {
		if (CheckBoxCPLDMagicConfig->Checked == false) {
			ap->Get_Rev_Data = &Get_Rev_Data_V46;
		}
		else {
			ap->Get_Rev_Data = &Get_Rev_Data_V46_MagicConfig;
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::TimerDownTabTimer(TObject *Sender) {
	if (MainForm->HIDSendThread->Sta == IDLE) {
		EnableButtons();
		TimerDownTab->Enabled = false;

		if (MainForm->HIDSendThread->Cancle == false) {
			StatusBar1->SimpleText = L"下载完成!";
			ShowMessage(L"下载完成!");
		}
		else {
			StatusBar1->SimpleText = L"下载失败!";
			MessageBox(NULL, L"失败", L"下载失败", MB_OK | MB_ICONERROR);
		}

		MainForm->HIDSendThread->DeviceRun();
	}
	else {
		int Percent = 0;
		String Msg = L"下载中，请稍候...";

		if (MainForm->HIDSendThread->Len) {
			Percent =
				MainForm->HIDSendThread->SendLen * 100 /
				MainForm->HIDSendThread->Len;
		}

		Msg += String(Percent) + "%";

		StatusBar1->SimpleText = Msg;
	}
}

// ---------------------------------------------------------------------------

void __fastcall TFormConfig::StringGrid1DrawCell(TObject *Sender, int ACol,
	int ARow, TRect &Rect, TGridDrawState State) {
	// ((TStringGrid*)Sender)->Canvas->FillRect(Rect); // 使用当前的brush填充矩形，覆盖目前cell中的内容显示 6

	((TStringGrid*)Sender)->Canvas->Brush->Style = bsSolid;
	((TStringGrid*)Sender)->Canvas->FillRect(Rect);
	// 使用当前的brush填充矩形，覆盖目前cell中的内容显示 6
	((TStringGrid*)Sender)->Canvas->Brush->Style = bsClear;

	if (ACol > 0 && ARow > 0) {
		((TStringGrid*)Sender)->Canvas->Font->Color = (TColor)clWebDarkRed;
	}
	else {
		((TStringGrid*)Sender)->Canvas->Pen->Color = clBlack;
	}

	DrawText(((TStringGrid*)Sender)->Canvas->Handle,
		((TStringGrid*)Sender)->Cells[ACol][ARow].c_str(), -1, (RECT*)&Rect,
		DT_SINGLELINE | DT_VCENTER | DT_CENTER);
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::ButtonDownParamClick(TObject *Sender) {
	// uint32_t ParamBaseAdd;
	FLASH_PARAM *fp = &FormUpdate->FlashParam;

	memset(&Param, 0, sizeof(Param));

	Param.Frames = _wtoi(EditFrame->Text.c_str());
	Param.LowFrames = _wtoi(EditLowFrame->Text.c_str());

	Param.TOn = _wtoi(EditTOn->Text.c_str());
	Param.TOff = _wtoi(EditTOff->Text.c_str());
	Param.AdTrig = _wtoi(EditADTrig->Text.c_str());
	Param.TimeOut = _wtoi(EditLowFrameTimeout->Text.c_str());
	Param.TRise = _wtoi(EditTRise->Text.c_str());

	for (int i = 0; i < StringGrid1->ColCount - 1; i += 1) {
		uint8_t Gain1 = _wtoi(StringGrid1->Cells[i + 1][1].c_str());
		uint8_t Gain2 = _wtoi(StringGrid1->Cells[i + 1][2].c_str());
		uint8_t EmitPower = _wtoi(StringGrid1->Cells[i + 1][3].c_str());
		int Len;
		String Msg = "";

		Msg.sprintf(L"编号为%d的板子：\n", i + 1);
		Len = Msg.Length();

		if (Gain1 > 7) {
			Msg += ("增益1越限[0,7]\n");
		}

		if (Gain2 > 3) {
			Msg += ("增益2越限[0,3]\n");
		}

		if (this->BootParam.HardwareVer == 540 && EmitPower > 7) {
			Msg += ("发射功率越限[0,7]\n");
		}

		if (Msg.Length() > Len) {
			ShowMessage(Msg);
			return;
		}

		if (i >= 45) {
			Param.Gain1_60[i - 45] |= Gain1 << 4;
			Param.Gain2_60[i - 45] |= Gain2 << 4;
			if (this->BootParam.HardwareVer == 540) {
				Param.EmitPow_60[i - 45] |= EmitPower << 4;
			}
		}
		else if (i >= 30) {
			Param.Gain1_60[i - 30] |= Gain1;
			Param.Gain2_60[i - 30] |= Gain2;
			if (this->BootParam.HardwareVer == 540) {
				Param.EmitPow_60[i - 30] |= EmitPower;
			}
			else {
				Param.EmitPow_60[i - 30] = EmitPower;
			}
		}
		else if (i >= 15) {
			Param.Gain1[i - 15] |= Gain1 << 4;
			Param.Gain2[i - 15] |= Gain2 << 4;
			if (this->BootParam.HardwareVer == 540) {
				Param.EmitPow[i - 15] |= EmitPower << 4;
			}
		}
		else {
			Param.Gain1[i] |= Gain1;
			Param.Gain2[i] |= Gain2;
			if (this->BootParam.HardwareVer == 540) {
				Param.EmitPow[i] |= EmitPower;
			}
			else {
				Param.EmitPow[i] = EmitPower;
			}
		}
	}

	Param.gesturestart = _wtoi(EditGestureStart->Text.c_str());
	Param.gestureend = _wtoi(EditGestureEnd->Text.c_str());

	String Msg = "";

	if (CheckBoxFeatureGPIOGesture->Checked || CheckBoxFeatureUSARTOn->Checked)
	{
		if (Param.gesturestart < 1 || Param.gesturestart > 10) {
			Msg.cat_printf(L"手势起始高度超出范围 [1, 10]\n");
		}

		if (Param.gestureend < 3 || Param.gestureend > 50) {
			Msg.cat_printf(L"手势终止高度超出范围 [3, 50]\n");
		}

		if (Param.gesturestart >= Param.gestureend) {
			Msg.cat_printf(L"手势终止高度应该大于手势起始高度\n");
		}
	}

	Param.tracepoints = _wtoi(EditTracePoints->Text.c_str());

	// if(Param.tracepoints < 1){
	// Msg.cat_printf("最大上报点数需大于0\n");
	// }

	Param.worktime = _wtoi(EditWorkTime->Text.c_str());
	if (Param.worktime > 2000) {
		Msg.cat_printf(L"工作时间太长！\n");
	}

	if (Msg.Length()) {
		ShowMessage(Msg);
		return;
	}

	// send it
	memset(ParamBuf, 0xFF, sizeof(ParamBuf));
	for (int i = 0; i < sizeof(ParamBuf) / 2; i += 1) {
		((uint16_t*)ParamBuf)[i] = rand();
	}

	Param.Feature = FEATURE_STD;

	for (int i = 0; i < FeatureCheckBox->Count; i += 1) {
		if (((TCheckBox*)FeatureCheckBox->Items[i])->Checked == true) {
			Param.Feature |= (1 << (i + 1));
		}
	}

	Param.Feature &= ~(0x0F << 10);

	if (CheckBoxFeatureUSARTOn->Checked == true)
		Param.Feature |= ((ComboBoxUartReportType->ItemIndex & 0x0F) << 10);

	if (sizeof(ParamBuf) < sizeof(Param) + 4 || sizeof(ParamBuf) <
		fp->ParamSize) // 4bytes for crc32
	{
		ShowMessage(L"参数太多，需要修改程序！");
		return;
	}

	memcpy(ParamBuf, &Param, sizeof(Param));

	((uint32_t*)(ParamBuf + fp->ParamSize - 4))[0] =
		FormUpdate->CRC32((uint32_t*)ParamBuf, fp->ParamSize / 4 - 1);

	Head.MemType = TYPE_PARAM;
	Head.Mode = MODE_PROGRAM;
	Head.StartAdd = fp->ParamStartAdd;

	if (MainForm->HIDSendThread->Connected == FALSE) {
		ShowMessage(L"设备未连接!");
		return;
	}

	if (MainForm->HIDSendThread->SubmitTask(TYPE_APP, fp->ParamSize, ParamBuf,
		&Head)) {
		ButtonDownTab->Enabled = false;
		ButtonDownParam->Enabled = false;
		TimerDownTab->Enabled = true;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormConfig::Button8Click(TObject *Sender) {
	TIniFile *ini;

	if (MessageBox(NULL, L"确定删除该配置?\n扫描表和参数配置将同时被删除!", L"删除配置",
		MB_OKCANCEL | MB_ICONWARNING) == IDCANCEL) {
		return;
	}

	ini = new TIniFile(ExtractFilePath(Application->ExeName) + "screen.ini");
	ini->EraseSection(ComboBoxStdScreen->Text);

	ComboBoxStdScreen->Clear();
	ini->ReadSections(ComboBoxStdScreen->Items);
	ComboBoxStdScreen->ItemIndex = 0;

	ComboBoxStdParam->Clear();
	ini->ReadSections(ComboBoxStdParam->Items);
	ComboBoxStdParam->ItemIndex = 0;

	delete ini;
}

// ---------------------------------------------------------------------------
void __fastcall TFormConfig::SaveScanTableConfig(TIniFile *ini, String Section)
{
	int Axis;

	// ------------------------------- X Configration ---------------------------
	Axis = _wtoi(ComboBoxXAxis->Text.c_str());

	ini->WriteString(Section, "XLeds", EditXLeds->Text);
	ini->WriteString(Section, "XBoardLed", ComboBoxXBoardLeds->Text);
	ini->WriteString(Section, "XLedDist", EditXDist->Text);
	ini->WriteString(Section, "XMargin", EditXMargin->Text);
	ini->WriteString(Section, "XMarginRight", EditXMarginRight->Text);
	ini->WriteString(Section, "XAxis", ComboBoxXAxis->Text);
	ini->WriteString(Section, "XEmitPower", ComboBoxXPower->Text);
	ini->WriteString(Section, "XOffsetDeg", EditXoffset->Text);
	ini->WriteString(Section, "XBoards", ComboBoxXBoards->Text);
	ini->WriteString(Section, "XTail1Leds", EditXtail1->Text);
	ini->WriteString(Section, "XTail2Leds", EditXtail2->Text);
	ini->WriteString(Section, "XTail3Leds", EditXtail3->Text);
	ini->WriteString(Section, "XRevPowerGroup", EditXMiniOff->Text);
	ini->WriteString(Section, "XStartJumpLed", EditXJump->Text);
	ini->WriteString(Section, "EmitXVer", ComboBoxEmitXVer->Text);
	ini->WriteString(Section, "RevXVer", ComboBoxRevXVer->Text);
	ini->WriteString(Section, "XScanGap", EditXScanGap->Text);

	for (int i = 0; i < Axis; i += 1) {
		String Msg;

		Msg.sprintf(L"X_ExpectedDeg_Off%d", i + 1);
		ini->WriteString(Section, Msg, ((TEdit *)XParamEdit->Items[i])->Text);

		Msg.sprintf(L"X_Gain1_Off%d", i + 1);
		ini->WriteString(Section, Msg,
			((TComboBox *)XParamEdit->Items[i + MAX_AXIS * 3])->Text);

		Msg.sprintf(L"X_Gain2_Off%d", i + 1);
		ini->WriteString(Section, Msg,
			((TComboBox *)XParamEdit->Items[i + MAX_AXIS * 4])->Text);

		Msg.sprintf(L"X_Corner_LedOff%d", i + 1);
		ini->WriteString(Section, Msg,
			((TComboBox *)XParamEdit->Items[i + MAX_AXIS * 5])->Text);
	}

	int Boards = _wtoi(ComboBoxXBoards->Text.c_str());
	int Cuts = XCutBoards->Count / 2;
	String Keys;

	for (int i = 0; i < Boards && i < Cuts; i += 1) {
		Keys.printf(L"XTail%dLeds", i + 1);
		ini->WriteString(Section, "XCutBoard" + String(i + 1),
			((TComboBox*)XCutBoards->Items[i])->Text);
		ini->WriteString(Section, Keys,
			((TEdit*)XCutBoards->Items[i + Cuts])->Text);
	}

	ini->WriteBool(Section, "XCutUnused", CheckBox17->Checked);
	ini->WriteString(Section, "XBoardLedTable", EditXBoardLed->Text);

	ini->WriteString(Section, "XDivMethod", ComboBoxXDivMethod->Text);

	// ------------------------------- Y Configration ---------------------------
	Axis = _wtoi(ComboBoxYAxis->Text.c_str());

	ini->WriteString(Section, "YLeds", EditYLeds->Text);
	ini->WriteString(Section, "YBoardLed", ComboBoxYBoardLeds->Text);
	ini->WriteString(Section, "YLedDist", EditYDist->Text);
	ini->WriteString(Section, "YMargin", EditYMargin->Text);
	ini->WriteString(Section, "YMarginDown", EditYMarginDown->Text);
	ini->WriteString(Section, "XMarginRight", EditXMarginRight->Text);
	ini->WriteString(Section, "YAxis", ComboBoxYAxis->Text);
	ini->WriteString(Section, "YEmitPower", ComboBoxYPower->Text);
	ini->WriteString(Section, "YOffsetDeg", EditYoffset->Text);
	ini->WriteString(Section, "YBoards", ComboBoxYBoards->Text);
	ini->WriteString(Section, "YTail1Leds", EditYtail1->Text);
	ini->WriteString(Section, "YTail2Leds", EditYtail2->Text);
	ini->WriteString(Section, "YTail3Leds", EditYtail3->Text);
	ini->WriteString(Section, "YRevPowerGroup", EditYMiniOff->Text);
	ini->WriteString(Section, "YStartJumpLed", EditYJump->Text);
	ini->WriteString(Section, "EmitYVer", ComboBoxEmitYVer->Text);
	ini->WriteString(Section, "RevYVer", ComboBoxRevYVer->Text);
	ini->WriteString(Section, "YScanGap", EditYScanGap->Text);

	for (int i = 0; i < Axis; i += 1) {
		String Msg;

		Msg.sprintf(L"Y_ExpectedDeg_Off%d", i + 1);
		ini->WriteString(Section, Msg, ((TEdit *)YParamEdit->Items[i])->Text);

		Msg.sprintf(L"Y_Gain1_Off%d", i + 1);
		ini->WriteString(Section, Msg,
			((TComboBox *)YParamEdit->Items[i + MAX_AXIS * 3])->Text);

		Msg.sprintf(L"Y_Gain2_Off%d", i + 1);
		ini->WriteString(Section, Msg,
			((TComboBox *)YParamEdit->Items[i + MAX_AXIS * 4])->Text);

		Msg.sprintf(L"Y_Corner_LedOff%d", i + 1);
		ini->WriteString(Section, Msg,
			((TComboBox *)YParamEdit->Items[i + MAX_AXIS * 5])->Text);
	}

	Boards = _wtoi(ComboBoxYBoards->Text.c_str());
	Cuts = YCutBoards->Count / 2;

	for (int i = 0; i < Boards && i < Cuts; i += 1) {
		Keys.printf(L"YTail%dLeds", i + 1);
		ini->WriteString(Section, "YCutBoard" + String(i + 1),
			((TComboBox*)YCutBoards->Items[i])->Text);
		ini->WriteString(Section, Keys,
			((TEdit*)YCutBoards->Items[i + Cuts])->Text);
	}

	ini->WriteBool(Section, "YCutUnused", CheckBox19->Checked);
	ini->WriteString(Section, "YBoardLedTable", EditYBoardLed->Text);

	ini->WriteString(Section, "YDivMethod", ComboBoxYDivMethod->Text);

	ini->WriteBool(Section, "SkipFirstAxis", CheckBoxSkipFirstAxis->Checked);
	ini->WriteBool(Section, "DualSPIData", CheckBoxDualSPIData->Checked);
	ini->WriteBool(Section, "SwitchEmit", CheckBoxSwitchEmit->Checked);
	ini->WriteBool(Section, "ShutdownEmit", CheckBoxShutDownEmit->Checked);
	ini->WriteBool(Section, "SPI8Bit", CheckBox8BitSPI->Checked);
	ini->WriteBool(Section, "GPIOGain", CheckBoxGPIOGain->Checked);
	ini->WriteBool(Section, "ReverseXScan", CheckBoxReverseXScan->Checked);
	ini->WriteBool(Section, "ReverseYScan", CheckBoxReverseYScan->Checked);
	ini->WriteBool(Section, "DoubleFirst", CheckBoxDoubleFirst->Checked);
	ini->WriteBool(Section, "AntiGlare", CheckBoxAntiGlare->Checked);
	ini->WriteBool(Section, "CPLDMagicConfig",
		CheckBoxCPLDMagicConfig->Checked);
	ini->WriteBool(Section, "SwitchEmitRev", CheckBoxSwEmitRev->Checked);
	ini->WriteBool(Section, "GPIOSwitch", CheckBoxGPIOSwitch->Checked);
	ini->WriteBool(Section, "SkipXEnd", CheckBoxSkipX->Checked);
	ini->WriteBool(Section, "EmitNotD7", CheckBoxNotD7->Checked);
	ini->WriteBool(Section, "WaitRxPowerOn", CheckBoxWaitRxPowerOn->Checked);

	ini->WriteString(Section, "GlareLever", ComboBoxGlareLever->Text);

}

void __fastcall TFormConfig::SaveParamConfig(TIniFile *ini, String Section) {
	String Gain1 = "", Gain2 = "", EmitPow = "";

	ini->WriteString(Section, "Frames", EditFrame->Text);
	ini->WriteString(Section, "LowFrames", EditLowFrame->Text);
	ini->WriteString(Section, "LowFrameTimeout", EditLowFrameTimeout->Text);
	ini->WriteString(Section, "TOn", EditTOn->Text);
	ini->WriteString(Section, "TOff", EditTOff->Text);
	ini->WriteString(Section, "ADTrigglePoint", EditADTrig->Text);
	ini->WriteString(Section, "TRise", EditTRise->Text);

	for (int i = 1; i < StringGrid1->ColCount; i += 1) {
		Gain1 += StringGrid1->Cells[i][1];
		Gain2 += StringGrid1->Cells[i][2];
		EmitPow += StringGrid1->Cells[i][3];
	}

	ini->WriteString(Section, "Gain1", Gain1);
	ini->WriteString(Section, "Gain2", Gain2);
	ini->WriteString(Section, "EmitPow", EmitPow);

	ini->WriteBool(Section, "FeatureSmooth", CheckBoxFeatureSmooth->Checked);
	ini->WriteBool(Section, "FeatureInterplation",
		CheckBoxFeatureInterplation->Checked);
	ini->WriteBool(Section, "FeatureMac", CheckBoxFeatureMac->Checked);
	ini->WriteBool(Section, "FeatureReverseX",
		CheckBoxFeatureReverseX->Checked);
	ini->WriteBool(Section, "FeatureReverseY",
		CheckBoxFeatureReverseY->Checked);
	ini->WriteBool(Section, "FeatureExchangeXY",
		CheckBoxFeatureExchangeXY->Checked);
	ini->WriteBool(Section, "FeatureUSARTOn", CheckBoxFeatureUSARTOn->Checked);
	ini->WriteBool(Section, "FeatureSmallPointOn",
		CheckBoxFeatureSmallPointOn->Checked);
	ini->WriteBool(Section, "FeatureFourceMouse",
		CheckBoxFeatureFourceMouse->Checked);
	// ini->WriteBool(Section, "FeatureProtocolFmt3", CheckBoxFeatureLinkSeeProtocol->Checked);
	// ini->WriteBool(Section, "FeatureGesture", CheckBoxFeatureSvaGesture->Checked);
	ini->WriteBool(Section, "FeatureBoardPoint",
		CheckBoxFeatureBoardPoint->Checked);
	ini->WriteBool(Section, "FeatureGainAdj", CheckBoxFeatureGainAdj->Checked);
	ini->WriteBool(Section, "FeatureEdgeAdsorption",
		CheckBoxFeatureEdgeAdsorption->Checked);
	ini->WriteBool(Section, "FeatureGPIOGesture",
		CheckBoxFeatureGPIOGesture->Checked);
	ini->WriteBool(Section, "FeatureDisableRightClick",
		CheckBoxFeatureDisableRightClick->Checked);
	ini->WriteBool(Section, "CheckBoxD2CurHigh", CheckBoxD2CurHigh->Checked);
	ini->WriteBool(Section, "FeatureAutoGainSub",
		CheckBoxFeatureAutoGainSub->Checked);
	ini->WriteBool(Section, "FeatureAutoGainUniform",
		CheckBoxFeatureAutoGainUniform->Checked);
	ini->WriteString(Section, "GestureStart", EditGestureStart->Text);
	ini->WriteString(Section, "GestureEnd", EditGestureEnd->Text);
}

void __fastcall TFormConfig::AddStdConfig(TIniFile *ini, String Section) {
	if (PageControl1->ActivePage == TabSheetScan) {
		SaveScanTableConfig(ini, Section);

		ComboBoxStdScreen->Clear();
		ini->ReadSections(ComboBoxStdScreen->Items);
		ComboBoxStdScreen->Text = Section;
	}
	else if (PageControl1->ActivePage == TabSheetParam) {
		SaveParamConfig(ini, Section);

		ComboBoxStdParam->Clear();
		ini->ReadSections(ComboBoxStdParam->Items);
		ComboBoxStdParam->Text = Section;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::Button5Click(TObject *Sender) {
	if (PageControl1->ActivePage == TabSheetScan) {
		FormAddStdParam->EditName->Text = ComboBoxStdScreen->Text;
	}
	else if (PageControl1->ActivePage == TabSheetParam) {
		FormAddStdParam->EditName->Text = ComboBoxStdParam->Text;
	}

	FormAddStdParam->ShowModal();
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::ComboBoxStdScreenChange(TObject *Sender) {
	TIniFile *ini = new TIniFile(ExtractFilePath(Application->ExeName) +
		"screen.ini");
	String Section = ComboBoxStdScreen->Text;
	int Axis;
	String Msg;

	if (Section.Length() == 0)
		return;
	if (!ini)
		return;

	// ------------------------------- X Configration ---------------------------
	Axis = ini->ReadInteger(Section, "XAxis", 3);

	for (int i = 0; i < Axis; i += 1) {
		Msg.sprintf(L"X_ExpectedDeg_Off%d", i + 1);
		((TEdit*)XParamEdit->Items[i])->Text =
			ini->ReadString(Section, Msg, "0");

		Msg.sprintf(L"X_Gain1_Off%d", i + 1);
		((TComboBox*)XParamEdit->Items[i + MAX_AXIS * 3])->Text =
			ini->ReadString(Section, Msg, "2");

		Msg.sprintf(L"X_Gain2_Off%d", i + 1);
		((TComboBox*)XParamEdit->Items[i + MAX_AXIS * 4])->Text =
			ini->ReadString(Section, Msg, "2");

		Msg.sprintf(L"X_Corner_LedOff%d", i + 1);
		((TEdit*)XParamEdit->Items[i + MAX_AXIS * 5])->Text =
			ini->ReadString(Section, Msg, "0");
	}

	// ComboBoxXBoardLeds->Text = ini->ReadString(Section, "XBoardLed", "69");
	Msg = ini->ReadString(Section, "XBoardLed", "69");
	ComboBoxXBoardLeds->ItemIndex = ComboBoxXBoardLeds->Items->IndexOf(Msg);

	EditXDist->Text = ini->ReadString(Section, "XLedDist", "5.960");
	EditXMargin->Text = ini->ReadString(Section, "XMargin", "8");
	EditXMarginRight->Text = ini->ReadString(Section, "XMarginRight",
		EditXMargin->Text);
	// ComboBoxXPower->Text = ini->ReadString(Section, "XEmitPower", "3");
	Msg = ini->ReadString(Section, "XEmitPower", "3");
	ComboBoxXPower->ItemIndex = ComboBoxXPower->Items->IndexOf(Msg);

	EditXoffset->Text = ini->ReadString(Section, "XOffsetDeg", "20");
	// ComboBoxXBoards->Text = ini->ReadString(Section, "XBoards", "1");
	Msg = ini->ReadString(Section, "XBoards", "1");
	ComboBoxXBoards->ItemIndex = ComboBoxXBoards->Items->IndexOf(Msg);
	ComboBoxXBoardsChange(ComboBoxXBoards);

	int Boards = _wtoi(ComboBoxXBoards->Text.c_str());
	int Cuts = XCutBoards->Count / 2;
	String Keys;

	for (int i = 0; i < Boards && i < Cuts; i += 1) {
		Keys.printf(L"XTail%dLeds", i + 1);
		Msg = ini->ReadString(Section, "XCutBoard" + String(i + 1),
			String(Boards - i));
		((TComboBox*)XCutBoards->Items[i])->ItemIndex =
			((TComboBox*)XCutBoards->Items[i])->Items->IndexOf(Msg);
		((TEdit*)XCutBoards->Items[i + Cuts])->Text =
			ini->ReadString(Section, Keys, ComboBoxXBoardLeds->Text);
	}

	CheckBox17->Checked = ini->ReadBool(Section, "XCutUnused", false);
	if (CheckBox17->Checked) {
		EditXBoardLed->Text = ini->ReadString(Section, "XBoardLedTable", "0");
		EditXBoardLedChange(NULL);

	}
	// ComboBoxXAxis->Text = ini->ReadString(Section, "XAxis", "3");
	// ComboBoxEmitXVer->Text = ini->ReadString(Section, "EmitXVer", "V4.6");
	// ComboBoxRevXVer->Text = ini->ReadString(Section, "RevXVer", "PD-V4.6");

	Msg = ini->ReadString(Section, "XAxis", "3");
	ComboBoxXAxis->ItemIndex = ComboBoxXAxis->Items->IndexOf(Msg);
	Msg = ini->ReadString(Section, "EmitXVer", "V4.6");
	ComboBoxEmitXVer->ItemIndex = ComboBoxEmitXVer->Items->IndexOf(Msg);
	Msg = ini->ReadString(Section, "RevXVer", "PD-V4.6");
	ComboBoxRevXVer->ItemIndex = ComboBoxRevXVer->Items->IndexOf(Msg);

	EditXMiniOff->Text = ini->ReadString(Section, "XRevPowerGroup", "1");
	EditXJump->Text = ini->ReadString(Section, "XStartJumpLed", "0");
	EditXScanGap->Text = ini->ReadString(Section, "XScanGap", "0");

	ComboBoxEmitXVer->ItemIndex = ComboBoxEmitXVer->Items->IndexOf
		(ComboBoxEmitXVer->Text);
	ComboBoxRevXVer->ItemIndex = ComboBoxRevXVer->Items->IndexOf
		(ComboBoxRevXVer->Text);

	// ComboBoxXDivMethod->Text = ini->ReadString(Section, "XDivMethod", "角度均匀分配");
	Msg = ini->ReadString(Section, "XDivMethod", "角度均匀分配");
	ComboBoxXDivMethod->ItemIndex = ComboBoxXDivMethod->Items->IndexOf(Msg);

	ComboBoxXAxisChange(NULL);
	// ------------------------------- Y Configration ---------------------------
	Axis = ini->ReadInteger(Section, "YAxis", 3);

	for (int i = 0; i < Axis; i += 1) {
		Msg.sprintf(L"Y_ExpectedDeg_Off%d", i + 1);
		((TEdit*)YParamEdit->Items[i])->Text =
			ini->ReadString(Section, Msg, "0");

		Msg.sprintf(L"Y_Gain1_Off%d", i + 1);
		((TComboBox*)YParamEdit->Items[i + MAX_AXIS * 3])->Text =
			ini->ReadString(Section, Msg, "2");

		Msg.sprintf(L"Y_Gain2_Off%d", i + 1);
		((TComboBox*)YParamEdit->Items[i + MAX_AXIS * 4])->Text =
			ini->ReadString(Section, Msg, "2");

		Msg.sprintf(L"Y_Corner_LedOff%d", i + 1);
		((TEdit*)YParamEdit->Items[i + MAX_AXIS * 5])->Text =
			ini->ReadString(Section, Msg, "0");
	}

	// ComboBoxYBoardLeds->Text = ini->ReadString(Section, "YBoardLed", "69");
	Msg = ini->ReadString(Section, "YBoardLed", "69");
	ComboBoxYBoardLeds->ItemIndex = ComboBoxYBoardLeds->Items->IndexOf(Msg);
	EditYDist->Text = ini->ReadString(Section, "YLedDist", "5.960");
	EditYMargin->Text = ini->ReadString(Section, "YMargin", "8");
	EditYMarginDown->Text = ini->ReadString(Section, "YMarginDown",
		EditYMargin->Text);
	// ComboBoxYPower->Text = ini->ReadString(Section, "YEmitPower", "3");
	Msg = ini->ReadString(Section, "YEmitPower", "3");
	ComboBoxYPower->ItemIndex = ComboBoxYPower->Items->IndexOf(Msg);
	EditYoffset->Text = ini->ReadString(Section, "YOffsetDeg", "20");
	// ComboBoxYBoards->Text = ini->ReadString(Section, "YBoards", "1");
	Msg = ini->ReadString(Section, "YBoards", "1");
	ComboBoxYBoards->ItemIndex = ComboBoxYBoards->Items->IndexOf(Msg);
	ComboBoxYBoardsChange(ComboBoxYBoards);

	Boards = _wtoi(ComboBoxYBoards->Text.c_str());
	Cuts = YCutBoards->Count / 2;

	for (int i = 0; i < Boards && i < Cuts; i += 1) {
		Keys.printf(L"YTail%dLeds", i + 1);
		// ((TComboBox*)YCutBoards->Items[i])->Text = ini->ReadString(Section, "YCutBoard" + String(i+1), String(Boards-i));
		Msg = ini->ReadString(Section, "YCutBoard" + String(i + 1),
			String(Boards - i));
		((TComboBox*)YCutBoards->Items[i])->ItemIndex =
			((TComboBox*)YCutBoards->Items[i])->Items->IndexOf(Msg);
		((TEdit*)YCutBoards->Items[i + Cuts])->Text =
			ini->ReadString(Section, Keys, ComboBoxYBoardLeds->Text);
	}

	CheckBox19->Checked = ini->ReadBool(Section, "YCutUnused", false);
	if (CheckBox19->Checked) {
		EditYBoardLed->Text = ini->ReadString(Section, "YBoardLedTable", "0");
		EditYBoardLedChange(NULL);
	}

	// ComboBoxYAxis->Text = ini->ReadString(Section, "YAxis", "3");
	// ComboBoxEmitYVer->Text = ini->ReadString(Section, "EmitYVer", "V4.6");
	// ComboBoxRevYVer->Text = ini->ReadString(Section, "RevYVer", "PD-V4.6");
	Msg = ini->ReadString(Section, "YAxis", "3");
	ComboBoxYAxis->ItemIndex = ComboBoxYAxis->Items->IndexOf(Msg);
	Msg = ini->ReadString(Section, "EmitYVer", "V4.6");
	ComboBoxEmitYVer->ItemIndex = ComboBoxEmitYVer->Items->IndexOf(Msg);
	Msg = ini->ReadString(Section, "RevYVer", "PD-V4.6");
	ComboBoxRevYVer->ItemIndex = ComboBoxRevYVer->Items->IndexOf(Msg);

	EditYMiniOff->Text = ini->ReadString(Section, "YRevPowerGroup", "1");
	EditYJump->Text = ini->ReadString(Section, "YStartJumpLed", "0");
	EditYScanGap->Text = ini->ReadString(Section, "YScanGap", "0");

	ComboBoxEmitYVer->ItemIndex = ComboBoxEmitYVer->Items->IndexOf
		(ComboBoxEmitYVer->Text);
	ComboBoxRevYVer->ItemIndex = ComboBoxRevYVer->Items->IndexOf
		(ComboBoxRevYVer->Text);

	// ComboBoxYDivMethod->Text = ini->ReadString(Section, "YDivMethod", "角度均匀分配");
	Msg = ini->ReadString(Section, "YDivMethod", "角度均匀分配");
	ComboBoxYDivMethod->ItemIndex = ComboBoxYDivMethod->Items->IndexOf(Msg);

	ComboBoxYAxisChange(NULL);

	CheckBoxSkipFirstAxis->Checked =
		ini->ReadBool(Section, "SkipFirstAxis", true);
	CheckBoxDualSPIData->Checked = ini->ReadBool(Section, "DualSPIData", true);
	CheckBoxSwitchEmit->Checked = ini->ReadBool(Section, "SwitchEmit", true);
	CheckBoxShutDownEmit->Checked =
		ini->ReadBool(Section, "ShutdownEmit", false);
	CheckBox8BitSPI->Checked = ini->ReadBool(Section, "SPI8Bit", false);
	CheckBoxGPIOGain->Checked = ini->ReadBool(Section, "GPIOGain", false);
	CheckBoxReverseXScan->Checked =
		ini->ReadBool(Section, "ReverseXScan", false);
	CheckBoxReverseYScan->Checked =
		ini->ReadBool(Section, "ReverseYScan", false);
	CheckBoxDoubleFirst->Checked = ini->ReadBool(Section, "DoubleFirst", false);
	CheckBoxAntiGlare->Checked = ini->ReadBool(Section, "AntiGlare", false);
	CheckBoxCPLDMagicConfig->Checked =
		ini->ReadBool(Section, "CPLDMagicConfig", false);
	CheckBoxSwEmitRev->Checked = ini->ReadBool(Section, "SwitchEmitRev", false);
	CheckBoxGPIOSwitch->Checked = ini->ReadBool(Section, "GPIOSwitch", false);
	CheckBoxNotD7->Checked = ini->ReadBool(Section, "EmitNotD7", false);
	CheckBoxWaitRxPowerOn->Checked =
		ini->ReadBool(Section, "WaitRxPowerOn", false);
	CheckBoxSkipX->Checked = ini->ReadBool(Section, "SkipXEnd", false);

	// ComboBoxGlareLever->Text = ini->ReadString(Section, "GlaveLever", "3");
	Msg = ini->ReadString(Section, "GlaveLever", "3");
	ComboBoxGlareLever->ItemIndex = ComboBoxGlareLever->Items->IndexOf(Msg);

	delete ini;

	Button4Click(NULL);
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::ButtonSwitchClick(TObject *Sender) {
	MainForm->HIDSendThread->SubmitCmd(BOOT_CMD_SW_MODE, NULL, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::ButtonGetStaClick(TObject *Sender) {
	Button15Click(NULL);

	MainForm->HIDSendThread->GetDeviceSta();
	MainForm->HIDSendThread->SubmitCmd(BOOT_CMD_READ_PARAM, NULL, 0);
}

// ---------------------------------------------------------------------------
void __fastcall TFormConfig::DispCPUID(uint8_t *Data, uint16_t Size) {
	String Msg = "";

	for (int i = 0; i < 12; i += 1) {
		CPUID[i] = Data[i];
		Msg.cat_printf(L"%.2X", Data[i]);
	}

	EditCPUID->Text = Msg;
}

void __fastcall TFormConfig::DispSta(void) {
	String Msg;
	const String RstTypes[] = {"上电", "看门狗", "指令", "低电压"};

	Msg.sprintf(L"%d.%d", (BootParam.BootLoaderVersion & 0x7F) / 10,
		(BootParam.BootLoaderVersion & 0x7F) % 10);

	if ((BootParam.BootLoaderVersion & 0x7F) < 60) {
		if (BootParam.BootLoaderVersion & 0x80) {
			Msg += "-10";
		}
		else {
			Msg += "-6";
		}
	}
	else if ((BootParam.BootLoaderVersion & 0x7F) < 80) {
		Msg += "-F3";
	}
	else {
		Msg += "-F4";
	}

	LabelBoot->Caption = "Boot版本：" + Msg;
	EditBootVer->Text = Msg;

	if (BootParam.HardwareVer != 0xFFFF) {
		Msg.sprintf(L"%d.%d", BootParam.HardwareVer / 100,
			BootParam.HardwareVer % 100);
	}
	else {
		Msg.sprintf(L"N/A");
	}
	LabelHWVer->Caption = "硬件版本：" + Msg;
	EditHardwareVer->Text = Msg;

	if (BootParam.SoftwareVer != 0xFFFF) {
		Msg.sprintf(L"%d", BootParam.SoftwareVer / 1000);
		Msg.cat_printf(L".%d", BootParam.SoftwareVer % 1000 / 10);
		Msg.cat_printf(L".%d", BootParam.SoftwareVer % 10);
	}
	else {
		Msg.sprintf(L"N/A");
	}
	LabelFirmwareVer->Caption = "固件版本：" + Msg;
	EditSoftwareVer->Text = Msg;

	switch (BootParam.AppSta) {
	case APP_OK:
		Msg = "正常";
		break;

	case APP_EMPTY:
		Msg = "空白";
		break;

	case APP_CORRUPT:
		Msg = "损坏";
		break;

	default:
		Msg = "未知";
		break;
	}
	LabelFwSta->Caption = "固件状态：" + Msg;
	Edit75->Text = Msg;

	Msg = "复位原因：";
	if (BootParam.ResetType < 4) {
		Msg += RstTypes[BootParam.ResetType];
	}
	else {
		Msg += "未知";
	}
	LabelResetReson->Caption = Msg;

	LabelRstTimes->Caption = "复位次数：" + String(BootParam.WDTRstTimes);
	LabelLastRunTime->Caption = "上次运行时间：" + String(BootParam.LastRstTime);
	LabelRunTime->Caption = "本次运行时间：" + String(BootParam.RunTime);

	if ((BootParam.RunSta & RUN_STA_MODE_MASK) == RUM_MODE_APP) {
		Msg = "正常运行";

		if (BootParam.RunSta & RUN_STA_PWR_ERR) {
			Msg = "供电不足";
		}
		else if (BootParam.RunSta & RUN_STA_PARAM_ERR) {
			Msg = "参数异常";
		}
		else if (BootParam.RunSta & RUN_STA_SCAN_ERR) {
			Msg = "扫描表异常";
		}
		else if (BootParam.RunSta & RUN_STA_MEM_ERR) {
			Msg = "内存不足";
		}
		else if (BootParam.RunSta & RUN_STA_SECRET_ERR) {
			Msg = "解密失败";
		}

		Edit76->Text = Msg;
		LabelAppRunSta->Caption = "运行状态：" + Msg;
	}
	else if ((BootParam.RunSta & RUN_STA_MODE_MASK) == RUM_MODE_BOOT) {
		LabelAppRunSta->Caption = "运行状态：停止";
	}
	else {
		LabelAppRunSta->Caption = "运行状态：N/A";
	}

	// Msg.sprintf("运行状态：%d/%d/%d", BootParam.WDTRstTimes, BootParam.LastRstTime, BootParam.RunTime);
	// LabelAppRunSta->Caption = Msg;

	Msg.sprintf(L"Flash大小：%d KB", BootParam.FlashSize * 32);

	if (BootParam.FlashSize * 32 == 64)
		Msg.cat_printf(L" (F103C8T6)");
	else if (BootParam.FlashSize * 32 == 128)
		Msg.cat_printf(L" (F103CBT6/F103RBT6)");
	else if (BootParam.FlashSize * 32 == 256) {
		if (BootParam.BootLoaderVersion >=
			80 && BootParam.BootLoaderVersion < 90) {
			Msg.cat_printf(L" (AT32F403RC/CC)");
		}
		else {
			Msg.cat_printf(L" (F103RCT6)");
		}
	}
	else if (BootParam.FlashSize * 32 == 512)
		Msg.cat_printf(L" (F103RET6)");
	else if (BootParam.FlashSize * 32 == 1024)
		Msg.cat_printf(L" (405RG/403RG/CG)");
	else
		Msg.cat_printf(L" (N/A)");

	LabelFlashSize->Caption = Msg;

	if (BootParam.Flags & BOOT_FLAG_WIN7) {
		LabelOS->Caption = "系统识别：Win7";
	}
	else {
		LabelOS->Caption = "系统识别：Win XP";
	}

	if (BootParam.Flags & BOOT_FLAG_RO) {
		ShapeRO->Brush->Color = clLime;
		Shape1->Brush->Color = clLime;
	}
	else {
		ShapeRO->Brush->Color = clRed;
		Shape1->Brush->Color = clRed;
	}

	if (BootParam.Flags & BOOT_FLAG_WO) {
		ShapeWO->Brush->Color = clLime;
		Shape2->Brush->Color = clLime;
		ButtonFlashProtect->Caption = "关闭写保护";
	}
	else {
		ShapeWO->Brush->Color = clRed;
		Shape2->Brush->Color = clRed;
		ButtonFlashProtect->Caption = "打开写保护";
	}

	uint32_t MaxPoints = GetMaxNum(BootParam.Flags >> 4);

	Label73->Caption = "最大点数：" + String(MaxPoints);

	if (MaxPoints <= 6) {
		Edit255->Text = String(MaxPoints) + " - " + String(Param.tracepoints);
		MaxPoints = Param.tracepoints;
	}
	else {
		Edit255->Text = String(MaxPoints) + " - " + String(MaxPoints);
	}

	if (EditType->Text.Length() <= 5) {
		if (BootParam.HardwareVer == 460) {
			EditType->Text = "W-" + String(MaxPoints) + "-N";
		}
		else if (BootParam.HardwareVer == 540) {
			EditType->Text = "T-" + String(MaxPoints) + "-N";
		}
		else if (BootParam.HardwareVer == 650 || BootParam.HardwareVer == 671) {
			EditType->Text = "S-" + String(MaxPoints) + "-N";
		}
		else if (BootParam.HardwareVer == 1000) {
			EditType->Text = "TII-" + String(MaxPoints) + "-N";
		}
		else if (BootParam.HardwareVer == 670 || BootParam.HardwareVer == 680 ||
			BootParam.HardwareVer == 690 || BootParam.HardwareVer == 692 ||
			BootParam.HardwareVer == 693) {
			EditType->Text = "SII-" + String(MaxPoints) + "-N";
		}
		else if (BootParam.HardwareVer == 1320) {
			EditType->Text = "D-" + String(MaxPoints) + "-N";
		}
		else if (BootParam.HardwareVer == 1400 || BootParam.HardwareVer == 1410)
		{
			EditType->Text = "E-" + String(MaxPoints) + "-N";
		}
		else {
			EditType->Text = "-" + String(MaxPoints) + "-N";
		}
	}
}

void __fastcall TFormConfig::DevConnect(void) {
	Caption = "触摸屏配置 - 设备已连接!";
	EnableButtons();
}

void __fastcall TFormConfig::DevDisConnect(void) {
	Caption = "触摸屏配置 - 设备未连接!";

	DisableButtons();
}

void __fastcall TFormConfig::Button10Click(TObject *Sender) {
	const uint8_t SerialNumChar[] = "2NPS9E5RHA7WCYVT4QLMXJF3K";
	uint8_t Base = sizeof(SerialNumChar) - 1;
	unsigned long long SerialNum;
	uint8_t Num;
	uint8_t MsgBuf[16];
	uint8_t EnBuf[16];
	tm tTime;
	String Msg;

	// 获取时间
	_getsystime(&tTime);
	ShipTime = mktime(&tTime);

	Msg.sprintf(L"%d-%d-%d", tTime.tm_year + 1900, tTime.tm_mon + 1,
		tTime.tm_mday);
	EditDate->Text = Msg;

	Msg.sprintf(L"%.2d:%.2d:%.2d", tTime.tm_hour, tTime.tm_min, tTime.tm_sec);
	EditTime->Text = Msg;

	// 生成 CPUID
	if ((CPUID[0] == 0x02 && CPUID[1] == 0x03 && CPUID[2] == 0x04 && CPUID[3]
		== 0x05 && CPUID[4] == 0x06) ||
		(CPUID[0] == 0xFF && CPUID[1] == 0xFF && CPUID[2] == 0xFF && CPUID[3]
		== 0xFF && CPUID[4] == 0xFF)) {
		String Msg = "";
		for (int i = 0; i < 12; i += 1) {
			CPUID[i] = rand();
			Msg.cat_printf(L"%.2X", CPUID[i]);
		}

		EditCPUID->Text = Msg;
	}

	// 3DES
	memcpy(MsgBuf, &ShipTime, 4);
	memcpy(MsgBuf + 4, CPUID, 12);

	for (int i = 0; i < 8; i += 1) {
		MsgBuf[i] ^= MsgBuf[8 + i];
	}

	Des_Go(EnBuf, MsgBuf, 8, des_key, sizeof(des_key), ENCRYPT);

	// 27进制
	SerialNum = 0;

	memcpy(&SerialNum, EnBuf, 8);
	Msg = "";

	for (int i = 0; i < 12; i += 1) {
		if (i > 0 && (i & 0x03) == 0) {
			Msg += "-";
		}

		Num = SerialNum % Base;
		SerialNum /= Base;
		Msg.cat_printf(L"%c", SerialNumChar[Num]);
	}

	EditSerialNum->Text = Msg;
}

// ---------------------------------------------------------------------------

void __fastcall TFormConfig::ButtonFlashProtectClick(TObject *Sender) {
	if (ButtonFlashProtect->Caption == "打开写保护") {
		MainForm->HIDSendThread->SubmitCmd(BOOT_CMD_WO_ON, NULL, 0);
	}
	else {
		MainForm->HIDSendThread->SubmitCmd(BOOT_CMD_WO_OFF, NULL, 0);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::ButtonGetCPUIDClick(TObject *Sender) {
	// 获取 CPUID
	EditCPUID->Text = "";
	memset(CPUID, 0, sizeof(CPUID));

	if (FormUpdate->BootParam.BootLoaderVersion == 0 ||
		FormUpdate->BootParam.BootLoaderVersion == 16 ||
		FormUpdate->BootParam.BootLoaderVersion == 18) {
		MainForm->HIDSendThread->SubmitCmd(BOOT_CMD_READ_SERIAL, NULL, 0);
	}
	else {
		MainForm->HIDSendThread->SubmitCmd(BOOT_CMD_CPUID, NULL, 0);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::Button15Click(TObject *Sender) {
	EditCPUID->Text = "";
	EditType->Text = "";
	EditSerialNum->Text = "";
	EditDate->Text = "";
	EditTime->Text = "";
	EditCode->Text = "";
	EditCustomer->Text = "";

	EditBootVer->Text = "";
	EditSoftwareVer->Text = "";
	EditHardwareVer->Text = "";
	Edit75->Text = "";
	Edit76->Text = "";
	Edit255->Text = "";
}
// ---------------------------------------------------------------------------

bool __fastcall TFormConfig::CheckParam(void) {
	String ErrMsg = "";
	char *Ptr;
	int i;

	// ----------- Check CPUID ------------------------------
	for (i = 0; i < sizeof(CPUID); i += 1) {
		if (CPUID[i] != 0)
			break;
	}

	if (i >= sizeof(CPUID)) {
		ErrMsg += "CPUID异常，请先获取CPUID!\n";
	}

	// ----------- Check Type ------------------------------
	if (EditType->Text.Length() < 5) {
		ErrMsg += "触摸屏型号有误，请确认!\n";
	}
	else if (EditType->Text.Length() > 16) {
		ErrMsg += "触摸屏型号过长(>16),请修改!\n";
	}
	else if ((Ptr = strchr(AnsiString(EditType->Text).c_str(), '-')) != NULL) {
		if (atoi(Ptr + 1) != GetMaxNum(BootParam.Flags >> 4)
			&& Param.tracepoints != atoi(Ptr + 1)) {
			ErrMsg += "触摸屏型号最大上报点数与实际不匹配！\n";
		}
	}

	// ----------- Check Serial Number ---------------------
	if (EditSerialNum->Text.Length() != 12 + 2) {
		ErrMsg += "请生成12位序列号!\n";
	}

	// ----------- Check Code ------------------------------
	if (EditCode->Text.Length() < 6) {
		ErrMsg += "请填写发货代码(快递单号)!\n";
	}
	else if (EditCode->Text.Length() > 12) {
		ErrMsg += "发货代码过长(>12),请修改!\n";
	}

	// ----------- Check Customer ------------------------------
	if (EditCustomer->Text.Length() == 0) {
		ErrMsg += "请填写客户信息!\n";
	}
	else if (EditCustomer->Text.Length() > 64) {
		ErrMsg += "客户信息过长(>64),请修改!\n";
	}

	// Versions
	if (EditBootVer->Text.Length() == 0) {
		ErrMsg += "请获取Boot版本!\n";
	}

	if (EditHardwareVer->Text.Length() == 0) {
		ErrMsg += "请获取硬件版本!\n";
	}

	if (EditSoftwareVer->Text.Length() == 0) {
		ErrMsg += "请获取固件版本!\n";
	}

	// // ----------- VID ---------------------
	// if (EditDispVID->Text.Length() != 4)
	// {
	// ErrMsg += "请输入4位十六进制VID!\n";
	// }
	//
	// // ----------- PID ---------------------
	// if (EditDispPID->Text.Length() != 4)
	// {
	// ErrMsg += "请输入4位十六进制PID!\n";
	// }
	//
	// // ----------- Manufacturer Information ---------------------
	// if (EditDispManuInfo->Text.Length() >= 64)
	// {
	// ErrMsg += "厂商信息不允许超过63个字符!\n";
	// }
	//
	// // ----------- Product Information ---------------------
	// if (EditDispProInfo->Text.Length() >= 64)
	// {
	// ErrMsg += "产品信息不允许超过63个字符!\n";
	// }

	if (ErrMsg.Length() != 0) {
		ShowMessage(ErrMsg);
		return false;
	}
	else {
		return true;
	}
}

// ---------------------------------------------------------------------------
long __fastcall TFormConfig::CheckSerialNum(FILE *fp) {
	long FileLen;
	long CurPos;
	long len;
	int i;
	uint8_t Buf[RecordSize];
	uint8_t *Ptr;

	fseek(fp, 0, SEEK_END);
	FileLen = ftell(fp);

	rewind(fp);

	while (FileLen) {
		CurPos = ftell(fp);
		len = fread(Buf, 1, sizeof(Buf), fp);

		Ptr = Buf +sizeof(CPUID)+sizeof(ShipTime);

		if (strcmp(Ptr, AnsiString(EditSerialNum->Text).c_str()) == 0) {
			break;
		}

		FileLen -= len;
	}

	if (FileLen) {
		fseek(fp, CurPos, SEEK_SET);
	}
	else {
		CurPos = -1;
	}

	return CurPos;
}

long __fastcall TFormConfig::CheckCPUID(FILE *fp) {
	long FileLen;
	long CurPos;
	long len;
	int i;
	uint8_t Buf[RecordSize];

	fseek(fp, 0, SEEK_END);
	FileLen = ftell(fp);

	rewind(fp);

	while (FileLen) {
		CurPos = ftell(fp);
		len = fread(Buf, 1, sizeof(Buf), fp);

		for (i = 0; i < sizeof(CPUID); i += 1) {
			if (Buf[i] != CPUID[i])
				break;
		}

		if (i >= sizeof(CPUID)) {
			break;
		}

		FileLen -= len;
	}

	if (FileLen) {
		fseek(fp, CurPos, SEEK_SET);
	}
	else {
		CurPos = -1;
	}

	return CurPos;
}

String __fastcall TFormConfig::GetCPUType(void) {
	String Type = "N/A";

	if (BootParam.FlashSize * 32 == 64)
		Type = "F103C8T6";
	else if (BootParam.FlashSize * 32 == 128) {
		if ((BootParam.BootLoaderVersion & 0x7F) >= 60) {
			if (BootParam.HardwareVer == 540 || BootParam.HardwareVer == 650 ||
				BootParam.HardwareVer == 700 || BootParam.HardwareVer == 1000 ||
				BootParam.HardwareVer == 460) {
				Type = "F303RBT6";
			}
			else {
				Type = "F303CBT6";
			}
		}
		else {
			if (BootParam.HardwareVer == 540 || BootParam.HardwareVer == 650 ||
				BootParam.HardwareVer == 700 || BootParam.HardwareVer == 1000 ||
				BootParam.HardwareVer == 460) {
				Type = "F103RBT6";
			}
			else {
				Type = "F103CBT6";
			}
		}
	}
	else if (BootParam.FlashSize * 32 == 256) {
		if ((BootParam.BootLoaderVersion & 0x7F) >= 60) {
			if (BootParam.HardwareVer == 540 || BootParam.HardwareVer == 650 ||
				BootParam.HardwareVer == 700 || BootParam.HardwareVer == 1000 ||
				BootParam.HardwareVer == 460) {
				Type = "F303RCT6";
			}
			else {
				Type = "F303CCT6";
			}
		}
		else {
			Type = "F103RCT6";
		}
	}
	else if (BootParam.FlashSize * 32 == 512)
		Type = "F103RET6";
	else if (BootParam.FlashSize * 32 == 1024)
		Type = "F405RGT6";

	return Type;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormConfig::WriteDataBase(void) {
	// open data file
	FILE *fp;
	uint8_t Buf[RecordSize];
	uint8_t *Ptr = Buf;
	String FileName = Application->ExeName;

	FileName = ExtractFileDir(FileName);
	FileName += "\\Serial.txt";

	if (CheckParam() == false)
		return false;

	if ((fp = fopen(AnsiString(FileName).c_str(), "rb+")) == NULL) {
		if ((fp = fopen(AnsiString(FileName).c_str(), "wb+")) == NULL) {
			ShowMessage(L"无法创建或者打开记录文件 Serial.txt!");
			return false;
		}
	}

	if (CheckCPUID(fp) >= 0) // find exist cpuid
	{
		if (MessageBox(NULL, L"CPUID已经存在，需要更新吗?", L"确认更新",
			MB_OKCANCEL | MB_ICONWARNING) == IDCANCEL) {
			fclose(fp);
			return false;
		}
	}
	else // seek end
	{
		fseek(fp, 0, SEEK_END);
	}

	memset(Buf, 0, sizeof(Buf));
	memcpy(Ptr, CPUID, sizeof(CPUID));
	Ptr += sizeof(CPUID);
	memcpy(Ptr, &ShipTime, sizeof(ShipTime));
	Ptr += sizeof(ShipTime);

	strcpy(Ptr, AnsiString(EditSerialNum->Text).c_str());
	Ptr += EditSerialNum->Text.Length() + 1;

	strcpy(Ptr, AnsiString(EditType->Text).c_str());
	Ptr += EditType->Text.Length() + 1;

	strcpy(Ptr, AnsiString(EditCode->Text).c_str());
	Ptr += EditCode->Text.Length() + 1;

	strcpy(Ptr, AnsiString(EditCustomer->Text).c_str());
	Ptr += EditCustomer->Text.Length() + 1;

	strcpy(Ptr, AnsiString(EditHardwareVer->Text).c_str());
	Ptr += EditHardwareVer->Text.Length() + 1;

	strcpy(Ptr, AnsiString(EditSoftwareVer->Text).c_str());
	Ptr += EditSoftwareVer->Text.Length() + 1;

	strcpy(Ptr, AnsiString(EditBootVer->Text).c_str());
	Ptr += EditBootVer->Text.Length() + 1;

	// CPUType
	strcpy(Ptr, AnsiString(GetCPUType()).c_str());

	fwrite(Buf, 1, sizeof(Buf), fp);

	fclose(fp);

	return true;
}

void __fastcall TFormConfig::Button9Click(TObject *Sender) {
	if (WriteDataBase() == true) {
		ShowMessage(L"成功写入数据库!");
	}
}

void __fastcall TFormConfig::ShowAutoGainResult(uint8_t *Buf, uint16_t Len) {
	int Boards = Buf[0];

	for (int i = 1; i < StringGrid1->ColCount && i < Boards + 1; i += 1) {
		StringGrid1->Cells[i][1] = String((uint32_t)(Buf[i] & 0x0F));
		StringGrid1->Cells[i][2] = String((uint32_t)((Buf[i] >> 4) & 0x0F));
		StringGrid1->Cells[i][4] = String((uint32_t)(Buf[i + Boards * 1]));
		StringGrid1->Cells[i][5] =
			String((uint32_t)(Buf[i + Boards * 2] & 0x0F));
		StringGrid1->Cells[i][6] =
			String((uint32_t)(Buf[i + Boards * 3] & 0x0F));
		StringGrid1->Cells[i][7] =
			String((uint32_t)(Buf[i + Boards * 4] & 0x0F));
		StringGrid1->Cells[i][8] =
			String((uint32_t)(Buf[i + Boards * 5] & 0x0F));
		StringGrid1->Cells[i][9] =
			String((uint32_t)(Buf[i + Boards * 6] & 0x0F));
		StringGrid1->Cells[i][10] =
			String((uint32_t)(Buf[i + Boards * 7] & 0x0F));
		StringGrid1->Cells[i][11] =
			String((uint32_t)(Buf[i + Boards * 8] & 0x0F));
		StringGrid1->Cells[i][12] =
			String((uint32_t)(Buf[i + Boards * 9] & 0x0F));
		StringGrid1->Cells[i][13] =
			String((uint32_t)(Buf[i + Boards * 10] & 0x0F));
		StringGrid1->Cells[i][14] =
			String((uint32_t)(Buf[i + Boards * 11] & 0x0F));
		StringGrid1->Cells[i][15] =
			String((uint32_t)(Buf[i + Boards * 12] & 0x0F));
		StringGrid1->Cells[i][16] =
			String((uint32_t)(Buf[i + Boards * 13] & 0x0F));
	}
}

const char *EmitVerStringMap[] = {
	"V4.6", "V5.4", "V6.5", "V7.0", "X-V1.1", "X-V8.0", "JY-T6-21-N", "V9.0",
	"X-PD-21.5-R2", "V10.0", "X-V5.2", "JY-T6-21-N-R2", "V6.7", "X-V12.0",
	"X-V12.1", "V6.8", "X-V12.2", "V6.9", "V13.0", "15M-X-HS-EMIT",
	"MainV6.9 EmitV6.8", "X-V14.1", "MainV6.9-3N EmitV6.8",
	"MainV6.9-3N EmitV6.9", "V5.6", "X-21.5-V14.2", "E19-V14.3", "E15-V14.4",
	"V5.7", };

#define  EmitVerStringMapSize (sizeof(EmitVerStringMap) / sizeof(EmitVerStringMap[0]))

const char *EmitVerYStringMap[] = {
	"V4.6", "V5.4", "V6.5", "V7.0", "X-V1.1", "X-V8.0", "JY-T6-21-N", "V9.0",
	"X-PD-21.5-R2", "V10.0", "X-V5.2", "JY-T6-21-N-R2", "V6.7", "X-V12.0",
	"X-V12.1", "V6.8", "X-V12.2", "V6.9", "V13.0", "15M-X-HS-EMIT", "X-V14.1",
	"V5.6", "X-21.5-V14.2", "E19-V14.3", "E15-V14.4", "V5.7", };

#define  EmitVerYStringMapSize (sizeof(EmitVerYStringMap) / sizeof(EmitVerYStringMap[0]))

const char *RevVerStringMap[] = {
	"PD-V4.6", "PD-V5.4", "PT-V5.4", "PD-V6.5", "PD-V7.0", "X-PT-V1.1",
	"X-PT-V8.0", "JY-T6-21-N", "PT-V9.0", "X-PD-21.5-R2", "PD-V10.0",
	"PT-V10.0", "X-PD-V5.2", "JY-T6-21-N-R2", "PD-V6.7", "X-PD-V12.0",
	"X-PD-V12.1", "PD-V6.8", "X-PD-V12.2", "PD-V6.9", "PD-V13.0",
	"15M-X-HS-REV", "PT-V13.0", "X-PT-V14.1", "PD-V5.6", "X-21.5-V14.2",
	"E19-V14.3", "E15-V14.4", "PD-V5.7", };

#define  RevVerStringMapSize (sizeof(RevVerStringMap) / sizeof(RevVerStringMap[0]))

void __fastcall TFormConfig::ShowScanParam(uint8_t *Buf, uint16_t Len) {
	String Msg;
	SCAN_TABLE_HEAD *sp = &ScanParam;
	int i, Cuts;

	if (Len >= (uint16_t)(&(((SCAN_TABLE_HEAD*)0)->Feature)) + 4) {
		memcpy(sp, Buf, Len);

		if (sp->Flags == 0xFF || sp->XAxis == 0xFF || sp->YAxis == 0xFF) {
			StatusBar1->SimpleText = L"扫描表尚未设置，读取失败！";
			return;
		}

		// -------------------- X --------------------------------------
		Msg.sprintf(L"%d", sp->XBoardLed);
		ComboBoxXBoardLeds->Text = Msg;
		ComboBoxXBoardLeds->ItemIndex = ComboBoxXBoardLeds->Items->IndexOf(Msg);

		Msg.sprintf(L"%d", sp->XBoards);
		ComboBoxXBoards->Text = Msg;
		ComboBoxXBoards->ItemIndex = ComboBoxXBoards->Items->IndexOf(Msg);

		for (i = 0, Cuts = 0; Cuts < XCutBoards->Count / 2 && i <
			sizeof(sp->XCuts); i += 2) {
			if (sp->XCuts[i + 1] != 0) {
				Msg.sprintf(L"%d", sp->XCuts[i] + 1);
				((TComboBox *)(XCutBoards->Items[Cuts]))->Text = Msg;
				((TComboBox*)(XCutBoards->Items[Cuts]))->ItemIndex =
					((TComboBox*)(XCutBoards->Items[Cuts]))
					->Items->IndexOf(Msg);

				Msg.sprintf(L"%d", sp->XCuts[i + 1]);
				((TEdit *)(XCutBoards->Items[Cuts + XCutBoards->Count / 2]))
					->Text = Msg;

				Cuts += 1;
			}
		}

		for (; Cuts < XCutBoards->Count / 2; Cuts += 1) {
			((TComboBox *)(XCutBoards->Items[Cuts]))->Text = "0";
			((TComboBox*)(XCutBoards->Items[Cuts]))->ItemIndex =
				((TComboBox*)(XCutBoards->Items[Cuts]))->Items->IndexOf("0");
			((TEdit *)(XCutBoards->Items[Cuts + XCutBoards->Count / 2]))
				->Text = "0";
		}

		if (sp->HIDToolsVer < 3004 || sp->XCutUnUsed != 0xAA) {
			for (i = 0; i < XCutBoards->Count / 2; i += 1) {
				if (i < sp->XBoards) {
					((TComboBox *)(XCutBoards->Items[i]))->Enabled = true;
					((TEdit *)(XCutBoards->Items[i + XCutBoards->Count / 2]))
						->Enabled = true;
				}
				else {
					((TComboBox *)(XCutBoards->Items[i]))->Enabled = false;
					((TEdit *)(XCutBoards->Items[i + XCutBoards->Count / 2]))
						->Enabled = false;
				}
			}
		}

		Msg.printf(L"%.3f", float(sp->XLedSpace) / 1000.0);
		EditXDist->Text = Msg;

		Msg.sprintf(L"%.1f", sp->MarginLeft);
		EditXMargin->Text = Msg;

		Msg.sprintf(L"%.1f", sp->MarginRight);
		EditXMarginRight->Text = Msg;

		Msg.sprintf(L"%d", sp->XPowerGroup);
		EditXMiniOff->Text = Msg;

		Msg.sprintf(L"%d", sp->XJumpLeds);
		EditXJump->Text = Msg;

		Msg.sprintf(L"%d", sp->XScanGap);
		EditXScanGap->Text = Msg;

		Msg.sprintf(L"%d", sp->XReScan);
		EditXReScan->Text = Msg;

		Msg.sprintf(L"%d", sp->XPow);
		ComboBoxXPower->Text = Msg;
		ComboBoxXPower->ItemIndex = ComboBoxXPower->Items->IndexOf(Msg);

		Msg.sprintf(L"%d", sp->XAxis);
		ComboBoxXAxis->Text = Msg;
		ComboBoxXAxis->ItemIndex = ComboBoxXAxis->Items->IndexOf(Msg);

		Msg.sprintf(L"%d", sp->XLeds);
		EditXLeds->Text = Msg;

		if (sp->HIDToolsVer >= 2082) {
			ComboBoxEmitXVer->ItemIndex = sp->XEmitVer;
			ComboBoxRevXVer->ItemIndex = sp->XRevVer;
		}
		else { // remap
			if (sp->XEmitVer < EmitVerStringMapSize) {
				ComboBoxEmitXVer->ItemIndex =
					ComboBoxEmitXVer->Items->IndexOf
					(String(EmitVerStringMap[sp->XEmitVer]));
			}
			else {
				ComboBoxEmitXVer->ItemIndex = 0;
			}

			if (sp->XRevVer < RevVerStringMapSize) {
				ComboBoxRevXVer->ItemIndex =
					ComboBoxRevXVer->Items->IndexOf
					(String(RevVerStringMap[sp->XRevVer]));
			}
			else {
				ComboBoxRevXVer->ItemIndex = 0;
			}
		}

		Msg.sprintf(L"%.2f", (float)sp->XMaxDeg / 100.0);
		EditXoffset->Text = Msg;

		// -------------------- Y --------------------------------------
		Msg.sprintf(L"%d", sp->YBoardLed);
		ComboBoxYBoardLeds->Text = Msg;
		ComboBoxYBoardLeds->ItemIndex = ComboBoxYBoardLeds->Items->IndexOf(Msg);

		Msg.sprintf(L"%d", sp->YBoards);
		ComboBoxYBoards->Text = Msg;
		ComboBoxYBoards->ItemIndex = ComboBoxYBoards->Items->IndexOf(Msg);

		for (i = 0, Cuts = 0; Cuts < YCutBoards->Count / 2 && i <
			sizeof(sp->YCuts); i += 2) {
			if (sp->YCuts[i + 1] != 0) {
				Msg.sprintf(L"%d", sp->YCuts[i] + 1);
				((TComboBox *)(YCutBoards->Items[Cuts]))->Text = Msg;
				((TComboBox*)(YCutBoards->Items[Cuts]))->ItemIndex =
					((TComboBox*)(YCutBoards->Items[Cuts]))
					->Items->IndexOf(Msg);

				Msg.sprintf(L"%d", sp->YCuts[i + 1]);
				((TEdit *)(YCutBoards->Items[Cuts + YCutBoards->Count / 2]))
					->Text = Msg;

				Cuts += 1;
			}
		}

		for (; Cuts < YCutBoards->Count / 2; Cuts += 1) {
			((TComboBox *)(YCutBoards->Items[Cuts]))->Text = "0";
			((TComboBox*)(YCutBoards->Items[Cuts]))->ItemIndex =
				((TComboBox*)(YCutBoards->Items[Cuts]))->Items->IndexOf("0");
			((TEdit *)(YCutBoards->Items[Cuts + YCutBoards->Count / 2]))
				->Text = "0";
		}

		if (sp->HIDToolsVer < 3004 || sp->YCutUnUsed != 0xAA) {
			for (i = 0; i < YCutBoards->Count / 2; i += 1) {
				if (i < sp->YBoards) {
					((TComboBox *)(YCutBoards->Items[i]))->Enabled = true;
					((TEdit *)(YCutBoards->Items[i + YCutBoards->Count / 2]))
						->Enabled = true;
				}
				else {
					((TComboBox *)(YCutBoards->Items[i]))->Enabled = false;
					((TEdit *)(YCutBoards->Items[i + YCutBoards->Count / 2]))
						->Enabled = false;
				}
			}
		}

		Msg.printf(L"%.3f", float(sp->YLedSpace) / 1000.0);
		EditYDist->Text = Msg;

		Msg.sprintf(L"%.1f", sp->MarginUp);
		EditYMargin->Text = Msg;

		Msg.sprintf(L"%.1f", sp->MarginDown);
		EditYMarginDown->Text = Msg;

		Msg.sprintf(L"%d", sp->YPowerGroup);
		EditYMiniOff->Text = Msg;

		Msg.sprintf(L"%d", sp->YJumpLeds);
		EditYJump->Text = Msg;

		Msg.sprintf(L"%d", sp->YScanGap);
		EditYScanGap->Text = Msg;

		Msg.sprintf(L"%d", sp->YReScan);
		EditYReScan->Text = Msg;

		Msg.sprintf(L"%d", sp->YPow);
		ComboBoxYPower->Text = Msg;
		ComboBoxYPower->ItemIndex = ComboBoxYPower->Items->IndexOf(Msg);

		Msg.sprintf(L"%d", sp->YAxis);
		ComboBoxYAxis->Text = Msg;
		ComboBoxYAxis->ItemIndex = ComboBoxYAxis->Items->IndexOf(Msg);

		Msg.sprintf(L"%d", sp->YLeds);
		EditYLeds->Text = Msg;

		if (sp->HIDToolsVer >= 2082) {
			ComboBoxEmitYVer->ItemIndex = sp->YEmitVer;
			ComboBoxRevYVer->ItemIndex = sp->YRevVer;
		}
		else {
			if (sp->YEmitVer < EmitVerYStringMapSize) {
				ComboBoxEmitYVer->ItemIndex =
					ComboBoxEmitYVer->Items->IndexOf
					(String(EmitVerYStringMap[sp->YEmitVer]));
			}
			else {
				ComboBoxEmitYVer->ItemIndex = 0;
			}

			if (sp->YRevVer < RevVerStringMapSize) {
				ComboBoxRevYVer->ItemIndex =
					ComboBoxRevYVer->Items->IndexOf
					(String(RevVerStringMap[sp->XRevVer]));
			}
			else {
				ComboBoxRevYVer->ItemIndex = 0;
			}
		}

		Msg.sprintf(L"%.2f", (float)sp->YMaxDeg / 100.0);
		EditYoffset->Text = Msg;

		for (int i = 0; i < ScanFeatureCheckBox->Count; i += 1) {
			if ((sp->Feature & (1 << i)) == 0) {
				((TCheckBox *)(ScanFeatureCheckBox->Items[i]))->Checked = false;
			}
			else {
				((TCheckBox *)(ScanFeatureCheckBox->Items[i]))->Checked = true;
			}
		}

		if (CheckBoxAntiGlare->Checked && sp->Maigc1 == 0x13579ABC &&
			sp->HIDToolsVer > 2071) {
			Msg.sprintf(L"%d", sp->GlareLever);
			ComboBoxGlareLever->Text = Msg;
			ComboBoxGlareLever->ItemIndex =
				ComboBoxGlareLever->Items->IndexOf(Msg);
		}
		else {
			ComboBoxGlareLever->Text = "3";
			Msg = "3";
			ComboBoxGlareLever->ItemIndex =
				ComboBoxGlareLever->Items->IndexOf(Msg);
		}

		ComboBoxXDivMethod->ItemIndex = sp->XDivMethod;
		ComboBoxYDivMethod->ItemIndex = sp->YDivMethod;

		ComboBoxXAxisChange(NULL);

		if (sp->HIDToolsVer >= 3004) {
			String msg = "";
			if (sp->XCutUnUsed == 0xAA) {
				for (int i = 0; i < sp->XBoards; i += 1) {
					msg.cat_printf(L"%d,", sp->BoardLed[i + sp->YBoards]);
				}

				EditXBoardLed->Text = msg;
				CheckBox17->Checked = true;
				EditXBoardLedChange(NULL);
			}
			else {
				CheckBox17->Checked = false;

				for (int i = 0; i < sp->XBoards; i += 1) {
					msg.cat_printf(L"%d,", XParam.BoradLeds[i]);
				}

				EditXBoardLed->Text = msg;
			}

			// CheckBox17Click(NULL);
		}

		// Button3Click(NULL);
		ComboBoxXDivMethodChange(NULL);

		ComboBoxYAxisChange(NULL);
		if (sp->HIDToolsVer >= 3004) {
			String msg = "";

			if (sp->YCutUnUsed == 0xAA) {
				for (int i = 0; i < sp->YBoards; i += 1) {
					msg.cat_printf(L"%d,", sp->BoardLed[i]);
				}

				EditYBoardLed->Text = msg;

				CheckBox19->Checked = true;
				EditYBoardLedChange(NULL);
			}
			else {

				CheckBox19->Checked = false;

				for (int i = 0; i < sp->YBoards; i += 1) {
					msg.cat_printf(L"%d,", YParam.BoradLeds[i]);
				}

				EditYBoardLed->Text = msg;
			}

			// CheckBox19Click(NULL);
		}
		// Button2Click(NULL);
		ComboBoxYDivMethodChange(NULL);
	}
	else {
		this->StatusBar1->SimpleText = L"获取数据长度有误，解析失败！";
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormConfig::ShowUserParam(uint8_t *Buf, uint16_t Len) {
	String Msg;

	memcpy(&UserParam, Buf, sizeof(Param));

	if (UserParam.Frames == 0xFF) {
		StatusBar1->SimpleText = L"参数读取成功,参数尚未设置！";
		return;
	}
	else {
		StatusBar1->SimpleText = L"参数读取成功！";
	}

	Msg.sprintf(L"%d", UserParam.Frames);
	Edit91->Text = Msg;

	// Msg.sprintf("%d", Param.LowFrames);
	// EditLowFrame->Text = Msg;
	//
	// Msg.sprintf("%d", Param.TOn);
	// EditTOn->Text = Msg;
	//
	// Msg.sprintf("%d", Param.TOff);
	// EditTOff->Text = Msg;
	//
	// Msg.sprintf("%d", Param.AdTrig);
	// EditADTrig->Text = Msg;
	//
	// Msg.sprintf("%d", Param.TimeOut);
	// EditLowFrameTimeout->Text = Msg;
	//
	// Msg.sprintf("%d", Param.TRise);
	// EditTRise->Text = Msg;

	// for(int i=1; i<16; i+=1)
	// {
	// StringGrid1->Cells[i][1] = String((uint32_t)(Param.Gain1[i-1]&0x07));
	// StringGrid1->Cells[i][2] = String((uint32_t)(Param.Gain2[i-1]&0x03));
	// StringGrid1->Cells[i][3] = String((uint32_t)(Param.EmitPow[i-1]&0x07));
	// }

	for (int i = 0; i < FeatureCheckBoxUser->Count; i += 1) {
		if (i == 2)
			continue;

		if ((UserParam.Feature & (1 << i + 1)) != 0) {
			((TCheckBox *)FeatureCheckBoxUser->Items[i])->Checked = true;
		}
		else {
			((TCheckBox *)FeatureCheckBoxUser->Items[i])->Checked = false;
		}
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormConfig::ShowParam(uint8_t *Buf, uint16_t Len) {
	String Msg;

	memcpy(&Param, Buf, sizeof(Param));

	if (FeatureCheckBox == NULL)
		return;

	if (this->BootParam.HardwareVer == 540) {
		StringGrid1->Cells[0][3] = L"发射功率";
	}
	else {
		StringGrid1->Cells[0][3] = L"采样点";
	}

	if (Param.TOn == 0xFF) {
		StatusBar1->SimpleText = L"参数读取成功,参数尚未设置！";
	}
	else {
		StatusBar1->SimpleText = L"参数读取成功！";
	}

	Msg.sprintf(L"%d", Param.Frames);
	EditFrame->Text = Msg;

	Msg.sprintf(L"%d", Param.LowFrames);
	EditLowFrame->Text = Msg;

	Msg.sprintf(L"%d", Param.TOn);
	EditTOn->Text = Msg;

	Msg.sprintf(L"%d", Param.TOff);
	EditTOff->Text = Msg;

	Msg.sprintf(L"%d", Param.AdTrig);
	EditADTrig->Text = Msg;

	Msg.sprintf(L"%d", Param.TimeOut);
	EditLowFrameTimeout->Text = Msg;

	Msg.sprintf(L"%d", Param.TRise);
	EditTRise->Text = Msg;

	if (Param.gesturestart < 1 || Param.gesturestart > Param.gestureend) {
		Param.gesturestart = 3;
		Param.gestureend = 10;
	}

	Msg.sprintf(L"%d", Param.gesturestart);
	EditGestureStart->Text = Msg;

	Msg.sprintf(L"%d", Param.gestureend);
	EditGestureEnd->Text = Msg;

	Msg.sprintf(L"%d", Param.tracepoints);
	EditTracePoints->Text = Msg;

	uint32_t MaxPoints = GetMaxNum(BootParam.Flags >> 4);

	if (MaxPoints <= 6) {
		Edit255->Text = String(MaxPoints) + " - " + String(Param.tracepoints);
	}
	else {
		Edit255->Text = String(MaxPoints) + " - " + String(MaxPoints);
	}

	for (int i = 1; i < StringGrid1->ColCount; i += 1) {
		if (i > 45) {
			StringGrid1->Cells[i][1] =
				String((uint32_t)((Param.Gain1[i - 46] >> 4) & 0x07));
			StringGrid1->Cells[i][2] =
				String((uint32_t)((Param.Gain2[i - 46] >> 4) & 0x03));

			if (this->BootParam.HardwareVer == 540) {
				StringGrid1->Cells[i][3] =
					String((uint32_t)((Param.EmitPow[i - 46] >> 4) & 0x07));
			}
			else {
				StringGrid1->Cells[i][3] = "0";
			}
		}
		else if (i > 30) {
			StringGrid1->Cells[i][1] =
				String((uint32_t)(Param.Gain1[i - 31] & 0x07));
			StringGrid1->Cells[i][2] =
				String((uint32_t)(Param.Gain2[i - 31] & 0x03));
			if (this->BootParam.HardwareVer == 540) {
				StringGrid1->Cells[i][3] =
					String((uint32_t)(Param.EmitPow[i - 31] & 0x07));
			}
			else {
				StringGrid1->Cells[i][3] =
					String((uint32_t)(Param.EmitPow[i - 31]));
			}
		}
		else if (i > 15) {
			StringGrid1->Cells[i][1] =
				String((uint32_t)((Param.Gain1[i - 16] >> 4) & 0x07));
			StringGrid1->Cells[i][2] =
				String((uint32_t)((Param.Gain2[i - 16] >> 4) & 0x03));

			if (this->BootParam.HardwareVer == 540) {
				StringGrid1->Cells[i][3] =
					String((uint32_t)((Param.EmitPow[i - 16] >> 4) & 0x07));
			}
			else {
				StringGrid1->Cells[i][3] = "0";
			}
		}
		else {
			StringGrid1->Cells[i][1] =
				String((uint32_t)(Param.Gain1[i - 1] & 0x07));
			StringGrid1->Cells[i][2] =
				String((uint32_t)(Param.Gain2[i - 1] & 0x03));
			if (this->BootParam.HardwareVer == 540) {
				StringGrid1->Cells[i][3] =
					String((uint32_t)(Param.EmitPow[i - 1] & 0x07));
			}
			else {
				StringGrid1->Cells[i][3] =
					String((uint32_t)(Param.EmitPow[i - 1]));
			}
		}
	}

	for (int i = 0; i < FeatureCheckBox->Count; i += 1) {
		if ((Param.Feature & (1 << i + 1)) != 0) {
			((TCheckBox *)FeatureCheckBox->Items[i])->Checked = true;
		}
		else {
			((TCheckBox *)FeatureCheckBox->Items[i])->Checked = false;
		}
	}

	if (this->CheckBoxFeatureUSARTOn->Checked == true) {
		ComboBoxUartReportType->Enabled = true;
		this->ComboBoxUartReportType->ItemIndex = (Param.Feature >> 10) & 0x00F;
	}
	else {
		ComboBoxUartReportType->Enabled = false;
		this->ComboBoxUartReportType->ItemIndex = 0;
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormConfig::ShowShipInfo(uint8_t *Buf, uint16_t Len) {
	time_t Secs;
	tm *TimeDate;
	String Msg = "";
	char *Ptr = Buf;

	// CPUID
	for (int i = 0; i < sizeof(CPUID); i += 1) {
		Msg.cat_printf(L"%.2X", (uint8_t)Ptr[i] & 0xFF);
		CPUID[i] = Ptr[i];
	}

	EditCPUID->Text = Msg;
	Ptr += sizeof(CPUID);

	// time
	memcpy(&Secs, Ptr, 4);
	Ptr += 4;
	ShipTime = Secs;

	if ((uint32_t)ShipTime == 0xFFFFFFFF) {
		EditDate->Text = "N/A";
		EditTime->Text = "N/A";
		EditSerialNum->Text = "N/A";
	}
	else {
		if ((TimeDate = localtime(&Secs)) != NULL) {
			Msg.printf(L"%d-%d-%d", TimeDate->tm_year + 1900,
				TimeDate->tm_mon + 1, TimeDate->tm_mday);
			EditDate->Text = Msg;

			Msg.printf(L"%.2d:%.2d:%.2d", TimeDate->tm_hour, TimeDate->tm_min,
				TimeDate->tm_sec);
			EditTime->Text = Msg;
		}

		// Serial number
		EditSerialNum->Text = String(Ptr);
		Ptr += strlen(Ptr) + 1;

		// Multi-Touch Screen Type
		EditType->Text = String(Ptr);
		Ptr += strlen(Ptr) + 1;

		// //VID
		// Msg.sprintf(L"%04X", MainForm->HIDSendThread->usVid);
		// EditDispVID->Text = Msg;
		// //PID
		// Msg.sprintf(L"%04X", MainForm->HIDSendThread->usPid);
		// EditDispPID->Text = Msg;
	}

	if (Len > 62) {
		// shipping code
		EditCode->Text = String(Ptr);
		Ptr += strlen(Ptr) + 1;

		// Customer information
		EditCustomer->Text = String(Ptr);
		Ptr += strlen(Ptr) + 1;

		// hardware version
		EditHardwareVer->Text = String(Ptr);
		Ptr += strlen(Ptr) + 1;

		// softwate version
		EditSoftwareVer->Text = String(Ptr);
		Ptr += strlen(Ptr) + 1;

		// Bootloader version
		EditBootVer->Text = String(Ptr);
	}

	return;
}

void __fastcall TFormConfig::Button13Click(TObject *Sender) {
	FILE *fp;
	uint8_t Buf[RecordSize];
	String FileName = Application->ExeName;

	FileName = ExtractFileDir(FileName);
	FileName += "\\Serial.txt";

	if ((fp = fopen(AnsiString(FileName).c_str(), "rb")) == NULL) {
		ShowMessage(L"无法打开记录文件:\n" + FileName);
		return;
	}

	if (EditCPUID->Text.Length() == 24) {
		for (int i = 0; i < 24; i += 2) {
			CPUID[i / 2] =
				hexfile.HexToInt(AnsiString(EditCPUID->Text).c_str() + i, 2);
		}

		if (CheckCPUID(fp) < 0) {
			ShowMessage(L"CPUID:" + EditCPUID->Text + L"\n无对应记录");
		}
		else {
			fread(Buf, 1, sizeof(Buf), fp);
			ShowShipInfo(Buf, sizeof(Buf));
		}
	}
	else if (EditSerialNum->Text.Length() == 14 || EditSerialNum->Text.Length()
		== 12) {
		String Msg = "";

		if (EditSerialNum->Text.Length() == 12) {
			for (int i = 0; i < 12; i += 1) {
				if (i > 0 && (i & 0x03) == 0) {
					Msg.cat_printf(L"-");
				}

				Msg.cat_printf(L"%c", *(EditSerialNum->Text.c_str() + i));
			}

			EditSerialNum->Text = Msg;
		}

		if (CheckSerialNum(fp) < 0) {
			ShowMessage(L"序列号:" + EditSerialNum->Text + L"\n无对应记录!");
		}
		else {
			fread(Buf, 1, sizeof(Buf), fp);
			ShowShipInfo(Buf, sizeof(Buf));
		}
	}
	else {
		ShowMessage(L"请填写CPUID或者12位序列号!");
	}

	fclose(fp);

	// ShowMessage("查询成功!");
}

// ---------------------------------------------------------------------------
void __fastcall TFormConfig::DisableButtons(void) {
	ButtonDownTab->Enabled = false;
	ButtonDownParam->Enabled = false;
	ButtonParamRead->Enabled = false;
	ButtonSecretInitial->Enabled = false;
	ButtonGetCPUID->Enabled = false;
	ButtonGetSta2->Enabled = false;
	ButtonFlashProtect->Enabled = false;
	ButtonForceWROff->Enabled = false;

	ButtonWriteSerialNum->Enabled = false;
	ButtonReadSerialNum->Enabled = false;

	ButtonSwitch->Enabled = false;
	ButtonGetSta->Enabled = false;

	ButtonGetScanParam->Enabled = false;

	Button16->Enabled = false;
	Button17->Enabled = false;
	Button20->Enabled = false;
	Button19->Enabled = false;
	Button21->Enabled = false;
}

// ---------------------------------------------------------------------------
void __fastcall TFormConfig::EnableButtons(void) {
	ButtonDownTab->Enabled = true;
	ButtonDownParam->Enabled = true;
	ButtonParamRead->Enabled = true;
	ButtonSecretInitial->Enabled = true;
	ButtonGetCPUID->Enabled = true;
	ButtonGetSta2->Enabled = true;
	ButtonFlashProtect->Enabled = true;
	ButtonForceWROff->Enabled = true;

	ButtonWriteSerialNum->Enabled = true;
	ButtonReadSerialNum->Enabled = true;

	ButtonSwitch->Enabled = true;
	ButtonGetSta->Enabled = true;

	ButtonGetScanParam->Enabled = true;

	Button16->Enabled = true;
	Button17->Enabled = true;
	Button20->Enabled = true;
	Button19->Enabled = true;
	Button21->Enabled = true;
}

void __fastcall TFormConfig::ButtonWriteSerialNumClick(TObject *Sender)
	// set screen
{
	uint8_t *Ptr = SerialBuf;
	FLASH_PARAM *fp = &FormUpdate->FlashParam;

	if (CheckParam() == false)
		return;

	// memset(RecordBuf, 0xFF, sizeof(RecordBuf));
	for (int i = 0; i < sizeof(SerialBuf) / 2; i += 1) {
		((uint16_t*)SerialBuf)[i] = rand();
	}

	// CPUID
	memcpy(Ptr, CPUID, sizeof(CPUID));
	Ptr += sizeof(CPUID);

	// shipping time
	memcpy(Ptr, &ShipTime, sizeof(ShipTime));
	Ptr += 4;

	// Serial Number
	strcpy(Ptr, AnsiString(EditSerialNum->Text).c_str());
	Ptr += EditSerialNum->Text.Length() + 1;

	// Type
	strcpy(Ptr, AnsiString(EditType->Text).c_str());
	Ptr += EditType->Text.Length() + 1;

	// Ptr = SerialBuf + 40;

	// //VID // 4
	// strcpy(Ptr, AnsiString(EditDispVID->Text).c_str());
	// Ptr += EditDispVID->Text.Length();
	//
	// //PID // 4
	// strcpy(Ptr, AnsiString(EditDispPID->Text).c_str());
	// Ptr += EditDispPID->Text.Length();
	//
	// //VectInfo
	// Ptr = SerialBuf + 40 + 8;
	// strcpy(Ptr, AnsiString(EditDispManuInfo->Text).c_str());
	// LengthVectInfo = EditDispManuInfo->Text.Length();
	//
	// //ProInfo
	// Ptr = SerialBuf + 40 + 8 + USB_MAX_STR_DESC_SIZ;
	// strcpy(Ptr, AnsiString(EditDispProInfo->Text).c_str());
	// LengthProInfo = EditDispProInfo->Text.Length();

	Head.MemType = TYPE_SERIAL;
	Head.Mode = MODE_PROGRAM;
	Head.StartAdd = fp->SerialStartAdd;

	((uint32_t*)(SerialBuf + fp->SerialSize - 4))[0] =
		FormUpdate->CRC32((uint32_t*)SerialBuf, fp->SerialSize / 4 - 1);

	if (MainForm->HIDSendThread->Connected == FALSE) {
		ShowMessage(L"设备未连接!");
		return;
	}

	if (MainForm->HIDSendThread->SubmitTask(TYPE_APP, fp->SerialSize, SerialBuf,
		&Head)) {
		DisableButtons();
		TimerDownTab->Enabled = true;
	}
}

// ---------------------------------------------------------------------------

void __fastcall TFormConfig::ComboBoxStdParamChange(TObject *Sender) {
	TIniFile *ini = new TIniFile(ExtractFilePath(Application->ExeName) +
		"screen.ini");
	String Section = ComboBoxStdParam->Text;
	String Gain1, Gain2, EmitPow;

	if (Section.Length() == 0)
		return;
	if (!ini)
		return;

	EditFrame->Text = ini->ReadString(Section, "Frames", "50");
	EditLowFrame->Text = ini->ReadString(Section, "LowFrames", "10");
	EditLowFrameTimeout->Text =
		ini->ReadString(Section, "LowFrameTimeout", "10");
	EditTOn->Text = ini->ReadString(Section, "TOn", "4");
	EditTOff->Text = ini->ReadString(Section, "TOff", "8");
	EditADTrig->Text = ini->ReadString(Section, "ADTrigglePoint", "55");
	EditTRise->Text = ini->ReadString(Section, "TRise", "15");

	Gain1 = ini->ReadString(Section, "Gain1", "222222222222222222222222222222");
	Gain2 = ini->ReadString(Section, "Gain2", "222222222222222222222222222222");
	EmitPow = ini->ReadString(Section, "EmitPow",
		"7777777777777777777777777777");

	if (Gain1.Length() < MAX_BOARDS)
		Gain1 += "222222222222222222222222222222";

	if (Gain2.Length() < MAX_BOARDS)
		Gain2 += "222222222222222222222222222222";

	if (EmitPow.Length() < MAX_BOARDS)
		EmitPow += "7777777777777777777777777777";

	for (int i = 1; i < StringGrid1->ColCount; i += 1) {
		StringGrid1->Cells[i][1] = Gain1.SubString(i, 1);
		StringGrid1->Cells[i][2] = Gain2.SubString(i, 1);
		StringGrid1->Cells[i][3] = EmitPow.SubString(i, 1);
	}

	CheckBoxFeatureSmooth->Checked =
		ini->ReadBool(Section, "FeatureSmooth", true);
	CheckBoxFeatureInterplation->Checked =
		ini->ReadBool(Section, "FeatureInterplation", true);
	CheckBoxFeatureMac->Checked = ini->ReadBool(Section, "FeatureMac", false);
	CheckBoxFeatureReverseX->Checked =
		ini->ReadBool(Section, "FeatureReverseX", false);
	CheckBoxFeatureReverseY->Checked =
		ini->ReadBool(Section, "FeatureReverseY", true);
	CheckBoxFeatureExchangeXY->Checked =
		ini->ReadBool(Section, "FeatureExchangeXY", false);
	CheckBoxFeatureUSARTOn->Checked =
		ini->ReadBool(Section, "FeatureUSARTOn", false);
	CheckBoxFeatureSmallPointOn->Checked =
		ini->ReadBool(Section, "FeatureSmallPointOn", false);
	CheckBoxFeatureFourceMouse->Checked =
		ini->ReadBool(Section, "FeatureFourceMouse", false);
	// CheckBoxFeatureLinkSeeProtocol->Checked = ini->ReadBool(Section, "FeatureProtocolFmt3", false);
	// CheckBoxFeatureSvaGesture->Checked = ini->ReadBool(Section, "FeatureGesture", false);
	CheckBoxFeatureBoardPoint->Checked =
		ini->ReadBool(Section, "FeatureBoardPoint", false);
	CheckBoxFeatureGainAdj->Checked =
		ini->ReadBool(Section, "FeatureGainAdj", false);
	CheckBoxFeatureEdgeAdsorption->Checked =
		ini->ReadBool(Section, "FeatureEdgeAdsorption", false);
	CheckBoxFeatureGPIOGesture->Checked =
		ini->ReadBool(Section, "FeatureGPIOGesture", false);
	CheckBoxFeatureDisableRightClick->Checked =
		ini->ReadBool(Section, "FeatureDisableRightClick", false);
	CheckBoxD2CurHigh->Checked =
		ini->ReadBool(Section, "CheckBoxD2CurHigh", false);
	CheckBoxFeatureAutoGainSub->Checked =
		ini->ReadBool(Section, "FeatureAutoGainSub", false);
	CheckBoxFeatureAutoGainUniform->Checked =
		ini->ReadBool(Section, "FeatureAutoGainUniform", false);

	CheckBoxFeatureGainAdjClick(NULL);
	EditGestureStart->Text = ini->ReadString(Section, "GestureStart", "3");
	EditGestureEnd->Text = ini->ReadString(Section, "GestureEnd", "10");

	delete ini;
}
// ---------------------------------------------------------------------------

// NLF - INTERNAL FUNCTION
// Purpose:	To perform a minor calculation of the SHA-1 algorithm
// Inputs:	Three int32 which are numbers for the calculation, and an int8 which
// is used to tell which calculation to perform
// Outputs:	An int32 which is the result of the performed calculation
uint32_t NLF(uint32_t b, uint32_t c, uint32_t d, uint32_t num) {
	uint32_t ret;

	if (num < 20)
		ret = ((b & c) | ((~b) & d));
	else if (num < 40)
		ret = (b ^ c ^ d);
	else if (num < 60)
		ret = ((b & c) | (b & d) | (c & d));
	else
		ret = (b ^ c ^ d);

	return ret;
} // NLF

// KTN - INTERNAL FUNTION
// Purpose:	To return a contant based on the input of the function, used for the
// SHA-1 algorithm
// Inputs:	An int8 which is used to decide what number to return
// Outputs:	An int32 which is one of four constants
uint32_t KTN(uint32_t num) {
	uint32_t ret;

	if (num < 20)
		ret = 0x5A827999;
	else if (num < 40)
		ret = 0x6ED9EBA1;
	else if (num < 60)
		ret = 0x8F1BBCDC;
	else
		ret = 0xCA62C1D6;

	return ret;
} // KTN

// ---------------------------------------------------------------------------
// gen_MAC - INTERNAL FUNCTION
// Purpose:	To use SHA-1 calculation to generate a message authentication code (MAC)
// Inputs:	A pointer to an array containing the 128-byte input data, and a pointer
// to an array in which the 20 byte MAC will be placed
// Outputs:	NONE
void Gen_SHA1_MAC(uint8_t* data, uint8_t *Mac) {
	uint32_t hash[5] = {
		0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
	int32_t i;
	uint32_t MTword[80];
	uint32_t ShftTmp;
	uint32_t Temp;

	for (i = 0; i < 16; i++) {
		((unsigned char *)&MTword[i])[3] = data[i * 4 + 0];
		((unsigned char *)&MTword[i])[2] = data[i * 4 + 1];
		((unsigned char *)&MTword[i])[1] = data[i * 4 + 2];
		((unsigned char *)&MTword[i])[0] = data[i * 4 + 3];
	}

	for (; i < 80; i++) {
		ShftTmp =
			(MTword[i - 3] ^ MTword[i - 8] ^ MTword[i - 14] ^ MTword[i - 16]);
		MTword[i] =
			(((ShftTmp << 1) & 0xFFFFFFFE) | ((ShftTmp >> 31) & 0x00000001));
	}

	for (i = 0; i < 80; i++) {
		ShftTmp =
			(((hash[0] << 5) & 0xFFFFFFE0) | ((hash[0] >> 27) & 0x0000001F));
		Temp = (NLF(hash[1], hash[2], hash[3], i) + hash[4] + KTN(i) +
			MTword[i] + ShftTmp);
		hash[4] = hash[3];
		hash[3] = hash[2];
		hash[2] =
			(((hash[1] << 30) & 0xC0000000) | ((hash[1] >> 2) & 0x3FFFFFFF));
		hash[1] = hash[0];
		hash[0] = Temp;
	}

	if (Mac) {
		memcpy(Mac, hash, sizeof(hash));
	}
} // gen_MAC

void __fastcall TFormConfig::ButtonSecretInitialClick(TObject *Sender) {
#if SECRET_INITIAL_ON == 1
	int i, j, Indexs;
	String Msg;
	uint8_t Index[sizeof(CPUID) * 2];
	uint8_t ShaDataBuf[128];

	FLASH_PARAM *fp = &FormUpdate->FlashParam;

	if (sizeof(SecretBuf) < fp->SecretSize) {
		ShowMessage(L"SecretBuf Too Small! Update software!");
		return;
	}

	// fill with random numbers
	for (i = 0; i < sizeof(SecretBuf) / 2; i += 1) {
		((uint16_t*)SecretBuf)[i] = rand();
	}

	do {
		Msg = "CPUID:";

		// random number for index
		for (i = 0; i < 128 / 2; i += 1) {
			((uint16_t*)SecretBuf)[i] = rand();
		}

		// Check CPUID
		for (i = 0; i < sizeof(CPUID); i += 1) {
			Msg.cat_printf(L"%.2x", CPUID[i]);
		}

		for (i = 0; i < sizeof(CPUID); i += 1) {
			if (CPUID[i] != 0)
				break;
		}

		//
		Indexs = 0;
		for (i = 0; i < 128; i += 1) {
			if (SecretBuf[i] < 128)
				continue;

			for (j = 0; j < Indexs; j += 1) {
				if (Index[j] == SecretBuf[i]) {
					break;
				}
			}

			if (j >= Indexs) {
				Index[Indexs++] = SecretBuf[i];
			}

			if (Indexs >= sizeof(CPUID) * 2)
				break;
		}
	}
	while (Indexs != sizeof(CPUID) * 2);

	// Gen SHA1
	memcpy(ShaDataBuf, SecretBuf + 256, 128);

	for (i = 0; i < sizeof(CPUID); i += 1) {
		ShaDataBuf[Index[i] - 128] = CPUID[i];
	}

	Gen_SHA1_MAC(SecretBuf + 256, SecretBuf + 256 + 128);

	// Msg += "\nSHA-1 1:";
	// for(i=0; i<20; i+=1)
	// {
	// Msg.cat_printf("%.2X", *(SecretBuf+256+128+i)&0xFF);;
	// }

	memcpy(ShaDataBuf, SecretBuf + 512, 128);

	for (i = 0; i < sizeof(CPUID); i += 1) {
		ShaDataBuf[Index[i +sizeof(CPUID)] - 128] ^= CPUID[i];
	}

	Gen_SHA1_MAC(SecretBuf + 512, SecretBuf + 512 + 128);

	// Msg += "\nSHA-1 2:";
	// for(i=0; i<20; i+=1)
	// {
	// Msg.cat_printf("%.2X", *(SecretBuf+512+128+i)&0xFF);
	// }

	for (i = 0; i < 12; i += 1) {
		SecretBuf[1024 + Index[i]] = CPUID[i];
	}

	((uint32_t*)(SecretBuf + fp->SecretSize - 4))[0] =
		FormUpdate->CRC32((uint32_t*)SecretBuf, fp->SecretSize / 4 - 1);
	((uint32_t*)(SecretBuf + fp->SecretSize - 4))[0] ^= 0xBEEFFACE;

	// Msg.cat_printf("\nCRC32:0x%8X", ((uint32_t *)(SecretBuf + SECRET_SIZE - 4))[0]);

	// ShowMessage(Msg);

	// Download to Multi-Touch Screen
	if (MainForm->HIDSendThread->Connected == FALSE) {
		ShowMessage(L"设备未连接!");
		return;
	}

	Head.MemType = TYPE_SECRET;
	Head.Mode = MODE_PROGRAM;
	Head.StartAdd = fp->SecretStartAdd;

	if (MainForm->HIDSendThread->SubmitTask(TYPE_APP, fp->SecretSize, SecretBuf,
		&Head)) {
		DisableButtons();
		TimerDownTab->Enabled = true;
	}
#endif
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::Button7Click(TObject *Sender) {
	String FileName = Application->ExeName;

	FileName = ExtractFileDir(FileName);
	FileName += "\\Serial.txt";

	FormViewDataBase->FileName = FileName;
	FormViewDataBase->Show();
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::Button11Click(TObject *Sender) {
	EditFrame->Text = "50";
	EditLowFrame->Text = "84";
	EditLowFrameTimeout->Text = "10";
	EditTOn->Text = "4";
	EditTOff->Text = "8";
	EditADTrig->Text = "55";
	EditTRise->Text = "15";

	for (int i = 1; i < StringGrid1->ColCount; i += 1) {
		StringGrid1->Cells[i][1] = "2";
		StringGrid1->Cells[i][2] = "2";

		if (BootParam.HardwareVer == 540) {
			StringGrid1->Cells[i][3] = "7";
		}
		else {
			StringGrid1->Cells[i][3] = "65";
		}
	}

	CheckBoxFeatureSmooth->Checked = true;
	CheckBoxFeatureInterplation->Checked = true;
	CheckBoxFeatureMac->Checked = false;
	CheckBoxFeatureReverseX->Checked = false;
	CheckBoxFeatureReverseY->Checked = true;
	CheckBoxFeatureExchangeXY->Checked = false;
	CheckBoxFeatureUSARTOn->Checked = false;
	CheckBoxFeatureSmallPointOn->Checked = false;
	CheckBoxFeatureFourceMouse->Checked = false;
	// CheckBoxFeatureLinkSeeProtocol->Checked = false;
	// CheckBoxFeatureSvaGesture->Checked = false;
	CheckBoxFeatureBoardPoint->Checked = false;
	CheckBoxFeatureGainAdj->Checked = false;
	CheckBoxFeatureAutoGainSub->Checked = false;
	CheckBoxFeatureAutoGainUniform->Checked = false;
	CheckBoxFeatureEdgeAdsorption->Checked = false;
	CheckBoxFeatureGPIOGesture->Checked = false;
	CheckBoxFeatureDisableRightClick->Checked = false;
	CheckBoxD2CurHigh->Checked = false;
	CheckBoxFeatureAutoGainSub->Enabled = false;
	CheckBoxFeatureAutoGainUniform->Enabled = false;

	TrackBarClick->Position = 5;
	TrackBarDoubleClick->Position = 5;

	EditGestureStart->Text = "3";
	EditGestureEnd->Text = "10";

	StatusBar1->SimpleText = L"成功导入默认值！";
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::ButtonParamReadClick(TObject *Sender) {
	StatusBar1->SimpleText = "";

	// read
	MainForm->HIDSendThread->SubmitCmd(BOOT_CMD_READ_PARAM, NULL, 0);
}
// ---------------------------------------------------------------------------

int __fastcall TFormConfig::GetMaxNum(uint8_t Num) {
	int MaxPoints = Num;

	switch (MaxPoints) {
	case 0:
		MaxPoints = 32;
		break;

	case 1:
		MaxPoints = 16;
		break;

	case 3:
		MaxPoints = 20;
		break;

	case 5:
		MaxPoints = 24;
		break;

	case 7:
		MaxPoints = 28;
		break;

	default:
		break;
	}

	return MaxPoints;
}

void __fastcall TFormConfig::Button12Click(TObject *Sender) {
	if (((TButton*)Sender) == Button12) {
		Edit93->Text = "123456";
		Edit94->Text = "闪联";

		// EditType->Text = "S4-69.5";
		if (BootParam.HardwareVer == 540)
			Edit92->Text = "T69.5-4-N";
		else if (BootParam.HardwareVer == 460)
			Edit92->Text = "W69.5-4-N";
	}
	else if (((TButton*)Sender) == Button14) {
		Edit93->Text = "123456";
		Edit94->Text = "闪联";

		// EditType->Text = "S4-70";
		if (BootParam.HardwareVer == 540)
			Edit92->Text = "T70-4-N";
		else if (BootParam.HardwareVer == 460)
			Edit92->Text = "W70-4-N";
	}

	EditCode->Text = Edit93->Text;
	EditCustomer->Text = Edit94->Text;
	EditType->Text = Edit92->Text;

	Button10Click(NULL);

	if (MainForm->HIDSendThread->Connected == FALSE) {
		ShowMessage(L"设备未连接!");
		return;
	}

	if (WriteDataBase() == true) {
		ButtonWriteSerialNumClick(NULL);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::ButtonGetScanParamClick(TObject *Sender) {
	// read
	StatusBar1->SimpleText = "";

	if (BootParam.SoftwareVer < 2010) {
		ShowMessage(L"固件版本太低，不支持该功能！");
	}
	else {
		MainForm->HIDSendThread->SubmitCmd(APP_CMD_READ_SCAN_PARAM, NULL, 0);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::Button19Click(TObject *Sender) {
	FLASH_PARAM *fp = &FormUpdate->FlashParam;
	Calibration_t *cp = &CalibParam;

	memset(cp, 0xFF, sizeof(Calibration_t));
	memset(ParamBuf, 0xFF, sizeof(ParamBuf));

	cp->Flag = 0;

	cp->xPoints = 2;
	cp->yPoints = 2;

	cp->Calib[0].x = 32768 / 2; // 中心坐标
	cp->Calib[0].y = 32768 / 2;

	cp->Calib[0].k[0] = 1.0;
	cp->Calib[0].k[1] = 0.0;
	cp->Calib[0].k[2] = 0.0;
	cp->Calib[0].k[3] = 0.0;
	cp->Calib[0].k[4] = 1.0;
	cp->Calib[0].k[5] = 0.0;

	if ((TButton *)Sender == Button19)
		memcpy(ParamBuf, cp, sizeof(Calibration_t));

	if (MainForm->HIDSendThread->Connected == FALSE) {
		ShowMessage(L"设备未连接!");
		return;
	}

	if ((TButton*)Sender == Button19)
		((uint32_t*)(ParamBuf + fp->CalibrateSize - 4))[0] =
			FormUpdate->CRC32((uint32_t*)ParamBuf, fp->CalibrateSize / 4 - 1);

	Head.MemType = TYPE_CALIB;
	Head.Mode = MODE_PROGRAM;
	Head.StartAdd = fp->CalibrateStartAdd;

	if (MainForm->HIDSendThread->SubmitTask(TYPE_APP, fp->CalibrateSize,
		ParamBuf, &Head)) {
		DisableButtons();
		TimerDownTab->Enabled = true;
	}

}

// ---------------------------------------------------------------------------
bool Calibration_ClcParam(Calibration_t *cp, Calib_Info_t *ci) {
	int i;
	int Points = ci->TotalPoints;
	Calib_Points_Info_t *Pt = ci->Calib;

	double a[3], b[3], c[3], d[3], k;

	if (Points < 3) {
		return false;
	}
	else {
		if (Points == 3) {
			for (i = 0; i < Points; i++) {
				a[i] = (double)Pt[i].x1;
				b[i] = (double)Pt[i].y1;
				c[i] = (double)Pt[i].x0;
				d[i] = (double)Pt[i].y0;
			}
		}
		else if (Points > 3) // 最小二乘解？
		{
			for (i = 0; i < 3; i++) {
				a[i] = 0;
				b[i] = 0;
				c[i] = 0;
				d[i] = 0;
			}

			for (i = 0; i < Points; i++) {
				a[0] += (double)(Pt[i].x1)*(double)(Pt[i].x1);
				a[1] += (double)(Pt[i].x1)*(double)(Pt[i].y1);
				a[2] += (double)Pt[i].x1;

				b[0] = a[1];
				b[1] += (double)(Pt[i].y1)*(double)(Pt[i].y1);
				b[2] += (double)Pt[i].y1;

				c[0] += (double)(Pt[i].x1)*(double)(Pt[i].x0);
				c[1] += (double)(Pt[i].y1)*(double)(Pt[i].x0);
				c[2] += (double)Pt[i].x0;

				d[0] += (double)(Pt[i].x1)*(double)(Pt[i].y0);
				d[1] += (double)(Pt[i].y1)*(double)(Pt[i].y0);
				d[2] += (double)Pt[i].y0;
			}

			a[0] = a[0] / a[2];
			b[0] = b[0] / a[2];
			c[0] = c[0] / a[2];
			d[0] = d[0] / a[2];

			a[1] = a[1] / b[2];
			b[1] = b[1] / b[2];
			c[1] = c[1] / b[2];
			d[1] = d[1] / b[2];

			a[2] = a[2] / Points;
			b[2] = b[2] / Points;
			c[2] = c[2] / Points;
			d[2] = d[2] / Points;
		}

		k = (a[0] - a[2]) * (b[1] - b[2]) - (a[1] - a[2]) * (b[0] - b[2]);

		if (k < 0.0001)
			return false;

		cp->Calib[0].k[0] =
			((c[0] - c[2]) * (b[1] - b[2]) - (c[1] - c[2]) * (b[0] - b[2])) / k;
		cp->Calib[0].k[1] =
			((c[1] - c[2]) * (a[0] - a[2]) - (c[0] - c[2]) * (a[1] - a[2])) / k;
		cp->Calib[0].k[2] =
			(b[0] * (a[2] * c[1] - a[1] * c[2]) +
			b[1] * (a[0] * c[2] - a[2] * c[0]) +
			b[2] * (a[1] * c[0] - a[0] * c[1])) / k;

		cp->Calib[0].k[3] =
			((d[0] - d[2]) * (b[1] - b[2]) - (d[1] - d[2]) * (b[0] - b[2])) / k;
		cp->Calib[0].k[4] =
			((d[1] - d[2]) * (a[0] - a[2]) - (d[0] - d[2]) * (a[1] - a[2])) / k;
		cp->Calib[0].k[5] =
			(b[0] * (a[2] * d[1] - a[1] * d[2]) +
			b[1] * (a[0] * d[2] - a[2] * d[0]) +
			b[2] * (a[1] * d[0] - a[0] * d[1])) / k;

		// k=(a[2]-a[0]) * (b[1]-b[0]) - (a[1]-a[2]) * (b[2]-b[0]);
		//
		// if(fabs(k) < 0.0001)	return false;
		// if(fabs(b[1] - b[0]) < 0.0001)	return false;
		//
		// cp->Calib[1].k[0] = ((c[2]-c[0])*(b[1]-b[0])-(c[1]-c[2])*(b[2]-b[0]))/k;
		// cp->Calib[1].k[1] = ((c[1]-c[0])- (a[1]-a[0])*cp->Calib[1].k[0])/(b[1] - b[0]);
		// cp->Calib[1].k[2] = c[0] - a[0]*cp->Calib[1].k[0] - b[0]*cp->Calib[1].k[1];
		//
		// cp->Calib[1].k[3] = ((d[2]-d[0])*(b[1]-b[0])-(d[1]-d[2])*(b[2]-b[0]))/k;
		// cp->Calib[1].k[4] = ((d[1]-d[0])- (a[1]-a[0])*cp->Calib[1].k[3])/(b[1] - b[0]);
		// cp->Calib[1].k[5] = d[0] - a[0]*cp->Calib[1].k[3] - b[0]*cp->Calib[1].k[4];
		return true;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormConfig::Button18Click(TObject *Sender) {
	// int Width = Screen->Width;
	// int Height = Screen->Height;
	int Temp;

	String Msg = "";

	if (MainForm->HIDSendThread->Connected == FALSE) {
		ShowMessage(L"设备未连接!");
		return;
	}

	//
	FormCalibrate->CalibInfo.xPoints = _wtoi(ComboBoxClibXPoints->Text.c_str());
	FormCalibrate->CalibInfo.yPoints = _wtoi(ComboBoxClibYPoints->Text.c_str());

	Temp = _wtoi(EditxPxiMargin->Text.c_str());

	// if(Temp < Width / 30 || Temp > Width/10)
	// {
	// Msg.cat_printf("左右边缘像素设置超出合理范围(%d,%d), 使用默认值80!\n", Width/30, Width/10);
	// FormCalibrate->xPxiMargin = 80;
	// }
	// else
	{
		FormCalibrate->xPxiMargin = Temp;
	}

	Temp = _wtoi(EdityPxiMargin->Text.c_str());

	// if(Temp < Height / 30 || Temp > Height/10)
	// {
	// Msg.cat_printf("上下边缘像素设置超出合理范围(%d,%d), 使用默认值80!\n", Height/30, Height/10);
	// FormCalibrate->yPxiMargin = 80;
	// }
	// else
	{
		FormCalibrate->yPxiMargin = Temp;
	}

	if (Msg.Length() != 0) {
		ShowMessage(Msg);
	}

	FormCalibrate->ShowModal();

	if ((FormCalibrate->CalibInfo.Flag & 0x01) != 0) {
		if (Calibration_ClcParam(&CalibParam, &FormCalibrate->CalibInfo))
			// 计算 校准系数
		{
			// 临时写入触摸屏
#if CALIB_DEBUG == 1
			MainForm->HIDSendThread->SubmitCmd(APP_CMD_SET_CALIB_COFF,
				(uint8_t *)CalibParam.Calib[0].k,
			sizeof(CalibParam.Calib[0].k));
#else

			// 写入rom
			FLASH_PARAM *fp = &FormUpdate->FlashParam;
			Calibration_t *cp = &CalibParam;

			memset(ParamBuf, 0xFF, sizeof(ParamBuf));

			cp->Flag |= CALIB_INFO_VALID;
			cp->xPoints = 2;
			cp->xPoints = 2;

			cp->Calib[0].x = 32768 / 2; // 中心坐标
			cp->Calib[0].y = 32768 / 2;

			memcpy(ParamBuf, cp, sizeof(Calibration_t));

			if (MainForm->HIDSendThread->Connected == FALSE) {
				ShowMessage(L"设备未连接!");
				return;
			}

			((uint32_t*)(ParamBuf + fp->CalibrateSize - 4))[0] =
				FormUpdate->CRC32((uint32_t*)ParamBuf,
				fp->CalibrateSize / 4 - 1);

			Head.MemType = TYPE_CALIB;
			Head.Mode = MODE_PROGRAM;
			Head.StartAdd = fp->CalibrateStartAdd;

			if (MainForm->HIDSendThread->SubmitTask(TYPE_APP, fp->CalibrateSize,
				ParamBuf, &Head)) {
				DisableButtons();
				TimerDownTab->Enabled = true;
			}
#endif

			ShowMessage(L"校准完成！");
		}
		else {
			ShowMessage(L"计算校准参数失败，请重新校准！");
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::Button21Click(TObject *Sender) {
	FILE *fp;

	if ((FormCalibrate->CalibInfo.Flag & CALIB_INFO_VALID) == 0) {
		ShowMessage(L"校准数据无效！");
		return;
	}

	if ((fp = fopen("calibration.txt", "w")) == NULL) {
		ShowMessage(L"创建文件失败！");
		return;
	}

	fprintf(fp, "%d\n", FormCalibrate->CalibInfo.TotalPoints);

	for (int i = 0; i < FormCalibrate->CalibInfo.TotalPoints; i += 1) {
		fprintf(fp, "%d ", FormCalibrate->CalibInfo.Calib[i].x0);
		fprintf(fp, "%d ", FormCalibrate->CalibInfo.Calib[i].y0);
		fprintf(fp, "%d ", FormCalibrate->CalibInfo.Calib[i].x1);
		fprintf(fp, "%d\n", FormCalibrate->CalibInfo.Calib[i].y1);
	}

	for (int i = 0; i < 6; i += 1) {
		fprintf(fp, "%f ", CalibParam.Calib[0].k[i]);
	}

	fprintf(fp, "\n");

	for (int i = 0; i < 6; i += 1) {
		fprintf(fp, "%f ", CalibParam.Calib[1].k[i]);
	}

	fprintf(fp, "\n");

	fclose(fp);
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::Button22Click(TObject *Sender) {
	if ((FormCalibrate->CalibInfo.Flag & CALIB_INFO_VALID) == 0) {
		ShowMessage(L"校准数据无效！");
		return;
	}

	FormCalibPerform->ShowModal();
}
// ---------------------------------------------------------------------------
// void __fastcall TFormConfig::Button22Click(TObject *Sender)
// {
// if((FormCalibrate->CalibInfo.Flag & CALIB_INFO_VALID) == 0)
// {
// ShowMessage(L"校准数据无效！");
// return;
// }
//
// FormCalibPerform->ShowModal();
// }

void __fastcall TFormConfig::Button16Click(TObject *Sender) {
	FLASH_PARAM *fp = &FormUpdate->FlashParam;
	RunTimeParam_t *rp = &UserParam;

	if (fp->UserParamSize == 0) {
		ShowMessage(L"无法设置用户参数，可能固件版本过低!");
		return;
	}

	rp->Frames = _wtoi(Edit91->Text.c_str());

	// send it
	memset(ParamBuf, 0xFF, sizeof(ParamBuf));
	for (int i = 0; i < sizeof(ParamBuf) / 2; i += 1) {
		((uint16_t*)ParamBuf)[i] = rand();
	}

	rp->Feature = FEATURE_STD;

	for (int i = 0; i < FeatureCheckBoxUser->Count; i += 1) {
		if (((TCheckBox*)FeatureCheckBoxUser->Items[i])->Checked == true) {
			rp->Feature |= (1 << (i + 1));
		}
	}

	rp->Feature &= ~(1 << 3); // clear mac feature

	if (sizeof(ParamBuf) < sizeof(RunTimeParam_t) + 4 || sizeof(ParamBuf) <
		fp->ParamSize) // 4bytes for crc32
	{
		ShowMessage(L"参数太多，需要修改程序！");
		return;
	}

	memcpy(ParamBuf, rp, sizeof(RunTimeParam_t));

	((uint32_t*)(ParamBuf + fp->UserParamSize - 4))[0] =
		FormUpdate->CRC32((uint32_t*)ParamBuf, fp->UserParamSize / 4 - 1);

	Head.MemType = TYPE_USER_PARAM;
	Head.Mode = MODE_PROGRAM;
	Head.StartAdd = fp->UserParamStartAdd;

	if (MainForm->HIDSendThread->Connected == FALSE) {
		ShowMessage(L"设备未连接!");
		return;
	}

	if (MainForm->HIDSendThread->SubmitTask(TYPE_APP, fp->UserParamSize,
		ParamBuf, &Head)) {
		// ButtonDownTab->Enabled = false;
		// ButtonDownParam->Enabled = false;

		DisableButtons();

		TimerDownTab->Enabled = true;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::Button17Click(TObject *Sender) {
	StatusBar1->SimpleText = "";

	// read
	MainForm->HIDSendThread->SubmitCmd(BOOT_CMD_READ_USER_PARAM, NULL, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::Button20Click(TObject *Sender) {
	StatusBar1->SimpleText = "";

	// read
	UserReq = true;
	MainForm->HIDSendThread->SubmitCmd(BOOT_CMD_READ_PARAM, NULL, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::Button23Click(TObject *Sender) {
	FLASH_PARAM *fp = &FormUpdate->FlashParam;
	// RunTimeParam_t *rp = &UserParam;

	if (fp->UserParamSize == 0) {
		ShowMessage(L"无法设置用户参数，可能固件版本过低!");
		return;
	}

	// send it
	memset(ParamBuf, 0xFF, sizeof(ParamBuf));
	// for(int i=0; i<sizeof(ParamBuf)/2; i+=1)
	// {
	// ((uint16_t *)ParamBuf)[i] = rand();
	// }

	if (sizeof(ParamBuf) < sizeof(RunTimeParam_t) + 4 || sizeof(ParamBuf) <
		fp->ParamSize) // 4bytes for crc32
	{
		ShowMessage(L"参数太多，需要修改程序！");
		return;
	}

	// memcpy(ParamBuf, rp, sizeof(RunTimeParam_t));

	// ((uint32_t *)(ParamBuf + fp->UserParamSize - 4))[0] = FormUpdate->CRC32((uint32_t *)ParamBuf, fp->UserParamSize/4 - 1);

	Head.MemType = TYPE_USER_PARAM;
	Head.Mode = MODE_PROGRAM;
	Head.StartAdd = fp->UserParamStartAdd;

	if (MainForm->HIDSendThread->Connected == FALSE) {
		ShowMessage(L"设备未连接!");
		return;
	}

	if (MainForm->HIDSendThread->SubmitTask(TYPE_APP, fp->UserParamSize,
		ParamBuf, &Head)) {
		DisableButtons();

		TimerDownTab->Enabled = true;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::Button24Click(TObject *Sender) {
	OpenDialogConfig->Filter = "配置文件(*.txt)|*.txt|配置文件(*.ini)|*.ini";

	OpenDialogConfig->FilterIndex = 1;

	if (OpenDialogConfig->Execute()) {
		String FileNamePath = OpenDialogConfig->FileName;

		TIniFile *ini = new TIniFile(FileNamePath);
		TIniFile *iniImp =
			new TIniFile(ExtractFilePath(Application->ExeName) + "screen.ini");

		if (ini && iniImp) {
			String Sels;
			String Val;
			String Key;
			String SectionKey = "";
			TStringList *Value = new TStringList;
			char *Ptr;

			FormConfigList->Caption = "选择需要导入的配置";
			FormConfigList->ini = ini;
			FormConfigList->ShowModal();

			Sels = FormConfigList->Sels;

			for (int i = 0; i < FormConfigList->SelCount; i += 1) {
				Ptr = strchr(AnsiString(Sels).c_str(), '\n');

				if (Ptr) {
					SectionKey =
						Sels.SubString(1, Ptr - AnsiString(Sels).c_str());
					Sels.Delete(1, Ptr - AnsiString(Sels).c_str() + 1);

					iniImp->ReadSectionValues(SectionKey, Value);

					if (Value->Count != 0) {
						String Msg = SectionKey;
						Msg += "\n配置已经存在，是否覆盖?";

						if (MessageBox(NULL, Msg.t_str(), L"配置覆盖",
							MB_OKCANCEL | MB_ICONWARNING) == IDCANCEL) {
							continue;
						}
					}

					ini->ReadSectionValues(SectionKey, Value);

					for (int j = 0; j < Value->Count; j += 1) {
						Key = Value->Strings[j];
						Ptr = strchr(AnsiString(Key).c_str(), '=');
						Val = Key.SubString(Ptr - AnsiString(Key).c_str() + 2,
							Key.Length() - (Ptr - AnsiString(Key).c_str() + 1));
						Key.Delete(Ptr - AnsiString(Key).c_str() + 1,
							Key.Length() - (Ptr - AnsiString(Key).c_str()));

						iniImp->WriteString(SectionKey, Key, Val);
					}
				}
				else {
					break;
				}
			}

			String Msg = ComboBoxStdScreen->Text;
			ComboBoxStdScreen->Clear();
			iniImp->ReadSections(ComboBoxStdScreen->Items);
			ComboBoxStdScreen->ItemIndex =
				ComboBoxStdScreen->Items->IndexOf(Msg);

			Msg = ComboBoxStdParam->Text;
			ComboBoxStdParam->Clear();
			iniImp->ReadSections(ComboBoxStdParam->Items);
			ComboBoxStdParam->ItemIndex = ComboBoxStdParam->Items->IndexOf(Msg);

			delete ini;
			delete iniImp;
			delete Value;

			ShowMessage(L"配置及参数导入成功");
		}
		else {
			if (ini)
				delete ini;
			if (iniImp)
				delete iniImp;

			ShowMessage(L"文件打开失败！");
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::Button25Click(TObject *Sender) {
	SaveDialogConfig->Filter = "配置文件(*.txt)|*.txt";
	SaveDialogConfig->FilterIndex = 1;

	if (SaveDialogConfig->Execute()) {
		String FileNamePath = ExpandFileName(SaveDialogConfig->FileName);
		String FileName = ExtractFileName(FileNamePath);

		if (FileName.Length() < 4 || FileName.SubString(FileName.Length() - 3,
			4) != ".txt") {
			FileNamePath += ".txt";
		}
		else {
			FileName = FileName.SubString(1, FileName.Length() - 4);
		}

		if (FileExists(FileNamePath)) {
			if (MessageBox(NULL, L"文件已经存在，需要覆盖吗?", L"确认覆盖",
				MB_OKCANCEL | MB_ICONWARNING) == IDCANCEL) {
				return;
			}

			int handle = FileCreate(FileNamePath.c_str(), fmOpenWrite);

			if (handle < 0) {
				ShowMessage(L"文件创建失败！");
				return;
			}

			FileClose(handle);
		}

		TIniFile *iniExp = new TIniFile(FileNamePath);
		TIniFile *ini = new TIniFile(ExtractFilePath(Application->ExeName) +
			"screen.ini");

		if (ini && iniExp) {
			String Sels;
			String Val;
			String Key;
			String SectionKey = "";
			TStringList *Value = new TStringList;
			char *Ptr;

			FormConfigList->Caption = "选择需要导出的配置";
			FormConfigList->ini = ini;
			FormConfigList->ShowModal();

			Sels = FormConfigList->Sels;

			for (int i = 0; i < FormConfigList->SelCount; i += 1) {
				Ptr = strchr(AnsiString(Sels).c_str(), '\n');

				if (Ptr) {
					SectionKey =
						Sels.SubString(1, Ptr - AnsiString(Sels).c_str());
					Sels.Delete(1, Ptr - AnsiString(Sels).c_str() + 1);

					ini->ReadSectionValues(SectionKey, Value);

					for (int j = 0; j < Value->Count; j += 1) {
						Key = Value->Strings[j];
						Ptr = strchr(AnsiString(Key).c_str(), '=');
						Val = Key.SubString(Ptr - AnsiString(Key).c_str() + 2,
							Key.Length() - (Ptr - AnsiString(Key).c_str() + 1));
						Key.Delete(Ptr - AnsiString(Key).c_str() + 1,
							Key.Length() - (Ptr - AnsiString(Key).c_str()));

						iniExp->WriteString(SectionKey, Key, Val);
					}
				}
				else {
					break;
				}
			}

			delete ini;
			delete iniExp;
			delete Value;

			ShowMessage(L"配置及参数导出至：\n" + FileNamePath);
		}
		else {
			if (ini)
				delete ini;
			if (iniExp)
				delete iniExp;

			ShowMessage(L"文件创建或者打开失败！");
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::Button26Click(TObject *Sender) {
	String DefFileName = "ST";
	if (GenScanTable() == false)
		return;

	SaveDialogConfig->Filter = "扫描表(*.h)|*.h";
	SaveDialogConfig->FilterIndex = 1;

	DefFileName += "_X" + String(XParam.TotalLeds);
	DefFileName += "_Y" + String(YParam.TotalLeds);
	DefFileName += "_A" + String(XParam.Axis);
	DefFileName += "_" + ComboBoxRevXVer->Text + ".h";
	SaveDialogConfig->FileName = DefFileName;

	if (SaveDialogConfig->Execute()) {
		String FileNamePath = ExpandFileName(SaveDialogConfig->FileName);
		String FileName = ExtractFileName(FileNamePath);
		FILE *fp;

		if (FileName.Length() < 3 || FileName.SubString(FileName.Length() - 1,
			2) != ".h") {
			FileNamePath += ".h";
		}
		else {
			FileName = FileName.SubString(1, FileName.Length() - 4);
		}

		if (FileExists(FileNamePath)) {
			if (MessageBox(NULL, L"文件已经存在，需要覆盖吗?", L"确认覆盖",
				MB_OKCANCEL | MB_ICONWARNING) == IDCANCEL) {
				return;
			}
		}

		if ((fp = fopen(AnsiString(FileNamePath).c_str(), "w")) == NULL) {
			ShowMessage(L"创建导出文件:\n" + FileNamePath + "\n失败!");
			return;
		}

		DefFileName = DefFileName.SubString(1, DefFileName.Length() - 2);
		for (int i = 0; i < DefFileName.Length(); i += 1) {
			if (DefFileName.c_str()[i] == '.' || DefFileName.c_str()
				[i] == '-' || DefFileName.c_str()[i] == ' ') {
				DefFileName.c_str()[i] = '_';
			}
		}

		fprintf(fp, "const unsigned char %s[] = {\n", DefFileName.c_str());

		for (uint32_t i = 0; i < TableLen; i += 1) {
			if ((i & 0x0F) == 0)
				fprintf(fp, "\t");

			fprintf(fp, "0x%.2X, ", Table[i]);

			if ((i & 0x0f) == 0x0F)
				fprintf(fp, "\n");
		}

		fprintf(fp, "\n};\n");

		fclose(fp);

		ShowMessage(L"成功导出扫描表至：\n" + FileNamePath);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::Button27Click(TObject *Sender) {
	FormBatchSet->ShowModal();
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::ButtonForceWROffClick(TObject *Sender) {
	MainForm->HIDSendThread->SubmitCmd(BOOT_CMD_WO_OFF, NULL, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::Button30Click(TObject *Sender) {
	StatusBar1->SimpleText = "";

	for (int i = 1; i < StringGrid1->ColCount; i += 1) {
		StringGrid1->Cells[i][4] = "";
	}

	MainForm->HIDSendThread->SubmitCmd(APP_CMD_GET_AUTO_GAIN, NULL, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::CheckBoxFeatureUSARTOnClick(TObject *Sender) {
	if (CheckBoxFeatureUSARTOn->Checked == true)
		ComboBoxUartReportType->Enabled = true;
	else
		ComboBoxUartReportType->Enabled = false;
}

// ---------------------------------------------------------------------------
void __fastcall TFormConfig::EditXoffsetChange(TObject *Sender) {
	// Button2Click(NULL);
	ComboBoxXDivMethodChange(NULL);
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::EditYoffsetChange(TObject *Sender) {
	// Button3Click(NULL);
	ComboBoxYDivMethodChange(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormConfig::ComboBoxYDivMethodChange(TObject *Sender) {
	float MaxYOff = _wtof(EditYoffset->Text.c_str());
	float OffStep;
	int Axis = _wtoi(ComboBoxYAxis->Text.c_str());
	String Msg;
	bool uniform = true;
	int MidAdjAxis = 1;

	switch (ComboBoxYDivMethod->ItemIndex) {
	case 1:
		uniform = true;
		MidAdjAxis = 3;
		break;
	case 2:
		uniform = true;
		MidAdjAxis = 5;
		break;
	case 3:
		uniform = false;
		MidAdjAxis = 3;
		break;
	case 4:
		uniform = false;
		MidAdjAxis = 5;
		break;
	default:
		uniform = true;
		MidAdjAxis = 1;
		break;
	}

	Msg.sprintf(L"%.2f", MaxYOff);
	// EditXoffset->Text = Msg;

	YParam.Axis = Axis;

	// XParam.Leds = _wtoi(EditXLeds->Text.c_str());
	XParam.Dis = _wtof(EditXDist->Text.c_str());
	XParam.Margin = _wtof(EditXMargin->Text.c_str());
	XParam.MarginRightDown = _wtof(EditXMarginRight->Text.c_str());

	// YParam.Leds = _wtoi(EditYLeds->Text.c_str());
	YParam.Dis = _wtof(EditYDist->Text.c_str());
	YParam.Margin = _wtof(EditYMargin->Text.c_str());
	YParam.MarginRightDown = _wtof(EditYMarginDown->Text.c_str());

	YParam.Len = XParam.Dis * (XParam.TotalLeds) + XParam.Margin +
		XParam.MarginRightDown;

	YParam.MiniOff = _wtoi(EditYMiniOff->Text.c_str());

	if (Axis == 1) {
		YParam.RequestDegOff[0] = MaxYOff;
	}
	else {
		YParam.RequestDegOff[0] = 0;
		if (MidAdjAxis >= 3) {
			YParam.RequestDegOff[1] =
				atan(-1 * YParam.Dis / YParam.Len) / PI * 180;
			YParam.RequestDegOff[2] =
				atan(1 * YParam.Dis / YParam.Len) / PI * 180;
		}

		if (MidAdjAxis >= 5) {
			YParam.RequestDegOff[3] =
				atan(-2 * YParam.Dis / YParam.Len) / PI * 180;
			YParam.RequestDegOff[4] =
				atan(2 * YParam.Dis / YParam.Len) / PI * 180;
		}

		if (Axis > MidAdjAxis) {
			if (uniform) {
				if (Axis - MidAdjAxis >= 2) {
					OffStep = MaxYOff / (int)((Axis - MidAdjAxis) / 2);
				}
				else {
					OffStep = MaxYOff;
				}

				for (int i = MidAdjAxis; i < Axis; i += 2) {
					int iTemp = (i - MidAdjAxis) / 2 + 1;
					float len;

					YParam.RequestDegOff[i + 0] = -1 * OffStep * iTemp;
					YParam.RequestDegOff[i + 1] = OffStep * iTemp;
				}
			}
			else {
				if (Axis - MidAdjAxis >= 4) {
					OffStep = MaxYOff / (int)((Axis - MidAdjAxis) / 4);
				}
				else {
					OffStep = MaxYOff;
				}

				for (int i = MidAdjAxis; i < Axis; i += 4) {
					int iTemp = (i - MidAdjAxis) / 4 + 1;
					float len;

					YParam.RequestDegOff[i + 2] = -1 * OffStep * iTemp;
					YParam.RequestDegOff[i + 3] = OffStep * iTemp;

					len = YParam.Len * tan
						(YParam.RequestDegOff[i + 3] / 180.0 * PI);

					YParam.RequestDegOff[i + 1] =
						atan((len - YParam.Dis) / YParam.Len) / PI * 180;
					YParam.RequestDegOff[i + 0] =
						-1 * YParam.RequestDegOff[i + 1];
				}
			}
		}
	}

	YParam.MaxOff = (YParam.Len * tan(MaxYOff / 180.0 * PI) + YParam.Dis / 2)
		/ YParam.Dis;

	ClcParam(YParamEdit, &YParam);
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::ComboBoxXDivMethodChange(TObject *Sender) {
	float MaxXOff = _wtof(EditXoffset->Text.c_str());
	float OffStep;
	int Axis = _wtoi(ComboBoxXAxis->Text.c_str());
	String Msg;
	bool uniform = true;
	int MidAdjAxis = 1;

	switch (ComboBoxXDivMethod->ItemIndex) {
	case 1:
		uniform = true;
		MidAdjAxis = 3;
		break;
	case 2:
		uniform = true;
		MidAdjAxis = 5;
		break;
	case 3:
		uniform = false;
		MidAdjAxis = 3;
		break;
	case 4:
		uniform = false;
		MidAdjAxis = 5;
		break;
	default:
		uniform = true;
		MidAdjAxis = 1;
		break;
	}

	Msg.sprintf(L"%.2f", MaxXOff);
	// EditXoffset->Text = Msg;

	XParam.Axis = Axis;

	// XParam.Leds = _wtoi(EditXLeds->Text.c_str());
	XParam.Dis = _wtof(EditXDist->Text.c_str());
	XParam.Margin = _wtof(EditXMargin->Text.c_str());
	XParam.MarginRightDown = _wtof(EditXMarginRight->Text.c_str());

	// YParam.Leds = _wtoi(EditYLeds->Text.c_str());
	YParam.Dis = _wtof(EditYDist->Text.c_str());
	YParam.Margin = _wtof(EditYMargin->Text.c_str());
	YParam.MarginRightDown = _wtof(EditYMarginDown->Text.c_str());

	XParam.Len = YParam.Dis * (YParam.TotalLeds) + YParam.Margin +
		YParam.MarginRightDown;

	XParam.MiniOff = _wtoi(EditXMiniOff->Text.c_str());

	if (Axis == 1) {
		XParam.RequestDegOff[0] = MaxXOff;
	}
	else {
		XParam.RequestDegOff[0] = 0;
		if (MidAdjAxis >= 3) {
			XParam.RequestDegOff[1] =
				atan(-1 * XParam.Dis / XParam.Len) / PI * 180;
			XParam.RequestDegOff[2] =
				atan(1 * XParam.Dis / XParam.Len) / PI * 180;
		}

		if (MidAdjAxis >= 5) {
			XParam.RequestDegOff[3] =
				atan(-2 * XParam.Dis / XParam.Len) / PI * 180;
			XParam.RequestDegOff[4] =
				atan(2 * XParam.Dis / XParam.Len) / PI * 180;
		}

		if (Axis > MidAdjAxis) {
			if (uniform) {
				if (Axis - MidAdjAxis >= 2) {
					OffStep = MaxXOff / (int)((Axis - MidAdjAxis) / 2);
				}
				else {
					OffStep = MaxXOff;
				}

				for (int i = MidAdjAxis; i < Axis; i += 2) {
					int iTemp = (i - MidAdjAxis) / 2 + 1;
					float len;

					XParam.RequestDegOff[i + 0] = -1 * OffStep * iTemp;
					XParam.RequestDegOff[i + 1] = OffStep * iTemp;
				}
			}
			else {
				if (Axis - MidAdjAxis >= 4) {
					OffStep = MaxXOff / (int)((Axis - MidAdjAxis) / 4);
				}
				else {
					OffStep = MaxXOff;
				}

				for (int i = MidAdjAxis; i < Axis; i += 4) {
					int iTemp = (i - MidAdjAxis) / 4 + 1;
					float len;

					XParam.RequestDegOff[i + 2] = -1 * OffStep * iTemp;
					XParam.RequestDegOff[i + 3] = OffStep * iTemp;

					len = XParam.Len * tan
						(XParam.RequestDegOff[i + 3] / 180.0 * PI);

					XParam.RequestDegOff[i + 1] =
						atan((len - XParam.Dis) / XParam.Len) / PI * 180;
					XParam.RequestDegOff[i + 0] =
						-1 * XParam.RequestDegOff[i + 1];
				}
			}
		}
	}

	XParam.MaxOff = (XParam.Len * tan(MaxXOff / 180.0 * PI) + XParam.Dis / 2)
		/ XParam.Dis;

	ClcParam(XParamEdit, &XParam);
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::CheckBox17Click(TObject *Sender) {
	EditXBoardLed->Enabled = CheckBox17->Checked;
	ComboBoxXBoards->Enabled = !CheckBox17->Checked;

	if (ComboBoxXBoards->Enabled) {
		// ComboBoxXBoardsChange(ComboBoxXBoards);
		int Cuts = XCutBoards->Count / 2;
		int i;
		for (i = 0; i < XParam.Boards && i < Cuts; i += 1) {
			String Msg = String(XParam.Boards - i);

			((TComboBox *)XCutBoards->Items[i])->Enabled = true;
			((TEdit *)XCutBoards->Items[i + Cuts])->Enabled = true;
			((TComboBox*)XCutBoards->Items[i])->ItemIndex =
				((TComboBox*)XCutBoards->Items[i])->Items->IndexOf(Msg);
		}

		for (i = XParam.Boards; i < Cuts; i += 1) {
			((TComboBox *)XCutBoards->Items[i])->Enabled = false;
			((TEdit *)XCutBoards->Items[i + Cuts])->Enabled = false;
		}
	}
	else {
		for (int i = 0; i < XCutBoards->Count / 2; i += 1) {
			((TComboBox*)XCutBoards->Items[i])->Enabled = !CheckBox17->Checked;
			((TEdit*)XCutBoards->Items[i + XCutBoards->Count / 2])->Enabled =
				!CheckBox17->Checked;
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::CheckBox19Click(TObject *Sender) {
	EditYBoardLed->Enabled = CheckBox19->Checked;
	ComboBoxYBoards->Enabled = !CheckBox19->Checked;

	if (ComboBoxYBoards->Enabled) {
		// ComboBoxYBoardsChange(ComboBoxYBoards);
		int Cuts = YCutBoards->Count / 2;
		int i;
		for (i = 0; i < YParam.Boards && i < Cuts; i += 1) {
			String Msg = String(YParam.Boards - i);

			((TComboBox *)YCutBoards->Items[i])->Enabled = true;
			((TEdit *)YCutBoards->Items[i + Cuts])->Enabled = true;
			((TComboBox*)YCutBoards->Items[i])->ItemIndex =
				((TComboBox*)YCutBoards->Items[i])->Items->IndexOf(Msg);
		}

		for (i = YParam.Boards; i < Cuts; i += 1) {
			((TComboBox *)YCutBoards->Items[i])->Enabled = false;
			((TEdit *)YCutBoards->Items[i + Cuts])->Enabled = false;
		}
	}
	else {
		for (int i = 0; i < YCutBoards->Count / 2; i += 1) {
			((TComboBox*)YCutBoards->Items[i])->Enabled = !CheckBox19->Checked;
			((TEdit*)YCutBoards->Items[i + XCutBoards->Count / 2])->Enabled =
				!CheckBox19->Checked;
		}
	}
}

int __fastcall TFormConfig::ToBoardLeds(uint8_t *str, int* BoardLed) {
	int Led = 0;
	int boards = 0;

	while (*str != '\0') {
		if (*str <= '9' && *str >= '0') {
			Led = Led * 10 + (*str - '0');
		}
		else {
			if (Led != 0) {
				BoardLed[boards++] = Led;
				if (boards >= MAX_BOARDS) {
					break;
				}
			}
			Led = 0;
		}

		str++;
	}

	if (Led && boards < MAX_BOARDS) {
		BoardLed[boards++] = Led;
	}

	return boards;
}

// ---------------------------------------------------------------------------
void __fastcall TFormConfig::EditXBoardLedChange(TObject *Sender) {
	int Led = 0;

	if (CheckBox17->Checked) {
		XParam.Boards = ToBoardLeds(AnsiString(EditXBoardLed->Text).c_str(),
			XParam.BoradLeds);

		for (int i = 0; i < XParam.Boards; i += 1) {
			Led += XParam.BoradLeds[i];
		}

		EditXLeds->Text = String(Led);
		ComboBoxXBoards->Text = String(XParam.Boards);
	}

	// ComboBoxXBoardsChange(NULL);
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::EditYBoardLedChange(TObject *Sender) {
	int Led = 0;

	if (CheckBox19->Checked) {
		YParam.Boards = ToBoardLeds(AnsiString(EditYBoardLed->Text).c_str(),
			YParam.BoradLeds);

		for (int i = 0; i < YParam.Boards; i += 1) {
			Led += YParam.BoradLeds[i];
		}

		EditYLeds->Text = String(Led);
		ComboBoxYBoards->Text = String(YParam.Boards);

		// ComboBoxYBoardsChange(NULL);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::FormActivate(TObject *Sender) {
	if (this->BootParam.HardwareVer == 540 ||
		this->BootParam.HardwareVer == 460) {
		StringGrid1->Cells[0][3] = "发射功率";
	}
	else {
		StringGrid1->Cells[0][3] = "采样点";
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::EditXScanGapChange(TObject *Sender) {
	//
}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::ShowManuProInfo(uint8_t *Buf, uint16_t Len) {
	// String Msg;
	// char *Ptr = Buf;
	//
	// Msg.sprintf(L"%s", String(Ptr));
	// EditDispManuInfo->Text = Msg;
	// Ptr += USB_MAX_STR_DESC_SIZ;
	// Msg.sprintf(L"%s", String(Ptr));
	// EditDispProInfo->Text = Msg;

}
// ---------------------------------------------------------------------------

void __fastcall TFormConfig::ButtonReadSerialNumClick(TObject *Sender) {
	// EditDispVID->Text = "";
	// EditDispPID->Text = "";
	// EditDispManuInfo->Text = "";
	// EditDispProInfo->Text = "";

	MainForm->HIDSendThread->SubmitCmd(BOOT_CMD_READ_SERIAL, NULL, 0);
	MainForm->HIDSendThread->SubmitCmd(APP_CMD_GET_MANUPRO_INFO, NULL, 0);
}

// ---------------------------------------------------------------------------
void __fastcall TFormConfig::CheckBoxFeatureGainAdjClick(TObject *Sender) {
	if (CheckBoxFeatureGainAdj->Checked) {
		CheckBoxFeatureAutoGainSub->Enabled = true;
		CheckBoxFeatureAutoGainUniform->Enabled = true;
	}
	else {
		CheckBoxFeatureAutoGainSub->Enabled = false;
		CheckBoxFeatureAutoGainUniform->Enabled = false;
	}
}

void __fastcall TFormConfig::CheckBoxWorkTimeClick(TObject *Sender) {
	if (CheckBoxWorkTime->Checked) {
		EditWorkTime->Enabled = true;
		Button41->Enabled = true;
	}
	else {
		EditWorkTime->Enabled = false;
		Button41->Enabled = false;
	}
}
// ---------------------------------------------------------------------------
