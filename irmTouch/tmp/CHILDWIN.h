//----------------------------------------------------------------------------
#ifndef ChildWinH
#define ChildWinH
//----------------------------------------------------------------------------
#include <Controls.hpp>
#include <Forms.hpp>
#include <Graphics.hpp>
#include <Classes.hpp>
#include <Windows.hpp>
#include <System.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>

#include <VCLTee.Chart.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VCLTee.Series.hpp>
//----------------------------------------------------------------------------

#include "Boundary.h"
class TMDIChild : public TForm
{
__published:
	TChart *Chart1;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:
public:
	virtual __fastcall TMDIChild(TComponent *Owner);
	void __fastcall UpdateChart(uint8_t Type, uint8_t *Data, uint16_t Len, int16_t Offset, interval_t &intv, SigQ_t *SigQ);
	void __fastcall UpdateChart(uint8_t *Data, uint16_t Len, int16_t Offset, interval_t &intv, SigQ_t *SigQ);
	void __fastcall UpdateChart(uint8_t *Data, tri_info_t *st, uint16_t Off, uint16_t Revs, uint16_t Emits);
	void __fastcall UpdateInterval(int16_t Offset, interval_t &intv);
};
//----------------------------------------------------------------------------
#endif
