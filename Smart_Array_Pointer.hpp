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

#ifndef _Smart_Array_Pointer_hpp_
#define _Smart_Array_Pointer_hpp_


namespace Geno
{

//Todo: We need the [] operator

//A Little class to prevent for mem leaks and catches out of memory exceptions!(Later we will split it up, so it don't catches the exceptions everytimes!
template< typename Type >
class Smart_Array_Pointer
{
public:
    inline Smart_Array_Pointer( const uint32_t);
    ~Smart_Array_Pointer() {if( m_Mem )delete[] m_Mem;};


    bool Is_Allocated() const {return m_Allocated;};
    bool operator !  () const {return !m_Allocated;};
    operator bool    () const {return m_Allocated;};

    Type* Get_Mem() const {return m_Mem;};
    Type* operator -> () const {return m_Mem;};
    operator Type*    () const {return m_Mem;};

    
private: 
    Type* m_Mem;
    bool m_Allocated;

    //Just to be save :)
    void* operator new   ( size_t );
    void* operator new[] ( size_t );
    void  operator delete   ( void* );
    void  operator delete[] ( void* );
    Smart_Array_Pointer();
    const Smart_Array_Pointer& operator = (const Smart_Array_Pointer&);
    Smart_Array_Pointer( const Smart_Array_Pointer& );
};


template< typename Type >
Smart_Array_Pointer< Type > ::Smart_Array_Pointer( const uint32_t Number )
: m_Mem( NULL ), m_Allocated( true )
{
    try{ m_Mem = new Type[ Number ]; }
    catch( const std::bad_alloc& ){ m_Allocated = false; };
}


}

#endif

