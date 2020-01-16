// ---------------------------------------------------------------------------

#pragma hdrstop

#include <string.h>
#include "untBeatCode.h"

// ---------------------------------------------------------------------------

#pragma package(smart_init)

//////////////////////////////////////////////////////////////////////////
// Code starts from Line 130
//////////////////////////////////////////////////////////////////////////
bool Des_Go(unsigned char *Out, unsigned char *In, long datalen,
	const unsigned char *Key, int keylen, bool Type) {
	long i, j;

	if (!(Out && In && Key && (datalen = (datalen + 7) & 0xfffffff8) != 0))
		return false;
	SetKey(Key, keylen);
	if (!Is3DES) { // 1次DES
		for (i = 0, j = datalen >> 3; i < j; ++i, Out += 8, In += 8)
			DES(Out, In, &SubKey[0], Type);
	}
	else { // 3次DES 加密:加(key0)-解(key1)-加(key0) 解密::解(key0)-加(key1)-解(key0)
		for (i = 0, j = datalen >> 3; i < j; ++i, Out += 8, In += 8) {
			DES(Out, In, &SubKey[0], Type);
			DES(Out, Out, &SubKey[1], !Type);
			DES(Out, Out, &SubKey[0], Type);
		}
	}
	return true;
}

void SetKey(const unsigned char* Key, int len) {
	memset(deskey, 0, 16);
	memcpy(deskey, Key, len > 16 ? 16 : len);
	SetSubKey(&SubKey[0], &deskey[0]);
	Is3DES = len > 8 ? (SetSubKey(&SubKey[1], &deskey[8]), true) : false;
}

void DES(char unsigned Out[8], char unsigned In[8], const PSubKey pSubKey,
	bool Type) {
	int i;
	static bool M[64], tmp[32], *Li = &M[0], *Ri = &M[32];

	ByteToBit(M, In, 64);
	Transform(M, M, IP_Table, 64);
	if (Type == ENCRYPT) {
		for (i = 0; i < 16; ++i) {
			memcpy(tmp, Ri, 32);
			F_func(Ri, (*pSubKey)[i]);
			Xor(Ri, Li, 32);
			memcpy(Li, tmp, 32);
		}
	}
	else {
		for (i = 15; i >= 0; --i) {
			memcpy(tmp, Li, 32);
			F_func(Li, (*pSubKey)[i]);
			Xor(Li, Ri, 32);
			memcpy(Ri, tmp, 32);
		}
	}
	Transform(M, M, IPR_Table, 64);
	BitToByte(Out, M, 64);
}

void SetSubKey(PSubKey pSubKey, const unsigned char Key[8]) {
	int i;
	static bool K[64], *KL = &K[0], *KR = &K[28];

	ByteToBit(K, Key, 64);
	Transform(K, K, PC1_Table, 56);
	for (i = 0; i < 16; ++i) {
		RotateL(KL, 28, LOOP_Table[i]);
		RotateL(KR, 28, LOOP_Table[i]);
		Transform((*pSubKey)[i], K, PC2_Table, 48);
	}
}

void F_func(bool In[32], bool Ki[48]) {
	static bool MR[48];
	Transform(MR, In, E_Table, 48);
	Xor(MR, Ki, 48);
	S_func(In, MR);
	Transform(In, In, P_Table, 32);
}

void S_func(bool Out[32], bool In[48]) {
	unsigned char i, j, k;

	for (i = 0; i < 8; ++i, In += 6, Out += 4) {
		j = (In[0] << 1) + In[5];
		k = (In[1] << 3) + (In[2] << 2) + (In[3] << 1) + In[4];
		ByteToBit(Out, &S_Box[i][j][k], 4);
	}
}

void Transform(bool *Out, bool *In, const unsigned char *Table, int len) {
	int i;

	for (i = 0; i < len; ++i)
		Tmp[i] = In[Table[i] - 1];
	memcpy(Out, Tmp, len);
}

void Xor(bool *InA, bool *InB, int len) {
	int i;

	for (i = 0; i < len; ++i)
		InA[i] ^= InB[i];
}

void RotateL(bool *In, int len, int loop) {
	memcpy(Tmp, In, loop);
	memcpy(In, In + loop, len - loop);
	memcpy(In + len - loop, Tmp, loop);
}

void ByteToBit(bool *Out, const unsigned char *In, int bits) {
	int i;

	for (i = 0; i < bits; ++i)
		Out[i] = (In[i >> 3] >> (i & 7)) & 1;
}

void BitToByte(unsigned char *Out, const bool *In, int bits) {
	int i, j;

	memset(Out, 0, bits >> 3);
	for (i = 0; i < bits; ++i) {
		Out[i >> 3] |= In[i] << (i & 7);
	}
}
