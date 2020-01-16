//---------------------------------------------------------------------------

#ifndef GenTableTriH
#define GenTableTriH

#include "stdint.h"
#include "config.h"

//72MHz
#define CLK_FOR_1us				72				//72MHz
#define EMIT_SW_FAST			24           	//4MHz
#define REV_GROUP_LEDS			6
#define EMIT_GROUP_LEDS			6
#define FRAME_TIMR_RES			144

#define X_FIRST		true
#define Y_FIRST		false

#define EMIT_LEFT	true
#define EMIT_RIGHT	false


extern int Gen_DMATable_Tri(uint32_t Flags, SCAN_TABLE_HEAD_TRI *sth, uint32_t TabBaseAdd, uint8_t *Tab);

//---------------------------------------------------------------------------
#endif
