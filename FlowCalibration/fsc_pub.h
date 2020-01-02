#ifndef FSC_PUB_H
#define FSC_PUB_H

#define XUNYIN_SET_KF_NUM                   19

class cls_fmData
{
public:
    char        fm_valueGAIN_CONTROL;
    int         fm_valu_read_valid;
    float       fm_valueSET_KF;
    int         fm_valueSET_KF_valid;
    float       fm_valueSET_KF1[XUNYIN_SET_KF_NUM];
    float       fm_valueSET_KF2[XUNYIN_SET_KF_NUM];
};

#endif // FSC_PLC_H
