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


#ifndef _Output_hpp_
#define _Output_hpp_



#include <iostream>

namespace Geno
{

//Later it could also output to a display, or compatible to special languages!
class Output
{
public:
    template< typename uTyp >
    inline static void Write( const uTyp&, const bool =true );
private:
};

//In the moment there is just console support
template< typename uTyp >
void Output::Write( const uTyp& ToWrite, const bool ShouldEnd )
{
    std::cout<<ToWrite;
    if( ShouldEnd )
        std::cout<<std::endl;
}


}

#endif
