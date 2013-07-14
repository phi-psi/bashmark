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

#ifndef _MD5_H_
#define _MD5_H_

#include <inttypes.h>

/* A not threadsafe version for little endian systems, without the implemenation for really long messages (over 2 ^ 64 bit in length ) and just for bytewise stuff. */ 

/* Use the 'FixedL' version if you know the string lenght (without terminating 0) else use the define ( hope than it was a null terminated string!) */

#define MD5Digest( Message ) MD5Digest_FixedL( Message, strlen( Message ) )
const uint32_t* MD5Digest_FixedL( const uint8_t*const, const uint32_t );
const uint8_t* MD5toHexString( const uint32_t* );
const uint32_t* HexStringtoMD5( const uint8_t* );


void SetCase( const uint8_t ); /*Sets the current case for hex notation. (Default: lowercase )
				 0 for uppercase and positive values for lowercase.
				 It will just affect the following hex constructions. */
				 
#endif

