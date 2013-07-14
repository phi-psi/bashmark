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
 

#ifndef _Stopwatch_hpp_
#define _Stopwatch_hpp_


#include "Main.hpp"

#if IsWin
   #include <windows.h>
   #include <mmsystem.h> //need also winmm.lib
#endif


namespace Geno
{

	
//The bool returns are for error returning. It returns false on a problem. ( Try to stop when a watch isn't started, or wanna look at it, when it isn't started )
//Currently we have no checks for none hardware clock pcs!
class Stopwatch
{
public:
    Stopwatch();

    void Start();
    bool Stop( double*const ); //Will Stop the watch and save the elapsed time inside the first Parameter,when it's not NULL!
    bool Look( double& ) const; //Take a look at the watch, but it don't stop it!

    static inline double Get_Time(); //Will first just work for linux properly
private:

    bool m_Running;
    double m_Time;
    
#if defined(Win32) || defined(__Win32__) || defined(_WIN32)
    static LONGLONG m_Freq; //The frequence of the hardware clock
    static double m_Multi; //Our time multiplicator!
#endif
};




#if IsLin
    #include <sys/time.h> //I don't like to include it here.... but else....
#endif



#ifndef NULL
    #define NULL static_cast< void* > ( 0 )
#endif


double Stopwatch::Get_Time()
{
#if IsLin
    timeval tmp;
    gettimeofday( &tmp, 0/*static_cast< timezone* > ( NULL )*/  ); //some does not like the cast ;) 
    
    return tmp.tv_sec  +  (tmp.tv_usec * 0.000001 ); //ie /1000000.0
    
    
#elif IsWin
    LONGLONG Time;
    QueryPerformanceCounter( reinterpret_cast< LARGE_INTEGER* > ( &Time )  );
    
    return m_Multi*Time;
#endif
}





}

#endif

