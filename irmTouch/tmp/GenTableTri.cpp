//---------------------------------------------------------------------------


#pragma hdrstop

#include "GenTableTri.h"
#include "stdint.h"

//---------------------------------------------------------------------------
const uint32_t RevSda_High = (1<<0);
const uint32_t RevSda_Low = (0<<0);
const uint32_t RevSck_High = (1<<1);
const uint32_t RevSck_Low = (0<<1);
const uint32_t EmitSda_High = (1<<2);
const uint32_t EmitSda_Low = (0<<2);
const uint32_t EmitSck_High = (1<<3);
const uint32_t EmitSck_Low = (0<<3);
const uint32_t EmitSega = (1<<4);
const uint32_t EmitSegb = (1<<5);
const uint32_t EmitSegc = (1<<6);
const uint32_t EmitSegMap[8] = {(0<<4),(1<<4), (2<<4), (3<<4),
								  (4<<4),(5<<4), (6<<4), (7<<4)};		//abc

const uint32_t EmitRst_High = (1<<7);
const uint32_t EmitRst_Low = (0<<7);

const uint32_t InvRevSda = (1<<0);
const uint32_t InvRevSck = (1<<1);
const uint32_t InvEmitSda = (1<<2);
const uint32_t InvEmitSck = (1<<3);
const uint32_t InvEmitSega = (1<<4);
const uint32_t InvEmitSegb = (1<<5);
const uint32_t InvEmitSegc = (1<<6);
const uint32_t InvEmitRst = (1<<7);

const uint32_t RightRevSda_High = (1<<0);
const uint32_t RightRevSda_Low = (0<<0);
const uint32_t RightRevSck_High = (1<<1);
const uint32_t RightRevSck_Low = (0<<1);
const uint32_t RightEmitSda_High = (1<<2);
const uint32_t RightEmitSda_Low = (0<<2);
const uint32_t RightEmitSck_High = (1<<3);
const uint32_t RightEmitSck_Low = (0<<3);
const uint32_t RightEmitSega = (1<<4);
const uint32_t RightEmitSegb = (1<<5);
const uint32_t RightEmitSegc = (1<<6);
const uint32_t RightEmitSegMap[8] = {(0<<4),(1<<4), (2<<4), (3<<4),
								  (4<<4),(5<<4), (6<<4), (7<<4)};		//abc

const uint32_t RightEmitRst_High = (1<<7);
const uint32_t RightEmitRst_Low = (0<<7);

const uint32_t LeftRevSda_High = (1<<8);
const uint32_t LeftRevSda_Low = (0<<8);
const uint32_t LeftRevSck_High = (1<<9);
const uint32_t LeftRevSck_Low = (0<<9);
const uint32_t LeftEmitSda_High = (1<<10);
const uint32_t LeftEmitSda_Low = (0<<10);
const uint32_t LeftEmitSck_High = (1<<11);
const uint32_t LeftEmitSck_Low = (0<<11);
const uint32_t LeftEmitSega = (1<<12);
const uint32_t LeftEmitSegb = (1<<13);
const uint32_t LeftEmitSegc = (1<<14);
const uint32_t LeftEmitSegMap[8] = {(0<<12),(1<<12), (2<<12), (3<<12),
								  (4<<12),(5<<12), (6<<12), (7<<12)};		//abc

const uint32_t LeftEmitRst_High = (1<<15);
const uint32_t LeftEmitRst_Low = (0<<15);

const uint32_t GPIO_PIN_MAP_V15[16] =
{
	9,   8, 6, 7, 12, 11, 10, 4,
	14, 13, 1, 0,  5,  3,  2, 4,
};

const uint16_t EmitSegMap_V15[8] ={
	6, 5, 4, 3, 2, 1, 7, 0
};

const uint16_t LOGIC_V15[16] =
{
	InvRevSda,
	0,//RevSck_High,
	0,//EmitSda_High,
	0,//EmitSck_High,
	0,0,0,
	InvEmitRst,

	InvRevSda,
	0,//RevSck_High,
	0,//EmitSda_High,
	0,//EmitSck_High,
	0,0,0,
	InvEmitRst,
};

const uint16_t LOGIC_V15_[16] =
{
	0,//RightRevSda_High,
	0,//RightRevSck_High,
	0,//RightEmitSda_High,
	0,//RightEmitSck_High,
	0,0,0,
	0,//EmitRst_High,

	0,//LeftRevSda_High,
	0,//LeftRevSck_High,
	0,//LeftEmitSda_High,
	0,//LeftEmitSck_High,
	0,0,0,
	0,//LeftEmitRst_High,
};

uint32_t GPIO_Map_(uint16_t Data, uint32_t *PinMap, uint16_t *Logic, bool EmitLeft)
{
	uint32_t GPIOBSRRReg = 0;

	if((Data & RightRevSda_High) ^ Logic[0]) GPIOBSRRReg |= (1 << PinMap[0]);
	if((Data & RightRevSck_High) ^ Logic[1]) GPIOBSRRReg |= (1 << PinMap[1]);

	if((Data & RightEmitSda_High) ^ Logic[2]) GPIOBSRRReg |= (1 << PinMap[2]);
	if((Data & RightEmitSck_High) ^ Logic[3]) GPIOBSRRReg |= (1 << PinMap[3]);
	if((Data & RightEmitSega) ^ Logic[4]) GPIOBSRRReg |= (1 << PinMap[4]);
	if((Data & RightEmitSegb) ^ Logic[5]) GPIOBSRRReg |= (1 << PinMap[5]);
	if((Data & RightEmitSegc) ^ Logic[6]) GPIOBSRRReg |= (1 << PinMap[6]);
	if((Data & RightEmitRst_High) ^ Logic[7]) GPIOBSRRReg |= (1 << PinMap[7]);

	if((Data & LeftRevSda_High) ^ Logic[8]) GPIOBSRRReg |= (1 << PinMap[8]);
	if((Data & LeftRevSck_High) ^ Logic[9]) GPIOBSRRReg |= (1 << PinMap[9]);

	if((Data & LeftEmitSda_High) ^ Logic[10]) GPIOBSRRReg |= (1 << PinMap[10]);
	if((Data & LeftEmitSck_High) ^ Logic[11]) GPIOBSRRReg |= (1 << PinMap[11]);
	if((Data & LeftEmitSega) ^ Logic[12]) GPIOBSRRReg |= (1 << PinMap[12]);
	if((Data & LeftEmitSegb) ^ Logic[13]) GPIOBSRRReg |= (1 << PinMap[13]);
	if((Data & LeftEmitSegc) ^ Logic[14]) GPIOBSRRReg |= (1 << PinMap[14]);
	if((Data & LeftEmitRst_High) ^ Logic[15]) GPIOBSRRReg |= (1 << PinMap[15]);

	GPIOBSRRReg = ((~GPIOBSRRReg) << 16) | GPIOBSRRReg;

	return GPIOBSRRReg;
}

uint32_t GPIO_Map(uint16_t Data, uint32_t *PinMap, uint16_t *Logic, bool EmitLeft)
{
	uint32_t GPIOBSRRReg = 0;

	if(EmitLeft)
	{
		//Rev Left Emit right default value
		if(Logic[8]) GPIOBSRRReg |= (1 << PinMap[8]);
		if(Logic[9]) GPIOBSRRReg |= (1 << PinMap[9]);
		if(Logic[2]) GPIOBSRRReg |= (1 << PinMap[2]);
		if(Logic[3]) GPIOBSRRReg |= (1 << PinMap[3]);
		if(Logic[4]) GPIOBSRRReg |= (1 << PinMap[4]);
		if(Logic[5]) GPIOBSRRReg |= (1 << PinMap[5]);
		if(Logic[6]) GPIOBSRRReg |= (1 << PinMap[6]);
		//if(Logic[7]) GPIOBSRRReg |= (1 << PinMap[7]);

		//Rev right Emit left value
		if((Data & RevSda_High) ^ Logic[0]) GPIOBSRRReg |= (1 << PinMap[0]);
		if((Data & RevSck_High) ^ Logic[1]) GPIOBSRRReg |= (1 << PinMap[1]);

		if((Data & EmitSda_High) ^ Logic[10]) GPIOBSRRReg |= (1 << PinMap[10]);
		if((Data & EmitSck_High) ^ Logic[11]) GPIOBSRRReg |= (1 << PinMap[11]);
		if((Data & EmitSega) ^ Logic[12]) GPIOBSRRReg |= (1 << PinMap[12]);
		if((Data & EmitSegb) ^ Logic[13]) GPIOBSRRReg |= (1 << PinMap[13]);
		if((Data & EmitSegc) ^ Logic[14]) GPIOBSRRReg |= (1 << PinMap[14]);
		if((Data & EmitRst_High) ^ Logic[15]) GPIOBSRRReg |= (1 << PinMap[15]);
	}
	else
	{
		//Rev right Emit left default value
		if(Logic[0]) GPIOBSRRReg |= (1 << PinMap[0]);
		if(Logic[1]) GPIOBSRRReg |= (1 << PinMap[1]);
		if(Logic[10]) GPIOBSRRReg |= (1 << PinMap[10]);
		if(Logic[11]) GPIOBSRRReg |= (1 << PinMap[11]);
		if(Logic[12]) GPIOBSRRReg |= (1 << PinMap[12]);
		if(Logic[13]) GPIOBSRRReg |= (1 << PinMap[13]);
		if(Logic[14]) GPIOBSRRReg |= (1 << PinMap[14]);
		//if(Logic[15]) GPIOBSRRReg |= (1 << PinMap[15]);

		//Rev Left Emit right value
		if((Data & RevSda_High) ^ Logic[8]) GPIOBSRRReg |= (1 << PinMap[8]);
		if((Data & RevSck_High) ^ Logic[9]) GPIOBSRRReg |= (1 << PinMap[9]);

		if((Data & EmitSda_High) ^ Logic[2]) GPIOBSRRReg |= (1 << PinMap[2]);
		if((Data & EmitSck_High) ^ Logic[3]) GPIOBSRRReg |= (1 << PinMap[3]);
		if((Data & EmitSega) ^ Logic[4]) GPIOBSRRReg |= (1 << PinMap[4]);
		if((Data & EmitSegb) ^ Logic[5]) GPIOBSRRReg |= (1 << PinMap[5]);
		if((Data & EmitSegc) ^ Logic[6]) GPIOBSRRReg |= (1 << PinMap[6]);
		if((Data & EmitRst_High) ^ Logic[7]) GPIOBSRRReg |= (1 << PinMap[7]);
	}

	GPIOBSRRReg = ((~GPIOBSRRReg) << 16) | GPIOBSRRReg;

	return GPIOBSRRReg;
}

uint16_t ActualPos(uint16_t EmitRevPos, bool XFirst, uint16_t Align)
{
	uint16_t Pos = EmitRevPos;
	uint16_t ActualPos = 0;
	uint16_t Board;

	AXI_PARAM_TRI *First = &XParamTri;
	AXI_PARAM_TRI *Second = &YParamTri;

	if(!XFirst)
	{
		First = &YParamTri;
		Second = &XParamTri;
	}

	for(int i=0; i<First->Boards; i+=1)
	{
		uint16_t BoardEmitLeds = First->PCBs[i].EmitLeds;

		if(Pos >= BoardEmitLeds)
		{
			Pos -= BoardEmitLeds;

			BoardEmitLeds += (Align - 1);             //group align
			BoardEmitLeds = BoardEmitLeds - BoardEmitLeds % Align;
			ActualPos += BoardEmitLeds;
		}
		else
		{
			ActualPos += Pos;
			return ActualPos;
		}
	}

	for(int i=0; i<Second->Boards; i+=1)
	{
		uint16_t BoardEmitLeds = Second->PCBs[i].EmitLeds;

		if(Pos > BoardEmitLeds)
		{
			Pos -= BoardEmitLeds;

			BoardEmitLeds += (Align - 1);             //group align
			BoardEmitLeds = BoardEmitLeds - BoardEmitLeds % Align;
			ActualPos += BoardEmitLeds;
		}
		else
		{
			ActualPos += Pos;
			return ActualPos;
		}
	}

	return ActualPos;
}

uint16_t Switch_Group(uint16_t DefaultSignal, uint16_t Sck_High, uint32_t *GPIOSwitch, uint16_t *TimerArr, bool EmitLeft)
{
	uint16_t Signal;

	Signal = DefaultSignal;
	*GPIOSwitch++ = GPIO_Map(Signal, (uint32_t *)GPIO_PIN_MAP_V15, (uint16_t *)LOGIC_V15, EmitLeft);
	*TimerArr++ = EMIT_SW_FAST - 1;

	Signal = DefaultSignal | Sck_High;
	*GPIOSwitch = GPIO_Map(Signal, (uint32_t *)GPIO_PIN_MAP_V15, (uint16_t *)LOGIC_V15, EmitLeft);
	*TimerArr = EMIT_SW_FAST - 1;

	return 2;
}

uint32_t Scan_Emits_for_One_Rev(uint32_t Flags, uint16_t RevGroupSw, AXI_PARAM_TRI *ept, uint16_t EmitOff, bool FirstEmit, uint16_t RevNum,
					 uint32_t *GPIOSwitch, uint16_t *TimerArr, uint32_t *GainTimerArr, uint32_t *Pulses_)
{
	uint32_t Time = 0;
	uint32_t Pulses = *Pulses_;
	uint16_t Signal;
	uint16_t EmitTime;
	bool EmitLeft;
	bool doublefirst = ((Flags & 0x01) != 0);

	if(FirstEmit == X_FIRST)	EmitLeft = false;
	else	EmitLeft = true;

	EmitTime = (ept->TOn + ept->TOff)*CLK_FOR_1us/4;

	//select Rev Led
	while(RevGroupSw --)
	{
//		if(Time == 0)
//		{
//			Signal = EmitRst_High;
//			GPIOSwitch[Pulses] = GPIO_Map(Signal, (uint32_t *)GPIO_PIN_MAP_V15, (uint16_t *)LOGIC_V15, EmitLeft);
//			TimerArr[Pulses] = (ept->TOn + ept->TOff)*CLK_FOR_1us/4 - 1;
//			Pulses += 1;
//
//			Signal = EmitRst_High | RevSck_High;
//			GPIOSwitch[Pulses]  = GPIO_Map(Signal, (uint32_t *)GPIO_PIN_MAP_V15, (uint16_t *)LOGIC_V15, EmitLeft);
//			TimerArr[Pulses] = EMIT_SW_FAST - 1;
//			Pulses += 1;
//
//			Time += EMIT_SW_FAST + (ept->TOn + ept->TOff)*CLK_FOR_1us/4;
//		}
//		else
		{
			Signal = EmitRst_High;
			Switch_Group(Signal, RevSck_High, &GPIOSwitch[Pulses], &TimerArr[Pulses], EmitLeft);
			Pulses += 2;

			Time += EMIT_SW_FAST * 2;
		}
	}

	//select right emit
	uint16_t EmitGroupLastPos = EMIT_GROUP_LEDS - 1;
	uint16_t TargetEmitPos = ActualPos(ept->EmitStart[RevNum] + EmitOff, FirstEmit, EMIT_GROUP_LEDS);

	Signal = EmitSda_High;
	Switch_Group(Signal, EmitSck_High, &GPIOSwitch[Pulses], &TimerArr[Pulses], EmitLeft);
	Pulses += 2;

	Time += EMIT_SW_FAST * 2;

	//select emit group
	while(TargetEmitPos > EmitGroupLastPos)
	{
		Switch_Group(0, EmitSck_High, &GPIOSwitch[Pulses], &TimerArr[Pulses], EmitLeft);
		Pulses += 2;

		Time += EMIT_SW_FAST * 2;

		EmitGroupLastPos += EMIT_GROUP_LEDS;
	}

	//int WaitTime = (uint32_t)(ept->TWait) * CLK_FOR_1us / 4 - Time;
	//int WaitClk = WaitTime
	//Rev switch wait Time check
	int RevWaitClk = EMIT_SW_FAST;
	//int RevWaitClk = CLK_FOR_1us;
	//int RevWaitClk = ept->TOn * CLK_FOR_1us / 4 - EMIT_SW_FAST;

	while(Time < (uint32_t)(ept->TWait) * CLK_FOR_1us / 4)       //wait after rev led switched
	{
		Signal = 0;
		GPIOSwitch[Pulses] = GPIO_Map(Signal, (uint32_t *)GPIO_PIN_MAP_V15, (uint16_t *)LOGIC_V15, EmitLeft);
		TimerArr[Pulses] = RevWaitClk - 1;
		Pulses += 1;

		Time += RevWaitClk;
	}

	Signal = 0;
	GPIOSwitch[Pulses] = GPIO_Map(Signal, (uint32_t *)GPIO_PIN_MAP_V15, (uint16_t *)LOGIC_V15, EmitLeft);
	TimerArr[Pulses] = EMIT_SW_FAST - 1;
	Pulses += 1;

	Time += EMIT_SW_FAST;

	//switch emit led
	bool InsertClkForEmitGroupSwitch;

	for(int Emit=ept->EmitStart[RevNum]; Emit<ept->EmitEnd[RevNum]; Emit+=1)
	{
		uint16_t ActualEmit = ActualPos(Emit + EmitOff, FirstEmit, EMIT_GROUP_LEDS);
		uint16_t EmitSeg = ActualEmit % EMIT_GROUP_LEDS;

		InsertClkForEmitGroupSwitch = false;

		if(ActualEmit > EmitGroupLastPos) {
			//Move to Next Group
			if((Signal & EmitSck_High) != 0) {
				//insert one fast clk here
//				Signal = EmitSck_Low | EmitSegMap[EmitSeg];
//				GPIOSwitch[Pulses] = GPIO_Map(Signal, (uint32_t *)GPIO_PIN_MAP_V15, (uint16_t *)LOGIC_V15, EmitLeft);
//				TimerArr[Pulses] = EMIT_SW_FAST - 1;
//				Pulses += 1;
//
//				Time += EMIT_SW_FAST;
//
//				EmitSck_High
				Signal = EmitSck_Low;
				InsertClkForEmitGroupSwitch = true;
			} else {
				Signal = EmitSck_High;
			}

			EmitGroupLastPos += EMIT_GROUP_LEDS;
		} else {
			Signal = EmitSck_Low;
		}

		Signal |= EmitSegMap[EmitSegMap_V15[EmitSeg]];

		GPIOSwitch[Pulses] = GPIO_Map(Signal, (uint32_t *)GPIO_PIN_MAP_V15, (uint16_t *)LOGIC_V15, EmitLeft);

		if(Emit == ept->EmitStart[RevNum]) {
			TimerArr[Pulses] = EMIT_SW_FAST - 1;
			Pulses += 1;
			Time += EMIT_SW_FAST;

			if(doublefirst){
				GPIOSwitch[Pulses] = GPIOSwitch[Pulses-1];

				TimerArr[Pulses] = EmitTime - 1;
				Pulses += 1;
				Time += EmitTime;
			}
		} else {
			TimerArr[Pulses] = EmitTime - 1;
			Pulses += 1;
			Time += EmitTime;

			if(InsertClkForEmitGroupSwitch)
			{
				Signal |= EmitSck_High;
				GPIOSwitch[Pulses] = GPIO_Map(Signal, (uint32_t *)GPIO_PIN_MAP_V15, (uint16_t *)LOGIC_V15, EmitLeft);
				TimerArr[Pulses] = EMIT_SW_FAST - 1;
				Pulses += 1;

				Time += EMIT_SW_FAST;
			}
		}
	}

	GPIOSwitch[Pulses] = GPIO_Map(0, (uint32_t *)GPIO_PIN_MAP_V15, (uint16_t *)LOGIC_V15, EmitLeft);
	TimerArr[Pulses] = EmitTime - 1;
	Pulses += 1;
	Time += EmitTime;

	*Pulses_ = Pulses;

	return Time;
}

void TextWave(char *Name, uint32_t *Gpio, uint16_t *TimerArr, uint32_t Pulses, uint32_t Pin, FILE *fp)
{
	uint32_t PinSta = 0;

	fprintf(fp, "\n%s", Name);

	for(uint32_t i=0; i<Pulses; i+=1)
	{
		if(TimerArr[i] == EMIT_SW_FAST - 1)
		{
			if(PinSta == (Gpio[i] & (1 << Pin)))
			{
				if(PinSta)
					fprintf(fp, "TTTTT");
				else
					fprintf(fp, "_____");
			}
			else
			{
				if(PinSta)
					fprintf(fp, "TT|__");
				else
					fprintf(fp, "__|TT");
			}
		}
		else
		{
			if(PinSta == (Gpio[i] & (1 << Pin)))
			{
				if(PinSta)
					fprintf(fp, "TTTTTTTTTT");
				else
					fprintf(fp, "__________");
			}
			else
			{
				if(PinSta)
					fprintf(fp, "TTTTTTT|__");
				else
					fprintf(fp, "_______|TT");
			}
		}

		PinSta = Gpio[i] & (1 << Pin);
	}
}

//uint32_t Generate_Table_V15(uint32_t Flags, uint32_t *GPIOSwitch, uint16_t *TimerArr, uint16_t *GainTimerArr)
int Generate_Table_V15(uint32_t Flags, SCAN_TABLE_HEAD_TRI *sth, uint32_t TabBaseAdd, uint8_t *Tab)
{
	uint32_t Time = 0;
	uint16_t RevNum;
	uint16_t RevGroupLastPos;
	uint16_t BoardNum = 0;
	uint32_t BoardTime = 0;
	uint32_t Pulses;
	uint16_t Signal = 0;
	uint32_t PulseMark[10] = {0};
	bool doublefirst = ((Flags & 0x01) != 0);

	AXI_PARAM_TRI *xpt = &XParamTri;
	AXI_PARAM_TRI *ypt = &YParamTri;
	bool EmitLeft;

	uint32_t *GPIOSwitch;
	uint16_t *TimerArr;
	uint32_t *GainTimerArr;

	TimerArr = (uint16_t *)(Tab + ((uint32_t)(sth->TimerArrTable) - TabBaseAdd));
	GPIOSwitch = (uint32_t *)(Tab + ((uint32_t)(sth->GpioSwitchTable) - TabBaseAdd));
	GainTimerArr = (uint32_t *)(Tab + ((uint32_t)(sth->GainTimerArrTable) - TabBaseAdd));

	//所有信号高电平有效  GPIO_Map 负责逻辑电平装换   比如由高电平有效变为低电平有效
//--------------------------------------------------------------------------------------------------
//------------------------ Left Receive Right Emit -------------------------------------------------
//--------------------------------------------------------------------------------------------------
	RevGroupLastPos = REV_GROUP_LEDS - 1;		//select first group  last rev led
	RevNum = 0;
	Pulses = 0;

	Signal = RevSda_High | EmitRst_High;          
	Switch_Group(Signal, RevSck_High, &GPIOSwitch[Pulses], &TimerArr[Pulses], EMIT_RIGHT);
	Pulses += 2;

	BoardTime += EMIT_SW_FAST * 2;

	for(int i=0; i<ypt->Boards; i+=1)
	{
		PCB_CONFIG *pcb = &(ypt->PCBs[i]);

		for(int j = 0; j<pcb->RevLeds; j+=1)
		{
			uint16_t TargetRevPos = ActualPos(ypt->RevPos[RevNum], Y_FIRST, REV_GROUP_LEDS);
			uint16_t RevGroup = 0;

			if(TargetRevPos > RevGroupLastPos)
			{
				RevGroup = (TargetRevPos - RevGroupLastPos + EMIT_GROUP_LEDS - 1)/EMIT_GROUP_LEDS;
				RevGroupLastPos += RevGroup * EMIT_GROUP_LEDS;
			}

			BoardTime += Scan_Emits_for_One_Rev(Flags, RevGroup, ypt, xpt->EmitLeds, X_FIRST, RevNum,
									GPIOSwitch, TimerArr, GainTimerArr, &Pulses);
			RevNum += 1;
		}

		GainTimerArr[BoardNum ++] = BoardTime - 1;
		Time += BoardTime;
		BoardTime = 0;
	}

	PulseMark[0] = Pulses;

//--------------------------------------------------------------------------------------------------
	//Up Receive Down Emit
//--------------------------------------------------------------------------------------------------
	RevNum = 0;
	for(int i=0; i<xpt->Boards; i+=1)
	{
		PCB_CONFIG *pcb = &(xpt->PCBs[i]);

		for(int j = 0; j<pcb->RevLeds; j+=1)
		{
			uint16_t TargetRevPos = ActualPos(xpt->RevPos[RevNum] + ypt->EmitLeds, Y_FIRST, REV_GROUP_LEDS);
			uint16_t RevGroup = 0;

			if(TargetRevPos > RevGroupLastPos)
			{
				RevGroup = (TargetRevPos - RevGroupLastPos + EMIT_GROUP_LEDS - 1)/EMIT_GROUP_LEDS;
				RevGroupLastPos += RevGroup * EMIT_GROUP_LEDS;
			}

			BoardTime += Scan_Emits_for_One_Rev(Flags, RevGroup, xpt, 0, X_FIRST, RevNum,
									GPIOSwitch, TimerArr, GainTimerArr, &Pulses);
			RevNum += 1;
		}

		GainTimerArr[BoardNum ++] = BoardTime - 1;
		Time += BoardTime;
		BoardTime = 0;
	}

PulseMark[1] = Pulses;
//--------------------------------------------------------------------------------------------------
	//Shutdown emit & receive
//--------------------------------------------------------------------------------------------------
	 Switch_Group(EmitRst_High, RevSck_High, &GPIOSwitch[Pulses], &TimerArr[Pulses], EMIT_RIGHT);
	 Pulses += 2;

	 BoardTime += EMIT_SW_FAST * 2;

PulseMark[2] = Pulses;
//--------------------------------------------------------------------------------------------------
	//Down Receive Up Emit
//--------------------------------------------------------------------------------------------------
	RevGroupLastPos = REV_GROUP_LEDS - 1;		//select first group  last rev led
	RevNum = 0;

	Signal = RevSda_High | EmitRst_High;
	Switch_Group(Signal, RevSck_High, &GPIOSwitch[Pulses], &TimerArr[Pulses], EMIT_LEFT);
	Pulses += 2;

	BoardTime += EMIT_SW_FAST * 2;

	for(int i=0; i<xpt->Boards; i+=1)
	{
		PCB_CONFIG *pcb = &(xpt->PCBs[i]);

		for(int j = 0; j<pcb->RevLeds; j+=1)
		{
			uint16_t TargetRevPos = ActualPos(xpt->RevPos[RevNum], X_FIRST, REV_GROUP_LEDS);
			uint16_t RevGroup = 0;

			if(TargetRevPos > RevGroupLastPos)
			{
				RevGroup = (TargetRevPos - RevGroupLastPos + EMIT_GROUP_LEDS - 1)/EMIT_GROUP_LEDS;
				RevGroupLastPos += RevGroup * EMIT_GROUP_LEDS;
			}

			BoardTime += Scan_Emits_for_One_Rev(Flags, RevGroup, xpt, ypt->EmitLeds, Y_FIRST, RevNum,
									GPIOSwitch, TimerArr, GainTimerArr, &Pulses);
			RevNum += 1;
		}

		GainTimerArr[BoardNum ++] = BoardTime - 1;
		Time += BoardTime;
		BoardTime = 0;
	}

PulseMark[3] = Pulses;
//--------------------------------------------------------------------------------------------------
	//Right Receive Left Emit
//--------------------------------------------------------------------------------------------------
	RevNum = 0;
	for(int i=0; i<ypt->Boards; i+=1)
	{
		PCB_CONFIG *pcb = &(ypt->PCBs[i]);

		for(int j = 0; j<pcb->RevLeds; j+=1)
		{
			uint16_t TargetRevPos = ActualPos(ypt->RevPos[RevNum]+xpt->EmitLeds, X_FIRST, REV_GROUP_LEDS);
			uint16_t RevGroup = 0;

			if(TargetRevPos > RevGroupLastPos)
			{
				RevGroup = (TargetRevPos - RevGroupLastPos + EMIT_GROUP_LEDS - 1)/EMIT_GROUP_LEDS;
				RevGroupLastPos += RevGroup * EMIT_GROUP_LEDS;
			}

			BoardTime += Scan_Emits_for_One_Rev(Flags, RevGroup, ypt, 0, Y_FIRST, RevNum,
									GPIOSwitch, TimerArr, GainTimerArr, &Pulses);
			RevNum += 1;
		}

		GainTimerArr[BoardNum ++] = BoardTime - 1;
		Time += BoardTime;
		BoardTime = 0;
	}

PulseMark[4] = Pulses;
//--------------------------------------------------------------------------------------------------
	//Frame timeing
//--------------------------------------------------------------------------------------------------
	Switch_Group(EmitRst_High, RevSck_High, &GPIOSwitch[Pulses], &TimerArr[Pulses], EMIT_LEFT);
	Pulses += 2;

	BoardTime += EMIT_SW_FAST * 2;
	Time += BoardTime;

	int tails = 0;

	while((Time % FRAME_TIMR_RES) != 0 && tails < 16)
	{
		GPIOSwitch[Pulses] = GPIO_Map(0, (uint32_t *)GPIO_PIN_MAP_V15, (uint16_t *)LOGIC_V15, EMIT_LEFT);
		TimerArr[Pulses] = EMIT_SW_FAST - 1;
		Pulses += 1;

		BoardTime += EMIT_SW_FAST;
		Time += EMIT_SW_FAST;

		tails += 1;
	}

	if(tails >= 16)
	{
		ShowMessage("BUG! Invalid!\n");
		return 0;
	}

	GainTimerArr[BoardNum-1] += BoardTime;

	sth->FrameTime = Time;

	for(int i=Pulses; i<sth->TimerArrTableLen/2; i+=1)
		TimerArr[i] = EMIT_SW_FAST-1;

	if((uint32_t)sth->TimerArrTableLen < Pulses *2)
	{
		ShowMessage("FIX BUG!! Too Much Pulses!\n");
		return 0;
	}
	else
	{
		sth->TimerArrTableLen = Pulses *2;         //actual length
		sth->GpioSwitchTableLen = Pulses *4;
	}

	sth->Signals = 0;

	for(uint32_t i=0; i<Pulses; i+=1)
	{
		//if(TimerArr[i] != EMIT_SW_FAST-1)
		if(TimerArr[i] == (sth->TOn + sth->TOff)*CLK_FOR_1us/4 - 1)
		{
			sth->Signals += 1;
		}
	}

	int Signals=0;

	for(int Leds=0; Leds <xpt->RevLeds; Leds+=1)
	{
		Signals += (xpt->EmitEnd[Leds] - xpt->EmitStart[Leds]) + (doublefirst?1:0);
	}

	for(int Leds=0; Leds <ypt->RevLeds; Leds+=1)
	{
		Signals += (ypt->EmitEnd[Leds] - ypt->EmitStart[Leds]) + (doublefirst?1:0);
	}

	if(Signals*2 != sth->Signals)
	{
		ShowMessage("FIX BUG!!! Different Signals!");
		return 0;
	}

#ifdef _DEBUG
	FILE *fp;
	uint16_t Cnt = 0;
	uint16_t Data = 0;

	if((fp = fopen("d:\\wave.txt", "w")) != NULL)
	{
		for(uint32_t i=0; i<Pulses; i+=1)
		{
			fprintf(fp, "%d, ", TimerArr[i]);
			Cnt ++;


			if(i+1<Pulses && TimerArr[i] != TimerArr[i+1])
			{
				fprintf(fp, "//%d - %.1fus\n", Cnt, Cnt*(TimerArr[i]+1)/72.0);
				Cnt = 0;
			}
			else if((Cnt % 10) == 0)
			{
				fprintf(fp, "\n");
			}

			for(int j=0; j<=4; j+=1)
			{
				if(i+1 == PulseMark[j])
				{
					fprintf(fp, "\n------------------------------------------------------------------\n\n");
					Cnt = 0;
					break;
				}
			}
		}


		//----------------------------------------------------
		fprintf(fp, "\n\nRef   ");

		for(uint32_t i=0; i<Pulses; i+=1)
		{
			if(TimerArr[i] == EMIT_SW_FAST-1)
			{
				fprintf(fp, "__|T|");
			}
			else
			{
				fprintf(fp, "_______|T|");
				Data += 1;
			}
		}

		fprintf(fp, "\n");

		//----------------------------------------------------
		TextWave("LRSda ", GPIOSwitch, TimerArr, Pulses, GPIO_PIN_MAP_V15[8], fp);
		TextWave("LRSck ", GPIOSwitch, TimerArr, Pulses, GPIO_PIN_MAP_V15[9], fp);
		TextWave("RERst ", GPIOSwitch, TimerArr, Pulses, GPIO_PIN_MAP_V15[7], fp);
		TextWave("RESda ", GPIOSwitch, TimerArr, Pulses, GPIO_PIN_MAP_V15[2], fp);
		TextWave("RESck ", GPIOSwitch, TimerArr, Pulses, GPIO_PIN_MAP_V15[3], fp);
		TextWave("RESga ", GPIOSwitch, TimerArr, Pulses, GPIO_PIN_MAP_V15[4], fp);
		TextWave("RESgb ", GPIOSwitch, TimerArr, Pulses, GPIO_PIN_MAP_V15[5], fp);
		TextWave("RESgc ", GPIOSwitch, TimerArr, Pulses, GPIO_PIN_MAP_V15[6], fp);

		fprintf(fp, "\n");

        //----------------------------------------------------
		TextWave("RRSda ", GPIOSwitch, TimerArr, Pulses, GPIO_PIN_MAP_V15[0], fp);
		TextWave("RRSck ", GPIOSwitch, TimerArr, Pulses, GPIO_PIN_MAP_V15[1], fp);
		TextWave("LERst ", GPIOSwitch, TimerArr, Pulses, GPIO_PIN_MAP_V15[15], fp);
		TextWave("LESda ", GPIOSwitch, TimerArr, Pulses, GPIO_PIN_MAP_V15[10], fp);
		TextWave("LESck ", GPIOSwitch, TimerArr, Pulses, GPIO_PIN_MAP_V15[11], fp);
		TextWave("LESga ", GPIOSwitch, TimerArr, Pulses, GPIO_PIN_MAP_V15[12], fp);
		TextWave("LESgb ", GPIOSwitch, TimerArr, Pulses, GPIO_PIN_MAP_V15[13], fp);
		TextWave("LESgc ", GPIOSwitch, TimerArr, Pulses, GPIO_PIN_MAP_V15[14], fp);

		fprintf(fp, "\n\nTotal %.4fms, Pulses = %d, Data = %d\n", Time/72000.0, Pulses, Data);
		fprintf(fp, "Boards:%d \n", BoardNum);

		int temp = 0;
		for(int i=0; i<BoardNum; i+=1)
		{
			fprintf(fp, "%d, %d, %.4fms\n", i, (GainTimerArr[i]+1), (GainTimerArr[i]+1)/72000.0);
			temp += GainTimerArr[i]+1;
		}

		fprintf(fp, "Total: %.4fms\n", temp/72000.0);

		int Wait=0, Scan=0;
		for(uint32_t i=0; i<Pulses; i+=1)
		{
			if(TimerArr[i] == EMIT_SW_FAST - 1)
			{
				Wait += EMIT_SW_FAST;
			}
			else
			{
				Scan += TimerArr[i] + 1;
			}
		}

		fprintf(fp, "Wait: %.4fms\n", Wait/72000.0);
		fprintf(fp, "Scan: %.4fms\n", Scan/72000.0);

		fclose(fp);
	}
#endif

	return Pulses;
}

int Gen_DMATable_Tri(uint32_t Flags, SCAN_TABLE_HEAD_TRI *sth, uint32_t TabBaseAdd, uint8_t *Tab)
{
	int i,j;
	int led;
	int gain;
	uint8_t temp;
	uint32_t Offset;
	uint16_t *iTemp, *Pos, *End, *Start;
	float *fTemp;

	//bool doublefirst = ((Flags & 0x01) != 0);

//------------------------------- Offsets -----------------------------------
	Offset = (uint32_t)(sth->RevPos) - TabBaseAdd;
	Pos = (uint16_t *)(Tab + Offset);

	Offset = (uint32_t)(sth->EmitStart) - TabBaseAdd;
	Start = (uint16_t *)(Tab + Offset);

	Offset = (uint32_t)(sth->EmitEnd) - TabBaseAdd;
	End = (uint16_t *)(Tab + Offset);

	int RevLeds = 0;
	//scan order    LR->UR->DR->RR
	//Left Receive
	for(int i=0; i<sth->YRevLeds; i+=1)
	{
		Pos[RevLeds] = YParamTri.RevPos[i];
		Start[RevLeds] = YParamTri.EmitStart[i];
		End[RevLeds] = YParamTri.EmitEnd[i];

		RevLeds += 1;
	}
	//Up Receive
	for(int i=0; i<sth->XRevLeds; i+=1)
	{
		Pos[RevLeds] = XParamTri.RevPos[i];
		Start[RevLeds] = XParamTri.EmitStart[i];
		End[RevLeds] = XParamTri.EmitEnd[i];

		RevLeds += 1;
	}
	//Down Receive
	for(int i=0; i<sth->XRevLeds; i+=1)
	{
		Pos[RevLeds] = XParamTri.RevPos[i];
		Start[RevLeds] = XParamTri.EmitStart[i];
		End[RevLeds] = XParamTri.EmitEnd[i];

		RevLeds += 1;
	}
	//Right Receive
	for(int i=0; i<sth->YRevLeds; i+=1)
	{
		Pos[RevLeds] = YParamTri.RevPos[i];
		Start[RevLeds] = YParamTri.EmitStart[i];
		End[RevLeds] = YParamTri.EmitEnd[i];

		RevLeds += 1;
	}


//------------------------------- Tan -----------------------------------
	Offset = (uint32_t)(sth->XTan) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<XParam.Axis; i+=1)
	{
		fTemp[i] = XParam.Tan[i];
	}

	Offset = (uint32_t)(sth->YTan) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<YParam.Axis; i+=1)
	{
		fTemp[i] = YParam.Tan[i];
	}

//------------------------------- CoTan -----------------------------------
	Offset = (uint32_t)(sth->XCoTan) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<XParam.Axis; i+=1)
	{
		if(XParam.Tan[i]!=0)
			fTemp[i] = 1/XParam.Tan[i];
		else
			fTemp[i] = 0;
	}

	Offset = (uint32_t)(sth->YCoTan) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<YParam.Axis; i+=1)
	{
		if(YParam.Tan[i]!=0)
			fTemp[i] = 1/YParam.Tan[i];
		else
			fTemp[i] = 0;
	}


//------------------------- scan table ------------------------------------
	int Pulses;

	Pulses = Generate_Table_V15(	Flags,
									sth,
									TabBaseAdd,
									Tab
								  );

	if(sth->Signals > 6000){
		ShowMessage("信号数量过多(" + String(sth->Signals) +")！(接收灯过多，或者接收角度过大)！");
		return -1;
	}

	if(Pulses <= 0)
		return -1;

#ifdef _DEBUG
//	String Msg = "";
//	int validsignals = sth->Signals - (doublefirst?sth->RevLeds:0);
//
//	Msg.cat_printf("Pulses: %d\n", Pulses);
//	Msg.cat_printf("Total Signals: %d (%.3f)\n", sth->Signals, (float)sth->Signals/(float)Pulses);
//	Msg.cat_printf("Valid Signals: %d (%.3f)\n", validsignals, (float)validsignals/(float)Pulses);
//	Msg.cat_printf("TimerArrTableLen: %d\n", sth->TimerArrTableLen);
//	Msg.cat_printf("GpioSwitchTableLen: %d\n", sth->GpioSwitchTableLen);
//	Msg.cat_printf("GainTimerArrTableLen: %d\n", sth->GainTimerArrTableLen);
//	Msg.cat_printf("\nEquivalent X Axis: %.1f\n", (float) (sth->XLedOff) * 2.0 / ((float)sth->XEmitLeds / (float)sth->XRevLeds));
//	Msg.cat_printf("Equivalent Y Axis: %.1f\n", (float) (sth->YLedOff) * 2.0 / ((float)sth->YEmitLeds / (float)sth->YRevLeds));
//
//	ShowMessage(Msg);
#endif

	return 0;
}

#pragma package(smart_init)
