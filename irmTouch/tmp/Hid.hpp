// Borland C++ Builder
// Copyright (c) 1995, 2005 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Hid.pas' rev: 10.00

#ifndef HidHPP
#define HidHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Hid
{
//-- type declarations -------------------------------------------------------
typedef Word *PUsage;

typedef Word TUsage;

typedef unsigned THIDPKeyboardModifierState;

#pragma option push -b-
enum THIDVariantFlags { Range, NotRange };
#pragma option pop

struct HIDD_CONFIGURATION;
typedef HIDD_CONFIGURATION *PHIDDConfiguration;

struct HIDD_CONFIGURATION
{
	
public:
	void *cookie;
	unsigned size;
	unsigned RingBufferSize;
} ;

typedef HIDD_CONFIGURATION  THIDDConfiguration;

struct HIDD_ATTRIBUTES;
typedef HIDD_ATTRIBUTES *PHIDDAttributes;

struct HIDD_ATTRIBUTES
{
	
public:
	unsigned Size;
	Word VendorID;
	Word ProductID;
	Word VersionNumber;
} ;

typedef HIDD_ATTRIBUTES  THIDDAttributes;

typedef void *PHIDPPreparsedData;

typedef unsigned *PHIDPReportType;

typedef unsigned THIDPReportType;

typedef unsigned *PHIDPKeyboardDirection;

typedef unsigned THIDPKeyboardDirection;

struct USAGE_AND_PAGE;
typedef USAGE_AND_PAGE *PUsageAndPage;

struct USAGE_AND_PAGE
{
	
public:
	Word Usage;
	Word UsagePage;
} ;

typedef USAGE_AND_PAGE  TUsageAndPage;

struct HIDP_BUTTON_CAPS;
typedef HIDP_BUTTON_CAPS *PHIDPButtonCaps;

struct HIDP_BUTTON_CAPS
{
	
public:
	Word UsagePage;
	Byte ReportID;
	Byte IsAlias;
	Word BitField;
	Word LinkCollection;
	Word LinkUsage;
	Word LinkUsagePage;
	Byte IsRange;
	Byte IsStringRange;
	Byte IsDesignatorRange;
	Byte IsAbsolute;
	unsigned Reserved[10];
	union
	{
		struct 
		{
			Word Usage;
			Word Reserved1;
			Word StringIndex;
			Word Reserved2;
			Word DesignatorIndex;
			Word Reserved3;
			Word DataIndex;
			Word Reserved4;
			
		};
		struct 
		{
			Word UsageMin;
			Word UsageMax;
			Word StringMin;
			Word StringMax;
			Word DesignatorMin;
			Word DesignatorMax;
			Word DataIndexMin;
			Word DataIndexMax;
			
		};
		
	};
} ;

typedef HIDP_BUTTON_CAPS  THIDPButtonCaps;

struct HIDP_VALUE_CAPS;
typedef HIDP_VALUE_CAPS *PHIDPValueCaps;

struct HIDP_VALUE_CAPS
{
	
public:
	Word UsagePage;
	Byte ReportID;
	Byte IsAlias;
	Word BitField;
	Word LinkCollection;
	Word LinkUsage;
	Word LinkUsagePage;
	Byte IsRange;
	Byte IsStringRange;
	Byte IsDesignatorRange;
	Byte IsAbsolute;
	Byte HasNull;
	Byte Reserved;
	Word BitSize;
	Word ReportCount;
	Word Reserved_[5];
	unsigned UnitsExp;
	unsigned Units;
	int LogicalMin;
	int LogicalMax;
	int PhysicalMin;
	int PhysicalMax;
	union
	{
		struct 
		{
			Word Usage;
			Word Reserved1;
			Word StringIndex;
			Word Reserved2;
			Word DesignatorIndex;
			Word Reserved3;
			Word DataIndex;
			Word Reserved4;
			
		};
		struct 
		{
			Word UsageMin;
			Word UsageMax;
			Word StringMin;
			Word StringMax;
			Word DesignatorMin;
			Word DesignatorMax;
			Word DataIndexMin;
			Word DataIndexMax;
			
		};
		
	};
} ;

typedef HIDP_VALUE_CAPS  THIDPValueCaps;

struct HIDP_LINK_COLLECTION_NODE;
typedef HIDP_LINK_COLLECTION_NODE *PHIDPLinkCollectionNode;

struct HIDP_LINK_COLLECTION_NODE
{
	
public:
	Word LinkUsage;
	Word LinkUsagePage;
	Word Parent;
	Word NumberOfChildren;
	Word NextSibling;
	Word FirstChild;
	Byte CollectionType;
	Byte IsAlias;
	Word Reserved;
	void *UserContext;
} ;

typedef HIDP_LINK_COLLECTION_NODE  THIDPLinkCollectionNode;

typedef char *PHIDPReportDescriptor;

struct HIDP_CAPS;
typedef HIDP_CAPS *PHIDPCaps;

struct HIDP_CAPS
{
	
public:
	Word Usage;
	Word UsagePage;
	Word InputReportByteLength;
	Word OutputReportByteLength;
	Word FeatureReportByteLength;
	Word Reserved[17];
	Word NumberLinkCollectionNodes;
	Word NumberInputButtonCaps;
	Word NumberInputValueCaps;
	Word NumberInputDataIndices;
	Word NumberOutputButtonCaps;
	Word NumberOutputValueCaps;
	Word NumberOutputDataIndices;
	Word NumberFeatureButtonCaps;
	Word NumberFeatureValueCaps;
	Word NumberFeatureDataIndices;
} ;

typedef HIDP_CAPS  THIDPCaps;

struct HIDP_DATA;
typedef HIDP_DATA *PHIDPData;

struct HIDP_DATA
{
	
public:
	Word DataIndex;
	Word Reserved;
	union
	{
		struct 
		{
			Byte On_;
			
		};
		struct 
		{
			unsigned RawValue;
			
		};
		
	};
} ;

typedef HIDP_DATA  THIDPData;

struct HIDP_UNKNOWN_TOKEN;
typedef HIDP_UNKNOWN_TOKEN *PHIDPUnknownToken;

struct HIDP_UNKNOWN_TOKEN
{
	
public:
	Byte Token;
	Byte Reserved[3];
	unsigned BitField;
} ;

typedef HIDP_UNKNOWN_TOKEN  THIDPUnknownToken;

struct HIDP_EXTENDED_ATTRIBUTES;
typedef HIDP_EXTENDED_ATTRIBUTES *PHIDPExtendedAttributes;

struct HIDP_EXTENDED_ATTRIBUTES
{
	
public:
	Byte NumGlobalUnknowns;
	Byte Reserved[3];
	HIDP_UNKNOWN_TOKEN *GlobalUnknowns;
	unsigned Data[1];
} ;

typedef HIDP_EXTENDED_ATTRIBUTES  THIDPExtendedAttributes;

typedef bool __stdcall (*THIDPInsertScanCodes)(void * Context, char * NewScanCodes, unsigned Length);

typedef unsigned __stdcall (*THidD_Hello)(char * Buffer, unsigned BufferLength);

typedef void __stdcall (*THidD_GetHidGuid)(GUID &HidGuid);

typedef BOOL __stdcall (*THidD_GetPreparsedData)(unsigned HidDeviceObject, void * &PreparsedData);

typedef BOOL __stdcall (*THidD_FreePreparsedData)(void * PreparsedData);

typedef BOOL __stdcall (*THidD_GetConfiguration)(unsigned HidDeviceObject, HIDD_CONFIGURATION &HidConfig, int Size);

typedef BOOL __stdcall (*THidD_SetConfiguration)(unsigned HidDeviceObject, const HIDD_CONFIGURATION &HidConfig, int Size);

typedef BOOL __stdcall (*THidD_FlushQueue)(unsigned HidDeviceObject);

typedef BOOL __stdcall (*THidD_GetFeature)(unsigned HidDeviceObject, void *Report, int Size);

typedef BOOL __stdcall (*THidD_SetFeature)(unsigned HidDeviceObject, void *Report, int Size);

typedef BOOL __stdcall (*THidD_GetNumInputBuffers)(unsigned HidDeviceObject, int &NumBufs);

typedef BOOL __stdcall (*THidD_SetNumInputBuffers)(unsigned HidDeviceObject, int NumBufs);

typedef BOOL __stdcall (*THidD_GetAttributes)(unsigned HidDeviceObject, HIDD_ATTRIBUTES &HidAttrs);

typedef BOOL __stdcall (*THidD_GetManufacturerString)(unsigned HidDeviceObject, WideChar * Buffer, int BufferLength);

typedef BOOL __stdcall (*THidD_GetProductString)(unsigned HidDeviceObject, WideChar * Buffer, int BufferLength);

typedef BOOL __stdcall (*THidD_GetSerialNumberString)(unsigned HidDeviceObject, WideChar * Buffer, int BufferLength);

typedef BOOL __stdcall (*THidD_GetPhysicalDescriptor)(unsigned HidDeviceObject, void *Buffer, int BufferLength);

typedef BOOL __stdcall (*THidD_GetIndexedString)(unsigned HidDeviceObject, int Index, WideChar * Buffer, int BufferLength);

typedef int __stdcall (*THidP_GetCaps)(void * PreparsedData, HIDP_CAPS &Capabilities);

typedef int __stdcall (*THidP_GetLinkCollectionNodes)(PHIDPLinkCollectionNode LinkCollectionNodes, unsigned &LinkCollectionNodesLength, void * PreparsedData);

typedef int __stdcall (*THidP_GetSpecificButtonCaps)(unsigned ReportType, Word UsagePage, Word LinkCollection, Word Usage, PHIDPButtonCaps ButtonCaps, Word &ButtonCapsLength, void * PreparsedData);

typedef int __stdcall (*THidP_GetSpecificValueCaps)(unsigned ReportType, Word UsagePage, Word LinkCollection, Word Usage, PHIDPValueCaps ValueCaps, Word &ValueCapsLength, void * PreparsedData);

typedef int __stdcall (*THidP_GetData)(unsigned ReportType, PHIDPData DataList, unsigned &DataLength, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_SetData)(unsigned ReportType, PHIDPData DataList, unsigned &DataLength, void * PreparsedData, void *Report, unsigned ReportLength);

typedef unsigned __stdcall (*THidP_MaxDataListLength)(unsigned ReportType, void * PreparsedData);

typedef int __stdcall (*THidP_GetUsages)(unsigned ReportType, Word UsagePage, Word LinkCollection, PUsage UsageList, unsigned &UsageLength, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_GetButtons)(unsigned ReportType, Word UsagePage, Word LinkCollection, PUsage UsageList, unsigned &UsageLength, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_GetUsagesEx)(unsigned ReportType, Word LinkCollection, PUsageAndPage UsageList, unsigned &UsageLength, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_GetButtonsEx)(unsigned ReportType, Word LinkCollection, PUsageAndPage UsageList, unsigned &UsageLength, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_SetUsages)(unsigned ReportType, Word UsagePage, Word LinkCollection, PUsage UsageList, unsigned &UsageLength, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_SetButtons)(unsigned ReportType, Word UsagePage, Word LinkCollection, PUsage ButtonList, unsigned &ButtonLength, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_UnsetUsages)(unsigned ReportType, Word UsagePage, Word LinkCollection, PUsage UsageList, unsigned &UsageLength, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_UnsetButtons)(unsigned ReportType, Word UsagePage, Word LinkCollection, PUsage ButtonList, unsigned &ButtonLength, void * PreparsedData, void *Report, unsigned ReportLength);

typedef unsigned __stdcall (*THidP_MaxUsageListLength)(unsigned ReportType, Word UsagePage, void * PreparsedData);

typedef unsigned __stdcall (*THidP_MaxButtonListLength)(unsigned ReportType, Word UsagePage, void * PreparsedData);

typedef int __stdcall (*THidP_GetUsageValue)(unsigned ReportType, Word UsagePage, Word LinkCollection, Word Usage, unsigned &UsageValue, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_GetScaledUsageValue)(unsigned ReportType, Word UsagePage, Word LinkCollection, Word Usage, int &UsageValue, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_GetUsageValueArray)(unsigned ReportType, Word UsagePage, Word LinkCollection, Word Usage, char * UsageValue, Word UsageValueByteLength, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_SetUsageValue)(unsigned ReportType, Word UsagePage, Word LinkCollection, Word Usage, unsigned UsageValue, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_SetScaledUsageValue)(unsigned ReportType, Word UsagePage, Word LinkCollection, Word Usage, int UsageValue, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_SetUsageValueArray)(unsigned ReportType, Word UsagePage, Word LinkCollection, Word Usage, char * UsageValue, Word UsageValueByteLength, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_UsageListDifference)(PUsage PreviousUsageList, PUsage CurrentUsageList, PUsage BreakUsageList, PUsage MakeUsageList, unsigned UsageListLength);

typedef int __stdcall (*THidP_TranslateUsagesToI8042ScanCodes)(PUsage ChangedUsageList, unsigned UsageListLength, unsigned KeyAction, unsigned &ModifierState, THIDPInsertScanCodes InsertCodesProcedure, void * InsertCodesContext);

//-- var, const, procedure ---------------------------------------------------
static const int HIDP_STATUS_SUCCESS = 1114112;
static const int HIDP_STATUS_NULL = -2146369535;
static const int HIDP_STATUS_INVALID_PREPARSED_DATA = -1072627711;
static const int HIDP_STATUS_INVALID_REPORT_TYPE = -1072627710;
static const int HIDP_STATUS_INVALID_REPORT_LENGTH = -1072627709;
static const int HIDP_STATUS_USAGE_NOT_FOUND = -1072627708;
static const int HIDP_STATUS_VALUE_OUT_OF_RANGE = -1072627707;
static const int HIDP_STATUS_BAD_LOG_PHY_VALUES = -1072627706;
static const int HIDP_STATUS_BUFFER_TOO_SMALL = -1072627705;
static const int HIDP_STATUS_INTERNAL_ERROR = -1072627704;
static const int HIDP_STATUS_I8042_TRANS_UNKNOWN = -1072627703;
static const int HIDP_STATUS_INCOMPATIBLE_REPORT_ID = -1072627702;
static const int HIDP_STATUS_NOT_VALUE_ARRAY = -1072627701;
static const int HIDP_STATUS_IS_VALUE_ARRAY = -1072627700;
static const int HIDP_STATUS_DATA_INDEX_NOT_FOUND = -1072627699;
static const int HIDP_STATUS_DATA_INDEX_OUT_OF_RANGE = -1072627698;
static const int HIDP_STATUS_BUTTON_NOT_PRESSED = -1072627697;
static const int HIDP_STATUS_REPORT_DOES_NOT_EXIST = -1072627696;
static const int HIDP_STATUS_NOT_IMPLEMENTED = -1072627680;
static const int HIDP_STATUS_I8242_TRANS_UNKNOWN = -1072627703;
static const Shortint HIDP_LINK_COLLECTION_ROOT = -1;
static const Shortint HIDP_LINK_COLLECTION_UNSPECIFIED = 0x0;
static const Shortint kmsLeftControl = 0x1;
static const Shortint kmsLeftShift = 0x2;
static const Shortint kmsLeftAlt = 0x4;
static const Shortint kmsLeftGUI = 0x8;
static const Shortint kmsRightControl = 0x10;
static const Shortint kmsRightShift = 0x20;
static const Shortint kmsRightAlt = 0x40;
static const Byte kmsRightGUI = 0x80;
static const Word kmsCapsLock = 0x100;
static const Word kmsScollLock = 0x200;
static const Word kmsNumLock = 0x400;
static const Shortint lcnIsAlias = 0x1;
static const Shortint HidP_Input = 0x0;
static const Shortint HidP_Output = 0x1;
static const Shortint HidP_Feature = 0x2;
static const Shortint HidP_Keyboard_Break = 0x0;
static const Shortint HidP_Keyboard_Make = 0x1;
extern PACKAGE THidD_Hello HidD_Hello;
extern PACKAGE THidD_GetHidGuid HidD_GetHidGuid;
extern PACKAGE THidD_GetPreparsedData HidD_GetPreparsedData;
extern PACKAGE THidD_FreePreparsedData HidD_FreePreparsedData;
extern PACKAGE THidD_GetConfiguration HidD_GetConfiguration;
extern PACKAGE THidD_SetConfiguration HidD_SetConfiguration;
extern PACKAGE THidD_FlushQueue HidD_FlushQueue;
extern PACKAGE THidD_GetFeature HidD_GetFeature;
extern PACKAGE THidD_SetFeature HidD_SetFeature;
extern PACKAGE THidD_GetNumInputBuffers HidD_GetNumInputBuffers;
extern PACKAGE THidD_SetNumInputBuffers HidD_SetNumInputBuffers;
extern PACKAGE THidD_GetAttributes HidD_GetAttributes;
extern PACKAGE THidD_GetManufacturerString HidD_GetManufacturerString;
extern PACKAGE THidD_GetProductString HidD_GetProductString;
extern PACKAGE THidD_GetSerialNumberString HidD_GetSerialNumberString;
extern PACKAGE THidD_GetPhysicalDescriptor HidD_GetPhysicalDescriptor;
extern PACKAGE THidD_GetIndexedString HidD_GetIndexedString;
extern PACKAGE THidP_GetCaps HidP_GetCaps;
extern PACKAGE THidP_GetLinkCollectionNodes HidP_GetLinkCollectionNodes;
extern PACKAGE THidP_GetSpecificButtonCaps HidP_GetSpecificButtonCaps;
extern PACKAGE THidP_GetSpecificValueCaps HidP_GetSpecificValueCaps;
extern PACKAGE THidP_GetData HidP_GetData;
extern PACKAGE THidP_SetData HidP_SetData;
extern PACKAGE THidP_MaxDataListLength HidP_MaxDataListLength;
extern PACKAGE THidP_GetUsages HidP_GetUsages;
extern PACKAGE THidP_GetButtons HidP_GetButtons;
extern PACKAGE THidP_GetUsagesEx HidP_GetUsagesEx;
extern PACKAGE THidP_GetButtonsEx HidP_GetButtonsEx;
extern PACKAGE THidP_SetUsages HidP_SetUsages;
extern PACKAGE THidP_SetButtons HidP_SetButtons;
extern PACKAGE THidP_UnsetUsages HidP_UnsetUsages;
extern PACKAGE THidP_UnsetButtons HidP_UnsetButtons;
extern PACKAGE THidP_MaxUsageListLength HidP_MaxUsageListLength;
extern PACKAGE THidP_MaxButtonListLength HidP_MaxButtonListLength;
extern PACKAGE THidP_GetUsageValue HidP_GetUsageValue;
extern PACKAGE THidP_GetScaledUsageValue HidP_GetScaledUsageValue;
extern PACKAGE THidP_GetUsageValueArray HidP_GetUsageValueArray;
extern PACKAGE THidP_SetUsageValue HidP_SetUsageValue;
extern PACKAGE THidP_SetScaledUsageValue HidP_SetScaledUsageValue;
extern PACKAGE THidP_SetUsageValueArray HidP_SetUsageValueArray;
extern PACKAGE THidP_UsageListDifference HidP_UsageListDifference;
extern PACKAGE THidP_TranslateUsagesToI8042ScanCodes HidP_TranslateUsagesToI8042ScanCodes;
#define HidModuleName "HID.dll"
extern PACKAGE int __fastcall HidP_GetButtonCaps_(unsigned ReportType, PHIDPButtonCaps ButtonCaps, Word &ButtonCapsLength, void * PreparsedData);
extern PACKAGE int __fastcall HidP_GetValueCaps_(unsigned ReportType, PHIDPValueCaps ValueCaps, Word &ValueCapsLength, void * PreparsedData);
extern PACKAGE bool __fastcall HidP_IsSameUsageAndPage_(USAGE_AND_PAGE u1, USAGE_AND_PAGE u2);
extern PACKAGE bool __fastcall IsHidLoaded(void);
extern PACKAGE bool __fastcall LoadHid(void);
extern PACKAGE void __fastcall UnloadHid(void);

}	/* namespace Hid */
using namespace Hid;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Hid
