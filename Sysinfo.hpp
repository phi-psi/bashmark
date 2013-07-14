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

#ifndef _Sysinfo_hpp_
#define _Sysinfo_hpp_


#include "Cpu_Info.hpp"
#include "Main.hpp"

#include <string>
#include <vector> //overkill? :x

namespace Geno
{

//Immo we can just read out the proc files!
class Sysinfo
{
public:
    inline static Sysinfo* Get_Instance();

    typedef std::vector< Cpu_Info* > Cpu_Storage;
    typedef const std::vector< Cpu_Info* >& Re_Cpu_Storage;


    inline Re_Cpu_Storage Get_Cpus() const;
    inline const char*const Get_Os() const;
    inline const char*const Get_Compiler() const;
    inline const char*const Get_Binary_Size() const;

private:
    Sysinfo();
    ~Sysinfo();

    void Gather_Infos();
    void Gather_Cpu_Info();
    void Gather_Os_Info();
    void Gather_Compiler_Info();
    void Gather_Binary_Size();

#if IsLin
    void Process_Cpuinfo_Line( const std::string& );
    
    bool Proc_Version();
    bool Proc_Cpuinfo();

    bool Uname_Version();
#elif IsWin
    bool Get_Registry_Cpu_Info();
#endif
    void Cleanup_Cpus();
    static void Filter_Cpu_Type( std::string*const );

    
    Cpu_Storage m_Cpus;
    std::string m_Os;
    std::string m_Compiler;
    std::string m_Binary_Size;
};


Sysinfo* Sysinfo::Get_Instance()
{
    static Sysinfo Instance;
    return &Instance;
}


Sysinfo::Re_Cpu_Storage Sysinfo::Get_Cpus() const
{
    return m_Cpus;
}
const char*const Sysinfo::Get_Os() const
{
    return m_Os.c_str();
}
const char*const Sysinfo::Get_Compiler() const
{
    return m_Compiler.c_str();
}
const char*const Sysinfo::Get_Binary_Size()const
{
    return m_Binary_Size.c_str();
}


}

#endif
