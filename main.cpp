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
using std::stringstream;


#include "Output.hpp"
#include "Sysinfo.hpp"
#include "Benchmark.hpp"
#include "Main.hpp"


using Geno::Output;
using Geno::Sysinfo;
using Geno::Benchmark;

bool Secure = false;
bool Just_Interface = false;
bool No_Sys_Info = false;
bool Just_Cpu_Test = false;
bool Just_Mem_Test = false;
bool Just_MT_Test = false;
bool No_MT_Test = false;
bool No_Cpu_Test = false;
bool No_Mem_Test = false;
bool Detailed = false;
bool Force_No_Sys_Info = false;
bool No_Ref_Info = false;
bool No_Infos = false;
bool Save_Result = false;

#if IsLin
bool Force_No_Threads = false;
#else
bool Force_No_Threads = true;
#endif



const uint32_t Ref_S_i8 = 1090,
               Ref_S_i16 = 1259,
	       Ref_S_i32 = 1423,
	       Ref_S_i64 = 521,
	       Ref_S_fp32 = 874,
	       Ref_S_fp64 = 874,
	       Ref_S_fpxx = 530,

	       Ref_S_Mem_RW2 =1372,
	       Ref_S_Mem_RW4 =1034,
	       Ref_S_Mem_DA2 =785,
	       Ref_S_Mem_DA4 =758,
	       Ref_S_Mem_DA8 =421,

	       Ref_S_MT_MLuL =2490,
	       
	       Ref_S_I = (Ref_S_i8 + Ref_S_i16 + Ref_S_i32 + Ref_S_i64 )>>2,
	       Ref_S_Fp = (Ref_S_fp32 + Ref_S_fp64 + Ref_S_fpxx)/3,
	       Ref_S_Mem_RW = (Ref_S_Mem_RW2 + Ref_S_Mem_RW4)>>1,
	       Ref_S_Mem_DA = (Ref_S_Mem_DA2 + Ref_S_Mem_DA4 + Ref_S_Mem_DA8)/3,
	       Ref_S_MT = Ref_S_MT_MLuL;

const char Ref_Sys[] = { "Reference system was Geno's pc with:\nAthlon XP 1800+ 1575.631MHz, 256KB\nLinux 2.6.11-ck1\nGCC 3.4.3-20050110 (compiled with standard cflags)\nglibc 2.3.4 (with nptl)\n128KB binary size\nScores gathered on March, 30th. 2005 with bashmark 0.6" };


int32_t Calc_Ratio( const uint32_t Score, const uint32_t Ref_Score )
{
    if( !Score )
        return -100;
    else if( !Ref_Score )
        return Score; //hm
    else if( Score > Ref_Score )
        return static_cast< int32_t > ( ((Score - Ref_Score )/ (Ref_Score/ 100.0))+0.5 );
    else if( Score < Ref_Score )
        return static_cast< int32_t > ( -(((Ref_Score - Score)/ (Ref_Score/100.0))+0.5) );
	
    else //equal
        return 0;
}
//evil :/
void Write_Test( const char* Test, const uint32_t Score, const uint32_t other_S, const bool Empty=false )
{
    static const uint8_t Test_Len = 25,
                         Score_Len = 10;
//	static const uint8_t Space_Len = 5;
	
    stringstream str;
    str<<':'<<Test;
    std::string ToWrite = str.str();
    unsigned int bla = Test_Len-ToWrite.size(); //hu? Bug! I want it in the for!
    for( uint8_t i=0; i < bla-1; ++i )
 	ToWrite += " ";
    ToWrite += ":    :";
    Output::Write( ToWrite.c_str(), false  );
    
    stringstream str2;
    if( !Empty )
        str2<<Score;
    ToWrite = str2.str();
    std::string tmp;
    bla = Score_Len - ToWrite.size(); //^^
    for( uint8_t i=0; i < bla ; ++i )
        tmp += ' ';
    ToWrite.insert( 0, tmp );
    ToWrite += ":  :";
    Output::Write( ToWrite.c_str(), false );


    stringstream str3;
    if( !Empty )
    {
        const float Ratio = Calc_Ratio( Score, other_S );
	if( Ratio >= 0 )
	    str3<<'+';
        str3<< Ratio;
	str3<< '%';
    }
    ToWrite = str3.str();
    
    //tmp.clear(); some maybe don't like it...so ok, I will do something else!
    std::string Space_Adder;
    bla = Score_Len-ToWrite.size();
    for( ; bla ; --bla )
        Space_Adder += ' ';
    ToWrite.insert( 0, Space_Adder );
    ToWrite += ':';
    Output::Write( ToWrite.c_str()  );
}

#if IsWin
    const char Help_Usage[] = {"The usage is: 'bashmark.exe [options]'."};
#else
    const char Help_Usage[] = {"The usage is: 'bashmark [options]'."}; //What about "./" ?
#endif

//We should really split that up, add some empty lines, sort it etc.
const char Help[] = {"The options are the following:\r\n--save-result :Saves your result in a file so that you can upload it to our database.\r\n--secure / -s :Not as fragile for use spikes, but outputs first if all tests are done.\r\n--just-interface :Outputs just the interface without benchmarking.\r\n--no-sys-info :Omit the outputing of system infos.\r\n--fno-sys-info :Like '--no-sys-info', but also we force that the infos don't get gathered!\r\n--no-ref-info :Omit the outputing of the infos from the reference system.\r\n--no-infos :Omit the output of system and reference system infos.\r\n--fno-threads :Force that no threads are used for tests.\r\n--just-cpu-test :Does just the cpu related benchmarks.\r\n--just-mem-test :Does just memory related benchmarks.\r\n--just-mt-test :Does just multithreading related benchmarks.\r\n--no-cpu-test :Does none cpu related benchmarks.\r\n--no-mem-test :Does none memory related benchmarks.\r\n--no-mt-test :Does none multithreading related benchmarks.\r\n--detailed /-d :Shows you each test result.\r\n--version / -v: Shows the version.\r\n--help / -h :Outputs this :)."};

const char Version[] = {"Bashmark 0.6.2 by Geno" };
const char Version_Number[] = {"0.6.2"};


const char jc_a_jm[] = {"'--just-cpu-test' and '--just-mem-test' aren't supported together!"};
const char jc_a_jmt[] = {"--just-cpu-test' and '--just-mt-test' aren't supported together!"};
const char jm_a_jmt[] = {"'--just-mem-test' and '--just-mt-test' aren't supported together!"};


const char jc_a_nc[] = {"'--just-cpu-test' and '--no-cpu-test' together are invalid!"};
const char jm_a_nm[] = {"'--just-mem-test' and '--no-mem-test' together are invalid!"};
const char jmt_a_nmt[] = {"'--just-mt-test' and '--no-mt-test' together are invalid!"};
const char ji_a_sr[] = {"'--just-interface' and '--save-result' together are invalid!"}; //Cause we don't know what he realy want!


const char Skipping[] = {"Skipping Bashmark!"};

const char Bash_me[] = {"Yeah, I will bash you!\r\n"};



//Well, we could optimize it ...Later I will do it... ;)
bool Process_Parameter( int argc, char* argv[] )
{
    for( int i=1; i < argc; ++i )
    {
	if( !strcmp( argv[i], "--secure" ) || !strcmp( argv[i], "-s" )  )
	    ::Secure = true;
	    
	else if( !strcmp( argv[i], "--just-interface" )  )
	{
	    if( ::Save_Result )
	    {
		Output::Write( ji_a_sr );
		return false;
	    }
	    Just_Interface = true;
	}
	    
	else if( !strcmp( argv[i], "--help" )  || !strcmp( argv[i], "-h" )  )
	{
	    Output::Write( Help_Usage );
	    Output::Write( Help );
	    return false;
	}
	
	else if( !strcmp( argv[i], "--version" ) || !strcmp( argv[i], "-v" )  )
	{
	    Output::Write( Version );
	    return false;
	}
	
	else if( !strcmp( argv[i], "--no-sys-info" )  )
	    ::No_Sys_Info = true;
	else if( !strcmp( argv[i], "--fno-sys-info" )  )
	{
	    if( ::Save_Result )
	    {
		Output::Write( "Ignoring '", false);
		Output::Write( argv[i], false );
		Output::Write( "' for '--save-result'!" );
	    }
	    else
	    {
		::No_Sys_Info = true;
		::Force_No_Sys_Info = true;
	    }
	}
	else if( !strcmp( argv[i], "--no-ref-info" ) )
	    ::No_Ref_Info = true;
	else if( !strcmp( argv[i], "--no-infos" ) )
	    ::No_Infos = true;
	else if( !strcmp( argv[i], "--fno-threads")  )
	    ::Force_No_Threads = true;

	
	else if( !strcmp( argv[i], "--just-cpu-test" )  )
	{
	    if( Just_Mem_Test )
	    {
		Output::Write( jc_a_jm );
		Output::Write( Skipping );
		return false;
	    }
	    else if( Just_MT_Test )
	    {
	        Output::Write( jc_a_jmt );
	        Output::Write( Skipping );
	        return false;
	    }
	    else if( No_Cpu_Test )
	    {
	 	Output::Write( jc_a_nc );
		Output::Write( Skipping );
		return false;
	    }
	    if( ::Save_Result )
	    {
		Output::Write( "Ignoring '", false);
		Output::Write( argv[i], false );
		Output::Write( "' for '--save-result'!" );
	    }
	    
	    Just_Cpu_Test = true;
 	}
	
	else if( !strcmp( argv[i], "--just-mem-test" )  )
	{
	    if( Just_Cpu_Test )
	    {
	        Output::Write( jc_a_jm );
		Output::Write( Skipping );
		return false;
	    }
	    else if( Just_MT_Test )
	    {
	        Output::Write( jm_a_jmt );
		Output::Write( Skipping );
		return false;
	    }
	    else if( No_Mem_Test )
	    {
	        Output::Write( jm_a_nm );
		Output::Write( Skipping );
		return false;
	    }
	    if( ::Save_Result )
	    {
		Output::Write( "Ignoring '", false);
		Output::Write( argv[i], false );
		Output::Write( "' for '--save-result'!" );
	    }
	    
	    Just_Mem_Test = true;
	}
	
	else if( !strcmp( argv[i], "--just-mt-test" )  )
	{
	    if( Just_Cpu_Test )
	    {
	        Output::Write( jc_a_jmt );
		Output::Write( Skipping );
		return false;
	    }
	    else if( Just_Mem_Test )
	    {
	        Output::Write( jm_a_jmt );
		Output::Write( Skipping );
		return false;
	    }
	    else if( No_MT_Test )
	    {
	        Output::Write( jmt_a_nmt );
		Output::Write( Skipping );
		return false;
	    }
	    if( ::Save_Result )
	    {
		Output::Write( "Ignoring '", false);
		Output::Write( argv[i], false );
		Output::Write( "' for '--save-result'!" );
	    }
	    
	    Just_MT_Test = true;
  	}
	
	else if( !strcmp( argv[i], "--no-cpu-test" )   )
	{
	    if( Just_Cpu_Test )
	    {
		Output::Write( jc_a_nc );
		Output::Write( Skipping );
		return false;
	    }
	    if( ::Save_Result )
	    {
		Output::Write( "Ignoring '", false);
		Output::Write( argv[i], false );
		Output::Write( "' for '--save-result'!" );
	    }

	    No_Cpu_Test = true;
	    
	}
	
	else if( !strcmp( argv[i], "--no-mem-test" )   )
	{
	    if( Just_Mem_Test )
	    {
		Output::Write( jm_a_nm );
		Output::Write( Skipping );
		return false;
	    }
	    if( ::Save_Result )
	    {
		Output::Write( "Ignoring '", false);
		Output::Write( argv[i], false );
		Output::Write( "' for '--save-result'!" );
	    }
	    
	    No_Mem_Test = true;
	}
	
	else if( !strcmp( argv[i], "--no-mt-test" )   )
	{
	    if( ::Just_MT_Test )
	    {
	        Output::Write( jmt_a_nmt );
		Output::Write( Skipping );
		return false;
	    }
	    if( ::Save_Result )
	    {
		Output::Write( "Ignoring '", false);
		Output::Write( argv[i], false );
		Output::Write( "' for '--save-result'!" );
	    }
	    ::No_MT_Test = true;
	}
	
	else if( !strcmp( argv[i], "--detailed" ) || !strcmp( argv[i], "-d"  )  )
	    Detailed = true;
	
	else if( !strcmp( argv[i], "--bash-me" )  )
	{
	    Output::Write( Bash_me );
	    Output::Write( Help_Usage );
	    Output::Write( Help );
	    return false;
	}
	else if( !strcmp( argv[i], "--save-result" ) )
	{
	    bool Need_Override( false );
	    if( ::Just_MT_Test )
	    {
		::Just_MT_Test = false;
		Need_Override = true;
	    }
	    if( ::Just_Cpu_Test )
	    {
		::Just_Cpu_Test = false;
		Need_Override = true;
	    }
	    if( ::Just_Mem_Test )
	    {
		::Just_Mem_Test = false;
		Need_Override = true;

	    }
	    if( ::No_Cpu_Test )
	    {
		::No_Cpu_Test = false;
		Need_Override = true;
	    }
	    if( ::No_Mem_Test )
	    {
		::No_Mem_Test = false;
		Need_Override = true;
	    }
	    if( ::No_MT_Test )
	    {
		::No_MT_Test = false;
		Need_Override = true;
	    }
	    if( ::Force_No_Sys_Info  )
	    {
		::Force_No_Sys_Info = false;
		Need_Override = true;
	    }
	    if( ::Just_Interface )
	    {
		Output::Write( ji_a_sr );
		return false;
	    }
	    ::Secure = true;
	    ::Save_Result = true;
	    if( Need_Override )
		Output::Write( "Need to override parameters for the use of '--save-Result'!");
	}
	else
	{
	    Output::Write( "Ignoring unknown command '", false );
	    Output::Write( argv[i], false );
	    Output::Write( "'" );
	}
	
    }
	
    return true;
}

void Output_Head()
{
    Output::Write( "#######################################################" );
    Output::Write( ":  T   E   S   T        :    :S C O R E :  : R A T I O:" );
    Output::Write( ":-----------------------------------------------------:" );
    std::cout.flush(); //hack
}

void Output_Error( const uint32_t Error )
{
    switch( Error )
    {
    case Benchmark::Mem_Alloc:
        Output::Write( "Error: We run out of Memory!" );
	break;
    default:
        Output::Write( "Canceled for unknown reason!" );
	break;;
    } 
}

static uint8_t Number_of_Tests=0;
static uint8_t Current_Test=0;


void Status_Done( const uint16_t Percent )
{
    static uint16_t Last_Percent( 1000 );
    uint16_t New_Percent;
    if( !::Secure  &&  !::Detailed )
    {
	New_Percent = static_cast< uint16_t >(((100.0/Number_of_Tests)*Current_Test)+Percent/Number_of_Tests);
    }
    else
	New_Percent = Percent;
    
    if( Last_Percent == New_Percent )
	return;
    else
	Last_Percent = New_Percent;
    
    Output::Write( "\r", false );
    Output::Write( New_Percent, false );
    Output::Write( "%", false);
    
    std::cout.flush(); //bad hack
}

#include "Write_Result.hpp"
using Geno::Write_Result;
bool Write_Save_Result( const Benchmark*const Results )
{
    if( Results  &&   Results->Get_Error() == Benchmark::None )
    {
	Write_Result File( "Result.save" );
	if( File.Is_Ok() )
	{
	    if( File.Write( Results->Get_Integer8_Score() )  &&  File.Write( Results->Get_Integer16_Score() )  &&
		File.Write( Results->Get_Integer32_Score() )   &&  File.Write( Results->Get_Integer64_Score() )  &&
		File.Write( (Results->Get_Integer8_Score () + Results->Get_Integer16_Score ()  +
			     Results->Get_Integer32_Score() + Results->Get_Integer64_Score ()) >>2 )  &&
		File.Write( Results->Get_Fp32_Score() )  &&  File.Write( Results->Get_Fp64_Score() ) )
	    {
		if( Results->Get_Fp96_Score() )
		{
		    if( !File.Write( Results->Get_Fp96_Score() )  ||  !File.Write( 96 )  )
			return false;
		}
		else
		{
		    if( !File.Write( Results->Get_Fp128_Score() ) ||  !File.Write( 128 )  )
			return false;
		}
		
		if( File.Write( (Results->Get_Fp32_Score() + Results->Get_Fp64_Score () +
				 Results->Get_Fp96_Score() + Results->Get_Fp128_Score() )/3 )  &&
		    File.Write( Results->Get_Mem_RW2_Score() )  &&  File.Write( Results->Get_Mem_RW4_Score() ) &&
		    File.Write( (Results->Get_Mem_RW2_Score() + Results->Get_Mem_RW4_Score() ) >> 1)  &&
		    File.Write( Results->Get_Mem_DA2_Score() )  &&  File.Write( Results->Get_Mem_DA4_Score() ) &&
		    File.Write( Results->Get_Mem_DA8_Score() )  &&  File.Write( (Results->Get_Mem_DA2_Score()   +
										 Results->Get_Mem_DA4_Score()   +
										 Results->Get_Mem_DA8_Score()) /3)  &&
		    File.Write( Results->Get_MT_MLuL_Score() )  )
		{
		    Sysinfo* Infos( Sysinfo::Get_Instance() );
		    Sysinfo::Cpu_Storage Cpus( Infos->Get_Cpus() );
		    File.Write( Cpus.size() );
		    for( uint8_t i=0; i < Cpus.size(); ++i )
		    {
			File.Write( Cpus[i]->Get_Number() );
			File.Write( Cpus[i]->Get_Type() );
			File.Write( Cpus[i]->Get_MHz() );
			File.Write( Cpus[i]->Get_Cache() );
		    }
		    File.Write( Infos->Get_Os() );
		    File.Write( Infos->Get_Compiler()  );
		    File.Write( Version_Number );
		    File.Write( Infos->Get_Binary_Size() );
		    File.Write_End();

		    return true;
		}
	    }
	}
    }
    return false;
}

using std::string;
string Binary_Location;

int main( int argc, char* argv[] )
{
    if( !Process_Parameter( argc, argv ) )
    {
    #if IsWin
	system("Pause");
    #endif
	return -1;
    }
    Binary_Location = argv[0];
    
    Output_Head();
    
#if IsWin
    #define usleep( Value ) Sleep(Value/1000) //work arround
#endif
    if( !::Force_No_Sys_Info  &&  !::Force_No_Threads  &&  !::Just_Interface )
        usleep( 10000 ); //If I don't do it, it don't output anything for a half minute! Very strange( But just with threading and I need higher waiting times if the system is very busy)!

    Benchmark MyBench;
    
    if( Secure )
    {
        if( !Just_Interface )
            MyBench.Run();
	if( MyBench.Get_Error() )
	{
	    Output_Error( MyBench.Get_Error() );
	    return -1;
	}
	Output::Write( "\r", false );
	
        if( !::Just_Mem_Test   &&   !::Just_MT_Test    &&   !::No_Cpu_Test )
        {
      	    if( Detailed )
       	    {
                Write_Test( " 8bit integer", MyBench.Get_Integer8_Score(), Ref_S_i8  );
                Write_Test( "16bit integer", MyBench.Get_Integer16_Score(), Ref_S_i16  );
                Write_Test( "32bit integer", MyBench.Get_Integer32_Score(), Ref_S_i32   );
                Write_Test( "64bit integer", MyBench.Get_Integer64_Score(), Ref_S_i64   );
                Write_Test( "32bit fp",  MyBench.Get_Fp32_Score(), Ref_S_fp32   );
                Write_Test( "64bit fp", MyBench.Get_Fp64_Score(), Ref_S_fp64   );
                if( sizeof( long double ) == 12 )
                    Write_Test( "96bit(80real) fp", MyBench.Get_Fp96_Score(), Ref_S_fpxx   );
                else if( sizeof( long double ) == 16 )
                    Write_Test( "128bit(80real) fp", MyBench.Get_Fp128_Score(), Ref_S_fpxx  );
       	     }
       	     else
       	     {
     	         Write_Test( "Cpu, Integer", ( MyBench.Get_Integer8_Score () +
        	  	                       MyBench.Get_Integer16_Score() +
        		                       MyBench.Get_Integer32_Score() +
        		                       MyBench.Get_Integer64_Score()  )>>2, 
					       Ref_S_I );
            
	         Write_Test( "Cpu, Floating point", ( MyBench.Get_Fp32_Score() +
        		                              MyBench.Get_Fp64_Score() +
        		                              MyBench.Get_Fp96_Score() +
        		                              MyBench.Get_Fp128_Score() )/3,
						      Ref_S_Fp);
             }
        }
    
        if( !::Just_Cpu_Test    &&   !::Just_MT_Test    &&    !::No_Mem_Test )
        {
      	    if( !::Just_Mem_Test   &&   !::No_Cpu_Test )
       	        Write_Test( " ",0, 0, true );
       	    if( Detailed )
       	    {
                Write_Test( "128B Mem r/w (cached)", MyBench.Get_Mem_RW2_Score(), Ref_S_Mem_RW2 );
                Write_Test( "256B Mem r/w (cached)", MyBench.Get_Mem_RW4_Score(), Ref_S_Mem_RW4  );
                Write_Test( " 32Byte Mem de-/alloc", MyBench.Get_Mem_DA2_Score(), Ref_S_Mem_DA2  );    
                Write_Test( " 64Byte Mem de-/alloc", MyBench.Get_Mem_DA4_Score(), Ref_S_Mem_DA4  );
                Write_Test( "128Byte Mem de-/alloc", MyBench.Get_Mem_DA8_Score(), Ref_S_Mem_DA8  );
       	     }
       	     else
       	     {
    	         Write_Test( "Memory r/w (cached)", ( MyBench.Get_Mem_RW2_Score() +
                                                    MyBench.Get_Mem_RW4_Score() ) >>1,
						    Ref_S_Mem_RW );
        
	         Write_Test( "Memory de-/alloc", ( MyBench.Get_Mem_DA2_Score() +
                                                   MyBench.Get_Mem_DA4_Score() +
                                                   MyBench.Get_Mem_DA8_Score() )/3,
						   Ref_S_Mem_DA );
              }
        }
	
        if( !::Just_Cpu_Test   &&   !::Just_Mem_Test   &&    !::No_MT_Test )
        {
            if( !::Just_MT_Test &&  (!::No_Mem_Test  ||  !::No_Cpu_Test) )
                Write_Test( " ",0, 0, true );
            if( Detailed )
            {
                Write_Test( "MThreading u-/lock", MyBench.Get_MT_MLuL_Score(), Ref_S_MT_MLuL  );
            }
            else
            {
                Write_Test( "Multithreading", MyBench.Get_MT_MLuL_Score(), Ref_S_MT );
            }
        }
	
    }
    
    else
    {
	if( !Just_Mem_Test  &&  !Just_MT_Test && !No_Cpu_Test )
	{
	    if( !Just_Interface )
	    {
		::Number_of_Tests = Benchmark::Number_Cpu_Int_Test;
                ::Current_Test    = 0;
	        MyBench.Integer8_Test();
		if( MyBench.Get_Error() )
		{
		    Output_Error( MyBench.Get_Error() );
		    return -1;
		}
	    }
	    if( Detailed )
	    {
	    	Output::Write( "\r", false );
                Write_Test( " 8bit integer", MyBench.Get_Integer8_Score(), Ref_S_i8 );
	    }
		    
	    if( !Just_Interface )
	    {
                ++::Current_Test;

	        MyBench.Integer16_Test();
		if( MyBench.Get_Error() )
		{
		    Output_Error( MyBench.Get_Error() );
		    return -1;
		}
	    }
	    if( Detailed )
	    {
		Output::Write( "\r", false );
                Write_Test( "16bit integer", MyBench.Get_Integer16_Score(), Ref_S_i16 );
	    }
		    
	    if( !Just_Interface )
	    {
		++::Current_Test;
	        MyBench.Integer32_Test();
		if( MyBench.Get_Error() )
		{
		    Output_Error( MyBench.Get_Error() );
		    return -1;
		}
	    }
	    if( Detailed )
	    {
		Output::Write( "\r", false );
    	        Write_Test( "32bit integer", MyBench.Get_Integer32_Score(), Ref_S_i32 );
	    }
		    
	    if( !Just_Interface )
	    {
		++::Current_Test;
	        MyBench.Integer64_Test();
		if( MyBench.Get_Error() )
		{
		    Output_Error( MyBench.Get_Error() );
		    return -1;
		}
	    }
	    if( Detailed )
	    {
		Output::Write( "\r", false );
                Write_Test( "64bit integer", MyBench.Get_Integer64_Score(), Ref_S_i64 );
	    }
		    
	    if( !Detailed )
	    {
		Output::Write( "\r", false );
       		Write_Test( "Cpu, Integer", ( MyBench.Get_Integer8_Score()  +
          		                      MyBench.Get_Integer16_Score() +
        		                      MyBench.Get_Integer32_Score() +
        		                      MyBench.Get_Integer64_Score() )>>2,
					      Ref_S_I );
            }
		   
		   
	    if( !Just_Interface )
	    {
		::Number_of_Tests = Benchmark::Number_Cpu_Fp_Test;
                ::Current_Test    = 0;

	        MyBench.Fp32_Test();
		if( MyBench.Get_Error() )
		{
		    Output_Error( MyBench.Get_Error() );
		    return -1;
		}
	    }
	    if( Detailed )
	    {
		Output::Write( "\r", false );
	        Write_Test( "32bit fp",  MyBench.Get_Fp32_Score(), Ref_S_fp32 );
	    }
	            
	    if( !Just_Interface )
	    {
		++::Current_Test;
	        MyBench.Fp64_Test();
		
		if( MyBench.Get_Error() )
		{
		    Output_Error( MyBench.Get_Error() );
		    return -1;
		}
	    }
	    if( Detailed )
	    {
		Output::Write( "\r", false );
	        Write_Test( "64bit fp", MyBench.Get_Fp64_Score(), Ref_S_fp64 );
	    }
	            
	    if( sizeof( long double ) == 12 )
	    {
	        if( !Just_Interface )
		{
		    ++::Current_Test;
		    MyBench.Fp96_Test();
		    if( MyBench.Get_Error() )
		    {
			Output_Error( MyBench.Get_Error() );
			return -1;
		    }
		}
		if( Detailed )
		{
		    Output::Write( "\r", false );
	            Write_Test( "96bit(80real) fp", MyBench.Get_Fp96_Score(), Ref_S_fpxx );
		}
	    }
	    else if( sizeof( long double ) == 16 )
	    {
	        if( !Just_Interface )
		{
		    ++::Current_Test;
		    MyBench.Fp128_Test();
		    if( MyBench.Get_Error() )
		    {
			Output_Error( MyBench.Get_Error() );
			return -1;
		    }
		}
		if( Detailed )
		{
		    Output::Write( "\r", false );
	            Write_Test( "128bit(80real) fp", MyBench.Get_Fp128_Score(), Ref_S_fpxx );
		}
	    }
	        
	    if( !Detailed )
	    {
		Output::Write( "\r", false );
        	Write_Test( "Cpu, Floating point", (  MyBench.Get_Fp32_Score() +
        		                              MyBench.Get_Fp64_Score() +
        		                              MyBench.Get_Fp96_Score() +
        		                              MyBench.Get_Fp128_Score() )/3,
						      Ref_S_Fp );
	   }
	        
	}
	
	if( !::Just_Cpu_Test   &&   !::Just_MT_Test   &&   !::No_Mem_Test )
	{
	    if( !::Just_Mem_Test  &&  !::No_Cpu_Test )
	        Write_Test( " ",0, 0, true );
	    if( !Just_Interface )
	    {
		::Number_of_Tests = Benchmark::Number_Mem_RW_Test;
                ::Current_Test    = 0;

	        MyBench.Mem_RW2_Test();
		if( MyBench.Get_Error() )
		{
		    Output_Error( MyBench.Get_Error() );
		    return -1;
		}
	    }
	    if( Detailed )
	    {
		Output::Write( "\r", false );
	        Write_Test( "128B Mem r/w (cached)", MyBench.Get_Mem_RW2_Score(), Ref_S_Mem_RW2  );
	    }
	        
	    if( !Just_Interface )
	    {
		++::Current_Test;
	        MyBench.Mem_RW4_Test();
				if( MyBench.Get_Error() )
		{
		    Output_Error( MyBench.Get_Error() );
		    return -1;
		}
	    }
	    if( Detailed )
	    {
		Output::Write( "\r", false );
	        Write_Test( "256B Mem r/w (cached)", MyBench.Get_Mem_RW4_Score(), Ref_S_Mem_RW4  );
	    }
	        
	    if( !Detailed )
	    {
		Output::Write( "\r", false );
        	Write_Test( "Memory r/w (cached)", (MyBench.Get_Mem_RW2_Score() +
        	                                  MyBench.Get_Mem_RW4_Score() ) >>1,
						  Ref_S_Mem_RW );
	    }
      
	    if( !Just_Interface )
	    {
		::Number_of_Tests = Benchmark::Number_Mem_DA_Test;
                ::Current_Test    = 0;

	        MyBench.Mem_DA2_Test();
		if( MyBench.Get_Error() )
		{
		    Output_Error( MyBench.Get_Error() );
		    return -1;
		}
	    }
	    if( Detailed )
	    {
		Output::Write( "\r", false );
	        Write_Test( " 32Byte Mem de-/alloc", MyBench.Get_Mem_DA2_Score(), Ref_S_Mem_DA2  );
	    }
	        
	    if( !Just_Interface )
	    {
		++::Current_Test;
	        MyBench.Mem_DA4_Test();
		if( MyBench.Get_Error() )
		{
		    Output_Error( MyBench.Get_Error() );
		    return -1;
		}
	    }
	    if( Detailed )
	    {
		Output::Write( "\r", false );
	        Write_Test( " 64Byte Mem de-/alloc", MyBench.Get_Mem_DA4_Score(), Ref_S_Mem_DA4  );
	    }
	        
	    if( !Just_Interface )
	    {
		++::Current_Test;
	        MyBench.Mem_DA8_Test();
		if( MyBench.Get_Error() )
		{
		    Output_Error( MyBench.Get_Error() );
		    return -1;
		}
	    }
	    if( Detailed )
	    {
		Output::Write( "\r", false );
	        Write_Test( "128Byte Mem de-/alloc", MyBench.Get_Mem_DA8_Score(), Ref_S_Mem_DA8  );
	    }
	        
	    if( !Detailed )
	    {
		Output::Write( "\r", false );
	        Write_Test( "Memory de-/alloc", (MyBench.Get_Mem_DA2_Score() +
        	                                 MyBench.Get_Mem_DA4_Score() +
        	                                 MyBench.Get_Mem_DA8_Score() )/3,
						 Ref_S_Mem_DA );
	    }
	        
       }      
       if( !::Just_Cpu_Test    &&    !::Just_Mem_Test    &&    !::No_MT_Test )
       {
       	   if( !::Just_MT_Test  &&  (!::No_Mem_Test || !::No_Cpu_Test) )
	       Write_Test( " ",0, 0, true );
	   if( !Just_Interface ) 
	   {
		::Number_of_Tests = Benchmark::Number_MT_MLuL_Test;
                ::Current_Test    = 0;

	       MyBench.MT_MLuL_Test();
		if( MyBench.Get_Error() )
		{
		    Output_Error( MyBench.Get_Error() );
		    return -1;
		}
	    }
	   if( Detailed )
	   {
		Output::Write( "\r", false );
	        Write_Test( "MThreading u-/lock", MyBench.Get_MT_MLuL_Score(), Ref_S_MT_MLuL  );
	    }
	   if( !Detailed )
	   {
		Output::Write( "\r", false );
	      	Write_Test( "Multithreading", MyBench.Get_MT_MLuL_Score() ,
					      Ref_S_MT);
	   }
	   
        }
	    
    }    

    
    if( ::Save_Result )
    {
	if( Write_Save_Result( &MyBench ) )
	    Output::Write( "Saving of the results done." );
	else
	    Output::Write( "Error while saving the results!" );
    }


    if( !No_Sys_Info && !No_Infos )
    {    
        Output::Write( "#######################################################");
        Output::Write( ":           S  Y  S  T  E  M     I  N  F  O           :");
        Output::Write( "-------------------------------------------------------");
	    
	
	//We really need to update it!!
	Geno::Sysinfo* PC( Geno::Sysinfo::Get_Instance()  );
	const Sysinfo::Cpu_Storage* cpus = &PC->Get_Cpus();
	for( unsigned short i=0; i < cpus->size(); ++i )
	{
	    Output::Write( (*cpus)[i]->Get_Number(), false );
	    Output::Write( "x ", false );
	    Output::Write( (*cpus)[i]->Get_Type(), false );
	    Output::Write( " ", false );
	    Output::Write( (*cpus)[i]->Get_MHz(), false );
	    Output::Write( "MHz", false );
	    if( strcmp((*cpus)[i]->Get_Cache(), "0" ) )
	    {
	        Output::Write( ", L2 ", false );
	        Output::Write( (*cpus)[i]->Get_Cache() );
	    }
	    else
		Output::Write( "" );
	}
       	
	if(  !cpus->size()  )
	{
	     if( strlen( PC->Get_Os() )   )
	         Output::Write( "I couldn't find out your cpu informations" );
             else
	         Output::Write( "I couldn't find out your Os informations" );
	
	}
	if( strlen( PC->Get_Os() )  )
	    Output::Write( PC->Get_Os()  );
	
	Output::Write( PC->Get_Compiler() );
	Output::Write( PC->Get_Binary_Size(), false );
	Output::Write( " binary size" );
     }

     if( !::No_Ref_Info  &&  !::No_Infos )
     {
	Output::Write( "#######################################################");
	Output::Write( ":      R E F E R E N C E   S Y S T E M   I N F O      :");
	Output::Write( "-------------------------------------------------------");
	Output::Write( Ref_Sys );
    }
     
         


     #if IsWin 
         system("Pause"); //Cause some won't use a console!
     #endif
}

