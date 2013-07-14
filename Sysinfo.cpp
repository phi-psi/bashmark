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

#include <cstring> 
#include "Sysinfo.hpp"

using Geno::Sysinfo;
using Geno::Cpu_Info;

Sysinfo::Sysinfo()
{
    Gather_Infos();
}
Sysinfo::~Sysinfo()
{
    if( !m_Cpus.empty() )
    {
        for( Sysinfo::Cpu_Storage::iterator it = m_Cpus.begin(); 
            it != m_Cpus.end(); ++it )
            delete (*it);

	m_Cpus.clear();
    }
}

void Sysinfo::Gather_Infos()
{
    Gather_Cpu_Info();
    Gather_Os_Info();
    Gather_Compiler_Info();
    Gather_Binary_Size();
}
#if defined(__INTEL_COMPILER)
    #include <sstream>
#endif
void Sysinfo::Gather_Compiler_Info()
{
#if defined(__INTEL_COMPILER)
    m_Compiler = "ICC ";
    std::stringstream Version_Converter(__INTEL_COMPILER/100.0);
    m_Compiler += Version_Converter.str();
#elif defined(__GNUC__)
    m_Compiler = __VERSION__;
    std::string::size_type Pos = m_Compiler.find( '(' );
    if( Pos != std::string::npos )
	m_Compiler.erase( Pos );
    m_Compiler.insert( 0, "GCC " );
#else
    m_Compiler = "Unknown";
#endif
}


#if IsLin



#include <fstream>
using std::ifstream;
using std::string;

bool Sysinfo::Proc_Version()
{
    bool Got_It( false );
    ifstream File( "//proc//version" );
    if( File.is_open()  )
    {
    	string Buf;
    	if( getline( File, Buf )  )
    	{
    	    string::size_type Pos( Buf.find( "version")  );
    	    if( Pos != string::npos )
    		Buf.erase( Pos-1, 8 );
    		    
    	    Pos = Buf.find( '(' );
    	    if( Pos != string::npos )
    	    {
    		Buf.erase( Pos );
    		m_Os = Buf;
		Got_It = true;
    	    }
    	}
    }
    return Got_It;
}
#endif

#if IsWin
    #include "windows.h"
    using std::string;
#endif

void Sysinfo::Gather_Os_Info()
{
#if IsLin
    if( !Proc_Version()  &&  !Uname_Version() )
        m_Os = "Unknown Linux kompatible os";

#elif IsWin
    if( m_Os.empty() ) //unneeded
    {
	OSVERSIONINFOEX info;
	memset( &info, 0, sizeof( OSVERSIONINFOEX )  );
	info.dwOSVersionInfoSize = sizeof( OSVERSIONINFOEX );
	if( !GetVersionEx ( reinterpret_cast<OSVERSIONINFO*> (&info) )  )
	{
	    info.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
	    if( !GetVersionEx( reinterpret_cast<OSVERSIONINFO*> (&info) ) )
	    {
		m_Os = "Unkown Windows os";
		return;
	    }
	}
    
	switch( info.dwPlatformId )
	{
	    case VER_PLATFORM_WIN32_NT:
	    {
		if( info.dwMajorVersion == 5 )
		{
		    if( info.dwMinorVersion == 2 )
			m_Os = "Windows 2003";
		    else if( info.dwMinorVersion == 1 )
			m_Os = "Windows XP";
		    else if( !info.dwMinorVersion )
			m_Os = "Windows 2000";
		    else
		    {
			m_Os = "Unknown Windows NT based os";
			return;
		    }
		}
		else if( info.dwMajorVersion <= 4 )
		    m_Os = "Windows NT";
		else
		{
		    m_Os = "Unknown Windows NT based os";
		    return;
		}
		break;
	    }
	    case VER_PLATFORM_WIN32_WINDOWS:
	    {
		if( info.dwMajorVersion == 4 )
		{
		    if( !info.dwMinorVersion)
			m_Os = "Windows 95";
		    if( info.dwMinorVersion == 10 )
		    {
			m_Os = "Windows 98";
			if( info.szCSDVersion[1] == 'A' )
			    m_Os += "SE";
		    }
		    else if( info.dwMinorVersion == 90 )
			m_Os = "Windows ME";
		    else
		    {
			m_Os = "Unknown Windows32 based os";
			return;
		    }
		}
		else
		{
		    m_Os = "Unknown Windows32 based os";
		    return;
		}
		break;
	    }
	    default:
		m_Os = "Unknown Windows os";
		return;
	}
    }
#else
    m_Os = "Unknown os";
#endif
}


#if IsLin

#include <fstream>
using std::ifstream;
using std::string;

bool Sysinfo::Proc_Cpuinfo()
{
    ifstream Proc( "//proc//cpuinfo" );
    if( Proc.is_open() )
    {
    	string Buf;
    	while(   getline( Proc, Buf )    )
    	{
    	    Process_Cpuinfo_Line( Buf );
    	}
	return true;
    }
    else 
        return false;
}

#endif
void Sysinfo::Cleanup_Cpus()
{
    for( Sysinfo::Cpu_Storage::iterator it( m_Cpus.begin()  )  ;
         it != m_Cpus.end(); ++it )
    {
    	if( !(*it)->Is_Valide() ) //If the cpuinfo is invalide!
    	{
	    delete (*it); //Then we just delete it 
    	    it = m_Cpus.erase( it ); //and throw it away!
    	}
    	else
    	{
    	    for( Sysinfo::Cpu_Storage::iterator it2( m_Cpus.begin()  )  ; 
    	         it2 != m_Cpus.end(); ++it2 ) //a inner loop to check on object with every other :)
    	    {
    	       	if( it2 == it) //We don't wanna check it with it self ;)
    	       	    continue;
    	        else if( ((*it)->operator==( *(*it2) ) )   ) //A little bit evil way to use the provided operator to check them
    	        {
    	            (*it)->Add( (*it2)->Get_Number() ); //We just add the number of cpus...
		    delete *it2; //delete the other...
		    m_Cpus.erase( it2 ); //throw it away...
    	            it = m_Cpus.begin()-1; //And know it will be tricky... we need to set the main iterator to the beginning -1. cause in the next loop begin it will be added by 1... so we point to the beginning :)
    	            break; //Also we need to jump out of that inner loop!
    	        }
    	    }
    	}
		
    }


}


#if IsWin
    #include <windows.h>
    #include <sstream>
    using std::string;
    using std::stringstream;

bool Sysinfo::Get_Registry_Cpu_Info()
{
    SYSTEM_INFO Sys_Info;
    GetSystemInfo( &Sys_Info );
    //Cpu_Info* New_Cpu = new Cpu_Info;
    //bool Error( false );
        
    static const char Base_Location[] = {"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\"};
    char Buffer[80];
    for( unsigned int i=0; true ; ++i )
    {
	HKEY hkey;
	DWORD Size;
	DWORD Type = REG_SZ;
	stringstream Location;
	Location << Base_Location;
	Location << i;
	Location << "\\";
	if( !RegOpenKeyEx( HKEY_LOCAL_MACHINE, Location.str().c_str(), 0, KEY_READ, &hkey) )
	{
	    Cpu_Info* New_Cpu = new Cpu_Info;
	    if( !RegQueryValueEx(hkey,"ProcessorNameString",NULL,&Type,(LPBYTE)&Buffer,&Size) )
	    {
		string Cpu_Type( Buffer );
		Filter_Cpu_Type( &Cpu_Type );
		
		New_Cpu->Set_Type( Cpu_Type.c_str() );
		Type = REG_DWORD;
		DWORD MHz(0);
		if( !RegQueryValueEx( hkey, "~MHz", NULL, &Type, (LPBYTE)&MHz, &Size)  )
		{
		    stringstream Converter;
		    Converter << MHz;
		    New_Cpu->Set_MHz( Converter.str().c_str() );
		}
		else	
		    New_Cpu->Set_MHz( "0" );
		 New_Cpu->Set_Cache( "0" );

		m_Cpus.push_back( New_Cpu );
		New_Cpu = NULL;
		RegCloseKey(hkey);
	    }
	    else
	    {
		delete New_Cpu;
		RegCloseKey(hkey);
		break;
	    }
	}
	else
	    break;
    }    
}
#endif

void Sysinfo::Gather_Cpu_Info()
{
#if IsWin
    if( Get_Registry_Cpu_Info() )
	Cleanup_Cpus();
#elif IsLin
    if( Proc_Cpuinfo() )
        Cleanup_Cpus();
#endif
}


using std::string;
void Sysinfo::Filter_Cpu_Type( string*const ToFilter )
{
    while( (*ToFilter)[0] == ' ' )
	ToFilter->erase( 0, 1); //Eliminate the leading space
    while( (*ToFilter)[(*ToFilter).size()-1] == ' ' )
	ToFilter->erase( (*ToFilter).size()-1 );
    string::size_type Pos;
    if( (Pos = ToFilter->find( "processor" ) ) != string::npos  ||  (Pos = ToFilter->find( "Processor" ) )!= string::npos ) //We don't wanna have the useless info in it, that it's a processor!
	ToFilter->erase( Pos, strlen("processor") ); 
    if( (Pos = ToFilter->find( "Hz" )  ) != string::npos ) //also we don't wanna know how much MHz/GHz it has! (not here ;> )
    {
	string::size_type Space_Pos = ToFilter->rfind( ' ', Pos -2 ); //Ok thats a bit tricky... we don't know how many numbers are before the MHz/GHz ... so we search for the next space, before the (G/M)Hz. The -2 is useful, cause we have the position of the H inside the MHz/GHz but wanna start 1 letter before it.
	if( Space_Pos != string::npos ) //should always be true
	{
	    if( Space_Pos == Pos-2 ) //Damn! Than it use a space between the MHz/GHz and the number!
	    {
		if( (Space_Pos = ToFilter->rfind( ' ', Space_Pos-1 )) ) //so we search further (also this should be true always
		    return; //we must break, else we would try to erase with that wrong position!
	    }
	    ToFilter->erase( Space_Pos, Pos+1-(Space_Pos-1) ); //Now we erase from the space till the end of MHz/GHz ! 
	}
	//We could break here if it's else, but it's easyer to correct such problems when we got a wrong than, then when we got no line :D
    }
    if( (Pos = ToFilter->find( "CPU" ) ) != string::npos  ||   (Pos = ToFilter->find( "Cpu" ) ) != string::npos  ||
	(Pos = ToFilter->find( "cpu" ) ) != string::npos  )
    {
	ToFilter->erase( Pos, strlen( "CPU" ) );
    }

    Pos = 0; //we need to Prepare it for it use!
    while( (Pos = ToFilter->find( "  ", Pos ) ) != string::npos ) //thats really expensive and fuck..but we really don't wanna have extra spaces!
	ToFilter->erase( Pos, 1 ); //Also we can just erase one space, even it there would be tausends ...
    
    while( (*ToFilter)[(*ToFilter).size()-1] == ' ' ) //Cause somewhere it got again inside the string, we delete them again
	ToFilter->erase( (*ToFilter).size()-1 );
}


#if IsLin
//brr....dirty
void Sysinfo::Process_Cpuinfo_Line( const string& Buf)
{
    switch( Buf[0] )
    {
        case 'p': //then it could be processor!
	    if( !Buf.find( "processor" ) )
	       m_Cpus.push_back( new Cpu_Info() );
	    break;
	case 'm': //then it could be model name!
	{
	    if( !Buf.find( "model name" ) ) //Ok, then we need some extra filtering!
	    {
		string::size_type Pos = Buf.find( ':' ); //That comes before the infos, so its the start
                if( Pos == string::npos || ++Pos >= Buf.size()  ) //should be always false!
		     break;
	        
		string Filter = Buf.substr( Pos+1, Buf.size() - Pos+1 );
		Filter_Cpu_Type( &Filter );

		//Puh... now we have done the Filtering!
	        Cpu_Info* cpu = m_Cpus.back(); //get the current cpu, so we can add the info :)
		if( cpu ) //should be always true!
		    cpu->Set_Type( Filter.c_str()  );
				
	    }
	    
	    break;
	}
	
        case 'c': //Could that be cpu MHz or cache size?? ;D
	{
	    if( !Buf.find( "cpu MHz" )  )
	    {
		string::size_type Pos = Buf.find( ':' ); //Just find the start
		if( Pos == string::npos  ||  (++Pos) >= Buf.size()  ) //should be always false!
		    break; //Wup?!
	    
		Cpu_Info* cpu= m_Cpus.back();
		if( cpu ) //should be always true
		    cpu->Set_MHz( Buf.substr( Pos+1, Buf.size() - Pos+1 ).c_str() ); //give it the number :)
	    }
	    else if( !Buf.find( "cache size" )  )
	    {
                string::size_type Pos = Buf.find( ':' ); //Just find the start
	        if( Pos == string::npos  ||  (++Pos) >= Buf.size()  ) //should be always false!
	            break; //Wup?! oO
		    
		//Now we start Filtering a bit...
		string Cache = Buf.substr( Pos+1, Buf.size() - Pos+1 );
	        if( (Pos = Cache.find( ' ' ) ) != string::npos )
		    Cache.erase( Pos, 1 ); //Just clears out the Space between the kb/mb and the number!

                //Thats enough for us!
		
	        Cpu_Info* cpu= m_Cpus.back();
		if( cpu ) //should be always true!		
		    cpu->Set_Cache( Cache.c_str()  );
	    }

	    break;
	}

 
    }    
}
#endif

#if IsLin
bool Sysinfo::Uname_Version()
{
    FILE* file = popen( "uname -sr", "r" );
    if( file )
    {
	char Buffer[80];
	size_t Pos;
	if( (Pos = fread( Buffer, 1, 79, file )) !=0 )
	{
	    Buffer[Pos] = '\0';
	    m_Os = Buffer;
	    return true;
	}
	pclose( file );
    }
    return false;
}
#endif

#include <cstdlib>
extern string Binary_Location;


void Sysinfo::Gather_Binary_Size()
{
#if IsLin
    char Buffer[7];
    string Operation( "ls -s " );
    if( Binary_Location.find( "/" ) == string::npos )
	Operation += "/usr/bin/";
    Operation += Binary_Location;

    FILE* file = popen( Operation.c_str(), "r" );
    size_t Pos;
    if( (Pos = fread( Buffer, 1, 6, file )) !=0 )
    {
	Buffer[Pos-1] = '\0';

	for( unsigned i=0; Buffer ; ++i )
	{
	    if( Buffer[i] == ' ' )
	    {
		Buffer[i] = '\0';
		break;
	    }
	}
	m_Binary_Size = Buffer;
	m_Binary_Size += "KB";
    }
    else
	m_Binary_Size = "0";

    pclose( file );

#elif IsWin
    //We use the "" arround the path even if we don't need them, cause the space check would need more time than using it everytime
    char* Buffer = new char[300]; //Just to get enough ;)
    string Operation( "dir \"" );
    Operation += Binary_Location;
    Operation += "\"";
    FILE* file = _popen( Operation.c_str(), "r" );
    size_t Pos;
    if( (Pos = fread( Buffer, 1, 299, file )) !=0 )
    {
	Buffer[Pos-1] = '\0';
	string NewBuffer = Buffer;
	string::size_type End;
	string FileName = Binary_Location;
	string::size_type Start;
	if( (Start = FileName.rfind( "\\" )) )
	    FileName.erase( 0, Start+1 );
	
	if( (End = NewBuffer.find( FileName )) != string::npos ) 
	{
	    End -= 2;
	    string::size_type Begin = NewBuffer.rfind( ' ', End);
	    if( Begin != string::npos )
	    {
		++Begin;
		NewBuffer.erase( End+1 );
		NewBuffer.erase( 0, Begin );
		while( (Begin = NewBuffer.find( '.' )) != string::npos )
		    NewBuffer.erase(Begin,1);
		unsigned Size( atoi(NewBuffer.c_str() ) );
		Size >>= 10;
		stringstream Converter;
		Converter << Size;
		m_Binary_Size  = Converter.str();
		m_Binary_Size += "KB";
	    }
	    else
		m_Binary_Size = "0";
	}
	else
	    m_Binary_Size = "0";  
    }
    else
	m_Binary_Size = "0";

    delete[] Buffer;
#else
    m_Binary_Size = "0";
#endif
}
