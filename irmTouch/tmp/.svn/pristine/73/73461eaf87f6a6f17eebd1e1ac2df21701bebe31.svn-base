//---------------------------------------------------------------------------

#ifndef HEXFileH
#define HEXFileH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "stdio.h"
//---------------------------------------------------------------------------
class HEXFile
{
private:	// User declarations
public:		// User declarations
	__fastcall HEXFile(void);
	long __fastcall HexToInt(unsigned char *Hex,int Len);
	int __fastcall GetFileData(String File);
	int __fastcall GetLine(FILE *fp,unsigned char *Buf,int BufLen);
	int __fastcall GetLine(FILE *fp, unsigned char *Buf, int BufLen, int FileLen);
};
//---------------------------------------------------------------------------
extern HEXFile hexfile;
#endif
