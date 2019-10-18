/*******************************************************************************
* File Name: serial_in_buff.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_serial_in_buff_H)
#define CY_ISR_serial_in_buff_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void serial_in_buff_Start(void);
void serial_in_buff_StartEx(cyisraddress address);
void serial_in_buff_Stop(void);

CY_ISR_PROTO(serial_in_buff_Interrupt);

void serial_in_buff_SetVector(cyisraddress address);
cyisraddress serial_in_buff_GetVector(void);

void serial_in_buff_SetPriority(uint8 priority);
uint8 serial_in_buff_GetPriority(void);

void serial_in_buff_Enable(void);
uint8 serial_in_buff_GetState(void);
void serial_in_buff_Disable(void);

void serial_in_buff_SetPending(void);
void serial_in_buff_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the serial_in_buff ISR. */
#define serial_in_buff_INTC_VECTOR            ((reg32 *) serial_in_buff__INTC_VECT)

/* Address of the serial_in_buff ISR priority. */
#define serial_in_buff_INTC_PRIOR             ((reg8 *) serial_in_buff__INTC_PRIOR_REG)

/* Priority of the serial_in_buff interrupt. */
#define serial_in_buff_INTC_PRIOR_NUMBER      serial_in_buff__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable serial_in_buff interrupt. */
#define serial_in_buff_INTC_SET_EN            ((reg32 *) serial_in_buff__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the serial_in_buff interrupt. */
#define serial_in_buff_INTC_CLR_EN            ((reg32 *) serial_in_buff__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the serial_in_buff interrupt state to pending. */
#define serial_in_buff_INTC_SET_PD            ((reg32 *) serial_in_buff__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the serial_in_buff interrupt. */
#define serial_in_buff_INTC_CLR_PD            ((reg32 *) serial_in_buff__INTC_CLR_PD_REG)


#endif /* CY_ISR_serial_in_buff_H */


/* [] END OF FILE */
