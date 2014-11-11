/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2013 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_timer_user.c
* Version      : CodeGenerator for RL78/G12 V2.00.00.07 [22 Feb 2013]
* Device(s)    : R5F10278
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for TAU module.
* Creation Date: 2013/06/04
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt INTTM00 r_tau0_channel0_interrupt
#pragma interrupt INTTM01 r_tau0_channel1_interrupt
#pragma interrupt INTTM02 r_tau0_channel2_interrupt
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
#include "u_grobal_func.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
static char	g_count_sw_off;
static char	g_count_sw_on;

static uint16_t g_output;
static uint16_t g_sh_output;

static uint16_t g_noise;
static uint16_t g_sh_value;

static uint16_t g_t_input;

static uint16_t g_env_input;
static uint16_t g_attack_coef;
static uint16_t g_decay_coef;

static char	g_input_up;
static char	g_bef_input_off;
static char	g_hold;
static char	g_count_input_on;
static char	g_count_input_off;

static uint32_t g_msignal;
static uint32_t g_msig_pos;

void u_make_output(void);
void u_make_noise(void);

void u_env_follower(uint16_t l_input);
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_tau0_channel0_interrupt
* Description  : This function is INTTM00 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	uint32_t l_output_in0;

	TDR01 = g_output;

	u_make_noise();
	
	switch(get_outputType()){
	case NOISE:	l_output_in0 = (uint32_t)g_noise * (uint32_t)g_t_input;
			l_output_in0 = l_output_in0 >> 10;
			g_output = (uint16_t)l_output_in0;
			break;
	case S_H:	g_output = g_sh_output;
			break;
	default:	break;
	}
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel1_interrupt
* Description  : This function is INTTM01 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel1_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel2_interrupt
* Description  : This function is INTTM02 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_tau0_channel2_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
     	uint16_t	temp_result;
	uint32_t	l_output;
	
	if( P0.0 == NOISE && get_outputType() == S_H ){
		g_count_sw_on = 0;
		g_count_sw_off++;
		if( g_count_sw_off == 3 ){
			set_outputType(NOISE);
			g_count_sw_off = 0;
		}
	}else if( P0.0 != NOISE && get_outputType() == NOISE ){
		g_count_sw_off = 0;
		g_count_sw_on++;
		if( g_count_sw_on == 3 ){
			set_outputType(S_H);
			g_count_sw_on = 0;
			g_output = 0x0;
		}
	}
	
	if(ADIF == 1){
		ADIF = 0;
		R_ADC_Get_Result(&temp_result);
		R_ADC_Start();
		
		if(temp_result < 15){
			set_input(0);
		}else{
			set_input(temp_result);
		}
		
		DI();
		g_t_input = get_input();
		EI();

		if(g_t_input > 50){
			if(g_bef_input_off > 0){
				g_input_up = 1;
				g_bef_input_off = 0;
			}
		}else{
			if(g_t_input < 40){
				g_bef_input_off = 1;
			}
		}
		if(g_input_up){
			g_sh_value = (g_msignal >> 22);
			g_input_up = 0;
		}
		l_output = (uint32_t)g_sh_value * (uint32_t)g_t_input;
		l_output = l_output >> 10;
		
		DI();
		g_sh_output = (uint16_t)l_output;
		EI();
	}
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/*
  xorshift random noise generator
  based on
  http://ja.wikipedia.org/wiki/Xorshift
 */ 
void u_make_noise(void)
{
	static uint32_t	x = 123456789;
	static uint32_t	y = 362436069;
	static uint32_t	z = 521288629;
	static uint32_t	w = 88675123; 

	uint32_t	t;
 
	t = x ^ (x << 11);
	x = y;	y = z;	z = w;
	w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)); 
	
	g_msignal = w;
	if(g_msignal & g_msig_pos){
		g_noise = 0x3FF;
	}else{
		g_noise = 0x0;
	}
}
void u_timer_Init(void)
{
	g_output	= 0;
	g_noise		= 0x0;
	
	g_input_up	= 0;
	g_hold		= 0;

	g_count_input_on = 0;
	g_count_input_off= 0;
	
	g_env_input	= 0x0;
	g_attack_coef	= 0x100;
	g_decay_coef	= 0x200;
	
	g_msignal	= M_INIT;
	g_msig_pos	= 0x200;
}
/* End user code. Do not edit comment generated here */
