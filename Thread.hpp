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

#ifndef _THREAD_HPP_
#define _THREAD_HPP_


#include "Main.hpp"


// There aren't checks whether or not a thread is successfull created, detached or joinable and so on

#if IsLin
    #include <pthread.h>
    
    #define ThreadHandle pthread_t 
    #define ThreadMutex pthread_mutex_t
    #define ThreadFunction( Name, Parameter_Name ) void* Name( void* Parameter_Name )
    #define ThreadExit( Value ) pthread_exit( Value )

    template< typename uType >
    inline int ThreadCreate( ThreadHandle& ID, void* (*Func) (void*), const uType Parameter )
    {
	return pthread_create( &ID, NULL, Func, reinterpret_cast< void* > ( Parameter ) );
    }
    
    
    inline void ThreadWaitFor( ThreadHandle* IDs, const unsigned Thread_Number )
    {
	for( unsigned i=0; i < Thread_Number; ++i )
	{
	    pthread_join( IDs[i], NULL );
	}
    }

    inline void ThreadDetach( ThreadHandle& ID)
    {
	pthread_detach( ID );
    }

#elif IsWin
    #include <windows.h>
    static DWORD ValueIgnore; //We need under some older win versions a point to save something useless...so we use this variable for that
    
    #define ThreadHandle HANDLE
    #define ThreadMutex CRITICAL_SECTION
    #define ThreadFunction( Name, Parameter_Name ) DWORD WINAPI Name( void* Parameter_Name )
    #define ThreadExit( Value ) return (DWORD)Value
    
    template< typename uType >
    inline HANDLE ThreadCreate( ThreadHandle ID, DWORD WINAPI (*Func) (void*), uType Parameter )
    {
	return ID = CreateThread( 0, 0, Func, reinterpret_cast< void* > ( Parameter ), 0, &ValueIgnore ), ID ;
    }

    //what about the return?
    inline void ThreadWaitFor( ThreadHandle*const IDs, const unsigned Thread_Number )
    {
	WaitForMultipleObjects( Thread_Number, IDs, FALSE, INFINITE ); //INFINITE is not nice...
    }
    
    inline void ThreadDetach( ThreadHandle& ID)
    {
	//Not needed
    }

#endif



#endif

