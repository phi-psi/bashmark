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
#include "Benchmark.hpp"
#include "Stopwatch.hpp"
#include "Main.hpp"


#include "Sysinfo.hpp" //Need it to get the cpu number, so we can change the thread number!


using Geno::Benchmark;
using Geno::Stopwatch;
using Geno::Sysinfo;


void Status_Done( uint16_t );

//Little work arround
#if IsWin
    #define usleep( Value ) Sleep( Value )
#endif


const uint8_t Benchmark::Time_Per_Test = 10;
const uint8_t Benchmark::Status_Mul = Time_Per_Test;
const uint8_t Benchmark::Number_Cpu_Int_Test = 4;
const uint8_t Benchmark::Number_Cpu_Fp_Test = 3;
const uint8_t Benchmark::Number_Mem_RW_Test = 2;
const uint8_t Benchmark::Number_Mem_DA_Test = 3;
const uint8_t Benchmark::Number_MT_MLuL_Test = 1;


uint8_t Benchmark::Thread_Number = 1; // But in the ctor we'll look for the cpu number and change the value!
uint32_t Benchmark::m_Error = None;

extern bool Force_No_Sys_Info;
extern bool Force_No_Threads;


Benchmark::Benchmark()
: m_Integer8_Score( 0 ), m_Integer16_Score( 0 ), m_Integer32_Score( 0 ), m_Integer64_Score( 0 ),
  m_Fp32_Score( 0 ), m_Fp64_Score(0), m_Fp96_Score( 0 ), m_Fp128_Score( 0 ),
  m_Mem_RW2_Score(0), m_Mem_RW4_Score(0), m_Mem_DA2_Score(0), m_Mem_DA4_Score(0), m_Mem_DA8_Score(0),
  m_MT_MLuL_Score( 0 )
{
    if( !::Force_No_Sys_Info  &&  !::Force_No_Threads )
    {
	Sysinfo* PC ( Sysinfo::Get_Instance()  );

	if( !PC->Get_Cpus().empty()  )
	{
	    Thread_Number = 0;
	    for( unsigned short i=0; i < PC->Get_Cpus().size() ;++i )
	    {
		Thread_Number += PC->Get_Cpus()[i]->Get_Number();
	    }
	}
    }
}


void Benchmark::Integer8_Test()
{
    double Score   ( 0 ),
           Cur_Time( 0 );
    uint32_t loops ( 0 );
    Stopwatch watch;
	
    watch.Start();
    for( ; Cur_Time < Time_Per_Test; ++loops, watch.Look( Cur_Time ), Status_Done( static_cast< uint32_t >( Status_Mul*Cur_Time  ))  )
        Score += Intern_Integer8_Test();
	  
    m_Integer8_Score = static_cast< uint32_t >(Score/loops);
}

void Benchmark::Integer16_Test()
{
	double tmp(0);
	double Cur_Time(0);
	uint32_t loops=0;
	Stopwatch watch;
	
	watch.Start();
	for( ; Cur_Time < Time_Per_Test; ++loops, watch.Look( Cur_Time ), Status_Done( static_cast< uint32_t >( Status_Mul*Cur_Time  ))  )
	    tmp += Intern_Integer16_Test();
	  
	m_Integer16_Score = static_cast< uint32_t >(tmp/loops);
}

void Benchmark::Integer32_Test()
{
	double tmp(0);
	double Cur_Time(0);
	uint32_t loops=0;
	Stopwatch watch;
	
	watch.Start();
	for( ; Cur_Time < Time_Per_Test; ++loops, watch.Look( Cur_Time ), Status_Done( static_cast< uint32_t >( Status_Mul*Cur_Time  ))  ) 
	    tmp += Intern_Integer32_Test();
	   
	m_Integer32_Score = static_cast< uint32_t >((tmp/loops));
}
void Benchmark::Integer64_Test()
{
	double tmp(0);
	double Cur_Time(0);
	uint32_t loops=0;
	Stopwatch watch;
	
	watch.Start();
	for( ; Cur_Time < Time_Per_Test; ++loops, watch.Look( Cur_Time ), Status_Done( static_cast< uint32_t >( Status_Mul*Cur_Time  ))  )
	   tmp += Intern_Integer64_Test();
	    
	m_Integer64_Score = static_cast< uint32_t >(tmp/loops);
}

void Benchmark::Fp32_Test()
{
    if( sizeof( float ) != 4  &&  sizeof( double ) != 4   )
        return;
	double tmp(0);
	double Cur_Time(0);
	uint32_t loops=0;
	Stopwatch watch;
	
	
	watch.Start();
	for( ; Cur_Time < Time_Per_Test; ++loops, watch.Look( Cur_Time ), Status_Done( static_cast< uint32_t >( Status_Mul*Cur_Time  ))  )
	    tmp += Intern_Fp32_Test(); 
      
	m_Fp32_Score = static_cast< uint32_t >(tmp/loops);
}
void Benchmark::Fp64_Test()
{
	if( sizeof( double ) != 8  && sizeof( long double ) != 8)  
	    return;
	double tmp(0);
	double Cur_Time(0);
	uint32_t loops=0;
	Stopwatch watch;
	

	watch.Start();
	for( ; Cur_Time < Time_Per_Test; ++loops, watch.Look( Cur_Time ), Status_Done( static_cast< uint32_t >( Status_Mul*Cur_Time  ))  )
	    tmp += Intern_Fp64_Test(); 

	    
	m_Fp64_Score = static_cast< uint32_t >(tmp/loops);
}
void Benchmark::Fp96_Test()
{
	if( sizeof( long double ) != 12 ) 
	    return;
	double tmp(0);
	double Cur_Time(0);
	uint32_t loops=0;
	Stopwatch watch;
	
	
	watch.Start();
	for( ; Cur_Time < Time_Per_Test; ++loops, watch.Look( Cur_Time ), Status_Done( static_cast< uint32_t >( Status_Mul*Cur_Time  ))  )
	    tmp += Intern_Fp96_Test();
	    
	m_Fp96_Score = static_cast< uint32_t >(tmp/loops);
}
void Benchmark::Fp128_Test()
{
	if( sizeof( long double ) != 16 )  
	    return;
	double tmp(0);
	double Cur_Time(0);
	uint32_t loops=0;
	Stopwatch watch;
	
	
	watch.Start();
	for( ; Cur_Time < Time_Per_Test; ++loops, watch.Look( Cur_Time ), Status_Done( static_cast< uint32_t >( Status_Mul*Cur_Time  ))  )
	    tmp += Intern_Fp128_Test();
	
	m_Fp128_Score = static_cast< uint32_t >(tmp/loops);
}

void Benchmark::MT_MLuL_Test()
{
	double tmp(0);
	double Cur_Time(0);
	uint32_t loops=0;
	Stopwatch watch;
	
	watch.Start();
	for( ; Cur_Time < Time_Per_Test; ++loops, watch.Look( Cur_Time ), Status_Done( static_cast< uint32_t >( Status_Mul*Cur_Time  ))  ) 
	    tmp += Intern_MT_MLuL_Test();
	   
	m_MT_MLuL_Score = static_cast< uint32_t >((tmp/loops));
}

Benchmark::Test_Re_Type Benchmark::Intern_Integer8_Test()
{
	return Calculation_Test< uint8_t, 50 > ();
}
Benchmark::Test_Re_Type Benchmark::Intern_Integer16_Test()
{
	return Calculation_Test< uint16_t, 50 > ();
}
Benchmark::Test_Re_Type Benchmark::Intern_Integer32_Test()
{
	return Calculation_Test< uint32_t, 50 > ();
}
Benchmark::Test_Re_Type Benchmark::Intern_Integer64_Test()
{
	return Calculation_Test< uint64_t, 30 > ();
}
Benchmark::Test_Re_Type Benchmark::Intern_Fp32_Test()
{
	if( sizeof( float ) != 4  &&  sizeof( double ) != 4   )
	    return 0;
	    
	if( sizeof( float ) == 4 )
	    return Calculation_Test< float, 50 > ();
	else if( sizeof( double ) == 4 )
	    return Calculation_Test< double, 50 > ();
	else
	    return 0;
}   
Benchmark::Test_Re_Type Benchmark::Intern_Fp64_Test()
{
	if( sizeof( float ) != 8  &&  sizeof( double ) != 8  && sizeof(long double) != 8 )
	    return 0;
	    
	if( sizeof( float ) == 8 )
	    return Calculation_Test< float, 50 > ();
	else if( sizeof( double ) == 8 )
	    return Calculation_Test< double, 50 > ();
	else if( sizeof( long double ) == 8 )
	    return Calculation_Test< long double, 50 > ();
	else
	    return 0;
}   
Benchmark::Test_Re_Type Benchmark::Intern_Fp96_Test()
{
	if( sizeof( long double ) != 12   )
	    return 0;
	    
	if( sizeof( long double ) == 12 )
	    return Calculation_Test< long double, 25 > ();
	else
	    return 0;
}   
Benchmark::Test_Re_Type Benchmark::Intern_Fp128_Test()
{
	if( sizeof( long double ) != 16   )
	    return 0;
	    
	if( sizeof( long double ) == 16 )
	    return Calculation_Test< long double, 25 > ();
	else
	    return 0;
}

Benchmark::Test_Re_Type Benchmark::Intern_Mem_RW4_Test()
{
    return Memory_RW_Test< 256, 3500 > ();
}

Benchmark::Test_Re_Type Benchmark::Intern_Mem_RW2_Test()
{
    return Memory_RW_Test< 128, 4000 > ();
}

Benchmark::Test_Re_Type Benchmark::Intern_Mem_DA8_Test()
{
    return Memory_DA_Test< 128, 1000 > ();
}

Benchmark::Test_Re_Type Benchmark::Intern_Mem_DA4_Test()
{
    return Memory_DA_Test< 64, 2000 > ();
}

Benchmark::Test_Re_Type Benchmark::Intern_Mem_DA2_Test()
{
    return Memory_DA_Test< 32, 2000 > ();
}
Benchmark::Test_Re_Type Benchmark::Intern_MT_MLuL_Test()
{
    return MultiT_MLuL_Test< 5000 > ();
}

void Benchmark::Mem_RW2_Test()
{
	double tmp(0);
	double Cur_Time(0);
	uint32_t loops=0;
	Stopwatch watch;
	
	watch.Start();
	for( ; Cur_Time < Time_Per_Test; ++loops, watch.Look( Cur_Time ), Status_Done( static_cast< uint32_t >( Status_Mul*Cur_Time  ))  )
	   tmp += Intern_Mem_RW2_Test();
	    
	m_Mem_RW2_Score = static_cast< uint32_t >(tmp/loops);	
}
void Benchmark::Mem_RW4_Test()
{
	double tmp(0);
	double Cur_Time(0);
	uint32_t loops=0;
	Stopwatch watch;
	
	watch.Start();
	for( ; Cur_Time < Time_Per_Test; ++loops, watch.Look( Cur_Time ), Status_Done( static_cast< uint32_t >( Status_Mul*Cur_Time  ))  )
	   tmp += Intern_Mem_RW4_Test();
	    
	m_Mem_RW4_Score = static_cast< uint32_t >(tmp/loops);	
}
void Benchmark::Mem_DA2_Test()
{
	double tmp(0);
	double Cur_Time(0);
	uint32_t loops=0;
	Stopwatch watch;
	
	watch.Start();
	for( ; Cur_Time < Time_Per_Test; ++loops, watch.Look( Cur_Time ), Status_Done( static_cast< uint32_t >( Status_Mul*Cur_Time  ))  )
	   tmp += Intern_Mem_DA2_Test();
	    
	m_Mem_DA2_Score = static_cast< uint32_t >(tmp/loops);	
}
void Benchmark::Mem_DA4_Test()
{
	double tmp(0);
	double Cur_Time(0);
	uint32_t loops=0;
	Stopwatch watch;
	
	watch.Start();
	for( ; Cur_Time < Time_Per_Test; ++loops, watch.Look( Cur_Time ), Status_Done( static_cast< uint32_t >( Status_Mul*Cur_Time  ))  )
	   tmp += Intern_Mem_DA4_Test();
	    
	m_Mem_DA4_Score = static_cast< uint32_t >(tmp/loops);	
}

void Benchmark::Mem_DA8_Test()
{
	double tmp(0);
	double Cur_Time(0);
	uint32_t loops=0;
	Stopwatch watch;
	
	watch.Start();
	for( ; Cur_Time < Time_Per_Test; ++loops, watch.Look( Cur_Time ), Status_Done( static_cast< uint32_t >( Status_Mul*Cur_Time  ))  )
	   tmp += Intern_Mem_DA8_Test();
	    
	m_Mem_DA8_Score = static_cast< uint32_t >(tmp/loops);	
}



extern bool Just_Mem_Test;
extern bool Just_Cpu_Test;
extern bool Just_MT_Test;
extern bool No_Cpu_Test;
extern bool No_Mem_Test;
extern bool No_MT_Test;

//todo: Use a typedef for the saving type
void Benchmark::Run()
{
		Stopwatch watch;
		uint32_t tmp_Time;
		if( ::Just_Mem_Test )
		    tmp_Time = Time_Per_Test * 5;
		else if( ::Just_Cpu_Test )
		    tmp_Time = Time_Per_Test * 7;
		else if( ::Just_MT_Test )
		    tmp_Time = Time_Per_Test * 1;
		else
		    tmp_Time = Time_Per_Test * 13;
		    
		if( No_Cpu_Test )
		    tmp_Time -= Time_Per_Test * 7;
		if( No_Mem_Test )
		    tmp_Time -= Time_Per_Test * 5;
		if( No_MT_Test )
		    tmp_Time -= Time_Per_Test * 1;
		
		
		const uint32_t Time( tmp_Time );
		uint32_t Loops(0);
		
		double   Re8int(0),
		         Re16int(0),
		         Re32int(0),
		         Re64int(0),
		         Re32Fp(0),
		         Re64Fp(0),
		         Re96Fp(0),
		         Re128Fp(0),
		         ReMemRW2(0),
		         ReMemRW4(0),
		         ReMemDA2(0),
		         ReMemDA4(0),
		         ReMemDA8(0),
		         ReMTMLuL(0);
		         
		double Cur_Time(0);
		Status_Done(0);
		watch.Start();
		for( ; Cur_Time < Time; ++Loops, watch.Look( Cur_Time ), Status_Done( static_cast< uint32_t >( (100.0/Time)*Cur_Time  )) )
		{
			if( !Just_Mem_Test && !Just_MT_Test  &&  !No_Cpu_Test)
			{
			    Re8int += static_cast< double > (Intern_Integer8_Test() );
			    if( m_Error )
			    	return;
			    Re16int += static_cast< double > (Intern_Integer16_Test() );
			    if( m_Error )
			    	return;
			    Re32int += static_cast< double > (Intern_Integer32_Test() );
			    if( m_Error )
			    	return;
			    Re64int += static_cast< double > (Intern_Integer64_Test() );
			
			    Re32Fp += static_cast< double > (Intern_Fp32_Test() );
			    if( m_Error )
			    	return;
			    Re64Fp += static_cast< double > (Intern_Fp64_Test() );
			    if( m_Error )
			    	return;
			    if( sizeof( long double ) == 12 )
			        Re96Fp += static_cast< double > (Intern_Fp96_Test() );
			    else if( sizeof( long double ) == 16 )
			        Re128Fp += static_cast< double > (Intern_Fp128_Test() );

			    if( m_Error )
			    	return;
			}
			if( !Just_Cpu_Test &&  !Just_MT_Test && !No_Mem_Test)
			{
			    ReMemRW2 += static_cast< double > (Intern_Mem_RW2_Test() );
			    if( m_Error )
				return;
			    ReMemRW4 += static_cast< double > (Intern_Mem_RW4_Test() );
			    if( m_Error )
			    	return;
			    ReMemDA2 += static_cast< double > (Intern_Mem_DA2_Test() );
			    if( m_Error )
			    	return;
			    ReMemDA8 += static_cast< double > (Intern_Mem_DA8_Test() );
			    if( m_Error )
			    	return;
			    ReMemDA4 += static_cast< double > (Intern_Mem_DA4_Test() ); 
			    if( m_Error )
			    	return;
			}
			if( !Just_Cpu_Test && !Just_Mem_Test  && !No_MT_Test )
			{
			    ReMTMLuL += static_cast< double > (Intern_MT_MLuL_Test() );
			    if( m_Error )
			        return;
			}
		}
		
		if( !Just_Mem_Test && !Just_MT_Test  && !No_Cpu_Test)
		{
		    //Todo:Change the cast to use the typedef!
		    m_Integer8_Score = static_cast< Benchmark::Score_Type >  ( (Re8int / static_cast<double> (Loops) )  );
		    m_Integer16_Score = static_cast< Benchmark::Score_Type > ( (Re16int / static_cast<double> (Loops) ) );
		    m_Integer32_Score = static_cast< Benchmark::Score_Type > ( (Re32int / Loops ) );
		    m_Integer64_Score = static_cast< Benchmark::Score_Type > ( (Re64int / static_cast<double> (Loops) ) );
		
		
		    m_Fp32_Score = static_cast< Benchmark::Score_Type >  ( (Re32Fp / static_cast<double> (Loops) ) );
		    m_Fp64_Score = static_cast< Benchmark::Score_Type >  ( (Re64Fp / static_cast<double> (Loops) ) );
		    m_Fp96_Score = static_cast< Benchmark::Score_Type >  ( (Re96Fp / static_cast<double> (Loops) ) );
		    m_Fp128_Score = static_cast< Benchmark::Score_Type > ( (Re128Fp / static_cast<double>(Loops) ) );
		}
		if( !Just_Cpu_Test  &&  !Just_MT_Test  && !No_Mem_Test)
		{
		    m_Mem_RW2_Score = static_cast< Benchmark::Score_Type > ( ReMemRW2 / Loops );
		    m_Mem_RW4_Score = static_cast< Benchmark::Score_Type > ( ReMemRW4 / Loops );
		    m_Mem_DA2_Score = static_cast< Benchmark::Score_Type > ( ReMemDA2 / Loops );
		    m_Mem_DA4_Score = static_cast< Benchmark::Score_Type > ( ReMemDA4 / Loops );
		    m_Mem_DA8_Score = static_cast< Benchmark::Score_Type > ( ReMemDA8 / Loops );
		}
		if( !Just_Cpu_Test && ! Just_Mem_Test && !No_MT_Test)
		{
			m_MT_MLuL_Score = static_cast< Benchmark::Score_Type > ( ReMTMLuL / Loops );
		}
	
	
}

#include "Smart_Array_Pointer.hpp"
using Geno::Smart_Array_Pointer;


class Little_Thread_Helper
{
public:
    Little_Thread_Helper(): m_Blocker( NULL ), m_Error( NULL ), m_Needed_Time( 0 ) {};

    bool* m_Blocker;
    uint32_t* m_Error;
    double m_Needed_Time;
};


template< uint32_t Buf_Size, uint32_t Loops >
static bool Intern_Mem_DA_Test()
{
    try
    {
    	for( uint32_t i=0; i < Loops; ++i)
    	{
	#define Test delete new uint8_t[Buf_Size]; //How to prevent for out optminizing?
	    Test Test Test Test Test
	    Test Test Test Test Test
	#undef Test
	}
    }
    catch( const std::bad_alloc& ){ return false; };
    return true;
}


template< const uint32_t Buf_Size, const uint32_t Loops >
static ThreadFunction( Memory_DA_Test_Thread, Par )
{
    Little_Thread_Helper* MyHelper = reinterpret_cast< Little_Thread_Helper* > ( Par );
    
    while( *MyHelper->m_Blocker ) usleep( 1 ); //Agains a strange bug!
    
    if( !Intern_Mem_DA_Test< Buf_Size, Loops >() )
    	*MyHelper->m_Error = Benchmark::Mem_Alloc;
    
    MyHelper->m_Needed_Time = Stopwatch::Get_Time();
    ThreadExit( NULL );    
}

template< const uint32_t Buf_Size, const uint32_t Loops >
Benchmark::Test_Re_Type Benchmark::Memory_DA_Test()
{
    Stopwatch watch;
    double Comp_Time(0);
  
    if( !::Force_No_Threads )
    {
	bool Blocker( true );
        
	Smart_Array_Pointer< ThreadHandle > Thread_Mem( Thread_Number );
	if( !Thread_Mem )
	{
	    m_Error = Mem_Alloc;
	    return 0;
	}
        ThreadHandle* Threads = Thread_Mem.Get_Mem();

	uint8_t i( 0 );
	
	Smart_Array_Pointer< Little_Thread_Helper > Thread_Helper_Mem( Thread_Number );
	if( !Thread_Helper_Mem )
	{
	    m_Error = Mem_Alloc;
	    return 0;
	}
	Little_Thread_Helper* Thread_Helpers = Thread_Helper_Mem.Get_Mem();
	for( ; i < Thread_Number; ++i )
	    Thread_Helpers[i].m_Blocker = &Blocker; //That isn't that nice...but needed
	        
	
	for( i=0; i < Thread_Number; ++i )
	     ThreadCreate( Threads[i], Memory_DA_Test_Thread<Buf_Size,Loops>, &Thread_Helpers[i]  );


	//Now it's starts!
	const double Start_Time = Stopwatch::Get_Time();
	Blocker = false;
	ThreadWaitFor( Threads, Thread_Number );
	
	if( m_Error )
	    return 0;
	for( i=0; i < Thread_Number; ++i )
	    if( Comp_Time < Thread_Helpers[i].m_Needed_Time - Start_Time )
	        Comp_Time = Thread_Helpers[i].m_Needed_Time - Start_Time;
	
	
	return ((Loops*Thread_Number) / Comp_Time)/1000;
    }
    else
    {
	watch.Start();
	
	if( !Intern_Mem_DA_Test< Buf_Size, Loops > () )
	{
	    m_Error = Benchmark::Mem_Alloc; 
	    return 0;
	}

	watch.Stop( &Comp_Time );
	return (Loops / Comp_Time)/1000;
    }
}


template< const uint32_t Loops >
static void Intern_MultiT_MLuL_Test( ThreadMutex*const MyMutex )
{
#if IsLin
    #define Test pthread_mutex_lock( MyMutex ); \
	    pthread_mutex_unlock( MyMutex );
#elif IsWin
    #define Test EnterCriticalSection( MyMutex ); \
                 LeaveCriticalSection( MyMutex );
#else 
    #define Test
#endif
    for( uint32_t i=0; i < Loops; ++i )
    {
        Test Test Test Test Test
	Test Test Test Test Test
    }
    #undef Test
}


class Special_Thread_Helper
{
public:
    Special_Thread_Helper() : m_Mutex( NULL ), m_Needed_Time( 0 ){};

    bool* m_Blocker;
    ThreadMutex* m_Mutex;
    double m_Needed_Time;
};


template< const uint32_t Loops >
static ThreadFunction( MultiT_MLuL_Test_Thread, Par )
{
    Special_Thread_Helper* MyHelper = reinterpret_cast< Special_Thread_Helper* > ( Par );
    ThreadMutex*const MyMutex = MyHelper->m_Mutex;
        
    while( *MyHelper->m_Blocker ) usleep(1); //Agains a strange bug!
    
    Intern_MultiT_MLuL_Test< Loops >( MyMutex );

    MyHelper->m_Needed_Time = Stopwatch::Get_Time();
    ThreadExit( NULL );    
}

	
#if IsLin

    class Little_Mutex_Helper //under linux we don't need a mutex destroy...but somewhere else we might need, so we should save it up!
    {
    public:
        Little_Mutex_Helper() { pthread_mutexattr_t attr;
	                  pthread_mutexattr_init( &attr );
	                  //pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_FAST_NP ); how we could do it than?
	                  pthread_mutex_init( &m_Mutex, &attr );
	                  pthread_mutexattr_destroy( &attr ); };
	                        
	~Little_Mutex_Helper(){ pthread_mutex_unlock( &m_Mutex );
	    	          pthread_mutex_destroy( &m_Mutex ); }
	
	pthread_mutex_t m_Mutex;
    };
#endif

template< const uint32_t Loops >
Benchmark::Test_Re_Type Benchmark::MultiT_MLuL_Test()
{
    Stopwatch watch;
    double Comp_Time( 0 );

    if( !::Force_No_Threads )
    {
	bool Blocker( true );
	uint8_t i( 0 );

	Smart_Array_Pointer< ThreadHandle > Thread_Mem( Thread_Number );
	ThreadHandle*const Threads = Thread_Mem.Get_Mem();
    #if IsLin
	Smart_Array_Pointer< Little_Mutex_Helper > Mutex_Mem( Thread_Number );
	if( !Thread_Mem.Is_Allocated()   ||   !Mutex_Mem.Is_Allocated()  )
	{
	    m_Error = Mem_Alloc;
	    return 0;
	}
	Little_Mutex_Helper*const Mutex = Mutex_Mem.Get_Mem();
	
    #elif IsWin
    	Smart_Array_Pointer< CRITICAL_SECTION > CS_Mem( Thread_Number );
        CRITICAL_SECTION*const CS = CS_Mem.Get_Mem();
	for( ; i < Thread_Number; ++i )
	    InitializeCriticalSection( &CS[i] );
    #endif
	
	//Preallocates us the memory needed...
	Smart_Array_Pointer< Special_Thread_Helper > Thread_Helpers_Mem( Thread_Number );
	if( !Thread_Helpers_Mem )
	{
	    m_Error = Mem_Alloc;
	    return 0;
	}
	Special_Thread_Helper* Thread_Helpers = Thread_Helpers_Mem.Get_Mem();
	
	for( i =0; i < Thread_Number; ++i )
	{
	    Thread_Helpers[i].m_Blocker = &Blocker; //That isn't that nice...but needed
	    Thread_Helpers[i].m_Mutex =
	    #if IsLin
	        &Mutex[i].m_Mutex
	    #elif IsWin
		&CS[i]
	    #endif
		;
	}
	        
	
	for( i=0; i < Thread_Number; ++i )
	    ThreadCreate( Threads[i], MultiT_MLuL_Test_Thread< Loops >, &Thread_Helpers[i] );


	//Now it's starts!
	//Maybe we should wait a moment, so every
	double Start_Time = Stopwatch::Get_Time();
	Blocker = false;
	ThreadWaitFor( Threads, Thread_Number );

	for( i=0; i < Thread_Number; ++i )
	    if( Comp_Time < Thread_Helpers[i].m_Needed_Time - Start_Time )
	        Comp_Time = Thread_Helpers[i].m_Needed_Time - Start_Time;
	
	
	return ((Loops*Thread_Number) / Comp_Time)/1000;
    }
    else
    { 
    #if IsLin
	Little_Mutex_Helper Mutex;
	
	watch.Start();
	Intern_MultiT_MLuL_Test< Loops > ( &Mutex.m_Mutex );
    #elif IsWin
	CRITICAL_SECTION CS; //Is critical section relly the right choice?
	InitializeCriticalSection( &CS );
    
	watch.Start();
	Intern_MultiT_MLuL_Test< Loops > ( &CS );
    #endif
    
	watch.Stop( &Comp_Time );	
	
	return (Loops / Comp_Time)/1000;
    }
}


template< typename Type, const uint32_t Size >
class Thread_Helper
{
public:
    Thread_Helper() : m_Mem ( Size ), m_Blocker( NULL ), m_Needed_Time( 0 ) {};
 //   ~Thread_Helper() {};

    Smart_Array_Pointer< Type > m_Mem;
    const bool* m_Blocker;

    double m_Needed_Time;
};

template< const uint8_t Block_Size, const uint32_t Loops >
static void Intern_Memory_RW_Test( uint8_t* Mem_Block1, uint8_t* Mem_Block2)
{
    for( uint32_t i=0; i < Loops; ++i)
    {
    #define Test memcpy( Mem_Block1, Mem_Block2, Block_Size ); \
                 memset( Mem_Block2, i, Block_Size );
    	Test Test Test Test Test
	Test Test Test Test Test
     #undef Test
    }
}


template< const uint32_t Block_Size, const uint32_t Loops >
static ThreadFunction( Memory_RW_Test_Thread, Par )
{
    Thread_Helper< uint8_t, (Block_Size<<1) >* MyHelper = reinterpret_cast< Thread_Helper< uint8_t, (Block_Size<<1) >* > (Par);
    uint8_t*const Mem_Block1( MyHelper->m_Mem.Get_Mem() ),
           *const Mem_Block2( MyHelper->m_Mem.Get_Mem() + (Block_Size) );

    while( *MyHelper->m_Blocker ) usleep(1); //Agains a strange bug!
    
    Intern_Memory_RW_Test< Block_Size, Loops >( Mem_Block1, Mem_Block2 );
    
    MyHelper->m_Needed_Time = Stopwatch::Get_Time();
    ThreadExit( NULL );
}



template< const uint32_t Buf_Size, const uint32_t Loops >
Benchmark::Test_Re_Type Benchmark::Memory_RW_Test()
{
    Stopwatch watch;
    double Comp_Time( 0 );
    static const uint32_t Half_Buf_Size( Buf_Size>>1 );
	
    if( !::Force_No_Threads )
    {
	bool Blocker( true );
        
	Smart_Array_Pointer< ThreadHandle > Thread_Mem( Thread_Number );
	if( !Thread_Mem )
	{
	    m_Error = Mem_Alloc;
	    return 0;
	}
	
        ThreadHandle* Threads = Thread_Mem;

	uint8_t i( 0 );
	
	
	Smart_Array_Pointer< Thread_Helper< uint8_t, Buf_Size > > Thread_Helper_Mem( Thread_Number );
	if( !Thread_Helper_Mem  || !Thread_Helper_Mem.Get_Mem()->m_Mem.Is_Allocated()  ) //uah...evil line :x
	{
	    m_Error = Mem_Alloc;
	    return 0;
	}
	Thread_Helper< uint8_t, Buf_Size >* Thread_Helpers = Thread_Helper_Mem.Get_Mem();
	for( ; i < Thread_Number; ++i )
	    Thread_Helpers[i].m_Blocker = &Blocker; //That isn't that nice...but needed
	        
	
	for( i=0; i < Thread_Number; ++i )
	    ThreadCreate( Threads[i], Memory_RW_Test_Thread< Half_Buf_Size, Loops >, &Thread_Helpers[i] );


	//Now it's starts!
	//Maybe we should wait a moment, so every
	//watch.Start();
	const double Start_Time = Stopwatch::Get_Time();
	Blocker = false;
	ThreadWaitFor( Threads, Thread_Number );
	
	//watch.Stop( &Comp_Time );
	for( i=0; i < Thread_Number; ++i )
	    if( Comp_Time < Thread_Helpers[i].m_Needed_Time - Start_Time )
	        Comp_Time = Thread_Helpers[i].m_Needed_Time - Start_Time;
	
	
	return ((Loops*Thread_Number) / Comp_Time)/1000;
    }
    else
    {	
	Smart_Array_Pointer< uint8_t > a( Half_Buf_Size );
	Smart_Array_Pointer< uint8_t > b( Half_Buf_Size );
	if( !a  ||  !b )
	{
	    m_Error = Mem_Alloc;
	    return 0;
	}
	
	watch.Start();
	Intern_Memory_RW_Test< Half_Buf_Size, Loops > ( a.Get_Mem(), b.Get_Mem() );
	watch.Stop( &Comp_Time );
		
	
	return (Loops / Comp_Time)/1000;
    }
}





template< typename Type, const uint32_t Loops >
static void Intern_Calculation_Test( Type*const Buffer )
{
    for( Type i =0 ; i < Loops; ++i )
    {
	for( Type j=0; j < Loops; ++j) //maybe make some use of j to prevent of out optimizing something?!
	{
	#define Test Buffer[static_cast< uint32_t >(i+1)] =  \
		Buffer[static_cast< uint32_t >(i)]   *       \
		Buffer[static_cast< uint32_t >(i+1)];        \
		                                             \
	    Buffer[static_cast< uint32_t >(i)] += 3;         \
		                                             \
	    if( !Buffer[static_cast< uint32_t >(i)] )        \
	         Buffer[static_cast< uint32_t >(i)] +=3;     \
	    Buffer[static_cast< uint32_t >(i+1)] /=          \
		Buffer[static_cast< uint32_t >(i)];          \
		                                             \
	    Buffer[static_cast< uint32_t >(i+1)] -= 5;       \
		                                             \
	    Buffer[static_cast< uint32_t >(i+1)] +=          \
		Buffer[static_cast< uint32_t >(i)]   *       \
		Buffer[static_cast< uint32_t >(i)];          \
		                                             \
	    Buffer[static_cast< uint32_t >(i)] *=            \
		Buffer[static_cast< uint32_t >(i+1)];        \
		                                             \
		                                             \
	    if( !Buffer[static_cast< uint32_t >(i)] )        \
		     Buffer[static_cast< uint32_t >(i)] -=7; \
		                                             \
	    Buffer[static_cast< uint32_t >(i+1)] /=          \
		Buffer[static_cast< uint32_t >(i)];          \
		                                             \
	    Buffer[static_cast< uint32_t >(i)] -=            \
		Buffer[static_cast< uint32_t >(i+1)];        \
		                                             \
	    if( !Buffer[static_cast< uint32_t >(i)] )        \
		 ++Buffer[static_cast< uint32_t >(i)];       \
		                                             \
	    Buffer[static_cast< uint32_t >(i)] *= 5;         \
	    Buffer[static_cast< uint32_t >(i)] /= 3;         \
		                                             \
	    Buffer[static_cast< uint32_t >(i+1)] *=          \
		-Buffer[static_cast< uint32_t >(i)];         
	    
	    Test Test Test Test Test
	    Test Test Test Test Test
	#undef Test
	}
    }

}


template< typename Type, const uint32_t Loops >
static ThreadFunction( Calculation_Test_Thread, Par )
{
    Thread_Helper< Type, Loops>* MyHelper( reinterpret_cast< Thread_Helper< Type, Loops >* >( Par ) );

    Type* Buffer = MyHelper->m_Mem.Get_Mem();
        
    while( *MyHelper->m_Blocker ) usleep(1); //Agains a strange bug!

    Intern_Calculation_Test< Type, Loops-1> ( Buffer );
    
    MyHelper->m_Needed_Time = Stopwatch::Get_Time();
    ThreadExit( NULL );
}

template< typename Type, const uint32_t Loops >
Benchmark::Test_Re_Type Benchmark::Calculation_Test()
{
    Stopwatch watch;
    double Comp_Time(0);
    
    if( !::Force_No_Threads )
    {
	bool Blocker( true );
        
	Smart_Array_Pointer< ThreadHandle > Threads_Mem( Thread_Number );
	if( !Threads_Mem )
	{
	    m_Error = Mem_Alloc;
	    return 0;
	}
	ThreadHandle* Threads = Threads_Mem.Get_Mem();

	    uint8_t i( 0 );
	
	    //Preallocates us the memory needed..
	    Smart_Array_Pointer< Thread_Helper< Type, Loops > > Thread_Helpers_Mem( Thread_Number );
	    if( !Thread_Helpers_Mem )
	    {
		m_Error = Mem_Alloc;
		return 0;
	    } 
	    Thread_Helper< Type, Loops >* Thread_Helpers = Thread_Helpers_Mem.Get_Mem();
	    for( ; i < Thread_Number; ++i )
		 Thread_Helpers[i].m_Blocker = &Blocker; //That isn't that nice...but needed
	        
	
	    for( i=0; i < Thread_Number; ++i )
		ThreadCreate( Threads[i], Calculation_Test_Thread< Type, Loops >, &Thread_Helpers[i] );

	    //Now it's starts!
	    //Maybe we should wait a moment, so every
	    //watch.Start();
	    const double Start_Time = Stopwatch::Get_Time();
	    Blocker = false;
	    ThreadWaitFor( Threads, Thread_Number );
	    
	    for( i=0; i < Thread_Number; ++i )
		if( Comp_Time < Thread_Helpers[i].m_Needed_Time - Start_Time )
		    Comp_Time = Thread_Helpers[i].m_Needed_Time - Start_Time;
		
	
	    double Score = static_cast< double > (  ((Loops-1)*(Loops-1)*Thread_Number) / Comp_Time);
	    return Score / 1000; //A little score modi :)
	}
	else
	{ 
	    Smart_Array_Pointer< Type > Buffer_Mem( Loops );
	    if( !Buffer_Mem )
	    {
		m_Error = Mem_Alloc;
		return 0;
	    }
	    Type* Buffer = Buffer_Mem.Get_Mem(); //Does we need some extra to prevent for buffer over/under flows?
	
	    watch.Start();
	    Intern_Calculation_Test< Type, Loops-1 > ( Buffer );
	    watch.Stop( &Comp_Time );
	
	
	    double Score = static_cast< double > (  ((Loops-1)*(Loops-1)) / Comp_Time);
	    return Score / 1000; //Little score modi!
	}
}

