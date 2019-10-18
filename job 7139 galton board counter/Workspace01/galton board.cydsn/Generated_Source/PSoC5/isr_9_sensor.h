/*******************************************************************************
* File Name: isr_9_sensor.h
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
#if !defined(CY_ISR_isr_9_sensor_H)
#define CY_ISR_isr_9_sensor_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void isr_9_sensor_Start(void);
void isr_9_sensor_StartEx(cyisraddress address);
void isr_9_sensor_Stop(void);

CY_ISR_PROTO(isr_9_sensor_Interrupt);

void isr_9_sensor_SetVector(cyisraddress address);
cyisraddress isr_9_sensor_GetVector(void);

void isr_9_sensor_SetPriority(uint8 priority);
uint8 isr_9_sensor_GetPriority(void);

void isr_9_sensor_Enable(void);
uint8 isr_9_sensor_GetState(void);
void isr_9_sensor_Disable(void);

void isr_9_sensor_SetPending(void);
void isr_9_sensor_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the isr_9_sensor ISR. */
#define isr_9_sensor_INTC_VECTOR            ((reg32 *) isr_9_sensor__INTC_VECT)

/* Address of the isr_9_sensor ISR priority. */
#define isr_9_sensor_INTC_PRIOR             ((reg8 *) isr_9_sensor__INTC_PRIOR_REG)

/* Priority of the isr_9_sensor interrupt. */
#define isr_9_sensor_INTC_PRIOR_NUMBER      isr_9_sensor__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable isr_9_sensor interrupt. */
#define isr_9_sensor_INTC_SET_EN            ((reg32 *) isr_9_sensor__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the isr_9_sensor interrupt. */
#define isr_9_sensor_INTC_CLR_EN            ((reg32 *) isr_9_sensor__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the isr_9_sensor interrupt state to pending. */
#define isr_9_sensor_INTC_SET_PD            ((reg32 *) isr_9_sensor__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the isr_9_sensor interrupt. */
#define isr_9_sensor_INTC_CLR_PD            ((reg32 *) isr_9_sensor__INTC_CLR_PD_REG)


#endif /* CY_ISR_isr_9_sensor_H */


/* [] END OF FILE */
