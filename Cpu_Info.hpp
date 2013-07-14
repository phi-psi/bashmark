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
 

#ifndef _Cpu_Info_hpp_
#define _Cpu_Info_hpp_


#include <string>


namespace Geno
{



class Cpu_Info
{
public:
    Cpu_Info() : m_Number(1) {};
    
    inline void Set_Type( const char* const );
    inline void Set_MHz( const char*const );
    inline void Set_Cache( const char*const );
    inline void Add( const unsigned short );
    
    inline const char* Get_Type() const;
    inline const char* Get_MHz() const;
    inline const char* Get_Cache() const;
    inline unsigned short Get_Number() const;
    
    inline bool Is_Valide() const; //Whether or not all infos a here
    
    inline bool operator == ( const Cpu_Info& ) const;
private:
    std::string m_Type;
    std::string m_MHz;
    std::string m_Cache;
    
    unsigned short m_Number;
};


void Cpu_Info::Set_Type( const char*const Type)
{
    m_Type = Type;
}
void Cpu_Info::Set_MHz( const char*const MHz )
{
    m_MHz = MHz;
}
void Cpu_Info::Set_Cache( const char*const Cache )
{
    m_Cache = Cache;
}
void Cpu_Info::Add( const unsigned short Number)
{
    m_Number += Number;
}

const char* Cpu_Info::Get_Type() const
{
    return m_Type.c_str();
}
const char* Cpu_Info::Get_MHz() const
{
    return m_MHz.c_str();
}
const char* Cpu_Info::Get_Cache() const
{
    return m_Cache.c_str();
}
unsigned short Cpu_Info::Get_Number() const
{
    return m_Number;
}
bool Cpu_Info::Is_Valide() const
{
    if( !m_MHz.empty()  ||  !m_Type.empty()  || !m_Cache.empty() )
        return true;
    else
        return false;
}

bool Cpu_Info::operator == ( const Cpu_Info& other ) const
{
    if( m_Type == other.m_Type   &&   m_MHz  == other.m_MHz  && m_Cache == other.m_Cache )
        return true;
    else
        return false;
}



}

#endif
