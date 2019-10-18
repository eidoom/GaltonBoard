/*******************************************************************************
* File Name: isr_8.h
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
#if !defined(CY_ISR_isr_8_H)
#define CY_ISR_isr_8_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void isr_8_Start(void);
void isr_8_StartEx(cyisraddress address);
void isr_8_Stop(void);

CY_ISR_PROTO(isr_8_Interrupt);

void isr_8_SetVector(cyisraddress address);
cyisraddress isr_8_GetVector(void);

void isr_8_SetPriority(uint8 priority);
uint8 isr_8_GetPriority(void);

void isr_8_Enable(void);
uint8 isr_8_GetState(void);
void isr_8_Disable(void);

void isr_8_SetPending(void);
void isr_8_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the isr_8 ISR. */
#define isr_8_INTC_VECTOR            ((reg32 *) isr_8__INTC_VECT)

/* Address of the isr_8 ISR priority. */
#define isr_8_INTC_PRIOR             ((reg8 *) isr_8__INTC_PRIOR_REG)

/* Priority of the isr_8 interrupt. */
#define isr_8_INTC_PRIOR_NUMBER      isr_8__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable isr_8 interrupt. */
#define isr_8_INTC_SET_EN            ((reg32 *) isr_8__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the isr_8 interrupt. */
#define isr_8_INTC_CLR_EN            ((reg32 *) isr_8__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the isr_8 interrupt state to pending. */
#define isr_8_INTC_SET_PD            ((reg32 *) isr_8__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the isr_8 interrupt. */
#define isr_8_INTC_CLR_PD            ((reg32 *) isr_8__INTC_CLR_PD_REG)


#endif /* CY_ISR_isr_8_H */


/* [] END OF FILE */
