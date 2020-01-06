#ifndef FSC_PUB_H
#define FSC_PUB_H

#include <QMainWindow>


#define XUNYIN_SET_KF_NUM                   19


#define XUNYIN_SET_FLOW_RANGE_NUM           10
#define XUNYIN_SET_ZERO_CAL_NUM             2

#define XUNYIN_SET_FLOW_RANGE_FLOW_MIN                  0
#define XUNYIN_SET_FLOW_RANGE_FLOW_MAX                  1

#define XUNYIN_SET_FLOW_RANGE_FLOW_METER_MIN            2
#define XUNYIN_SET_FLOW_RANGE_FLOW_METER_MAX            3

#define XUNYIN_SET_FLOW_RANGE_FLOW_RATE_MIN             4
#define XUNYIN_SET_FLOW_RANGE_FLOW_RATE_MAX             5

#define XUNYIN_SET_FLOW_RANGE_FLOW_RATE_CUTOFF          6

#define XUNYIN_SET_FLOW_RANGE_FLOW_FREQENCY_MIN         7
#define XUNYIN_SET_FLOW_RANGE_FLOW_FREQENCY_MAX         8

class cls_fmData
{
public:
    char        fm_valueGAIN_CONTROL;

    int         fm_valu_read_valid;

    float       fm_valueSET_KF;
    float       fm_valueSET_KF1[XUNYIN_SET_KF_NUM];
    float       fm_valueSET_KF2[XUNYIN_SET_KF_NUM];

    float       fm_valueFLOW_RANGE[XUNYIN_SET_FLOW_RANGE_NUM];

    float       fm_valueZERO_CAL[XUNYIN_SET_ZERO_CAL_NUM];


    uint16_t    fm_0x80_GAP_PLS_ADC_START = 0x80;
    uint16_t    fm_0x81_NUM_PLS = 0x81;
    uint16_t    fm_0x82_GAP_UPS_DNS = 0x82;
    uint16_t    fm_0x83_GAP_UPS_UPS = 0x83;
    uint8_t     fm_0x84_GAIN_CONTROL = 1;
    float       fm_0x85_METER_CONSTANT = static_cast<float>(0.1);
    uint16_t    fm_0x86_XT2_FREQ = 0x86;
    uint16_t    fm_0x87_ADC_SAMP_FREQ = 0x87;
    uint16_t    fm_0x88_SIG_SAMP_FREQ = 0x88;
    uint8_t     fm_0x89_ADC_OVERSAMPLING = 2;
    uint32_t    fm_0x8A_DELTA_TOF_OFFSET = 0x123456;
    float       fm_0x8B_ABS_TOF_ADDITIONAL = static_cast<float>(0.2);
    uint16_t    fm_0x8C_CAPTURE_DURATION = 0x8c;
    int32_t     fm_0x8D_PARAM1 = 0x123457;
    int32_t     fm_0x8E_PARAM2 = 0x123458;
    int32_t     fm_0x8F_PARAM3 = 0x123459;
    int32_t     fm_0x90_PARAM4 = 0x123460;
    int32_t     fm_0x9E_PARAM5 = 0x123461;
    int32_t     fm_0x9F_PARAM6 = 0x123462;
    int32_t     fm_0x91_PARAM7 = 0x123463;
    int32_t     fm_0x92_PARAM8 = 0x123464;
    int32_t     fm_0x93_PARAM9 = 0x123465;
    int32_t     fm_0x94_PARAM10 = 0x123466;
    int16_t     fm_0x96_APP_UPDATE_REQ = 0x96;

    uint16_t    fm_0x98_APP_REQ_CAPTURES1[125];
    int16_t     fm_0x99_APP_VERSION = 0x99;
    uint16_t    fm_0x9B_F1_F2[2];
    int16_t     fm_0xe0_AUTO_GAIN_ENABLE = 0xe0;
    float       fm_0xe1_FLOW_RANGE[10];
    uint8_t     fm_0xe2_RUNNING_AVG = 3;
    uint16_t    fm_0xe3_SET_DAC[2];
    float       fm_0xe4_SET_KF = static_cast<float>(0.3);
    float       fm_0xe5_SET_KF1[10];
    float       fm_0xe6_SET_KF2[10];


    uint16_t    fm_0xe7_SET_SNSR_TYP = 0xe7;
    int16_t     fm_0xe8_LNCAL_EN = 0xe8;
    int16_t     fm_0xe9_STA_UP = 0xe9;
    int16_t     fm_0xeA_RESET = 0xea;
    float       fm_0xeB_ZERO_CAL[2];
    int8_t      fm_0xeC_AUTO_GAIN_UP = 4;
    int8_t      fm_0xeD_ADDR = 5;
    uint16_t    fm_0xef_APP_REQ_CAPTURES2[125];
    uint16_t    fm_0xf0_APP_REQ_CAPTURES3[125];
    uint16_t    fm_0xf1_APP_REQ_CAPTURES4[125];

    float       fm_0x97_APP_REQ_DATA[54];


};

#endif // FSC_PLC_H
