/************************************************************************************************************
 * Module: Systick_Timer
 *
 * File Name: Systick_Timer.c
 *
 * Description: Source file for TM4C123GH6PM Micro-controller - SysTick Timer Driver - Non-AUTOSAR Driver.
 * 				and also contains its Interrupt handler
 *
 * Author: Mohamed Tarek / Ahmed Desoky
 ************************************************************************************************************/
#include "Systick_Timer.h"

/* Global Static (Private) pointer to function used to point upper layer functions
 * to be used in Call Back */
static void (*g_SysTick_Call_Back_Ptr)(void) = NULL_PTR;
/************************************************************************************************************
* Interrupt Service Routine Name: SysTick_Handler
* Description: SysTick Timer ISR
************************************************************************************************************/
void SysTick_Handler(void)
{
    /* Check if the Timer0_setCallBack is already called */
    if(g_SysTick_Call_Back_Ptr != NULL_PTR)
    {
        (*g_SysTick_Call_Back_Ptr)(); /* call the function in the scheduler using call-back concept */
    }
    /* No need to clear the trigger flag (COUNT) bit ... it cleared automatically by the HW */
}

/*************************************************************************************************************
* Service Name: SysTick_Start
* Sync/Async: Synchronous
* Parameters (in): Time to count in millisecond
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Start SysTick Timer.
*************************************************************************************************************/
void SysTick_Start(uint16 Tick_Time)
{
    SYSTICK_CTRL_REG    = 0;                       					  /* Disable the SysTick Timer by Clear the ENABLE Bit */
    SYSTICK_RELOAD_REG  = (ITERATIONS_FOR_1MS-1) * Tick_Time;        				 /* Set the Reload value to count n miliiseconds */
    SYSTICK_CURRENT_REG = 0;                       					  /* Clear the Current Register value */
    /* Configure the SysTick Control Register
     * Enable the SysTick Timer (ENABLE = 1)
     * Enable SysTick Interrupt (INTEN = 1)
     * Choose the clock source to be System Clock (CLK_SRC = 1) */
    SYSTICK_CTRL_REG   |= 0x07;
    /* Assign priority level 3 to the SysTick Interrupt */
    NVIC_SYSTEM_PRI3_REG =  (NVIC_SYSTEM_PRI3_REG & SYSTICK_PRIORITY_MASK) | (SYSTICK_INTERRUPT_PRIORITY << SYSTICK_PRIORITY_BITS_POS);
    /* No Need to Enable SysTick Interrupt in NVIC */
}

/*************************************************************************************************************
* Service Name: SysTick_Stop
* Sync/Async: Synchronous
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Stop SysTick Timer.
*************************************************************************************************************/
void SysTick_Stop(void)
{
    SYSTICK_CTRL_REG = 0; /* Disable the SysTick Timer by Clear the ENABLE Bit */
}

/*************************************************************************************************************
* Service Name: SysTick_SetCallBack
* Sync/Async: Synchronous
* Parameters (in): Ptr2Func - Call Back function address
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Setup the SysTick Timer call back
*************************************************************************************************************/
void SysTick_SetCallBack(void(*Ptr2Func)(void))
{
    g_SysTick_Call_Back_Ptr = Ptr2Func;
}
