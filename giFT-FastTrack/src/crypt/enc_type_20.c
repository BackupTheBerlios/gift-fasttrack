/*
 * $Id: enc_type_20.c,v 1.5 2003/07/14 19:34:14 weinholt Exp $
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
 * This is the most recent pad mingling code for FastTrack as of 03/04/28
 * Used for encryption version 0x20
 */

/* our crude SEH replacement */

typedef int THROWS_RET;
#define THROW return 1;
#define TRY(x) { if((x) == 1) return 0; }
#define RETURN return 0;

/* a macro for easier access to the key */

#define KEY(x) (*((unsigned int*)(key+x)))

/* some helper funcs */ 

/* Truth-table for (unsigned int) floor(sqrt(((double)(((unsigned char)(i))))+1) + 0.001). */
const int sqrt_table[256] = {
	1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9,
	9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 10, 10, 10,
	10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
	11, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
	12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 13,
	13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
	13, 13, 13, 13, 13, 13, 13, 13, 14, 14, 14, 14, 14, 14, 14, 14, 14,
	14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
	14, 14, 14, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	16
};

#ifndef __GNUC__
#define __attribute__(x)
#endif

/* my_cos() and my_sin() are equal to cos()<0 and sin()<0. */
int __attribute__((const)) my_cos(unsigned char i)
{
	return (unsigned)((i * 39) % 245) - 62 < 122;
}

int __attribute__((const)) my_sin(unsigned char i)
{
	return (i * 46) % 289 > 144;
}

/* this is (unsigned int) floor(sqrt(((double)(((unsigned char)(i))))+1) + 0.001). */
int __attribute__((const)) my_sqrt(unsigned char i)
{
	return sqrt_table[i];
}

static unsigned int ROR(unsigned int value, unsigned int count)
{
  count &= 0x1f;
  return (value >> count) | (value << (32 - count));
}

static unsigned int ROL(unsigned int value, unsigned int count)
{
  count &= 0x1f;
  return (value << count) | (value >> (32 - count));
}

/* the entry point of this mess */
/* this all works on unsigned ints so endianess is not an issue */

THROWS_RET enc_20_mix (unsigned char *key, unsigned int seed);

void enc_type_20 (unsigned int *key, unsigned int seed)
{
	enc_20_mix ((unsigned char*)key, seed);
}

/* major functions which make calls to other funcs */

static THROWS_RET enc_20_major_1 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_major_2 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_major_3 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_major_4 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_major_5 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_major_6 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_major_7 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_major_8 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_major_9 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_major_10 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_major_11 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_major_12 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_major_13 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_major_14 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_major_15 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_major_16 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_major_17 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_major_18 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_major_19 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_major_21 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_major_22 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_major_23 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_major_24 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_major_25 (unsigned char *key, unsigned int seed);

/* functions which throw exceptions */

static THROWS_RET enc_20_4AE420 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_4BAC10 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_4D56B0 (unsigned char *key);
static THROWS_RET enc_20_4D7900 (unsigned char *key);
static THROWS_RET enc_20_4DA120 (unsigned char *key);
static THROWS_RET enc_20_4DA200 (unsigned char *key);
static THROWS_RET enc_20_4DA4D0 (unsigned char *key);
static THROWS_RET enc_20_4EEC50 (unsigned char *key);
static THROWS_RET enc_20_49AA90 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_4D76F0 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_49B1F0 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_4A1640 (unsigned char *key, unsigned int seed);
static THROWS_RET enc_20_4E7C50 (unsigned char *key);

/* simple key manipulation functions */

static void enc_20_49B260 (unsigned char *key, unsigned int seed);
static void enc_20_4AB3B0 (unsigned char *key, unsigned int seed);
static void enc_20_4ABC40 (unsigned char *key, unsigned int seed);
static void enc_20_4D70D0 (unsigned char *key);
static void enc_20_4D7660 (unsigned char *key, unsigned int seed);
static void enc_20_4E7CC0 (unsigned char *key);
static void enc_20_4E7FB0 (unsigned char *key);
static void enc_20_4EB2A0 (unsigned char *key, unsigned int seed);
static void enc_20_4F2AF0 (unsigned char *key);
static void enc_20_4BABE0 (unsigned char *key, unsigned int seed);
static void enc_20_4C4A60 (unsigned char *key, unsigned int seed);
static void enc_20_4D5720 (unsigned char *key, unsigned int seed);

static void enc_20_4C07D0 (unsigned char *key);
static void enc_20_4D4100 (unsigned char *key);
static void enc_20_4D6FB0 (unsigned char *key);
static void enc_20_4D7000 (unsigned char *key);
static void enc_20_4D7080 (unsigned char *key);
static void enc_20_4E4F30 (unsigned char *key, unsigned int seed);
static void enc_20_4EB2C0 (unsigned char *key, unsigned int seed);
static void enc_20_4AE3C0 (unsigned char *key, unsigned int seed);
static void enc_20_4F2A40 (unsigned char *key);
static void enc_20_49CED0 (unsigned char *key);
static void enc_20_4D7790 (unsigned char *key);
static void enc_20_4EB310 (unsigned char *key);


/* minor implementation details below this line ;) */

void enc_20_49B260 (unsigned char *key, unsigned int seed)
{
	KEY(0x10) = ROR(KEY(0x10), (unsigned char)((seed << 5) - seed));
}

void enc_20_4AB3B0 (unsigned char *key, unsigned int seed)
{
	KEY(0x34) *= seed + 0x0601F603;
}

void enc_20_4ABC40 (unsigned char *key, unsigned int seed)
{
	KEY(0x40) += seed * 73;
}

void enc_20_4D70D0 (unsigned char *key)
{
	KEY(0x4C) ^= (((KEY(0x1C) * 8) - KEY(0x1C)) * 4 + KEY(0x1C)) << 1;
}

void enc_20_4D7660 (unsigned char *key, unsigned int seed)
{
	KEY(0x24) ^= seed ^ 0x334EC044;
}

void enc_20_4E7CC0 (unsigned char *key)
{
	KEY(0x18) = ROL(KEY(0x18), (unsigned char)ROR(KEY(0x20), 0x0E));
}

void enc_20_4E7FB0 (unsigned char *key)
{
	KEY(0x00) &= ROR(KEY(0x48), 0x1F);
}

void enc_20_4EB2A0 (unsigned char *key, unsigned int seed)
{
	KEY(0x24) &= seed - 0x2507B6E9;
}

void enc_20_4F2AF0 (unsigned char *key)
{
	KEY(0x0C) = ROR(KEY(0x0C), (unsigned char)(KEY(0x2C) ^ 0x0BBEA527));
}

void enc_20_4BABE0 (unsigned char *key, unsigned int seed)
{
	KEY(0x1C) &= (seed * 8 - seed) * 15;
}

void enc_20_4C4A60 (unsigned char *key, unsigned int seed)
{
	KEY(0x30) += seed - 0x075D8F4F;
}

void enc_20_4D5720 (unsigned char *key, unsigned int seed)
{
	KEY(0x18) ^= seed + 0x25283A4A;
}

THROWS_RET enc_20_4AE420 (unsigned char *key, unsigned int seed)
{
	KEY(0x20) += my_sin(seed) ? 0x04F0CF8D : seed;

	return KEY(0x20) & 1;
}

THROWS_RET enc_20_4BAC10 (unsigned char *key, unsigned int seed)
{
	KEY(0x08) += KEY(0x08) < 0x36DEF3E1 ? KEY(0x08) : seed;

	return KEY(0x08) & 1;
}

THROWS_RET enc_20_4D56B0 (unsigned char *key)
{
	KEY(0x28) ^= ROL(KEY(0x04), 0x34);

	return KEY(0x28) & 1;
}

THROWS_RET enc_20_4D7900 (unsigned char *key)
{
	KEY(0x40) -= KEY(0x18);

	return KEY(0x40) & 1;
}

THROWS_RET enc_20_4DA120 (unsigned char *key)
{
	KEY(0x28) += (((0-KEY(0x24)) << 4) - KEY(0x24)) * 5;

	return KEY(0x28) & 1;
}

THROWS_RET enc_20_4DA200 (unsigned char *key)
{
	KEY(0x00) = ROL(KEY(0x00), (unsigned char)(KEY(0x4C) ^ 0x0290626C));

	return KEY(0x00) & 1;
}

THROWS_RET enc_20_4DA4D0 (unsigned char *key)
{
	KEY(0x44) += KEY(0x20) * 0xF6084C92;

	return KEY(0x44) & 1;
}

THROWS_RET enc_20_4EEC50 (unsigned char *key)
{
	KEY(0x30) ^= KEY(0x28) & 0x28ACEC82;

	return KEY(0x30) & 1;
}

THROWS_RET enc_20_49AA90 (unsigned char *key, unsigned int seed)
{
	KEY(0x30) *= KEY(0x30) < 0x012D7BED ? KEY(0x30) : seed;

	return KEY(0x30) & 1;
}

THROWS_RET enc_20_4D76F0 (unsigned char *key, unsigned int seed)
{
	KEY(0x48) += KEY(0x14) < 0x0FD0AA3F ? KEY(0x14) : seed;

	return KEY(0x48) & 1;
}

THROWS_RET enc_20_49B1F0 (unsigned char *key, unsigned int seed)
{
	KEY(0x04) = ROR(KEY(0x04), (unsigned char)(seed * 0x1592D04));

	return KEY(0x04) & 1;
}

THROWS_RET enc_20_4A1640 (unsigned char *key, unsigned int seed)
{
	KEY(0x2C) &= my_cos(KEY(0x48)) ? 0x146A49CC : seed;

	return KEY(0x2C) & 1;
}

THROWS_RET enc_20_4E7C50 (unsigned char *key)
{
	KEY(0x08) &= my_cos(KEY(0x08)) ? 0x07EBBFDE : KEY(0x2C);

	return KEY(0x08) & 1;
}


void enc_20_4C07D0 (unsigned char *key)
{
	KEY(0x00) |= my_cos(KEY(0x04)) ? 0x056E0E99 : KEY(0x20);
}

void enc_20_4D4100 (unsigned char *key)
{
	KEY(0x48) += my_cos(KEY(0x3C)) ? 0x10D11D00 : KEY(0x24);
}

void enc_20_4D6FB0 (unsigned char *key)
{
	KEY(0x28) -= my_cos(KEY(0x3C)) ? 0x268CCA84 : KEY(0x24);
}

void enc_20_4D7000 (unsigned char *key)
{
	KEY(0x0C) -= my_cos(KEY(0x18)) ? 0x2031618A : KEY(0x20);
}

void enc_20_4D7080 (unsigned char *key)
{
	KEY(0x04) = ROL(KEY(0x04), my_sin(KEY(0x14)) ? 0x4 : (unsigned char)KEY(0x18));
}

void enc_20_4E4F30 (unsigned char *key, unsigned int seed)
{
	KEY(0x44) = ROR(KEY(0x44), my_sin(KEY(0x18)) ? 0x1d : (unsigned char)seed);
}

void enc_20_4EB2C0 (unsigned char *key, unsigned int seed)
{
	KEY(0x3C) ^= my_sin(KEY(0x38)) ? 0x40A33FD4 : seed;
}

void enc_20_4AE3C0 (unsigned char *key, unsigned int seed)
{
	KEY(0x24) = ROR(KEY(0x24), (unsigned char)my_sqrt(seed));
}

void enc_20_4F2A40 (unsigned char *key)
{
	KEY(0x1C) ^= my_sqrt(KEY(0x2C));
}

void enc_20_49CED0 (unsigned char *key)
{
	KEY(0x14) += my_sqrt(KEY(0x1C));
}



void enc_20_4D7790 (unsigned char *key)
{
	KEY(0x0C) ^= KEY(0x2C) * 0xeef27425;
	KEY(0x0C) += my_sqrt(KEY(0x00));
	KEY(0x3C) *= KEY(0x04) ^ 0xD89B4A;
	KEY(0x40) = ROR(KEY(0x40), (unsigned char)(KEY(0x40) & 0x1BEEB131));
	KEY(0x48) *= KEY(0x4C) + 0xa0d8c0cf;
	KEY(0x1C) *= KEY(0x00) < 0x6765080e ? KEY(0x00) : KEY(0x48);

	if(KEY(0x14) < 0xe848f43c)
		KEY(0x24) = ROL(KEY(0x24), (unsigned char)(KEY(0x14)));
	KEY(0x08) ^= KEY(0x14) < 0xa0d8c0cf ? KEY(0x14) : KEY(0x24) - 0xe848f43c;
	KEY(0x30) = ROL(KEY(0x30), (unsigned char) ROL(KEY(0x24) - 0xe848f43c, 0x8b));
}

void enc_20_4EB310 (unsigned char *key)
{
	KEY(0x08) = ROL(KEY(0x08), (unsigned char)(KEY(0x1C) + 0x1259dbb));
	KEY(0x08) ^= KEY(0x24) * 0x7941955;
	KEY(0x08) -= 0x796fa0af;
	KEY(0x0C) *= my_sin(KEY(0x4C)) ? 0x5ea67f83 : KEY(0x14);
	KEY(0x10) -= KEY(0x10) ^ 0x692C9EF9;
	KEY(0x28) += KEY(0x04) ^ 0xc43baf0b;
	KEY(0x30) *= KEY(0x1C) - 0xc43baf0b;
	KEY(0x34) ^= 0xd;
	KEY(0x44) ^= KEY(0x44) - 0x1259dbb;
	KEY(0x44) = ROR(KEY(0x44), 0xa);
	KEY(0x48) += KEY(0x00) ^ 0x3CF1856;
}


THROWS_RET enc_20_major_1 (unsigned char *key, unsigned int seed)
{
	unsigned int type = (KEY(0x44) ^ KEY(0x10) ^ KEY(0x34)) % 0x0D;

	if(type == 0x09)
	{
		KEY(0x1C) |= 0x3e73450d;
		enc_20_4E4F30 (key, 0x499C9229);
		enc_20_4D7790 (key);
	}

	KEY(0x2C) &= KEY(0x4C) & 0x170B54ED;

	if(type == 0x0A)
	{
		KEY(0x30) ^= KEY(0x3C) - 0x0F5CFDE0;
		enc_20_4D4100 (key);
		TRY(enc_20_major_23 (key, KEY(0x20)));
	}

	if(KEY(0x38) < 0x164D8D96)
		KEY(0x04) = ROR(KEY(0x04), (unsigned char)KEY(0x38));
	else
		KEY(0x04) = ROR(KEY(0x04), (unsigned char)KEY(0x10));

	if(type == 0x0C)
	{
		TRY(enc_20_4AE420 (key, 0xC0948CF0));
		enc_20_4D6FB0 (key);
		TRY(enc_20_major_24 (key, KEY(0x48)));
	}

	if(type == 0x00)
	{
		KEY(0x30) ^= KEY(0x3C) - 0x0F5CFDE0;
		enc_20_4E4F30 (key, 0x047FEEB5);
		TRY(enc_20_major_19 (key, KEY(0x30)));
	}

	KEY(0x18) = ROL(KEY(0x18), (unsigned char)(KEY(0x34) ^ 0x0B31DDE2));

	if(type == 0x06)
	{
		TRY(enc_20_49B1F0 (key, 0x0DA426A1));
		KEY(0x24) ^= ((KEY(0x1C) << 4) + KEY(0x1C)) << 2;
		TRY(enc_20_major_25 (key, seed));
	}

	seed += my_sin(seed) ? 0x160DF35D : seed;

	if(type == 0x03)
	{
		KEY(0x08) ^= my_sin(KEY(0x34)) ? 0x0FD08092 : KEY(0x28);
		KEY(0x34) *= ROR(KEY(0x0C), 0xA5);
		TRY(enc_20_major_17 (key, KEY(0x3C)));
	}

	if(type == 0x00)
	{
		enc_20_4E7CC0 (key);
		KEY(0x34) *= ROR(KEY(0x0C), 0xA5);
		TRY(enc_20_major_4 (key, KEY(0x20)));
	}

	seed &= KEY(0x4C) | 0xe00682c6;

	if(type == 0x01)
	{
		KEY(0x1C) &= KEY(0x34) ^ 0x21AAF758;
		KEY(0x24) |= KEY(0x1C) ^ 0x2A19119F;
		TRY(enc_20_major_18 (key, KEY(0x30)));
	}

	KEY(0x40) += my_sin(seed) ? 0xe00682c6 : KEY(0x1C);

	if(type == 0x02)
	{
		KEY(0x34) += KEY(0x3C) < 0x137BFFEB ? KEY(0x3C) : KEY(0x2C);
		KEY(0x4C) ^= my_cos(KEY(0x24)) ? 0x057337B8 : KEY(0x38);
		TRY(enc_20_major_15 (key, seed));
	}

	if(type == 0x07)
	{
		KEY(0x10) -= KEY(0x44) ^ 0x2217CF47;
		KEY(0x34) *= ROR(KEY(0x0C), 0xA5);
		TRY(enc_20_major_3 (key, KEY(0x38)));
	}

	seed += KEY(0x3C) ^ 0x1777BC26;

	if(type == 0x04)
	{
		KEY(0x4C) ^= my_cos(KEY(0x24)) ? 0x057337B8 : KEY(0x38);
		enc_20_4F2A40 (key);
		TRY(enc_20_major_21 (key, KEY(0x48)));
	}

	KEY(0x14) *= my_sqrt(KEY(0x24));

	if(type == 0x0B)
	{
		enc_20_49B260 (key, 0xCFB45C3E);
		enc_20_4EB2C0 (key, 0x8517AE30);
		TRY(enc_20_major_16 (key, KEY(0x10)));
	}

	KEY(0x34) &= KEY(0x48) - 0x0EB6DEE4;

	if(type == 0x05)
	{
		enc_20_4D7080 (key);
		TRY(enc_20_4D56B0 (key));
		enc_20_4D7790 (key);
	}

	if(type == 0x08)
	{
		KEY(0x1C) |= 0x7de964ed;
		TRY(enc_20_4D7900 (key));
		TRY(enc_20_major_23 (key, KEY(0x0C)));
	}

	RETURN;
}


THROWS_RET enc_20_major_2 (unsigned char *key, unsigned int seed)
{
	unsigned int type = KEY(0x28) & 0x0F;

	if(type == 0x05)
	{
		enc_20_4D4100 (key);
		KEY(0x1C) &= KEY(0x34) ^ 0x21AAF758;
		TRY(enc_20_major_25 (key, KEY(0x00)));
	}

	KEY(0x00) += ((0 - (seed * 3)) * 9) << 1;

	if(type == 0x0D)
	{
		KEY(0x08) *= KEY(0x0C) + 0x0D6863A6;
		KEY(0x18) += 0xFE07AF0E - KEY(0x0C);
		TRY(enc_20_major_17 (key, seed));
	}

	if(type == 0x0C)
	{
		enc_20_4D5720 (key, 0x6E4C891E);
		enc_20_4BABE0 (key, 0x8CD76CBE);
		TRY(enc_20_major_4 (key, KEY(0x38)));
	}

	KEY(0x1C) -= KEY(0x20) | 0x1A1A9407;

	if(type == 0x6)
	{
		enc_20_4AE3C0 (key, 0x63EDE696);
		KEY(0x0C) -= KEY(0x00) ^ 0x185F3B0D;
		TRY(enc_20_major_18 (key, KEY(0x38)));
	}

	KEY(0x08) += KEY(0x00) + 0x19259d5;

	if(type == 0x8)
	{
		KEY(0x24) ^= ((KEY(0x1C) << 4) + KEY(0x1C)) << 2;
		KEY(0x08) ^= KEY(0x3C) << 5;
		TRY(enc_20_major_15 (key, seed));
	}

	if(type == 0x0B)
	{
		enc_20_4F2A40 (key);
		KEY(0x0C) -= KEY(0x00) ^ 0x185F3B0D;
		TRY(enc_20_major_3 (key, KEY(0x3C)));
	}

	KEY(0x40) &= seed -0x1BADCB5;

	if(type == 0x0F)
	{
		KEY(0x38) |= KEY(0x0C) ^ 0x04345732;
		enc_20_49B260 (key, 0x2A42F19F);
		TRY(enc_20_major_21 (key, KEY(0x0C)));
	}

	KEY(0x14) -= my_cos(KEY(0x10)) ? 0xffcdb92f : KEY(0x38);

	if(type == 0x1)
	{
		KEY(0x34) -= KEY(0x04);
		KEY(0x1C) |= 0x45e184c5;
		TRY(enc_20_major_16 (key, KEY(0x24)));
		TRY(enc_20_4AE420 (key, 0x149A97A0));
		TRY(enc_20_4D76F0 (key, 0x0D87D888E));
		TRY(enc_20_major_1 (key, KEY(0x24)));
	}

	KEY(0x14) *= KEY(0x20) + 0xffcdb92f;

	if(type == 0x4)
	{
		TRY(enc_20_4D76F0 (key, 0x130AA218));
		KEY(0x34) *= ROR(KEY(0x0C), 0xA5);
		TRY(enc_20_major_14 (key, KEY(0x18)));
	}

	if(KEY(0x3C) < 0xbdc3f45b)
		KEY(0x04) = ROL(KEY(0x04), (unsigned char)KEY(0x3C));
	else
		KEY(0x04) = ROL(KEY(0x04), (unsigned char)KEY(0x24));

	if(type == 0x0E)
	{
		enc_20_4D7080 (key);
		enc_20_4AB3B0 (key, 0x790B68AA);
		TRY(enc_20_major_6 (key, KEY(0x14)));
	}

	if(type == 0x0)
	{
		KEY(0x18) += KEY(0x4C) - 0x3F5675D6;
		TRY(enc_20_4DA120 (key));
		TRY(enc_20_major_9 (key, seed));
	}

	KEY(0x18) += KEY(0x0C) * 121;

	if(type == 0x9)
	{
		enc_20_4EB2A0 (key, 0x63BCA480);
		enc_20_4D7080 (key);
		TRY(enc_20_major_25 (key, KEY(0x18)));
	}

	KEY(0x40) ^= my_cos(KEY(0x1C)) ? 0x2D36F243 : KEY(0x34);

	if(type == 0x0)
	{
		KEY(0x00) += KEY(0x48) ^ 0x4AC16B8D;
		enc_20_4D6FB0 (key);
		TRY(enc_20_major_17 (key, KEY(0x8)));
	}

	if(type == 0x7)
	{
		KEY(0x28) += 0x08958821;
		TRY(enc_20_4AE420 (key, 0x115E64D4));
		TRY(enc_20_major_4 (key, KEY(0x4C)));
	}

	KEY(0x38) &= KEY(0x0C) ^ 0xb8eb772d;

	if(type == 0x0A)
	{
		KEY(0x34) -= KEY(0x04);
		KEY(0x08) ^= KEY(0x3C) << 5;
		TRY(enc_20_major_18 (key, KEY(0x20)));
	}

	KEY(0x04) = ROR(KEY(0x04), (unsigned char)(KEY(0x30) * 101));

	if(type == 0x3)
	{
		KEY(0x08) ^= my_sin(KEY(0x34)) ? 0x0FD08092 : KEY(0x28);
		KEY(0x08) ^= my_sin(KEY(0x34)) ? 0x0FD08092 : KEY(0x28);
		TRY(enc_20_major_15 (key, KEY(0x3C)));
	}

	if(type == 0x2)
	{
		enc_20_4BABE0 (key, 0x785A1B8A);
		KEY(0x34) *= 0x0A02FE00;
		TRY(enc_20_major_3 (key, KEY(0x38)));
	}

	KEY(0x30) ^= my_sin(KEY(0x00)) ? 0x96d5a5a4 : KEY(0x14);

	RETURN;
}


THROWS_RET enc_20_major_3 (unsigned char *key, unsigned int seed)
{
	unsigned int type = (KEY(0x14) ^ seed ^ KEY(0x30)) % 0x0A;
	unsigned int var_14 = 0x3074A456;
    
	var_14 += 0x51CE37 - 0x18D0A343;
	seed *= KEY(0x18) | 0x04723B25;

	if(type == 0x0)
	{
		enc_20_4F2AF0 (key);
		TRY(enc_20_4DA120 (key));
		enc_20_4EB310 (key);
	}

	KEY(0x08) += 0 - (((KEY(0x10) * 8 - KEY(0x10)) << 1) - KEY(0x10));

	if(type == 0x5)
	{
		KEY(0x1C) ^= 0x414517EA;
		enc_20_4D7000 (key);
		enc_20_4D7790 (key);
	}

	var_14 += 0x51CE37 - 0x0C736D91;
	seed = (KEY(0x30) * 5 + seed) + (KEY(0x30) * 20);

	if(type == 0x1)
	{
		KEY(0x34) += KEY(0x3C) < 0x137BFFEB ? KEY(0x3C) : KEY(0x2C);
		enc_20_4E7CC0 (key);
		TRY(enc_20_major_23 (key, seed));
	}

	seed += KEY(0x1C) + var_14;

	if(type == 0x2)
	{
		enc_20_4D7000 (key);
		KEY(0x40) += 0x1f5b0c59;
		TRY(enc_20_major_24 (key, seed));
	}

	var_14 += 0x51CE37 - 0x6C5479A8;
	KEY(0x3C) -= KEY(0x00) ^ 0x16BEE8C4;

	if(type == 0x4)
	{
		TRY(enc_20_4DA4D0 (key));
		enc_20_4D6FB0 (key);
		TRY(enc_20_major_19 (key, seed));
	}

	KEY(0x48) ^= KEY(0x2C) + var_14;

	if(type == 0x6)
	{
		KEY(0x18) += KEY(0x4C) - 0x3F5675D6;
		KEY(0x18) += KEY(0x4C) - 0x3F5675D6;
		TRY(enc_20_major_25 (key, seed));
	}

	var_14 += 0x51CE37 - 0x1BF2C739;
	KEY(0x38) = ROL(KEY(0x38), (unsigned char)(KEY(0x4C)-0x8C1CE40));

	if(type == 0x8)
	{
		enc_20_4D7080 (key);
		KEY(0x30) += KEY(0x18) + 0x21D7BF61;
		TRY(enc_20_major_17 (key, seed));
	}

	KEY(0x00) = ROR(KEY(0x00), (unsigned char)(KEY(0x34) * 83));

	if(type == 0x9)
	{
		TRY(enc_20_4BAC10 (key, 0x70DA1D6F));
		enc_20_4D7000 (key);
		TRY(enc_20_major_4 (key, seed));
	}

	var_14 += 0x51CE37 - 0x31D3FEFD;

	if(KEY(0x40) < 0x33671DE9)
		seed ^= KEY(0x40);
	else
		seed ^= KEY(0x44);

	if(type == 0x7)
	{
		enc_20_4F2AF0 (key);
		TRY(enc_20_4D56B0 (key));
		TRY(enc_20_major_18 (key, KEY(0x14)));
	}

	seed &= seed << 6;

	if(type == 0x3)
	{
		enc_20_4D70D0 (key);
		KEY(0x08) ^= KEY(0x3C) << 5;
		TRY(enc_20_major_15 (key, KEY(0x4C)));
	}

	RETURN;
}


THROWS_RET enc_20_major_4 (unsigned char *key, unsigned int seed)
{
	unsigned int type = KEY(0x18) % 7;
	unsigned int var_14 = 0x775fd18;

	seed ^= ROL(KEY(0x0C), 0x72);

	if(type == 0x6)
	{
		KEY(0x18) += KEY(0x4C) - 0x3F5675D6;
		TRY(enc_20_4DA120 (key));
		enc_20_4EB310 (key);
	}

	KEY(0x3C) += (seed * 25) << 1;

	var_14 += 0x51CE37 - 0x12202C25;
	KEY(0x14) += 0x0C93495E4 - KEY(0x38);

	if(type == 0x2)
	{
		TRY(enc_20_4D76F0 (key, 0x10DB4A9D));
		KEY(0x18) += 0xFE07AF0E - KEY(0x0C);
		enc_20_4D7790 (key);
	}

	KEY(0x30) *= my_cos(KEY(0x38)) ? var_14 : KEY(0x44);

	if(type == 0x0)
	{
		enc_20_4D70D0 (key);
		KEY(0x24) |= KEY(0x1C) ^ 0x2A19119F;
		TRY(enc_20_major_23 (key, KEY(0x20)));
	}

	var_14 += 0x51CE37 - 0x15CE1247;
	KEY(0x18) &= KEY(0x1C) | var_14;

	KEY(0x2C) ^= my_cos(KEY(0x00)) ? 0x3A2C762B : seed;

	if(type == 0x4)
	{
		TRY(enc_20_4D56B0 (key));
		TRY(enc_20_49B1F0 (key, 0x44BF5D5F));
		TRY(enc_20_major_24 (key, seed));
	}

	var_14 += 0x51CE37 - 0x128DFD61;
	KEY(0x0C) -= my_sqrt(KEY(0x24));

	if(type == 0x5)
	{
		enc_20_4D5720 (key, 0x0DF523ED5);
		KEY(0x00) += KEY(0x48) ^ 0x4AC16B8D;
		TRY(enc_20_major_19 (key, KEY(0x48)));
	}

	seed &= my_cos(KEY(0x1C)) ? var_14 : KEY(0x0C);

	var_14 += 0x51CE37 - 0x3DE94AC6;
	KEY(0x00) += 0 - ((((KEY(0x3C) << 4) + KEY(0x3C)) << 2) - KEY(0x3C));

	if(type == 0x1)
	{
		enc_20_4E7CC0 (key);
		enc_20_4D5720 (key, 0x1D251133);
		TRY(enc_20_major_25 (key, KEY(0x0C)));
	}

	KEY(0x04) -=  ROR(KEY(0x48), 0x33);
	var_14 += 0x51CE37 - 0x26BE8A9B;

	KEY(0x44) ^= my_sin(KEY(0x38)) ? var_14 : KEY(0x40);

	if(type == 0x0)
	{
		KEY(0x0C) -= KEY(0x00) ^ 0x185F3B0D;
		KEY(0x08) *= KEY(0x0C) + 0x0D6863A6;
		TRY(enc_20_major_17 (key, KEY(0x38)));
	}

	RETURN;
}


THROWS_RET enc_20_major_5 (unsigned char *key, unsigned int seed)
{
	unsigned int type = (KEY(0x34) ^ KEY(0x18) ^ KEY(0x40)) & 0x0F;
	unsigned int var_18 = 0x0FE1D15;

	if(type == 0x7)
	{
		enc_20_4E7FB0 (key);
		KEY(0x24) += ROL(KEY(0x10), 0xE9);
		TRY(enc_20_major_17 (key, KEY(0x3C)));
	}

	var_18 += 0x51CE37 - 0x20B0889D;
	KEY(0x08) ^= KEY(0x3C) - var_18;

	if(type == 0x0F)
	{
		enc_20_4E4F30 (key, 0x0C7337307);
		KEY(0x24) |= KEY(0x1C) ^ 0x2A19119F;
		TRY(enc_20_major_4 (key, KEY(0x28)));
	}

	if(type == 0x0E)
	{
		enc_20_4D7660 (key, 0x32D5440A);
		KEY(0x14) -= KEY(0x3C);
		TRY(enc_20_major_18 (key, seed));
	}

	KEY(0x14) += KEY(0x20) * 73;

	if(type == 0x4)
	{
		KEY(0x38) |= KEY(0x0C) ^ 0x04345732;
		TRY(enc_20_4EEC50 (key));
		TRY(enc_20_major_15 (key, KEY(0x4C)));
	}

	if(type == 0x1)
	{
		enc_20_4D4100 (key);
		enc_20_4D70D0 (key);
		TRY(enc_20_major_3 (key, KEY(0x10)));
	}

	var_18 += 0x51CE37 - 0x0B2F7E4D;

	if(KEY(0x40) < 0x4DFE57F8)
		seed += KEY(0x40);
	else
		seed += KEY(0x44);

	if(type == 0x2)
	{
		enc_20_4AB3B0 (key, 0x74E11AD9);
		KEY(0x30) ^= KEY(0x3C) - 0x0F5CFDE0;
		TRY(enc_20_major_21 (key, KEY(0x2C)));
		KEY(0x34) *= ROR(KEY(0x0C), 0xA5);
		enc_20_4E7FB0 (key);
		TRY(enc_20_major_16 (key, KEY(0x28)));
	}

	KEY(0x14) += KEY(0x18) + var_18;

	if(type == 0x0A)
	{
		KEY(0x4C) ^= my_cos(KEY(0x24)) ? 0x057337B8 : KEY(0x38);
		KEY(0x24) |= KEY(0x1C) ^ 0x2A19119F;
		TRY(enc_20_major_1 (key, KEY(0x28)));
	}

	if(type == 0x0C)
	{
		KEY(0x40) += 0x203fdf50;
		enc_20_4E7FB0 (key);
		TRY(enc_20_major_14 (key, KEY(0x20)));
	}

	var_18 += 0x51CE37 - 0x1B56493C;

	KEY(0x04) += my_sin(seed) ? var_18 : seed;

	if(type == 0x6)
	{
		KEY(0x10) ^= ROR(0x51CF3579, 0x85);
		KEY(0x34) -= KEY(0x04);
		TRY(enc_20_major_6 (key, KEY(0x38)));
	}

	if(type == 0x3)
	{
		KEY(0x34) += KEY(0x3C) < 0x137BFFEB ? KEY(0x3C) : KEY(0x2C);
		KEY(0x08) ^= KEY(0x3C) << 5;
		TRY(enc_20_major_9 (key, seed));
	}

	KEY(0x0C) |= my_cos(KEY(0x0C)) ? var_18 : KEY(0x44);

	if(type == 0x9)
	{
		enc_20_4D70D0 (key);
		KEY(0x14) -= KEY(0x3C);
		TRY(enc_20_major_2 (key, seed));
	}

	if(type == 0x0D)
	{
		enc_20_49B260 (key, 0x78EBCA1E);
		KEY(0x10) -= KEY(0x44) ^ 0x2217CF47;
		TRY(enc_20_major_17 (key, seed));
	}

	var_18 += 0x51CE37 - 0x8B3397E;
	seed ^= KEY(0x08) * var_18;

	if(type == 0x0B)
	{
		KEY(0x4C) += 0x12B9E29D - KEY(0x30);
		KEY(0x1C) &= KEY(0x34) ^ 0x21AAF758;
		TRY(enc_20_major_4 (key, KEY(0x44)));
	}

	if(type == 0x0)
	{
		KEY(0x30) ^= KEY(0x3C) - 0x0F5CFDE0;
		enc_20_4E4F30 (key, 0x4FF6E927);
		TRY(enc_20_major_18 (key, seed));
	}

	KEY(0x20) += 0xF1030E9C - KEY(0x30);

	if(type == 0x5)
	{
		enc_20_4D5720 (key, 0x0C571A70B);
		KEY(0x4C) += 0x12B9E29D - KEY(0x30);
		TRY(enc_20_major_15 (key, seed));
	}

	if(type == 0x8)
	{
		enc_20_4BABE0 (key, 0x7D6B64A8);
		KEY(0x08) *= KEY(0x0C) + 0x0D6863A6;
		TRY(enc_20_major_3 (key, KEY(0x44)));
	}

	var_18 += 0x51CE37 - 0x9BA58BD;
	KEY(0x3C) += var_18 - KEY(0x04);

	if(type == 0x0)
	{
		KEY(0x08) *= KEY(0x0C) + 0x0D6863A6;
		KEY(0x0C) -= KEY(0x00) ^ 0x185F3B0D;
		TRY(enc_20_major_21 (key, seed));
	}

	if(type == 0x1)
	{
		TRY(enc_20_4D56B0 (key));
		KEY(0x30) *= KEY(0x30);
		TRY(enc_20_major_16 (key, KEY(0x30)));
	}

	KEY(0x18) *= KEY(0x14) * 29;

	RETURN;
}


THROWS_RET enc_20_major_6 (unsigned char *key, unsigned int seed)
{
	unsigned int type = KEY(0x44) % 0x0F;
	unsigned int var_18 = 0x10572198;

	if(type == 0x00)
	{
		KEY(0x34) += KEY(0x3C) < 0x137BFFEB ? KEY(0x3C) : KEY(0x2C);
		enc_20_4AB3B0 (key, 0x1CDB9F1C);
		TRY(enc_20_major_24 (key, KEY(0x20)));
	}

	var_18 += 0x51CE37 - 0x2EA3BDA3;

	KEY(0x2C) -= my_sin(KEY(0x24)) ? var_18 : KEY(0x1C);

	if(type == 0x0D)
	{
		TRY(enc_20_49B1F0 (key, 0x422E82A9));
		KEY(0x30) ^= KEY(0x3C) - 0x0F5CFDE0;
		TRY(enc_20_major_19 (key, KEY(0x00)));
	}

	KEY(0x28) -= KEY(0x18) ^ 0x1289DE2;

	if(type == 0x8)
	{
		enc_20_4AE3C0 (key, 0x8FED4B73);
		TRY(enc_20_4E7C50 (key));
		TRY(enc_20_major_25 (key, KEY(0x10)));
	}

	if(type == 0x5)
	{
		enc_20_4AB3B0 (key, 0x6492D146);
		KEY(0x48) -=  KEY(0x34) ^ 0x154ABCDF;
		TRY(enc_20_major_17 (key, seed));
	}

	var_18 += 0x51CE37 - 0x1A21C37A;
	KEY(0x40) = ROL(KEY(0x40), (unsigned char)my_sqrt(KEY(0x44)));

	if(type == 0x2)
	{
		KEY(0x40) += 0x3f147441;
		TRY(enc_20_major_4 (key, KEY(0x40)));
	}

	KEY(0x24) += my_sqrt(KEY(0x0C));

	if(type == 0x0E)
	{
		enc_20_4AE3C0 (key, 0xA3D7A7FC);
		KEY(0x34) -= KEY(0x04);
		TRY(enc_20_major_18 (key, seed));
	}

	var_18 += 0x51CE37 - 0x5B8499EE;
	seed = KEY(0x18) ^ seed ^ 0x202AB323;

	if(type == 0x9)
	{
		KEY(0x14) += KEY(0x00) ^ 0x3E17ADD3;
		KEY(0x10) -= KEY(0x44) ^ 0x2217CF47;
		TRY(enc_20_major_15 (key, KEY(0x20)));
	}

	if(type == 0x6)
	{
		KEY(0x08) *= KEY(0x0C) + 0x0D6863A6;
		KEY(0x18) += KEY(0x4C) - 0x3F5675D6;
		TRY(enc_20_major_3 (key, KEY(0x40)));
	}

	KEY(0x3C) ^= my_sqrt(KEY(0x28));

	if(type == 0x1)
	{
		TRY(enc_20_4BAC10 (key, 0x0B30D40D0));
		KEY(0x28) *= KEY(0x28) - 0x05EAE6BF;
		TRY(enc_20_major_21 (key, KEY(0x34)));
	}

	var_18 += 0x51CE37 - 0x1DDA929B;
	KEY(0x00) -= KEY(0x2C) ^ 0x1284AF29;

	if(type == 0x4)
	{
		KEY(0x14) += KEY(0x00) ^ 0x3E17ADD3;
		enc_20_4D7000 (key);
		TRY(enc_20_major_16 (key, KEY(0x44)));
	}

	seed = ROL(seed, (unsigned char)((KEY(0x2C) * 8 - KEY(0x2C)) << 4));

	if(type == 0x0B)
	{
		enc_20_4D7660 (key, 0x2EC1F3E2);
		TRY(enc_20_49AA90 (key, 0x13EE15C3));
		TRY(enc_20_major_1 (key, KEY(0x4C)));
	}

	if(type == 0x0)
	{
		TRY(enc_20_4D56B0 (key));
		KEY(0x4C) ^= my_cos(KEY(0x24)) ? 0x057337B8 : KEY(0x38);
		TRY(enc_20_major_14 (key, KEY(0x40)));
	}

	var_18 += 0x51CE37 - 0x5A8079C;
	KEY(0x24) |= KEY(0x24) ^ 0x2AD7629;

	if(type == 0x0A)
	{
		TRY(enc_20_49B1F0 (key, 0x4CDAADC3));
		TRY(enc_20_49AA90 (key, 0x0E8869877));
		TRY(enc_20_major_24 (key, seed));
	}

	KEY(0x10) *= KEY(0x30) * var_18;

	if(type == 0x0C)
	{
		KEY(0x24) += ROL(KEY(0x10), 0xE9);
		TRY(enc_20_4DA4D0 (key));
		TRY(enc_20_major_19 (key, KEY(0x14)));
	}

	if(type == 0x7)
	{
		KEY(0x38) |= KEY(0x0C) ^ 0x04345732;
		TRY(enc_20_49AA90 (key, 0x0DD1CA541));
		TRY(enc_20_major_25 (key, KEY(0x4)));
	}

	var_18 += 0x51CE37 - 0x2DE8F3D8;
	seed *= KEY(0x10) + 0x76E5A087;

	if(type == 0x3)
	{
		TRY(enc_20_4DA120 (key));
		TRY(enc_20_4AE420 (key, 0x62F4D3C4));
		TRY(enc_20_major_17 (key, seed));
	}

	RETURN;
}


THROWS_RET enc_20_major_7 (unsigned char *key, unsigned int seed)
{
	unsigned int type = (KEY(0x28) ^ KEY(0x2C) ^ KEY(0x48)) & 0x0F;
	unsigned int var_18 = 0xc6ef5e80;

	if(type == 0x3)
	{
		enc_20_4D4100 (key);
		TRY(enc_20_4BAC10 (key, 0x54BCDE17));
		TRY(enc_20_major_1 (key, KEY(0x38)));
	}

	if(type == 0x9)
	{
		KEY(0x1C) &= KEY(0x34) ^ 0x21AAF758;
		KEY(0x34) -= KEY(0x04);
		TRY(enc_20_major_14 (key, KEY(0x30)));
	}

	KEY(0x20) |= var_18 + KEY(0x04);

	if(type == 0x2)
	{
		KEY(0x18) += KEY(0x4C) - 0x3F5675D6;
		KEY(0x18) += 0xFE07AF0E - KEY(0x0C);
		TRY(enc_20_major_6 (key, KEY(0x40)));
	}

	if(type == 0x5)
	{
		KEY(0x14) -= KEY(0x3C);
		KEY(0x40) += 0x3fa3dc2f;
		TRY(enc_20_major_9 (key, KEY(0x14)));
	}

	if(type == 0x1)
	{
		KEY(0x34) += KEY(0x3C) < 0x137BFFEB ? KEY(0x3C) : KEY(0x2C);
		KEY(0x38) |= KEY(0x0C) ^ 0x04345732;
		TRY(enc_20_major_2 (key, KEY(0x0C)));
	}

	KEY(0x3C) -= var_18 + KEY(0x4C);

	if(type == 0x4)
	{
		KEY(0x08) ^= KEY(0x3C) << 5;
		enc_20_4D6FB0 (key);
		TRY(enc_20_major_5 (key, KEY(0x1C)));
	}

	if(type == 0x1)
	{
		KEY(0x00) += (KEY(0x18) * 15) << 2;
		enc_20_4E4F30 (key, 0x0CA4795D5);
		TRY(enc_20_major_12 (key, KEY(0x40)));
	}

	if(type == 0x9)
	{
		KEY(0x14) += KEY(0x00) ^ 0x3E17ADD3;
		enc_20_4EB2A0 (key, 0x70E051EB);
		TRY(enc_20_major_11 (key, KEY(0x4C)));
	}

	var_18 += 0x51CE37 - 0x28A8FE0;
	seed -= KEY(0x00) ^ 0x3B61016B;

	if(type == 0x4)
	{
		KEY(0x14) -= KEY(0x3C);
		TRY(enc_20_49AA90 (key, 0x1984A749));
		TRY(enc_20_major_13 (key, KEY(0x0C)));
	}

	if(type == 0x3)
	{
		KEY(0x24) += ROL(KEY(0x10), 0xE9);
		enc_20_4D6FB0 (key);
		TRY(enc_20_major_22 (key, KEY(0x10)));
	}

	if(type == 0x7)
	{
		TRY(enc_20_4D56B0 (key));
		enc_20_4E4F30 (key, 0x1C91FACD);
		TRY(enc_20_major_8 (key, KEY(0x14)));
	}

	KEY(0x2C) = ROL(KEY(0x2C), (unsigned char)(KEY(0x28) ^ 0x469FB6FA));

	if(type == 0x8)
	{
		KEY(0x48) -=  KEY(0x34) ^ 0x154ABCDF;
		KEY(0x4C) += 0x12B9E29D - KEY(0x30);
		TRY(enc_20_major_10 (key, seed));
	}

	if(type == 0x0E)
	{
		KEY(0x4C) += 0x12B9E29D - KEY(0x30);
		enc_20_4AE3C0 (key, 0x0BFD9F90B);
		TRY(enc_20_major_1 (key, seed));
	}

	var_18 += 0x51CE37 - 0x4DA526F;
	seed -= KEY(0x38) * var_18;

	if(type == 0x0)
	{
		enc_20_4D5720 (key, 0x6FC267C3);
		enc_20_4D4100 (key);
		TRY(enc_20_major_14 (key, KEY(0x18)));
	}

	if(type == 0x0D)
	{
		enc_20_4C4A60 (key, 0x32230D49);
		KEY(0x08) ^= KEY(0x3C) << 5;
		TRY(enc_20_major_6 (key, KEY(0x10)));
	}

	if(type == 0x8)
	{
		KEY(0x48) *= KEY(0x28) + 0x466E09CF;
		KEY(0x34) -= KEY(0x04);
		TRY(enc_20_major_9 (key, seed));
	}

	KEY(0x10) += 0xA207344D - seed;

	if(type == 0x0C)
	{
		TRY(enc_20_4BAC10 (key, 0x80A1DA17));
		KEY(0x34) *= 0x0A02FE00;
		TRY(enc_20_major_2 (key, KEY(0x0C)));
	}

	if(type == 0x2)
	{
		enc_20_4AB3B0 (key, 0x64A3D689);
		enc_20_4D5720 (key, 0x0E5C778D8);
		TRY(enc_20_major_5 (key, KEY(0x24)));
	}

	if(type == 0x0)
	{
		enc_20_4E4F30 (key, 0x4A5DE3AB);
		KEY(0x14) += KEY(0x00) ^ 0x3E17ADD3;
		TRY(enc_20_major_12 (key, KEY(0x14)));
	}

	var_18 += 0x51CE37 - 0x352F1985;
	seed ^= KEY(0x48) ^ 0xE6830C9;

	if(type == 0x6)
	{
		KEY(0x24) |= KEY(0x1C) ^ 0x2A19119F;
		KEY(0x1C) &= KEY(0x34) ^ 0x21AAF758;
		TRY(enc_20_major_11 (key, seed));
	}

	if(type == 0x7)
	{
		KEY(0x0C) -= KEY(0x00) ^ 0x185F3B0D;
		TRY(enc_20_4BAC10 (key, 0x0B11DA063));
		TRY(enc_20_major_13 (key, KEY(0x0C)));
	}

	if(type == 0x0F)
	{
		enc_20_4F2A40 (key);
		KEY(0x10) ^= ROR(0x3CC69EC8, 0x85);
		TRY(enc_20_major_22 (key, KEY(0x44)));
	}

	KEY(0x00) ^= my_sin(seed) ? var_18 : KEY(0x20);

	if(type == 0x0B)
	{
		enc_20_4D6FB0 (key);
		KEY(0x08) ^= KEY(0x3C) << 5;
		TRY(enc_20_major_8 (key, seed));
	}

	if(type == 0x5)
	{
		KEY(0x24) += ROL(KEY(0x10), 0xE9);
		KEY(0x1C) &= KEY(0x34) ^ 0x21AAF758;
		TRY(enc_20_major_10 (key, KEY(0x40)));
	}

	var_18 += 0x51CE37 - 0x50402411;
	seed += ((KEY(0x04) << 5) - KEY(0x04)) * 2;

	if(type == 0x6)
	{
		enc_20_4F2A40 (key);
		enc_20_49B260 (key, 0x39DB8DF7);
		TRY(enc_20_major_1 (key, KEY(0x30)));
	}

	if(type == 0x0A)
	{
		enc_20_4F2AF0 (key);
		KEY(0x30) ^= KEY(0x3C) - 0x0F5CFDE0;
		TRY(enc_20_major_14 (key, seed));
	}

	KEY(0x04) ^= KEY(0x08) & var_18;

	RETURN;
}


THROWS_RET enc_20_major_8 (unsigned char *key, unsigned int seed)
{
	unsigned int type = (KEY(0x08) ^ seed ^ KEY(0x44)) & 0x0F;
	unsigned int var_18 = 0x16332817;

	if(type == 0x7)
	{
		KEY(0x34) -= KEY(0x04);
		enc_20_4C07D0 (key);
		TRY(enc_20_major_21 (key, seed));
	}

	if(type == 0x0)
	{
		enc_20_49CED0 (key);
		TRY(enc_20_4BAC10 (key, 0x0E0B52E33));
		TRY(enc_20_major_16 (key, KEY(0x3C)));
	}

	var_18 += 0x51CE37 - 0x330C4928;
	seed -= ROR(KEY(0x08), (unsigned char)var_18);

	if(type == 0x5)
	{
		KEY(0x10) ^= ROR(0x355281E1, 0x85);
		KEY(0x14) += KEY(0x00) ^ 0x3E17ADD3;
		TRY(enc_20_major_1 (key, KEY(0x30)));
	}

	if(type == 0x1)
	{
		enc_20_4F2AF0 (key);
		enc_20_4D70D0 (key);
		TRY(enc_20_major_14 (key, KEY(0x4)));
	}

	KEY(0x30) += (0 - (((KEY(0x44) * 15) << 1) - KEY(0x44))) << 2;

	if(type == 0x1)
	{
		KEY(0x30) += KEY(0x18) + 0x21D7BF61;
		KEY(0x34) *= ROR(KEY(0x0C), 0xA5);
		TRY(enc_20_major_6 (key, KEY(0x14)));
	}

	if(type == 0x0E)
	{
		KEY(0x10) ^= ROR(0x35880FD2, 0x85);
		TRY(enc_20_4DA4D0 (key));
		TRY(enc_20_major_9 (key, seed));
	}

	var_18 += 0x51CE37 - 0x1A275E87;
	KEY(0x0C) ^= KEY(0x1C) + 0x137C9F7D;

	if(type == 0x0)
	{
		KEY(0x1C) &= KEY(0x34) ^ 0x21AAF758;
		KEY(0x08) ^= KEY(0x3C) << 5;
		TRY(enc_20_major_2 (key, KEY(0x14)));
	}

	if(type == 0x4)
	{
		KEY(0x28) += 0x08958821;
		KEY(0x40) += 0x3d2948e4;
		TRY(enc_20_major_5 (key, KEY(0x10)));
	}

	if(type == 0x3)
	{
		KEY(0x0C) -= KEY(0x00) ^ 0x185F3B0D;
		KEY(0x24) |= KEY(0x1C) ^ 0x2A19119F;
		TRY(enc_20_major_12 (key, KEY(0x30)));
	}

	if(KEY(0x44) < var_18)
		KEY(0x2C) += KEY(0x44);
	else
		KEY(0x2C) += KEY(0x10);
		
	if(type == 0x4)
	{
		enc_20_4D7660 (key, 0x2F26A2DC);
		KEY(0x30) ^= KEY(0x3C) - 0x0F5CFDE0;
		TRY(enc_20_major_11 (key, KEY(0x24)));
	}

	if(type == 0x0D)
	{
		KEY(0x1C) |= 0x378d3869;
		TRY(enc_20_4E7C50 (key));
		TRY(enc_20_major_13 (key, KEY(0x34)));
	}

	var_18 += 0x51CE37 - 0xC5AF489;
	seed *= KEY(0x30) + var_18;

	if(type == 0x7)
	{
		KEY(0x18) += KEY(0x4C) - 0x3F5675D6;
		KEY(0x4C) ^= my_cos(KEY(0x24)) ? 0x057337B8 : KEY(0x38);
		TRY(enc_20_major_22 (key, seed));
	}

	if(type == 0x8)
	{
		enc_20_4AB3B0 (key, 0x17CCEE88);
		enc_20_4D7080 (key);
		TRY(enc_20_major_21 (key, seed));
	}

	KEY(0x28) += 0xAA3373FC - KEY(0x18); 

	if(type == 0x5)
	{
		enc_20_4E4F30 (key, 0x0AE768CE);
		TRY(enc_20_4AE420 (key, 0x0BC90D50));
		TRY(enc_20_major_16 (key, seed));
	}

	if(type == 0x0F)
	{
		KEY(0x24) |= KEY(0x1C) ^ 0x2A19119F;
		KEY(0x00) += KEY(0x48) ^ 0x4AC16B8D;
		TRY(enc_20_major_1 (key, seed));
	}

	if(type == 0x6)
	{
		enc_20_4F2A40 (key);
		KEY(0x08) ^= my_sin(KEY(0x34)) ? 0x0FD08092 : KEY(0x28);
		TRY(enc_20_major_14 (key, seed));
	}

	var_18 += 0x51CE37 - 0x190E1B61;

	if(KEY(0x2C) < var_18)
		seed ^= KEY(0x2C);
	else
		seed ^= KEY(0x08);

	if(type == 0x6)
	{
		KEY(0x40) += 0x1a36972b;
		enc_20_4D70D0 (key);
		TRY(enc_20_major_6 (key, KEY(0x14)));
	}

	if(type == 0x0C)
	{
		TRY(enc_20_4A1640 (key, 0x0B6571D3F));
		KEY(0x00) += (KEY(0x18) * 15) << 2;
		TRY(enc_20_major_9 (key, KEY(0x24)));
	}

	seed *= KEY(0x38) + 0x9BAA8DB;

	if(type == 0x9)
	{
		TRY(enc_20_49AA90 (key, 0x0E378A0ED));
		KEY(0x14) += KEY(0x00) ^ 0x3E17ADD3;
		TRY(enc_20_major_2 (key, KEY(0x20)));
	}

	if(type == 0x0B)
	{
		TRY(enc_20_4D76F0 (key, 0x0BD149BD9));
		enc_20_4EB2C0 (key, 0x6476F303);
		TRY(enc_20_major_5 (key, seed));
	}

	var_18 += 0x51CE37 - 0x11E8B829;
	KEY(0x44) += my_sqrt(KEY(0x30));

	if(type == 0x0A)
	{
		KEY(0x34) *= 0x0A02FE00;
		KEY(0x30) += KEY(0x18) + 0x21D7BF61;
		TRY(enc_20_major_12 (key, KEY(0x4C)));
	}

	if(type == 0x3)
	{
		enc_20_4AB3B0 (key, 0x0B198ECA);
		enc_20_4D7080 (key);
		TRY(enc_20_major_11 (key, KEY(0x8)));
	}

	KEY(0x1C) ^= KEY(0x24) * 0x27219096;

	if(type == 0x2)
	{
		KEY(0x18) += KEY(0x4C) - 0x3F5675D6;
		enc_20_4ABC40 (key, 0x88BE6100);
		TRY(enc_20_major_13 (key, KEY(0x3C)));
		TRY(enc_20_4DA200 (key));
		KEY(0x4C) ^= my_cos(KEY(0x24)) ? 0x057337B8 : KEY(0x38);
		TRY(enc_20_major_22 (key, KEY(0x4C)));
	}

	var_18 += 0x51CE37 - 0x2790F4EC;
	KEY(0x08) = ROL(KEY(0x08), (unsigned char)(var_18 ^ seed));

	RETURN;
}


THROWS_RET enc_20_major_9 (unsigned char *key, unsigned int seed)
{
	unsigned int type = KEY(0x20) & 0x0F;
	unsigned int var_18 = 0x3A6D8FF;

	if(type == 0x0A)
	{
		enc_20_4D7000 (key);
		KEY(0x1C) &= KEY(0x34) ^ 0x21AAF758;
		TRY(enc_20_major_19 (key, KEY(0x00)));
	}

	var_18 += 0x51CE37 - 0x20E20A59;
	seed |= seed + 0x20029BC7;

	if(type == 0x3)
	{
		KEY(0x40) += 0x45e88961;
		TRY(enc_20_4EEC50 (key));
		TRY(enc_20_major_25 (key, KEY(0x3C)));
	}

	if(type == 0x8)
	{
		TRY(enc_20_4DA200 (key));
		enc_20_4F2AF0 (key);
		TRY(enc_20_major_17 (key, KEY(0x8)));
	}

	KEY(0x20) |= (((KEY(0x24) * 27) << 1) - KEY(0x24)) << 1;

	if(type == 0x0)
	{
		enc_20_4BABE0 (key, 0x7F3CF484);
		KEY(0x24) ^= ((KEY(0x1C) << 4) + KEY(0x1C)) << 2;
		TRY(enc_20_major_4 (key, KEY(0x2C)));
	}

	if(type == 0x0E)
	{
		KEY(0x34) += KEY(0x3C) < 0x137BFFEB ? KEY(0x3C) : KEY(0x2C);
		TRY(enc_20_4DA4D0 (key));
		TRY(enc_20_major_18 (key, KEY(0x34)));
	}

	var_18 += 0x51CE37 - 0x10983F4F;
	KEY(0x28) &= KEY(0x18) - 0x1286A10;

	if(type == 0x0C)
	{
		KEY(0x24) += ROL(KEY(0x10), 0xE9);
		TRY(enc_20_4D7900 (key));
		TRY(enc_20_major_15 (key, KEY(0x44)));
	}

	if(type == 0x2)
	{
		TRY(enc_20_4EEC50 (key));
		enc_20_4E7FB0 (key);
		TRY(enc_20_major_3 (key, KEY(0x34)));
	}

	KEY(0x38) = ROR(KEY(0x38), (unsigned char)(ROL(seed, 0x48)));

	if(type == 0x9)
	{
		KEY(0x1C) &= KEY(0x34) ^ 0x21AAF758;
		enc_20_4C07D0 (key);
		TRY(enc_20_major_21 (key, KEY(0x14)));
	}

	var_18 += 0x51CE37 - 0x5CA8FE0;
	seed += 0x176CF052 - KEY(0x30);

	if(type == 0x0F)
	{
		KEY(0x30) ^= KEY(0x3C) - 0x0F5CFDE0;
		KEY(0x34) *= 0x0A02FE00;
		TRY(enc_20_major_16 (key, seed));
	}

	if(type == 0x1)
	{
		KEY(0x34) *= ROR(KEY(0x0C), 0xA5);
		KEY(0x4C) ^= my_cos(KEY(0x24)) ? 0x057337B8 : KEY(0x38);
		TRY(enc_20_major_1 (key, KEY(0x44)));
	}

	KEY(0x20) = ROL(KEY(0x20), (unsigned char)(KEY(0x10) | 0x702AAAF));

	if(type == 0x5)
	{
		KEY(0x24) ^= ((KEY(0x1C) << 4) + KEY(0x1C)) << 2;
		enc_20_4BABE0 (key, 0x77C07A82);
		TRY(enc_20_major_14 (key, KEY(0x18)));
	}

	if(type == 0x4)
	{
		KEY(0x14) -= KEY(0x3C);
		enc_20_4D5720 (key, 0x5FEA0895);
		TRY(enc_20_major_6 (key, seed));
	}

	var_18 += 0x51CE37 - 0xECDFFDA;
	KEY(0x34) *= KEY(0x08) * 101;

	if(type == 0x0)
	{
		KEY(0x38) |= KEY(0x0C) ^ 0x04345732;
		KEY(0x34) += KEY(0x3C) < 0x137BFFEB ? KEY(0x3C) : KEY(0x2C);
		TRY(enc_20_major_19 (key, KEY(0x28)));
	}

	if(type == 0x6)
	{
		enc_20_4D7000 (key);
		enc_20_4F2AF0 (key);
		TRY(enc_20_major_25 (key, seed));
	}

	KEY(0x2C) |= ROR(KEY(0x44), 0xFD);

	if(type == 0x0D)
	{
		KEY(0x28) *= KEY(0x28) - 0x05EAE6BF;
		enc_20_4ABC40 (key, 0x981510F3);
		TRY(enc_20_major_17 (key, KEY(0x48)));
	}

	var_18 += 0x51CE37 - 0x146D65D3;
	KEY(0x44) &= (seed * 3) << 4;

	if(type == 0x7)
	{
		enc_20_4D4100 (key);
		TRY(enc_20_4A1640 (key, 0x65EC261));
		TRY(enc_20_major_4 (key, KEY(0x00)));
	}

	if(type == 0x0B)
	{
		KEY(0x38) |= KEY(0x0C) ^ 0x04345732;
		KEY(0x00) += KEY(0x48) ^ 0x4AC16B8D;
		TRY(enc_20_major_18 (key, KEY(0x40)));
	}

	KEY(0x34) |= ((KEY(0x0C) << 5) - KEY(0x0C)) << 1;

	RETURN;
}


THROWS_RET enc_20_major_10 (unsigned char *key, unsigned int seed)
{
	unsigned int type = (KEY(0x10) ^ KEY(0x30) ^ KEY(0x44)) & 0x0F;
	unsigned int var_18 = 0x7A66DF8;

	if(type == 0x9)
	{
		TRY(enc_20_4A1640 (key, 0x9FEBCD24));
		enc_20_4BABE0 (key, 0x7CB379C8);
		TRY(enc_20_major_16 (key, KEY(0x38)));
	}

	if(type == 0x4)
	{
		enc_20_4D5720 (key, 0x82BEBF6F);
		KEY(0x28) *= KEY(0x28) - 0x05EAE6BF;
		TRY(enc_20_major_1 (key, KEY(0x8)));
	}

	var_18 += 0x51CE37 - 0x4204423;
	KEY(0x24) += KEY(0x2C) < var_18 ? KEY(0x2C) : KEY(0x24);

	if(type == 0x6)
	{
		TRY(enc_20_4D76F0 (key, 0x0ECE6BFA0));
		KEY(0x38) |= KEY(0x0C) ^ 0x04345732;
		TRY(enc_20_major_14 (key, seed));
	}

	if(type == 0x8)
	{
		TRY(enc_20_4DA4D0 (key));
		KEY(0x48) *= KEY(0x28) + 0x466E09CF;
		TRY(enc_20_major_6 (key, KEY(0x30)));
	}

	if(type == 0x2)
	{
		KEY(0x30) += KEY(0x18) + 0x21D7BF61;
		KEY(0x38) |= KEY(0x0C) ^ 0x04345732;
		TRY(enc_20_major_9 (key, KEY(0x4)));
	}

	KEY(0x28) *= my_cos(seed) ? 0x16B578EE : KEY(0x08);

	if(type == 0x6)
	{
		enc_20_4E7CC0 (key);
		enc_20_4D7000 (key);
		TRY(enc_20_major_2 (key, KEY(0x28)));
	}

	if(type == 0x0D)
	{
		KEY(0x24) ^= ((KEY(0x1C) << 4) + KEY(0x1C)) << 2;
		enc_20_49CED0 (key);
		TRY(enc_20_major_5 (key, KEY(0x28)));
	}

	var_18 += 0x51CE37 - 0x1472F876;
	KEY(0x44) += seed * 77;

	if(type == 0x1)
	{
		enc_20_4C07D0 (key);
		KEY(0x4C) ^= my_cos(KEY(0x24)) ? 0x057337B8 : KEY(0x38);
		TRY(enc_20_major_12 (key, seed));
	}

	if(type == 0x0E)
	{
		TRY(enc_20_4EEC50 (key));
		enc_20_4E4F30 (key, 0x9A35E7);
		TRY(enc_20_major_11 (key, KEY(0x4C)));
	}

	if(type == 0x1)
	{
		KEY(0x4C) ^= KEY(0x3C) ^ 0x03574ED3;
		KEY(0x28) += 0x9F2550BD;
		TRY(enc_20_major_13 (key, seed));
	}

	seed = ROL(seed, (unsigned char)(KEY(0x1C) * 0xD46040D));

	if(type == 0x0C)
	{
		enc_20_4EB2A0 (key, 0x542B84AF);
		KEY(0x28) *= KEY(0x28) - 0x05EAE6BF;
		TRY(enc_20_major_22 (key, KEY(0x8)));
	}

	if(type == 0x3)
	{
		enc_20_4D6FB0 (key);
		enc_20_4C07D0 (key);
		TRY(enc_20_major_8 (key, KEY(0x10)));
		KEY(0x30) *= KEY(0x30);
		TRY(enc_20_4DA200 (key));
		TRY(enc_20_major_16 (key, KEY(0x48)));
	}

	var_18 += 0x51CE37 - 0xC2920FC;

	KEY(0x10) += my_sin(KEY(0x00)) ? 0x1873296 : KEY(0x04);

	if(type == 0x0A)
	{
		enc_20_4C07D0 (key);
		TRY(enc_20_4E7C50 (key));
		TRY(enc_20_major_1 (key, KEY(0x20)));
	}

	if(type == 0x4)
	{
		KEY(0x30) *= 0x0F44CB55;
		KEY(0x40) += 0x75a864cf;
		TRY(enc_20_major_14 (key, KEY(0x10)));
	}

	KEY(0x30) += 0x1C0BD6DB - KEY(0x2C);

	if(type == 0x7)
	{
		TRY(enc_20_4DA4D0 (key));
		enc_20_4C07D0 (key);
		TRY(enc_20_major_6 (key, KEY(0x38)));
	}

	if(type == 0x2)
	{
		enc_20_4AB3B0 (key, 0x11B24BD8);
		TRY(enc_20_4A1640 (key, 0x8951503F));
		TRY(enc_20_major_9 (key, KEY(0x4C)));
	}

	if(type == 0x0B)
	{
		KEY(0x00) += (KEY(0x18) * 15) << 2;
		KEY(0x10) -= KEY(0x44) ^ 0x2217CF47;
		TRY(enc_20_major_2 (key, KEY(0x20)));
	}

	var_18 += 0x51CE37 - 0x64E3C4EE;
	KEY(0x48) += (0 - (((KEY(0x18) * 3) << 2) - KEY(0x18))) << 2;

	if(type == 0x7)
	{
		KEY(0x48) *= KEY(0x28) + 0x466E09CF;
		enc_20_4D7000 (key);
		TRY(enc_20_major_5 (key, KEY(0x3C)));
	}

	if(type == 0x0)
	{
		enc_20_4AB3B0 (key, 0x1253B4B9);
		TRY(enc_20_4D56B0 (key));
		TRY(enc_20_major_12 (key, seed));
	}

	if(type == 0x5)
	{
		KEY(0x48) *= KEY(0x28) + 0x466E09CF;
		KEY(0x24) ^= ((KEY(0x1C) << 4) + KEY(0x1C)) << 2;
		TRY(enc_20_major_11 (key, KEY(0x28)));
	}

	KEY(0x18) ^= KEY(0x40) ^ 0x354E354D;

	if(type == 0x0F)
	{
		KEY(0x24) |= KEY(0x1C) ^ 0x2A19119F;
		enc_20_4D6FB0 (key);
		TRY(enc_20_major_13 (key, seed));
	}

	if(type == 0x8)
	{
		KEY(0x38) |= KEY(0x0C) ^ 0x04345732;
		enc_20_4EB2C0 (key, 0x8A0E1AD7);
		TRY(enc_20_major_22 (key, KEY(0x20)));
	}

	var_18 += 0x51CE37 - 0x43F58A7F;
	seed += var_18 ^ KEY(0x44);

	if(type == 0x0)
	{
		TRY(enc_20_4EEC50 (key));
		enc_20_4F2AF0 (key);
		TRY(enc_20_major_8 (key, KEY(0x20)));
	}

	if(type == 0x5)
	{
		KEY(0x18) += 0xFE07AF0E - KEY(0x0C);
		KEY(0x34) *= 0x0A02FE00;
		TRY(enc_20_major_16 (key, KEY(0x28)));
	}

	KEY(0x0C) += KEY(0x34) + var_18;

	RETURN;	  
}


THROWS_RET enc_20_major_11 (unsigned char *key, unsigned int seed)
{
	unsigned int type = (KEY(0x18) ^ seed ^ KEY(0x38)) & 0x0F;
	unsigned int var_18 = 0x1E171745;

	if(type == 0x2)
	{
		KEY(0x4C) ^= KEY(0x3C) ^ 0x03574ED3;
		KEY(0x34) -= KEY(0x04);
		TRY(enc_20_major_18 (key, KEY(0x00)));
	}

	if(type == 0x0)
	{
		KEY(0x1C) ^= 0x414517EA;
		TRY(enc_20_4D7900 (key));
		TRY(enc_20_major_15 (key, KEY(0x34)));
	}

	var_18 += 0x51CE37 - 0x1A8B0B2;
	KEY(0x38) &= (seed << 6) - seed;

	if(type == 0x0A)
	{
		enc_20_4E7CC0 (key);
		enc_20_4EB2A0 (key, 0x5D0DEC41);
		TRY(enc_20_major_3 (key, KEY(0x28)));
	}

	if(type == 0x0F)
	{
		enc_20_4F2A40 (key);
		enc_20_4C4A60 (key, 0x6372112C);
		TRY(enc_20_major_21 (key, KEY(0x2C)));
	}

	KEY(0x28) = ROR(KEY(0x28), (unsigned char)(KEY(0x38) * 83));

	if(type == 0x8)
	{
		KEY(0x4C) ^= my_cos(KEY(0x24)) ? 0x057337B8 : KEY(0x38);
		KEY(0x30) ^= KEY(0x3C) - 0x0F5CFDE0;
		TRY(enc_20_major_16 (key, seed));
	}

	if(type == 0x4)
	{
		KEY(0x0C) -= KEY(0x00) ^ 0x185F3B0D;
		enc_20_4D5720 (key, 0x82E916DD);
		TRY(enc_20_major_1 (key, KEY(0x14)));
	}

	var_18 += 0x51CE37 - 0x141DA8E2;
	KEY(0x2C) ^= seed - 0x3C17609C;

	if(type == 0x1)
	{
		KEY(0x30) += KEY(0x18) ^ 0x211F5E40;
		enc_20_4D7000 (key);
		TRY(enc_20_major_14 (key, seed));
	}

	if(type == 0x7)
	{
		enc_20_4AE3C0 (key, 0x0B7D63681);
		TRY(enc_20_4D56B0 (key));
		TRY(enc_20_major_6 (key, KEY(0x2C)));
	}

	KEY(0x38) += my_sin(KEY(0x24)) ? 0x2D3F1771 : KEY(0x2C);

	if(type == 0x5)
	{
		TRY(enc_20_4EEC50 (key));
		KEY(0x00) += KEY(0x48) ^ 0x4AC16B8D;
		TRY(enc_20_major_9 (key, KEY(0x24)));
	}

	if(type == 0x2)
	{
		KEY(0x30) *= 0x0F44CB55;
		enc_20_4E4F30 (key, 0x87959F1F);
		TRY(enc_20_major_2 (key, KEY(0x34)));
	}

	if(type == 0x1)
	{
		enc_20_4EB2A0 (key, 0x643C9851);
		KEY(0x48) *= KEY(0x28) + 0x466E09CF;
		TRY(enc_20_major_5 (key, seed));
	}

	var_18 += 0x51CE37 - 0x287AE3DA;
	KEY(0x48) &= KEY(0x44) + 0x21012257;

	if(type == 0x0E)
	{
		KEY(0x18) += 0xFE07AF0E - KEY(0x0C);
		TRY(enc_20_4BAC10 (key, 0x51F9A91A));
		TRY(enc_20_major_12 (key, KEY(0x38)));
	}

	if(type == 0x0C)
	{
		KEY(0x38) |= KEY(0x0C) ^ 0x04345732;
		KEY(0x40) += 0x485c892b;
		TRY(enc_20_major_18 (key, KEY(0x30)));
	}

	KEY(0x4C) &= KEY(0x28) ^ 0x6FC516D5;

	if(type == 0x6)
	{
		TRY(enc_20_4DA120 (key));
		KEY(0x30) += KEY(0x18) ^ 0x211F5E40;
		TRY(enc_20_major_15 (key, KEY(0x34)));
	}

	if(type == 0x0B)
	{
		TRY(enc_20_4DA200 (key));
		enc_20_4D6FB0 (key);
		TRY(enc_20_major_3 (key, seed));
	}

	var_18 += 0x51CE37 - 0x5875981F;
	KEY(0x20) ^= KEY(0x2C) * 41 * 3;

	if(type == 0x4)
	{
		KEY(0x08) *= KEY(0x0C) + 0x0D6863A6;
		enc_20_4C07D0 (key);
		TRY(enc_20_major_21 (key, KEY(0x10)));
	}

	if(type == 0x3)
	{
		KEY(0x4C) ^= KEY(0x3C) ^ 0x03574ED3;
		enc_20_4D70D0 (key);
		TRY(enc_20_major_16 (key, KEY(0x24)));
	}

	KEY(0x00) += KEY(0x34) + var_18;

	if(type == 0x0)
	{
		TRY(enc_20_4BAC10 (key, 0x0F10F9D87));
		KEY(0x30) ^= KEY(0x3C) - 0x0F5CFDE0;
		TRY(enc_20_major_1 (key, KEY(0x3C)));
	}

	if(type == 0x9)
	{
		KEY(0x34) -= KEY(0x04);
		KEY(0x08) ^= my_sin(KEY(0x34)) ? 0x0FD08092 : KEY(0x28);
		TRY(enc_20_major_14 (key, KEY(0x48)));
	}

	var_18 += 0x51CE37 - 0x3809EB5D;
	seed *= KEY(0x20) - 0x44260E37;

	if(type == 0x3)
	{
		enc_20_4C07D0 (key);
		KEY(0x28) += 0x08958821;
		TRY(enc_20_major_6 (key, KEY(0x20)));
	}

	if(type == 0x0D)
	{
		KEY(0x00) += KEY(0x48) ^ 0x4AC16B8D;
		KEY(0x1C) ^= 0x129D6C5E;
		TRY(enc_20_major_9 (key, seed));
	}

	KEY(0x08) &= ROL(KEY(0x4C), (unsigned char)var_18);

	RETURN;
}


THROWS_RET enc_20_major_12 (unsigned char *key, unsigned int seed)
{
	unsigned int type = (KEY(0x1C) ^ seed ^ KEY(0x48)) & 0x0F;
	unsigned int var_18 = 0x9E24650;

	if(type == 0x0F)
	{
		TRY(enc_20_4DA4D0 (key));
		KEY(0x34) += KEY(0x3C) < 0x137BFFEB ? KEY(0x3C) : KEY(0x2C);
		TRY(enc_20_major_4 (key, KEY(0x44)));
	}

	var_18 += 0x51CE37 - 0x28E92C0D;
	KEY(0x20) |= seed + 0xE43FC6B;

	if(type == 0x1)
	{
		enc_20_4EB2C0 (key, 0x979304F6);
		enc_20_4C07D0 (key);
		TRY(enc_20_major_18 (key, KEY(0x18)));
	}

	if(type == 0x8)
	{
		enc_20_4E7FB0 (key);
		TRY(enc_20_4A1640 (key, 0x0F7131053));
		TRY(enc_20_major_15 (key, KEY(0x34)));
	}

	KEY(0x4C) ^= seed * 75;

	if(type == 0x1)
	{
		enc_20_4AB3B0 (key, 0x7F675F82);
		KEY(0x34) *= 0x2C9514D7;
		TRY(enc_20_major_3 (key, KEY(0x44)));
	}

	if(type == 0x0A)
	{
		KEY(0x24) |= KEY(0x1C) ^ 0x2A19119F;
		TRY(enc_20_4AE420 (key, 0x433A0094));
		TRY(enc_20_major_21 (key, KEY(0x20)));
	}

	var_18 += 0x51CE37 - 0x5763A27F;
	KEY(0x04) ^= (KEY(0x38) * 11) << 1;

	if(type == 0x0)
	{
		enc_20_4AB3B0 (key, 0x0D644EBB);
		TRY(enc_20_4DA200 (key));
		TRY(enc_20_major_16 (key, KEY(0x38)));
	}

	KEY(0x1C) |= seed ^ 0xE857063;

	if(type == 0x4)
	{
		KEY(0x48) -=  KEY(0x34) ^ 0x154ABCDF;
		enc_20_4C4A60 (key, 0x0FFF1F0B9);
		TRY(enc_20_major_1 (key, KEY(0x18)));
	}

	if(type == 0x9)
	{
		TRY(enc_20_4D56B0 (key));
		enc_20_4BABE0 (key, 0x6E480136);
		TRY(enc_20_major_14 (key, KEY(0x1C)));
	}

	var_18 += 0x51CE37 - 0x676F0B3;
	KEY(0x18) = ROR(KEY(0x18), (unsigned char)(KEY(0x24) * var_18));

	if(type == 0x3)
	{
		TRY(enc_20_49AA90 (key, 0x0CD88EA76));
		TRY(enc_20_4EEC50 (key));
		TRY(enc_20_major_6 (key, KEY(0x4C)));
	}

	if(type == 0x0D)
	{
		enc_20_4AE3C0 (key, 0x6E062619);
		TRY(enc_20_49B1F0 (key, 0x408B95D2));
		TRY(enc_20_major_9 (key, KEY(0x4)));
	}

	KEY(0x18) -= KEY(0x44) < 0x417E2F7B ? KEY(0x44) : KEY(0x4C);

	if(type == 0x5)
	{
		KEY(0x24) |= KEY(0x1C) ^ 0x2A19119F;
		enc_20_4E7CC0 (key);
		TRY(enc_20_major_2 (key, KEY(0x4)));
	}

	if(type == 0x0C)
	{
		KEY(0x34) -= KEY(0x04);
		KEY(0x00) += (KEY(0x18) * 15) << 2;
		TRY(enc_20_major_5 (key, KEY(0x1C)));
	}

	var_18 += 0x51CE37 - 0x4496C1CE;
	KEY(0x18) |= my_sqrt(seed);

	if(type == 0x2)
	{
		KEY(0x08) ^= KEY(0x3C) << 5;
		KEY(0x1C) &= KEY(0x34) ^ 0x21AAF758;
		TRY(enc_20_major_4 (key, seed));
	}

	KEY(0x08) ^= KEY(0x20) + 0x3E85747B;

	if(type == 0x0)
	{
		enc_20_4E7FB0 (key);
		enc_20_4EB2A0 (key, 0x7F696176);
		TRY(enc_20_major_18 (key, KEY(0x38)));
	}

	if(type == 0x0B)
	{
		KEY(0x18) += 0xFE07AF0E - KEY(0x0C);
		enc_20_4EB2C0 (key, 0x0BF47F027);
		TRY(enc_20_major_15 (key, KEY(0x0C)));
	}

	var_18 += 0x51CE37 - 0x345F167D;
	KEY(0x08) &= seed;

	if(type == 0x3)
	{
		enc_20_4AB3B0 (key, 0x69AFA76);
		KEY(0x48) -=  KEY(0x34) ^ 0x154ABCDF;
		TRY(enc_20_major_3 (key, KEY(0x10)));
	}

	if(type == 0x7)
	{
		KEY(0x30) ^= KEY(0x3C) - 0x0F5CFDE0;
		TRY(enc_20_4D7900 (key));
		TRY(enc_20_major_21 (key, KEY(0x38)));
	}

	seed += KEY(0x24) - var_18;

	if(type == 0x0E)
	{
		TRY(enc_20_4EEC50 (key));
		KEY(0x24) ^= ((KEY(0x1C) << 4) + KEY(0x1C)) << 2;
		TRY(enc_20_major_16 (key, seed));
	}

	var_18 += 0x51CE37 - 0x2393ADEE;
	KEY(0x48) += KEY(0x2C) * 91;

	if(type == 0x2)
	{
		KEY(0x1C) &= KEY(0x34) ^ 0x21AAF758;
		enc_20_4D70D0 (key);
		TRY(enc_20_major_1 (key, KEY(0x4)));
	}

	if(type == 0x6)
	{
		KEY(0x08) ^= KEY(0x3C) << 5;
		KEY(0x48) -=  KEY(0x34) ^ 0x154ABCDF;
		TRY(enc_20_major_14 (key, seed));
	}

	KEY(0x10) ^= KEY(0x10) - var_18;

	RETURN;
}


THROWS_RET enc_20_major_13 (unsigned char *key, unsigned int seed)
{
	unsigned int type = (KEY(0x10) ^ seed ^ KEY(0x48)) & 0x0F;
	unsigned int var_18 = 0x26334b11;

	if(type == 0x0C)
	{
		enc_20_4D7000 (key);
		KEY(0x24) |= KEY(0x1C) ^ 0x2A19119F;
		TRY(enc_20_major_15 (key, KEY(0x2C)));
	}

	if(type == 0x4)
	{
		enc_20_4AE3C0 (key, 0x0A095474);
		KEY(0x30) ^= KEY(0x3C) - 0x0F5CFDE0;
		TRY(enc_20_major_3 (key, seed));
	}

	seed ^= (KEY(0x04) * 27) << 2;

	if(type == 0x1)
	{
		TRY(enc_20_4A1640 (key, 0x0AD86172C));
		KEY(0x30) ^= KEY(0x3C) - 0x0F5CFDE0;
		TRY(enc_20_major_21 (key, KEY(0x1C)));
	}

	if(type == 0x9)
	{
		KEY(0x00) += (KEY(0x18) * 15) << 2;
		enc_20_4EB2A0 (key, 0x0C7D3C53A);
		TRY(enc_20_major_16 (key, KEY(0x28)));
	}

	KEY(0x2C) += KEY(0x20) - 0xEF3B680;

	if(type == 0x5)
	{
		enc_20_4E7FB0 (key);
		KEY(0x08) ^= my_sin(KEY(0x34)) ? 0x0FD08092 : KEY(0x28);
		TRY(enc_20_major_1 (key, seed));
	}

	if(type == 0x2)
	{
		enc_20_4C4A60 (key, 0x26995743);
		KEY(0x24) += ROL(KEY(0x10), 0xE9);
		TRY(enc_20_major_14 (key, KEY(0x0C)));
	}

	var_18 += 0x51CE37 - 0x3B6D1B57;
	KEY(0x4C) -= seed ^ 0x42B04005;

	if(type == 0x8)
	{
		TRY(enc_20_4E7C50 (key));
		KEY(0x00) += KEY(0x48) ^ 0x4AC16B8D;
		TRY(enc_20_major_6 (key, KEY(0x8)));
	}

	if(type == 0x3)
	{
		TRY(enc_20_4E7C50 (key));
		enc_20_4D70D0 (key);
		TRY(enc_20_major_9 (key, seed));
	}

	KEY(0x00) += my_sqrt(KEY(0x40));

	if(type == 0x0B)
	{
		KEY(0x30) ^= KEY(0x3C) - 0x0F5CFDE0;
		enc_20_49B260 (key, 0x3D2FA7C7);
		TRY(enc_20_major_2 (key, KEY(0x4C)));
	}

	if(type == 0x0D)
	{
		TRY(enc_20_4D7900 (key));
		enc_20_4C07D0 (key);
		TRY(enc_20_major_5 (key, KEY(0x1C)));
	}

	var_18 += 0x51CE37 - 0x921FDE;
	seed += KEY(0x44) | var_18;

	if(type == 0x1)
	{
		TRY(enc_20_4AE420 (key, 0x0D3280A0));
		KEY(0x1C) ^= 0x3EB9D37;
		TRY(enc_20_major_12 (key, KEY(0x30)));
	}

	if(type == 0x0E)
	{
		enc_20_4D6FB0 (key);
		enc_20_4E4F30 (key, 0x994A9D7F);
		TRY(enc_20_major_11 (key, seed));
	}

	KEY(0x08) = ROR(KEY(0x08), (unsigned char)(KEY(0x3C) < 0x3F2998C ? KEY(0x3C) : seed));

	if(type == 0x5)
	{
		TRY(enc_20_4A1640 (key, 0x2DD0E73));
		enc_20_4C07D0 (key);
		TRY(enc_20_major_15 (key, seed));
	}

	if(type == 0x3)
	{
		TRY(enc_20_49AA90 (key, 0x21602B81));
		enc_20_4AB3B0 (key, 0x4CF85493);
		TRY(enc_20_major_3 (key, seed));
	}

	var_18 += 0x51CE37 - 0x1979BCBA;
	KEY(0x10) += KEY(0x08) ^ 0x1579499;

	if(type == 0x0A)
	{
		TRY(enc_20_4DA4D0 (key));
		KEY(0x14) += KEY(0x00) ^ 0x3E17ADD3;
		TRY(enc_20_major_21 (key, KEY(0x14)));
	}

	if(type == 0x0)
	{
		TRY(enc_20_4DA120 (key));
		KEY(0x00) += (KEY(0x18) * 15) << 2;
		TRY(enc_20_major_16 (key, KEY(0x20)));
	}
	
	seed += (0 - (((KEY(0x08) * 15) << 1) - KEY(0x08))) * 4;

	if(type == 0x0)
	{
		KEY(0x34) -= KEY(0x04);
		KEY(0x4C) ^= KEY(0x3C) ^ 0x03574ED3;
		TRY(enc_20_major_1 (key, seed));
	}

	if(type == 0x0F)
	{
		enc_20_4EB2A0 (key, 0x58549EB8);
		TRY(enc_20_4D76F0 (key, 0x0BCBC7BB));
		TRY(enc_20_major_14 (key, seed));
	}

	var_18 += 0x51CE37 - 0x23629E3F;
	KEY(0x28) -= KEY(0x28) | var_18;

	if(type == 0x6)
	{
		TRY(enc_20_4D56B0 (key));
		TRY(enc_20_4DA200 (key));
		TRY(enc_20_major_6 (key, KEY(0x2C)));
	}

	if(type == 0x4)
	{
		KEY(0x08) ^= my_sin(KEY(0x34)) ? 0x0FD08092 : KEY(0x28);
		enc_20_4AB3B0 (key, 0x3F5EC9D);
		TRY(enc_20_major_9 (key, KEY(0x4C)));
	}

	seed += KEY(0x44) ^ var_18;

	if(type == 0x7)
	{
		TRY(enc_20_4EEC50 (key));
		KEY(0x48) *= KEY(0x28) + 0x466E09CF;
		TRY(enc_20_major_2 (key, KEY(0x20)));
	}

	if(type == 0x2)
	{
		TRY(enc_20_4D56B0 (key));
		KEY(0x24) |= KEY(0x1C) ^ 0x2A19119F;
		TRY(enc_20_major_5 (key, KEY(0x48)));
	}

	KEY(0x40) -= KEY(0x2C) < 0x1E7D86EE ? KEY(0x2C) : seed;

	RETURN;
}


THROWS_RET enc_20_major_14 (unsigned char *key, unsigned int seed)
{
	unsigned int type = (KEY(0x20) ^ seed ^ KEY(0x2C)) % 0x0E;
	unsigned int var_18 = 0xf74450ff;

	if(type == 0x0)
	{
		TRY(enc_20_4AE420 (key, 0x0E32BDCA0));
		enc_20_4D7080 (key);
		TRY(enc_20_major_23 (key, KEY(0x4C)));
	}

	seed -= seed ^ var_18;
	
	if(type == 0x1)
	{
		enc_20_4EB2C0 (key, 0x788C78A4);
		KEY(0x34) -= KEY(0x04);
		TRY(enc_20_major_24 (key, seed));
	}

	KEY(0x34) -= my_cos(KEY(0x0C)) ? var_18 : KEY(0x10);

	if(type == 0x9)
	{
		KEY(0x24) |= KEY(0x1C) ^ 0x2A19119F;
		enc_20_4AE3C0 (key, 0x79D06B80);
		TRY(enc_20_major_19 (key, seed));
	}

	var_18 += 0x51CE37 - 0x2E870A32;
	KEY(0x24) ^= ((KEY(0x18) * 45) << 1) - KEY(0x18);

	if(type == 0x7)
	{
		enc_20_4C07D0 (key);
		KEY(0x18) += 0xFE07AF0E - KEY(0x0C);
		TRY(enc_20_major_25 (key, KEY(0x2C)));
	}

	if(type == 0x8)
	{
		KEY(0x34) -= KEY(0x04);
		KEY(0x10) ^= ROR(0x292E3197, 0x85);
		TRY(enc_20_major_17 (key, KEY(0x1C)));
	}

	KEY(0x04) ^= my_sin(seed) ? var_18 : KEY(0x44);

	if(type == 0x0D)
	{
		enc_20_4EB2A0 (key, 0x7EDBE9A7);
		KEY(0x48) -=  KEY(0x34) ^ 0x154ABCDF;
		TRY(enc_20_major_4 (key, KEY(0x10)));
	}

	var_18 += 0x51CE37 - 0x2E50FA12;
	KEY(0x44) += KEY(0x34) < 0xAC24EB8 ? KEY(0x34) : KEY(0x24);

	if(type == 0x5)
	{
		KEY(0x10) ^= ROR(0x798A34E7, 0x85);
		enc_20_4C4A60 (key, 0x56245E85);
		TRY(enc_20_major_18 (key, KEY(0x4)));
	}

	seed |= ROR(KEY(0x48), 0xEB);

	if(type == 0x3)
	{
		enc_20_4E7FB0 (key);
		KEY(0x14) -= KEY(0x3C);
		TRY(enc_20_major_15 (key, KEY(0x00)));
	}

	var_18 += 0x51CE37 - 0x14D04E2A;
	KEY(0x10) += seed + 0xF65EFBD;

	if(type == 0x0A)
	{
		TRY(enc_20_4DA120 (key));
		KEY(0x08) *= KEY(0x0C) + 0x0D6863A6;
		TRY(enc_20_major_3 (key, KEY(0x14)));
	}

	if(type == 0x0B)
	{
		enc_20_4BABE0 (key, 0x7E789968);
		TRY(enc_20_4DA200 (key));
		TRY(enc_20_major_21 (key, seed));
	}

	KEY(0x10) ^= ROL(KEY(0x20), (unsigned char)var_18);

	if(type == 0x6)
	{
		TRY(enc_20_4D76F0 (key, 0x0EC30BD82));
		KEY(0x08) *= KEY(0x0C) + 0x0D6863A6;
		TRY(enc_20_major_16 (key, KEY(0x34)));
	}

	var_18 += 0x51CE37 - 0x214227C5;
	seed *= KEY(0x18) + 0x6BBEB974;

	if(type == 0x2)
	{
		KEY(0x48) *= KEY(0x28) + 0x466E09CF;
		KEY(0x08) *= KEY(0x0C) + 0x0D6863A6;
		TRY(enc_20_major_1 (key, KEY(0x18)));
	}

	KEY(0x40) -= KEY(0x08) * var_18;

	if(type == 0x0C)
	{
		KEY(0x4C) ^= KEY(0x3C) ^ 0x03574ED3;
		enc_20_4AE3C0 (key, 0x7667AFED);
		TRY(enc_20_major_23 (key, KEY(0x38)));
	}

	KEY(0x34) = ROR(KEY(0x34), (unsigned char)my_sqrt(seed));

	if(type == 0x4)
	{
		enc_20_4E7CC0 (key);
		enc_20_4D70D0 (key);
		TRY(enc_20_major_24 (key, KEY(0x00)));
	}

	if(type == 0x0)
	{
		KEY(0x1C) ^= 0x0C9D1F4A2;
		enc_20_4C07D0 (key);
		TRY(enc_20_major_19 (key, seed));
	}

	KEY(0x30) -= my_sin(KEY(0x28)) ? 0x2818AE3C : seed;

	RETURN;
}


THROWS_RET enc_20_major_15 (unsigned char *key, unsigned int seed)
{
	unsigned int type = (KEY(0x44) ^ seed ^ KEY(0x4C)) % 0x09;
	unsigned int var_14 = 0xf9976b51;

	KEY(0x4C) = ROR(KEY(0x4C), (unsigned char)(KEY(0x4C) + 0x222C438A));

	if(type == 0x4)
	{
		enc_20_4E7CC0 (key);
		enc_20_4D5720 (key, 0x0CF99677E);
		enc_20_4EB310 (key);
	}
	
	KEY(0x14) ^= seed + 0x1FF8749D;

	if(type == 0x5)
	{
		enc_20_49B260 (key, 0x95649C47);
		KEY(0x24) += ROL(KEY(0x10), 0xE9);
		enc_20_4D7790 (key);
	}

	var_14 += 0x51CE37 - 0x4E1FF97A;
	KEY(0x34) ^= KEY(0x3C) + 0x19AD9D3;

	if(type == 0x0)
	{
		KEY(0x38) |= KEY(0x0C) ^ 0x04345732;
		enc_20_4C07D0 (key);
		TRY(enc_20_major_23 (key, KEY(0x34)));
	}

	KEY(0x0C) = ROR(KEY(0x0C), (unsigned char)my_sqrt(KEY(0x24)));

	if(type == 0x1)
	{
		KEY(0x40) += 0x188ae78f;
		KEY(0x08) ^= KEY(0x3C) << 5;
		TRY(enc_20_major_24 (key, KEY(0x30)));
	}

	var_14 += 0x51CE37 - 0x29D1BF9E;
	seed ^= KEY(0x30) ^ var_14;

	if(type == 0x0)
	{
		KEY(0x38) |= KEY(0x0C) ^ 0x04345732;
		enc_20_4BABE0 (key, 0x640D6589);
		TRY(enc_20_major_19 (key, KEY(0x18)));
	}

	KEY(0x00) = ROL(KEY(0x00), (unsigned char)(var_14 & seed));

	if(type == 0x7)
	{
		enc_20_4E7FB0 (key);
		TRY(enc_20_49AA90 (key, 0x0D3D79CB4));
		TRY(enc_20_major_25 (key, KEY(0x18)));
	}

	var_14 += 0x51CE37 - 0x43D4EFBB;
	KEY(0x48) ^= KEY(0x24) - 0x5606038;

	if(type == 0x3)
	{
		KEY(0x34) += KEY(0x3C) < 0x137BFFEB ? KEY(0x3C) : KEY(0x2C);
		KEY(0x40) += 0x6a07a3d0;
		TRY(enc_20_major_17 (key, KEY(0x20)));
	}

	KEY(0x24) |= my_sin(KEY(0x1C)) ? var_14 : KEY(0x18);

	if(type == 0x2)
	{
		KEY(0x48) *= KEY(0x28) + 0x466E09CF;
		TRY(enc_20_4DA200 (key));
		TRY(enc_20_major_4 (key, KEY(0x4)));
	}

	if(type == 0x6)
	{
		enc_20_4D4100 (key);
		enc_20_4F2AF0 (key);
		TRY(enc_20_major_18 (key, KEY(0x00)));
	}

	RETURN;
}


THROWS_RET enc_20_major_16 (unsigned char *key, unsigned int seed)
{
	unsigned int type = (KEY(0x2C) ^ seed ^ KEY(0x14)) % 0x0C;
	unsigned int var_18 = 0x16BFB62C;

	if(type == 0x5)
	{
		KEY(0x08) *= KEY(0x0C) + 0x0D6863A6;
		enc_20_4F2AF0 (key);
		enc_20_4EB310 (key);
	}

	var_18 += 0x51CE37 - 0xC1435B7;
	KEY(0x10) ^= seed - var_18;

	if(type == 0x2)
	{
		TRY(enc_20_4DA120 (key));
		KEY(0x00) += KEY(0x48) ^ 0x4AC16B8D;
		enc_20_4D7790 (key);
	}

	KEY(0x3C) -= var_18 ^ seed;

	if(type == 0x0)
	{
		TRY(enc_20_4BAC10 (key, 0x80E3E69E));
		enc_20_4AE3C0 (key, 0x8BD64F99);
		TRY(enc_20_major_23 (key, KEY(0x10)));
	}

	var_18 += 0x51CE37 - 0x3E87DD87;
	KEY(0x20) ^= my_sqrt(KEY(0x40));

	if(type == 0x3)
	{
		enc_20_4D7660 (key, 0x3BA8DA0B);
		KEY(0x34) -= KEY(0x04);
		TRY(enc_20_major_24 (key, seed));
	}

	if(type == 0x0A)
	{
		KEY(0x18) += KEY(0x4C) - 0x3F5675D6;
		KEY(0x34) *= 0x0A02FE00;
		TRY(enc_20_major_19 (key, KEY(0x18)));
	}

	seed -= seed & 0x179DA692;

	if(type == 0x4)
	{
		TRY(enc_20_4EEC50 (key));
		TRY(enc_20_4DA120 (key));
		TRY(enc_20_major_25 (key, KEY(0x00)));
	}

	var_18 += 0x51CE37 - 0xE35570;
	KEY(0x20) ^= ((KEY(0x3C) * 3) << 5) - KEY(0x3C);

	if(type == 0x0)
	{
		enc_20_4EB2C0 (key, 0x6191EFEC);
		KEY(0x34) += KEY(0x3C) < 0x137BFFEB ? KEY(0x3C) : KEY(0x2C);
		TRY(enc_20_major_17 (key, KEY(0x24)));
	}

	KEY(0x18) &= my_sin(seed) ? var_18 : KEY(0x38);

	if(type == 0x9)
	{
		KEY(0x18) += 0xFE07AF0E - KEY(0x0C);
		KEY(0x00) += KEY(0x48) ^ 0x4AC16B8D;
		TRY(enc_20_major_4 (key, KEY(0x00)));
	}

	if(type == 0x6)
	{
		KEY(0x1C) |= 0xa885099;
		enc_20_4D7660 (key, 0x3E9D8E2F);
		TRY(enc_20_major_18 (key, seed));
	}

	var_18 += 0x51CE37 - 0x390A1A2;

	seed -= my_cos(KEY(0x4C)) ? 0xC818C81 : KEY(0x4C);

	if(type == 0x7)
	{
		enc_20_4C4A60 (key, 0x65CCD7B0);
		KEY(0x48) -=  KEY(0x34) ^ 0x154ABCDF;
		TRY(enc_20_major_15 (key, KEY(0x38)));
	}

	KEY(0x28) += KEY(0x04) + 0x217F7A00;

	if(type == 0x1)
	{
		KEY(0x00) += KEY(0x48) ^ 0x4AC16B8D;
		enc_20_4D4100 (key);
		TRY(enc_20_major_3 (key, KEY(0x44)));
	}

	var_18 += 0x51CE37 - 0x3B888541;
	KEY(0x14) &= ROR(KEY(0x00), (unsigned char)var_18);

	if(type == 0x8)
	{
		KEY(0x08) *= KEY(0x0C) + 0x0D6863A6;
		KEY(0x24) ^= ((KEY(0x1C) << 4) + KEY(0x1C)) << 2;
		TRY(enc_20_major_21 (key, KEY(0x34)));
	}

	if(type == 0x0B)
	{
		enc_20_4D7080 (key);
		KEY(0x34) += KEY(0x3C) < 0x137BFFEB ? KEY(0x3C) : KEY(0x2C);
		enc_20_4EB310 (key);
	}

	KEY(0x30) |= ROL(KEY(0x1C), (unsigned char)var_18);
	
	RETURN;
}


THROWS_RET enc_20_major_17 (unsigned char *key, unsigned int seed)
{
	unsigned int type = (KEY(0x20) ^ KEY(0x1C) ^ KEY(0x30)) % 0x06;
	unsigned int var_14 = 0xb87e62ab;

	KEY(0x04) |= KEY(0x10) ^ 0x10104D4;

	if(type == 0x3)
	{
		enc_20_4E7FB0 (key);
		enc_20_4AE3C0 (key, 0x0A9EB159D);
		enc_20_4EB310 (key);
	}

	var_14 += 0x51CE37 - 0x3CB4EBF6;
	seed = ((seed ^ 0x1EA9DA8) + seed) * KEY(0x48) * 13;

	if(type == 0x0)
	{
		TRY(enc_20_4AE420 (key, 0x10381FF0));
		KEY(0x08) *= KEY(0x0C) + 0x0D6863A6;
		enc_20_4D7790 (key);
	}
	
	KEY(0x38) += KEY(0x30) * 25;
	var_14 += 0x51CE37 - 0x14CF48EC;
	KEY(0x08) -= my_sqrt(KEY(0x14));

	if(type == 0x4)
	{
		enc_20_4ABC40 (key, 0x39E08612);
		KEY(0x24) ^= ((KEY(0x1C) << 4) + KEY(0x1C)) << 2;
		TRY(enc_20_major_23 (key, seed));
	}

	KEY(0x18) &= KEY(0x10) - var_14;
	var_14 += 0x51CE37 - 0x2A19B6A4;
	KEY(0x04) ^= KEY(0x40) + 0x988DB31;

	if(type == 0x0)
	{
		KEY(0x1C) ^= 0x0A98896DD;
		TRY(enc_20_4D56B0 (key));
		TRY(enc_20_major_24 (key, KEY(0x18)));
	}

	KEY(0x18) += ROR(seed, (unsigned char)var_14);
	var_14 += 0x51CE37 - 0x143D834B;
	seed -= KEY(0x00) < var_14 ? KEY(0x00) : KEY(0x0C);

	if(type == 0x2)
	{
		enc_20_49CED0 (key);
		KEY(0x30) ^= KEY(0x3C) - 0x0F5CFDE0;
		TRY(enc_20_major_19 (key, seed));
	}

	seed *= my_sqrt(seed);
	var_14 += 0x51CE37 - 0x9210725;

	KEY(0x14) *= my_cos(seed) ? var_14 : KEY(0x4C);

	if(type == 0x5)
	{
		enc_20_4D6FB0 (key);
		KEY(0x34) *= 0x0A02FE00;
		TRY(enc_20_major_25 (key, KEY(0x34)));
	}

	RETURN;
}


THROWS_RET enc_20_major_18 (unsigned char *key, unsigned int seed)
{
	unsigned int type = (KEY(0x38) ^ KEY(0x2C) ^ KEY(0x44)) & 0x07;
	unsigned int var_14 = 0x128c2b75;

	KEY(0x2C) ^= ROR(KEY(0x34), (unsigned char)var_14);

	if(type == 0x5)
	{
		KEY(0x18) += KEY(0x4C) - 0x3F5675D6;
		enc_20_4D7660 (key, 0x3A03C13B);
		enc_20_4EB310 (key);
	}

	KEY(0x0C) = ROR(KEY(0x0C), (unsigned char)(KEY(0x40) * 37 * 3));

	if(type == 0x2)
	{
		KEY(0x14) += KEY(0x00) ^ 0x3E17ADD3;
		enc_20_4F2A40 (key);
		enc_20_4D7790 (key);
	}

	var_14 += 0x51CE37 - 0x38937140;
	KEY(0x2C) -= my_sqrt(KEY(0x24));
	KEY(0x30) += 0x17267C5B - KEY(0x2C);

	if(type == 0x3)
	{
		enc_20_4E4F30 (key, 0x0CA8D3E2B);
		KEY(0x1C) &= KEY(0x34) ^ 0x21AAF758;
		TRY(enc_20_major_23 (key, KEY(0x00)));
	}

	var_14 += 0x51CE37 - 0x390FD83B;
	KEY(0x44) ^= seed ^ 0x35EDDEA4; 

	if(type == 0x0)
	{
		KEY(0x28) += 0x3409139C;
		KEY(0x4C) ^= my_cos(KEY(0x24)) ? 0x057337B8 : KEY(0x38);
		TRY(enc_20_major_24 (key, KEY(0x18)));
	}

	KEY(0x18) *= KEY(0x44) + 0xB89B51C;

	if(type == 0x1)
	{
		KEY(0x18) += 0xFE07AF0E - KEY(0x0C);
		TRY(enc_20_4BAC10 (key, 0x90254266));
		TRY(enc_20_major_19 (key, KEY(0x18)));
	}

	var_14 += 0x51CE37 - 0x4A885C91;
	KEY(0x4C) ^= KEY(0x0C) < var_14 ? KEY(0x0C) : KEY(0x04);
	KEY(0x3C) ^= ((KEY(0x30) * 3) << 3) - KEY(0x30);

	if(type == 0x7)
	{
		KEY(0x34) *= ROR(KEY(0x0C), 0xA5);
		KEY(0x34) *= ROR(KEY(0x0C), 0xA5);
		TRY(enc_20_major_25 (key, KEY(0x24)));
	}

	var_14 += 0x51CE37 - 0x47CA3B11;
	KEY(0x28) += 0x395F1D29 - seed;

	if(type == 0x0)
	{
		enc_20_4C4A60 (key, 0x29CFE0BE);
		enc_20_4AB3B0 (key, 0x42E1F1A9);
		TRY(enc_20_major_17 (key, KEY(0x40)));
	}

	KEY(0x04) = ROL(KEY(0x04), (unsigned char)ROL(KEY(0x20),(unsigned char)var_14));
	var_14 += 0x51CE37 - 0x466E93D6;
	seed -= KEY(0x24) ^ var_14;

	if(type == 0x6)
	{
		TRY(enc_20_4BAC10 (key, 0x10B4EAEF));
		enc_20_4C4A60 (key, 0x298D7844);
		TRY(enc_20_major_4 (key, seed));
	}

	KEY(0x48) = ROL(KEY(0x48), (unsigned char)(KEY(0x1C) & 0x34490731));

	RETURN;
}


THROWS_RET enc_20_major_19 (unsigned char *key, unsigned int seed)
{
	unsigned int type = (KEY(0x48) ^ KEY(0x18) ^ KEY(0x3C)) & 0x03;
	unsigned int var_14 = 0xe42c799d;

	{
		unsigned int var = KEY(0x3C) * seed * 0x3C02927;
		seed = ROR(var, (unsigned char)(((var * 9) << 3) - var));
	}

	if(type == 0x0)
	{
		KEY(0x30) += KEY(0x18) ^ 0x211F5E40;
		enc_20_4D7660 (key, 0x58053BA7);
		enc_20_4EB310 (key);
	}

	var_14 += 0x51CE37 - 0x2B57AC24;
	seed ^= KEY(0x18) ^ 0xC1FCDA0;

	KEY(0x14) -= my_cos(KEY(0x18)) ? var_14 : KEY(0x28);

	if(type == 0x0)
	{
		enc_20_4D7660 (key, 0x4370ACC2);
		enc_20_4ABC40 (key, 0x56BB8205);
		enc_20_4D7790 (key);
	}

	var_14 += 0x51CE37 - 0x4EADE9E0;
	seed *= KEY(0x4C) + 0x11500E47;	
	KEY(0x0C) ^= ROL(KEY(0x10), 0x34);

	if(type == 0x3)
	{
		KEY(0x08) ^= KEY(0x3C) << 5;
		KEY(0x4C) ^= KEY(0x3C) ^ 0x03574ED3;
		TRY(enc_20_major_23 (key, KEY(0x3C)));
	}

	var_14 += 0x51CE37 - 0x399B1DAD;
	KEY(0x34) -= my_sqrt(seed);

	if(my_cos(seed))
		seed = ROR(seed, 0x7);
	else
		seed = ROR(seed, (unsigned char)KEY(0x28));

	KEY(0x40) = (KEY(0x3C) * KEY(0x40) * 37) << 1;

	if(type == 0x1)
	{
		KEY(0x1C) ^= 0x0B3BB63F;
		KEY(0x10) -= KEY(0x44) ^ 0x2217CF47;
		TRY(enc_20_major_24 (key, seed));
	}

	RETURN;
}


THROWS_RET enc_20_major_21 (unsigned char *key, unsigned int seed)
{
	unsigned int type = (KEY(0x04) ^ KEY(0x00) ^ KEY(0x40)) % 0x0B;
	unsigned int var_14 = 0xcb1d507c;

	if(type == 0x2)
	{
		KEY(0x08) *= KEY(0x0C) + 0x0D6863A6;
		KEY(0x30) ^= KEY(0x3C) - 0x0F5CFDE0;
		enc_20_4EB310 (key);
	}

	KEY(0x14) -= seed;

	if(type == 0x8)
	{
		KEY(0x40) += 0x2b058ae8;
		KEY(0x18) += 0xFE07AF0E - KEY(0x0C);
		enc_20_4D7790 (key);
	}

	KEY(0x44) ^= ROL(KEY(0x48), (unsigned char)var_14);

	if(type == 0x4)
	{
		KEY(0x08) *= KEY(0x0C) + 0x0D6863A6;
		enc_20_4EB2C0 (key, 0x79FB5201);
		TRY(enc_20_major_23 (key, KEY(0x1C)));
	}

	var_14 += 0x51CE37 - 0x419B6D78;
	KEY(0x00) ^= my_sqrt(KEY(0x30));

	if(type == 0x0)
	{
		KEY(0x4C) ^= KEY(0x3C) ^ 0x03574ED3;
		TRY(enc_20_4DA120 (key));
		TRY(enc_20_major_24 (key, KEY(0x8)));
	}

	KEY(0x28) ^= (seed * 27) << 2;

	if(type == 0x9)
	{
		KEY(0x08) ^= my_sin(KEY(0x34)) ? 0x0FD08092 : KEY(0x28);
		enc_20_4EB2C0 (key, 0x6DDF8C10);
		TRY(enc_20_major_19 (key, KEY(0x28)));
	}

	var_14 += 0x51CE37 - 0x2CBEEE4;

	KEY(0x20) -= my_cos(KEY(0x30)) ? var_14 : seed;

	if(type == 0x7)
	{
		enc_20_4E7CC0 (key);
		enc_20_4D7000 (key);
		TRY(enc_20_major_25 (key, KEY(0x4)));
	}

	seed ^= my_sin(seed) ? 0x2C99FADE : KEY(0x38);

	if(type == 0x1)
	{
		TRY(enc_20_4A1640 (key, 0x3FCF3163));
		KEY(0x24) ^= ((KEY(0x1C) << 4) + KEY(0x1C)) << 2;
		TRY(enc_20_major_17 (key, seed));
	}

	var_14 += 0x51CE37 - 0x6D17119;

	KEY(0x3C) += my_cos(KEY(0x2C)) ? 0x1BEC01F : seed;

	if(type == 0x5)
	{
		enc_20_4AB3B0 (key, 0x15D31F7C);
		KEY(0x18) += KEY(0x4C) - 0x3F5675D6;
		TRY(enc_20_major_4 (key, KEY(0x3C)));
	}

	KEY(0x04) = ROR(KEY(0x04), (unsigned char)(var_14 * KEY(0x40)));

	if(type == 0x0)
	{
		TRY(enc_20_4D76F0 (key, 0x0FDE30E03));
		KEY(0x24) |= KEY(0x1C) ^ 0x2A19119F;
		TRY(enc_20_major_18 (key, seed));
	}

	var_14 += 0x51CE37 - 0x25D9C0CA;
	KEY(0x1C) &= KEY(0x3C) * 0xA8F285;

	if(type == 0x0A)
	{
		KEY(0x1C) ^= 0x0EF011757;
		enc_20_4AE3C0 (key, 0x2412315A);
		TRY(enc_20_major_15 (key, KEY(0x34)));
	}

	if(type == 0x3)
	{
		KEY(0x30) += KEY(0x18) + 0x21D7BF61;
		KEY(0x18) += KEY(0x4C) - 0x3F5675D6;
		TRY(enc_20_major_3 (key, KEY(0x28)));
	}

	KEY(0x0C) *= my_sin(KEY(0x20)) ? var_14 : KEY(0x08);

	if(type == 0x6)
	{
		KEY(0x24) += ROL(KEY(0x10), 0xE9);
		enc_20_4F2AF0 (key);
		enc_20_4EB310 (key);
	}

	KEY(0x2C) ^= ((KEY(0x44) << 4) + KEY(0x44)) << 2;

	RETURN;
}


THROWS_RET enc_20_major_22 (unsigned char *key, unsigned int seed)
{
	unsigned int type = (KEY(0x14) ^ KEY(0x00) ^ seed) & 0x0F;
	unsigned int var_18 = 0x7C36F793;

	if(type == 0x3)
	{
		TRY(enc_20_4DA200 (key));
		enc_20_4E4F30 (key, 0x4BB6BBB3);
		TRY(enc_20_major_3 (key, seed));
	}

	if(type == 0x0)
	{
		enc_20_4D5720 (key, 0x3B3E4742);
		KEY(0x34) -= KEY(0x04);
		TRY(enc_20_major_21 (key, KEY(0x8)));
	}

	var_18 += 0x51CE37 - 0x513E5C0;
	KEY(0x38) ^= ROL(KEY(0x40), 0x76);

	if(type == 0x0C)
	{
		KEY(0x28) += 0x830BA927;
		enc_20_4EB2C0 (key, 0x6F3A3876);
		TRY(enc_20_major_16 (key, KEY(0x20)));
	}

	if(type == 0x1)
	{
		enc_20_4F2A40 (key);
		KEY(0x40) += 0x1bc7b861;
		TRY(enc_20_major_1 (key, KEY(0x18)));
	}

	KEY(0x30) ^= KEY(0x2C) < 0x521B2180 ? KEY(0x2C) : KEY(0x24);

	if(type == 0x1)
	{
		enc_20_4E4F30 (key, 0x42261FF2);
		KEY(0x00) += (KEY(0x18) * 15) << 2;
		TRY(enc_20_major_14 (key, KEY(0x3C)));
	}

	if(type == 0x8)
	{
		TRY(enc_20_4DA120 (key));
		KEY(0x48) *= KEY(0x28) + 0x466E09CF;
		TRY(enc_20_major_6 (key, KEY(0x34)));
	}

	if(type == 0x4)
	{
		KEY(0x24) += ROL(KEY(0x10), 0xE9);
		KEY(0x08) *= KEY(0x0C) + 0x0D6863A6;
		TRY(enc_20_major_9 (key, KEY(0x40)));
	}

	var_18 += 0x51CE37 - 0x10AD1B5;
	KEY(0x48) &= my_sqrt(KEY(0x24));

	if(type == 0x5)
	{
		TRY(enc_20_4D7900 (key));
		KEY(0x08) ^= KEY(0x3C) << 5;
		TRY(enc_20_major_2 (key, KEY(0x8)));
	}

	if(type == 0x6)
	{
		KEY(0x08) *= KEY(0x0C) + 0x0D6863A6;
		KEY(0x18) += 0xFE07AF0E - KEY(0x0C);
		TRY(enc_20_major_5 (key, KEY(0x38)));
	}

	KEY(0x48) += 0 - (((KEY(0x40) * 15) << 3) - KEY(0x40));

	if(type == 0x9)
	{
		KEY(0x10) ^= ROR(0x12C33EF4, 0x85);
		enc_20_49B260 (key, 0x827C0747);
		TRY(enc_20_major_12 (key, KEY(0x28)));
	}

	if(type == 0x0A)
	{
		KEY(0x30) ^= KEY(0x3C) - 0x0F5CFDE0;
		KEY(0x34) *= 0x6CD0251E;
		TRY(enc_20_major_11 (key, KEY(0x00)));
	}

	if(type == 0x6)
	{
		KEY(0x08) *= KEY(0x0C) + 0x0D6863A6;
		enc_20_49B260 (key, 0x9CBCA826);
		TRY(enc_20_major_13 (key, seed));
	}

	var_18 += 0x51CE37 - 0x2DDCCCC0;
	KEY(0x34) ^= var_18 ^ seed;

	if(type == 0x2)
	{
		KEY(0x28) += 0x6467451;
		KEY(0x10) -= KEY(0x44) ^ 0x2217CF47;
		TRY(enc_20_major_3 (key, KEY(0x8)));
	}

	if(type == 0x7)
	{
		KEY(0x10) -= KEY(0x44) ^ 0x2217CF47;
		KEY(0x00) += KEY(0x48) ^ 0x4AC16B8D;
		TRY(enc_20_major_21 (key, KEY(0x00)));
	}

	KEY(0x18) -= my_sqrt(KEY(0x28));

	if(type == 0x3)
	{
		TRY(enc_20_49AA90 (key, 0x5B9D1F9));
		KEY(0x28) += 0x08958821;
		TRY(enc_20_major_16 (key, KEY(0x20)));
	}

	if(type == 0x4)
	{
		KEY(0x34) -= KEY(0x04);
		enc_20_4AB3B0 (key, 0x6C475699);
		TRY(enc_20_major_1 (key, seed));
	}

	if(type == 0x0D)
	{
		KEY(0x30) += KEY(0x18) + 0x21D7BF61;
		KEY(0x34) *= ROR(KEY(0x0C), 0xA5);
		TRY(enc_20_major_14 (key, KEY(0x8)));
	}

	var_18 += 0x51CE37 - 0xE3C05FB;
	seed -= ROR(KEY(0x20), 0x91);

	if(type == 0x0F)
	{
		KEY(0x4C) += 0x12B9E29D - KEY(0x30);
		KEY(0x00) += (KEY(0x18) * 15) << 2;
		TRY(enc_20_major_6 (key, KEY(0x10)));
	}

	if(type == 0x2)
	{
		KEY(0x4C) ^= my_cos(KEY(0x24)) ? 0x057337B8 : KEY(0x38);
		enc_20_4E7FB0 (key);
		TRY(enc_20_major_9 (key, KEY(0x40)));
	}

	KEY(0x38) = ROR(KEY(0x38), (unsigned char)(seed - var_18));

	if(type == 0x0B)
	{
		KEY(0x4C) ^= my_cos(KEY(0x24)) ? 0x057337B8 : KEY(0x38);
		KEY(0x24) += ROL(KEY(0x10), 0xE9);
		TRY(enc_20_major_2 (key, KEY(0x24)));
	}

	if(type == 0x5)
	{
		KEY(0x4C) += 0x12B9E29D - KEY(0x30);
		TRY(enc_20_4D7900 (key));
		TRY(enc_20_major_5 (key, KEY(0x18)));
	}

	if(type == 0x0)
	{
		enc_20_4E7FB0 (key);
		enc_20_4D5720 (key, 0x849F0F1F);
		TRY(enc_20_major_12 (key, KEY(0x38)));
	}

	var_18 += 0x51CE37 - 0x31DBA030;
	KEY(0x20) ^= ROR(seed, (unsigned char)var_18);

	if(type == 0x0E)
	{
		enc_20_4C4A60 (key, 0x5159E8CF);
		KEY(0x0C) -= KEY(0x00) ^ 0x185F3B0D;
		TRY(enc_20_major_11 (key, seed));
	}

	KEY(0x00) += my_sin(KEY(0x00)) ? var_18 : KEY(0x38);

	RETURN;
}


THROWS_RET enc_20_major_23 (unsigned char *key, unsigned int seed)
{
	unsigned int type = seed & 0x01;

	KEY(0x10) += KEY(0x20) - 0x16F911E4;
	KEY(0x24) ^= KEY(0x08) * 11;
	KEY(0x28) ^= KEY(0x1C) < 0x402226F ? KEY(0x1C) : KEY(0x08);

	seed |= KEY(0x44) - 0x1E97AEB;
	seed |= KEY(0x38) < 0xf3b1e0b3 ? KEY(0x38) : KEY(0x14);

	if(type == 0x00)
	{
		KEY(0x1C) &= KEY(0x34) ^ 0x21AAF758;
		enc_20_4EB2C0 (key, 0x640F077D);
		enc_20_4EB310 (key);
	}

	KEY(0x04) += ((((0 - KEY(0x4C)) << 2) - KEY(0x4C)) * 5) << 2;
	KEY(0x04) += seed - 0x18D1B90;
	KEY(0x1C) -= KEY(0x0C) ^ 0x44de1958;
	KEY(0x2C) ^= ROL(KEY(0x08), 0xA9);
	KEY(0x44) += ROL(KEY(0x30), 0x1B);

	if(type == 0)
	{
		TRY(enc_20_49AA90 (key, 0xDC306F47));
		KEY(0x24) ^= ((KEY(0x1C) << 4) + KEY(0x1C)) << 2;
		enc_20_4D7790 (key);
	}

	KEY(0x1C) = ROR(KEY(0x1C), (unsigned char)(KEY(0x34) + 0x16075020));

	RETURN;
}


THROWS_RET enc_20_major_24 (unsigned char *key, unsigned int seed)
{
	unsigned int type = (KEY(0x08) ^ seed ^ KEY(0x1C)) % 0x03;

	seed *= my_cos(seed) ? 0x6be8f94 : seed;

	KEY(0x08) ^= KEY(0x08) + 0x3786364B;
	KEY(0x44) = ROL(KEY(0x44), (unsigned char)(seed - 0x2A2F6EB0));

	if(type == 0)
	{
		enc_20_49CED0 (key);
		enc_20_4D4100 (key);
		enc_20_4EB310 (key);
	}

	seed += KEY(0x0C) ^ 0xff342d3c;
	seed = ROL(seed, my_sin(KEY(0x2C)) ? 0x11 : KEY(0x00));
	KEY(0x14) += my_sin(KEY(0x40)) ? 0x3AF2A8E2 : KEY(0x40);
    
	if(type == 0x00)
	{
		TRY(enc_20_4DA120 (key));
		KEY(0x08) *= KEY(0x0C) + 0x0D6863A6;
		enc_20_4D7790 (key);
	}

	KEY(0x34) ^= my_cos(KEY(0x40)) ? 0xf6951daa : KEY(0x04);
	KEY(0x48) |= KEY(0x44) & 0x6361A322;

	seed += my_sqrt(KEY(0x28));

	if(type == 0x01)
	{
		KEY(0x34) *= ROR(KEY(0x0C), 0xA5);
		enc_20_4AB3B0 (key, 0xAC5ABC0C);
		TRY(enc_20_major_23 (key, KEY(0x3C)));
	}

	RETURN;
}


THROWS_RET enc_20_major_25 (unsigned char *key, unsigned int seed)
{
	unsigned int type = (KEY(0x1C) ^ KEY(0x08) ^ seed) % 0x05;

	KEY(0x08) -= 0x31b8a51 & seed;

	if(type == 0x03)
	{
		KEY(0x08) ^= my_sin(KEY(0x34)) ? 0x0FD08092 : KEY(0x28);
		enc_20_4EB2A0 (key, 0x6EAF97B0);
		enc_20_4EB310 (key);
	}

	KEY(0x04) &= ROR(seed, 0xBD);

	if(my_cos(KEY(0x04)))
		KEY(0x30) = ROL(KEY(0x30), 0x1b);
	else
		KEY(0x30) = ROL(KEY(0x30), (unsigned char)KEY(0x14));

	if(type == 0x02)
	{
		TRY(enc_20_4D7900 (key));
		KEY(0x08) ^= my_sin(KEY(0x34)) ? 0x0FD08092 : KEY(0x28);
		enc_20_4D7790 (key);
	}
	
	seed = ROR(seed, (unsigned char)my_sqrt(seed));
	KEY(0x44) += (KEY(0x4C) * 61) << 1;

	if(type == 0x00)
	{
		KEY(0x28) += 0x08958821;
		KEY(0x48) *= KEY(0x28) + 0x466E09CF;
		TRY(enc_20_major_23 (key, KEY(0x28)));
	}

	if(my_cos(KEY(0x18)))
		KEY(0x48) = ROR(KEY(0x48), 0x11);
	else
		KEY(0x48) = ROR(KEY(0x48), (unsigned char)KEY(0x04));

	seed ^= 0xc63d7671 * seed;

	if(type == 0x04)
	{
		TRY(enc_20_4DA4D0 (key));
		enc_20_4D7660 (key, 0x3006CEAA);
		TRY(enc_20_major_24 (key, seed));
	}

	KEY(0x28) -= my_sqrt(seed);
	KEY(0x2C) &= (seed << 6) - seed;

	if(type == 0x00)
	{
		KEY(0x48) *= KEY(0x28) + 0x466E09CF;
		enc_20_4AB3B0 (key, 0x69F5B967);
		TRY(enc_20_major_19 (key, KEY(0x44)));
	}

	KEY(0x04) = ROL(KEY(0x04), (unsigned char)(KEY(0x3C) + 0xbbf8f6f9));

	RETURN;
}


THROWS_RET enc_20_mix (unsigned char *key, unsigned int seed)
{
	unsigned int type = (KEY(0x14) ^ KEY(0x24) ^ KEY(0x4C)) & 0x0F;

	switch (type) {
	case 0x0:
		KEY(0x08) ^= my_sin(KEY(0x34)) ? 0x0FD08092 : KEY(0x28);
		TRY(enc_20_4DA120 (key));
		TRY(enc_20_major_5 (key, 0x45835EB3));
		break;
	case 0x5:
		TRY(enc_20_4EEC50 (key));
		enc_20_4C4A60 (key, 0x3B1AD72C);
		TRY(enc_20_major_6 (key, KEY(0x24)));
		break;
	case 0x8:
		enc_20_4D6FB0 (key);
		KEY(0x18) += KEY(0x4C) - 0x3F5675D6;
		TRY(enc_20_major_9 (key, KEY(0x28)));
		break;
	case 0x9:
		TRY(enc_20_4DA4D0 (key));
		TRY(enc_20_4E7C50 (key));
		TRY(enc_20_major_14 (key, seed));
		break;
	case 0x0A:
		TRY(enc_20_4DA120 (key));
		enc_20_4D5720 (key, 0x0E8D8F62);
		TRY(enc_20_major_2 (key, KEY(0x00)));
		break;
	}

	KEY(0x08) ^= KEY(0x18) + 0x1847DE17;

	switch (type) {
	case 0x1:
		KEY(0x24) ^= 0x1df05ea2;
		KEY(0x4C) += 0x12B9E29D - KEY(0x30);
		TRY(enc_20_major_12 (key, 0x45835EB3));
		break;
	case 0x0A:
		enc_20_4D6FB0 (key);
		KEY(0x30) *= KEY(0x30);
		TRY(enc_20_major_11 (key, KEY(0x18)));
		break;
	}

	KEY(0x4C) += (KEY(0x30) * 13) << 3;

	switch (type) {
	case 0x2:
		enc_20_4D6FB0 (key);
		KEY(0x30) ^= KEY(0x3C) - 0x0F5CFDE0;
		TRY(enc_20_major_13 (key, KEY(0x4C)));
		KEY(0x18) += KEY(0x4C) - 0x3F5675D6;
		enc_20_4C4A60 (key, 0x678012FE);
		TRY(enc_20_major_22 (key, KEY(0x00)));
		break;
	case 0x6:
		TRY(enc_20_4AE420 (key, 0x706A6BC));
		TRY(enc_20_4BAC10 (key, 0x82B598A1));
		TRY(enc_20_major_8 (key, 0x45835EB3));
		break;
	}

	KEY(0x1C) -= KEY(0x38) & 0x1ADA7FA;

	switch (type) {
	case 0x4:
		enc_20_4E7FB0 (key);
		KEY(0x30) += KEY(0x18) + 0x21D7BF61;
		TRY(enc_20_major_10 (key, KEY(0x40)));
		break;
	case 0x6:
		TRY(enc_20_4DA4D0 (key));
		TRY(enc_20_4BAC10 (key, 0x0D2950F8C));
		TRY(enc_20_major_7 (key, 0x45835EB3));
		break;
	}

	seed = (KEY(0x00) + 0xd092d1bb) & 0x45835EB3;

	switch (type) {
	case 0x3:
		TRY(enc_20_4DA120 (key));
		KEY(0x1C) &= KEY(0x34) ^ 0x21AAF758;
		TRY(enc_20_major_9 (key, KEY(0x44)));
		break;
	case 0x4:
		KEY(0x34) -= KEY(0x04);
		KEY(0x48) *= KEY(0x28) + 0x466E09CF;
		TRY(enc_20_major_14 (key, KEY(0x14)));
		break;
	case 0x7:
		TRY(enc_20_49AA90 (key, 0x6D32760));
		KEY(0x24) ^= ((KEY(0x1C) << 4) + KEY(0x1C)) << 2;
		TRY(enc_20_major_6 (key, KEY(0x20)));
		break;
	}

	KEY(0x20) = ROL(KEY(0x20), (unsigned char)(KEY(0x0C) ^ 0x4437EB26));

	switch (type) {
	case 0x0:
		KEY(0x1C) &= KEY(0x34) ^ 0x21AAF758;
		KEY(0x38) |= KEY(0x0C) ^ 0x04345732;
		TRY(enc_20_major_12 (key, KEY(0x24)));
		break;
	case 0x5:
		enc_20_4F2AF0 (key);
		enc_20_4D7000 (key);
		TRY(enc_20_major_2 (key, KEY(0x4)));
		break;
	case 0x9:
		KEY(0x34) -= KEY(0x04);
		TRY(enc_20_4DA4D0 (key));
		TRY(enc_20_major_5 (key, KEY(0x8)));
		break;
	}

	KEY(0x24) *= KEY(0x38) | 0xbbf1fbef;

	switch (type) {
	case 0x0F:
		KEY(0x30) += KEY(0x18) ^ 0x211F5E40;
		KEY(0x14) += KEY(0x00) ^ 0x3E17ADD3;
		TRY(enc_20_major_11 (key, KEY(0x20)));
		break;
	case 0x0C:
		KEY(0x4C) ^= KEY(0x3C) ^ 0x03574ED3;
		enc_20_4D7080 (key);
		TRY(enc_20_major_13 (key, KEY(0x40)));
		break;
	}

	seed *= my_sqrt(KEY(0x04));

	switch (type) {
	case 0x0B:
		enc_20_4EB2C0 (key, 0x678AAE2C);
		enc_20_4F2AF0 (key);
		TRY(enc_20_major_22 (key, KEY(0x2C)));
		break;
	case 0x0E:
		KEY(0x34) *= ROR(KEY(0x0C), 0xA5);
		enc_20_49CED0 (key);
		TRY(enc_20_major_8 (key, KEY(0x48)));
		break;
	case 0x7:
		enc_20_49B260 (key, 0x0CDDA283E);
		enc_20_4AE3C0 (key, 0x3A13B215);
		TRY(enc_20_major_10 (key, KEY(0x00)));
		break;
	}

	KEY(0x0C) -= KEY(0x1C) ^ 0x4E46F05D;

	switch (type) {
	case 0x3:
		KEY(0x1C) ^= 0x0EDA01E71;
		KEY(0x34) -= KEY(0x04);
		TRY(enc_20_major_7 (key, KEY(0x2C)));
		break;
	case 0x8:
		KEY(0x08) ^= my_sin(KEY(0x34)) ? 0x0FD08092 : KEY(0x28);
		KEY(0x24) += ROL(KEY(0x10), 0xE9);
		TRY(enc_20_major_14 (key, KEY(0x8)));
		break;
	}

	KEY(0x4C) ^= 0xb1bdd560 ^ seed;

	switch (type) {
	case 0x1:
		KEY(0x30) ^= KEY(0x3C) - 0x0F5CFDE0;
		KEY(0x30) *= KEY(0x30);
		TRY(enc_20_major_9 (key, KEY(0x14)));
		break;
	case 0x0D:
		KEY(0x48) *= KEY(0x28) + 0x466E09CF;
		KEY(0x08) ^= KEY(0x3C) << 5;
		TRY(enc_20_major_6 (key, KEY(0x3C)));
		break;
	}

	KEY(0x18) ^= my_sqrt(KEY(0x14));

	return 0;
}
