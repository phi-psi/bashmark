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

#ifndef _Write_Result_hpp_
#define _Write_Result_hpp_

#include <fstream>
#include <inttypes.h>
#include "Benchmark.hpp"


namespace Geno
{
	
class Write_Result
{
public:
    Write_Result( const char*const );

    bool Write( const char*const );
    bool Write( const uint32_t );
    bool Write_End();

    static void Crypt( std::string& );
    //static const std::string* Checksum( const std::string& );
	    
    bool Is_Ok() {return m_File.is_open() & m_File.good();};
    
private:
    std::ofstream m_File;
    std::string m_Whole_File;
};




}

#endif
