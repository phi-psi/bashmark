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

#ifndef _Benchmark_hpp_
#define _Benchmark_hpp_

#include <inttypes.h>
#include "Main.hpp"
#include "Thread.hpp"

namespace Geno
{

//We need a big cleanup :x
//We need an error code or something for the debug mode...so I can see why he couldn't geht the info!
class Benchmark
{
private:
    typedef uint32_t Score_Type ;
    typedef uint32_t Score_Re_Type;
    typedef double   Test_Re_Type;
public:
    enum Error{None=0,Mem_Alloc, Thread_Creation, Mutex_Creation};

    Benchmark();
    
    
    void Run();
    
    
    inline Score_Re_Type Get_Integer8_Score() const;
    inline Score_Re_Type Get_Integer16_Score() const;
    inline Score_Re_Type Get_Integer32_Score() const;
    inline Score_Re_Type Get_Integer64_Score() const;
	
    inline Score_Re_Type Get_Fp32_Score() const;
    inline Score_Re_Type Get_Fp64_Score() const;
    inline Score_Re_Type Get_Fp96_Score() const;
    inline Score_Re_Type Get_Fp128_Score() const;
	
	
    inline Score_Re_Type Get_Mem_RW2_Score() const;
    inline Score_Re_Type Get_Mem_DA2_Score() const;
    inline Score_Re_Type Get_Mem_RW4_Score() const;
    inline Score_Re_Type Get_Mem_DA4_Score() const;
    inline Score_Re_Type Get_Mem_DA8_Score() const;
	
	
    inline Score_Re_Type Get_MT_MLuL_Score() const;


	
	

    void Integer8_Test();
    void Integer16_Test();
    void Integer32_Test();
    void Integer64_Test();
	
	
    void Fp32_Test();
    void Fp64_Test();
    void Fp96_Test();
    void Fp128_Test();
	
	
    void Mem_RW2_Test();
    void Mem_RW4_Test();
    void Mem_DA2_Test();
    void Mem_DA4_Test();
    void Mem_DA8_Test();
 
    void MT_MLuL_Test();

    
    uint32_t Get_Error()const {return m_Error;}; //Maybe reset it?


    static const uint8_t Time_Per_Test;
    static const uint8_t Number_Cpu_Int_Test;
    static const uint8_t Number_Cpu_Fp_Test;
    
    static const uint8_t Number_Mem_RW_Test;
    static const uint8_t Number_Mem_DA_Test;

    static const uint8_t Number_MT_MLuL_Test;

	
private:
    template< typename, const uint32_t >
    static Test_Re_Type Calculation_Test();
    
    
    template< const uint32_t, const uint32_t >
    static Test_Re_Type Memory_RW_Test();
    
    template< const uint32_t, const uint32_t >
    static Test_Re_Type Memory_DA_Test();
    
    
    template< const uint32_t >
    static Test_Re_Type MultiT_MLuL_Test();

    
    
    
    Score_Type m_Integer8_Score,
               m_Integer16_Score,
               m_Integer32_Score,
               m_Integer64_Score,
    
    
               m_Fp32_Score,
               m_Fp64_Score,
               m_Fp96_Score,
               m_Fp128_Score,
               
               
               m_Mem_RW2_Score,
               m_Mem_RW4_Score,
               m_Mem_DA2_Score,
               m_Mem_DA4_Score,
               m_Mem_DA8_Score,
               
               
               m_MT_MLuL_Score;
               
               
    static const uint8_t Status_Mul; //100/Time_Per_Test!Just to prevent for unneeded calculations!
    

    
    static Test_Re_Type Intern_Integer8_Test();
    static Test_Re_Type Intern_Integer16_Test();
    static Test_Re_Type Intern_Integer32_Test();
    static Test_Re_Type Intern_Integer64_Test();
	
	
    static Test_Re_Type Intern_Fp32_Test();
    static Test_Re_Type Intern_Fp64_Test();
    static Test_Re_Type Intern_Fp96_Test();
    static Test_Re_Type Intern_Fp128_Test();
	
	
    static Test_Re_Type Intern_Mem_RW2_Test();
    static Test_Re_Type Intern_Mem_RW4_Test();	
	
    static Test_Re_Type Intern_Mem_DA2_Test();
    static Test_Re_Type Intern_Mem_DA4_Test();
    static Test_Re_Type Intern_Mem_DA8_Test();
	
	
    static Test_Re_Type Intern_MT_MLuL_Test();
   

    static uint8_t Thread_Number; //Will be the used thread number, dependent on the number of cpus!

    static uint32_t m_Error; //hmm..
};


Benchmark::Score_Re_Type Benchmark::Get_Integer32_Score() const
{
    return m_Integer32_Score;
}
Benchmark::Score_Re_Type Benchmark::Get_Integer64_Score() const
{
    return m_Integer64_Score;
}
Benchmark::Score_Re_Type Benchmark::Get_Integer16_Score() const
{
    return m_Integer16_Score;
}
Benchmark::Score_Re_Type Benchmark::Get_Integer8_Score() const
{
    return m_Integer8_Score;
}
Benchmark::Score_Re_Type Benchmark::Get_Fp32_Score() const
{
    return m_Fp32_Score;
}
Benchmark::Score_Re_Type Benchmark::Get_Fp64_Score() const
{
    return m_Fp64_Score;
}
Benchmark::Score_Re_Type Benchmark::Get_Fp96_Score() const
{
    return m_Fp96_Score;
}
Benchmark::Score_Re_Type Benchmark::Get_Fp128_Score() const
{
    return m_Fp128_Score;
}
Benchmark::Score_Re_Type Benchmark::Get_Mem_RW2_Score() const
{
    return m_Mem_RW2_Score;
}
Benchmark::Score_Re_Type Benchmark::Get_Mem_DA2_Score() const
{
    return m_Mem_DA2_Score;
}
Benchmark::Score_Re_Type Benchmark::Get_Mem_RW4_Score() const
{
    return m_Mem_RW4_Score;
}
Benchmark::Score_Re_Type Benchmark::Get_Mem_DA4_Score() const
{
    return m_Mem_DA4_Score;
}
Benchmark::Score_Re_Type Benchmark::Get_Mem_DA8_Score() const
{
    return m_Mem_DA8_Score;
}
Benchmark::Score_Re_Type Benchmark::Get_MT_MLuL_Score() const
{
    return m_MT_MLuL_Score;
}




}

#endif
