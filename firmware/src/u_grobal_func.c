/***********************************************************************************************************************
 * Copyright 2014 littleBits Electronics
 *
 * This file is part of i34-random.
 *
 * i34-random is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * i34-random is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License at <http://www.gnu.org/licenses/> for more details.
***********************************************************************************************************************/

#include "u_grobal_func.h"
#include <math.h>

// common parameters
static char	g_outputType;
static uint16_t g_input_signal;

// common functions  
void u_g_param_Init(void)
{
	g_outputType	= 0;
	g_input_signal	= 128;	
}
// output type /////
char get_outputType(void)
{
	return g_outputType;
}

void set_outputType(char c)
{
		g_outputType = c;
}
// input /////
uint16_t get_input(void)
{
	return g_input_signal;
}
void set_input(uint16_t u)
{
	if(g_input_signal != u){
		g_input_signal = (u + g_input_signal) >> 1;
	}
}
