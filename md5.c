/***************************************************************************
 *   Copyright (C) 2005 by Steven Bartnick <Genohunter@gmx.net>            *
 *                                                                         *
 * This file is part of Bashmark.                                          *
 *                                                                         *
 * Bashmark is free software; you can redistribute it and/or modify        *
 * it under the terms of the GNU Lesser General Public License as          *
 * published by the Free Software Foundation; either version 2 of the      *
 * License, or (at your option) any later version.                         *
 *                                                                         *
 * Bashmark is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with this software; if not, write to the Free Software            *
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.               *
 ***************************************************************************/

/* For the strlen */
#if defined(_cplusplus) || defined(_CPLUSPLUS)
    #include <string>
    using std::string;
#else
    #include <string.h> 
#endif

#include "md5.h"


/* Many magic numbers for MD5 are hidden here */
static const uint32_t SinTable[] = { 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 
  0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x2441453, 0xd8a1e681, 0xe7d3fbc8, 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x4881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };

static const uint8_t ShiftTable[] = { 7, 12, 17, 22, 5, 9 ,14, 20, 4, 11, 16, 23, 6, 10, 15, 21 };


/* Is 'a' or 'A' dependend whether you want upper or lower case hex representation */
static uint8_t UsedCase='a';

/* The 4 calculation functions used by MD5 */
#define F1( x, y, z) ( (x & y)  | ((~x) & z) )
#define F2( x, y, z) F1(z, x, y)
#define F3( x, y, z) (x ^ y ^ z)
#define F4( x, y, z) (y ^ (x | (~z)))

/* The main calculation used by MD5 */
#define MD5CoreOperation( Function, A, B, C, D, Data, SinPart, ShiftPart ) \
	(A += Function( B, C, D )+Data+SinTable[ SinPart ], A = A <<ShiftTable[ ShiftPart ] | A>>(32-ShiftTable[ ShiftPart ]) , A+= B)


/* Calculations how many additional bits we need, with help of the length of the string in bits */
/* We want 512bit blocks, which will be padded till the 448th. bit. If it's allready at the 448 bit, we fill it up to the next 448th. bit */
uint16_t MD5CalculatePadding( uint64_t Len )
{
    uint16_t ToReturn = Len % 512;
    if( ToReturn < 448 )
	ToReturn = 448 - ToReturn;
    else
	ToReturn = 512 - (ToReturn - 448);
    
    return ToReturn;
}

const uint8_t* MD5toHexString( const uint32_t*const Digest )
{
    static uint8_t ToReturn[33];
    uint8_t* Current= (uint8_t*)Digest;
    uint8_t Temp =0;
    uint8_t i=0;
    
    for( ; i < (4<<2) ; ++i, ++Current )
    {
	Temp = ((*Current >>4) & 15);
	if( Temp < 10 )
	    ToReturn[i<<1] = '0' + Temp;
	else
	    ToReturn[i<<1] = UsedCase + (Temp-10);
	
	Temp = *Current & 0xF;
	if( Temp < 10 )
	    ToReturn[(i<<1)+1] = '0' + Temp;
	else
	    ToReturn[(i<<1)+1] = UsedCase + (Temp-10);
    }

    ToReturn[32] = 0;

    return ToReturn;
}

/* If Message is not null terminated, you will get thrown out of the universe! */
const uint32_t* MD5Digest_FixedL( const uint8_t*const Message, const uint32_t Len )
{
    if( !Message )
	return NULL;
    const uint64_t Len_Bit = Len<<3;
    uint64_t i=0;
    if( !Len )
	return NULL;

    uint16_t Padding = MD5CalculatePadding( Len<<3 )>>3;
    
	    
    /* The MD5 magic base numbers */
    uint32_t A = 0x67452301,
	     B = 0xefcdab89,
	     C = 0x98badcfe,
	     D = 0x10325476;
    
    
    uint32_t* Data=0;
    uint8_t* Data_Byte = 0;
    static uint32_t* Buffer[64>>2];
    
    for( ; i < Len+Padding+8; i+=(16<<2) )  /* i stand for byte, even we are working with 4 byte vars! */
    {
	uint32_t TempA = A,
		 TempB = B,
		 TempC = C,
		 TempD = D;
	
	if( i+(16<<2) < Len ) /* Then everything comes from message */
	{
	    Data = (uint32_t*)(Message+i);
	}
	else if( i <= Len ) /* Mix from message and additional is needed */
	{
	    Data = (uint32_t*)Buffer; //oO
	    memcpy( Data, Message+i, Len -i );
	    Data_Byte = (uint8_t*)Data;
	    Data_Byte[Len-i] = 128; //To set the leading bit

	    memset( Data_Byte+Len-i+1, 0, Padding-1 ); //Padding-1 isn't correct, is it?
	    *(uint64_t*)(Data_Byte+Len-i+Padding) = Len_Bit;
	}
	else //just the additional is needed 
	{
	     Data = (uint32_t*)Buffer; //oO
	     Data_Byte = (uint8_t*)Data;

	     memset( Data_Byte, 0, 56 ); //56 is the block size without the len info
	     *((uint64_t*)(Data_Byte+56)) = Len_Bit;
	}


	MD5CoreOperation( F1, TempA, TempB, TempC, TempD, Data[0], 0, 0 );
	MD5CoreOperation( F1, TempD, TempA, TempB, TempC, Data[1], 1, 1 );
	MD5CoreOperation( F1, TempC, TempD, TempA, TempB, Data[2], 2, 2 );
	MD5CoreOperation( F1, TempB, TempC, TempD, TempA, Data[3], 3, 3 );

	MD5CoreOperation( F1, TempA, TempB, TempC, TempD, Data[4], 4, 0 );
	MD5CoreOperation( F1, TempD, TempA, TempB, TempC, Data[5], 5, 1 );
	MD5CoreOperation( F1, TempC, TempD, TempA, TempB, Data[6], 6, 2 );
	MD5CoreOperation( F1, TempB, TempC, TempD, TempA, Data[7], 7, 3 );

	MD5CoreOperation( F1, TempA, TempB, TempC, TempD, Data[8], 8, 0 );
	MD5CoreOperation( F1, TempD, TempA, TempB, TempC, Data[9], 9, 1 );
	MD5CoreOperation( F1, TempC, TempD, TempA, TempB, Data[10], 10, 2 );
	MD5CoreOperation( F1, TempB, TempC, TempD, TempA, Data[11], 11, 3 );

	MD5CoreOperation( F1, TempA, TempB, TempC, TempD, Data[12], 12, 0 );
	MD5CoreOperation( F1, TempD, TempA, TempB, TempC, Data[13], 13, 1 );
	MD5CoreOperation( F1, TempC, TempD, TempA, TempB, Data[14], 14, 2 );
	MD5CoreOperation( F1, TempB, TempC, TempD, TempA, Data[15], 15, 3 );



	
	
	MD5CoreOperation( F2, TempA, TempB, TempC, TempD, Data[1], 16, 4 );
	MD5CoreOperation( F2, TempD, TempA, TempB, TempC, Data[6], 17, 5 );
	MD5CoreOperation( F2, TempC, TempD, TempA, TempB, Data[11], 18, 6 );
	MD5CoreOperation( F2, TempB, TempC, TempD, TempA, Data[0], 19, 7 );

	MD5CoreOperation( F2, TempA, TempB, TempC, TempD, Data[5], 20, 4 );
	MD5CoreOperation( F2, TempD, TempA, TempB, TempC, Data[10], 21, 5 );
	MD5CoreOperation( F2, TempC, TempD, TempA, TempB, Data[15], 22, 6 );
	MD5CoreOperation( F2, TempB, TempC, TempD, TempA, Data[4], 23, 7 );

	MD5CoreOperation( F2, TempA, TempB, TempC, TempD, Data[9], 24, 4 );
	MD5CoreOperation( F2, TempD, TempA, TempB, TempC, Data[14], 25, 5 );
	MD5CoreOperation( F2, TempC, TempD, TempA, TempB, Data[3], 26, 6 );
	MD5CoreOperation( F2, TempB, TempC, TempD, TempA, Data[8], 27, 7 );

	MD5CoreOperation( F2, TempA, TempB, TempC, TempD, Data[13], 28, 4 );
	MD5CoreOperation( F2, TempD, TempA, TempB, TempC, Data[2], 29, 5 );
	MD5CoreOperation( F2, TempC, TempD, TempA, TempB, Data[7], 30, 6 );
	MD5CoreOperation( F2, TempB, TempC, TempD, TempA, Data[12], 31, 7 );




	MD5CoreOperation( F3, TempA, TempB, TempC, TempD, Data[5], 32, 8 );
	MD5CoreOperation( F3, TempD, TempA, TempB, TempC, Data[8], 33, 9 );
	MD5CoreOperation( F3, TempC, TempD, TempA, TempB, Data[11], 34, 10 );
	MD5CoreOperation( F3, TempB, TempC, TempD, TempA, Data[14], 35, 11 );

	MD5CoreOperation( F3, TempA, TempB, TempC, TempD, Data[1], 36, 8 );
	MD5CoreOperation( F3, TempD, TempA, TempB, TempC, Data[4], 37, 9 );
	MD5CoreOperation( F3, TempC, TempD, TempA, TempB, Data[7], 38, 10 );
	MD5CoreOperation( F3, TempB, TempC, TempD, TempA, Data[10], 39, 11 );

	MD5CoreOperation( F3, TempA, TempB, TempC, TempD, Data[13], 40, 8 );
	MD5CoreOperation( F3, TempD, TempA, TempB, TempC, Data[0], 41, 9 );
	MD5CoreOperation( F3, TempC, TempD, TempA, TempB, Data[3], 42, 10 );
	MD5CoreOperation( F3, TempB, TempC, TempD, TempA, Data[6], 43, 11 );

	MD5CoreOperation( F3, TempA, TempB, TempC, TempD, Data[9], 44, 8 );
	MD5CoreOperation( F3, TempD, TempA, TempB, TempC, Data[12], 45, 9 );
	MD5CoreOperation( F3, TempC, TempD, TempA, TempB, Data[15], 46, 10 );
	MD5CoreOperation( F3, TempB, TempC, TempD, TempA, Data[2], 47, 11 );


	

	MD5CoreOperation( F4, TempA, TempB, TempC, TempD, Data[0], 48, 12 );
	MD5CoreOperation( F4, TempD, TempA, TempB, TempC, Data[7], 49, 13 );
	MD5CoreOperation( F4, TempC, TempD, TempA, TempB, Data[14], 50, 14 );
	MD5CoreOperation( F4, TempB, TempC, TempD, TempA, Data[5], 51, 15 );

	MD5CoreOperation( F4, TempA, TempB, TempC, TempD, Data[12], 52, 12 );
	MD5CoreOperation( F4, TempD, TempA, TempB, TempC, Data[3], 53, 13 );
	MD5CoreOperation( F4, TempC, TempD, TempA, TempB, Data[10], 54, 14 );
	MD5CoreOperation( F4, TempB, TempC, TempD, TempA, Data[1], 55, 15 );

	MD5CoreOperation( F4, TempA, TempB, TempC, TempD, Data[8], 56, 12 );
	MD5CoreOperation( F4, TempD, TempA, TempB, TempC, Data[15], 57, 13 );
	MD5CoreOperation( F4, TempC, TempD, TempA, TempB, Data[6], 58, 14 );
	MD5CoreOperation( F4, TempB, TempC, TempD, TempA, Data[13], 59, 15 );

	MD5CoreOperation( F4, TempA, TempB, TempC, TempD, Data[4], 60, 12 );
	MD5CoreOperation( F4, TempD, TempA, TempB, TempC, Data[11], 61, 13 );
	MD5CoreOperation( F4, TempC, TempD, TempA, TempB, Data[2], 62, 14 );
	MD5CoreOperation( F4, TempB, TempC, TempD, TempA, Data[9], 63, 15 );	

	
	A += TempA;
	B += TempB;
	C += TempC;
	D += TempD;
    }

    static uint32_t Digest[5] = { 0 }; //Just for safety when used as string... so we get a null termination
    Digest[0] = A;
    Digest[1] = B;
    Digest[2] = C;
    Digest[3] = D;

    return Digest;
}

/* Not that nice, but works */
const uint32_t* HexStringtoMD5( const uint8_t* HexString )
{
    if( !HexString )
	return NULL;

    static uint32_t Digest[4];
    uint8_t* Byte = (uint8_t*)Digest;
    
    uint8_t i=0;
    
    for( ; i < 32; i+=2, ++Byte )
    {
	if( HexString[i] >= UsedCase )
	    *Byte = (HexString[i] - UsedCase +10)<<4;
	else
	    *Byte = (HexString[i] - '0')<<4;

	if( HexString[i+1] >= 'A' )
	    *Byte += HexString[i+1] - UsedCase +10;
	else
	    *Byte += HexString[i+1] -'0';
    }
    
    return Digest;
}

void SetCase( const uint8_t LowerCase )
{
    if( LowerCase )
	UsedCase = 'a';
    else
	UsedCase = 'A';
}

