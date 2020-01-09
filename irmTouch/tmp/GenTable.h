//---------------------------------------------------------------------------

#ifndef GenTableH
#define GenTableH

#include <stdint.h>
#include "config.h"

typedef uint32_t (*GetTab_Data_t)(uint16_t Addr, uint8_t Led, uint16_t Pow);

//---------------------------------------------------------------------------
extern int Gen_DMATable_c(uint32_t Flags, SCAN_TABLE_HEAD *sth, uint32_t TabBaseAdd, uint8_t *Tab);

//------------------------------ V46 -------------------------------------------
extern uint32_t Get_Emit_Data_V46(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Rev_Data_V46(uint16_t Addr, uint8_t Led, uint16_t Gain);
//cpld magic config
extern uint32_t Get_Emit_Data_V46_MagicConfig(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Rev_Data_V46_MagicConfig(uint16_t Addr, uint8_t Led, uint16_t Gain);

//------------------------------ V54 -------------------------------------------
extern uint32_t Get_Emit_Data_V54(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Rev_Data_PDV54(uint16_t Addr, uint8_t Led, uint16_t Gain);
extern uint32_t Get_Rev_Data_PTV54(uint16_t Addr, uint8_t Led, uint16_t Gain);

extern uint32_t Get_Emit_Data_V54_MagicConfig(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Rev_Data_PDV54_MagicConfig(uint16_t Addr, uint8_t Led, uint16_t Gain);

extern uint32_t Get_Emit_Data_V54_Support_64_Boards(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Rev_Data_PDV54_Support_64_Boards(uint16_t Addr, uint8_t Led, uint16_t Gain);

//------------------------------ V57 -------------------------------------------
extern uint32_t Get_Emit_Data_V57_MagicConfig(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Rev_Data_PDV57_MagicConfig(uint16_t Addr, uint8_t Led, uint16_t Gain);

//------------------------------ V56 -------------------------------------------
extern uint32_t Get_Emit_Data_V56(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Rev_Data_PDV56(uint16_t Addr, uint8_t Led, uint16_t Gain);
extern uint32_t Get_Rev_Data_PTV56(uint16_t Addr, uint8_t Led, uint16_t Gain);

//------------------------------ V65 -------------------------------------------
extern uint32_t Get_Emit_Data_V65(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Rev_Data_V65(uint16_t Addr, uint8_t Led, uint16_t Gain);

extern uint32_t Get_Emit_Data_V65_MagicConfig(uint16_t Addr, uint8_t Led, uint16_t Pow);

//------------------------------ V70 -------------------------------------------
extern uint32_t Get_Emit_Data_V70(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Rev_Data_V70(uint16_t Addr, uint8_t Led, uint16_t Gain);

extern uint32_t Get_Emit_Data_V70_MagicConfig(uint16_t Addr, uint8_t Led, uint16_t Pow);

//------------------------------ V11 -------------------------------------------
extern uint32_t Get_EmitX_Data_TypeX_V11(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_EmitY_Data_TypeX_V11(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_RevX_Data_TypeX_PTV11(uint16_t Addr, uint8_t Led, uint16_t Gain);
extern uint32_t Get_RevY_Data_TypeX_PTV11(uint16_t Addr, uint8_t Led, uint16_t Gain);

//------------------------------ V80 -------------------------------------------
extern uint32_t Get_EmitX_Data_TypeX_V80(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_EmitY_Data_TypeX_V80(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_RevX_Data_TypeX_PTV80(uint16_t Addr, uint8_t Led, uint16_t Gain);
extern uint32_t Get_RevY_Data_TypeX_PTV80(uint16_t Addr, uint8_t Led, uint16_t Gain);

//------------------------------21.3" VR1 -------------------------------------------
extern uint32_t Get_EmitX_Data_TypeX_JYT6N_VR1(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_EmitY_Data_TypeX_JYT6N_VR1(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_RevX_Data_TypeX_JYT6N_PDVR1(uint16_t Addr, uint8_t Led, uint16_t Gain);
extern uint32_t Get_RevY_Data_TypeX_JYT6N_PDVR1(uint16_t Addr, uint8_t Led, uint16_t Gain);

//------------------------------21.3" VR2 -------------------------------------------
extern uint32_t Get_EmitX_Data_TypeX_JYT6N_VR2(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_EmitY_Data_TypeX_JYT6N_VR2(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_RevX_Data_TypeX_JYT6N_PDVR2(uint16_t Addr, uint8_t Led, uint16_t Gain);
extern uint32_t Get_RevY_Data_TypeX_JYT6N_PDVR2(uint16_t Addr, uint8_t Led, uint16_t Gain);

//------------------------------ V90 -------------------------------------------
extern uint32_t Get_Emit_Data_V90(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Rev_Data_V90(uint16_t Addr, uint8_t Led, uint16_t Gain);

//------------------------------21.5" VR2 -------------------------------------------
extern uint32_t Get_EmitX_Data_TypeX_VR2(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_EmitY_Data_TypeX_VR2(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_RevX_Data_TypeX_PDVR2(uint16_t Addr, uint8_t Led, uint16_t Gain);
extern uint32_t Get_RevY_Data_TypeX_PDVR2(uint16_t Addr, uint8_t Led, uint16_t Gain);

//------------------------------ V10.0 -------------------------------------------
extern uint32_t Get_Emit_Data_PDV100(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Emit_Data_PTV100(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Rev_Data_PTV100(uint16_t Addr, uint8_t Led, uint16_t Pow);

//------------------------------ V13.0 -------------------------------------------
extern uint32_t Get_Emit_Data_PDV130(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Emit_Data_PTV130(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Rev_Data_PTV130(uint16_t Addr, uint8_t Led, uint16_t Pow);

//------------------------------ V13.3 -------------------------------------------
extern uint32_t Get_Emit_Data_PDV133(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Emit_Data_PDV1331(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Emit_Data_PDV1333(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Emit_Data_PDV1334(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Emit_Data_PDV1335(uint16_t Addr, uint8_t Led, uint16_t Pow);

//------------------------------ PT-V15.0 -------------------------------------------
extern uint32_t Get_Emit_Data_PTV150(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Rev_Data_PTV150(uint16_t Addr, uint8_t Led, uint16_t Pow);

//------------------------------ V6.7 -------------------------------------------
extern uint32_t Get_Emit_Data_PDV67(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Emit_Data_PTV67(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Rev_Data_PTV67(uint16_t Addr, uint8_t Led, uint16_t Pow);

//------------------------------ V6.8 -------------------------------------------
extern uint32_t Get_Emit_Data_PDV68(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Emit_Data_PTV68(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Rev_Data_PTV68(uint16_t Addr, uint8_t Led, uint16_t Pow);

//------------------------------ V6.9 -------------------------------------------
extern uint32_t Get_Emit_Data_PDV69(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Emit_Data_PDV69_V68(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Emit_Data_PTV69(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Emit_Data_PDV693_V68(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Emit_Data_PDV693_V69(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_Rev_Data_PTV69(uint16_t Addr, uint8_t Led, uint16_t Pow);

//------------------------------ TypeX PD V5.2 -------------------------------------------
extern uint32_t Get_EmitX_Data_TypeX_V52(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_EmitY_Data_TypeX_V52(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_RevX_Data_TypeX_PDV52(uint16_t Addr, uint8_t Led, uint16_t Gain);
extern uint32_t Get_RevY_Data_TypeX_PDV52(uint16_t Addr, uint8_t Led, uint16_t Gain);

//------------------------------ TypeX PD V5.3 -------------------------------------------
extern uint32_t Get_EmitX_Data_TypeX_V53(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_EmitY_Data_TypeX_V53(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_RevX_Data_TypeX_PDV53(uint16_t Addr, uint8_t Led, uint16_t Gain);
extern uint32_t Get_RevY_Data_TypeX_PDV53(uint16_t Addr, uint8_t Led, uint16_t Gain);

//------------------------------ TypeX PD V12.0 -------------------------------------------
extern uint32_t Get_EmitX_Data_TypeX_V120(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_EmitY_Data_TypeX_V120(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_RevX_Data_TypeX_PDV120(uint16_t Addr, uint8_t Led, uint16_t Gain);
extern uint32_t Get_RevY_Data_TypeX_PDV120(uint16_t Addr, uint8_t Led, uint16_t Gain);

//------------------------------ TypeX PD V12.1 -------------------------------------------
extern uint32_t Get_EmitX_Data_TypeX_V121(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_EmitY_Data_TypeX_V121(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_RevX_Data_TypeX_PDV121(uint16_t Addr, uint8_t Led, uint16_t Gain);
extern uint32_t Get_RevY_Data_TypeX_PDV121(uint16_t Addr, uint8_t Led, uint16_t Gain);

//------------------------------ TypeX PD V12.2 -------------------------------------------
extern uint32_t Get_EmitX_Data_TypeX_V122(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_EmitY_Data_TypeX_V122(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_RevX_Data_TypeX_PDV122(uint16_t Addr, uint8_t Led, uint16_t Gain);
extern uint32_t Get_RevY_Data_TypeX_PDV122(uint16_t Addr, uint8_t Led, uint16_t Gain);

//------------------------------ TypeX PT 15" V14.0 ---------------------------------------
extern uint32_t Get_EmitX_Data_TypeX_V140(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_EmitY_Data_TypeX_V140(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_RevX_Data_TypeX_PTV140(uint16_t Addr, uint8_t Led, uint16_t Gain);
extern uint32_t Get_RevY_Data_TypeX_PTV140(uint16_t Addr, uint8_t Led, uint16_t Gain);

//------------------------------ TypeX PT 15" V14.2 ---------------------------------------
extern uint32_t Get_EmitX_Data_TypeX_V142(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_EmitY_Data_TypeX_V142(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_RevX_Data_TypeX_PTV142(uint16_t Addr, uint8_t Led, uint16_t Gain);
extern uint32_t Get_RevY_Data_TypeX_PTV142(uint16_t Addr, uint8_t Led, uint16_t Gain);

//------------------------------ TypeX PT E19 19" V14.3 ---------------------------------------
extern uint32_t Get_EmitX_Data_TypeX_V143(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_EmitY_Data_TypeX_V143(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_RevX_Data_TypeX_PTV143(uint16_t Addr, uint8_t Led, uint16_t Gain);
extern uint32_t Get_RevY_Data_TypeX_PTV143(uint16_t Addr, uint8_t Led, uint16_t Gain);

//------------------------------ TypeX PT E20.1 20.1" V14.5 ---------------------------------------
extern uint32_t Get_EmitX_Data_TypeX_V145(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_EmitY_Data_TypeX_V145(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_RevX_Data_TypeX_PTV145(uint16_t Addr, uint8_t Led, uint16_t Gain);
extern uint32_t Get_RevY_Data_TypeX_PTV145(uint16_t Addr, uint8_t Led, uint16_t Gain);

//------------------------------ TypeX PT E12.1 V14.7 ---------------------------------------
extern uint32_t Get_EmitX_Data_TypeX_V147(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_EmitY_Data_TypeX_V147(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_RevX_Data_TypeX_PTV147(uint16_t Addr, uint8_t Led, uint16_t Gain);
extern uint32_t Get_RevY_Data_TypeX_PTV147(uint16_t Addr, uint8_t Led, uint16_t Gain);

//------------------------------ TypeX PT E15 V14.8 ---------------------------------------
extern uint32_t Get_EmitX_Data_TypeX_V148(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_EmitY_Data_TypeX_V148(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_RevX_Data_TypeX_PTV148(uint16_t Addr, uint8_t Led, uint16_t Gain);
extern uint32_t Get_RevY_Data_TypeX_PTV148(uint16_t Addr, uint8_t Led, uint16_t Gain);

//------------------------------ TypeX V14.3 E24" PT ----------------------------
extern uint32_t Get_EmitY_Data_TypeX_V146(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_RevX_Data_TypeX_PTV146(uint16_t Addr, uint8_t Led, uint16_t Gain);

//------------------------------ TypeX PT 15" V14.1 ---------------------------------------
extern uint32_t Get_EmitX_Data_TypeX_V141(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_EmitY_Data_TypeX_V141(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_RevX_Data_TypeX_PTV141(uint16_t Addr, uint8_t Led, uint16_t Gain);
extern uint32_t Get_RevY_Data_TypeX_PTV141(uint16_t Addr, uint8_t Led, uint16_t Gain);


//------------------------------ fct ---------------------------------------
extern uint32_t Get_EmitX_Data_FCT(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_EmitY_Data_FCT(uint16_t Addr, uint8_t Led, uint16_t Pow);
extern uint32_t Get_RevX_Data_FCT(uint16_t Addr, uint8_t Led, uint16_t Gain);
extern uint32_t Get_RevY_Data_FCT(uint16_t Addr, uint8_t Led, uint16_t Gain);

#endif


