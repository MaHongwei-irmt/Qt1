//---------------------------------------------------------------------------


#pragma hdrstop

#include "GenTable.h"
#include "math.h"

//---------------------------------------------------------------------------
const uint16_t PwRemap_V46[] = {2, 1, 0, 3, 5, 7, 6, 4};
const uint16_t Gain1Remap_V46[] = {4, 6, 7, 5, 3, 0, 1, 2};
const uint16_t Gain2Remap_V46[] = {3<<3, 0<<3, 1<<3, 2<<3};
const uint16_t RevSegRemap_V46[] = {4, 6, 7, 5, 3, 0, 1, 2};
const uint16_t RevSeg5Remap_V46[] = {6, 4, 7, 5, 3, 0, 1, 2};
const uint16_t RevComRemap_V46[] = {7, 6, 4, 10, 2, 1, 0, 3, 5};

const uint16_t PwRemap_V54[] = {2, 1, 0, 3, 5, 7, 6, 4};
const uint16_t EmitSegRemap_V54[] = {6, 5, 4, 3, 2, 1, 0, 7};
const uint16_t EmitComRemap_V54[] = {4<<1, 3<<1, 2<<1, 1<<1, 0<<1, 5<<1, 6<<1, 7<<1, 7<<1|1, 6<<1|1, 5<<1|1, 4<<1|1, 3<<1|1, 2<<1|1, 1<<1|1, 0<<1|1};
const uint16_t Gain1Remap_PDV54[] = {4, 6, 7, 5, 3, 0, 1, 2};
const uint16_t Gain2Remap_PDV54[] = {3<<3, 0<<3, 1<<3, 2<<3};
const uint16_t RevSegRemap_PDV54[] = {4, 6, 7, 5, 3, 0, 1, 2};
const uint16_t RevComRemap_PDV54[] = {7, 6, 4, 5, 2, 10, 9, 8, 11, 15, 14, 12, 0, 1, 3, 13};
const uint16_t Gain1Remap_PTV54[] = {4, 6, 7, 5, 3, 0, 1, 2};
const uint16_t Gain2Remap_PTV54[] = {3<<3, 0<<3, 1<<3, 2<<3};
const uint16_t RevSegRemap_PTV54[] = {2, 1, 0, 3, 7, 6, 5, 4};
const uint16_t RevComRemap_PTV54[] = {0, 1, 2, 3, 4, 6, 5, 7, 8, 9, 10, 15};

//------------------------------------------------------------------------------
const uint16_t PwRemap_V56[] = {2, 1, 0, 3, 5, 7, 6, 4};
const uint16_t EmitSegRemap_V56[] = {0, 1, 2, 3, 4, 5, 6, 7};
const uint16_t EmitComRemap_V56[] = {4, 3, 2, 0, 1, 5, 6, 7};
const uint16_t Gain1Remap_PTV56[] = {4, 6, 7, 5, 3, 0, 1, 2};
const uint16_t Gain2Remap_PTV56[] = {3<<3, 0<<3, 1<<3, 2<<3};
const uint16_t RevSegRemap_PDV56[] = {4, 6, 7, 5, 3, 0, 1, 2};
const uint16_t RevComRemap_PDV56[] = {7, 4, 6, 2, 1, 0, 3, 5};

//------------------------------------------------------------------------------
const uint16_t EmitSegRemap_V57[] = {0, 1, 2, 3, 4, 5, 6, 7};
const uint16_t EmitComRemap_V57[] = {9, 7, 5, 1, 3, 11, 13, 15, 6, 4, 2, 0, 14, 12, 10, 8};
const uint16_t RevSegRemap_PDV57[] = {4, 6, 7, 5, 3, 0, 1, 2};
const uint16_t RevComRemap_PDV57[] = {7, 4, 6, 2, 1, 0, 3, 5, 13, 15, 14, 12, 8, 9, 10, 11};

//------------------------------------------------------------------------------
const uint16_t PwRemap_V65[] = {2, 1, 0, 3, 5, 7, 6, 4};
const uint16_t Gain1Remap_V65[] = {4, 6, 7, 5, 3, 0, 1, 2};
const uint16_t Gain2Remap_V65[] = {3<<3, 0<<3, 1<<3, 2<<3};
const uint16_t RevSegRemap_V65[] = {5, 7, 4, 6, 3, 0, 1, 2};
const uint16_t RevComRemap_V65[] = {6, 7, 5, 8, 2, 1, 0, 3, 4};

const uint16_t PwRemap_V70[] = {2, 1, 0, 3, 5, 7, 6, 4};
const uint16_t Gain1Remap_V70[] = {4, 6, 7, 5, 3, 0, 1, 2};
const uint16_t Gain2Remap_V70[] = {3<<3, 0<<3, 1<<3, 2<<3};
const uint16_t RevSegRemap_V70[] = {4, 6, 7, 5, 3, 0, 1, 2};
const uint16_t RevComRemap_V70[] = {4, 7, 5, 8, 2, 1, 0, 3, 6};

const uint16_t PwRemap_X_PTV11[] = {2, 1, 0, 3, 5, 7, 6, 4};
const uint16_t Gain1Remap_X_PTV11[] = {4, 6, 7, 5, 3, 0, 1, 2};
const uint16_t Gain2Remap_X_PTV11[] = {3<<3, 0<<3, 1<<3, 2<<3};

const uint16_t HC4051Remap_TypeX_PTV11[] = {5, 7, 6, 4, 2, 1, 0, 3};
const uint16_t EmitXSegRemap_TypeX_V11[] = {6, 4, 2, 1, 5, 0, 3, 7};
const uint16_t EmitXComRemap_TypeX_V11[] = {5, 1, 6, 2, 4, 0, 13, 14, 10, 11, 12};
const uint16_t EmitYSegRemap_TypeX_V11[] = {7, 1, 4, 5, 2, 3, 6, 0};
const uint16_t EmitYComRemap_TypeX_V11[] = {16, 17, 18, 19, 20, 21, 22, 23, 23,23,23,23,23,23,23,23};
const uint16_t RevXSegRemap_TypeX_PTV11[] = {5, 7, 3, 1, 2, 4, 0, 6};
const uint16_t RevXComRemap_TypeX_PTV11[] = {2, 1, 0, 3, 5, 4, 28, 26, 25, 24, 27};
const uint16_t RevYSegRemap_TypeX_PTV11[] = {6, 7, 3, 2, 1, 4, 0, 5};
const uint16_t RevYComRemap_TypeX_PTV11[] = {22, 20, 16, 18, 19, 17, 8,8,8,8,8,8,8,8,8,8,8};

const uint16_t HC4051Remap_TypeX_PTV80[] = {4, 6, 7, 5, 3, 0, 1, 2};
const uint16_t EmitXSegRemap_TypeX_V80[] = {14, 6, 10, 2, 12, 4, 8, 0};
const uint16_t EmitXComRemap_TypeX_V80[] = {7, 11, 3, 5, 13, 4, 12, 2, 10, 6, 8, 0, 14, 14, 14, 14};
const uint16_t EmitYSegRemap_TypeX_V80[] = {15,7, 11, 3, 13, 5, 9, 1};
const uint16_t EmitYComRemap_TypeX_V80[] = {2, 10, 6, 0, 8, 4, 12, 15, 15, 15, 15, 15, 15, 15, 15, 15};
const uint16_t RevXSegRemap_TypeX_PTV80[] = {4, 6, 7, 5, 3, 0, 1, 2};
const uint16_t RevXComRemap_TypeX_PTV80[] = {6, 7, 5, 4, 1, 2, 0, 0};
const uint16_t RevYSegRemap_TypeX_PTV80[] = {4, 6, 7, 5, 3, 0, 1, 2};
const uint16_t RevYComRemap_TypeX_PTV80[] = {7, 5, 6, 4, 0, 0, 0, 0};

const uint16_t EmitXSegRemap_TypeX_JYT6N_VR1[] = {7, 2, 1, 0, 3, 4, 5, 6};
const uint16_t EmitXComRemap_TypeX_JYT6N_VR1[] = {9, 11, 10, 15, 14, 13, 12, 8, 7, 2, 3, 4, 5, 6, 0, 0};
const uint16_t EmitYSegRemap_TypeX_JYT6N_VR1[] = {0, 1, 2, 3, 4, 5, 6, 7};
const uint16_t EmitYComRemap_TypeX_JYT6N_VR1[] = {11, 12, 13, 14, 15, 5, 4, 0, 1, 2, 3, 7, 7, 7, 7, 7};
const uint16_t RevXSegRemap_TypeX_JYT6N_PDVR1[] = {4, 6, 7, 5, 3, 0, 1, 2};
const uint16_t RevXComRemap_TypeX_JYT6N_PDVR1[] = {28, 30, 29, 31, 24, 26, 25, 4, 6, 7, 3, 0, 1, 2, 5, 5, 5};
const uint16_t RevYSegRemap_TypeX_JYT6N_PDVR1[] = {4, 6, 7, 5, 2, 1, 0, 3};
const uint16_t RevYComRemap_TypeX_JYT6N_PDVR1[] = {20, 22, 23, 21, 17, 18, 10, 9, 8, 11, 13, 12, 12, 12, 12, 12};

const uint16_t EmitXSegRemap_TypeX_JYT6N_VR2[] = {7, 2, 1, 0, 3, 4, 5, 6};
const uint16_t EmitXComRemap_TypeX_JYT6N_VR2[] = {8, 9, 10, 12, 11, 13, 7, 6, 5, 4, 0, 0, 0, 0, 0, 0};
const uint16_t EmitYSegRemap_TypeX_JYT6N_VR2[] = {0, 1, 2, 3, 4, 5, 6, 7};
const uint16_t EmitYComRemap_TypeX_JYT6N_VR2[] = {3, 4, 5, 7, 6, 2, 1, 0, 7, 7, 7, 7, 7, 7, 7, 7};
const uint16_t RevXSegRemap_TypeX_JYT6N_PDVR2[] = {5, 7, 6, 4, 2, 1, 0, 3};
const uint16_t RevXComRemap_TypeX_JYT6N_PDVR2[] = {3, 4, 5, 6, 7, 15, 14, 0 , 1, 2, 8, 9, 10, 11, 12, 13};
const uint16_t RevYSegRemap_TypeX_JYT6N_PDVR2[] = {4, 6, 7, 5, 2, 1, 0, 3};
const uint16_t RevYComRemap_TypeX_JYT6N_PDVR2[] = {5, 7, 4, 6, 3, 0, 1, 2, 7, 7, 7, 7, 7, 7, 7, 7};


const uint16_t HC4051Remap_V90[] = {4, 6, 7, 5, 3, 0, 1, 2};
const uint16_t EmitSegRemap_V90[] = {3, 7, 5, 1, 6, 2, 4, 0};
const uint16_t EmitComRemap_V90[] = {6, 10, 2, 14, 0, 7, 11, 3, 13, 5, 9, 1, 15, 15, 15, 15};
const uint16_t RevSegRemap_V90[] = {6,4,7,5,3,0,1,2, 5,6,4,7,3,0,1,2, 4,6,7,5,3,0,2,1};
const uint16_t RevComRemap_V90[] = {1, 0, 2, 3};
const uint16_t VrRemap_V90[] = {1<<10, 1<<8, 1<<11, 1<<9};
const uint16_t EmitAddRemap_V90[] = {1<<14, 1<<13, 1<<12, 1<<11, 1<<9, 1<<10, 1<<15};
const uint16_t RevAddRemap_V90[] = {7, 3, 5, 1, 6, 2, 4};

//21.5寸 二极管
const uint16_t HC4051Remap_TypeX_PDVR2[] = {2, 1, 0, 3, 5, 7, 6, 4};
const uint16_t EmitXSegRemap_TypeX_VR2[] = {0, 4, 2, 6, 1, 5, 3, 7};
const uint16_t EmitXComRemap_TypeX_VR2[] = {6, 10, 2, 12, 4, 14, 1, 9, 5, 13, 3, 11, 15, 15, 15, 15};
const uint16_t EmitYSegRemap_TypeX_VR2[] = {0, 4, 2, 6, 1, 5, 3, 7};
const uint16_t EmitYComRemap_TypeX_VR2[] = {6, 2, 7, 0, 1, 5, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4};
const uint16_t RevXSegRemap_TypeX_PDVR2[] = {2, 1, 0, 3, 5, 7, 6, 4};
const uint16_t RevXComRemap_TypeX_PDVR2[] = {12, 11, 10, 9, 8, 13, 7, 6, 5, 4, 15, 14, 0, 0, 0, 0};
const uint16_t RevYSegRemap_TypeX_PDVR2[] = {2, 1, 0, 3, 5, 7, 6, 4};
const uint16_t RevYComRemap_TypeX_PDVR2[] = {7, 6, 4, 5, 1, 3, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2};

//V10.0 二极管 三极管
const uint16_t EmitSegRemap_V100[] = {0, 1, 2, 3, 4, 5, 6, 7};
const uint16_t EmitComRemap_V100[] = {7, 3, 5, 1, 6, 2, 4, 0, 15, 11, 13, 9, 14, 10, 12, 8};
const uint16_t EmitMainSegRemap_V100[] = {0, 1, 2, 3, 4, 5, 6, 7};
const uint16_t EmitMainComRemap_V100[] = {2, 1, 3, 5, 6, 4, 15, 14, 13, 12, 11, 10, 9, 8, 8, 8};
const uint16_t RevSegRemap_PTV100[] = {0,3,6,4,7,5,1,2, 6,4,7,5,3,0,2,1, 4,6,7,5,2,1,0,3};
const uint16_t RevComRemap_PTV100[] = {1<<7, 1<<6, 1<<5, 1<<5};
const uint16_t RevVrRemap_PTV100[] = {3,2,1,0};

//V13.0 二极管 三极管
const uint16_t EmitSegRemap_V130[] = {0, 4, 2, 6, 1, 5, 3, 7};
const uint16_t EmitComRemap_V130[] = {7, 6, 5, 4, 3, 2, 1, 0, 12, 10, 9 ,8, 11, 13, 14, 15};
const uint16_t EmitMainSegRemap_V130[] = {0, 4, 2, 6, 1, 5, 3, 7};
const uint16_t EmitMainComRemap_V130[] = {2, 1, 3, 5, 6, 4, 0, 7};
const uint16_t RevSegRemap_PTV130[] = {4, 6, 7, 5, 1, 0, 3, 2};
const uint16_t RevComRemap_PTV130[] = {1, 2, 3, 3};
const uint16_t RevVrRemap_PTV130[] = {0, 1, 4, 5, 2, 3, 6, 7};


//V6.7 二极管 三极管  from V10
const uint16_t EmitSegRemap_V67[] = {0, 4, 2, 6, 1, 5, 3, 7};
const uint16_t EmitComRemap_V67[] = {6, 5, 4, 3, 2, 14, 13, 12, 9, 10, 11, 15, 7, 7, 7, 7};
const uint16_t RevSegRemap_PTV67[] = {0,3,6,4,7,5,1,2, 6,4,7,5,3,0,2,1, 4,6,7,5,2,1,0,3};
const uint16_t RevComRemap_PTV67[] = {1<<7, 1<<6, 1<<5, 1<<5};
const uint16_t RevVrRemap_PTV67[] = {3,2,1,0};

//V6.8 二极管 三极管  from V10
const uint16_t EmitSegRemap_V68[] = {0, 4, 2, 6, 1, 5, 3, 7};
const uint16_t EmitComRemap_V68[] = {7, 6, 5, 4, 3, 2, 1, 0, 12, 13, 14, 15, 8, 8, 8, 8};
const uint16_t RevSegRemap_PTV68[] = {0,3,6,4,7,5,1,2, 6,4,7,5,3,0,2,1, 4,6,7,5,2,1,0,3};
const uint16_t RevComRemap_PTV68[] = {1<<7, 1<<6, 1<<5, 1<<5};
const uint16_t RevVrRemap_PTV68[] = {3,2,1,0};

//V6.9 二极管 三极管  from V10
const uint16_t EmitSegRemap_V69[] = {0, 4, 2, 6, 1, 5, 3, 7};
const uint16_t EmitComRemap_V69[] = {7, 6, 5, 4, 3, 2, 1, 0, 8, 9, 10, 11, 15, 15, 15, 15};
const uint16_t RevSegRemap_PTV69[] = {0,3,6,4,7,5,1,2, 6,4,7,5,3,0,2,1, 4,6,7,5,2,1,0,3};
const uint16_t RevComRemap_PTV69[] = {1<<7, 1<<6, 1<<5, 1<<5};
const uint16_t RevVrRemap_PTV69[] = {3,2,1,0};

//V6.9-3N 二极管 三极管  from V10
const uint16_t EmitSegRemap_V693[] = {0, 1, 2, 3, 4, 5, 6, 7};
const uint16_t EmitComRemap_V693[] = {7, 6, 5, 4, 3, 2, 1, 0, 12, 13, 14, 15, 8, 9, 10, 11};


//21.5"~27" V5.2
const uint16_t EmitXSegRemap_TypeX_V52[] = {7, 3, 5, 1, 6, 2, 4, 0};
const uint16_t EmitXComRemap_TypeX_V52[] = {1, 2, 3, 0, 7, 6, 5, 9, 8, 15, 13, 12, 11, 10, 14, 4};
const uint16_t EmitYSegRemap_TypeX_V52[] = {7, 3, 5, 1, 6, 2, 4, 0};
const uint16_t EmitYComRemap_TypeX_V52[] = {2, 3, 0, 1, 4, 11, 12, 13, 14, 15, 15, 15, 15, 15, 15, 15};

const uint16_t RevXSegRemap_TypeX_PDV52[] = {4, 6, 7, 5, 3, 0, 1, 2};
const uint16_t RevXComRemap_TypeX_PDV52[] = {6, 7, 5, 4, 1, 2, 0, 0};
const uint16_t RevYSegRemap_TypeX_PDV52[] = {4, 6, 7, 5, 3, 0, 1, 2};
const uint16_t RevYComRemap_TypeX_PDV52[] = {7, 5, 6, 4, 0, 0, 0, 0};

//15“ ~ 27”  V12
const uint16_t EmitXSegRemap_TypeX_V120[] = {7, 3, 5, 1, 6, 2, 4, 0};
const uint16_t EmitXComRemap_TypeX_V120[] = {1, 2, 3, 0, 7, 6, 5, 4, 8, 15, 13, 12, 11, 10, 14, 9};
const uint16_t EmitYSegRemap_TypeX_V120[] = {7, 3, 5, 1, 6, 2, 4, 0};
const uint16_t EmitYComRemap_TypeX_V120[] = {1, 2, 3, 0, 7, 6, 5, 4, 8, 15, 13, 12, 11, 10, 14, 9};
const uint16_t RevXSegRemap_TypeX_PDV120[] = {5, 7, 6, 4, 2, 1, 0, 3};
const uint16_t RevXComRemap_TypeX_PDV120[] = {7, 6, 5, 4, 3, 2, 1, 0, 15, 14, 13, 12, 11, 10, 9, 8};
const uint16_t RevYSegRemap_TypeX_PDV120[] = {5, 7, 6, 4, 2, 1, 0, 3};
const uint16_t RevYComRemap_TypeX_PDV120[] = {7, 6, 5, 4, 3, 2, 1, 0, 15, 14, 13, 12, 11, 10, 9, 8};

//15“ ~ 19”  V12.1
const uint16_t EmitXSegRemap_TypeX_V121[] = {7, 3, 5, 1, 6, 2, 4, 0};
const uint16_t EmitXComRemap_TypeX_V121[] = {6, 5, 0, 1, 14, 13, 12, 11, 10, 9, 7, 7, 7, 7, 7, 7};
const uint16_t EmitYSegRemap_TypeX_V121[] = {7, 3, 5, 1, 6, 2, 4, 0};
const uint16_t EmitYComRemap_TypeX_V121[] = {6, 5, 0, 1, 14, 13, 12, 11, 10, 9, 7, 7, 7, 7, 7, 7};
const uint16_t RevXSegRemap_TypeX_PDV121[] = {3, 0, 1, 2, 4, 6, 7, 5};
const uint16_t RevXComRemap_TypeX_PDV121[] = {3, 0, 1, 2, 12, 14, 4, 6, 7, 5, 8, 8, 8, 8, 8, 8};
const uint16_t RevYSegRemap_TypeX_PDV121[] = {3, 0, 1, 2, 4, 6, 7, 5};
const uint16_t RevYComRemap_TypeX_PDV121[] = {2, 3, 0, 1, 4, 6, 7, 5};

//15“ ~ 19”  V12.1
const uint16_t EmitXSegRemap_TypeX_V122[] = {7, 3, 5, 1, 6, 2, 4, 0};
const uint16_t EmitXComRemap_TypeX_V122[] = {3, 2, 0, 1, 7, 6, 5, 4, 10, 15, 15, 15, 15, 15, 15, 15};
const uint16_t EmitYSegRemap_TypeX_V122[] = {7, 3, 5, 1, 6, 2, 4, 0};
const uint16_t EmitYComRemap_TypeX_V122[] = {3, 2, 0, 1, 7, 6, 5, 4};

const uint16_t RevXSegRemap_TypeX_PDV122[] = {3, 0, 1, 2, 4, 6, 7, 5};
const uint16_t RevXComRemap_TypeX_PDV122[] = {3, 0, 1, 2, 7, 5, 4, 6, 12, 15,15,15,15,15,15,15};
const uint16_t RevYSegRemap_TypeX_PDV122[] = {3, 0, 1, 2, 4, 6, 7, 5};
const uint16_t RevYComRemap_TypeX_PDV122[] = {2, 3, 0, 1, 4, 6, 7, 5};

//15" PT
const uint32_t EmitXSegRemap_TypeX_V140[] = {(0x0F<<16+7 | 0x0F<<7),
											 (0x0F<<16+7 | 0x0E<<7),
											 (0x0F<<16+7 | 0x0D<<7),
											 (0x0F<<16+7 | 0x0C<<7),
											 (0x0F<<16+7 | 0x0B<<7),
											 (0x0F<<16+7 | 0x0A<<7),
											 (0x0F<<16+7 | 0x09<<7),
											 (0x0F<<16+7 | 0x08<<7)};

const uint32_t EmitXComRemap_TypeX_V140[] = {(0x72<<16 | 0x060<<0),
											 (0x72<<16 | 0x040<<0),
											 (0x72<<16 | 0x050<<0),
											 (0x72<<16 | 0x042<<0),
											 (0x72<<16 | 0x062<<0),
											 (0x72<<16 | 0x070<<0),
											 (0x72<<16 | 0x052<<0),
											 (0x72<<16 | 0x072<<0)};

const uint32_t EmitYSegRemap_TypeX_V140[] = {(0x0F<<16+7 | 2<<7),
											 (0x0F<<16+7 | 1<<7),
											 (0x0F<<16+7 | 0<<7),
											 (0x0F<<16+7 | 7<<7),
											 (0x0F<<16+7 | 6<<7),
											 (0x0F<<16+7 | 5<<7),
											 (0x0F<<16+7 | 4<<7),
											 (0x0F<<16+7 | 3<<7)};

const uint32_t EmitYComRemap_TypeX_V140[] = {(0x72<<16 | 0x010<<0),
											 (0x72<<16 | 0x002<<0),
											 (0x72<<16 | 0x000<<0),
											 (0x72<<16 | 0x032<<0),
											 (0x72<<16 | 0x012<<0),
											 (0x72<<16 | 0x030<<0),
											 (0x72<<16 | 0x022<<0),
											 (0x72<<16 | 0x020<<0)};

const uint32_t RevXSegRemap_TypeX_PTV140[] = {(0x07<<16+3 | 0<<3),
											  (0x07<<16+3 | 1<<3),
											  (0x07<<16+3 | 2<<3),
											  (0x07<<16+3 | 3<<3),
											  (0x07<<16+3 | 4<<3),
											  (0x07<<16+3 | 5<<3),
											  (0x07<<16+3 | 6<<3),
											  (0x07<<16+3 | 7<<3)};

const uint32_t RevXComRemap_TypeX_PTV140[] = {((uint32_t)0x3B<<16+10 | 0x20<<10),
											  ((uint32_t)0x3B<<16+10 | 0x22<<10),
											  ((uint32_t)0x3B<<16+10 | 0x21<<10),
											  ((uint32_t)0x3B<<16+10 | 0x23<<10),
											  ((uint32_t)0x3B<<16+10 | 0x29<<10),
											  ((uint32_t)0x3B<<16+10 | 0x2A<<10),
											  ((uint32_t)0x3B<<16+10 | 0x28<<10),
											  ((uint32_t)0x3B<<16+10 | 0x2B<<10)};

const uint32_t RevYSegRemap_TypeX_PTV140[] = {(0x07<<16+3 | 0<<3),
											  (0x07<<16+3 | 1<<3),
											  (0x07<<16+3 | 2<<3),
											  (0x07<<16+3 | 3<<3),
											  (0x07<<16+3 | 4<<3),
											  (0x07<<16+3 | 5<<3),
											  (0x07<<16+3 | 6<<3),
											  (0x07<<16+3 | 7<<3)};

const uint32_t RevYComRemap_TypeX_PTV140[] = {((uint32_t)0x3B<<16+10 | 0x10<<10),
											  ((uint32_t)0x3B<<16+10 | 0x12<<10),
											  ((uint32_t)0x3B<<16+10 | 0x11<<10),
											  ((uint32_t)0x3B<<16+10 | 0x13<<10),
											  ((uint32_t)0x3B<<16+10 | 0x19<<10),
											  ((uint32_t)0x3B<<16+10 | 0x1B<<10),
											  ((uint32_t)0x3B<<16+10 | 0x1A<<10),
											  ((uint32_t)0x3B<<16+10 | 0x18<<10)};

//21.5" PT
const uint32_t EmitXSegRemap_TypeX_V142[] = {(0x0702<<16 | 0x202<<0),
											 (0x0702<<16 | 0x402<<0),
											 (0x0702<<16 | 0x002<<0),
											 (0x0702<<16 | 0x702<<0),
											 (0x0702<<16 | 0x302<<0),
											 (0x0702<<16 | 0x502<<0),
											 (0x0702<<16 | 0x102<<0),
											 (0x0702<<16 | 0x602<<0)
											 };

const uint32_t EmitXComRemap_TypeX_V142[] = {(0xF<<16+4 | 0x07<<4),
											 (0xF<<16+4 | 0x0B<<4),
											 (0xF<<16+4 | 0x03<<4),
											 (0xF<<16+4 | 0x0D<<4),
											 (0xF<<16+4 | 0x05<<4),
											 (0xF<<16+4 | 0x0E<<4),
											 (0xF<<16+4 | 0x06<<4),
											 (0xF<<16+4 | 0x0A<<4),
											 (0xF<<16+4 | 0x02<<4),
											 (0xF<<16+4 | 0x0C<<4),
											 (0xF<<16+4 | 0x04<<4),

											 (0xF<<16+4 | 0x0F<<4),
											 (0xF<<16+4 | 0x09<<4),
											 (0xF<<16+4 | 0x08<<4),
											 (0xF<<16+4 | 0x01<<4),
											 (0xF<<16+4 | 0x00<<4),
											 };

const uint32_t EmitYSegRemap_TypeX_V142[] = {(0x0702<<16 | 0x200<<0),
											 (0x0702<<16 | 0x400<<0),
											 (0x0702<<16 | 0x000<<0),
											 (0x0702<<16 | 0x700<<0),
											 (0x0702<<16 | 0x300<<0),
											 (0x0702<<16 | 0x500<<0),
											 (0x0702<<16 | 0x100<<0),
											 (0x0702<<16 | 0x600<<0)
											 };

const uint32_t EmitYComRemap_TypeX_V142[] = {(0xF<<16+4 | 0x04<<4),
											 (0xF<<16+4 | 0x08<<4),
											 (0xF<<16+4 | 0x00<<4),
											 (0xF<<16+4 | 0x0E<<4),
											 (0xF<<16+4 | 0x06<<4),
											 (0xF<<16+4 | 0x0A<<4),
											 (0xF<<16+4 | 0x02<<4),
											 (0xF<<16+4 | 0x0C<<4),
											 };

const uint32_t RevXSegRemap_TypeX_PTV142[] = {(0x07<<16+3 | 7<<3),
											  (0x07<<16+3 | 3<<3),
											  (0x07<<16+3 | 6<<3),
											  (0x07<<16+3 | 1<<3),
											  (0x07<<16+3 | 5<<3),
											  (0x07<<16+3 | 2<<3),
											  (0x07<<16+3 | 0<<3),
											  (0x07<<16+3 | 4<<3)};

const uint32_t RevXComRemap_TypeX_PTV142[] = {( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 7<<13 | 2<<10 | 1<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 3<<13 | 2<<10 | 1<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 1<<13 | 2<<10 | 1<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 5<<13 | 2<<10 | 1<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 4<<13 | 2<<10 | 1<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 0<<13 | 2<<10 | 1<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 6<<13 | 2<<10 | 1<<6),

											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 1<<13 | 1<<10 | 1<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 3<<13 | 1<<10 | 1<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 7<<13 | 1<<10 | 1<<6),

											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 2<<13 | 2<<10 | 1<<6),

											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 0<<13 | 1<<10 | 1<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 4<<13 | 1<<10 | 1<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 2<<13 | 1<<10 | 1<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 6<<13 | 1<<10 | 1<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 5<<13 | 1<<10 | 1<<6)
											  };

const uint32_t RevYSegRemap_TypeX_PTV142[] = {(0x07<<16+3 | 7<<3),
											  (0x07<<16+3 | 3<<3),
											  (0x07<<16+3 | 6<<3),
											  (0x07<<16+3 | 1<<3),
											  (0x07<<16+3 | 5<<3),
											  (0x07<<16+3 | 2<<3),
											  (0x07<<16+3 | 0<<3),
											  (0x07<<16+3 | 4<<3)};

const uint32_t RevYComRemap_TypeX_PTV142[] = {( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 3<<13 | 3<<10 | 0<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 1<<13 | 3<<10 | 0<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 7<<13 | 3<<10 | 0<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 5<<13 | 3<<10 | 0<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 0<<13 | 3<<10 | 0<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 6<<13 | 3<<10 | 0<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 4<<13 | 3<<10 | 0<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 2<<13 | 3<<10 | 0<<6),
											  };

// E19" PT ---------------------------------------------------------------------------
const uint32_t EmitXSegRemap_TypeX_V143[] = {(0x0702<<16 | 0x202<<0),
											 (0x0702<<16 | 0x402<<0),
											 (0x0702<<16 | 0x002<<0),
											 (0x0702<<16 | 0x702<<0),
											 (0x0702<<16 | 0x302<<0),
											 (0x0702<<16 | 0x502<<0),
											 (0x0702<<16 | 0x102<<0),
											 (0x0702<<16 | 0x602<<0)
											 };

const uint32_t EmitXComRemap_TypeX_V143[] = {(0x7<<16+5 | 3<<5),
											 (0x7<<16+5 | 5<<5),
											 (0x7<<16+5 | 1<<5),
											 (0x7<<16+5 | 6<<5),
											 (0x7<<16+5 | 2<<5),
											 (0x7<<16+5 | 7<<5),
											 (0x7<<16+5 | 0<<5),
											 (0x7<<16+5 | 4<<5),
											 };

const uint32_t EmitYSegRemap_TypeX_V143[] = {(0x0702<<16 | 0x200<<0),
											 (0x0702<<16 | 0x400<<0),
											 (0x0702<<16 | 0x000<<0),
											 (0x0702<<16 | 0x700<<0),
											 (0x0702<<16 | 0x300<<0),
											 (0x0702<<16 | 0x500<<0),
											 (0x0702<<16 | 0x100<<0),
											 (0x0702<<16 | 0x600<<0)
											 };

const uint32_t EmitYComRemap_TypeX_V143[] = {(7<<16+5 | 2<<5),
											 (7<<16+5 | 4<<5),
											 (7<<16+5 | 0<<5),
											 (7<<16+5 | 7<<5),
											 (7<<16+5 | 3<<5),
											 (7<<16+5 | 5<<5),
											 (7<<16+5 | 1<<5),
											 (7<<16+5 | 6<<5),
											 };

const uint32_t RevXSegRemap_TypeX_PTV143[] = {(0x07<<16+3 | 7<<3),
											  (0x07<<16+3 | 3<<3),
											  (0x07<<16+3 | 6<<3),
											  (0x07<<16+3 | 1<<3),
											  (0x07<<16+3 | 5<<3),
											  (0x07<<16+3 | 2<<3),
											  (0x07<<16+3 | 0<<3),
											  (0x07<<16+3 | 4<<3)};

const uint32_t RevXComRemap_TypeX_PTV143[] = {( ((uint32_t)7<<13 | 1<<6) << 16 | 7<<13 | 1<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 3<<13 | 1<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 1<<13 | 1<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 5<<13 | 1<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 4<<13 | 1<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 0<<13 | 1<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 6<<13 | 1<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 2<<13 | 1<<6),
											  };

const uint32_t RevYSegRemap_TypeX_PTV143[] = {(0x07<<16+3 | 7<<3),
											  (0x07<<16+3 | 3<<3),
											  (0x07<<16+3 | 6<<3),
											  (0x07<<16+3 | 1<<3),
											  (0x07<<16+3 | 5<<3),
											  (0x07<<16+3 | 2<<3),
											  (0x07<<16+3 | 0<<3),
											  (0x07<<16+3 | 4<<3)};

const uint32_t RevYComRemap_TypeX_PTV143[] = {( ((uint32_t)7<<13 | 1<<6) << 16 | 3<<13 | 0<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 1<<13 | 0<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 7<<13 | 0<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 5<<13 | 0<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 0<<13 | 0<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 6<<13 | 0<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 4<<13 | 0<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 2<<13 | 0<<6),
											  };

// E20.1" PT ---------------------------------------------------------------------------
const uint32_t EmitXSegRemap_TypeX_V145[] = {(0x0702<<16 | 0x200<<0),
											 (0x0702<<16 | 0x400<<0),
											 (0x0702<<16 | 0x000<<0),
											 (0x0702<<16 | 0x700<<0),
											 (0x0702<<16 | 0x300<<0),
											 (0x0702<<16 | 0x500<<0),
											 (0x0702<<16 | 0x100<<0),
											 (0x0702<<16 | 0x600<<0)
											 };

const uint32_t EmitXComRemap_TypeX_V145[] = {(7<<16+5 | 2<<5),
											 (7<<16+5 | 4<<5),
											 (7<<16+5 | 0<<5),
											 (7<<16+5 | 7<<5),
											 (7<<16+5 | 3<<5),
											 (7<<16+5 | 5<<5),
											 (7<<16+5 | 1<<5),
											 (7<<16+5 | 6<<5),
											 };

const uint32_t EmitYSegRemap_TypeX_V145[] = {(0x0702<<16 | 0x202<<0),
											 (0x0702<<16 | 0x402<<0),
											 (0x0702<<16 | 0x002<<0),
											 (0x0702<<16 | 0x702<<0),
											 (0x0702<<16 | 0x302<<0),
											 (0x0702<<16 | 0x502<<0),
											 (0x0702<<16 | 0x102<<0),
											 (0x0702<<16 | 0x602<<0)
											 };

const uint32_t EmitYComRemap_TypeX_V145[] = {(0x7<<16+5 | 3<<5),
											 (0x7<<16+5 | 5<<5),
											 (0x7<<16+5 | 1<<5),
											 (0x7<<16+5 | 6<<5),
											 (0x7<<16+5 | 2<<5),
											 (0x7<<16+5 | 4<<5),
											 (0x7<<16+5 | 0<<5),
											 (0x7<<16+5 | 7<<5),
											 };


const uint32_t RevXSegRemap_TypeX_PTV145[] = {(0x07<<16+3 | 7<<3),
											  (0x07<<16+3 | 3<<3),
											  (0x07<<16+3 | 6<<3),
											  (0x07<<16+3 | 1<<3),
											  (0x07<<16+3 | 5<<3),
											  (0x07<<16+3 | 2<<3),
											  (0x07<<16+3 | 0<<3),
											  (0x07<<16+3 | 4<<3)};

const uint32_t RevXComRemap_TypeX_PTV145[] = {( ((uint32_t)7<<13 | 1<<6) << 16 | 3<<13 | 0<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 1<<13 | 0<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 7<<13 | 0<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 5<<13 | 0<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 0<<13 | 0<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 4<<13 | 0<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 2<<13 | 0<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 6<<13 | 0<<6),
											  };


const uint32_t RevYSegRemap_TypeX_PTV145[] = {(0x07<<16+3 | 7<<3),
											  (0x07<<16+3 | 3<<3),
											  (0x07<<16+3 | 6<<3),
											  (0x07<<16+3 | 1<<3),
											  (0x07<<16+3 | 5<<3),
											  (0x07<<16+3 | 2<<3),
											  (0x07<<16+3 | 0<<3),
											  (0x07<<16+3 | 4<<3)};

const uint32_t RevYComRemap_TypeX_PTV145[] = {( ((uint32_t)7<<13 | 1<<6) << 16 | 7<<13 | 1<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 3<<13 | 1<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 1<<13 | 1<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 5<<13 | 1<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 4<<13 | 1<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 2<<13 | 1<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 0<<13 | 1<<6),
											  ( ((uint32_t)7<<13 | 1<<6) << 16 | 6<<13 | 1<<6),
											  };

// E12.1" PT ---------------------------------------------------------------------------
const uint32_t EmitSegRemap_TypeX_V147[] = { (5<<8+16 | 2<<8),
											 (6<<8+16 | 1<<8),
											 (7<<8+16 | 0<<8),
											 (0<<8+16 | 7<<8),
											 (1<<8+16 | 6<<8),
											 (2<<8+16 | 5<<8),
											 (3<<8+16 | 4<<8),
											 (4<<8+16 | 3<<8)
											 };

const uint32_t EmitComRemap_TypeX_V147[] = { (7<<16+1 | 2<<1),
											 (7<<16+1 | 4<<1),
											 (7<<16+1 | 0<<1),
											 (7<<16+1 | 7<<1),
											 (7<<16+1 | 3<<1),
											 (7<<16+1 | 5<<1),
											 (7<<16+1 | 1<<1),
											 (7<<16+1 | 6<<1),
											 };

const uint32_t RevSegRemap_TypeX_PTV147[] = { (1<<16+13 | 6<<13),
											  (0<<16+13 | 7<<13),
											  (2<<16+13 | 5<<13),
											  (3<<16+13 | 4<<13),
											  (7<<16+13 | 0<<13),
											  (6<<16+13 | 1<<13),
											  (5<<16+13 | 2<<13),
											  (4<<16+13 | 3<<13)};

const uint32_t RevComRemap_TypeX_PTV147[] = { (7<<16+3 | 0<<3),
											  (6<<16+3 | 1<<3),
											  (5<<16+3 | 2<<3),
											  (4<<16+3 | 3<<3),
											  (3<<16+3 | 4<<3),
											  (2<<16+3 | 5<<3),
											  (1<<16+3 | 6<<3),
											  (0<<16+3 | 7<<3)};

// E15" FPC PT ---------------------------------------------------------------------------
const uint32_t EmitComRemap_TypeX_V148[8] = {    //GPIOA
//0,0,0,0,0,0,0,0
			1<<(1+16) | (~(1<<1) & 0x80FE),
			1<<(2+16) | (~(1<<2) & 0x80FE),
			1<<(3+16) | (~(1<<3) & 0x80FE),
			1<<(4+16) | (~(1<<4) & 0x80FE),
			1<<(5+16) | (~(1<<5) & 0x80FE),
			1<<(6+16) | (~(1<<6) & 0x80FE),
			1<<(7+16) | (~(1<<7) & 0x80FE),
			(uint32_t)1<<(15+16) | (~(1<<15) & 0x80FE),
		};

const uint32_t EmitSegRemap_TypeX_V148[8] = {       //GPIOB
//0,0,0,0,0,0,0,0
			(0x02FB<<16 | 1<<0),
			(0x02FB<<16 | 1<<1),
			(0x02FB<<16 | 1<<3),
			(0x02FB<<16 | 1<<4),
			(0x02FB<<16 | 1<<5),
			(0x02FB<<16 | 1<<6),
			(0x02FB<<16 | 1<<7),
			(0x02FB<<16 | 1<<9),
};

#define HC4051_CHANNEL(ch) ((uint32_t)((~ch) & 0x07)<<(13 + 16) | ch << 13)
#define REV_POWER_CHANNEL(ch) ((uint32_t)((~ch) & 0x07)<<(8 + 16) | ch << 8)

const uint32_t RevComRemap_TypeX_PTV148[8] = {       //GPIOA
//0,0,0,0,0,0,0,0
			REV_POWER_CHANNEL(0),
			REV_POWER_CHANNEL(1),
			REV_POWER_CHANNEL(2),
			REV_POWER_CHANNEL(3),
			REV_POWER_CHANNEL(4),
			REV_POWER_CHANNEL(5),
			REV_POWER_CHANNEL(6),
			REV_POWER_CHANNEL(7),
		};

const uint32_t RevSegRemap_TypeX_PTV148[8] = {     //GPIOB
//0,0,0,0,0,0,0,0
			HC4051_CHANNEL(6),
			HC4051_CHANNEL(7),
			HC4051_CHANNEL(5),
			HC4051_CHANNEL(4),
			HC4051_CHANNEL(0),
			HC4051_CHANNEL(1),
			HC4051_CHANNEL(2),
			HC4051_CHANNEL(3),
		};

//E24" ----------------------------------------------------------------------
const uint32_t EmitYComRemap_TypeX_V146[] = {(0xF<<16+4 | 0x04<<4),
											 (0xF<<16+4 | 0x08<<4),
											 (0xF<<16+4 | 0x00<<4),
											 (0xF<<16+4 | 0x0E<<4),
											 (0xF<<16+4 | 0x06<<4),
											 (0xF<<16+4 | 0x02<<4),
											 (0xF<<16+4 | 0x0A<<4),
											 (0xF<<16+4 | 0x0C<<4),
											 };
const uint32_t RevXComRemap_TypeX_PTV146[] = {( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 7<<13 | 2<<10 | 1<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 3<<13 | 2<<10 | 1<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 1<<13 | 2<<10 | 1<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 5<<13 | 2<<10 | 1<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 4<<13 | 2<<10 | 1<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 0<<13 | 2<<10 | 1<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 6<<13 | 2<<10 | 1<<6),

											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 1<<13 | 1<<10 | 1<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 3<<13 | 1<<10 | 1<<6),
                                              ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 5<<13 | 1<<10 | 1<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 7<<13 | 1<<10 | 1<<6),

											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 2<<13 | 2<<10 | 1<<6),

											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 0<<13 | 1<<10 | 1<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 4<<13 | 1<<10 | 1<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 2<<13 | 1<<10 | 1<<6),
											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 6<<13 | 1<<10 | 1<<6),

											  };

//21.5" PT XY运放分离
//const uint32_t EmitXSegRemap_TypeX_V143[] = {(0x0702<<16 | 0x202<<0),
//											 (0x0702<<16 | 0x402<<0),
//											 (0x0702<<16 | 0x002<<0),
//											 (0x0702<<16 | 0x702<<0),
//											 (0x0702<<16 | 0x302<<0),
//											 (0x0702<<16 | 0x502<<0),
//											 (0x0702<<16 | 0x102<<0),
//											 (0x0702<<16 | 0x602<<0)
//											 };
//
//const uint32_t EmitXComRemap_TypeX_V143[] = {(0xF<<16+4 | 0x07<<4),
//											 (0xF<<16+4 | 0x0B<<4),
//											 (0xF<<16+4 | 0x03<<4),
//											 (0xF<<16+4 | 0x0D<<4),
//											 (0xF<<16+4 | 0x05<<4),
//											 (0xF<<16+4 | 0x0E<<4),
//											 (0xF<<16+4 | 0x06<<4),
//											 (0xF<<16+4 | 0x0A<<4),
//											 (0xF<<16+4 | 0x02<<4),
//											 (0xF<<16+4 | 0x0C<<4),
//
//											 (0xF<<16+4 | 0x0F<<4),
//											 (0xF<<16+4 | 0x09<<4),
//											 (0xF<<16+4 | 0x08<<4),
//											 (0xF<<16+4 | 0x04<<4),
//											 (0xF<<16+4 | 0x01<<4),
//											 (0xF<<16+4 | 0x00<<4),
//											 };
//
//const uint32_t EmitYSegRemap_TypeX_V143[] = {(0x0702<<16 | 0x200<<0),
//											 (0x0702<<16 | 0x400<<0),
//											 (0x0702<<16 | 0x000<<0),
//											 (0x0702<<16 | 0x700<<0),
//											 (0x0702<<16 | 0x300<<0),
//											 (0x0702<<16 | 0x500<<0),
//											 (0x0702<<16 | 0x100<<0),
//											 (0x0702<<16 | 0x600<<0)
//											 };
//
//const uint32_t EmitYComRemap_TypeX_V143[] = {(0xF<<16+4 | 0x04<<4),
//											 (0xF<<16+4 | 0x08<<4),
//											 (0xF<<16+4 | 0x00<<4),
//											 (0xF<<16+4 | 0x0E<<4),
//											 (0xF<<16+4 | 0x06<<4),
//											 (0xF<<16+4 | 0x0A<<4),
//											 (0xF<<16+4 | 0x02<<4),
//											 (0xF<<16+4 | 0x0C<<4),
//											 };
//
//const uint32_t RevXSegRemap_TypeX_PTV143[] = {(0x07<<16+3 | 7<<3),
//											  (0x07<<16+3 | 3<<3),
//											  (0x07<<16+3 | 6<<3),
//											  (0x07<<16+3 | 1<<3),
//											  (0x07<<16+3 | 5<<3),
//											  (0x07<<16+3 | 2<<3),
//											  (0x07<<16+3 | 0<<3),
//											  (0x07<<16+3 | 4<<3)};
//
//const uint32_t RevXComRemap_TypeX_PTV143[] = {( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 7<<13 | 2<<10 | 1<<6),
//											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 3<<13 | 2<<10 | 1<<6),
//											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 1<<13 | 2<<10 | 1<<6),
//											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 5<<13 | 2<<10 | 1<<6),
//											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 4<<13 | 2<<10 | 1<<6),
//											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 0<<13 | 2<<10 | 1<<6),
//											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 6<<13 | 2<<10 | 1<<6),
//
//											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 1<<13 | 1<<10 | 1<<6),
//											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 3<<13 | 1<<10 | 1<<6),
//											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 7<<13 | 1<<10 | 1<<6),
//
//											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 2<<13 | 2<<10 | 1<<6),
//
//											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 0<<13 | 1<<10 | 1<<6),
//											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 4<<13 | 1<<10 | 1<<6),
//											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 2<<13 | 1<<10 | 1<<6),
//											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 6<<13 | 1<<10 | 1<<6),
//											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 5<<13 | 1<<10 | 1<<6)
//											  };
//
//const uint32_t RevYSegRemap_TypeX_PTV143[] = {(0x07<<16+3 | 7<<3),
//											  (0x07<<16+3 | 3<<3),
//											  (0x07<<16+3 | 6<<3),
//											  (0x07<<16+3 | 1<<3),
//											  (0x07<<16+3 | 5<<3),
//											  (0x07<<16+3 | 2<<3),
//											  (0x07<<16+3 | 0<<3),
//											  (0x07<<16+3 | 4<<3)};
//
//const uint32_t RevYComRemap_TypeX_PTV143[] = {( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 3<<13 | 3<<10 | 0<<6),
//											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 1<<13 | 3<<10 | 0<<6),
//											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 7<<13 | 3<<10 | 0<<6),
//											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 5<<13 | 3<<10 | 0<<6),
//											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 0<<13 | 3<<10 | 0<<6),
//											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 6<<13 | 3<<10 | 0<<6),
//											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 4<<13 | 3<<10 | 0<<6),
//											  ( ((uint32_t)7<<13 | 3<<10 | 1<<6) << 16 | 2<<13 | 3<<10 | 0<<6),
//											  };

//15" - 19" PT
const uint32_t EmitXSegRemap_TypeX_V141[] = {6, 5, 3, 2, 1, 7, 4, 0};
const uint32_t EmitXComRemap_TypeX_V141[] = {12, 4, 8, 0, 14, 3, 10, 2, 13, 5, 9, 1, 15, 15, 15, 15};
const uint32_t EmitYSegRemap_TypeX_V141[] = {6, 5, 3, 2, 1, 7, 4, 0};
const uint32_t EmitYComRemap_TypeX_V141[] = {12, 4, 8, 0, 14, 3, 10, 2, 13, 5, 9, 1, 15, 15, 15, 15};
const uint32_t RevXSegRemap_TypeX_PTV141[] = {0, 1, 2, 3, 4, 5, 6, 7};
const uint32_t RevXComRemap_TypeX_PTV141[] = {7, 3, 1, 5, 4, 0, 6, 2, 15, 11, 9, 13, 14, 12, 10, 8};
const uint32_t RevYSegRemap_TypeX_PTV141[] = {0, 1, 2, 3, 4, 5, 6, 7};
const uint32_t RevYComRemap_TypeX_PTV141[] = {7, 3, 1, 5, 4, 0, 6, 2, 15, 11, 9, 13, 14, 12, 10, 8};

#define NULL_LED 127
#define BOARD_SEL_LOGIC(a)		(~(a))

uint16_t ConvertData(uint16_t Data)
{
	int i;
	uint16_t ret = 0;

	for(i=0; i<8; i+=1)
	{
		if((Data&(1<<i+8)) != 0)
		{
			ret |= (1<<2*i+1);
		}

		if((Data&(1<<i)) != 0)
		{
			ret |= (1<<2*i);
		}
	}

	return ret;
}

bool CheckBoardStartLed(AXI_PARAM *ap, int Leds)
{
	int TotalLeds = ap->TotalLeds * (ap->ScanGap + 1);
	
	if(Leds<0 || Leds>= TotalLeds)
	{
		return false;
	}

	Leds += ap->JumpLeds;

	for(int i=0; i<ap->Boards; i+=1)
	{
		if(Leds < ap->BoradLeds[i])
		{
			if((i==0 && Leds == ap->JumpLeds) || Leds == 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			Leds -= ap->BoradLeds[i];
		}
	}

	return false;
}

bool CheckBoardStartLed(AXI_PARAM *ap, int board, int Leds, bool reverse)
{
	if(board<0 || board >= ap->Boards || Leds<0 || Leds>= ap->BoradLeds[board])
	{
		return false;
	}

	if( !reverse) {
		if(Leds == 0 || (board == 0 && Leds == ap->JumpLeds)) {
			return true;
		}

		if(ap->ScanGap >= Leds){
			return true;
		}
	} else {
		if(Leds == ap->BoradLeds[board]-1){
			return true;
		}

		if(Leds >= ap->BoradLeds[board]- 1 - ap->ScanGap) {
			return true;
		}
	}

	return false;
}


bool CheckBoardEndLed(AXI_PARAM *ap, int Leds)
{
	int TotalLeds = ap->TotalLeds * (ap->ScanGap + 1);

	if(Leds<0 || Leds>= TotalLeds)
	{
		return false;
	}

	Leds += ap->JumpLeds;

	for(int i=0; i<ap->Boards; i+=1)
	{
		if(Leds < ap->BoradLeds[i])
		{
			if(Leds == ap->BoradLeds[i]-1)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			Leds -= ap->BoradLeds[i];
		}
	}

	return false;
}

int FindBoardLed(AXI_PARAM *ap, int Leds, short *Led, short *Board)
{
	int i;
	int TotalLeds = ap->TotalLeds * (ap->ScanGap + 1);

	if(Leds<0 || Leds>= TotalLeds)
	{
		*Led = NULL_LED;
		*Board = YParam.Boards;

		return 1;
	}

	Leds += ap->JumpLeds;

	for(i=0; i<ap->Boards; i+=1)
	{
		if(Leds < ap->BoradLeds[i])
		{
			*Led = Leds;
			*Board = i;
			return 0;
		}
		else
		{
			Leds -= ap->BoradLeds[i];
		}
	}

	return 1;
}

//--------------------------------- V4.6 69 Leds -------------------------------
uint32_t Get_Emit_Data_V46(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint32_t Data;
	uint16_t com, seg;

	Addr &= 0x0F;
	Led &= 0x7F;
	Pow = PwRemap_V46[Pow];

	Data = Addr << 12 | Pow << 7 | Led;

	return Data;
}

uint32_t Get_Rev_Data_V46(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg;

	Addr &= 0x0F;

	seg = Led%8;
	com = Led/8;

	if(com == 5)
	{
		seg = RevSeg5Remap_V46[seg];
	}
	else
	{
		seg = RevSegRemap_V46[seg];
	}

	com = RevComRemap_V46[com];
	Gain = ( Gain1Remap_V46[Gain&0x07] | Gain2Remap_V46[(Gain>>3)&0x03] ) & 0x1F;

	Data = Addr << 12 | Gain << 7 | com << 3 | seg;

	return Data;
}

uint32_t Get_Emit_Data_V46_MagicConfig(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	Addr &= 0x1F;
	Led &= 0x7F;
	//Pow = PwRemap_V46[Pow];

	Data = Addr << 7 | Led;

	return Data;
}

uint32_t Get_Rev_Data_V46_MagicConfig(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg;

	Addr &= 0x1F;

	seg = Led%8;
	com = Led/8;

	if(com == 5)
	{
		seg = RevSeg5Remap_V46[seg];
	}
	else
	{
		seg = RevSegRemap_V46[seg];
	}

	com = RevComRemap_V46[com];
	//Gain = ( Gain1Remap_V46[Gain&0x07] | Gain2Remap_V46[(Gain>>3)&0x03] ) & 0x1F;

	Data = Addr << 7 | com << 3 | seg;

	return Data;
}

//----------------------------- V5.4 96 Leds -----------------------------------
uint32_t Get_Emit_Data_V54(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	Addr &= 0x0F;
	seg = EmitSegRemap_V54[Led%8];
	com = EmitComRemap_V54[Led/8];
	Pow = PwRemap_V54[Pow];

	Data = seg<<13 | Pow<<10 | Addr<<4 | com;

	return Data;
}

uint32_t Get_Rev_Data_PDV54(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg;

	Addr &= 0x0F;
	seg = RevSegRemap_PDV54[Led%8];
	com = RevComRemap_PDV54[Led/8];
	Gain = ( Gain1Remap_PDV54[Gain&0x07] | Gain2Remap_PDV54[(Gain>>3)&0x03] ) & 0x1F;

	Data = Addr << 12 | Gain << 7 | com << 3 | seg;

	return Data;
}

uint32_t Get_Rev_Data_PTV54(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg;

	Addr &= 0x0F;
	seg = RevSegRemap_PTV54[Led%8];
	com = RevComRemap_PTV54[Led/8];
	Gain = ( Gain1Remap_PTV54[Gain&0x07] | Gain2Remap_PTV54[(Gain>>3)&0x03] ) & 0x1F;

	Data = Addr << 12 | Gain << 7 | com << 3 | seg;
	return Data;
}

//---------------- V54 magic config -------------------------------------------
uint32_t Get_Emit_Data_V54_MagicConfig(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	Addr &= 0x1F;
	seg = EmitSegRemap_V54[Led%8];
	com = EmitComRemap_V54[Led/8];
	//Pow = PwRemap_V54[Pow];

	Data = Addr<<7 | com<<3 | seg;

	return Data;
}

uint32_t Get_Rev_Data_PDV54_MagicConfig(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg;

	Addr &= 0x1F;
	seg = RevSegRemap_PDV54[Led%8];
	com = RevComRemap_PDV54[Led/8];
	//Gain = ( Gain1Remap_PDV54[Gain&0x07] | Gain2Remap_PDV54[(Gain>>3)&0x03] ) & 0x1F;

	Data = Addr << 7 | com << 3 | seg;

	return Data;
}

//---------------- V54 magic config support 64boards-------------------------------------------
uint32_t Get_Emit_Data_V54_Support_64_Boards(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	Addr &= 0x3F;
	seg = EmitSegRemap_V54[Led%8];
	com = EmitComRemap_V54[Led/8];
	//Pow = PwRemap_V54[Pow];

	Data = Addr<<7 | com<<3 | seg;

	return Data;
}

uint32_t Get_Rev_Data_PDV54_Support_64_Boards(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg;

	Addr &= 0x3F;
	seg = RevSegRemap_PDV54[Led%8];
	com = RevComRemap_PDV54[Led/8];
	//Gain = ( Gain1Remap_PDV54[Gain&0x07] | Gain2Remap_PDV54[(Gain>>3)&0x03] ) & 0x1F;

	Data = Addr << 7 | com << 3 | seg;

	return Data;
}


//----------------------------- V5.7 96 Leds -----------------------------------
//magic config
uint32_t Get_Emit_Data_V57_MagicConfig(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	Addr &= 0x1F;
	seg = EmitSegRemap_V57[Led%8];
	com = EmitComRemap_V57[Led/8];
	//Pow = PwRemap_V54[Pow];

	Data = Addr<<7 | com<<3 | seg;

	return Data;
}

uint32_t Get_Rev_Data_PDV57_MagicConfig(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg;

	Addr &= 0x1F;
	seg = RevSegRemap_PDV57[Led%8];
	com = RevComRemap_PDV57[Led/8];
	//Gain = ( Gain1Remap_PDV54[Gain&0x07] | Gain2Remap_PDV54[(Gain>>3)&0x03] ) & 0x1F;

	Data = Addr << 7 | com << 3 | seg;

	return Data;
}

//----------------------------- V5.6 64 Leds 8mm-----------------------------------
uint32_t Get_Emit_Data_V56(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	if(Led > 63)	return 0;
	
	Addr &= 0x1F;
	seg = EmitSegRemap_V56[Led%8];
	com = EmitComRemap_V56[Led/8];
	//Pow = PwRemap_V56[Pow];

	Data = Addr<<7 | com<<4 | seg;

	return Data;
}

uint32_t Get_Rev_Data_PDV56(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg;

	if(Led > 63)	return 0;
	
	Addr &= 0x1F;
	seg = RevSegRemap_PDV56[Led%8];
	com = RevComRemap_PDV56[Led/8];

	Data = Addr << 7 | com << 3 | seg;

	return Data;
}

//----------------------------- V6.5 72 Leds -----------------------------------
uint32_t Get_Emit_Data_V65(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	Addr &= 0x0F;
	Pow = PwRemap_V65[Pow];

	Data = Addr<<12 | Pow<<7 | (Led & 0x7F);

	return Data;
}

uint32_t Get_Rev_Data_V65(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg;

	Addr &= 0x0F;
	seg = RevSegRemap_V65[Led%8];
	com = RevComRemap_V65[Led/8];
	Gain = ( Gain1Remap_V65[Gain&0x07] | Gain2Remap_V65[(Gain>>3)&0x03] ) & 0x1F;

	Data = Addr << 12 | Gain << 7 | com << 3 | seg;
	return Data;
}

//magic config
uint32_t Get_Emit_Data_V65_MagicConfig(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	Addr &= 0x0F;
	//Pow = PwRemap_V65[Pow];

	Data = Addr<<7 | (Led & 0x7F);

	return Data;
}

//----------------------------- V70 72 Leds 22mm PCB ---------------------------
uint32_t Get_Emit_Data_V70(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	Addr &= 0x0F;
	Pow = PwRemap_V70[Pow];

	Data = Addr<<12 | Pow<<7 | (Led & 0x7F);

	return Data;
}

uint32_t Get_Rev_Data_V70(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg;

	Addr &= 0x0F;
	seg = RevSegRemap_V70[Led%8];
	com = RevComRemap_V70[Led/8];
	Gain = ( Gain1Remap_V70[Gain&0x07] | Gain2Remap_V70[(Gain>>3)&0x03] ) & 0x1F;

	Data = Addr << 12 | Gain << 7 | com << 3 | seg;
	return Data;
}

//magic config
uint32_t Get_Emit_Data_V70_MagicConfig(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	Addr &= 0x0F;
	//Pow = PwRemap_V70[Pow];

	Data = Addr<<7 | (Led & 0x7F);

	return Data;
}
//--------------------------- TypeX V11 21.5" ----------------------------------
uint32_t Get_EmitX_Data_TypeX_V11(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = EmitXSegRemap_TypeX_V11[Led & 0x07];
	com = EmitXComRemap_TypeX_V11[Led / 8];

	Data = com<<3 | seg;

	return Data;
}

uint32_t Get_EmitY_Data_TypeX_V11(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = EmitYSegRemap_TypeX_V11[Led & 0x07];
	com = EmitYComRemap_TypeX_V11[Led / 8];

	Data = com<<3 | seg;

	return Data;
}

uint32_t Get_RevX_Data_TypeX_PTV11(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = RevXSegRemap_TypeX_PTV11[Led & 0x07];
	com = RevXComRemap_TypeX_PTV11[Led / 8];

	Data = com<<3 | seg;

	return Data;
}

uint32_t Get_RevY_Data_TypeX_PTV11(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = RevYSegRemap_TypeX_PTV11[Led & 0x07];
	com = RevYComRemap_TypeX_PTV11[Led / 8];

	Data = com<<3 | seg;

	return Data;
}

//------------------------------ TypeX V80 26" ---------------------------------
uint32_t Get_EmitX_Data_TypeX_V80(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = EmitXSegRemap_TypeX_V80[Led & 0x07];
	com = EmitXComRemap_TypeX_V80[Led / 8];

	Data = com | seg<<4;

	return Data;
}

uint32_t Get_EmitY_Data_TypeX_V80(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = EmitYSegRemap_TypeX_V80[Led & 0x07];
	com = EmitYComRemap_TypeX_V80[Led / 8];

	Data = com | seg<<4;

	return Data;
}

uint32_t Get_RevX_Data_TypeX_PTV80(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg, oddeven;

	oddeven = (Led + 1) & 0x01;
	seg = RevXSegRemap_TypeX_PTV80[(Led>>1) & 0x07];
	com = RevXComRemap_TypeX_PTV80[Led / 16];

	Data = com<<5 | 0<<4 | seg << 1 | oddeven;

	return Data;
}

uint32_t Get_RevY_Data_TypeX_PTV80(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg, oddeven;

	oddeven = (Led + 1) & 0x01;
	seg = RevYSegRemap_TypeX_PTV80[(Led>>1) & 0x07];
	com = RevYComRemap_TypeX_PTV80[Led / 16];

	Data = com<<5 | 1<<4 | seg<<1 | oddeven;

	return Data;
}

//------------------------------ TypeX PD 21.3" R1---------------------------------
uint32_t Get_EmitX_Data_TypeX_JYT6N_VR1(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = EmitXSegRemap_TypeX_JYT6N_VR1[Led & 0x07];
	com = EmitXComRemap_TypeX_JYT6N_VR1[Led / 8];

	Data = 1<<7 | com<<3 | seg;

	return Data;
}

uint32_t Get_EmitY_Data_TypeX_JYT6N_VR1(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = EmitYSegRemap_TypeX_JYT6N_VR1[Led & 0x07];
	com = EmitYComRemap_TypeX_JYT6N_VR1[Led / 8];

	Data = com<<3 | seg;

	return Data;
}

uint32_t Get_RevX_Data_TypeX_JYT6N_PDVR1(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = RevXSegRemap_TypeX_JYT6N_PDVR1[Led & 0x07];
	com = RevXComRemap_TypeX_JYT6N_PDVR1[Led / 8];

	Data = com<<3 | seg;

	return Data;
}

uint32_t Get_RevY_Data_TypeX_JYT6N_PDVR1(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = RevYSegRemap_TypeX_JYT6N_PDVR1[Led & 0x07];
	com = RevYComRemap_TypeX_JYT6N_PDVR1[Led / 8];

	Data = com<<3 | seg;

	return Data;
}

//------------------------------ TypeX PD 21.3" R2---------------------------------
uint32_t Get_EmitX_Data_TypeX_JYT6N_VR2(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = EmitXSegRemap_TypeX_JYT6N_VR2[Led & 0x07];
	com = EmitXComRemap_TypeX_JYT6N_VR2[Led / 8];

	Data = 1<<7 | com<<3 | seg;

	return Data;
}

uint32_t Get_EmitY_Data_TypeX_JYT6N_VR2(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	if(Led > 63) Led = 63;
	
	seg = EmitYSegRemap_TypeX_JYT6N_VR2[Led & 0x07];
	com = EmitYComRemap_TypeX_JYT6N_VR2[Led / 8];

	Data = 0<<7 | com<<3 | seg;

	return Data;
}

uint32_t Get_RevX_Data_TypeX_JYT6N_PDVR2(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = RevXSegRemap_TypeX_JYT6N_PDVR2[Led & 0x07];
	com = RevXComRemap_TypeX_JYT6N_PDVR2[Led / 8];

	Data = 0<<7 | com<<3 | seg;

	return Data;
}

uint32_t Get_RevY_Data_TypeX_JYT6N_PDVR2(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = RevYSegRemap_TypeX_JYT6N_PDVR2[Led & 0x07];
	com = RevYComRemap_TypeX_JYT6N_PDVR2[Led / 8];

	Data = 1<<7 | com<<3 | seg;

	return Data;
}

//------------------------------ TypeL PT V90 ---------------------------------
uint32_t Get_Emit_Data_V90(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg, Add;

	seg = EmitSegRemap_V90[Led & 0x07];
	com = EmitComRemap_V90[Led / 8];

	Add = ~EmitAddRemap_V90[(Addr-1) % 7];
	Add &= 0xFF00;

	Data = Add | com<<4 | seg <<1;

	return Data;
}

uint32_t Get_Rev_Data_V90(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg, Add, Vr, gain;

	Vr = Led & 0x03;
	Vr = VrRemap_V90[Vr];
	Vr = (~Vr) & (0x0F << 8);

	Led /= 4;

	seg = 0;
	if(Led < 3*8)
	{
		seg = RevSegRemap_V90[Led];
	}

	com = RevComRemap_V90[Led / 8];
	Add = RevAddRemap_V90[(Addr-1) % 7];

	gain = HC4051Remap_V90[Gain&0x07];
	gain = (gain &0x01) << 12 | (gain & 0x06) << 2;	//D4 D3 D12

	Data = com<<14| Vr | seg<<5 | gain | Add;

	return Data;

	//return 0xF0F0;
}

//------------------------------ TypeX PD 21.5" ---------------------------------
uint32_t Get_EmitX_Data_TypeX_VR2(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = EmitXSegRemap_TypeX_VR2[Led & 0x07];
	com = EmitXComRemap_TypeX_VR2[Led / 8];

	Data = seg<<5 | 0<<4 | com;

	return Data;
}

uint32_t Get_EmitY_Data_TypeX_VR2(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = EmitYSegRemap_TypeX_VR2[Led & 0x07];
	com = EmitYComRemap_TypeX_VR2[Led / 8];

	Data = seg<<5 | 1<<4 | com<<1;

	return Data;
}

uint32_t Get_RevX_Data_TypeX_PDVR2(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = RevXSegRemap_TypeX_PDVR2[Led & 0x07];
	com = RevXComRemap_TypeX_PDVR2[Led / 8];

	Data = com<<4 | seg<<1 | 0<<0;

	return Data;
}

uint32_t Get_RevY_Data_TypeX_PDVR2(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = RevYSegRemap_TypeX_PDVR2[Led & 0x07];
	com = RevYComRemap_TypeX_PDVR2[Led / 8];

	Data = com<<4 | seg<<1 | 1<<0;

	return Data;
}

//--------------------------------- V10.0 Leds -------------------------------
uint32_t Get_Emit_Data_PDV100(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	if(Addr == 1)
	{
		seg = EmitMainSegRemap_V100[Led & 0x07];
		com = EmitMainComRemap_V100[Led / 8];
	}
	else
	{
		seg = EmitSegRemap_V100[Led & 0x07];
		com = EmitComRemap_V100[Led / 8];
	}

	Data = 1<<7 | com<<3 | seg<<0;

	return Data;
}

uint32_t Get_Emit_Data_PTV100(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;
	uint16_t Add;

	if(Addr == 1)
	{
		seg = EmitMainSegRemap_V100[Led & 0x07];
		com = EmitMainComRemap_V100[Led / 8];
	}
	else
	{
		seg = EmitSegRemap_V100[Led & 0x07];
		com = EmitComRemap_V100[Led / 8];
	}

	Add = BOARD_SEL_LOGIC(1<<(Addr-1)) & 0xFF;

	Data = 1<<15 | com<<11 | seg<<8 | Add;

	return Data;
	//return 0xD1FB;
}

uint32_t Get_Rev_Data_PTV100(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg, Add, Vr, gain;

	Vr = Led & 0x03;
	Vr = RevVrRemap_PTV100[Vr];

	Led /= 4;

	seg = 0;
	if(Led < 3*8)
	{
		seg = RevSegRemap_PTV100[Led];
	}

	com = ~RevComRemap_PTV100[Led / 8];
	com = (com>>5) & 0x07;
	Add = BOARD_SEL_LOGIC(1<<(Addr-1));

	Data = com<<13 | seg << 10 | Vr<<8 | (Add&0x0FF);

	return Data;
	//return 0xD1FB;
}

//--------------------------------- V13.0 Leds -------------------------------
uint32_t Get_Emit_Data_PDV130(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	if(Led > 71)	return 0;

	if(Addr == 1)
	{
		seg = EmitMainSegRemap_V130[Led & 0x07];
		com = EmitMainComRemap_V130[Led / 8];
	}
	else
	{
		seg = EmitSegRemap_V130[Led & 0x07];
		com = EmitComRemap_V130[Led / 8];
	}

	Data = 1<<7 | seg<<4 | 0<<3 | com<<0;

	return Data;
}

uint32_t Get_Emit_Data_PTV130(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;
	uint16_t Add;
	
	if(Led > 71)	return 0xFFFF;
	
	if(Addr == 1)
	{
		seg = EmitMainSegRemap_V130[Led & 0x07];
		com = EmitMainComRemap_V130[Led / 8];
	}
	else
	{
		seg = EmitSegRemap_V130[Led & 0x07];
		com = EmitComRemap_V130[Led / 8];
	}

	Add = BOARD_SEL_LOGIC(1<<(Addr-1)) & 0xFF;

	Data = 1<<15 | seg<<12 | com<<8 | Add;

	return Data;
	//return Led;
}

uint32_t Get_Rev_Data_PTV130(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg, Add, Vr, gain;

	if(Led > 71)	return 0xFFFF;
	
	Vr = Led & 0x07;
	Vr = RevVrRemap_PTV130[Vr];

	Led /= 8;
	seg = RevSegRemap_PTV130[Led & 0x07];
	com = RevComRemap_PTV130[Led / 8];

	Add = BOARD_SEL_LOGIC(1<<(Addr-1));

	Data = com<<14 | seg << 11 | Vr<<8 | (Add&0x0FF);

	return Data;
	//return 0x60FE;
}

//--------------------------------- V13.3 9mm D2 64 Leds -------------------------------
//--------------------------------- PT-V15.0 Leds -------------------------------
const uint16_t EmitSegRemap_V133[8] = {0, 1, 2, 3, 4, 5, 6, 7};
const uint16_t EmitMainComRemap_V133[8] = {0, 4, 2, 6, 1, 5, 3, 7};
const uint16_t EmitEmitComRemap_V133[8] = {7, 3, 5, 1, 6, 2, 4, 0};

uint32_t Get_Emit_Data_PDV133(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	static uint32_t Data[4] = {0, 0, 0, 0};
	uint32_t com, seg;
	uint16_t Add;

	if(Led > 64)	Led = 63;

	seg = EmitSegRemap_V133[Led & 0x07];

	if(Addr == 1) {
		com = EmitMainComRemap_V133[(Led >>3) & 0x07];
	} else {
		com = EmitEmitComRemap_V133[(Led >>3) & 0x07];
	}

	seg <<= 3; //PB3 ~ PB5
	seg = (~seg & (0x07<<3)) << 16 | seg;

	com <<= 7;	//PC7 ~ PC9
	com = (~com & (0x07<<7)) << 16 | com;

	Data[1] = seg;
	Data[2] = com;
	
	return (uint32_t)Data;
	//return Led;
}

const uint16_t EmitSegRemap_V1331[8] = {0, 1, 2, 3, 4, 5, 6, 7};
const uint16_t EmitMainComRemap_V1331[8] = {0, 4, 2, 6, 1, 5, 3, 7};
const uint16_t EmitEmitComRemap_V1331[8] = {7, 3, 5, 1, 6, 2, 4, 0};

uint32_t Get_Emit_Data_PDV1331(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	static uint32_t Data[4] = {0, 0, 0, 0};           //gpioa gpiob gpioc gpiod
	uint32_t com, seg;
	uint16_t Add;

	if(Led > 64)	Led = 63;

	seg = EmitSegRemap_V1331[Led & 0x07];

	if(Addr == 1) {
		com = EmitMainComRemap_V1331[(Led >>3) & 0x07];
	} else {
		com = EmitEmitComRemap_V1331[(Led >>3) & 0x07];
	}

	seg <<= 4; //PC4 ~ PC6
	seg = (~seg & (0x07<<4)) << 16 | seg;

	com <<= 7;	//PC7 ~ PC9
	com = (~com & (0x07<<7)) << 16 | com;

	Data[2] = com | seg;
	
	return (uint32_t)Data;
	//return Led;
}

uint32_t Get_Emit_Data_PDV1333(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	static uint32_t Data[4] = {0, 0, 0, 0};           //gpioa gpiob gpioc gpiod
	uint32_t com, seg;
	uint16_t Add;

	if(Led > 64)	{
		Led = 63;
		Addr = 0xFF;
	}

	seg = EmitSegRemap_V1331[Led & 0x07];

	if(Addr == 1) {
		com = EmitMainComRemap_V1331[(Led >>3) & 0x07];
	} else {
		com = EmitEmitComRemap_V1331[(Led >>3) & 0x07];
	}

	seg <<= 4; //PC4 ~ PC6
	seg = (~seg & (0x07<<4)) << 16 | seg;

	com <<= 7;	//PC7 ~ PC9
	com = (~com & (0x07<<7)) << 16 | com;

	Data[2] = com | seg;

	if(Addr == 1) {
		Data[1] = (1<<13+16) | (1<<15);
	} else if(Addr == 2) {
		Data[1] = (1<<13) | (1<<15+16);
	} else {
		Data[1] = (1<<13+16) | (1<<15+16);
	}

	return (uint32_t)Data;
	//return Led;
}


//sc 7.2mm
const uint16_t EmitSegRemap_V1334[8] = {0, 1, 2, 3, 4, 5, 6, 7};
const uint16_t EmitMainComRemap_V1334[16] = {0<<7|1<<0, 4<<7|1<<0, 2<<7|1<<0, 6<<7|1<<0, 1<<7|1<<0, 5<<7|1<<0, 3<<7|1<<0, 7<<7|1<<0,
											 0<<7, 4<<7, 2<<7, 6<<7, 1<<7, 5<<7, 3<<7, 7<<7};
const uint16_t EmitEmitComRemap_V1334[16] = {7<<7|1<<0, 3<<7|1<<0, 5<<7|1<<0, 1<<7|1<<0, 6<<7|1<<0, 2<<7|1<<0, 4<<7|1<<0, 0<<7|1<<0,
											 0<<7, 4<<7, 2<<7, 6<<7, 1<<7, 5<<7, 3<<7, 7<<7};

uint32_t Get_Emit_Data_PDV1334(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	static uint32_t Data[4] = {0, 0, 0, 0};           //gpioa gpiob gpioc gpiod
	uint32_t com, seg;
	uint16_t Add;

	if(Led > 80)	Led = 80;

	seg = EmitSegRemap_V1334[Led & 0x07];

	if(Addr == 1) {
		com = EmitMainComRemap_V1334[(Led >>3) & 0x0F];
	} else {
		com = EmitEmitComRemap_V1334[(Led >>3) & 0x0F];
	}

	seg <<= 4; //PC4 ~ PC6
	seg = (~seg & (0x07<<4)) << 16 | seg;

	com = (~com & (0x07<<7 | 1<<0)) << 16 | com;

	Data[2] = com | seg;

	return (uint32_t)Data;
	//return Led;
}

uint32_t Get_Emit_Data_PDV1335(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	return  Get_Emit_Data_PDV1334(Addr, Led, Pow);
}

//--------------------------------- PT-V15.0 Leds -------------------------------
const uint16_t EmitSegRemap_V150[8] = {
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
};

#define EMIT_HC138_ABC_PIN(a)	(((a) & 0x03)<<0 | (((a) >> 2)&0x01)<<10)	//PB0 PB1 PB10
#define HC138_ABC(cba)    		(EMIT_HC138_ABC_PIN(~cba)<<16) | EMIT_HC138_ABC_PIN(cba)

uint32_t Get_Emit_Data_PTV150(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	static uint16_t Data[5];
	uint16_t com, seg;
	uint16_t LedGroup;

	memset(Data, 0xFFFF , sizeof(Data));

	com = Led / 8;
	seg = Led & 0x07;

	LedGroup = Addr + com;

	if(Led < 64 && LedGroup < 8*6){      //
		Data[2-LedGroup/16] = ~(1 << LedGroup % 16);
	}

	Data[3] = EMIT_HC138_ABC_PIN(seg);
	Data[4] = EMIT_HC138_ABC_PIN((~seg)&0x07);

	return (uint32_t)Data;
}

#define REV_SCK_PIN_HIGH		(1<<3)	//PB3
#define REV_SCK_PIN_LOW     	(1<<3+16)
#define REV_HC4051_ABC_PIN(a)	(((a) & 0x07)<<5)	//PA5 PA6 PA7
#define HC4051_ABC(cba)    		(REV_HC4051_ABC_PIN(~cba)<<16) | REV_HC4051_ABC_PIN(cba)

uint32_t Get_Rev_Data_PTV150(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint32_t Data[2];
	uint16_t seg, Add;	//com, 

	memset(Data, 0, sizeof(Data));

	//com = Led/8;
	seg = Led & 0x07;

	if(seg == 0){	//switch to next power group
		Data[0] =  REV_SCK_PIN_HIGH | HC4051_ABC(seg);
		Data[1] =  REV_SCK_PIN_LOW | HC4051_ABC(seg);
	} else {
		Data[0] =  REV_SCK_PIN_LOW | HC4051_ABC(seg);
		Data[1] =  REV_SCK_PIN_LOW | HC4051_ABC(seg);
	}

	return (uint32_t)Data;
}

//--------------------------------- V6.7 Leds -------------------------------
uint32_t Get_Emit_Data_PDV67(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = EmitSegRemap_V67[Led & 0x07];
	com = EmitComRemap_V67[Led / 8];

	Data = 1<<7 | seg<<4 | com << 0;

	return Data;
}

uint32_t Get_Emit_Data_PTV67(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;
	uint16_t Add;

	seg = EmitSegRemap_V67[Led & 0x07];
	com = EmitComRemap_V67[Led / 8];

	Add = BOARD_SEL_LOGIC(1<<(Addr-1)) & 0xFF;

	Data = 1<<15 | com<<11 | seg<<8 | Add;

	return Data;
	//return 0xD1FB;
}

uint32_t Get_Rev_Data_PTV67(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg, Add, Vr, gain;

	Vr = Led & 0x03;
	Vr = RevVrRemap_PTV67[Vr];
	
	Led /= 4;

	seg = 0;
	if(Led < 3*8)
	{
		seg = RevSegRemap_PTV67[Led];
	}

	com = ~RevComRemap_PTV67[Led / 8];
	com = (com>>5) & 0x07;
	Add = BOARD_SEL_LOGIC(1<<(Addr-1));

	Data = com<<13 | seg << 10 | Vr<<8 | (Add&0x0FF);

	return Data;
	//return 0xD1FB;
}

//--------------------------------- V6.7 Leds -------------------------------
uint32_t Get_Emit_Data_PDV68(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = EmitSegRemap_V68[Led & 0x07];
	com = EmitComRemap_V68[Led / 8];

//	if(Addr == 1)
//		Data = 1<<7 | com << 3 | (seg & 0x07) << 0;
//	else
		Data = 1<<7 | seg<<4 | com << 0;

	return Data;
}

uint32_t Get_Emit_Data_PTV68(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;
	uint16_t Add;

	seg = EmitSegRemap_V68[Led & 0x07];
	com = EmitComRemap_V68[Led / 8];

	Add = BOARD_SEL_LOGIC(1<<(Addr-1)) & 0xFF;

	Data = 1<<15 | com<<11 | seg<<8 | Add;

	return Data;
	//return 0xD1FB;
}

uint32_t Get_Rev_Data_PTV68(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg, Add, Vr, gain;

	Vr = Led & 0x03;
	Vr = RevVrRemap_PTV68[Vr];
	
	Led /= 4;

	seg = 0;
	if(Led < 3*8)
	{
		seg = RevSegRemap_PTV68[Led];
	}

	com = ~RevComRemap_PTV68[Led / 8];
	com = (com>>5) & 0x07;
	Add = BOARD_SEL_LOGIC(1<<(Addr-1));

	Data = com<<13 | seg << 10 | Vr<<8 | (Add&0x0FF);

	return Data;
	//return 0xD1FB;
}

//--------------------------------- V6.9 Leds -------------------------------
uint32_t Get_Emit_Data_PDV69(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = EmitSegRemap_V69[Led & 0x07];
	com = EmitComRemap_V69[Led / 8];

//	if(Addr == 1)
//		Data = 1<<7 | com << 3 | (seg & 0x07) << 0;
//	else
		Data = 1<<7 | seg<<4 | com << 0;

	return Data;
}

uint32_t Get_Emit_Data_PDV693(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = EmitSegRemap_V693[Led & 0x07];
	com = EmitComRemap_V693[Led / 8];

	Data = 1<<7 | seg<<4 | com << 0;

	return Data;
}

uint32_t Get_Emit_Data_PDV69_V68(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	if(Addr == 1)
		return  Get_Emit_Data_PDV69(Addr, Led, Pow);
	else
		return  Get_Emit_Data_PDV68(Addr, Led, Pow);
}

uint32_t Get_Emit_Data_PDV693_V68(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	if(Addr == 1)
		return  Get_Emit_Data_PDV693(Addr, Led, Pow);
	else
		return  Get_Emit_Data_PDV68(Addr, Led, Pow);
}

uint32_t Get_Emit_Data_PDV693_V69(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	if(Addr == 1)
		return  Get_Emit_Data_PDV693(Addr, Led, Pow);
	else
		return  Get_Emit_Data_PDV69(Addr, Led, Pow);
}

uint32_t Get_Emit_Data_PTV69(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;
	uint16_t Add;

	seg = EmitSegRemap_V69[Led & 0x07];
	com = EmitComRemap_V69[Led / 8];

	Add = BOARD_SEL_LOGIC(1<<(Addr-1)) & 0xFF;

	Data = 1<<15 | com<<11 | seg<<8 | Add;

	return Data;
	//return 0xD1FB;
}

uint32_t Get_Rev_Data_PTV69(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg, Add, Vr, gain;

	Vr = Led & 0x03;
	Vr = RevVrRemap_PTV69[Vr];

	Led /= 4;

	seg = 0;
	if(Led < 3*8)
	{
		seg = RevSegRemap_PTV69[Led];
	}

	com = ~RevComRemap_PTV69[Led / 8];
	com = (com>>5) & 0x07;
	Add = BOARD_SEL_LOGIC(1<<(Addr-1));

	Data = com<<13 | seg << 10 | Vr<<8 | (Add&0x0FF);

	return Data;
	//return 0xD1FB;
}

//------------------------------ TypeX V52 21.5" ~ 27" ---------------------------------
uint32_t Get_EmitX_Data_TypeX_V52(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = EmitXSegRemap_TypeX_V52[Led & 0x07];
	com = EmitXComRemap_TypeX_V52[Led / 8];

	Data = com<<4 | seg<<1 | 1<<0;

	return Data;
}

uint32_t Get_EmitY_Data_TypeX_V52(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = EmitYSegRemap_TypeX_V52[Led & 0x07];
	com = EmitYComRemap_TypeX_V52[Led / 8];

	Data = com<<4 | seg<<1 | 0<<0;

	return Data;
}

uint32_t Get_RevX_Data_TypeX_PDV52(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	return 0;
}

uint32_t Get_RevY_Data_TypeX_PDV52(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	return 0;
}

//------------------------------ TypeX V52 21.5" ~ 27" ---------------------------------
uint32_t Get_EmitX_Data_TypeX_V120(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = EmitXSegRemap_TypeX_V120[Led & 0x07];
	com = EmitXComRemap_TypeX_V120[Led / 8];

	Data = 0<<7 | (com&0x07)<<4 | seg<<1 | (com &0x08)>>3;

	return Data;
}

uint32_t Get_EmitY_Data_TypeX_V120(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = EmitYSegRemap_TypeX_V120[Led & 0x07];
	com = EmitYComRemap_TypeX_V120[Led / 8];

	Data = 1<<7 | (com&0x07)<<4 | seg<<1 | (com &0x08)>>3;

	return Data;
}

uint32_t Get_RevX_Data_TypeX_PDV120(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = RevXSegRemap_TypeX_PDV120[Led & 0x07];
	com = RevXComRemap_TypeX_PDV120[Led / 8];

	Data = 1<<7 | com<<3 | seg<<0;

	return Data;
}

uint32_t Get_RevY_Data_TypeX_PDV120(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = RevYSegRemap_TypeX_PDV120[Led & 0x07];
	com = RevYComRemap_TypeX_PDV120[Led / 8];

	Data = 0<<7 | com<<3 | seg<<0;

	return Data;
}

//------------------------------ TypeX V52 21.5" ~ 15" ---------------------------------
uint32_t Get_EmitX_Data_TypeX_V121(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = EmitXSegRemap_TypeX_V121[Led & 0x07];
	com = EmitXComRemap_TypeX_V121[Led / 8];

	Data = 0<<7 | (com&0x07)<<4 | seg<<1 | (com &0x08)>>3;

	return Data;
}

uint32_t Get_EmitY_Data_TypeX_V121(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = EmitYSegRemap_TypeX_V121[Led & 0x07];
	com = EmitYComRemap_TypeX_V121[Led / 8];

	Data = 1<<7 | (com&0x07)<<4 | seg<<1 | (com &0x08)>>3;

	return Data;
}

uint32_t Get_RevX_Data_TypeX_PDV121(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = RevXSegRemap_TypeX_PDV121[Led & 0x07];
	com = RevXComRemap_TypeX_PDV121[Led / 8];

	Data = 1<<7 | com<<3 | seg<<0;

	return Data;
}

uint32_t Get_RevY_Data_TypeX_PDV121(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg;

	seg = RevYSegRemap_TypeX_PDV121[Led & 0x07];
	com = RevYComRemap_TypeX_PDV121[Led / 8];

	Data = 0<<7 | com<<3 | seg<<0;

	return Data;
}


//------------------------------ TypeX V512.2 15" ~ 19" ---------------------------------
uint32_t Get_EmitX_Data_TypeX_V122(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	if(Led > 127)	Led = 127;

	seg = EmitXSegRemap_TypeX_V122[Led & 0x07];
	com = EmitXComRemap_TypeX_V122[Led / 8];

	Data = 0<<7 | (com&0x07)<<4 | seg<<1 | (com &0x08)>>3;

	return Data;
}

uint32_t Get_EmitY_Data_TypeX_V122(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint16_t Data;
	uint16_t com, seg;

	if(Led > 63)	Led = 63;

	seg = EmitYSegRemap_TypeX_V122[Led & 0x07];
	com = EmitYComRemap_TypeX_V122[Led / 8];

	Data = 1<<7 | (com&0x07)<<4 | seg<<1 | (com &0x08)>>3;

	return Data;
}

uint32_t Get_RevX_Data_TypeX_PDV122(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg;

	if(Led > 127)	Led = 127;

	seg = RevXSegRemap_TypeX_PDV122[Led & 0x07];
	com = RevXComRemap_TypeX_PDV122[Led / 8];

	Data = 1<<7 | com<<3 | seg<<0;

	return Data;
}

uint32_t Get_RevY_Data_TypeX_PDV122(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint16_t Data;
	uint16_t com, seg;

	if(Led > 63)	Led = 63;

	seg = RevYSegRemap_TypeX_PDV122[Led & 0x07];
	com = RevYComRemap_TypeX_PDV122[Led / 8];

	Data = 0<<7 | com<<3 | seg<<0;

	return Data;
}

//------------------------------ TypeX V14.0 15" PT ----------------------------
uint32_t Get_EmitX_Data_TypeX_V140(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led > 63)	Led = 63;

	seg = EmitXSegRemap_TypeX_V140[Led & 0x07];
	com = EmitXComRemap_TypeX_V140[Led / 8];

	Data = com | seg;

	return Data;
}

uint32_t Get_EmitY_Data_TypeX_V140(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led > 63)	Led = 63;

	seg = EmitYSegRemap_TypeX_V140[Led & 0x07];
	com = EmitYComRemap_TypeX_V140[Led / 8];

	Data = com | seg;

	return Data;
}

uint32_t Get_RevX_Data_TypeX_PTV140(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led > 63)	Led = 63;

	seg = RevXSegRemap_TypeX_PTV140[Led & 0x07];
	com = RevXComRemap_TypeX_PTV140[Led / 8];

	Data = com | seg;

	return Data;
}

uint32_t Get_RevY_Data_TypeX_PTV140(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led > 63)	Led = 63;

	seg = RevYSegRemap_TypeX_PTV140[Led & 0x07];
	com = RevYComRemap_TypeX_PTV140[Led / 8];

	Data = com | seg;

	return Data;
}

//------------------------------ TypeX V14.2 21.5" PT ----------------------------
uint32_t Get_EmitX_Data_TypeX_V142(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led > 127)	Led = 127;

	seg = EmitXSegRemap_TypeX_V142[Led & 0x07];
	com = EmitXComRemap_TypeX_V142[Led / 8];

	Data = com | seg;

	return Data;
}

uint32_t Get_EmitY_Data_TypeX_V142(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led > 63)	Led = 63;

	seg = EmitYSegRemap_TypeX_V142[Led & 0x07];
	com = EmitYComRemap_TypeX_V142[Led / 8];

	Data = com | seg;

	return Data;
}

uint32_t Get_RevX_Data_TypeX_PTV142(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led > 127)	Led = 127;

	seg = RevXSegRemap_TypeX_PTV142[Led & 0x07];
	com = RevXComRemap_TypeX_PTV142[Led / 8];

	Data = com | seg;

	return Data;
}

uint32_t Get_RevY_Data_TypeX_PTV142(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led > 63)	Led = 63;

	seg = RevYSegRemap_TypeX_PTV142[Led & 0x07];
	com = RevYComRemap_TypeX_PTV142[Led / 8];

	Data = com | seg;

	return Data;
}

//------------------------------ TypeX V14.3 E24" PT ----------------------------
uint32_t Get_EmitY_Data_TypeX_V146(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led > 63)	Led = 63;

	seg = EmitYSegRemap_TypeX_V142[Led & 0x07];
	com = EmitYComRemap_TypeX_V146[Led / 8];

	Data = com | seg;

	return Data;
}

uint32_t Get_RevX_Data_TypeX_PTV146(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led > 127)	Led = 127;

	seg = RevXSegRemap_TypeX_PTV142[Led & 0x07];
	com = RevXComRemap_TypeX_PTV146[Led / 8];

	Data = com | seg;

	return Data;
}


//------------------------------ TypeX V14.3 E19 19" PT ----------------------------
uint32_t Get_EmitX_Data_TypeX_V143(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led > 62)	Led = 63;

	seg = EmitXSegRemap_TypeX_V143[Led & 0x07];
	com = EmitXComRemap_TypeX_V143[Led / 8];

	Data = com | seg;

	return Data;
}

uint32_t Get_EmitY_Data_TypeX_V143(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led > 49)	Led = 63;

	seg = EmitYSegRemap_TypeX_V143[Led & 0x07];
	com = EmitYComRemap_TypeX_V143[Led / 8];

	Data = com | seg;

	return Data;
}

uint32_t Get_RevX_Data_TypeX_PTV143(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led > 62)	Led = 63;

	seg = RevXSegRemap_TypeX_PTV143[Led & 0x07];
	com = RevXComRemap_TypeX_PTV143[Led / 8];

	Data = com | seg;

	return Data;
}

uint32_t Get_RevY_Data_TypeX_PTV143(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led > 49)	Led = 63;

	seg = RevYSegRemap_TypeX_PTV143[Led & 0x07];
	com = RevYComRemap_TypeX_PTV143[Led / 8];

	Data = com | seg;

	return Data;
}

//------------------------------ TypeX V14.5 E20.1 20.1" PT ----------------------------
uint32_t Get_EmitX_Data_TypeX_V145(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led <= 63) {
		seg = EmitXSegRemap_TypeX_V145[Led & 0x07];
		com = EmitXComRemap_TypeX_V145[Led / 8];
	} else {     //select null led
		Led = 63;
		seg = EmitYSegRemap_TypeX_V145[Led & 0x07];
		com = EmitYComRemap_TypeX_V145[Led / 8];
	}

	Data = com | seg;

	return Data;
}

uint32_t Get_EmitY_Data_TypeX_V145(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led > 47)	Led = 63;

	seg = EmitYSegRemap_TypeX_V145[Led & 0x07];
	com = EmitYComRemap_TypeX_V145[Led / 8];

	Data = com | seg;

	return Data;
}

uint32_t Get_RevX_Data_TypeX_PTV145(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led <= 63){
		seg = RevXSegRemap_TypeX_PTV145[Led & 0x07];
		com = RevXComRemap_TypeX_PTV145[Led / 8];
		Data = com | seg;
	} else {
    	Data = 0;
	}

	return Data;
}

uint32_t Get_RevY_Data_TypeX_PTV145(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led > 49)	Led = 63;

	seg = RevYSegRemap_TypeX_PTV145[Led & 0x07];
	com = RevYComRemap_TypeX_PTV145[Led / 8];

	Data = com | seg;

	return Data;
}


//------------------------------ TypeX V14.7 E12.1" PT ----------------------------
uint32_t Get_EmitX_Data_TypeX_V147(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led >= 36) {
		Led = 63; 	//NULL LED
	}

	seg = EmitSegRemap_TypeX_V147[Led & 0x07];
	com = EmitComRemap_TypeX_V147[Led / 8];

	Data = com | seg;

	return Data;
}

uint32_t Get_EmitY_Data_TypeX_V147(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led >= 27)	{
		Led = 63;
	} else {
		Led += 36;
	}

	seg = EmitSegRemap_TypeX_V147[Led & 0x07];
	com = EmitComRemap_TypeX_V147[Led / 8];

	Data = com | seg;

	return Data;
}

uint32_t Get_RevX_Data_TypeX_PTV147(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led < 36){
		Led += 27;
		seg = RevSegRemap_TypeX_PTV147[Led & 0x07];
		com = RevComRemap_TypeX_PTV147[Led / 8];
		Data = com | seg;
	} else {
    	Data = 0;
	}

	return Data;
}

uint32_t Get_RevY_Data_TypeX_PTV147(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led >= 27)	Led = 63;

	seg = RevSegRemap_TypeX_PTV147[Led & 0x07];
	com = RevComRemap_TypeX_PTV147[Led / 8];

	Data = com | seg;

	return Data;
}


//------------------------------ TypeX V14.8 E15" FPC PT ----------------------------
uint32_t Get_EmitX_Data_TypeX_V148(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	static uint32_t Gpio[2];
	uint32_t com, seg;

	if(Led > 35) {
		Gpio[0] = 0x80FE<<0;// shut down all emit common
		Gpio[1] = 0x02FB<<16;
	} else {
		Led = 35 - Led;
		Led += 27;
		Gpio[0] = EmitComRemap_TypeX_V148[Led / 8];
		Gpio[1] = EmitSegRemap_TypeX_V148[Led & 0x07];
	}

	return (uint32_t)Gpio;
}

uint32_t Get_EmitY_Data_TypeX_V148(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	static uint32_t Gpio[2];
	uint32_t com, seg;

	if(Led > 26)	{
		Gpio[0] = 0x80FE<<0;// shut down all emit common
		Gpio[1] = 0x02FB<<16;
	} else {
		Led = 26 - Led;
		Gpio[0] = EmitComRemap_TypeX_V148[Led / 8];
		Gpio[1] = EmitSegRemap_TypeX_V148[Led & 0x07];
	}
	return (uint32_t)Gpio;
}

uint32_t Get_RevX_Data_TypeX_PTV148(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	static uint32_t Gpio[2];
	uint32_t com, seg;

	if(Led < 36){
		Led = 35 - Led;
		Gpio[1] = RevSegRemap_TypeX_PTV148[Led & 0x07];
		Gpio[0] = RevComRemap_TypeX_PTV148[Led / 8];
	} else {
		Gpio[0] = Gpio[1] = 0;  //do not switch rev
	}

	return (uint32_t)Gpio;
}

uint32_t Get_RevY_Data_TypeX_PTV148(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	static uint32_t Gpio[2];
	uint32_t com, seg;

	if(Led < 27)	{
		Led = 26 - Led;
		Led += 36;
		Gpio[1] = RevSegRemap_TypeX_PTV148[Led & 0x07];
		Gpio[0] = RevComRemap_TypeX_PTV148[Led / 8];
	} else {
		Gpio[1] = Gpio[0] = 0;    //do not switch rev
	}

	return (uint32_t)Gpio;
}

//------------------------------ TypeX V14.1 15" - 19" PT ----------------------------
uint32_t Get_EmitX_Data_TypeX_V141(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led >= 96)	return 0x8080;

	seg = EmitXSegRemap_TypeX_V141[Led & 0x07];
	com = EmitXComRemap_TypeX_V141[Led / 8];

	Data = 0x80 << 8 | seg << 4 | com ;

	return Data;
}

uint32_t Get_EmitY_Data_TypeX_V141(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led >= 96)	return 0x8080;

	seg = EmitYSegRemap_TypeX_V141[Led & 0x07];
	com = EmitYComRemap_TypeX_V141[Led / 8];

	Data = (seg << 4 | com) << 8 | 0x80;

	return Data;
}

uint32_t Get_RevX_Data_TypeX_PTV141(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led >= 96)	return 0;

	seg = RevXSegRemap_TypeX_PTV141[Led & 0x07];
	com = RevXComRemap_TypeX_PTV141[Led / 8];

	Data = (1<<7 | com<<3 | seg) << 8;

	return Data;
}

uint32_t Get_RevY_Data_TypeX_PTV141(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	uint32_t Data;
	uint32_t com, seg;

	if(Led >= 96)	return 0;

	seg = RevYSegRemap_TypeX_PTV141[Led & 0x07];
	com = RevYComRemap_TypeX_PTV141[Led / 8];

	Data = (1<<7 | com<<3 | seg);

	return Data;
}

//------------------------------ fct ---------------------------------------
uint32_t Get_EmitX_Data_FCT(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	return 0;
}

uint32_t Get_EmitY_Data_FCT(uint16_t Addr, uint8_t Led, uint16_t Pow)
{
	return 0;
}

uint32_t Get_RevX_Data_FCT(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	return 0;
}

uint32_t Get_RevY_Data_FCT(uint16_t Addr, uint8_t Led, uint16_t Gain)
{
	return 0;
}



#define INVALID_AD_MAP	0xFFFF

#if 1
//-------------------------------------------------------------------------------------
void Gen_Switch_Emit_Table(uint32_t Flags, uint16_t *EmitTable, uint16_t *RevTable, uint16_t *AdMap)
{
	int i,j,k;
	short Led; //led index inside one board
	short Boards;
	short Address;
	uint16_t Index;
	uint32_t Data;
	int SkipAxis;
	int LedToFind;
	bool spiAdd = false;
	bool SkipFirstAxi = (Flags & 0x01) != 0;
	bool DualSpiData = (Flags & 0x04) != 0;
	bool ShutdownEmit = (Flags & 0x08) != 0;
	bool SPI8Bits = (Flags & 0x10) != 0;
	bool ReverseX = (Flags & 0x40) != 0;
	bool ReverseY = (Flags & 0x80) != 0;
	bool DoubleScanBrdFirst = (Flags & 0x100) != 0;
	bool Anti = (Flags & 0x200) != 0;
	bool SwGPIO = (Flags & (1<<12)) != 0;

	uint16_t XLeds = XParam.TotalLeds;
	uint16_t YLeds = YParam.TotalLeds;
	uint16_t XAxis = XParam.Axis;

	//uint16_t YAxis = YParam.Axis;

	uint16_t Starty = XLeds * XAxis;

	if(Anti)	SkipAxis = ANTI_AXIS;
	else if(SkipFirstAxi) SkipAxis = 1;
	else SkipAxis = 0;

	Index = 0;
	if(XParam.Get_Rev_Data != NULL && YParam.Get_Rev_Data != NULL)
	{
	//------------------------------- Rev x --------------------------------------
		for (i=0; i<XParam.TotalLeds; i++)
		{
			if(ReverseX)
				LedToFind = XParam.TotalLeds - 1 - i;
			else
				LedToFind = i;

			if(XParam.ScanGap > 0) {
				LedToFind *= (XParam.ScanGap + 1);
			}

			FindBoardLed(&XParam, LedToFind, &Led, &Boards);

			//FindBoardLed(&XParam, i, &Led, &Boards);
			Address = (YParam.Boards + 1 + Boards);

			Data = (*XParam.Get_Rev_Data)(Address, Led, XParam.Gain[0]);

			if(DualSpiData) Data = ConvertData(Data);

			if(SPI8Bits)
				((uint8_t *)RevTable)[Index ++] = Data & 0x0FF;
			else if(SwGPIO)
				((uint32_t *)RevTable)[Index ++] = Data;
			else
				RevTable[Index++] = Data & 0x0FFFF;

			if(DoubleScanBrdFirst && CheckBoardStartLed(&XParam, Boards, Led, XParam.ReverseXScan) == true)
			{
				if(SPI8Bits)
					((uint8_t *)RevTable)[Index ++] = Data & 0x0FF;
				else if(SwGPIO)
					((uint32_t *)RevTable)[Index ++] = Data;
				else
					RevTable[Index++] = Data & 0x0FFFF;
			}
		}

	//------------------------------- Rev y --------------------------------------
		for (i=0; i<YParam.TotalLeds; i++)
		{
			if(ReverseY)
				LedToFind = YParam.TotalLeds - 1 - i;
			else
				LedToFind = i;

			if(YParam.ScanGap > 0) {
				LedToFind *= (YParam.ScanGap + 1);
			}

			FindBoardLed(&YParam, LedToFind, &Led, &Boards);

			//FindBoardLed(&YParam, i, &Led, &Boards);
			Address = (1 + Boards);

			Data = (*YParam.Get_Rev_Data)(Address, Led, YParam.Gain[0]);

			if(DualSpiData) Data = ConvertData(Data);

			if(SPI8Bits)
				((uint8_t *)RevTable)[Index++] = Data & 0x0FF;
			else if(SwGPIO)
				((uint32_t *)RevTable)[Index ++] = Data;
			else
				RevTable[Index++] = Data & 0x0FFFF;

			if(DoubleScanBrdFirst && CheckBoardStartLed(&YParam, Boards, Led, YParam.ReverseYScan) == true)
			{
				if(SPI8Bits)
					((uint8_t *)RevTable)[Index ++] = Data & 0x0FF;
				else if(SwGPIO)
					((uint32_t *)RevTable)[Index ++] = Data;
				else
					RevTable[Index++] = Data & 0x0FFFF;
			}
		}

		if(ShutdownEmit)
		{
			Data = (*YParam.Get_Rev_Data)(Address, NULL_LED, YParam.Gain[0]);
			if(DualSpiData) Data = ConvertData(Data);

			if(SPI8Bits)
				((uint8_t *)RevTable)[Index++] = Data & 0x0FF;
			else if(SwGPIO)
				((uint32_t *)RevTable)[Index ++] = Data;
			else
				RevTable[Index++] = Data & 0x0FFFF;
		}
	}
	else
	{
		spiAdd = true;
	}

//------------------------------- Emit x --------------------------------------
	Index = 0;
	for (i=0; i<XParam.TotalLeds; i++)
	{
		if(ReverseX)
			LedToFind = XParam.TotalLeds - 1 - i;
		else
			LedToFind = i;

		if(XParam.ScanGap) {
			LedToFind *= (XParam.ScanGap + 1);
		}

		FindBoardLed(&XParam, LedToFind, &Led, &Boards);

		if(DoubleScanBrdFirst && CheckBoardStartLed(&XParam, Boards, Led, XParam.ReverseXScan) == true)   //2013.2.19
		{
			Data = (*XParam.Get_Emit_Data)(Address, NULL_LED, XParam.EmitPower);
			if(DualSpiData) Data = ConvertData(Data);

			for (j=0; j<XParam.Axis; j+=1)
			{
				if(j==0)
				{
					for(k=0; k<SkipAxis; k+=1)
					{
						if(spiAdd) RevTable[Index] = BOARD_SEL_LOGIC(0);

						if(AdMap) AdMap[Index] = INVALID_AD_MAP;
						
						if(SPI8Bits)
							((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
						else if(SwGPIO)
							((uint32_t *)EmitTable)[Index ++] = Data;
						else
							EmitTable[Index ++] = Data & 0x0FFFF;
					}
				}

				if(spiAdd) RevTable[Index] = BOARD_SEL_LOGIC(0);

				if(AdMap) AdMap[Index] = INVALID_AD_MAP;

				if(SPI8Bits)
					((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
				else if(SwGPIO)
					((uint32_t *)EmitTable)[Index ++] = Data;
				else
					EmitTable[Index ++] = Data & 0x0FFFF;
			}
		}

		for (j=0; j<XParam.Axis; j+=1)
		{
			if(ReverseX)
				LedToFind = XParam.TotalLeds - 1 - i - XParam.LedOff[j];
			else
				LedToFind = i + XParam.LedOff[j];

			if(XParam.ScanGap) {
            	LedToFind *= (XParam.ScanGap + 1);
			}

			FindBoardLed(&XParam, LedToFind, &Led, &Boards);

			//FindBoardLed(&XParam, i + XParam.LedOff[j], &Led, &Boards);

			if(Led == NULL_LED)
			{
				if(ReverseX)
					LedToFind = XParam.TotalLeds - 1 - i - XParam.AuxLedOff[j];
				else
					LedToFind = i + XParam.AuxLedOff[j];

				if(XParam.ScanGap) {
					LedToFind *= (XParam.ScanGap + 1);
				}

				FindBoardLed(&XParam, LedToFind, &Led, &Boards);

				//FindBoardLed(&XParam, i + XParam.AuxLedOff[j], &Led, &Boards);
			}

			Address = (1 + Boards);

			if(j==0)
			{
				uint32_t InvalidData = (*XParam.Get_Emit_Data)(Address, NULL_LED, XParam.EmitPower);
				uint32_t ValidData = (*XParam.Get_Emit_Data)(Address, Led, XParam.EmitPower);

				//if(DualSpiData) Data = ConvertData(Data);

				for(k=0; k<SkipAxis; k+=1)
				{
					if(k == SkipAxis -1 || i==0) {
						Data = ValidData;
					} else {
						Data = InvalidData;
					}
					
					if(DualSpiData) Data = ConvertData(Data);

					if(spiAdd) RevTable[Index] = BOARD_SEL_LOGIC(0);

					if(AdMap) AdMap[Index] = INVALID_AD_MAP;

					if(SPI8Bits)
						((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
					else if(SwGPIO)
						((uint32_t *)EmitTable)[Index ++] = Data;
					else
						EmitTable[Index ++] = Data & 0x0FFFF;
				}
			}

			Data = (*XParam.Get_Emit_Data)(Address, Led, XParam.EmitPower);
			if(DualSpiData) Data = ConvertData(Data);

			if(spiAdd) RevTable[Index] = BOARD_SEL_LOGIC(1<<(Address - 1));

			if(AdMap) AdMap[Index] = i + j*XLeds;

			if(SPI8Bits)
				((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
			else if(SwGPIO)
				((uint32_t *)EmitTable)[Index ++] = Data;
			else
				EmitTable[Index ++] = Data & 0x0FFFF;
		}
	}

//------------------------------- Emit y --------------------------------------
	for (i=0; i<YParam.TotalLeds; i++)
	{
		if(ReverseY)
			LedToFind = YParam.TotalLeds - 1 - i;
		else
			LedToFind = i;

		if(YParam.ScanGap) {
			LedToFind *= (YParam.ScanGap + 1);
		}

		FindBoardLed(&YParam, LedToFind, &Led, &Boards);

		if(DoubleScanBrdFirst && CheckBoardStartLed(&YParam, Boards, Led, YParam.ReverseYScan) == true)   //2013.2.19
		{
			Data = (*YParam.Get_Emit_Data)(Address, NULL_LED, YParam.EmitPower);
			if(DualSpiData) Data = ConvertData(Data);

			for (j=0; j<YParam.Axis; j+=1)
			{
				if(j==0)
				{
					for(k=0; k<SkipAxis; k+=1)
					{
						if(spiAdd) RevTable[Index] = BOARD_SEL_LOGIC(0);

						if(AdMap) AdMap[Index] = INVALID_AD_MAP;

						if(SPI8Bits)
							((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
						else if(SwGPIO)
							((uint32_t *)EmitTable)[Index ++] = Data;
						else
							EmitTable[Index ++] = Data & 0x0FFFF;
					}
				}

				if(spiAdd) RevTable[Index] = BOARD_SEL_LOGIC(0);

				if(AdMap) AdMap[Index] = INVALID_AD_MAP;

				if(SPI8Bits)
					((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
				else if(SwGPIO)
					((uint32_t *)EmitTable)[Index ++] = Data;
				else
					EmitTable[Index ++] = Data & 0x0FFFF;
			}
		}

		for (j=0; j<YParam.Axis; j+=1)
		{
			if(ReverseY)
				LedToFind = YParam.TotalLeds - 1 - i - YParam.LedOff[j];
			else
				LedToFind = i + YParam.LedOff[j];

			if(YParam.ScanGap > 0) {
				LedToFind *= (YParam.ScanGap + 1);
			}

			FindBoardLed(&YParam, LedToFind, &Led, &Boards);

			//FindBoardLed(&YParam, i + YParam.LedOff[j], &Led, &Boards);

			if(Led == NULL_LED)
			{
				if(ReverseY)
					LedToFind = YParam.TotalLeds - 1 - i - YParam.AuxLedOff[j];
				else
					LedToFind = i + YParam.AuxLedOff[j];

				if(YParam.ScanGap > 0) {
					LedToFind *= (YParam.ScanGap + 1);
				}

				FindBoardLed(&YParam, LedToFind, &Led, &Boards);
				//FindBoardLed(&YParam, i + YParam.AuxLedOff[j], &Led, &Boards);
			}

			Address = (XParam.Boards + 1 + Boards);

			if(j==0)
			{
				uint32_t InvalidData = (*YParam.Get_Emit_Data)(Address, NULL_LED, YParam.EmitPower);
				uint32_t ValidData = (*YParam.Get_Emit_Data)(Address, Led, YParam.EmitPower);

				//if(DualSpiData) Data = ConvertData(Data);

				for(k=0; k<SkipAxis; k+=1)
				{
					if(k == SkipAxis -1) {
						Data = ValidData;
					} else {
						Data = InvalidData;
					}

					if(DualSpiData) Data = ConvertData(Data);
					
					if(i==0){
						if(k==0) {
							Data = (*YParam.Get_Emit_Data)(Address, Led, YParam.EmitPower);
							if(spiAdd) RevTable[Index] = BOARD_SEL_LOGIC(1<<(Address - 1));
						} else {
							Data = (*YParam.Get_Emit_Data)(Address, NULL_LED, YParam.EmitPower);
							if(spiAdd) RevTable[Index] = BOARD_SEL_LOGIC(0);
						}
						
						if(DualSpiData) Data = ConvertData(Data);
					} else {
						if(spiAdd) RevTable[Index] = BOARD_SEL_LOGIC(0);
					}

					if(AdMap) AdMap[Index] = INVALID_AD_MAP;

					if(SPI8Bits)
						((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
					else if(SwGPIO)
						((uint32_t *)EmitTable)[Index ++] = Data;
					else
						EmitTable[Index ++] = Data & 0x0FFFF;
				}
			}

			Data = (*YParam.Get_Emit_Data)(Address, Led, YParam.EmitPower);
			if(DualSpiData) Data = ConvertData(Data);

			if(spiAdd) RevTable[Index] = BOARD_SEL_LOGIC(1<<(Address - 1));

			if(AdMap) AdMap[Index] = Starty + i + j*YLeds;

			if(SPI8Bits)
				((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
			else if(SwGPIO)
				((uint32_t *)EmitTable)[Index ++] = Data;
			else
				EmitTable[Index ++] = Data & 0x0FFFF;
		}
	}

	if(ShutdownEmit)
	{
		Data = (*YParam.Get_Emit_Data)(Address, NULL_LED, YParam.EmitPower);
		if(DualSpiData) Data = ConvertData(Data);

		for (j=0; j<YParam.Axis; j+=1)
		{
			if(spiAdd) RevTable[Index] = BOARD_SEL_LOGIC(0);

			if(AdMap) AdMap[Index] = INVALID_AD_MAP;

			if(SPI8Bits)
				((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
			else if(SwGPIO)
				((uint32_t *)EmitTable)[Index ++] = Data;
			else
				EmitTable[Index ++] = Data & 0x0FFFF;
		}

		for(k=0; k<SkipAxis; k+=1)
		{
			if(spiAdd) RevTable[Index] = BOARD_SEL_LOGIC(0);

			if(AdMap) AdMap[Index] = INVALID_AD_MAP;

			if(SPI8Bits)
				((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
			else if(SwGPIO)
				((uint32_t *)EmitTable)[Index ++] = Data;
			else
				EmitTable[Index ++] = Data & 0x0FFFF;
		}
	}
}

#else
//-------------------------------------------------------------------------------------
void Gen_Switch_Emit_Table(uint32_t Flags, uint16_t *EmitTable, uint16_t *RevTable, uint16_t *AdMap)
{
	int i,j,k;
	short Led;
	short Boards;
	short Address;
	uint16_t Index;
	uint32_t Data;
	int SkipAxis;
	int LedToFind;
	bool spiAdd = false;
	bool SkipFirstAxi = (Flags & 0x01) != 0;
	bool DualSpiData = (Flags & 0x04) != 0;
	bool ShutdownEmit = (Flags & 0x08) != 0;
	bool SPI8Bits = (Flags & 0x10) != 0;
	bool ReverseX = (Flags & 0x40) != 0;
	bool ReverseY = (Flags & 0x80) != 0;
	bool DoubleScanBrdFirst = (Flags & 0x100) != 0;
	bool Anti = (Flags & 0x200) != 0;
	bool SwGPIO = (Flags & (1<<12)) != 0;

	uint16_t XLeds = XParam.TotalLeds;
	uint16_t YLeds = YParam.TotalLeds;
	uint16_t XAxis = XParam.Axis;
	//uint16_t YAxis = YParam.Axis;

	uint16_t Starty = XLeds * XAxis;

	if(Anti)	SkipAxis = ANTI_AXIS;
	else if(SkipFirstAxi) SkipAxis = 1;
	else SkipAxis = 0;

	Index = 0;
	if(XParam.Get_Rev_Data != NULL && YParam.Get_Rev_Data != NULL)
	{
	//------------------------------- Rev x --------------------------------------
		for (i=0; i<XParam.TotalLeds; i++)
		{
			if(ReverseX)
				LedToFind = XParam.TotalLeds - 1 - i;
			else
				LedToFind = i;

			FindBoardLed(&XParam, LedToFind, &Led, &Boards);

			//FindBoardLed(&XParam, i, &Led, &Boards);
			Address = (YParam.Boards + 1 + Boards);

			Data = (*XParam.Get_Rev_Data)(Address, Led, XParam.Gain[0]);

			if(DualSpiData) Data = ConvertData(Data);

			if(SPI8Bits)
				((uint8_t *)RevTable)[Index ++] = Data & 0x0FF;
			else if(SwGPIO)
				((uint32_t *)RevTable)[Index ++] = Data;
			else
				RevTable[Index++] = Data & 0x0FFFF;

			if(DoubleScanBrdFirst && CheckBoardStartLed(&XParam, i) == true)
			{
				if(SPI8Bits)
					((uint8_t *)RevTable)[Index ++] = Data & 0x0FF;
				else if(SwGPIO)
					((uint32_t *)RevTable)[Index ++] = Data;
				else
					RevTable[Index++] = Data & 0x0FFFF;
			}
		}

	//------------------------------- Rev y --------------------------------------
		for (i=0; i<YParam.TotalLeds; i++)
		{
			if(ReverseY)
				LedToFind = YParam.TotalLeds - 1 - i;
			else
				LedToFind = i;

			FindBoardLed(&YParam, LedToFind, &Led, &Boards);

			//FindBoardLed(&YParam, i, &Led, &Boards);
			Address = (1 + Boards);

			Data = (*YParam.Get_Rev_Data)(Address, Led, YParam.Gain[0]);

			if(DualSpiData) Data = ConvertData(Data);

			if(SPI8Bits)
				((uint8_t *)RevTable)[Index++] = Data & 0x0FF;
			else if(SwGPIO)
				((uint32_t *)RevTable)[Index ++] = Data;
			else
				RevTable[Index++] = Data & 0x0FFFF;

			if(DoubleScanBrdFirst && CheckBoardStartLed(&YParam, i) == true)
			{
				if(SPI8Bits)
					((uint8_t *)RevTable)[Index ++] = Data & 0x0FF;
				else if(SwGPIO)
					((uint32_t *)RevTable)[Index ++] = Data;
				else
					RevTable[Index++] = Data & 0x0FFFF;
			}
		}

		if(ShutdownEmit)
		{
			Data = (*YParam.Get_Rev_Data)(Address, NULL_LED, YParam.Gain[0]);
			if(DualSpiData) Data = ConvertData(Data);

			if(SPI8Bits)
				((uint8_t *)RevTable)[Index++] = Data & 0x0FF;
			else if(SwGPIO)
				((uint32_t *)RevTable)[Index ++] = Data;
			else
				RevTable[Index++] = Data & 0x0FFFF;
		}
	}
	else
	{
		spiAdd = true;
	}

//------------------------------- Emit x --------------------------------------
	Index = 0;
	for (i=0; i<XParam.TotalLeds; i++)
	{
		if(DoubleScanBrdFirst && CheckBoardStartLed(&XParam, i) == true)   //2013.2.19
		{
			Data = (*XParam.Get_Emit_Data)(Address, NULL_LED, XParam.EmitPower);
			if(DualSpiData) Data = ConvertData(Data);

			for (j=0; j<XParam.Axis; j+=1)
			{
				if(j==0)
				{
					for(k=0; k<SkipAxis; k+=1)
					{
						if(spiAdd) RevTable[Index] = BOARD_SEL_LOGIC(0);

						if(AdMap) AdMap[Index] = INVALID_AD_MAP;
						
						if(SPI8Bits)
							((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
						else if(SwGPIO)
							((uint32_t *)EmitTable)[Index ++] = Data;
						else
							EmitTable[Index ++] = Data & 0x0FFFF;
					}
				}

				if(spiAdd) RevTable[Index] = BOARD_SEL_LOGIC(0);

				if(AdMap) AdMap[Index] = INVALID_AD_MAP;

				if(SPI8Bits)
					((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
				else if(SwGPIO)
					((uint32_t *)EmitTable)[Index ++] = Data;
				else
					EmitTable[Index ++] = Data & 0x0FFFF;
			}
		}

		for (j=0; j<XParam.Axis; j+=1)
		{
			if(ReverseX)
				LedToFind = XParam.TotalLeds - 1 - i - XParam.LedOff[j];
			else
				LedToFind = i + XParam.LedOff[j];

			FindBoardLed(&XParam, LedToFind, &Led, &Boards);

			//FindBoardLed(&XParam, i + XParam.LedOff[j], &Led, &Boards);

			if(Led == NULL_LED)
			{
				if(ReverseX)
					LedToFind = XParam.TotalLeds - 1 - i - XParam.AuxLedOff[j];
				else
					LedToFind = i + XParam.AuxLedOff[j];

				FindBoardLed(&XParam, LedToFind, &Led, &Boards);

				//FindBoardLed(&XParam, i + XParam.AuxLedOff[j], &Led, &Boards);
			}

			Address = (1 + Boards);

			if(j==0)
			{
				Data = (*XParam.Get_Emit_Data)(Address, NULL_LED, XParam.EmitPower);
				//Data = (*XParam.Get_Emit_Data)(Address, Led, XParam.EmitPower);

				if(DualSpiData) Data = ConvertData(Data);

				for(k=0; k<SkipAxis; k+=1)
				{
					if(spiAdd) RevTable[Index] = BOARD_SEL_LOGIC(0);

					if(AdMap) AdMap[Index] = INVALID_AD_MAP;

					if(SPI8Bits)
						((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
					else if(SwGPIO)
						((uint32_t *)EmitTable)[Index ++] = Data;
					else
						EmitTable[Index ++] = Data & 0x0FFFF;
				}
			}

			Data = (*XParam.Get_Emit_Data)(Address, Led, XParam.EmitPower);
			if(DualSpiData) Data = ConvertData(Data);

			if(spiAdd) RevTable[Index] = BOARD_SEL_LOGIC(1<<(Address - 1));

			if(AdMap) AdMap[Index] = i + j*XLeds;

			if(SPI8Bits)
				((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
			else if(SwGPIO)
				((uint32_t *)EmitTable)[Index ++] = Data;
			else
				EmitTable[Index ++] = Data & 0x0FFFF;
		}
	}

//------------------------------- Emit y --------------------------------------
	for (i=0; i<YParam.TotalLeds; i++)
	{
		if(DoubleScanBrdFirst && CheckBoardStartLed(&YParam, i) == true)   //2013.2.19
		{
			Data = (*YParam.Get_Emit_Data)(Address, NULL_LED, YParam.EmitPower);
			if(DualSpiData) Data = ConvertData(Data);

			for (j=0; j<YParam.Axis; j+=1)
			{
				if(j==0)
				{
					for(k=0; k<SkipAxis; k+=1)
					{
						if(spiAdd) RevTable[Index] = BOARD_SEL_LOGIC(0);

						if(AdMap) AdMap[Index] = INVALID_AD_MAP;

						if(SPI8Bits)
							((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
						else if(SwGPIO)
							((uint32_t *)EmitTable)[Index ++] = Data;
						else
							EmitTable[Index ++] = Data & 0x0FFFF;
					}
				}

				if(spiAdd) RevTable[Index] = BOARD_SEL_LOGIC(0);

				if(AdMap) AdMap[Index] = INVALID_AD_MAP;

				if(SPI8Bits)
					((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
				else if(SwGPIO)
					((uint32_t *)EmitTable)[Index ++] = Data;
				else
					EmitTable[Index ++] = Data & 0x0FFFF;
			}
		}

		for (j=0; j<YParam.Axis; j+=1)
		{
			if(ReverseY)
				LedToFind = YParam.TotalLeds - 1 - i - YParam.LedOff[j];
			else
				LedToFind = i + YParam.LedOff[j];

			FindBoardLed(&YParam, LedToFind, &Led, &Boards);

			//FindBoardLed(&YParam, i + YParam.LedOff[j], &Led, &Boards);

			if(Led == NULL_LED)
			{
				if(ReverseY)
					LedToFind = YParam.TotalLeds - 1 - i - YParam.AuxLedOff[j];
				else
					LedToFind = i + YParam.AuxLedOff[j];

				FindBoardLed(&YParam, LedToFind, &Led, &Boards);
				//FindBoardLed(&YParam, i + YParam.AuxLedOff[j], &Led, &Boards);
			}

			Address = (XParam.Boards + 1 + Boards);

			if(j==0)
			{
				Data = (*YParam.Get_Emit_Data)(Address, NULL_LED, YParam.EmitPower);
				//Data = (*YParam.Get_Emit_Data)(Address, Led, YParam.EmitPower);

				if(DualSpiData) Data = ConvertData(Data);

				for(k=0; k<SkipAxis; k+=1)
				{
					if(i==0){
						if(k==0) {
							Data = (*YParam.Get_Emit_Data)(Address, Led, YParam.EmitPower);
							if(spiAdd) RevTable[Index] = BOARD_SEL_LOGIC(1<<(Address - 1));
						} else {
							Data = (*YParam.Get_Emit_Data)(Address, NULL_LED, YParam.EmitPower);
							if(spiAdd) RevTable[Index] = BOARD_SEL_LOGIC(0);
						}
						
						if(DualSpiData) Data = ConvertData(Data);
					} else {
						if(spiAdd) RevTable[Index] = BOARD_SEL_LOGIC(0);
					}

					if(AdMap) AdMap[Index] = INVALID_AD_MAP;

					if(SPI8Bits)
						((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
					else if(SwGPIO)
						((uint32_t *)EmitTable)[Index ++] = Data;
					else
						EmitTable[Index ++] = Data & 0x0FFFF;
				}
			}

			Data = (*YParam.Get_Emit_Data)(Address, Led, YParam.EmitPower);
			if(DualSpiData) Data = ConvertData(Data);

			if(spiAdd) RevTable[Index] = BOARD_SEL_LOGIC(1<<(Address - 1));

			if(AdMap) AdMap[Index] = Starty + i + j*YLeds;

			if(SPI8Bits)
				((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
			else if(SwGPIO)
				((uint32_t *)EmitTable)[Index ++] = Data;
			else
				EmitTable[Index ++] = Data & 0x0FFFF;
		}
	}

	if(ShutdownEmit)
	{
		Data = (*YParam.Get_Emit_Data)(Address, NULL_LED, YParam.EmitPower);
		if(DualSpiData) Data = ConvertData(Data);

		for (j=0; j<YParam.Axis; j+=1)
		{
			if(spiAdd) RevTable[Index] = BOARD_SEL_LOGIC(0);

			if(AdMap) AdMap[Index] = INVALID_AD_MAP;

			if(SPI8Bits)
				((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
			else if(SwGPIO)
				((uint32_t *)EmitTable)[Index ++] = Data;
			else
				EmitTable[Index ++] = Data & 0x0FFFF;
		}

		for(k=0; k<SkipAxis; k+=1)
		{
			if(spiAdd) RevTable[Index] = BOARD_SEL_LOGIC(0);

			if(AdMap) AdMap[Index] = INVALID_AD_MAP;

			if(SPI8Bits)
				((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
			else if(SwGPIO)
				((uint32_t *)EmitTable)[Index ++] = Data;
			else
				EmitTable[Index ++] = Data & 0x0FFFF;
		}
	}

//		{
//		FILE *fp;
//		int lednum = 0;
//
//		if((fp = fopen("d:\\scan.c", "w")) == NULL)
//		{
//			ShowMessage("Can not open file!");
//
//			return;
//		}
//
//		fprintf(fp, "unsigned short Emit_Table [] = {\n");
//
//		for(i=0; i<Index; i+=1)
//		{
//			fprintf(fp, "0x%.4X, ", EmitTable[i]);
//
//			if(i % (SkipAxis + YParam.Axis) == SkipAxis + YParam.Axis - 1)
//			{
//				fprintf(fp, "/* %d */\n", ++lednum);
//			}
//		}
//
//		fprintf(fp, "};\n\n");
//		fprintf(fp, "unsigned short Rev_Table [] = {\n");
//
//		lednum = 0;
//
//		for(i=0; i<Index/(SkipAxis + YParam.Axis); i+=1)
//		{
//			fprintf(fp, "0x%.4X, /* %d */\n", RevTable[i], ++lednum);
//		}
//
//		fprintf(fp, "};\n\n");
//
//		fclose(fp);
//	}
}
#endif

void Gen_Switch_Emit_Only_Table_V15(uint32_t Flags, uint16_t *EmitTable, uint16_t *SpiTable, uint16_t *AdMap)
{
	int i,j,k;
	short Led;
	short Boards;
	short GroupsBefore;
	uint16_t Index;
	uint16_t *dat;
	int SkipAxis;
	uint16_t BoardGroups[MAX_BOARDS] = {0};

	bool SkipFirstAxi = (Flags & 0x01) != 0;
	//bool SPI8Bits = (Flags & 0x10) != 0;
	bool Anti = (Flags & 0x200) != 0;
	bool SkipLastY = (Flags & 0x2000) != 0;
	bool Skipx = (Flags & 0x4000) != 0;
	bool NotD7 = (Flags & (1<<16)) != 0;

	uint16_t XLeds = XParam.TotalLeds;
	uint16_t YLeds = YParam.TotalLeds;
	uint16_t XAxis = XParam.Axis;

	uint16_t Starty = XLeds * XAxis;

	uint16_t xbrds = XParam.Boards;

	BoardGroups[0] = 0;
	for(int i=0; i<XParam.Boards; i+=1){
		BoardGroups[i+1] = BoardGroups[i] + (XParam.BoradLeds[i]+7)/8;
	}

	for(int i=0; i<YParam.Boards; i+=1){
		BoardGroups[i + 1 + xbrds] = BoardGroups[i+xbrds] + (YParam.BoradLeds[i]+7)/8;
	}


	if(Anti)	SkipAxis = ANTI_AXIS;
	else if(SkipFirstAxi) SkipAxis = 1;
	else SkipAxis = 0;

	Index = 0;
	//------------------------------- Emit y first--------------------------------------
	for (i=0; i<YParam.TotalLeds; i++)
	{
		for (j=0; j<YParam.Axis; j+=1)
		{
			FindBoardLed(&YParam, i + YParam.LedOff[j], &Led, &Boards);

			if(Led == NULL_LED){
				FindBoardLed(&YParam, i + YParam.AuxLedOff[j], &Led, &Boards);
			}

			if(NotD7){
				GroupsBefore = BoardGroups[XParam.Boards + Boards];
			} else {
				GroupsBefore = (XParam.Boards + Boards) * 8;
			}

			if(j==0)
			{
				dat = (uint16_t *)(*YParam.Get_Emit_Data)(0, NULL_LED, YParam.EmitPower);      //shutdown emit

				for(k=0; k<SkipAxis; k+=1)
				{
					SpiTable[Index*3 + 0] = dat[0];
					SpiTable[Index*3 + 1] = dat[1];
					SpiTable[Index*3 + 2] = dat[2];

					if(AdMap) AdMap[Index] = INVALID_AD_MAP;

					EmitTable[Index*2 + 0] = dat[3];
					EmitTable[Index*2 + 1] = dat[4];

					Index ++;
				}
			}

			dat = (uint16_t *)(*YParam.Get_Emit_Data)(GroupsBefore, Led, YParam.EmitPower);

			SpiTable[Index*3 + 0] = dat[0];
			SpiTable[Index*3 + 1] = dat[1];
			SpiTable[Index*3 + 2] = dat[2];

			if(AdMap) AdMap[Index] = Starty + i + j * YLeds;

			EmitTable[Index*2 + 0] = dat[3];
			EmitTable[Index*2 + 1] = dat[4];

			Index ++;
		}

		FindBoardLed(&YParam, i, &Led, &Boards);

		if(Led == YParam.BoradLeds[Boards]-1 && (YParam.BoradLeds[Boards] & 0x07) != 0)    //the last led
		{
			dat = (uint16_t *)(*YParam.Get_Emit_Data)(0, NULL_LED, YParam.EmitPower);          //shutdown emit

			for(j=0; j<7-(Led&0x07); j+=1)
			{
				for(k=0; k<SkipAxis + YParam.Axis; k+=1)
				{
					SpiTable[Index*3 + 0] = dat[0];
					SpiTable[Index*3 + 1] = dat[1];
					SpiTable[Index*3 + 2] = dat[2];

					if(AdMap) AdMap[Index] = INVALID_AD_MAP;

					EmitTable[Index*2 + 0] = dat[3];
					EmitTable[Index*2 + 1] = dat[4];

					Index ++;
				}
			}
		}
		else if(SkipLastY && i==YParam.TotalLeds - 1 && (YParam.TotalLeds & 0x07) == 0)
		{
			//shit!  here we need to skip 8 leds!!!!!
			dat = (uint16_t *)(*YParam.Get_Emit_Data)(0, NULL_LED, YParam.EmitPower);

			for(j=0; j<8; j+=1)
			{
				for(k=0; k<SkipAxis + YParam.Axis; k+=1)
				{
					SpiTable[Index*3 + 0] = dat[0];
					SpiTable[Index*3 + 1] = dat[1];
					SpiTable[Index*3 + 2] = dat[2];

					if(AdMap) AdMap[Index] = INVALID_AD_MAP;

					EmitTable[Index*2 + 0] = dat[3];
					EmitTable[Index*2 + 1] = dat[4];

					Index ++;
				}
			}
		}
	}

	//------------------------------- Emit x --------------------------------------
	for (i=0; i<XParam.TotalLeds; i++)
	{
		for (j=0; j<XParam.Axis; j+=1)
		{
			FindBoardLed(&XParam, i + XParam.LedOff[j], &Led, &Boards);

			if(Led == NULL_LED) {
				FindBoardLed(&XParam, i + XParam.AuxLedOff[j], &Led, &Boards);
			}

			if(NotD7){
				GroupsBefore = BoardGroups[Boards];
			} else {
				GroupsBefore = Boards * 8;
			}

			if(j==0)
			{
				dat = (uint16_t *)(*XParam.Get_Emit_Data)(0, NULL_LED, XParam.EmitPower);

				for(k=0; k<SkipAxis; k+=1)
				{
					SpiTable[Index*3 + 0] = dat[0];
					SpiTable[Index*3 + 1] = dat[1];
					SpiTable[Index*3 + 2] = dat[2];

					if(AdMap) AdMap[Index] = INVALID_AD_MAP;

					EmitTable[Index*2 + 0] = dat[3];
					EmitTable[Index*2 + 1] = dat[4];

					Index ++;
				}
			}

			dat = (uint16_t *)(*XParam.Get_Emit_Data)(GroupsBefore, Led, XParam.EmitPower);

			SpiTable[Index*3 + 0] = dat[0];
			SpiTable[Index*3 + 1] = dat[1];
			SpiTable[Index*3 + 2] = dat[2];

			if(AdMap) AdMap[Index] = i + j*XLeds;;

			EmitTable[Index*2 + 0] = dat[3];
			EmitTable[Index*2 + 1] = dat[4];

			Index ++;
		}

		FindBoardLed(&XParam, i, &Led, &Boards);

		if(Led == XParam.BoradLeds[Boards]-1 && (XParam.BoradLeds[Boards] & 0x07) != 0)    //the last led
		{
			if(Boards == XParam.Boards-1 && Skipx == false)        //we need to skip the end of x for V16
				continue;

			dat = (uint16_t *)(*XParam.Get_Emit_Data)(0, NULL_LED, XParam.EmitPower);

			for(j=0; j<7-(Led&0x07); j+=1)
			{
				for(k=0; k<SkipAxis + XParam.Axis; k+=1)
				{
					SpiTable[Index*3 + 0] = dat[0];
					SpiTable[Index*3 + 1] = dat[1];
					SpiTable[Index*3 + 2] = dat[2];

					if(AdMap) AdMap[Index] = INVALID_AD_MAP;

					EmitTable[Index*2 + 0] = dat[3];
					EmitTable[Index*2 + 1] = dat[4];

					Index ++;
				}
			}
		}
	}
}

typedef struct {
	uint32_t *GPIOA;
	uint32_t *GPIOB;
	uint32_t *GPIOC;
	uint32_t *GPIOD;
}gpio_t;

void CopyGpioData(gpio_t *gpio, uint32_t *GpioData, int Index)
{
	if(GpioData[0] != 0) {
		gpio->GPIOA[Index] = GpioData[0];
	}

	if(GpioData[1] != 0) {
		gpio->GPIOB[Index] = GpioData[1];
	}

	if(GpioData[2] != 0) {
		gpio->GPIOC[Index] = GpioData[2];
	}

	if(GpioData[3] != 0) {
		gpio->GPIOD[Index] = GpioData[3];
	}
}

//#define BOARD_SEL_LOGIC_V133(a)	a
#define BOARD_SEL_LOGIC_V133(a)   (~(a))
void Gen_Switch_Emit_GPIO(uint32_t Flags, uint16_t *EmitTable, uint16_t *RevTable, gpio_t *gpio, uint16_t *AdMap)
{
	int i,j,k;
	short Led;
	short Boards;
	short Address;
	uint16_t Index;
	uint32_t *GpioData;
	int SkipAxis;
	//bool spiAdd = false;
	bool SkipFirstAxi = (Flags & 0x01) != 0;
	//bool DualSpiData = (Flags & 0x04) != 0;
	//bool ShutdownEmit = (Flags & 0x08) != 0;
	//bool SPI8Bits = (Flags & 0x10) != 0;
	//bool GPIOGain = (Flags & 0x20) != 0;
	//bool DoubleScanBrdFirst = (Flags & 0x100) != 0;
	bool Anti = (Flags & 0x200) != 0;
	bool SkipLastY = (Flags & 0x2000) != 0;
	bool Skipx = (Flags & 0x4000) != 0;
	bool WaitRxPowerOn = (Flags &(1<<17)) != 0;
	bool DynamicScan = false;

	uint16_t XLeds = XParam.TotalLeds;
	uint16_t YLeds = YParam.TotalLeds;
	uint16_t XAxis = XParam.Axis;
	//uint16_t YAxis = YParam.Axis;

	if(XAxis > 9) {
		DynamicScan = true;
	}

	uint16_t Starty = XLeds * XAxis;

	if(Anti)	SkipAxis = ANTI_AXIS;
	else if(SkipFirstAxi) SkipAxis = 1;
	else SkipAxis = 0;

	Index = 0;
	//------------------------------- Emit y first--------------------------------------
	for (i=0; i<YParam.TotalLeds; i++)
	{
		if(DynamicScan) {    //main axi only
			FindBoardLed(&YParam, i, &Led, &Boards);
			Address = (XParam.Boards + 1 + Boards);

			GpioData = (uint32_t *)((*YParam.Get_Emit_Data)(Address, Led, YParam.EmitPower));

			EmitTable[Index] =  BOARD_SEL_LOGIC_V133(1<<(Address - 1));

			if(AdMap) AdMap[Index] = Starty + i + j * YLeds;

			CopyGpioData(gpio, GpioData, Index);
			Index ++;
		} else {
			if(WaitRxPowerOn && !DynamicScan) {
				FindBoardLed(&YParam, i, &Led, &Boards);

				if((Led & 0x07) == 0){
					GpioData = (uint32_t *)(*YParam.Get_Emit_Data)(Address, NULL_LED, YParam.EmitPower);

					for(k=0; k<YParam.Axis + SkipAxis; k+=1)
					{
						EmitTable[Index] = BOARD_SEL_LOGIC_V133(0);

						if(AdMap) AdMap[Index] = INVALID_AD_MAP;

						CopyGpioData(gpio, GpioData, Index);
						Index ++;
					}
				}
			}

			for (j=0; j<YParam.Axis; j+=1)
			{
				FindBoardLed(&YParam, i + YParam.LedOff[j], &Led, &Boards);

				if(Led == NULL_LED){
					FindBoardLed(&YParam, i + YParam.AuxLedOff[j], &Led, &Boards);
				}

				Address = (XParam.Boards + 1 + Boards);

				if(j==0)
				{
					GpioData = (uint32_t *)(*YParam.Get_Emit_Data)(Address, NULL_LED, YParam.EmitPower);

					for(k=0; k<SkipAxis; k+=1)
					{
						EmitTable[Index] = BOARD_SEL_LOGIC_V133(0);

						if(AdMap) AdMap[Index] = INVALID_AD_MAP;

						CopyGpioData(gpio, GpioData, Index);
						Index ++;
					}
				}

				GpioData = (uint32_t *)((*YParam.Get_Emit_Data)(Address, Led, YParam.EmitPower));

				EmitTable[Index] =  BOARD_SEL_LOGIC_V133(1<<(Address - 1));

				if(AdMap) AdMap[Index] = Starty + i + j * YLeds;

				CopyGpioData(gpio, GpioData, Index);
				Index ++;
			}

			FindBoardLed(&YParam, i, &Led, &Boards);

			if(Led == YParam.BoradLeds[Boards]-1 && (YParam.BoradLeds[Boards] & 0x07) != 0)    //the last led
			{
				GpioData = (uint32_t *)(*YParam.Get_Emit_Data)(Address, NULL_LED, YParam.EmitPower);

				for(j=0; j<7-(Led&0x07); j+=1)
				{
					for(k=0; k<SkipAxis + YParam.Axis; k+=1)
					{
						EmitTable[Index] = BOARD_SEL_LOGIC_V133(0);

						if(AdMap) AdMap[Index] = INVALID_AD_MAP;

						CopyGpioData(gpio, GpioData, Index);
						Index ++;
					}
				}
			}
			else if(SkipLastY && i==YParam.TotalLeds - 1 && (YParam.TotalLeds & 0x07) == 0)
			{
				//shit!  here we need to skip 8 leds!!!!!
				GpioData = (uint32_t *)(*YParam.Get_Emit_Data)(Address, NULL_LED, YParam.EmitPower);

				for(j=0; j<8; j+=1)
				{
					for(k=0; k<SkipAxis + YParam.Axis; k+=1)
					{
						EmitTable[Index] = BOARD_SEL_LOGIC_V133(0);

						if(AdMap) AdMap[Index] = INVALID_AD_MAP;

						CopyGpioData(gpio, GpioData, Index);
						Index ++;
					}
				}
			}
		}
	}

	//------------------------------- Emit x --------------------------------------
	for (i=0; i<XParam.TotalLeds; i++)
	{
		if(DynamicScan) {    //main axi only
			FindBoardLed(&XParam, i, &Led, &Boards);
			Address = (1 + Boards);

			GpioData = (uint32_t *)(*XParam.Get_Emit_Data)(Address, Led, XParam.EmitPower);

			EmitTable[Index] = BOARD_SEL_LOGIC_V133(1<<(Address - 1));

			if(AdMap) AdMap[Index] = i + j*XLeds;

			CopyGpioData(gpio, GpioData, Index);
			Index ++;
		} else {
			if(WaitRxPowerOn) {
				FindBoardLed(&XParam, i, &Led, &Boards);

				if((Led & 0x07) == 0){
					GpioData = (uint32_t *)(*XParam.Get_Emit_Data)(Address, NULL_LED, XParam.EmitPower);

					for(k=0; k<XParam.Axis + SkipAxis; k+=1)
					{
						EmitTable[Index] = BOARD_SEL_LOGIC_V133(0);

						if(AdMap) AdMap[Index] = INVALID_AD_MAP;

						CopyGpioData(gpio, GpioData, Index);
						Index ++;
					}
				}
			}

			for (j=0; j<XParam.Axis; j+=1)
			{
				FindBoardLed(&XParam, i + XParam.LedOff[j], &Led, &Boards);

				if(Led == NULL_LED)
				{
					FindBoardLed(&XParam, i + XParam.AuxLedOff[j], &Led, &Boards);
				}

				Address = (1 + Boards);

				if(j==0)
				{
					GpioData = (uint32_t *)(*XParam.Get_Emit_Data)(Address, NULL_LED, XParam.EmitPower);

					for(k=0; k<SkipAxis; k+=1)
					{
						EmitTable[Index] = BOARD_SEL_LOGIC_V133(0);

						if(AdMap) AdMap[Index] = INVALID_AD_MAP;

						CopyGpioData(gpio, GpioData, Index);
						Index ++;
					}
				}

				GpioData = (uint32_t *)(*XParam.Get_Emit_Data)(Address, Led, XParam.EmitPower);

				EmitTable[Index] = BOARD_SEL_LOGIC_V133(1<<(Address - 1));

				if(AdMap) AdMap[Index] = i + j*XLeds;

				CopyGpioData(gpio, GpioData, Index);
				Index ++;
			}

			FindBoardLed(&XParam, i, &Led, &Boards);

			if(Led == XParam.BoradLeds[Boards]-1) {   //the last led
				int skips = 0;

				if((XParam.BoradLeds[Boards] & 0x07) != 0) {
					skips = 7-(Led&0x07);
				} else if(Boards == XParam.Boards -1 && (XParam.Get_Emit_Data == Get_Emit_Data_PDV1334 || \
														 XParam.Get_Emit_Data == Get_Emit_Data_PDV1335 )){
					skips = 8;
					if(WaitRxPowerOn) skips +=1;
				} else {
					continue;
				}

				if(Boards == XParam.Boards-1 && Skipx == false)        //we need to skip the end of x for V16
					continue;

				GpioData = (uint32_t *)(*XParam.Get_Emit_Data)(Address, NULL_LED, XParam.EmitPower);

				for(j=0; j<skips; j+=1)
				{
					for(k=0; k<SkipAxis + XParam.Axis; k+=1)
					{
						EmitTable[Index] = BOARD_SEL_LOGIC_V133(0);

						if(AdMap) AdMap[Index] = INVALID_AD_MAP;

						CopyGpioData(gpio, GpioData, Index);
						Index ++;
					}
				}
			}
		}

	}

	//------------------------- receive switch table -----------------------
	const uint8_t V133_HC4051_REMAP[8] = {3, 0, 1, 2, 5, 7, 6, 4};
	const uint8_t V1334_HC4051_REMAP[8] = {2, 1, 0, 3, 5, 7, 6, 4};
	const uint32_t V1335_HC4051_REMAP[8] = {2, 4, 0, 6, 5, 7, 3, 1};

	if(!DynamicScan && (XParam.Get_Emit_Data == Get_Emit_Data_PDV133 || XParam.Get_Emit_Data == Get_Emit_Data_PDV1331 ||
	   XParam.Get_Emit_Data == Get_Emit_Data_PDV1333 || XParam.Get_Emit_Data == Get_Emit_Data_PDV1334 || \
	   XParam.Get_Emit_Data == Get_Emit_Data_PDV1335)) {

		uint32_t Data;
		Index = 0;

		for(int i=0; i<8; i+=1){

			if(XParam.Get_Emit_Data == Get_Emit_Data_PDV1335) {
				Data = V1335_HC4051_REMAP[i];
				Data = (Data & 0x07) << 8 | ((~Data) & 0x07) << (8+16);
			} else if(XParam.Get_Emit_Data == Get_Emit_Data_PDV1334) {
				Data = V1334_HC4051_REMAP[i];
				Data = (Data & 0x04) << 1 | (Data & 0x03);
				Data = (~Data & 0xB) << 16 | Data;
			} else {
				Data = V133_HC4051_REMAP[i];
				Data = (Data & 0x04) << 1 | (Data & 0x03);
				Data = (~Data & 0xB) << 16 | Data;
			}

//			if(i<4){
//				Data |= 1<<(6);	//rev clk high
//			} else {
				Data |= 1<<(6+16);	//rev clk low
//			}

			if(WaitRxPowerOn && i == 0) {
				for(int j=0; j<SkipAxis + XParam.Axis; j+=1){
					((uint32_t *)RevTable)[Index ++] = Data;
				}
			}
			
			for(int j=0; j<SkipAxis + XParam.Axis; j+=1){
				((uint32_t *)RevTable)[Index ++] = Data;
			}
		}

		for(i=1; i<8; i+=1){
			Data = ((uint32_t *)RevTable)[i];
			Data |= 1<<6;                    //clock high
			Data &= ~(1<<(6+16));
			((uint32_t *)RevTable)[i] = Data;
		}
	}
}

void Gen_Switch_Emit_Only_Table(uint32_t Flags, uint16_t *EmitTable, uint16_t *RevTable, uint16_t *AdMap)
{
	int i,j,k;
	short Led;
	short Boards;
	short Address;
	uint16_t Index;
	uint32_t Data;
	int SkipAxis;
	//bool spiAdd = false;
	bool SkipFirstAxi = (Flags & 0x01) != 0;
	//bool DualSpiData = (Flags & 0x04) != 0;
	//bool ShutdownEmit = (Flags & 0x08) != 0;
	bool SPI8Bits = (Flags & 0x10) != 0;
	//bool GPIOGain = (Flags & 0x20) != 0;
	//bool DoubleScanBrdFirst = (Flags & 0x100) != 0;
	bool Anti = (Flags & 0x200) != 0;
	bool SkipLastY = (Flags & 0x2000) != 0;
	bool Skipx = (Flags & 0x4000) != 0;

	uint16_t XLeds = XParam.TotalLeds;
	uint16_t YLeds = YParam.TotalLeds;
	uint16_t XAxis = XParam.Axis;
	//uint16_t YAxis = YParam.Axis;

	uint16_t Starty = XLeds * XAxis;

	if(Anti)	SkipAxis = ANTI_AXIS;
	else if(SkipFirstAxi) SkipAxis = 1;
	else SkipAxis = 0;

	Index = 0;
	//------------------------------- Emit y first--------------------------------------
	for (i=0; i<YParam.TotalLeds; i++)
	{
		for (j=0; j<YParam.Axis; j+=1)
		{
			FindBoardLed(&YParam, i + YParam.LedOff[j], &Led, &Boards);

			if(Led == NULL_LED){
				FindBoardLed(&YParam, i + YParam.AuxLedOff[j], &Led, &Boards);
			}

			Address = (XParam.Boards + 1 + Boards);

			if(j==0)
			{
				if(i==0) {
					Data = (*YParam.Get_Emit_Data)(Address, Led, YParam.EmitPower);
				} else {
					Data = (*YParam.Get_Emit_Data)(Address, NULL_LED, YParam.EmitPower);
				}

				for(k=0; k<SkipAxis; k+=1)
				{
					RevTable[Index] = BOARD_SEL_LOGIC(0);

					if(AdMap) AdMap[Index] = INVALID_AD_MAP;

					if(SPI8Bits)
						((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
					else
						EmitTable[Index ++] = Data & 0x0FFFF;
				}
			}

			Data = (*YParam.Get_Emit_Data)(Address, Led, YParam.EmitPower);

			RevTable[Index] = BOARD_SEL_LOGIC(1<<(Address - 1));

			if(AdMap) AdMap[Index] = Starty + i + j * YLeds;

			if(SPI8Bits)
				((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
			else
				EmitTable[Index ++] = Data & 0x0FFFF;
		}

		FindBoardLed(&YParam, i, &Led, &Boards);

		if(Led == YParam.BoradLeds[Boards]-1 && (YParam.BoradLeds[Boards] & 0x07) != 0)    //the last led
		{
			Data = (*YParam.Get_Emit_Data)(Address, NULL_LED, YParam.EmitPower);

			for(j=0; j<7-(Led&0x07); j+=1)
			{
				for(k=0; k<SkipAxis + YParam.Axis; k+=1)
				{
					RevTable[Index] = BOARD_SEL_LOGIC(0);

					if(AdMap) AdMap[Index] = INVALID_AD_MAP;

					if(SPI8Bits)
						((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
					else
						EmitTable[Index ++] = Data & 0x0FFFF;
				}
			}
		}
		else if(SkipLastY && i==YParam.TotalLeds - 1 && (YParam.TotalLeds & 0x07) == 0)
		{
			//shit!  here we need to skip 8 leds!!!!!
			Data = (*YParam.Get_Emit_Data)(Address, NULL_LED, YParam.EmitPower);

			for(j=0; j<8; j+=1)
			{
				for(k=0; k<SkipAxis + YParam.Axis; k+=1)
				{
					RevTable[Index] = BOARD_SEL_LOGIC(0);

					if(AdMap) AdMap[Index] = INVALID_AD_MAP;

					if(SPI8Bits)
						((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
					else
						EmitTable[Index ++] = Data & 0x0FFFF;
				}
			}
		}
	}

	//------------------------------- Emit x --------------------------------------
	for (i=0; i<XParam.TotalLeds; i++)
	{
		for (j=0; j<XParam.Axis; j+=1)
		{
			FindBoardLed(&XParam, i + XParam.LedOff[j], &Led, &Boards);

			if(Led == NULL_LED)
			{
				FindBoardLed(&XParam, i + XParam.AuxLedOff[j], &Led, &Boards);
			}

			Address = (1 + Boards);

			if(j==0)
			{
				Data = (*XParam.Get_Emit_Data)(Address, NULL_LED, XParam.EmitPower);

				for(k=0; k<SkipAxis; k+=1)
				{
					RevTable[Index] = BOARD_SEL_LOGIC(0);

					if(AdMap) AdMap[Index] = INVALID_AD_MAP;

					if(SPI8Bits)
						((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
					else
						EmitTable[Index ++] = Data & 0x0FFFF;
				}
			}

			Data = (*XParam.Get_Emit_Data)(Address, Led, XParam.EmitPower);

			RevTable[Index] = BOARD_SEL_LOGIC(1<<(Address - 1));

			if(AdMap) AdMap[Index] = i + j*XLeds;

			if(SPI8Bits)
				((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
			else
				EmitTable[Index ++] = Data & 0x0FFFF;
		}

		FindBoardLed(&XParam, i, &Led, &Boards);

		if(Led == XParam.BoradLeds[Boards]-1 && (XParam.BoradLeds[Boards] & 0x07) != 0)    //the last led
		{
			if(Boards == XParam.Boards-1 && Skipx == false)        //we need to skip the end of x for V16
				continue;

			Data = (*XParam.Get_Emit_Data)(Address, NULL_LED, XParam.EmitPower);

			for(j=0; j<7-(Led&0x07); j+=1)
			{
				for(k=0; k<SkipAxis + XParam.Axis; k+=1)
				{
					RevTable[Index] = BOARD_SEL_LOGIC(0);

					if(AdMap) AdMap[Index] = INVALID_AD_MAP;

					if(SPI8Bits)
						((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
					else
						EmitTable[Index ++] = Data & 0x0FFFF;
				}
			}
		}
	}
}

//for type x only
void Gen_Switch_Emit_Rev_Table(uint32_t Flags, uint16_t *EmitTable, uint16_t *RevTable)
{
	int i,j,k;
	short RevLed, EmitLed;
	short Boards;
	short Address;
	uint16_t Index;
	uint32_t Data;
	uint32_t RevData, EmitData;
	int SkipAxis;
	//bool spiAdd = false;
	bool SkipFirstAxi = (Flags & 0x01) != 0;
	//bool DualSpiData = (Flags & 0x04) != 0;
	//bool ShutdownEmit = (Flags & 0x08) != 0;
	//bool SPI8Bits = (Flags & 0x10) != 0;
	//bool GPIOGain = (Flags & 0x20) != 0;
	bool DoubleScanBrdFirst = (Flags & 0x100) != 0;
	bool Anti = (Flags & 0x200) != 0;
	bool SwGPIO = (Flags & (1<<12)) != 0;

	Index = 0;

	if(Anti)	SkipAxis = ANTI_AXIS;
	else if(SkipFirstAxi) SkipAxis = 1;
	else SkipAxis = 0;

	Index = 0;
	//------------------------------- Switch Emit x & Rev--------------------------------------
	for (i=0; i<XParam.TotalLeds; i++)
	{
		FindBoardLed(&XParam, i, &RevLed, &Boards);
		Address = (YParam.Boards + 1 + Boards);

		RevData = (*XParam.Get_Rev_Data)(Address, RevLed, XParam.Gain[0]);

		for (j=0; j<XParam.Axis; j+=1)
		{
			FindBoardLed(&XParam, i + XParam.LedOff[j], &EmitLed, &Boards);

			if(EmitLed == NULL_LED)
			{
				FindBoardLed(&XParam, i + XParam.AuxLedOff[j], &EmitLed, &Boards);
			}

			Address = (1 + Boards);

			if(j==0)
			{
				EmitData = (*XParam.Get_Emit_Data)(Address, NULL_LED, XParam.EmitPower);

				for(k=0; k<SkipAxis; k+=1)
				{
					if(!SwGPIO) {
						Data = ((EmitData<<8) & 0xFF00) | (RevData & 0x00FF);
						EmitTable[Index ++] = Data & 0x0FFFF;
					} else {
						uint32_t GpioA = ((uint32_t *)EmitData)[0] | ((uint32_t *)RevData)[0];
						uint32_t GpioB = ((uint32_t *)EmitData)[1] | ((uint32_t *)RevData)[1];
						EmitTable[Index] = ((uint16_t *)&GpioA)[0];
						EmitTable[Index + 1] = ((uint16_t *)&GpioA)[1];
						RevTable[Index] = ((uint16_t *)&GpioB)[0];
						RevTable[Index + 1] = ((uint16_t *)&GpioB)[1];
						Index += 2;
					}
				}
			}

			EmitData = (*XParam.Get_Emit_Data)(Address, EmitLed, XParam.EmitPower);

			if(!SwGPIO) {
				Data = ((EmitData<<8) & 0xFF00) | (RevData & 0x00FF);
				EmitTable[Index ++] = Data & 0x0FFFF;
			} else {
				uint32_t GpioA = ((uint32_t *)EmitData)[0] | ((uint32_t *)RevData)[0];
				uint32_t GpioB = ((uint32_t *)EmitData)[1] | ((uint32_t *)RevData)[1];
				EmitTable[Index] = ((uint16_t *)&GpioA)[0];
				EmitTable[Index + 1] = ((uint16_t *)&GpioA)[1];
				RevTable[Index] = ((uint16_t *)&GpioB)[0];
				RevTable[Index + 1] = ((uint16_t *)&GpioB)[1];
				Index += 2;
			}
		}

		if(RevLed == 0 && DoubleScanBrdFirst) {
			int allAxis = XParam.Axis + SkipAxis;
			for(int k=0; k<allAxis; k+=1) {
				if(!SwGPIO) {
					EmitTable[Index] = EmitTable[Index - allAxis];
					Index ++;
				} else {
					EmitTable[Index] =  EmitTable[Index - allAxis*2];
					EmitTable[Index + 1] = EmitTable[Index - allAxis*2+1];
					RevTable[Index] = RevTable[Index - allAxis*2];
					RevTable[Index + 1] = RevTable[Index - allAxis*2 + 1];
					Index += 2;
				}
			}
		}
	}

	//------------------------------- Emit y --------------------------------------
	for (i=0; i<YParam.TotalLeds; i++)
	{
		FindBoardLed(&YParam, i, &RevLed, &Boards);
		Address = (1 + Boards);

		RevData = (*YParam.Get_Rev_Data)(Address, RevLed, YParam.Gain[0]);

		for (j=0; j<YParam.Axis; j+=1)
		{
			FindBoardLed(&YParam, i + YParam.LedOff[j], &EmitLed, &Boards);

			if(EmitLed == NULL_LED)
			{
				FindBoardLed(&YParam, i + YParam.AuxLedOff[j], &EmitLed, &Boards);
			}

			Address = (XParam.Boards + 1 + Boards);

			if(j==0)
			{
				EmitData = (*YParam.Get_Emit_Data)(Address, NULL_LED, YParam.EmitPower);

				for(k=0; k<SkipAxis; k+=1)
				{
					if(!SwGPIO) {
						EmitTable[Index ++] = (((EmitData<<8) & 0xFF00) | (RevData & 0x00FF)) & 0x0FFFF;
					} else {
						uint32_t GpioA = ((uint32_t *)EmitData)[0] | ((uint32_t *)RevData)[0];
						uint32_t GpioB = ((uint32_t *)EmitData)[1] | ((uint32_t *)RevData)[1];
						EmitTable[Index] = ((uint16_t *)&GpioA)[0];
						EmitTable[Index + 1] = ((uint16_t *)&GpioA)[1];
						RevTable[Index] = ((uint16_t *)&GpioB)[0];
						RevTable[Index + 1] = ((uint16_t *)&GpioB)[1];
						Index += 2;
					}
				}
			}

			EmitData = (*YParam.Get_Emit_Data)(Address, EmitLed, YParam.EmitPower);

			if(!SwGPIO) {      //spi switch
				EmitTable[Index ++] = (((EmitData<<8) & 0xFF00) | (RevData & 0x00FF)) & 0x0FFFF;
			} else {
				uint32_t GpioA = ((uint32_t *)EmitData)[0] | ((uint32_t *)RevData)[0];
				uint32_t GpioB = ((uint32_t *)EmitData)[1] | ((uint32_t *)RevData)[1];
				EmitTable[Index] = ((uint16_t *)&GpioA)[0];
				EmitTable[Index + 1] = ((uint16_t *)&GpioA)[1];
				RevTable[Index] = ((uint16_t *)&GpioB)[0];
				RevTable[Index + 1] = ((uint16_t *)&GpioB)[1];
				Index += 2;
			}
		}

		if(RevLed == 0 && DoubleScanBrdFirst) {
			int allAxis = XParam.Axis + SkipAxis;
			for(int k=0; k<allAxis; k+=1) {
				if(!SwGPIO) {
					EmitTable[Index] = EmitTable[Index - allAxis];
					Index ++;
				} else {
					EmitTable[Index] =  EmitTable[Index - allAxis*2];
					EmitTable[Index + 1] = EmitTable[Index - allAxis*2+1];
					RevTable[Index] = RevTable[Index - allAxis*2];
					RevTable[Index + 1] = RevTable[Index - allAxis*2 + 1];
					Index += 2;
				}
			}
		}
	}
}

void Gen_Switch_Rev_Table(uint32_t Flags, uint16_t *EmitTable, uint16_t *RevTable)
{
	int i,j;
	short Led;
	short LedToFind;
	uint16_t Data;
	short Boards;
	short Address;
	uint16_t Index;

	bool SkipFirstAxi = (Flags & 0x01) != 0;
	bool DualSpiData = (Flags & 0x04) != 0;
	bool ShutdownEmit = (Flags & 0x08) != 0;
	bool SPI8Bits = (Flags & 0x10) != 0;
	//bool GPIOGain = (Flags & 0x20) != 0;
	bool ReverseX = (Flags & 0x40) != 0;
	bool ReverseY = (Flags & 0x80) != 0;
	
	//------------------------------- Rev x --------------------------------------
	Index = 0;
	for (i=0; i<XParam.TotalLeds; i++)
	{
		for (j=0; j<XParam.Axis; j+=1)
		{
			if(ReverseX)
				LedToFind = XParam.TotalLeds - 1 - i - XParam.LedOff[j];
			else
				LedToFind = i + XParam.LedOff[j];

			FindBoardLed(&XParam, LedToFind, &Led, &Boards);

			if(Led == NULL_LED && XParam.MiniOff > 1)    //multi_power group, switch power group is forbidden！
			{
				if(ReverseX)
					LedToFind = XParam.TotalLeds - 1 - i;
				else
					LedToFind = i;

				FindBoardLed(&XParam, LedToFind, &Led, &Boards);
			}

			Address = (YParam.Boards + 1 + Boards);

			if(j==0 && SkipFirstAxi)
			{
				Data = (*XParam.Get_Rev_Data)(Address, Led, XParam.Gain[j]);
				if(DualSpiData)	Data = ConvertData(Data);

				//RevTable[Index++] = Data;
				if(SPI8Bits)
					((uint8_t *)RevTable)[Index ++] = Data & 0x0FF;
				else
					RevTable[Index++] = Data & 0x0FFFF;
			}

			Data = (*XParam.Get_Rev_Data)(Address, Led, XParam.Gain[j]);
			if(DualSpiData)	Data = ConvertData(Data);

			//RevTable[Index++] = Data;
			if(SPI8Bits)
				((uint8_t *)RevTable)[Index ++] = Data & 0x0FF;
			else
				RevTable[Index++] = Data & 0x0FFFF;
		}
	}

//------------------------------- Rev y --------------------------------------
	for (i=0; i<YParam.TotalLeds; i++)
	{
		for (j=0; j<YParam.Axis; j+=1)
		{
			if(ReverseY)
				LedToFind = YParam.TotalLeds - 1 - i - YParam.LedOff[j];
			else
				LedToFind = i + YParam.LedOff[j];

			FindBoardLed(&YParam, LedToFind, &Led, &Boards);

			if(Led == NULL_LED && XParam.MiniOff > 1)
			{
				if(ReverseY)
					LedToFind = YParam.TotalLeds - 1 - i;
				else
					LedToFind = i;

				FindBoardLed(&YParam, LedToFind, &Led, &Boards);
			}

			Address = (1 + Boards);

			if(j==0 && SkipFirstAxi)
			{
				Data = (*YParam.Get_Rev_Data)(Address, Led, YParam.Gain[j]);
				if(DualSpiData)	Data = ConvertData(Data);

				//RevTable[Index++] = Data;
				if(SPI8Bits)
					((uint8_t *)RevTable)[Index ++] = Data & 0x0FF;
				else
					RevTable[Index++] = Data & 0x0FFFF;
			}

			Data = (*YParam.Get_Rev_Data)(Address, Led, YParam.Gain[j]);
			if(DualSpiData)	Data = ConvertData(Data);

			//RevTable[Index++] = Data;
			if(SPI8Bits)
				((uint8_t *)RevTable)[Index ++] = Data & 0x0FF;
			else
				RevTable[Index++] = Data & 0x0FFFF;
		}
	}

	if(ShutdownEmit)
	{
		Data = (*YParam.Get_Rev_Data)(Address, NULL_LED, YParam.Gain[0]);
		if(DualSpiData)	Data = ConvertData(Data);

		for (j=0; j<YParam.Axis; j+=1)
		{
			if(SPI8Bits)
				((uint8_t *)RevTable)[Index ++] = Data & 0x0FF;
			else
				RevTable[Index++] = Data & 0x0FFFF;
		}

		if(SkipFirstAxi)
		{
			if(SPI8Bits)
				((uint8_t *)RevTable)[Index ++] = Data & 0x0FF;
			else
				RevTable[Index++] = Data & 0x0FFFF;
		}
	}
//------------------------------- Emit x --------------------------------------
	Index = 0;
	for (i=0; i<XParam.TotalLeds; i++)
	{
		if(ReverseX)
			LedToFind = XParam.TotalLeds - 1 - i;
		else
			LedToFind = i;

		FindBoardLed(&XParam, LedToFind, &Led, &Boards);
		Address = (1 + Boards);

		Data = (*XParam.Get_Emit_Data)(Address, Led, XParam.EmitPower);
		if(DualSpiData)	Data = ConvertData(Data);

		//EmitTable[Index ++] = Data;
		if(SPI8Bits)
			((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
		else
			EmitTable[Index ++] = Data & 0x0FFFF;
	}

//------------------------------- Emit y --------------------------------------
	for (i=0; i<YParam.TotalLeds; i++)
	{
		if(ReverseY)
			LedToFind = YParam.TotalLeds - 1 - i;
		else
			LedToFind = i;

		FindBoardLed(&YParam, LedToFind, &Led, &Boards);
		Address = (XParam.Boards + 1 + Boards);

		Data = (*YParam.Get_Emit_Data)(Address, Led, YParam.EmitPower);
		if(DualSpiData)	Data = ConvertData(Data);

		//EmitTable[Index ++] = Data;
		if(SPI8Bits)
			((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
		else
			EmitTable[Index ++] = Data & 0x0FFFF;
	}

	if(ShutdownEmit)
	{
		Data = (*YParam.Get_Emit_Data)(Address, NULL_LED, YParam.EmitPower);
		if(DualSpiData)	Data = ConvertData(Data);

    	if(SPI8Bits)
			((uint8_t *)EmitTable)[Index ++] = Data & 0x0FF;
		else
			EmitTable[Index ++] = Data & 0x0FFFF;
	}
}

//void Gen_Reorder_Ad_Mapping_Table(bool ScanXFirst, uint32_t Flags, uint16_t *MapTable)
//{
//	bool SkipLastY = (Flags & 0x2000) != 0;
//	bool Skipx = (Flags & 0x4000) != 0;
//	bool SkipFirstAxi = (Flags & 0x01) != 0;
//	bool ShutdownEmit = (Flags & 0x08) != 0;
//	bool DoubleScanBrdFirst = (Flags & 0x100) != 0;
//	bool Anti = (Flags & 0x200) != 0;
//
//	uint16_t Startx;
//	uint16_t Starty;
//	uint16_t XLeds = XParam.TotalLeds;
//	uint16_t YLeds = YParam.TotalLeds;
//	uint16_t XAxis = XParam.Axis;
//	uint16_t YAxis = YParam.Axis;
//
//	if(ScanXFirst)
//	{
//		Startx = 0;
//		Starty = XLeds * XAxis;
//	}
//	else
//	{
//		Startx = YLeds * YAxis;
//		Starty = 0;
//	}
//
//	
//	for (int i=0; i<XParam.TotalLeds; i++)
//	{
//		
//		if(DoubleScanBrdFirst && CheckBoardStartLed(&XParam, i) == true)
//		{
//
//		}
//	}
//
//	//if()
//}

int Gen_DMATable_c(uint32_t Flags, SCAN_TABLE_HEAD *sth, uint32_t TabBaseAdd, uint8_t *Tab)
{
	int i,j;
	int led;
	int gain;
	uint8_t temp;
	uint32_t Offset;
	int16_t *iTemp;
	float *fTemp;
	uint8_t *ctemp;
	uint16_t *utemp;
	bool SwEmit = (Flags & (1<<1)) != 0;
	bool SwEmitRev = (Flags & (1<<11)) != 0;
	bool SkipLastY = (Flags & 0x2000) != 0;
	bool Skipx = (Flags & 0x4000) != 0;
	bool AdMap = (Flags & 0x8000) != 0;
	bool WaitRxPowerOn = (Flags & (1<<17)) != 0;

//------------------------------- Offsets -----------------------------------
	Offset = (uint32_t)(sth->XOffsets) - TabBaseAdd;
	iTemp = (int16_t *)(Tab + Offset);
	for(int i=0; i<XParam.Axis; i+=1)
	{
		iTemp[i] = XParam.LedOff[i];
	}

	Offset = (uint32_t)(sth->YOffsets) - TabBaseAdd;
	iTemp = (int16_t *)(Tab + Offset);
	for(int i=0; i<YParam.Axis; i+=1)
	{
		iTemp[i] = YParam.LedOff[i];
	}

	Offset = (uint32_t)(sth->Offsets) - TabBaseAdd;
	iTemp = (int16_t *)(Tab + Offset);
	for(int i=0; i<YParam.Axis + XParam.Axis; i+=1)
	{
		if(i<XParam.Axis)
			iTemp[i] = XParam.LedOff[i];
		else
			iTemp[i] = YParam.LedOff[i-XParam.Axis];
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

	Offset = (uint32_t)(sth->Tan) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<YParam.Axis + XParam.Axis; i+=1)
	{
		if(i<XParam.Axis)
			fTemp[i] = XParam.Tan[i];
		else
			fTemp[i] = YParam.Tan[i-XParam.Axis];
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

	Offset = (uint32_t)(sth->CoTan) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<YParam.Axis + XParam.Axis; i+=1)
	{
		if(i<XParam.Axis)
		{
			if(XParam.Tan[i]!=0)
				fTemp[i] = 1/XParam.Tan[i];
			else
				fTemp[i] = 0;
		}
		else
		{
			if(YParam.Tan[i-XParam.Axis]!=0)
				fTemp[i] = 1/YParam.Tan[i-XParam.Axis];
			else
				fTemp[i] = 0;
		}
	}

//------------------------------- Len -----------------------------------
	Offset = (uint32_t)(sth->XOffLen) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<XParam.Axis; i+=1)
	{
		fTemp[i] = XParam.LenOff[i];
	}

	Offset = (uint32_t)(sth->YOffLen) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<YParam.Axis; i+=1)
	{
		fTemp[i] = YParam.LenOff[i];
	}

	Offset = (uint32_t)(sth->OffLen) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<YParam.Axis + XParam.Axis; i+=1)
	{
		if(i<XParam.Axis)
			fTemp[i] = XParam.LenOff[i];
		else
			fTemp[i] = YParam.LenOff[i-XParam.Axis];
	}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------- Aux Offsets ----------------------------------
	Offset = (uint32_t)(sth->AuxXOffsets) - TabBaseAdd;
	iTemp = (int16_t *)(Tab + Offset);
	for(int i=0; i<XParam.Axis; i+=1)
	{
		iTemp[i] = XParam.AuxLedOff[i];
	}

	Offset = (uint32_t)(sth->AuxYOffsets) - TabBaseAdd;
	iTemp = (int16_t *)(Tab + Offset);
	for(int i=0; i<YParam.Axis; i+=1)
	{
		iTemp[i] = YParam.AuxLedOff[i];
	}

	Offset = (uint32_t)(sth->AuxOffsets) - TabBaseAdd;
	iTemp = (int16_t *)(Tab + Offset);
	for(int i=0; i<YParam.Axis + XParam.Axis; i+=1)
	{
		if(i<XParam.Axis)
			iTemp[i] = XParam.AuxLedOff[i];
		else
			iTemp[i] = YParam.AuxLedOff[i-XParam.Axis];
	}

//------------------------------ Aux Tan -----------------------------------
	Offset = (uint32_t)(sth->AuxXTan) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<XParam.Axis; i+=1)
	{
		fTemp[i] = XParam.AuxTan[i];
	}

	Offset = (uint32_t)(sth->AuxYTan) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<YParam.Axis; i+=1)
	{
		fTemp[i] = YParam.AuxTan[i];
	}

	Offset = (uint32_t)(sth->AuxTan) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<YParam.Axis + XParam.Axis; i+=1)
	{
		if(i<XParam.Axis)
			fTemp[i] = XParam.AuxTan[i];
		else
			fTemp[i] = YParam.AuxTan[i-XParam.Axis];
	}

//-------------------------------Aux CoTan -----------------------------------
	Offset = (uint32_t)(sth->AuxXCoTan) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<XParam.Axis; i+=1)
	{
		if(XParam.AuxTan[i]!=0)
			fTemp[i] = 1/XParam.AuxTan[i];
		else
			fTemp[i] = 0;
	}

	Offset = (uint32_t)(sth->AuxYCoTan) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<YParam.Axis; i+=1)
	{
		if(YParam.AuxTan[i]!=0)
			fTemp[i] = 1/YParam.AuxTan[i];
		else
			fTemp[i] = 0;
	}

	Offset = (uint32_t)(sth->AuxCoTan) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<YParam.Axis + XParam.Axis; i+=1)
	{
		if(i<XParam.Axis)
		{
			if(XParam.AuxTan[i]!=0)
				fTemp[i] = 1/XParam.AuxTan[i];
			else
				fTemp[i] = 0;
		}
		else
		{
			if(YParam.AuxTan[i-XParam.Axis]!=0)
				fTemp[i] = 1/YParam.AuxTan[i-XParam.Axis];
			else
				fTemp[i] = 0;
		}
	}

//----------------------------- Aux Len -----------------------------------
	Offset = (uint32_t)(sth->AuxXOffLen) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<XParam.Axis; i+=1)
	{
		fTemp[i] = XParam.AuxLenOff[i];
	}

	Offset = (uint32_t)(sth->AuxYOffLen) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<YParam.Axis; i+=1)
	{
		fTemp[i] = YParam.AuxLenOff[i];
	}

	Offset = (uint32_t)(sth->AuxOffLen) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<YParam.Axis + XParam.Axis; i+=1)
	{
		if(i<XParam.Axis)
			fTemp[i] = XParam.AuxLenOff[i];
		else
			fTemp[i] = YParam.AuxLenOff[i-XParam.Axis];
	}

/////////////////////////////////////////////////////////////////////////////////
/////////////////// Mix Scan Param //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

	//------------------------------- Offsets -----------------------------------
	Offset = (uint32_t)(sth->MixXLedOff) - TabBaseAdd;
	iTemp = (int16_t *)(Tab + Offset);
	for(int i=0; i<XParam.MixAxis; i+=1)
	{
		iTemp[i] = XParam.MixLedOff[i];
	}

	Offset = (uint32_t)(sth->MixYLedOff) - TabBaseAdd;
	iTemp = (int16_t *)(Tab + Offset);
	for(int i=0; i<YParam.MixAxis; i+=1)
	{
		iTemp[i] = YParam.MixLedOff[i];
	}

	Offset = (uint32_t)(sth->MixLedOff) - TabBaseAdd;
	iTemp = (int16_t *)(Tab + Offset);
	for(int i=0; i<YParam.MixAxis + XParam.MixAxis; i+=1)
	{
		if(i<XParam.MixAxis)
			iTemp[i] = XParam.MixLedOff[i];
		else
			iTemp[i] = YParam.MixLedOff[i-XParam.MixAxis];
	}

//------------------------------- Tan -----------------------------------
	Offset = (uint32_t)(sth->MixXTan) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<XParam.MixAxis; i+=1) {
		fTemp[i] = XParam.MixTan[i];
	}

	Offset = (uint32_t)(sth->MixYTan) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<YParam.MixAxis; i+=1) {
		fTemp[i] = YParam.MixTan[i];
	}

	Offset = (uint32_t)(sth->MixTan) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<YParam.MixAxis + XParam.MixAxis; i+=1) {
		if(i<XParam.MixAxis)
			fTemp[i] = XParam.MixTan[i];
		else
			fTemp[i] = YParam.MixTan[i-XParam.MixAxis];
	}

//------------------------------- Cos -----------------------------------
	Offset = (uint32_t)(sth->MixXCos) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<XParam.MixAxis; i+=1) {
		fTemp[i] = XParam.MixCos[i];
	}

	Offset = (uint32_t)(sth->MixYCos) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<YParam.MixAxis; i+=1) {
		fTemp[i] = YParam.MixCos[i];
	}

	Offset = (uint32_t)(sth->MixCos) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<YParam.MixAxis + XParam.MixAxis; i+=1) {
		if(i<XParam.MixAxis)
			fTemp[i] = XParam.MixCos[i];
		else
			fTemp[i] = YParam.MixCos[i-XParam.MixAxis];
	}

//-------------------------------AxisId -----------------------------------
	Offset = (uint32_t)(sth->MixXAxisId) - TabBaseAdd;
	ctemp = (uint8_t *)(Tab + Offset);
	for(int i=0; i<XParam.MixAxis; i+=1) {
		ctemp[i] = XParam.MixAxisId[i];
	}

	Offset = (uint32_t)(sth->MixYAxisId) - TabBaseAdd;
	ctemp = (uint8_t *)(Tab + Offset);
	for(int i=0; i<YParam.MixAxis; i+=1) {
		ctemp[i] = YParam.MixAxisId[i] + XParam.Axis;
	}

	Offset = (uint32_t)(sth->MixAxisId) - TabBaseAdd;
	ctemp = (uint8_t *)(Tab + Offset);
	for(int i=0; i<YParam.MixAxis + XParam.MixAxis; i+=1) {
		if(i<XParam.MixAxis)
			ctemp[i] = XParam.MixAxisId[i];
		else
			ctemp[i] = YParam.MixAxisId[i-XParam.MixAxis] + XParam.Axis;;
	}
//-------------------------------MixXStartLed -----------------------------------
	Offset = (uint32_t)(sth->MixXStartLed) - TabBaseAdd;
	utemp = (uint16_t *)(Tab + Offset);
	for(int i=0; i<XParam.MixAxis; i+=1) {
		utemp[i] = XParam.MixStartLed[i];
	}

	Offset = (uint32_t)(sth->MixYStartLed) - TabBaseAdd;
	utemp = (uint16_t *)(Tab + Offset);
	for(int i=0; i<YParam.MixAxis; i+=1) {
		utemp[i] = YParam.MixStartLed[i];
	}

	Offset = (uint32_t)(sth->MixStartLed) - TabBaseAdd;
	utemp = (uint16_t *)(Tab + Offset);
	for(int i=0; i<YParam.MixAxis + XParam.MixAxis; i+=1) {
		if(i<XParam.MixAxis)
			utemp[i] = XParam.MixStartLed[i];
		else
			utemp[i] = YParam.MixStartLed[i-XParam.MixAxis];
	}

//-------------------------------MixXStartLed -----------------------------------
	Offset = (uint32_t)(sth->MixXEndLed) - TabBaseAdd;
	utemp = (uint16_t *)(Tab + Offset);
	for(int i=0; i<XParam.MixAxis; i+=1) {
		utemp[i] = XParam.MixEndLed[i];
	}

	Offset = (uint32_t)(sth->MixYEndLed) - TabBaseAdd;
	utemp = (uint16_t *)(Tab + Offset);
	for(int i=0; i<YParam.MixAxis; i+=1) {
		utemp[i] = YParam.MixEndLed[i];
	}

	Offset = (uint32_t)(sth->MixEndLed) - TabBaseAdd;
	utemp = (uint16_t *)(Tab + Offset);
	for(int i=0; i<YParam.MixAxis + XParam.MixAxis; i+=1) {
		if(i<XParam.MixAxis)
			utemp[i] = XParam.MixEndLed[i];
		else
			utemp[i] = YParam.MixEndLed[i-XParam.MixAxis];
	}

/////////////////////////////////////////////////////////////////////////////////
/////////////////// Soft Scan Param //////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
	Offset = (uint32_t)(sth->ScXTan) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<= XParam.MaxOff; i+=1) {
		fTemp[i] = i*XParam.Dis / XParam.Len;
	}

	Offset = (uint32_t)(sth->ScXCoTan) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	fTemp[0] = 0;
	for(int i=1; i<= XParam.MaxOff; i+=1) {
		fTemp[i] = XParam.Len /(i*XParam.Dis);
	}

	Offset = (uint32_t)(sth->ScXCos) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<= XParam.MaxOff; i+=1) {
		fTemp[i] = sqrt(1/(1+(i*XParam.Dis/XParam.Len) * (i*XParam.Dis/XParam.Len)));
	}

	Offset = (uint32_t)(sth->ScXSin) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	fTemp[0] = 0;
	for(int i=1; i<= XParam.MaxOff; i+=1) {
		fTemp[i] = sqrt(1/(1+ XParam.Len * XParam.Len/(i*XParam.Dis) /(i*XParam.Dis)));
	}

	//y
	Offset = (uint32_t)(sth->ScYTan) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<= YParam.MaxOff; i+=1) {
		fTemp[i] = i*YParam.Dis / YParam.Len;
	}

	Offset = (uint32_t)(sth->ScYCoTan) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	fTemp[0] = 0;
	for(int i=1; i<= YParam.MaxOff; i+=1) {
		fTemp[i] = YParam.Len /(i*YParam.Dis);
	}

	Offset = (uint32_t)(sth->ScYCos) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	for(int i=0; i<= YParam.MaxOff; i+=1) {
		fTemp[i] = sqrt(1/(1+(i*YParam.Dis/YParam.Len) * (i*YParam.Dis/YParam.Len)));
	}

	Offset = (uint32_t)(sth->ScYSin) - TabBaseAdd;
	fTemp = (float *)(Tab + Offset);
	fTemp[0] = 0;
	for(int i=1; i<= YParam.MaxOff; i+=1) {
		fTemp[i] = sqrt(1/(1+ YParam.Len * YParam.Len/(i*YParam.Dis) /(i*YParam.Dis)));
	}

 /////////////////////////////////////////////////////////////////////////
//------------------------- Gain Setting ----------------------------------
	Offset = (uint32_t)(sth->XGainSet) - TabBaseAdd;
	for(int i=0; i<XParam.Axis; i+=1)
	{
		((uint8_t *)(Tab + Offset))[i] = XParam.Gain[i];
	}

	Offset = (uint32_t)(sth->YGainSet) - TabBaseAdd;
	for(int i=0; i<YParam.Axis; i+=1)
	{
		((uint8_t *)(Tab + Offset))[i] = YParam.Gain[i];
	}

//------------------------- scan table ------------------------------------
	uint32_t ETabOff = (uint32_t)(sth->EmitTab) - TabBaseAdd;
	uint32_t RTabOff = (uint32_t)(sth->RevTab) - TabBaseAdd;
	uint32_t V15SPIOff = (uint32_t)(sth->V15EmitGroupTable) - TabBaseAdd;
	//uint32_t METabOff = (uint32_t)(sth->MainEmitTab) - TabBaseAdd;
	//uint32_t MRTabOff = (uint32_t)(sth->MainRevTab) - TabBaseAdd;

//------------------------- Jump Led table --------------------------------
	Offset = (uint32_t)(sth->JumpLedsTable) - TabBaseAdd;
	iTemp = (int16_t *)(Tab + Offset);

	if(XParam.Get_Rev_Data == NULL || YParam.Get_Rev_Data == NULL || XParam.Get_Emit_Data == Get_Emit_Data_PTV150)
	{
		int16_t StartLed = 0;
		int16_t Skips = 0;

		for(int i=0; i<YParam.Boards; i+=1)
		{
			if(WaitRxPowerOn){
				for(int j=0; j<YParam.BoradLeds[i]; j+=8) {
					iTemp[Skips++] = StartLed + j;
					StartLed ++;
				}
			}

			StartLed += YParam.BoradLeds[i];

			if(SkipLastY && i == YParam.Boards-1)    //
			{
				StartLed -= 1;
				iTemp[Skips++] = StartLed ++;
			}

			if((YParam.BoradLeds[i] & 0x07) != 0)
			{
				for(j=0; j<8-(YParam.BoradLeds[i] & 0x07); j+=1)
				{
					iTemp[Skips++] = StartLed ++;
				}
			}
		}

		for(int i=0; i<XParam.Boards; i+=1)
		{
			if(WaitRxPowerOn) {
				for(int j=0; j<XParam.BoradLeds[i]; j+=8) {
					iTemp[Skips++] = StartLed + j;
					StartLed ++;
				}
			}

			StartLed += XParam.BoradLeds[i];

			if(i<XParam.Boards-1) {
				if((XParam.BoradLeds[i] & 0x07) != 0) {
					for(j=0; j<8-(XParam.BoradLeds[i] & 0x07); j+=1) {
						iTemp[Skips++] = StartLed ++;
					}
				}
			} else if(Skipx) {
				if((XParam.BoradLeds[XParam.Boards-1] & 0x07) != 0) {
					for(j=0; j<8-(XParam.BoradLeds[XParam.Boards-1] & 0x07); j+=1)
					{
						iTemp[Skips++] = StartLed ++;
					}
				} else if(XParam.Get_Emit_Data == Get_Emit_Data_PDV1334 || XParam.Get_Emit_Data == Get_Emit_Data_PDV1335) {
					if(WaitRxPowerOn){
						iTemp[Skips++] = StartLed++;
					}

					for(j=0; j<8; j+=1) {
						iTemp[Skips++] = StartLed ++;
					}
				}
			}
		}
	}
	
//------------------------- V15 Receive Switch --------------------------------
	const uint16_t V15_HC4051_REMAP[8] = {1 , 3, 7, 5, 4, 0, 6, 2};

	if(XParam.Get_Emit_Data == Get_Emit_Data_PTV150) {
		uint32_t *RevTab = (uint32_t *)(Tab + RTabOff);
		uint32_t ptr = 0;
		for(int i=0; i<8; i+=1){
			uint32_t Data = HC4051_ABC(V15_HC4051_REMAP[i]);
			for(int j=0; j<sth->RevTabLen/8; j+=4){
				RevTab[ptr ++] = Data;
			}
		}

		RTabOff = V15SPIOff;		//fixme!
	}

//------------------------- Reorder ad value mapping table --------------------------------
	uint32_t ReorderMapOff = (uint32_t)(sth->ReorderAdMap) - TabBaseAdd;

	gpio_t gpio;

	gpio.GPIOA = (uint32_t *)(Tab + ((uint32_t)(sth->GPIOA) - TabBaseAdd));
	gpio.GPIOB = (uint32_t *)(Tab + ((uint32_t)(sth->GPIOB) - TabBaseAdd));
	gpio.GPIOC = (uint32_t *)(Tab + ((uint32_t)(sth->GPIOC) - TabBaseAdd));
	gpio.GPIOD = (uint32_t *)(Tab + ((uint32_t)(sth->GPIOD) - TabBaseAdd));

	uint16_t *AdMapTab = NULL;

	if(AdMap) {
		AdMapTab = (uint16_t *)(Tab + ReorderMapOff);
	}

	if(SwEmit)
	{
		if(XParam.Get_Emit_Data == Get_Emit_Data_PDV133 || XParam.Get_Emit_Data == Get_Emit_Data_PDV1331 ||
		   XParam.Get_Emit_Data == Get_Emit_Data_PDV1333 || XParam.Get_Emit_Data == Get_Emit_Data_PDV1334 ||
		   XParam.Get_Emit_Data == Get_Emit_Data_PDV1335) {
			Gen_Switch_Emit_GPIO(Flags, (uint16_t *)(Tab + ETabOff), (uint16_t *)(Tab + RTabOff), &gpio, AdMapTab);
		} else if(XParam.Get_Emit_Data == Get_Emit_Data_PTV150) {
			Gen_Switch_Emit_Only_Table_V15(Flags, (uint16_t *)(Tab + ETabOff), (uint16_t *)(Tab + RTabOff), AdMapTab);
		} else if(XParam.Get_Rev_Data == NULL || YParam.Get_Rev_Data == NULL) {
			Gen_Switch_Emit_Only_Table(Flags, (uint16_t *)(Tab + ETabOff), (uint16_t *)(Tab + RTabOff), AdMapTab);
		} else {
			Gen_Switch_Emit_Table(Flags, (uint16_t *)(Tab + ETabOff), (uint16_t *)(Tab + RTabOff), AdMapTab);
		}
		//Gen_Switch_Emit_Table(Flags, (uint16_t *)(Tab + METabOff), (uint16_t *)(Tab + MRTabOff),true);
	} else if(SwEmitRev) {
		Gen_Switch_Emit_Rev_Table(Flags, (uint16_t *)(Tab + ETabOff), (uint16_t *)(Tab + RTabOff));
	} else {
		Gen_Switch_Rev_Table(Flags, (uint16_t *)(Tab + ETabOff), (uint16_t *)(Tab + RTabOff));
		//Gen_Switch_Rev_Table(Flags, (uint16_t *)(Tab + METabOff), (uint16_t *)(Tab + MRTabOff),true);
	}

//	if(AdMap) {
//		FILE *fp;
//
//		if((fp = fopen("map.txt", "w")) != NULL)
//		{
//			fprintf(fp, "%d\n", sth->ReorderAdMapLen/2);
//
//			for(int i=0; i<sth->ReorderAdMapLen/2; i+=1)
//			{
//				fprintf(fp, "%.4d, ", ((uint16_t *)(Tab + ReorderMapOff))[i]);
//
//				if(i % (XParam.Axis +1)  == XParam.Axis)
//				{
//                	fprintf(fp, "\n");
//				}
//			}
//
//			fclose(fp);
//		}
//	}

    return 0;
}

#pragma package(smart_init)
