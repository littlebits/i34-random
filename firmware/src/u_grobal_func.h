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

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"

#include "r_cg_userdefine.h"

/* common functions declare */
void		u_g_param_Init(void);

char		get_outputType(void);
void		set_outputType(char c);

uint16_t	get_input(void);
void		set_input(uint16_t u);
