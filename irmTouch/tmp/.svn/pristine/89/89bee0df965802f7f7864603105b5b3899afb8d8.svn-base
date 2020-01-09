// Borland C++ Builder
// Copyright (c) 1995, 2005 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Dbt.pas' rev: 10.00

#ifndef DbtHPP
#define DbtHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

#include <dbt.h>


namespace Dbt
{
//-- type declarations -------------------------------------------------------
typedef DEV_BROADCAST_HDR *PDevBroadcastHdr;

typedef DEV_BROADCAST_HDR  TDevBroadcastHdr;

typedef VolLockBroadcast *PVolLockBroadcast;

typedef VolLockBroadcast  TVolLockBroadcast;

struct DEV_BROADCAST_HEADER;
typedef DEV_BROADCAST_HEADER *PDevBroadcastHeader;

#pragma pack(push,1)
struct DEV_BROADCAST_HEADER
{
	
public:
	unsigned dbcd_size;
	unsigned dbcd_devicetype;
	unsigned dbcd_reserved;
} ;
#pragma pack(pop)

typedef DEV_BROADCAST_HEADER  TDevBroadcastHeader;

typedef DEV_BROADCAST_OEM *PDevBroadcastOem;

typedef DEV_BROADCAST_OEM  TDevBroadcastOem;

typedef DEV_BROADCAST_DEVNODE *PDevBroadcastDevNode;

typedef DEV_BROADCAST_DEVNODE  TDevBroadcastDevNode;

typedef DEV_BROADCAST_VOLUME *PDevBroadcastVolume;

typedef DEV_BROADCAST_VOLUME  TDevBroadcastVolume;

typedef DEV_BROADCAST_PORT_A *PDevBroadCastPortA;

typedef DEV_BROADCAST_PORT_A  TDevBroadCastPortA;

typedef DEV_BROADCAST_PORT_W *PDevBroadCastPortW;

typedef DEV_BROADCAST_PORT_W  TDevBroadCastPortW;

typedef DEV_BROADCAST_PORT_A *PDevBroadCastPort;

typedef DEV_BROADCAST_NET *PDevBroadcastNet;

typedef DEV_BROADCAST_NET  TDevBroadcastNet;

typedef DEV_BROADCAST_DEVICEINTERFACE_A *PDevBroadcastDeviceInterfaceA;

typedef DEV_BROADCAST_DEVICEINTERFACE_A  TDevBroadcastDeviceInterfaceA;

typedef DEV_BROADCAST_DEVICEINTERFACE_W *PDevBroadcastDeviceInterfaceW;

typedef DEV_BROADCAST_DEVICEINTERFACE_W  TDevBroadcastDeviceInterfaceW;

typedef DEV_BROADCAST_DEVICEINTERFACE_A *PDevBroadcastDeviceInterface;

typedef DEV_BROADCAST_HANDLE *PDevBroadcastHandle;

typedef DEV_BROADCAST_HANDLE  TDevBroadcastHandle;

struct DEV_BROADCAST_USERDEFINED;
typedef DEV_BROADCAST_USERDEFINED *PDevBroadcastUserdefined;

#pragma pack(push,1)
struct DEV_BROADCAST_USERDEFINED
{
	
public:
	#pragma pack(push,1)
	DEV_BROADCAST_HDR dbud_dbh;
	#pragma pack(pop)
	char dbud_szName[1];
} ;
#pragma pack(pop)

typedef DEV_BROADCAST_USERDEFINED  TDevBroadcastUserdefined;

struct TWMDeviceChange
{
	
public:
	unsigned Msg;
	unsigned Event;
	void *dwData;
	int Result;
} ;

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Dbt */
using namespace Dbt;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Dbt
