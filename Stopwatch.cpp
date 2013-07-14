/***************************************************************************
 *   Copyright (C) 2005 by Steven Bartnick <Genohunter@gmx.net>            *
 *                                                                         *
 * This file is part of the Benchmark.                                     *
 *                                                                         *
 * Benchmark is free software; you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as          *
 * published by the Free Software Foundation; either version 2 of the      *
 * License, or (at your option) any later version.                         *
 *                                                                         *
 * Benchmark is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with this software; if not, write to the Free Software            *
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.               *
 ***************************************************************************/

#include "Stopwatch.hpp"
using Geno::Stopwatch;
#include "Main.hpp"

#if IsWin
    LONGLONG Stopwatch::m_Freq = 0;
    double Stopwatch::m_Multi = 0;
    
#endif

Stopwatch::Stopwatch()
: m_Running( false ), m_Time( 0 )
{
#if IsWin
    if( !m_Freq )
    {
    	QueryPerformanceFrequency( reinterpret_cast< LARGE_INTEGER* > ( &m_Freq )   );
    	m_Multi = 1.0/m_Freq;
    }
#endif
}


void Stopwatch::Start()
{
    m_Time = Get_Time();

    m_Running = true;
}



bool Stopwatch::Stop( double* ToReturn )
{
    if( !m_Running )
        return false;
    
    m_Time =  Get_Time() - m_Time;


    if( ToReturn )
        *ToReturn  =  m_Time;

    m_Running = false;
    return true;  
}


bool Stopwatch::Look( double& ToReturn ) const
{
    if( !m_Running  &&  !m_Time  ) // well, we would need something to check, whether or not the watch runs sometimes befor. I think the check with m_Time is enough at the moment. Cause there aren't computers that can start/stop in under a micro sec!
    return false;
    
    ToReturn  =  Get_Time() - m_Time;
     
    return true;
}

