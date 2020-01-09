// Borland C++ Builder
// Copyright (c) 1995, 2005 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Hidtoken.pas' rev: 10.00

#ifndef HidtokenHPP
#define HidtokenHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Hidtoken
{
//-- type declarations -------------------------------------------------------
//-- var, const, procedure ---------------------------------------------------
static const Byte HIDP_ITEM_LONG = 0xfe;
static const Shortint HIDP_ITEM_LENGTH_DATA = 0x3;
static const Byte HIDP_MAIN_INPUT_1 = 0x81;
static const Byte HIDP_MAIN_INPUT_2 = 0x82;
static const Byte HIDP_MAIN_OUTPUT_1 = 0x91;
static const Byte HIDP_MAIN_OUTPUT_2 = 0x92;
static const Byte HIDP_MAIN_FEATURE_1 = 0xb1;
static const Byte HIDP_MAIN_FEATURE_2 = 0xb2;
static const Byte HIDP_MAIN_COLLECTION = 0xa1;
static const Byte HIDP_MAIN_ENDCOLLECTION = 0xc0;
static const Shortint HIDP_MAIN_COLLECTION_LINK = 0x0;
static const Shortint HIDP_MAIN_COLLECTION_APP = 0x1;
static const Shortint HIDP_GLOBAL_USAGE_PAGE_1 = 0x5;
static const Shortint HIDP_GLOBAL_USAGE_PAGE_2 = 0x6;
static const Shortint HIDP_GLOBAL_USAGE_PAGE_4 = 0x7;
static const Shortint HIDP_GLOBAL_LOG_MIN_1 = 0x15;
static const Shortint HIDP_GLOBAL_LOG_MIN_2 = 0x16;
static const Shortint HIDP_GLOBAL_LOG_MIN_4 = 0x17;
static const Shortint HIDP_GLOBAL_LOG_MAX_1 = 0x25;
static const Shortint HIDP_GLOBAL_LOG_MAX_2 = 0x26;
static const Shortint HIDP_GLOBAL_LOG_MAX_4 = 0x27;
static const Shortint HIDP_GLOBAL_PHY_MIN_1 = 0x35;
static const Shortint HIDP_GLOBAL_PHY_MIN_2 = 0x36;
static const Shortint HIDP_GLOBAL_PHY_MIN_4 = 0x37;
static const Shortint HIDP_GLOBAL_PHY_MAX_1 = 0x45;
static const Shortint HIDP_GLOBAL_PHY_MAX_2 = 0x46;
static const Shortint HIDP_GLOBAL_PHY_MAX_4 = 0x47;
static const Shortint HIDP_GLOBAL_UNIT_EXP_1 = 0x55;
static const Shortint HIDP_GLOBAL_UNIT_EXP_2 = 0x56;
static const Shortint HIDP_GLOBAL_UNIT_EXP_4 = 0x57;
static const Shortint HIDP_GLOBAL_UNIT_1 = 0x65;
static const Shortint HIDP_GLOBAL_UNIT_2 = 0x66;
static const Shortint HIDP_GLOBAL_UNIT_4 = 0x67;
static const Shortint HIDP_GLOBAL_REPORT_SIZE = 0x75;
static const Byte HIDP_GLOBAL_REPORT_ID = 0x85;
static const Byte HIDP_GLOBAL_REPORT_COUNT_1 = 0x95;
static const Byte HIDP_GLOBAL_REPORT_COUNT_2 = 0x96;
static const Byte HIDP_GLOBAL_PUSH = 0xa4;
static const Byte HIDP_GLOBAL_POP = 0xb4;
static const Shortint HIDP_LOCAL_USAGE_1 = 0x9;
static const Shortint HIDP_LOCAL_USAGE_2 = 0xa;
static const Shortint HIDP_LOCAL_USAGE_4 = 0xb;
static const Shortint HIDP_LOCAL_USAGE_MIN_1 = 0x19;
static const Shortint HIDP_LOCAL_USAGE_MIN_2 = 0x1a;
static const Shortint HIDP_LOCAL_USAGE_MIN_4 = 0x1b;
static const Shortint HIDP_LOCAL_USAGE_MAX_1 = 0x29;
static const Shortint HIDP_LOCAL_USAGE_MAX_2 = 0x2a;
static const Shortint HIDP_LOCAL_USAGE_MAX_4 = 0x2b;
static const Shortint HIDP_LOCAL_DESIG_INDEX = 0x39;
static const Shortint HIDP_LOCAL_DESIG_MIN = 0x49;
static const Shortint HIDP_LOCAL_DESIG_MAX = 0x59;
static const Shortint HIDP_LOCAL_STRING_INDEX = 0x79;
static const Byte HIDP_LOCAL_STRING_MIN = 0x89;
static const Byte HIDP_LOCAL_STRING_MAX = 0x99;
static const Byte HIDP_LOCAL_DELIMITER = 0xa9;
extern PACKAGE bool __fastcall HidPIsMain(int x);
extern PACKAGE bool __fastcall HidPIsMainItem(int x);
extern PACKAGE bool __fastcall HidPIsGlobalItem(int x);
extern PACKAGE bool __fastcall HidPIsLocalItem(int x);
extern PACKAGE bool __fastcall HidPIsReservedItem(int x);

}	/* namespace Hidtoken */
using namespace Hidtoken;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Hidtoken
