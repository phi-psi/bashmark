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

#include <sstream>
#include "Write_Result.hpp"
#include "md5.h"
using Geno::Write_Result;
using Geno::Benchmark;
using std::string;
using std::stringstream;

Write_Result::Write_Result( const char*const File_Name )
{
    if( File_Name  )
    {
	m_File.open( File_Name );
    }
}

bool Write_Result::Write( const char*const ToWrite )
{
    if( m_File.is_open()  &&  m_File.good() )
    {
	string ToCrypt( ToWrite );
	m_Whole_File += ToWrite;
	m_Whole_File += '\n';
		
	Crypt( ToCrypt );
	m_File << ToCrypt <<'\n';
	return true;
    }
    return false;	
}
bool Write_Result::Write_End()
{
    if( m_File.is_open()  &&  m_File.good() )
    {
	string ToCrypt = (const char*)MD5toHexString(MD5Digest_FixedL( (const uint8_t*)m_Whole_File.c_str(), m_Whole_File.size() ));
	Crypt( ToCrypt );
	m_File<<ToCrypt<<'\n';
	return true;
    }
    return false;
}
//ugly 
bool Write_Result::Write( uint32_t ToWrite )
{
    if( m_File.is_open()  &&  m_File.good()  )
    {
	stringstream Converter;
	Converter<<ToWrite;
	string ToCrypt = Converter.str();
	m_Whole_File += ToCrypt;
	m_Whole_File += '\n';

	Crypt( ToCrypt );
	m_File << ToCrypt << '\n';
	return true;
    }
    return false;
}

void Write_Result::Crypt( string& ToCrypt )
{/* Currently disabled!
    for( string::size_type i =0; i < ToCrypt.size(); ++i )
	ToCrypt[i] ^= i+11; //really simple cryption eh? ;) But enough for our needs :)
*/
}

