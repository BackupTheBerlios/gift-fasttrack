/*
 * $Id: enc_type_80.c,v 1.4 2003/09/17 15:53:19 weinholt Exp $
 *
 * Copyright (C) 2003 giFT-FastTrack project
 * http://developer.berlios.de/projects/gift-fasttrack
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

/*
 * This is the most recent pad mingling code for FastTrack as of 03/06/01
 * Used for encryption version 0x80
 */

typedef unsigned int u32;
typedef unsigned char u8;

/* our not so structured exception handling */

typedef int THROWS_RET;

#define THROW return -1;
#define TRY(x) { if((x) == -1) return 0; }
#define RETURN return 0;

/* some helper funcs */

#ifndef __GNUC__
#define __attribute__(x)
#endif

/* my_cos() and my_sin() are equal to cos()<0 and sin()<0. */
static int __attribute__ ((const)) my_cos (unsigned char i)
{
	return (i * 39 + 61) % 245 > 122;
}

/* It works because of fmod(i, 2*M_PI) > M_PI. */
static int __attribute__ ((const)) my_sin (unsigned char i)
{
	return (i * 46) % 289 > 144;
}

/* this is (unsigned int) floor(sqrt(((double)(((unsigned char)(i))))+1) + 0.001). */
static int __attribute__ ((const)) my_sqrt (unsigned char i)
{
	int j, k;

	for (j = 0, k = 0; j++ <= i; j += ++k << 1);
	return k;
}

/* some constants and helper funcs */

static u32 ROR (u32 value, u32 count)
{
	count = (count & 0xff) % 32;
	return (value >> count) | (value << (32 - count));
}

static u32 ROL (u32 value, u32 count)
{
	count = (count & 0xff) % 32;
	return (value << count) | (value >> (32 - count));
}

/* the entry point of this mess */
/* this all works on unsigned ints so endianess is not an issue */

static THROWS_RET enc_80_mix (u32 *pad, u32 seed);

void enc_type_80 (u32 *pad, u32 seed)
{
	enc_80_mix (pad, seed);
}

/* major functions which make calls to other funcs */
static THROWS_RET enc_80_major_4E5F90 (u32 *pad, u32 seed);
static THROWS_RET enc_80_major_4E86B0 (u32 *pad, u32 seed);
static THROWS_RET enc_80_major_4ECD20 (u32 *pad, u32 seed);
static THROWS_RET enc_80_major_4EF430 (u32 *pad, u32 seed);
static THROWS_RET enc_80_major_4F3220 (u32 *pad, u32 seed);
static THROWS_RET enc_80_major_49B330 (u32 *pad, u32 seed);
static THROWS_RET enc_80_major_49E930 (u32 *pad, u32 seed);
static THROWS_RET enc_80_major_49ED30 (u32 *pad, u32 seed);
static THROWS_RET enc_80_major_4A1BB0 (u32 *pad, u32 seed);
static THROWS_RET enc_80_major_4AC300 (u32 *pad, u32 seed);
static THROWS_RET enc_80_major_4AC560 (u32 *pad, u32 seed);
static THROWS_RET enc_80_major_4AE780 (u32 *pad, u32 seed);
static THROWS_RET enc_80_major_4B2FD0 (u32 *pad, u32 seed);
static THROWS_RET enc_80_major_4BB410 (u32 *pad, u32 seed);
static THROWS_RET enc_80_major_4C1A00 (u32 *pad, u32 seed);
static THROWS_RET enc_80_major_4C52A0 (u32 *pad, u32 seed);
static THROWS_RET enc_80_major_4CFE70 (u32 *pad, u32 seed);
static THROWS_RET enc_80_major_4D2500 (u32 *pad, u32 seed);
static THROWS_RET enc_80_major_4D2900 (u32 *pad, u32 seed);
static THROWS_RET enc_80_major_4D7AE0 (u32 *pad, u32 seed);
static THROWS_RET enc_80_major_4DB520 (u32 *pad, u32 seed);

/* functions which throw exceptions */
static THROWS_RET enc_80_4F31A0 (u32 *pad);
static THROWS_RET enc_80_4ECBE0 (u32 *pad);
static THROWS_RET enc_80_4DB3D0 (u32 *pad);
static THROWS_RET enc_80_4B2BC0 (u32 *pad, u32 seed);
static THROWS_RET enc_80_4EF350 (u32 *pad);
static THROWS_RET enc_80_4D57E0 (u32 *pad);
static THROWS_RET enc_80_4DB090 (u32 *pad);
static THROWS_RET enc_80_4C5090 (u32 *pad, u32 seed);
static THROWS_RET enc_80_49E8E0 (u32 *pad, u32 seed);
static THROWS_RET enc_80_4AC2B0 (u32 *pad);
static THROWS_RET enc_80_4AEC50 (u32 *pad, u32 seed);
static THROWS_RET enc_80_4B2C60 (u32 *pad);
static THROWS_RET enc_80_4D5760 (u32 *pad);
static THROWS_RET enc_80_4D5830 (u32 *pad);
static THROWS_RET enc_80_4D7A40 (u32 *pad);
static THROWS_RET enc_80_4D7A90 (u32 *pad);
static THROWS_RET enc_80_4E5EE0 (u32 *pad);
static THROWS_RET enc_80_4EF230 (u32 *pad);
static THROWS_RET enc_80_4EF3B0 (u32 *pad);
static THROWS_RET enc_80_mix (u32 *pad, u32 seed);

/* simple functions which modify pad */
static void enc_80_4ECCF0 (u32 *pad);
static void enc_80_4E8590 (u32 *pad);
static void enc_80_4ECCC0 (u32 *pad);
static void enc_80_4ECC60 (u32 *pad);
static void enc_80_4E8680 (u32 *pad);
static void enc_80_4ECC90 (u32 *pad);
static void enc_80_4E8500 (u32 *pad, u32 seed);
static void enc_80_49B2E0 (u32 *pad);
static void enc_80_49B310 (u32 *pad);
static void enc_80_49B600 (u32 *pad, u32 seed);
static void enc_80_4A1B20 (u32 *pad, u32 seed);
static void enc_80_4A1B50 (u32 *pad);
static void enc_80_4A1B80 (u32 *pad);
static void enc_80_4AECA0 (u32 *pad);
static void enc_80_4B2CB0 (u32 *pad);
static void enc_80_4B2CE0 (u32 *pad);
static void enc_80_4C19B0 (u32 *pad);
static void enc_80_4C19E0 (u32 *pad);
static void enc_80_4C5060 (u32 *pad);
static void enc_80_4C5240 (u32 *pad);
static void enc_80_4C5270 (u32 *pad);
static void enc_80_4D57B0 (u32 *pad);
static void enc_80_4D7940 (u32 *pad);
static void enc_80_4D7980 (u32 *pad);
static void enc_80_4D79B0 (u32 *pad);
static void enc_80_4D79E0 (u32 *pad);
static void enc_80_4D7A10 (u32 *pad);
static void enc_80_4DB240 (u32 *pad);
static void enc_80_4DB000 (u32 *pad, u32 seed);
static void enc_80_4DB480 (u32 *pad, u32 seed);
static void enc_80_4E5D40 (u32 *pad);
static void enc_80_4E5D70 (u32 *pad);
static void enc_80_4E5DA0 (u32 *pad, u32 seed);
static void enc_80_4E5EB0 (u32 *pad, u32 seed);
static void enc_80_4EF200 (u32 *pad, u32 seed);
static void enc_80_4EF280 (u32 *pad, u32 seed);
static void enc_80_4EF400 (u32 *pad);
static void enc_80_4E5DD0 (u32 *pad);
static void enc_80_4E8530 (u32 *pad);
static void enc_80_4EF2A0 (u32 *pad);
static void enc_80_49B280 (u32 *pad);
static void enc_80_4E85C0 (u32 *pad);
static void enc_80_4E8620 (u32 *pad, u32 seed);
static void enc_80_4EF300 (u32 *pad);
static void enc_80_4E5E00 (u32 *pad, u32 seed);
static void enc_80_4E5E60 (u32 *pad);
static void enc_80_4E5F30 (u32 *pad);
static void enc_80_49ECD0 (u32 *pad);
static void enc_80_4DB030 (u32 *pad);
static void enc_80_4DB270 (u32 *pad, u32 seed);
static void enc_80_4DB2E0 (u32 *pad);
static void enc_80_49B630 (u32 *pad, u32 seed);
static void enc_80_4C50E0 (u32 *pad);

/* and so it begins... */

void enc_80_4ECCF0 (u32 *pad)
{
	pad[5] += pad[4] * 73;
}

void enc_80_4E8590 (u32 *pad)
{
	pad[0] += pad[5] & 0x1B65B2C8;
}

void enc_80_4ECCC0 (u32 *pad)
{
	pad[9] += pad[6] + 0x124D08A0;
}

void enc_80_4ECC60 (u32 *pad)
{
	pad[16] ^= (pad[0] * 41) << 1;
}

void enc_80_4E8680 (u32 *pad)
{
	pad[14] ^= pad[10] - 0x403483CE;
}

void enc_80_4ECC90 (u32 *pad)
{
	pad[3] ^= (pad[11] * 41) << 1;
}

void enc_80_4E8500 (u32 *pad, u32 seed)
{
	pad[10] = ROL (pad[10], (seed * 15) << 2);
}

void enc_80_49B2E0 (u32 *pad)
{
	pad[1] = ROR (pad[1], (pad[8] * 7) << 4);
}

void enc_80_49B310 (u32 *pad)
{
	pad[10] *= ROL (pad[1], 0xc);
}

void enc_80_49B600 (u32 *pad, u32 seed)
{
	pad[18] = ROL (pad[18], seed - 0xE066BF0);
}

void enc_80_4A1B20 (u32 *pad, u32 seed)
{
	pad[18] -= ((seed << 4) - seed) * 5;
}

void enc_80_4A1B50 (u32 *pad)
{
	pad[16] |= pad[18] | 0xB25175E;
}

void enc_80_4A1B80 (u32 *pad)
{
	pad[2] |= pad[15] - 0x1886D6A;
}

void enc_80_4AECA0 (u32 *pad)
{
	pad[16] -= ROL (pad[3], 0x1b);
}

void enc_80_4B2CB0 (u32 *pad)
{
	pad[11] |= pad[13] * 9;
}

void enc_80_4B2CE0 (u32 *pad)
{
	pad[6] &= pad[19] ^ 0x1FAF0F41;
}

void enc_80_4C19B0 (u32 *pad)
{
	pad[4] = ROR (pad[4], (pad[17] * 11) << 3);
}

void enc_80_4C19E0 (u32 *pad)
{
	pad[17] *= pad[6];
}

void enc_80_4C5060 (u32 *pad)
{
	pad[3] ^= pad[9] + 0x5B1A81FD;
}

void enc_80_4C5240 (u32 *pad)
{
	pad[17] *= ROR (pad[10], 0x1a);
}

void enc_80_4C5270 (u32 *pad)
{
	pad[19] ^= pad[3] ^ 0x19859C46;
}

void enc_80_4D57B0 (u32 *pad)
{
	pad[16] += pad[1] + 0x5EDB78DA;
}

void enc_80_4D7940 (u32 *pad)
{
	pad[12] = ROL (pad[12], (pad[3] * 43) << 1);
}

void enc_80_4D7980 (u32 *pad)
{
	pad[17] *= pad[13] - 0x68C0E272;
}

void enc_80_4D79B0 (u32 *pad)
{
	pad[13] += pad[13] | 0x5E919E06;
}

void enc_80_4D79E0 (u32 *pad)
{
	pad[16] = (pad[9] * pad[16] * 37) << 1;
}

void enc_80_4D7A10 (u32 *pad)
{
	pad[2] |= pad[15] + 0x44B04775;
}

void enc_80_4DB240 (u32 *pad)
{
	pad[8] = (pad[8] * pad[8] * 13) << 2;
}

void enc_80_4DB000 (u32 *pad, u32 seed)
{
	pad[17] = ROR (pad[17], seed + 0x451498EC);
}

void enc_80_4DB480 (u32 *pad, u32 seed)
{
	pad[10] = ((pad[10] * seed * 27) << 2) - (seed * pad[10]);
}

void enc_80_4E5D40 (u32 *pad)
{
	pad[8] += pad[17] + 0x4E0679BE;
}

void enc_80_4E5D70 (u32 *pad)
{
	pad[10] ^= pad[6] ^ 0x2BE68205;
}

void enc_80_4E5DA0 (u32 *pad, u32 seed)
{
	pad[7] -= (((seed * 3) << 3) - seed) << 2;
}

void enc_80_4E5EB0 (u32 *pad, u32 seed)
{
	pad[13] &= ROL (seed, 0x10);
}

void enc_80_4EF200 (u32 *pad, u32 seed)
{
	pad[9] &= seed * 75;
}

void enc_80_4EF280 (u32 *pad, u32 seed)
{
	pad[0] += seed * 25;
}

void enc_80_4EF400 (u32 *pad)
{
	pad[8] *= pad[6] ^ 0x377C08D2;
}

void enc_80_4E5DD0 (u32 *pad)
{
	pad[18] -= pad[10] < 0xB6C6C3E ? pad[10] : pad[13];
}

void enc_80_4E8530 (u32 *pad)
{
	pad[19] += my_sqrt (pad[18]);
}

void enc_80_4EF2A0 (u32 *pad)
{
	pad[11] ^= my_sqrt (pad[4]);
}

void enc_80_49B280 (u32 *pad)
{
	pad[17] ^= my_sqrt (pad[16]);
}

void enc_80_4E85C0 (u32 *pad)
{
	pad[9] += my_sqrt (pad[13]);
}

void enc_80_4E8620 (u32 *pad, u32 seed)
{
	pad[10] -= my_cos (seed) ? 0x19C6C6E : pad[7];
}

void enc_80_4EF300 (u32 *pad)
{
	pad[10] -= my_cos (pad[10]) ? 0x5F18F01 : pad[13];
}

void enc_80_4E5E00 (u32 *pad, u32 seed)
{
	pad[17] = ROL (pad[17], my_sin (pad[7]) ? 0x4262ED6B : seed);
}

void enc_80_4E5E60 (u32 *pad)
{
	pad[9] += my_cos (pad[6]) ? 0x14F5046C : pad[11];
}

void enc_80_4E5F30 (u32 *pad)
{
	pad[14] ^= my_cos (pad[11]) ? 0x562482FA : pad[14];
}

void enc_80_49ECD0 (u32 *pad)
{
	pad[3] -= my_cos (pad[14]) ? 0x39702EDD : pad[6];
}

void enc_80_4DB030 (u32 *pad)
{
	pad[6] ^= my_cos (pad[10]) ? 0x53236223 : pad[7];
}

void enc_80_4DB270 (u32 *pad, u32 seed)
{
	pad[1] |= my_sin (seed) ? 0x1BE7FECF : pad[16];
}

void enc_80_4DB2E0 (u32 *pad)
{
	pad[17] *= my_cos (pad[16]) ? 0x2C15B485 : pad[3];
}

THROWS_RET enc_80_4F31A0 (u32 *pad)
{
	pad[19] += my_sqrt (pad[0]);

	if (pad[19] & 1)
		THROW;

	RETURN;
}

THROWS_RET enc_80_4ECBE0 (u32 *pad)
{
	pad[7] = ROL (pad[7], my_sqrt (pad[1]));

	if (pad[7] & 1)
		THROW;

	RETURN;
}

THROWS_RET enc_80_4DB3D0 (u32 *pad)
{
	pad[4] += my_cos (pad[1]) ? 0x890AFEF : pad[4];

	if (pad[4] & 1)
		THROW;

	RETURN;
}

THROWS_RET enc_80_4B2BC0 (u32 *pad, u32 seed)
{
	pad[7] = ROL (pad[7], my_sin (pad[18]) ? 0x14D1DE3D : seed);

	if (pad[7] & 1)
		THROW;

	RETURN;
}

THROWS_RET enc_80_4EF350 (u32 *pad)
{
	pad[10] &= pad[1] < 0x1F2DD61 ? pad[1] : pad[10];

	if (pad[10] & 1)
		THROW;

	RETURN;
}

THROWS_RET enc_80_4D57E0 (u32 *pad)
{
	pad[0] |= ((pad[10] * 3) << 4) + pad[10];

	if (pad[0] & 1)
		THROW;

	RETURN;
}

THROWS_RET enc_80_4DB090 (u32 *pad)
{
	pad[9] ^= pad[1] < 0xB01609F ? pad[1] : pad[16];

	if (pad[9] & 1)
		THROW;

	RETURN;
}

THROWS_RET enc_80_4C5090 (u32 *pad, u32 seed)
{
	pad[13] = ROR (pad[13], (seed * 9) << 1);

	if (pad[13] & 1)
		THROW;

	RETURN;
}

THROWS_RET enc_80_49E8E0 (u32 *pad, u32 seed)
{
	pad[7] &= seed + 0x28495DA2;

	if (pad[7] & 1)
		THROW;

	RETURN;
}

THROWS_RET enc_80_4AC2B0 (u32 *pad)
{
	pad[11] += ROL (pad[5], 0x14);

	if (pad[11] & 1)
		THROW;

	RETURN;
}

THROWS_RET enc_80_4AEC50 (u32 *pad, u32 seed)
{
	pad[5] ^= seed + 0x391B56A;

	if (pad[5] & 1)
		THROW;

	RETURN;
}

THROWS_RET enc_80_4B2C60 (u32 *pad)
{
	pad[3] ^= pad[14] * 0x711881F7;

	if (pad[3] & 1)
		THROW;

	RETURN;
}

THROWS_RET enc_80_4D5760 (u32 *pad)
{
	pad[6] -= pad[18] ^ 0x4402CAF;

	if (pad[6] & 1)
		THROW;

	RETURN;
}

THROWS_RET enc_80_4D5830 (u32 *pad)
{
	pad[2] += ROR (pad[14], 0x14);

	if (pad[2] & 1)
		THROW;

	RETURN;
}

THROWS_RET enc_80_4D7A40 (u32 *pad)
{
	pad[6] += 0x287735D1 - pad[3];

	if (pad[6] & 1)
		THROW;

	RETURN;
}

THROWS_RET enc_80_4D7A90 (u32 *pad)
{
	pad[3] += 0x247B4DE9 - pad[1];

	if (pad[3] & 1)
		THROW;

	RETURN;
}

THROWS_RET enc_80_4E5EE0 (u32 *pad)
{
	pad[11] += pad[6] * 0xE8C67004;

	if (pad[11] & 1)
		THROW;

	RETURN;
}

THROWS_RET enc_80_4EF230 (u32 *pad)
{
	pad[15] |= pad[1] ^ 0x37B6FE35;

	if (pad[15] & 1)
		THROW;

	RETURN;
}

THROWS_RET enc_80_4EF3B0 (u32 *pad)
{
	pad[14] = ROR (pad[14], pad[13] | 0x3433BE6);

	if (pad[14] & 1)
		THROW;

	RETURN;
}

void enc_80_49B630 (u32 *pad, u32 seed)
{
	pad[10] *= my_cos (pad[3]) ? 0x1DD34A4 : pad[2];
	pad[12] |= (pad[16] * 11) << 2;
	pad[0] ^= pad[19] ^ 0x509e41a1;
	pad[13] -= (((pad[9] * 15) << 1) - pad[9]) << 2;
	seed -= my_cos (pad[2]) ? 0xD7A79F4 : pad[5];
	pad[2] -= my_sin (pad[12]) ? 0x5134639d : pad[1];
	seed ^= my_sin (pad[4]) ? 0x241147A3 : pad[13];
	pad[3] += my_sqrt (seed);
	pad[0] -= my_sin (seed) ? 0x5176ffdf : pad[15];
	pad[3] -= (((pad[9] * 15) << 1) - pad[9]) * 3;
	pad[14] |= pad[2] < 0x18897 ? pad[2] : pad[10];
	pad[13] ^= 0x18897 + seed;
}

void enc_80_4C50E0 (u32 *pad)
{
	pad[13] -= pad[2];
	pad[19] += 0x471c95 & pad[12];
	pad[14] += pad[7] < 0x471c95 ? pad[7] : pad[2];
	pad[12] -= pad[11] ^ 0x5E823762;
	pad[15] &= (pad[0] * 13) << 2;
	pad[17] ^= pad[8] * 15;
	pad[7] += pad[4] & 0x3996FD51;
	pad[2] += my_sin (pad[19]) ? 0x34311111 : pad[5];
}

THROWS_RET enc_80_major_4E5F90 (u32 *pad, u32 seed)
{
	u32 branch = (pad[8] ^ pad[16] ^ seed) % 5;

	seed *= my_sqrt (seed);
	pad[4] |= seed < 0x67f88 ? seed : pad[3];

	if (branch == 1)
	{
		enc_80_4D57B0 (pad);
		enc_80_49B280 (pad);
		enc_80_4C50E0 (pad);
	}

	pad[7] = ROR (pad[7], pad[17] & 0x67f88);
	pad[0] += ROR (pad[6], 0x8);

	if (branch == 0)
	{
		enc_80_4A1B80 (pad);
		enc_80_4E8500 (pad, 0xB899C92B);
		enc_80_49B630 (pad, seed);
	}

	seed -= pad[14] ^ 0xffc56d16;
	seed &= my_sqrt (pad[1]);

	if (branch == 2)
	{
		enc_80_4DB270 (pad, 0xF9FFE780);
		TRY (enc_80_4D7A40 (pad));
		TRY (enc_80_major_4AC300 (pad, pad[6]));
	}

	seed = ROL (seed, pad[13] + 0x8D810DF);
	pad[7] ^= (seed * 11) << 1;

	if (branch == 3)
	{
		enc_80_4A1B20 (pad, 0x7ADA2000);
		enc_80_4DB2E0 (pad);
		TRY (enc_80_major_4D7AE0 (pad, pad[8]));
	}

	pad[8] ^= my_cos (pad[16]) ? 0x12DA5B58 : pad[2];
	pad[14] ^= pad[13] - 0xff8c9a4d;

	if (branch == 4)
	{
		enc_80_4AECA0 (pad);
		enc_80_4E5F30 (pad);
		TRY (enc_80_major_49B330 (pad, seed));
	}

	pad[6] *= pad[17] < 0x9D9106A ? pad[17] : pad[8];

	RETURN;
}

THROWS_RET enc_80_major_4E86B0 (u32 *pad, u32 seed)
{
	u32 branch = (pad[10] ^ pad[0] ^ pad[13]) % 0x0E;

	if (branch == 3)
	{
		TRY (enc_80_4D5830 (pad));
		enc_80_4D7980 (pad);
		TRY (enc_80_major_4AC300 (pad, pad[10]));
	}

	pad[19] = ROL (pad[19], seed * 41);

	if (branch == 0x0D)
	{
		TRY (enc_80_4EF230 (pad));
		enc_80_4A1B80 (pad);
		TRY (enc_80_major_4D7AE0 (pad, seed));
	}

	if (branch == 2)
	{
		enc_80_4AECA0 (pad);
		enc_80_4DB2E0 (pad);
		TRY (enc_80_major_49B330 (pad, pad[1]));
	}

	pad[12] &= seed & 0x162E075D;

	if (branch == 0)
	{
		enc_80_4EF2A0 (pad);
		enc_80_4DB000 (pad, 0xCC7693F);
		TRY (enc_80_major_4E5F90 (pad, pad[3]));
	}

	pad[7] += 0xC0CF1E75 - pad[18];

	if (branch == 0)
	{
		enc_80_4E5DD0 (pad);
		enc_80_4E5DA0 (pad, 0x3E303464);
		TRY (enc_80_major_4EF430 (pad, pad[13]));
	}

	if (branch == 0x0B)
	{
		TRY (enc_80_4B2C60 (pad));
		enc_80_49ECD0 (pad);
		TRY (enc_80_major_4C52A0 (pad, seed));
	}

	pad[10] *= seed + 0x15A0944D;

	if (branch == 7)
	{
		enc_80_4E5D70 (pad);
		TRY (enc_80_4B2BC0 (pad, 0xE636D969));
		TRY (enc_80_major_49E930 (pad, pad[14]));
	}

	pad[13] += pad[0] - 0x3B1C8FB8;

	if (branch == 8)
	{
		enc_80_4EF400 (pad);
		enc_80_4E5D40 (pad);
		TRY (enc_80_major_49ED30 (pad, pad[5]));
	}

	if (branch == 6)
	{
		TRY (enc_80_4AEC50 (pad, 0x34472A0D));
		enc_80_4E8530 (pad);
		TRY (enc_80_major_4D2500 (pad, pad[1]));
	}

	pad[18] *= pad[2] < 0xF9B0FAE ? pad[2] : pad[19];

	if (branch == 0x0C)
	{
		TRY (enc_80_4B2BC0 (pad, 0xDF3D4CFC));
		TRY (enc_80_4E5EE0 (pad));
		TRY (enc_80_major_4AE780 (pad, seed));
	}

	pad[1] += (((pad[5] * 3) << 3) - pad[5]) << 1;

	if (branch == 1)
	{
		enc_80_4E5E60 (pad);
		enc_80_4EF280 (pad, 0x288A7CDC);
		TRY (enc_80_major_4F3220 (pad, pad[17]));
	}

	if (branch == 4)
	{
		enc_80_4E5E60 (pad);
		enc_80_4E5F30 (pad);
		TRY (enc_80_major_4C1A00 (pad, seed));
	}

	pad[11] ^= 0xffb29fb8 | pad[13];

	if (branch == 0x0A)
	{
		TRY (enc_80_4AC2B0 (pad));
		enc_80_4C19E0 (pad);
		TRY (enc_80_major_4AC300 (pad, pad[11]));
	}

	pad[13] ^= seed ^ 0x265916C9;

	if (branch == 5)
	{
		enc_80_4C19B0 (pad);
		TRY (enc_80_4EF350 (pad));
		TRY (enc_80_major_4D7AE0 (pad, pad[11]));
	}

	if (branch == 9)
	{
		enc_80_4C5060 (pad);
		enc_80_4ECC90 (pad);
		TRY (enc_80_major_49B330 (pad, pad[1]));
	}

	pad[2] *= pad[5] * 105;

	RETURN;
}

THROWS_RET enc_80_major_4ECD20 (u32 *pad, u32 seed)
{
	u32 branch = (pad[3] ^ pad[14] ^ seed) & 0x0F;

	if (branch == 0x0C)
	{
		enc_80_4D7980 (pad);
		enc_80_4DB270 (pad, 0xE7C0DE4B);
		TRY (enc_80_major_49E930 (pad, seed));
	}

	if (branch == 4)
	{
		enc_80_4ECCF0 (pad);
		enc_80_4D79B0 (pad);
		TRY (enc_80_major_49ED30 (pad, pad[19]));
	}

	seed += ((-pad[5] << 3) - pad[5]) * 2;

	if (branch == 8)
	{
		TRY (enc_80_49E8E0 (pad, 0x60C57D12));
		enc_80_49B310 (pad);
		TRY (enc_80_major_4D2500 (pad, seed));
	}

	if (branch == 0x0E)
	{
		enc_80_4D7980 (pad);
		enc_80_4C5060 (pad);
		TRY (enc_80_major_4AE780 (pad, pad[4]));
	}

	pad[19] ^= 0xbd8b55f ^ pad[11];

	if (branch == 0x0B)
	{
		enc_80_4E5EB0 (pad, 0x20AA3ABD);
		TRY (enc_80_4AEC50 (pad, 0x5D894D2));
		TRY (enc_80_major_4F3220 (pad, pad[17]));
	}

	if (branch == 4)
	{
		enc_80_4E5F30 (pad);
		enc_80_4E5DD0 (pad);
		TRY (enc_80_major_4C1A00 (pad, seed));
	}

	pad[16] -= (((pad[3] * 3) << 2) - pad[3]) << 1;

	if (branch == 1)
	{
		enc_80_4C5270 (pad);
		enc_80_4DB480 (pad, 0xDA9100);
		TRY (enc_80_major_4E86B0 (pad, seed));
	}

	if (branch == 9)
	{
		enc_80_4D7940 (pad);
		enc_80_4D7A10 (pad);
		TRY (enc_80_major_4AC560 (pad, pad[9]));
	}

	pad[14] *= pad[5] | 0x132A8FBD;

	if (branch == 0x0A)
	{
		enc_80_4DB000 (pad, 0x3D008A13);
		enc_80_4EF200 (pad, 0x2B5507BC);
		TRY (enc_80_major_4BB410 (pad, seed));
	}

	if (branch == 0x0D)
	{
		enc_80_4A1B80 (pad);
		enc_80_4C5240 (pad);
		TRY (enc_80_major_4B2FD0 (pad, seed));
	}

	if (branch == 2)
	{
		TRY (enc_80_4F31A0 (pad));
		enc_80_4E5EB0 (pad, 0xFE2E49B0);
		TRY (enc_80_major_4D2900 (pad, pad[17]));
	}

	pad[12] += ((-(((seed * 5) << 2) - seed)) * 3) << 1;

	if (branch == 1)
	{
		TRY (enc_80_4C5090 (pad, 0x667591C7));
		enc_80_4C5240 (pad);
		TRY (enc_80_major_4CFE70 (pad, pad[14]));
	}

	if (branch == 2)
	{
		enc_80_4E5D70 (pad);
		TRY (enc_80_4DB3D0 (pad));
		TRY (enc_80_major_49E930 (pad, seed));
	}

	pad[16] *= my_cos (seed) ? 0x40E92E8A : pad[9];

	if (branch == 0)
	{
		enc_80_4C19B0 (pad);
		TRY (enc_80_4D5830 (pad));
		TRY (enc_80_major_49ED30 (pad, pad[5]));
		enc_80_4D57B0 (pad);
		enc_80_4E5E60 (pad);
		TRY (enc_80_major_4D2500 (pad, pad[10]));
	}

	seed *= pad[2] ^ 0xE5C69EA;

	if (branch == 5)
	{
		TRY (enc_80_4C5090 (pad, 0x9240545D));
		enc_80_4D79B0 (pad);
		TRY (enc_80_major_4AE780 (pad, pad[9]));
	}

	if (branch == 7)
	{
		TRY (enc_80_4D57E0 (pad));
		enc_80_49B2E0 (pad);
		TRY (enc_80_major_4F3220 (pad, pad[0]));
	}

	pad[8] += seed - 0x1DBCEFC2;

	if (branch == 3)
	{
		enc_80_4C5270 (pad);
		enc_80_4ECCF0 (pad);
		TRY (enc_80_major_4C1A00 (pad, pad[14]));
	}

	if (branch == 6)
	{
		enc_80_4D79B0 (pad);
		enc_80_4E5DA0 (pad, 0x907C5B26);
		TRY (enc_80_major_4E86B0 (pad, seed));
	}

	seed += pad[15] - 0x3206A0CA;

	if (branch == 3)
	{
		enc_80_49B310 (pad);
		enc_80_4A1B80 (pad);
		TRY (enc_80_major_4AC560 (pad, seed));
	}

	if (branch == 0x0F)
	{
		TRY (enc_80_4D7A40 (pad));
		enc_80_4ECC90 (pad);
		TRY (enc_80_major_4BB410 (pad, pad[1]));
	}

	pad[14] ^= pad[8] & 0x7d271;

	RETURN;
}

THROWS_RET enc_80_major_4EF430 (u32 *pad, u32 seed)
{
	u32 branch = (pad[12] ^ pad[11] ^ seed) % 0x06;

	pad[11] *= 0x65300 | seed;

	if (branch == 2)
	{
		enc_80_4C19E0 (pad);
		enc_80_4A1B50 (pad);
		enc_80_4C50E0 (pad);
	}

	seed += 0 - (((pad[6] * 5) << 4) - pad[6]);
	pad[8] = ROL (pad[8], my_sqrt (pad[9]));

	if (branch == 0)
	{
		TRY (enc_80_4DB090 (pad));
		enc_80_4ECC60 (pad);
		enc_80_49B630 (pad, pad[4]);
	}

	pad[0] -= pad[14] ^ 0xff7db6c1;
	pad[6] = ROL (pad[6], pad[11] ^ 0xffc1081c);

	if (branch == 5)
	{
		enc_80_4AECA0 (pad);
		enc_80_4AECA0 (pad);
		TRY (enc_80_major_4AC300 (pad, pad[7]));
	}

	seed -= pad[11] * 3 << 4;

	if (branch == 0)
	{
		enc_80_49B310 (pad);
		enc_80_4AECA0 (pad);
		TRY (enc_80_major_4D7AE0 (pad, seed));
	}

	pad[13] |= ROR (pad[16], 0xc);
	pad[3] |= pad[9] + 0x27C4C44E;

	if (branch == 1)
	{
		enc_80_4DB000 (pad, 0xE8890374);
		enc_80_4E8680 (pad);
		TRY (enc_80_major_49B330 (pad, seed));
	}

	pad[13] -= (((pad[6] * 5) << 3) - pad[6]) << 1;
	pad[10] ^= ROL (pad[11], 0x4);

	if (branch == 3)
	{
		enc_80_49B280 (pad);
		TRY (enc_80_4D57E0 (pad));
		TRY (enc_80_major_4E5F90 (pad, pad[10]));
	}

	RETURN;
}

THROWS_RET enc_80_major_4F3220 (u32 *pad, u32 seed)
{
	u32 branch = (pad[14] ^ pad[2] ^ pad[19]) % 0x0C;

	if (branch == 4)
	{
		enc_80_4C19E0 (pad);
		TRY (enc_80_4ECBE0 (pad));
		enc_80_4C50E0 (pad);
	}

	seed |= pad[11] ^ 0x4E05B048;

	if (branch == 0x0A)
	{
		enc_80_4E85C0 (pad);
		enc_80_4E5F30 (pad);
		enc_80_49B630 (pad, pad[1]);
	}

	seed += pad[2] * 50;

	if (branch == 1)
	{
		TRY (enc_80_4D57E0 (pad));
		enc_80_4D79B0 (pad);
		TRY (enc_80_major_4AC300 (pad, seed));
	}

	seed = (seed * pad[3] * 13) << 1;

	if (branch == 3)
	{
		enc_80_4D79B0 (pad);
		enc_80_4D57B0 (pad);
		TRY (enc_80_major_4D7AE0 (pad, pad[7]));
	}

	pad[10] |= pad[19] & 0x42A403D;

	if (branch == 6)
	{
		enc_80_4D7940 (pad);
		TRY (enc_80_4D7A40 (pad));
		TRY (enc_80_major_49B330 (pad, seed));
	}

	pad[4] = ROR (pad[4], pad[18] + 0x486c59);

	if (branch == 2)
	{
		TRY (enc_80_4D57E0 (pad));
		enc_80_4E5DA0 (pad, 0xBCD76101);
		TRY (enc_80_major_4E5F90 (pad, pad[4]));
	}

	pad[11] += my_sin (seed) ? 0x44ACFBD : pad[9];

	if (branch == 9)
	{
		enc_80_4C5240 (pad);
		enc_80_4E8620 (pad, 0xE0A4F875);
		TRY (enc_80_major_4EF430 (pad, pad[12]));
	}

	seed = ROR (seed, seed < 0xaa6dc ? seed : pad[17]);

	if (branch == 0)
	{
		enc_80_4C19E0 (pad);
		TRY (enc_80_4DB3D0 (pad));
		TRY (enc_80_major_4C52A0 (pad, pad[15]));
	}

	pad[16] *= 0xffc209cf ^ seed;

	if (branch == 0)
	{
		enc_80_4EF400 (pad);
		enc_80_4DB480 (pad, 0xF0F77B83);
		TRY (enc_80_major_49E930 (pad, pad[5]));
	}

	pad[4] += pad[17] * 0xE9F05570;

	if (branch == 5)
	{
		enc_80_4E8530 (pad);
		enc_80_4D79B0 (pad);
		TRY (enc_80_major_49ED30 (pad, pad[4]));
	}

	pad[13] *= pad[8] ^ 0x4001ca;

	if (branch == 7)
	{
		enc_80_4C5060 (pad);
		enc_80_4E5EB0 (pad, 0x1D1451DE);
		TRY (enc_80_major_4D2500 (pad, pad[1]));
	}

	if (branch == 8)
	{
		enc_80_4C5060 (pad);
		enc_80_4ECC90 (pad);
		TRY (enc_80_major_4AE780 (pad, pad[11]));
	}

	pad[13] &= pad[9] * 0x4463ff;

	if (branch == 0x0B)
	{
		enc_80_4C5240 (pad);
		enc_80_4A1B50 (pad);
		enc_80_4C50E0 (pad);
	}

	RETURN;
}

THROWS_RET enc_80_major_49B330 (u32 *pad, u32 seed)
{
	u32 branch = (pad[9] ^ pad[5] ^ pad[6]) & 0x03;

	pad[12] += my_sqrt (pad[1]);
	pad[18] += pad[19] + 0x2B7FD413;

	if (branch == 0)
	{
		enc_80_4C19B0 (pad);
		enc_80_4C5270 (pad);
		enc_80_4C50E0 (pad);
	}

	seed = ROR (seed, pad[4] - 0xA4E29DD);
	seed ^= ROR (pad[17], 0x1c);

	if (branch == 0)
	{
		TRY (enc_80_4D5760 (pad));
		enc_80_4C19E0 (pad);
		enc_80_49B630 (pad, pad[3]);
	}

	pad[5] += ((seed * 27) << 1) - seed;
	pad[18] += pad[6] - 0x24A58A0;

	if (branch == 3)
	{
		enc_80_4DB030 (pad);
		enc_80_4E8500 (pad, 0x38209953);
		TRY (enc_80_major_4AC300 (pad, pad[18]));
	}

	pad[10] &= my_sin (pad[17]) ? 0xf1f3d : pad[13];

	pad[19] |= my_sin (pad[18]) ? 0x4f2a72 : pad[4];
	pad[13] *= ROR (pad[12], 0x12);

	if (branch == 2)
	{
		TRY (enc_80_4EF3B0 (pad));
		enc_80_4A1B20 (pad, 0xE13AFBA1);
		TRY (enc_80_major_4D7AE0 (pad, pad[2]));
	}

	pad[6] += (pad[1] << 6) + pad[1];

	RETURN;
}

THROWS_RET enc_80_major_49E930 (u32 *pad, u32 seed)
{
	u32 branch = pad[9] & 0x07;

	seed -= 0x646ede77 & pad[7];

	if (branch == 0)
	{
		enc_80_4B2CE0 (pad);
		TRY (enc_80_4AC2B0 (pad));
		enc_80_4C50E0 (pad);
	}

	seed = ROR (seed, (seed * 59) << 1);

	if (branch == 6)
	{
		enc_80_4C19E0 (pad);
		enc_80_4DB000 (pad, 0x1D050D83);
		enc_80_49B630 (pad, pad[4]);
	}

	pad[3] += my_sin (seed) ? 0x4c8444 : pad[8];

	if (branch == 5)
	{
		enc_80_4E8530 (pad);
		enc_80_49B600 (pad, 0x2AB2E7E2);
		TRY (enc_80_major_4AC300 (pad, pad[18]));
	}

	pad[2] = ROL (pad[2], (((pad[4] * 3) << 3) - pad[4]) * 3);

	if (branch == 3)
	{
		enc_80_4C5240 (pad);
		enc_80_4E5EB0 (pad, 0xD1FC123);
		TRY (enc_80_major_4D7AE0 (pad, seed));
	}

	pad[0] += my_sin (pad[4]) ? 0xda2b08 : seed;

	if (branch == 2)
	{
		TRY (enc_80_4E5EE0 (pad));
		enc_80_4EF200 (pad, 0x2F40BB4D);
		TRY (enc_80_major_49B330 (pad, pad[9]));
	}

	seed ^= seed * 0x11;

	if (branch == 4)
	{
		enc_80_4E8680 (pad);
		enc_80_4AECA0 (pad);
		TRY (enc_80_major_4E5F90 (pad, pad[10]));
	}

	pad[4] *= 0xdeaf0b ^ pad[2];

	if (branch == 1)
	{
		enc_80_4ECCC0 (pad);
		TRY (enc_80_4EF230 (pad));
		TRY (enc_80_major_4EF430 (pad, seed));
	}

	pad[9] = ((pad[11] * pad[9] * 5) << 3) - pad[11] * pad[9];
	pad[1] += ((pad[10] << 5) + pad[10]) * 3;

	if (branch == 0)
	{
		TRY (enc_80_4E5EE0 (pad));
		TRY (enc_80_4EF350 (pad));
		TRY (enc_80_major_4C52A0 (pad, pad[0]));
	}

	RETURN;
}

THROWS_RET enc_80_major_49ED30 (u32 *pad, u32 seed)
{
	u32 branch = (pad[4] ^ pad[13] ^ pad[17]) % 0x09;

	seed ^= ROR (seed, 0x6);

	if (branch == 0)
	{
		TRY (enc_80_4AEC50 (pad, 0x2816580B));
		enc_80_4B2CE0 (pad);
		enc_80_4C50E0 (pad);
	}

	pad[2] &= pad[16] ^ 0x448522;

	if (branch == 8)
	{
		TRY (enc_80_4B2C60 (pad));
		TRY (enc_80_4EF3B0 (pad));
		enc_80_49B630 (pad, pad[15]);
	}

	pad[7] &= my_sin (seed) ? 0x5228985F : pad[5];

	if (branch == 3)
	{
		TRY (enc_80_4DB3D0 (pad));
		enc_80_4A1B20 (pad, 0x9312E9EA);
		TRY (enc_80_major_4AC300 (pad, pad[18]));
	}

	seed *= pad[3] ^ 0x93fa66;

	if (branch == 7)
	{
		TRY (enc_80_4D57E0 (pad));
		enc_80_4EF280 (pad, 0x142A13B1);
		TRY (enc_80_major_4D7AE0 (pad, pad[13]));
	}

	pad[7] -= ((seed * 5) << 2) - seed;

	if (branch == 4)
	{
		enc_80_4E5DA0 (pad, 0x96D075FF);
		enc_80_4D79B0 (pad);
		TRY (enc_80_major_49B330 (pad, pad[13]));
	}

	pad[12] = ROL (pad[12], (((seed << 4) + seed) * 3) << 1);

	if (branch == 6)
	{
		enc_80_4DB270 (pad, 0xB554145);
		TRY (enc_80_4B2C60 (pad));
		TRY (enc_80_major_4E5F90 (pad, pad[5]));
	}

	pad[19] = ROL (pad[19], seed + 0x4685D5BE);

	if (branch == 1)
	{
		TRY (enc_80_4DB090 (pad));
		enc_80_4C19E0 (pad);
		TRY (enc_80_major_4EF430 (pad, pad[14]));
	}

	pad[5] += pad[6] | 0x301DE279;

	if (branch == 2)
	{
		enc_80_4ECC90 (pad);
		enc_80_4E8530 (pad);
		TRY (enc_80_major_4C52A0 (pad, pad[17]));
	}

	pad[6] *= pad[0] ^ 0xffffffbb;

	if (branch == 0)
	{
		enc_80_4ECCF0 (pad);
		enc_80_49B600 (pad, 0x11B2399C);
		TRY (enc_80_major_49E930 (pad, pad[14]));
	}

	pad[17] |= my_sqrt (pad[12]);

	RETURN;
}

THROWS_RET enc_80_major_4A1BB0 (u32 *pad, u32 seed)
{
	u32 branch = pad[9] & 0x0F;

	if (branch == 0x0D)
	{
		enc_80_4E5D40 (pad);
		enc_80_4AECA0 (pad);
		TRY (enc_80_major_4D2500 (pad, pad[13]));
	}

	if (branch == 2)
	{
		TRY (enc_80_4EF3B0 (pad));
		enc_80_4C19E0 (pad);
		TRY (enc_80_major_4AE780 (pad, pad[10]));
	}

	pad[1] -= 0x49d06938 ^ pad[7];

	if (branch == 6)
	{
		TRY (enc_80_4EF350 (pad));
		enc_80_49B310 (pad);
		TRY (enc_80_major_4F3220 (pad, seed));
	}

	if (branch == 0x0E)
	{
		enc_80_4DB000 (pad, 0x3A1AC951);
		TRY (enc_80_4DB3D0 (pad));
		TRY (enc_80_major_4C1A00 (pad, seed));
	}

	pad[13] *= my_cos (seed) ? 0x46223265 : pad[18];

	if (branch == 6)
	{
		TRY (enc_80_4F31A0 (pad));
		TRY (enc_80_4D57E0 (pad));
		TRY (enc_80_major_4E86B0 (pad, pad[3]));
	}

	if (branch == 4)
	{
		enc_80_4D7980 (pad);
		enc_80_4E5D40 (pad);
		TRY (enc_80_major_4AC560 (pad, seed));
	}

	pad[6] += (seed * 3) << 3;

	if (branch == 4)
	{
		TRY (enc_80_4AC2B0 (pad));
		enc_80_4E8620 (pad, 0x177D7BB6);
		TRY (enc_80_major_4BB410 (pad, pad[14]));
	}

	if (branch == 2)
	{
		enc_80_4EF280 (pad, 0x32FB8883);
		TRY (enc_80_4ECBE0 (pad));
		TRY (enc_80_major_4B2FD0 (pad, pad[0]));
	}

	seed ^= my_sin (pad[11]) ? 0x2F24FB19 : pad[16];

	if (branch == 0x0A)
	{
		enc_80_4C19E0 (pad);
		enc_80_49ECD0 (pad);
		TRY (enc_80_major_4D2900 (pad, pad[7]));
	}

	if (branch == 0x0B)
	{
		enc_80_4DB240 (pad);
		enc_80_4DB270 (pad, 0xEBCD1E05);
		TRY (enc_80_major_4CFE70 (pad, pad[9]));
	}

	if (branch == 5)
	{
		enc_80_4DB480 (pad, 0xFDB8F3CF);
		enc_80_4AECA0 (pad);
		TRY (enc_80_major_4ECD20 (pad, seed));
	}

	pad[18] *= pad[2] + 0x92c5d3;

	if (branch == 1)
	{
		enc_80_4A1B80 (pad);
		enc_80_4E85C0 (pad);
		TRY (enc_80_major_4DB520 (pad, pad[7]));
	}

	if (branch == 5)
	{
		TRY (enc_80_4D7A40 (pad));
		enc_80_4E5E60 (pad);
		TRY (enc_80_major_4D2500 (pad, seed));
	}

	pad[6] -= pad[8] & 0x42D2AFE6;

	if (branch == 0)
	{
		enc_80_4D7980 (pad);
		TRY (enc_80_4EF350 (pad));
		TRY (enc_80_major_4AE780 (pad, pad[9]));
	}

	if (branch == 7)
	{
		enc_80_4ECC60 (pad);
		enc_80_4E85C0 (pad);
		TRY (enc_80_major_4F3220 (pad, pad[3]));
	}

	pad[14] = ROR (pad[14], pad[3] + 0x44060020);

	if (branch == 3)
	{
		enc_80_4A1B80 (pad);
		enc_80_4EF400 (pad);
		TRY (enc_80_major_4C1A00 (pad, pad[2]));
	}

	if (branch == 9)
	{
		enc_80_4C19B0 (pad);
		enc_80_4DB000 (pad, 0x2B886022);
		TRY (enc_80_major_4E86B0 (pad, seed));
	}

	pad[11] &= seed ^ 0x26649BD;

	if (branch == 0x0C)
	{
		enc_80_4C5270 (pad);
		enc_80_4C19E0 (pad);
		TRY (enc_80_major_4AC560 (pad, seed));
	}

	if (branch == 0x0F)
	{
		enc_80_4A1B20 (pad, 0x3B290E5);
		enc_80_49B310 (pad);
		TRY (enc_80_major_4BB410 (pad, pad[17]));
	}

	pad[3] = ROR (pad[3], 0xd783ed & pad[11]);

	if (branch == 1)
	{
		TRY (enc_80_4D57E0 (pad));
		enc_80_4ECC90 (pad);
		TRY (enc_80_major_4B2FD0 (pad, pad[13]));
	}

	if (branch == 8)
	{
		enc_80_4ECC90 (pad);
		enc_80_4C19E0 (pad);
		TRY (enc_80_major_4D2900 (pad, pad[11]));
	}

	if (branch == 0)
	{
		TRY (enc_80_4DB090 (pad));
		enc_80_4A1B80 (pad);
		TRY (enc_80_major_4CFE70 (pad, seed));
	}

	pad[10] += my_cos (pad[0]) ? 0xF1B21FE : seed;

	if (branch == 3)
	{
		enc_80_4E5DD0 (pad);
		TRY (enc_80_4B2C60 (pad));
		TRY (enc_80_major_4ECD20 (pad, pad[1]));
	}

	pad[19] *= pad[18] * 73;

	RETURN;
}

THROWS_RET enc_80_major_4AC300 (u32 *pad, u32 seed)
{
	u32 branch = (pad[7] ^ pad[3] ^ seed) & 0x01;

	pad[10] &= 0x75eefb0 ^ seed;

	pad[11] ^= my_cos (pad[1]) ? 0x2E0A5BE7 : pad[6];
	pad[0] += seed + 0x1FE76B44;

	pad[13] += 0x76173ce - seed;

	if (branch == 1)
	{
		enc_80_4E5EB0 (pad, 0xCD17011);
		enc_80_4D79E0 (pad);
		enc_80_4C50E0 (pad);
	}

	pad[16] -= pad[11] < 0x56C0185B ? pad[11] : pad[19];
	pad[0] ^= my_sin (pad[16]) ? 0x5A271260 : seed;

	pad[6] += pad[11] | 0x58E035D2;

	pad[14] = ROL (pad[14], ROL (pad[1], 0x8));
	pad[13] ^= pad[15] * 91;

	if (branch == 0)
	{
		enc_80_4EF200 (pad, 0x77783699);
		TRY (enc_80_4AC2B0 (pad));
		enc_80_49B630 (pad, pad[14]);
	}

	RETURN;
}

THROWS_RET enc_80_major_4AC560 (u32 *pad, u32 seed)
{
	u32 branch = pad[14] % 0x0F;

	if (branch == 0x0A)
	{
		enc_80_4E5DD0 (pad);
		enc_80_4D7A10 (pad);
		TRY (enc_80_major_4D7AE0 (pad, pad[19]));
	}

	pad[11] = ROR (pad[11], 0xa938539 & pad[5]);

	if (branch == 3)
	{
		enc_80_4E5D40 (pad);
		TRY (enc_80_4C5090 (pad, 0x7643972C));
		TRY (enc_80_major_49B330 (pad, seed));
	}

	if (branch == 0)
	{
		enc_80_49ECD0 (pad);
		TRY (enc_80_4D5760 (pad));
		TRY (enc_80_major_4E5F90 (pad, pad[12]));
	}

	pad[7] *= my_cos (pad[4]) ? 0xA45B84A : pad[13];

	if (branch == 4)
	{
		TRY (enc_80_4ECBE0 (pad));
		enc_80_4D57B0 (pad);
		TRY (enc_80_major_4EF430 (pad, pad[0]));
	}

	seed |= my_sin (seed) ? 0xae00950 : pad[10];

	if (branch == 0x0D)
	{
		TRY (enc_80_4C5090 (pad, 0x4611946));
		enc_80_4EF300 (pad);
		TRY (enc_80_major_4C52A0 (pad, pad[7]));
	}

	if (branch == 0)
	{
		enc_80_4ECCF0 (pad);
		enc_80_49ECD0 (pad);
		TRY (enc_80_major_49E930 (pad, seed));
	}

	pad[15] += pad[13] + 0x519A438A;

	if (branch == 1)
	{
		TRY (enc_80_4AC2B0 (pad));
		enc_80_4C5060 (pad);
		TRY (enc_80_major_49ED30 (pad, pad[4]));
	}

	if (branch == 2)
	{
		enc_80_4ECCF0 (pad);
		enc_80_4D7980 (pad);
		TRY (enc_80_major_4D2500 (pad, pad[15]));
	}

	pad[9] -= my_cos (seed) ? 0xaa60c04 : pad[15];

	if (branch == 0x0C)
	{
		enc_80_4ECC60 (pad);
		enc_80_4E8590 (pad);
		TRY (enc_80_major_4AE780 (pad, pad[7]));
	}

	if (branch == 5)
	{
		TRY (enc_80_4AC2B0 (pad));
		enc_80_4E5D40 (pad);
		TRY (enc_80_major_4F3220 (pad, pad[15]));
	}

	pad[8] += ROR (pad[19], 0x4);

	if (branch == 0x0B)
	{
		enc_80_49B310 (pad);
		enc_80_4B2CE0 (pad);
		TRY (enc_80_major_4C1A00 (pad, pad[4]));
	}

	pad[7] -= my_sin (pad[5]) ? 0xaf6039c : pad[12];

	if (branch == 6)
	{
		enc_80_49ECD0 (pad);
		enc_80_4E5E00 (pad, 0xB27F04F);
		TRY (enc_80_major_4E86B0 (pad, seed));
	}

	if (branch == 0x0E)
	{
		enc_80_4D79E0 (pad);
		TRY (enc_80_4DB3D0 (pad));
		TRY (enc_80_major_4D7AE0 (pad, seed));
	}

	pad[5] *= my_cos (seed) ? 0x5DF8323 : seed;

	if (branch == 8)
	{
		enc_80_4DB480 (pad, 0x1CAA72D);
		enc_80_4E8590 (pad);
		TRY (enc_80_major_49B330 (pad, seed));
	}

	if (branch == 9)
	{
		enc_80_4E5D40 (pad);
		enc_80_4D7980 (pad);
		TRY (enc_80_major_4E5F90 (pad, pad[14]));
	}

	pad[7] ^= pad[15] ^ 0x460390;

	if (branch == 7)
	{
		enc_80_4ECCC0 (pad);
		enc_80_4D57B0 (pad);
		TRY (enc_80_major_4EF430 (pad, seed));
	}

	pad[19] = 0;

	RETURN;
}

THROWS_RET enc_80_major_4AE780 (u32 *pad, u32 seed)
{
	u32 branch = pad[15] % 0x0B;

	if (branch == 5)
	{
		TRY (enc_80_4D5830 (pad));
		enc_80_4EF2A0 (pad);
		enc_80_4C50E0 (pad);
	}

	pad[4] -= my_cos (pad[14]) ? 0x76F737A3 : seed;

	if (branch == 0x0A)
	{
		enc_80_4C5240 (pad);
		enc_80_4E8620 (pad, 0xEA2E6DBA);
		enc_80_49B630 (pad, seed);
	}

	pad[3] &= pad[10] ^ 0x20773F85;

	if (branch == 4)
	{
		enc_80_4D79B0 (pad);
		enc_80_4ECC90 (pad);
		TRY (enc_80_major_4AC300 (pad, pad[14]));
	}

	pad[12] = ROL (pad[12], pad[7] * 91);

	if (branch == 9)
	{
		enc_80_4B2CB0 (pad);
		enc_80_4DB2E0 (pad);
		TRY (enc_80_major_4D7AE0 (pad, seed));
	}

	if (branch == 2)
	{
		enc_80_4E5E00 (pad, 0xE8B01B9B);
		enc_80_4E5D70 (pad);
		TRY (enc_80_major_49B330 (pad, pad[8]));
	}

	pad[3] += ROL (pad[11], 0x1d);

	if (branch == 6)
	{
		enc_80_49ECD0 (pad);
		enc_80_4ECCF0 (pad);
		TRY (enc_80_major_4E5F90 (pad, pad[0]));
	}

	if (branch == 8)
	{
		enc_80_4E8500 (pad, 0x8E0D3DC1);
		enc_80_4A1B50 (pad);
		TRY (enc_80_major_4EF430 (pad, pad[12]));
	}

	pad[9] *= pad[0] < 0xED7837 ? pad[0] : pad[15];

	if (branch == 3)
	{
		TRY (enc_80_4AEC50 (pad, 0x19068E93));
		enc_80_4ECCC0 (pad);
		TRY (enc_80_major_4C52A0 (pad, pad[15]));
	}

	if (branch == 0)
	{
		enc_80_4B2CB0 (pad);
		enc_80_4D7940 (pad);
		TRY (enc_80_major_49E930 (pad, pad[0]));
	}

	pad[16] ^= pad[6] ^ 0x28AA2736;

	if (branch == 1)
	{
		enc_80_4EF400 (pad);
		enc_80_4B2CB0 (pad);
		TRY (enc_80_major_49ED30 (pad, pad[2]));
	}

	if (branch == 0)
	{
		TRY (enc_80_4D7A90 (pad));
		enc_80_4D79E0 (pad);
		TRY (enc_80_major_4D2500 (pad, pad[6]));
	}

	pad[2] ^= pad[19] * 101;

	if (branch == 7)
	{
		enc_80_49B600 (pad, 0x2A4F7758);
		enc_80_4C19B0 (pad);
		enc_80_4C50E0 (pad);
	}

	pad[0] &= my_cos (pad[0]) ? 0xfff2b634 : pad[5];

	RETURN;
}

THROWS_RET enc_80_major_4B2FD0 (u32 *pad, u32 seed)
{
	u32 branch = (pad[1] ^ pad[16] ^ pad[18]) & 0x0F;

	if (branch == 1)
	{
		enc_80_4E5E60 (pad);
		enc_80_4E8500 (pad, 0x22072F2B);
		TRY (enc_80_major_4E5F90 (pad, pad[3]));
	}

	pad[5] += 0x37C9C771 - pad[18];

	if (branch == 0)
	{
		enc_80_4E5F30 (pad);
		enc_80_4C19B0 (pad);
		TRY (enc_80_major_4EF430 (pad, pad[1]));
	}

	if (branch == 8)
	{
		enc_80_4EF300 (pad);
		TRY (enc_80_4D7A40 (pad));
		TRY (enc_80_major_4C52A0 (pad, pad[7]));
	}

	pad[0] -= my_sin (seed) ? 0xE2D0D9 : seed;

	if (branch == 7)
	{
		enc_80_4EF280 (pad, 0x6609D8FC);
		TRY (enc_80_4D7A90 (pad));
		TRY (enc_80_major_49E930 (pad, pad[0]));
	}

	if (branch == 6)
	{
		enc_80_4C5060 (pad);
		TRY (enc_80_4EF350 (pad));
		TRY (enc_80_major_49ED30 (pad, pad[0]));
	}

	pad[11] = ROR (pad[11], my_cos (seed) ? 0xf1085c5 : pad[11]);

	if (branch == 1)
	{
		enc_80_4E8500 (pad, 0xEA082B69);
		enc_80_4E5EB0 (pad, 0x2CDA662B);
		TRY (enc_80_major_4D2500 (pad, pad[3]));
	}

	if (branch == 0x0F)
	{
		TRY (enc_80_4D7A90 (pad));
		enc_80_4C5270 (pad);
		TRY (enc_80_major_4AE780 (pad, pad[18]));
	}

	pad[17] -= ROL (pad[11], 0x18);

	if (branch == 2)
	{
		enc_80_49ECD0 (pad);
		enc_80_4E85C0 (pad);
		TRY (enc_80_major_4F3220 (pad, pad[17]));
	}

	if (branch == 9)
	{
		enc_80_49ECD0 (pad);
		enc_80_49B280 (pad);
		TRY (enc_80_major_4C1A00 (pad, seed));
	}

	if (branch == 0x0A)
	{
		enc_80_49ECD0 (pad);
		enc_80_4EF280 (pad, 0x27F49EA2);
		TRY (enc_80_major_4E86B0 (pad, pad[5]));
	}

	if (branch == 0x0E)
	{
		TRY (enc_80_4DB3D0 (pad));
		enc_80_4C5240 (pad);
		TRY (enc_80_major_4AC560 (pad, pad[0]));
	}

	if (branch == 0x0C)
	{
		TRY (enc_80_4AC2B0 (pad));
		enc_80_4C5060 (pad);
		TRY (enc_80_major_4BB410 (pad, pad[18]));
	}

	if (branch == 3)
	{
		enc_80_4C5270 (pad);
		enc_80_4A1B20 (pad, 0xEDBBBAEC);
		TRY (enc_80_major_4E5F90 (pad, pad[12]));
	}

	pad[14] ^= pad[4] | 0xC893A6;

	if (branch == 0x0B)
	{
		enc_80_4D7940 (pad);
		enc_80_4A1B20 (pad, 0x1FAC3747);
		TRY (enc_80_major_4EF430 (pad, pad[12]));
	}

	if (branch == 0)
	{
		TRY (enc_80_4AEC50 (pad, 0x398C59E0));
		TRY (enc_80_4D5760 (pad));
		TRY (enc_80_major_4C52A0 (pad, pad[15]));
	}

	pad[19] ^= my_cos (pad[14]) ? 0x4d552e : pad[10];

	if (branch == 0x0D)
	{
		enc_80_4A1B80 (pad);
		enc_80_4DB000 (pad, 0x15091EA6);
		TRY (enc_80_major_49E930 (pad, pad[2]));
	}

	pad[13] = ((pad[13] * 27) << 1) - pad[13];

	if (branch == 4)
	{
		TRY (enc_80_4B2BC0 (pad, 0xD48681AE));
		enc_80_4DB000 (pad, 0x19716A46);
		TRY (enc_80_major_49ED30 (pad, pad[6]));
	}

	if (branch == 5)
	{
		TRY (enc_80_4E5EE0 (pad));
		enc_80_4E8500 (pad, 0xCC56F70A);
		TRY (enc_80_major_4D2500 (pad, pad[16]));
	}

	pad[8] ^= pad[10] + 0x4d757e;

	RETURN;
}

THROWS_RET enc_80_major_4BB410 (u32 *pad, u32 seed)
{
	u32 branch = (pad[12] ^ pad[2] ^ pad[19]) & 0x0F;

	if (branch == 0x0F)
	{
		TRY (enc_80_4DB3D0 (pad));
		enc_80_4ECC60 (pad);
		TRY (enc_80_major_49B330 (pad, pad[19]));
	}

	pad[3] += ((seed * 3) << 4) + seed;

	if (branch == 9)
	{
		TRY (enc_80_49E8E0 (pad, 0x4AE8ED5A));
		enc_80_4E5F30 (pad);
		TRY (enc_80_major_4E5F90 (pad, seed));
	}

	if (branch == 3)
	{
		enc_80_4E5D40 (pad);
		enc_80_4D57B0 (pad);
		TRY (enc_80_major_4EF430 (pad, seed));
	}

	pad[9] -= pad[2] & 0x8742C2;

	if (branch == 6)
	{
		TRY (enc_80_4EF3B0 (pad));
		enc_80_4E5E00 (pad, 0xE37DF00A);
		TRY (enc_80_major_4C52A0 (pad, seed));
	}

	seed ^= pad[6] < 0x4c2ac4 ? pad[6] : pad[5];

	if (branch == 0x0C)
	{
		enc_80_4EF280 (pad, 0xF579E970);
		enc_80_4D79E0 (pad);
		TRY (enc_80_major_49E930 (pad, seed));
	}

	if (branch == 0x0E)
	{
		enc_80_4EF200 (pad, 0x39259610);
		enc_80_4E5F30 (pad);
		enc_80_major_49ED30 (pad, seed);
	}

	pad[8] ^= my_sqrt (pad[17]);

	if (branch == 4)
	{
		TRY (enc_80_4B2BC0 (pad, 0xCAD836AF));
		enc_80_4ECCC0 (pad);
		TRY (enc_80_major_4D2500 (pad, pad[16]));
	}

	pad[1] += 0x904F9E8D - pad[5];

	if (branch == 2)
	{
		TRY (enc_80_4DB3D0 (pad));
		TRY (enc_80_4B2C60 (pad));
		TRY (enc_80_major_4AE780 (pad, pad[2]));
	}

	if (branch == 0)
	{
		enc_80_4A1B20 (pad, 0xBDE6C765);
		enc_80_4B2CE0 (pad);
		TRY (enc_80_major_4F3220 (pad, pad[17]));
	}

	pad[5] += pad[6] - 0x49781f;

	if (branch == 5)
	{
		TRY (enc_80_4ECBE0 (pad));
		enc_80_4EF2A0 (pad);
		TRY (enc_80_major_4C1A00 (pad, pad[8]));
	}

	if (branch == 0x0D)
	{
		enc_80_4C5270 (pad);
		TRY (enc_80_49E8E0 (pad, 0x73D5887D));
		TRY (enc_80_major_4E86B0 (pad, seed));
	}

	pad[0] ^= pad[6] + 0x356DCF35;

	if (branch == 1)
	{
		TRY (enc_80_4B2C60 (pad));
		enc_80_4E8500 (pad, 0xCA4F0A68);
		TRY (enc_80_major_4AC560 (pad, pad[0]));
	}

	pad[8] ^= ROL (pad[8], 0x1d);

	if (branch == 0x0B)
	{
		enc_80_4EF2A0 (pad);
		enc_80_4ECC60 (pad);
		TRY (enc_80_major_49B330 (pad, pad[9]));
	}

	if (branch == 8)
	{
		enc_80_4C5240 (pad);
		enc_80_4A1B20 (pad, 0xED213C2A);
		TRY (enc_80_major_4E5F90 (pad, seed));
	}

	pad[19] |= pad[4] * 73;

	if (branch == 0x0A)
	{
		enc_80_4ECC60 (pad);
		TRY (enc_80_4DB3D0 (pad));
		TRY (enc_80_major_4EF430 (pad, pad[2]));
	}

	pad[11] += 0xb888d - pad[10];

	if (branch == 7)
	{
		enc_80_4ECCF0 (pad);
		enc_80_4ECC60 (pad);
		TRY (enc_80_major_4C52A0 (pad, seed));
	}

	if (branch == 0)
	{
		TRY (enc_80_4D5830 (pad));
		TRY (enc_80_4C5090 (pad, 0x14496A43));
		TRY (enc_80_major_49E930 (pad, pad[19]));
	}

	pad[17] ^= (pad[3] * 5) << 3;

	RETURN;
}

THROWS_RET enc_80_major_4C1A00 (u32 *pad, u32 seed)
{
	u32 branch = (pad[0] ^ pad[3] ^ seed) % 0x0D;

	if (branch == 0)
	{
		enc_80_4C19E0 (pad);
		enc_80_4A1B80 (pad);
		enc_80_49B630 (pad, pad[18]);
	}

	pad[18] *= seed * 25;

	if (branch == 1)
	{
		TRY (enc_80_4D7A90 (pad));
		enc_80_4E8680 (pad);
		TRY (enc_80_major_4AC300 (pad, pad[19]));
	}

	pad[12] &= pad[8] ^ 0x3038E4EB;

	if (branch == 3)
	{
		enc_80_4E8530 (pad);
		TRY (enc_80_4ECBE0 (pad));
		TRY (enc_80_major_4D7AE0 (pad, pad[6]));
	}

	if (branch == 6)
	{
		TRY (enc_80_4ECBE0 (pad));
		enc_80_4ECC60 (pad);
		TRY (enc_80_major_49B330 (pad, pad[12]));
	}

	seed ^= ROR (pad[0], 0x9);

	if (branch == 2)
	{
		enc_80_4EF2A0 (pad);
		enc_80_4DB2E0 (pad);
		TRY (enc_80_major_4E5F90 (pad, pad[0]));
	}

	pad[8] -= ROR (pad[6], 0x1c);

	if (branch == 0x0B)
	{
		TRY (enc_80_4AC2B0 (pad));
		TRY (enc_80_4D5830 (pad));
		TRY (enc_80_major_4EF430 (pad, pad[8]));
	}

	if (branch == 8)
	{
		enc_80_4C5240 (pad);
		TRY (enc_80_4DB090 (pad));
		TRY (enc_80_major_4C52A0 (pad, seed));
	}

	pad[17] ^= pad[2] + 0x187F79E8;

	if (branch == 0)
	{
		enc_80_4DB030 (pad);
		TRY (enc_80_4D7A90 (pad));
		TRY (enc_80_major_49E930 (pad, pad[12]));
	}

	pad[13] ^= (seed * 25) << 2;

	if (branch == 9)
	{
		TRY (enc_80_4AC2B0 (pad));
		enc_80_49B280 (pad);
		TRY (enc_80_major_49ED30 (pad, pad[3]));
	}

	if (branch == 0x0A)
	{
		TRY (enc_80_4E5EE0 (pad));
		enc_80_4EF280 (pad, 0xC2B1F58);
		TRY (enc_80_major_4D2500 (pad, pad[9]));
	}

	pad[8] = ROR (pad[8], pad[17] - 0x12c0bc1b);

	if (branch == 0x0C)
	{
		TRY (enc_80_4DB090 (pad));
		TRY (enc_80_4EF3B0 (pad));
		TRY (enc_80_major_4AE780 (pad, pad[18]));
	}

	pad[11] ^= pad[12] - 0x409402;

	if (branch == 7)
	{
		enc_80_4E5D40 (pad);
		enc_80_4E8590 (pad);
		TRY (enc_80_major_4F3220 (pad, seed));
	}

	pad[4] = ROR (pad[4], pad[6] - 0x409402);

	if (branch == 5)
	{
		enc_80_4D57B0 (pad);
		enc_80_49B600 (pad, 0x52374450);
		enc_80_49B630 (pad, pad[19]);
	}

	if (branch == 4)
	{
		enc_80_4EF280 (pad, 0x29B6CBD2);
		enc_80_4EF300 (pad);
		TRY (enc_80_major_4AC300 (pad, pad[19]));
	}

	pad[3] ^= pad[6] - 0x400000;

	RETURN;
}

THROWS_RET enc_80_major_4C52A0 (u32 *pad, u32 seed)
{
	u32 branch = pad[5] % 0x07;

	pad[2] -= seed ^ 0x1E9E3A1C;

	if (branch == 4)
	{
		enc_80_4B2CE0 (pad);
		enc_80_4D79E0 (pad);
		enc_80_4C50E0 (pad);
	}

	pad[9] &= pad[4] ^ 0x46b160;

	if (branch == 5)
	{
		enc_80_4C5270 (pad);
		enc_80_4EF2A0 (pad);
		enc_80_49B630 (pad, pad[11]);
	}

	seed += pad[17] ^ 0x46b160;

	if (branch == 1)
	{
		TRY (enc_80_4B2BC0 (pad, 0xC6D7E2F7));
		enc_80_49B2E0 (pad);
		TRY (enc_80_major_4AC300 (pad, seed));
	}

	pad[13] |= my_sqrt (pad[19]);

	pad[7] -= my_cos (pad[10]) ? 0x90069b : pad[17];

	if (branch == 0)
	{
		TRY (enc_80_4D5760 (pad));
		enc_80_4D7940 (pad);
		TRY (enc_80_major_4D7AE0 (pad, pad[7]));
	}

	pad[15] -= 0x90069b | seed;

	if (branch == 6)
	{
		enc_80_4C19B0 (pad);
		enc_80_4E8500 (pad, 0xCE673B32);
		TRY (enc_80_major_49B330 (pad, pad[1]));
	}

	seed += pad[6] & 0x44782589;

	if (branch == 3)
	{
		enc_80_4E5D40 (pad);
		TRY (enc_80_4D5760 (pad));
		TRY (enc_80_major_4E5F90 (pad, seed));
	}

	pad[8] += pad[11] & 0x2645009B;

	if (branch == 0)
	{
		enc_80_4E8500 (pad, 0xC827DEB1);
		enc_80_4E85C0 (pad);
		TRY (enc_80_major_4EF430 (pad, pad[10]));
	}

	pad[8] *= pad[16] ^ 0x2666A4CB;

	RETURN;
}

THROWS_RET enc_80_major_4CFE70 (u32 *pad, u32 seed)
{
	u32 branch = (pad[2] ^ pad[6] ^ seed) & 0x0F;

	if (branch == 4)
	{
		enc_80_4DB270 (pad, 0x21186FAA);
		enc_80_4C5270 (pad);
		TRY (enc_80_major_4C52A0 (pad, pad[6]));
	}

	if (branch == 0x0A)
	{
		enc_80_49B310 (pad);
		enc_80_4DB270 (pad, 0x4AFBD7FD);
		TRY (enc_80_major_49E930 (pad, pad[0]));
	}

	pad[15] += ROR (pad[16], 0x5);

	if (branch == 2)
	{
		enc_80_4D7A10 (pad);
		enc_80_49ECD0 (pad);
		TRY (enc_80_major_49ED30 (pad, pad[4]));
	}

	if (branch == 9)
	{
		enc_80_4EF200 (pad, 0x659B54D7);
		enc_80_4DB240 (pad);
		TRY (enc_80_major_4D2500 (pad, seed));
	}

	seed = (pad[3] * seed * 19) << 1;

	if (branch == 8)
	{
		enc_80_4C19B0 (pad);
		enc_80_4DB030 (pad);
		TRY (enc_80_major_4AE780 (pad, pad[8]));
	}

	if (branch == 3)
	{
		enc_80_4EF280 (pad, 0x15993072);
		enc_80_4EF2A0 (pad);
		TRY (enc_80_major_4F3220 (pad, pad[14]));
	}

	pad[11] *= seed + 0x19948E09;

	if (branch == 3)
	{
		enc_80_4D7A10 (pad);
		enc_80_4DB2E0 (pad);
		TRY (enc_80_major_4C1A00 (pad, seed));
	}

	if (branch == 0x0E)
	{
		TRY (enc_80_49E8E0 (pad, 0x1B85EB94));
		TRY (enc_80_49E8E0 (pad, 0xA3F69A0B));
		TRY (enc_80_major_4E86B0 (pad, seed));
	}

	pad[2] = ROR (pad[2], seed - 0xc5ef5be);

	if (branch == 0)
	{
		enc_80_4ECC60 (pad);
		enc_80_4E8590 (pad);
		TRY (enc_80_major_4AC560 (pad, seed));
	}

	if (branch == 0x0F)
	{
		enc_80_4E5F30 (pad);
		enc_80_4E8530 (pad);
		TRY (enc_80_major_4BB410 (pad, seed));
	}

	pad[12] -= my_cos (seed) ? 0xcaba2d3 : pad[9];

	if (branch == 0x0B)
	{
		enc_80_4E5EB0 (pad, 0x27BA9331);
		enc_80_4E8500 (pad, 0xB888054B);
		TRY (enc_80_major_4B2FD0 (pad, seed));
	}

	if (branch == 7)
	{
		enc_80_4A1B50 (pad);
		enc_80_4DB480 (pad, 0xE89F6224);
		TRY (enc_80_major_4D2900 (pad, pad[1]));
	}

	pad[17] -= pad[17] < 0xcaba2d3 ? pad[17] : pad[18];

	if (branch == 0)
	{
		enc_80_4D57B0 (pad);
		enc_80_4D79E0 (pad);
		TRY (enc_80_major_4C52A0 (pad, pad[10]));
	}

	if (branch == 1)
	{
		enc_80_4A1B20 (pad, 0x4C87F8E5);
		enc_80_4C5270 (pad);
		TRY (enc_80_major_49E930 (pad, pad[17]));
	}

	pad[6] += pad[0] - 0xD6249F6;

	if (branch == 0x0C)
	{
		TRY (enc_80_4EF3B0 (pad));
		TRY (enc_80_4ECBE0 (pad));
		TRY (enc_80_major_49ED30 (pad, pad[6]));
	}

	if (branch == 1)
	{
		enc_80_49ECD0 (pad);
		enc_80_4EF200 (pad, 0x200D9DB1);
		TRY (enc_80_major_4D2500 (pad, seed));
	}

	pad[5] += ((pad[16] * 7) << 4) + pad[16];

	if (branch == 6)
	{
		enc_80_4DB240 (pad);
		TRY (enc_80_4D7A40 (pad));
		TRY (enc_80_major_4AE780 (pad, pad[8]));
	}

	if (branch == 2)
	{
		TRY (enc_80_4B2C60 (pad));
		TRY (enc_80_4EF230 (pad));
		TRY (enc_80_major_4F3220 (pad, pad[15]));
	}

	pad[11] |= pad[13] * 0x11;

	if (branch == 0x0D)
	{
		enc_80_49B600 (pad, 0x1CF92975);
		TRY (enc_80_4AEC50 (pad, 0x2F50247E));
		TRY (enc_80_major_4C1A00 (pad, pad[11]));
	}

	if (branch == 5)
	{
		enc_80_4EF200 (pad, 0x36559050);
		enc_80_4A1B50 (pad);
		TRY (enc_80_major_4E86B0 (pad, seed));
	}

	pad[10] += pad[10] ^ 0x53350A5E;

	RETURN;
}

THROWS_RET enc_80_major_4D2500 (u32 *pad, u32 seed)
{
	u32 branch = (pad[15] ^ pad[1] ^ pad[18]) % 0x0A;

	pad[14] *= pad[18] - 0x3438A280;

	if (branch == 9)
	{
		TRY (enc_80_4B2BC0 (pad, 0xDC6C3D21));
		enc_80_4ECC60 (pad);
		enc_80_4C50E0 (pad);
	}

	pad[2] += pad[3] & 0x244268df;

	if (branch == 4)
	{
		enc_80_4EF300 (pad);
		enc_80_4DB000 (pad, 0xFA8D40AB);
		enc_80_49B630 (pad, seed);
	}

	pad[18] *= ROL (seed, 0x1f);

	if (branch == 5)
	{
		enc_80_4DB480 (pad, 0x44A427);
		TRY (enc_80_4D5760 (pad));
		TRY (enc_80_major_4AC300 (pad, pad[14]));
	}

	pad[6] += pad[10] + 0x5308B1DB;

	if (branch == 1)
	{
		enc_80_4C19B0 (pad);
		enc_80_4AECA0 (pad);
		TRY (enc_80_major_4D7AE0 (pad, seed));
	}

	pad[5] += ROR (pad[8], 0x19);

	if (branch == 0)
	{
		TRY (enc_80_4E5EE0 (pad));
		enc_80_4A1B50 (pad);
		TRY (enc_80_major_49B330 (pad, pad[5]));
	}

	pad[9] |= seed & 0x27BE8251;

	if (branch == 2)
	{
		TRY (enc_80_4B2BC0 (pad, 0x9DBE27AD));
		TRY (enc_80_4AC2B0 (pad));
		TRY (enc_80_major_4E5F90 (pad, pad[9]));
	}

	pad[12] ^= pad[2] * 73;

	if (branch == 8)
	{
		enc_80_4EF400 (pad);
		enc_80_4E8590 (pad);
		TRY (enc_80_major_4EF430 (pad, seed));
	}

	pad[14] &= seed - 0x7413C08;

	if (branch == 6)
	{
		enc_80_4D79B0 (pad);
		TRY (enc_80_4AEC50 (pad, 0x2725C3FB));
		TRY (enc_80_major_4C52A0 (pad, pad[17]));
	}

	pad[9] += 0x4c3ada - seed;

	if (branch == 3)
	{
		enc_80_4C5060 (pad);
		enc_80_4D7A10 (pad);
		TRY (enc_80_major_49E930 (pad, seed));
	}

	seed += 0x8ea63 | pad[18];

	if (branch == 0)
	{
		enc_80_4C5060 (pad);
		enc_80_4E8530 (pad);
		TRY (enc_80_major_49ED30 (pad, pad[19]));
	}

	pad[3] += seed < 0x4efe6b ? seed : pad[4];

	RETURN;
}

THROWS_RET enc_80_major_4D2900 (u32 *pad, u32 seed)
{
	u32 branch = (pad[11] ^ pad[6] ^ pad[19]) & 0x0F;

	if (branch == 7)
	{
		TRY (enc_80_4D57E0 (pad));
		enc_80_4A1B50 (pad);
		TRY (enc_80_major_4EF430 (pad, seed));
	}

	pad[18] = ROR (pad[18], pad[6] + 0x50bb3362);

	if (branch == 0x0D)
	{
		enc_80_4ECC60 (pad);
		TRY (enc_80_4D57E0 (pad));
		TRY (enc_80_major_4C52A0 (pad, pad[14]));
	}

	if (branch == 0x0F)
	{
		TRY (enc_80_4B2BC0 (pad, 0xCE7048A1));
		enc_80_4E5D40 (pad);
		TRY (enc_80_major_49E930 (pad, pad[0]));
	}

	pad[6] -= my_cos (pad[14]) ? 0x50ff65ed : pad[9];

	if (branch == 2)
	{
		enc_80_4EF200 (pad, 0x1F00A68E);
		enc_80_4E5E60 (pad);
		TRY (enc_80_major_49ED30 (pad, pad[13]));
	}

	if (branch == 8)
	{
		enc_80_4ECC60 (pad);
		enc_80_4D7A10 (pad);
		TRY (enc_80_major_4D2500 (pad, seed));
	}

	seed ^= ROR (seed, 0xd);

	if (branch == 0x0A)
	{
		enc_80_4E8620 (pad, 0xDF91CB30);
		enc_80_4B2CE0 (pad);
		TRY (enc_80_major_4AE780 (pad, seed));
	}

	if (branch == 5)
	{
		enc_80_4E5DA0 (pad, 0x7D8CAF0B);
		enc_80_4D7940 (pad);
		TRY (enc_80_major_4F3220 (pad, pad[10]));
	}

	pad[17] ^= pad[4] | 0x50ffe7ed;

	if (branch == 3)
	{
		enc_80_49B2E0 (pad);
		enc_80_4EF400 (pad);
		TRY (enc_80_major_4C1A00 (pad, pad[17]));
	}

	if (branch == 0)
	{
		enc_80_4ECCC0 (pad);
		TRY (enc_80_4B2C60 (pad));
		TRY (enc_80_major_4E86B0 (pad, pad[8]));
	}

	seed ^= seed < 0x5149b5c9 ? seed : pad[5];

	if (branch == 0x0B)
	{
		enc_80_4D7980 (pad);
		enc_80_4EF280 (pad, 0x3337FB6C);
		TRY (enc_80_major_4AC560 (pad, seed));
	}

	if (branch == 2)
	{
		TRY (enc_80_4DB090 (pad));
		TRY (enc_80_4B2C60 (pad));
		TRY (enc_80_major_4BB410 (pad, pad[1]));
	}

	pad[9] += pad[7] ^ 0x5149b5c9;

	if (branch == 0x0E)
	{
		enc_80_4DB000 (pad, 0x21CDDA75);
		enc_80_49B280 (pad);
		TRY (enc_80_major_4B2FD0 (pad, pad[6]));
	}

	if (branch == 1)
	{
		enc_80_4DB030 (pad);
		enc_80_4A1B50 (pad);
		TRY (enc_80_major_4EF430 (pad, pad[19]));
	}

	pad[17] |= pad[10] - 0x514ffdff;

	if (branch == 6)
	{
		enc_80_4EF2A0 (pad);
		enc_80_4D57B0 (pad);
		TRY (enc_80_major_4C52A0 (pad, pad[8]));
	}

	if (branch == 9)
	{
		enc_80_4EF280 (pad, 0x1D7942BA);
		TRY (enc_80_4C5090 (pad, 0xF46A752));
		TRY (enc_80_major_49E930 (pad, pad[2]));
	}

	pad[6] ^= my_cos (pad[13]) ? 0xB29627F : seed;

	if (branch == 0x0C)
	{
		enc_80_49ECD0 (pad);
		enc_80_49B2E0 (pad);
		TRY (enc_80_major_49ED30 (pad, pad[5]));
	}

	if (branch == 0)
	{
		enc_80_4EF400 (pad);
		TRY (enc_80_4AEC50 (pad, 0x664D4285));
		TRY (enc_80_major_4D2500 (pad, pad[14]));
	}

	if (branch == 4)
	{
		TRY (enc_80_4AEC50 (pad, 0x281B56A6));
		TRY (enc_80_4D7A90 (pad));
		TRY (enc_80_major_4AE780 (pad, pad[18]));
	}

	if (branch == 1)
	{
		enc_80_4ECCC0 (pad);
		enc_80_4E8590 (pad);
		TRY (enc_80_major_4F3220 (pad, pad[4]));
	}

	pad[16] *= pad[3] * 0x3a94d;

	RETURN;
}

THROWS_RET enc_80_major_4D7AE0 (u32 *pad, u32 seed)
{
	u32 branch = (pad[13] ^ pad[8] ^ pad[17]) % 0x03;

	seed ^= 0x39aa3db4 ^ pad[17];

	pad[15] = ROL (pad[15], my_sin (seed) ? 0x236D13F3 : pad[19]);
	pad[16] ^= my_sqrt (seed);

	if (branch == 0)
	{
		enc_80_49ECD0 (pad);
		enc_80_4EF300 (pad);
		enc_80_4C50E0 (pad);
	}

	seed = (seed * seed * 5) << 4;

	pad[18] |= pad[8] * 0x1E4B94EC;
	pad[3] += seed & 0x2708964F;

	if (branch == 0)
	{
		enc_80_4D7940 (pad);
		enc_80_4E5D40 (pad);
		enc_80_49B630 (pad, seed);
	}

	pad[0] -= pad[10] | 0x390ac159;

	pad[8] -= my_cos (pad[6]) ? 0x84019 : pad[10];
	pad[17] &= 0x84019 | seed;

	pad[14] *= ROL (pad[2], 0x1f);

	if (branch == 1)
	{
		TRY (enc_80_4D7A90 (pad));
		enc_80_4E5DD0 (pad);
		TRY (enc_80_major_4AC300 (pad, pad[7]));
	}

	pad[8] *= pad[15] - 0x51214;

	RETURN;
}

THROWS_RET enc_80_major_4DB520 (u32 *pad, u32 seed)
{
	u32 branch = (pad[15] ^ pad[4] ^ pad[19]) & 0x0F;

	if (branch == 8)
	{
		enc_80_4C19B0 (pad);
		TRY (enc_80_4D7A40 (pad));
		TRY (enc_80_major_49ED30 (pad, pad[12]));
	}

	pad[16] += 0x19deda30 - pad[1];

	if (branch == 2)
	{
		enc_80_4EF280 (pad, 0x60A53BB0);
		TRY (enc_80_4F31A0 (pad));
		TRY (enc_80_major_4D2500 (pad, seed));
	}

	if (branch == 3)
	{
		enc_80_49B310 (pad);
		enc_80_4EF280 (pad, 0x56DE6466);
		TRY (enc_80_major_4AE780 (pad, seed));
	}

	seed -= my_cos (pad[10]) ? 0x199972a0 : seed;

	if (branch == 4)
	{
		enc_80_49B2E0 (pad);
		enc_80_4DB270 (pad, 0x46E1A196);
		TRY (enc_80_major_4F3220 (pad, seed));
	}

	if (branch == 9)
	{
		enc_80_4EF400 (pad);
		TRY (enc_80_4EF230 (pad));
		TRY (enc_80_major_4C1A00 (pad, pad[16]));
	}

	pad[16] *= my_sqrt (pad[2]);

	if (branch == 0x0E)
	{
		enc_80_4E5EB0 (pad, 0x103EC068);
		enc_80_4C5240 (pad);
		TRY (enc_80_major_4E86B0 (pad, pad[8]));
	}

	if (branch == 5)
	{
		enc_80_4D79E0 (pad);
		enc_80_4A1B50 (pad);
		TRY (enc_80_major_4AC560 (pad, pad[9]));
	}

	pad[18] -= my_sqrt (pad[2]);

	if (branch == 6)
	{
		enc_80_4E5E00 (pad, 0x903B6F4);
		enc_80_4B2CB0 (pad);
		TRY (enc_80_major_4BB410 (pad, seed));
	}

	if (branch == 1)
	{
		enc_80_4C5270 (pad);
		enc_80_4DB2E0 (pad);
		TRY (enc_80_major_4B2FD0 (pad, pad[13]));
	}

	seed += pad[8] < 0x2B940916 ? pad[8] : seed;

	if (branch == 3)
	{
		enc_80_4E5DA0 (pad, 0xE7559324);
		TRY (enc_80_4EF230 (pad));
		TRY (enc_80_major_4D2900 (pad, seed));
	}

	if (branch == 2)
	{
		enc_80_4E8500 (pad, 0x445B7A34);
		TRY (enc_80_4D7A90 (pad));
		TRY (enc_80_major_4CFE70 (pad, pad[5]));
	}

	seed *= pad[5] + 0x1D13557B;

	if (branch == 0x0B)
	{
		TRY (enc_80_4B2BC0 (pad, 0xF4F5ADCC));
		enc_80_49ECD0 (pad);
		TRY (enc_80_major_4ECD20 (pad, pad[17]));
	}

	if (branch == 0x0A)
	{
		enc_80_4E5E60 (pad);
		TRY (enc_80_4ECBE0 (pad));
		enc_80_major_49ED30 (pad, seed);
	}

	pad[0] -= my_sqrt (seed);

	if (branch == 0x0D)
	{
		enc_80_4A1B80 (pad);
		TRY (enc_80_4EF350 (pad));
		TRY (enc_80_major_4D2500 (pad, pad[17]));
	}

	if (branch == 0)
	{
		TRY (enc_80_4EF230 (pad));
		TRY (enc_80_4D7A40 (pad));
		TRY (enc_80_major_4AE780 (pad, pad[19]));
	}

	pad[18] += 0xffb953f8 + seed;

	if (branch == 0x0F)
	{
		TRY (enc_80_4EF350 (pad));
		enc_80_4E5F30 (pad);
		TRY (enc_80_major_4F3220 (pad, seed));
	}

	if (branch == 1)
	{
		TRY (enc_80_4E5EE0 (pad));
		enc_80_4D79B0 (pad);
		TRY (enc_80_major_4C1A00 (pad, pad[8]));
	}

	pad[3] ^= my_cos (seed) ? 0xffb953f8 : pad[4];

	if (branch == 7)
	{
		enc_80_49B2E0 (pad);
		enc_80_4ECC90 (pad);
		TRY (enc_80_major_4E86B0 (pad, pad[1]));
	}

	if (branch == 0)
	{
		enc_80_4ECC90 (pad);
		enc_80_49ECD0 (pad);
		TRY (enc_80_major_4AC560 (pad, pad[8]));
	}

	pad[1] ^= pad[3] * 0x620312F0;

	if (branch == 0x0C)
	{
		enc_80_49B280 (pad);
		enc_80_4E8620 (pad, 0xB7C094D3);
		TRY (enc_80_major_4BB410 (pad, pad[12]));
	}

	pad[7] ^= my_sqrt (pad[10]);

	if (branch == 4)
	{
		enc_80_4B2CE0 (pad);
		enc_80_4E5DD0 (pad);
		TRY (enc_80_major_4B2FD0 (pad, pad[4]));
	}

	if (branch == 5)
	{
		enc_80_4D57B0 (pad);
		enc_80_4ECCF0 (pad);
		TRY (enc_80_major_4D2900 (pad, seed));
	}

	pad[18] ^= pad[14] + 0x9AD3708;

	RETURN;
}

THROWS_RET enc_80_mix (u32 *pad, u32 seed)
{
	u32 branch = (pad[7] ^ pad[11] ^ pad[18]) & 0x0F;

	if (branch == 0x0E)
	{
		enc_80_4A1B20 (pad, 0xD2CA38AA);
		enc_80_4A1B50 (pad);
		TRY (enc_80_major_4AE780 (pad, seed));
	}

	if (branch == 9)
	{
		enc_80_4C5060 (pad);
		enc_80_49ECD0 (pad);
		TRY (enc_80_major_4F3220 (pad, pad[7]));
	}

	pad[11] += 0 - (((pad[18] * 5) << 4) - pad[18]);

	if (branch == 0x0C)
	{
		enc_80_4C5270 (pad);
		enc_80_4E8680 (pad);
		TRY (enc_80_major_4C1A00 (pad, pad[2]));
	}

	if (branch == 3)
	{
		enc_80_4E8500 (pad, 0x32372936);
		TRY (enc_80_4EF230 (pad));
		TRY (enc_80_major_4E86B0 (pad, pad[7]));
	}

	if (branch == 0x0B)
	{
		enc_80_49ECD0 (pad);
		enc_80_4A1B80 (pad);
		TRY (enc_80_major_4AC560 (pad, pad[17]));
	}

	pad[19] ^= ROR (pad[12], 0x1d);

	if (branch == 4)
	{
		TRY (enc_80_4D57E0 (pad));
		enc_80_4B2CB0 (pad);
		TRY (enc_80_major_4BB410 (pad, pad[1]));
	}

	if (branch == 2)
	{
		TRY (enc_80_4D5830 (pad));
		TRY (enc_80_4EF3B0 (pad));
		TRY (enc_80_major_4B2FD0 (pad, pad[2]));
	}

	seed ^= pad[14] - 0x12821;

	if (branch == 4)
	{
		enc_80_4A1B20 (pad, 0x556A5BA2);
		enc_80_4C5240 (pad);
		TRY (enc_80_major_4D2900 (pad, seed));
	}

	if (branch == 1)
	{
		TRY (enc_80_4AC2B0 (pad));
		enc_80_4ECC90 (pad);
		TRY (enc_80_major_4CFE70 (pad, seed));
	}

	if (branch == 0)
	{
		enc_80_4D79E0 (pad);
		enc_80_4EF2A0 (pad);
		TRY (enc_80_major_4ECD20 (pad, pad[11]));
	}

	pad[4] |= 0x472ca7 ^ pad[4];	// 0x10468B00

	if (branch == 0x0D)
	{
		enc_80_4D7980 (pad);
		TRY (enc_80_4AC2B0 (pad));
		TRY (enc_80_major_4DB520 (pad, pad[13]));
	}

	if (branch == 0x0F)
	{
		enc_80_4DB2E0 (pad);
		enc_80_4C5270 (pad);
		TRY (enc_80_major_4A1BB0 (pad, pad[13]));
	}

	pad[13] -= pad[16] < 0x97CBA53 ? pad[16] : seed;

	if (branch == 2)
	{
		enc_80_4E8590 (pad);
		TRY (enc_80_4C5090 (pad, 0xD1006F43));
		TRY (enc_80_major_4AE780 (pad, pad[17]));
	}

	if (branch == 6)
	{
		TRY (enc_80_4EF230 (pad));
		TRY (enc_80_4F31A0 (pad));
		TRY (enc_80_major_4F3220 (pad, pad[15]));
	}

	if (branch == 7)
	{
		enc_80_4DB270 (pad, 0xAD4B36B);
		enc_80_4DB240 (pad);
		TRY (enc_80_major_4C1A00 (pad, pad[8]));
	}

	pad[10] ^= ((pad[2] << 5) - pad[2]) << 1;

	if (branch == 6)
	{
		TRY (enc_80_49E8E0 (pad, 0x171DFEAC));
		enc_80_4C5060 (pad);
		TRY (enc_80_major_4E86B0 (pad, pad[16]));
	}

	if (branch == 5)
	{
		enc_80_4D7A10 (pad);
		enc_80_4E5EB0 (pad, 0x87C73F);
		TRY (enc_80_major_4AC560 (pad, pad[15]));
	}

	pad[13] &= pad[9] ^ 0x473fe6;

	if (branch == 7)
	{
		enc_80_4EF2A0 (pad);
		enc_80_4ECC60 (pad);
		TRY (enc_80_major_4BB410 (pad, seed));
	}

	if (branch == 1)
	{
		TRY (enc_80_4F31A0 (pad));
		TRY (enc_80_4D5760 (pad));
		TRY (enc_80_major_4B2FD0 (pad, pad[5]));
	}

	if (branch == 0)
	{
		enc_80_4ECCC0 (pad);
		enc_80_49B2E0 (pad);
		TRY (enc_80_major_4D2900 (pad, pad[17]));
	}

	pad[1] -= ROL (pad[7], 0xc);

	if (branch == 8)
	{
		enc_80_4E8620 (pad, 0xC80536A9);
		enc_80_4E8680 (pad);
		TRY (enc_80_major_4CFE70 (pad, pad[2]));
	}

	if (branch == 0x0A)
	{
		enc_80_4E5E00 (pad, 0xF611CB26);
		enc_80_4E8500 (pad, 0x8A2C966F);
		TRY (enc_80_major_4ECD20 (pad, pad[13]));
	}

	pad[10] -= pad[0] < 0x6c6c ? pad[0] : pad[5];

	if (branch == 3)
	{
		enc_80_4A1B20 (pad, 0x6795F8E2);
		enc_80_4D7A10 (pad);
		TRY (enc_80_major_4DB520 (pad, pad[0]));
	}

	if (branch == 5)
	{
		enc_80_4E8680 (pad);
		enc_80_4B2CE0 (pad);
		TRY (enc_80_major_4A1BB0 (pad, pad[16]));
	}

	pad[5] += ((seed << 5) + seed) * 3;

	RETURN;
}
