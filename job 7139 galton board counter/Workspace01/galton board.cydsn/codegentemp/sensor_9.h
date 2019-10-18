/*******************************************************************************
* File Name: sensor_9.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_sensor_9_H) /* Pins sensor_9_H */
#define CY_PINS_sensor_9_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "sensor_9_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 sensor_9__PORT == 15 && ((sensor_9__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    sensor_9_Write(uint8 value);
void    sensor_9_SetDriveMode(uint8 mode);
uint8   sensor_9_ReadDataReg(void);
uint8   sensor_9_Read(void);
void    sensor_9_SetInterruptMode(uint16 position, uint16 mode);
uint8   sensor_9_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the sensor_9_SetDriveMode() function.
     *  @{
     */
        #define sensor_9_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define sensor_9_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define sensor_9_DM_RES_UP          PIN_DM_RES_UP
        #define sensor_9_DM_RES_DWN         PIN_DM_RES_DWN
        #define sensor_9_DM_OD_LO           PIN_DM_OD_LO
        #define sensor_9_DM_OD_HI           PIN_DM_OD_HI
        #define sensor_9_DM_STRONG          PIN_DM_STRONG
        #define sensor_9_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define sensor_9_MASK               sensor_9__MASK
#define sensor_9_SHIFT              sensor_9__SHIFT
#define sensor_9_WIDTH              1u

/* Interrupt constants */
#if defined(sensor_9__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in sensor_9_SetInterruptMode() function.
     *  @{
     */
        #define sensor_9_INTR_NONE      (uint16)(0x0000u)
        #define sensor_9_INTR_RISING    (uint16)(0x0001u)
        #define sensor_9_INTR_FALLING   (uint16)(0x0002u)
        #define sensor_9_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define sensor_9_INTR_MASK      (0x01u) 
#endif /* (sensor_9__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define sensor_9_PS                     (* (reg8 *) sensor_9__PS)
/* Data Register */
#define sensor_9_DR                     (* (reg8 *) sensor_9__DR)
/* Port Number */
#define sensor_9_PRT_NUM                (* (reg8 *) sensor_9__PRT) 
/* Connect to Analog Globals */                                                  
#define sensor_9_AG                     (* (reg8 *) sensor_9__AG)                       
/* Analog MUX bux enable */
#define sensor_9_AMUX                   (* (reg8 *) sensor_9__AMUX) 
/* Bidirectional Enable */                                                        
#define sensor_9_BIE                    (* (reg8 *) sensor_9__BIE)
/* Bit-mask for Aliased Register Access */
#define sensor_9_BIT_MASK               (* (reg8 *) sensor_9__BIT_MASK)
/* Bypass Enable */
#define sensor_9_BYP                    (* (reg8 *) sensor_9__BYP)
/* Port wide control signals */                                                   
#define sensor_9_CTL                    (* (reg8 *) sensor_9__CTL)
/* Drive Modes */
#define sensor_9_DM0                    (* (reg8 *) sensor_9__DM0) 
#define sensor_9_DM1                    (* (reg8 *) sensor_9__DM1)
#define sensor_9_DM2                    (* (reg8 *) sensor_9__DM2) 
/* Input Buffer Disable Override */
#define sensor_9_INP_DIS                (* (reg8 *) sensor_9__INP_DIS)
/* LCD Common or Segment Drive */
#define sensor_9_LCD_COM_SEG            (* (reg8 *) sensor_9__LCD_COM_SEG)
/* Enable Segment LCD */
#define sensor_9_LCD_EN                 (* (reg8 *) sensor_9__LCD_EN)
/* Slew Rate Control */
#define sensor_9_SLW                    (* (reg8 *) sensor_9__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define sensor_9_PRTDSI__CAPS_SEL       (* (reg8 *) sensor_9__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define sensor_9_PRTDSI__DBL_SYNC_IN    (* (reg8 *) sensor_9__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define sensor_9_PRTDSI__OE_SEL0        (* (reg8 *) sensor_9__PRTDSI__OE_SEL0) 
#define sensor_9_PRTDSI__OE_SEL1        (* (reg8 *) sensor_9__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define sensor_9_PRTDSI__OUT_SEL0       (* (reg8 *) sensor_9__PRTDSI__OUT_SEL0) 
#define sensor_9_PRTDSI__OUT_SEL1       (* (reg8 *) sensor_9__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define sensor_9_PRTDSI__SYNC_OUT       (* (reg8 *) sensor_9__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(sensor_9__SIO_CFG)
    #define sensor_9_SIO_HYST_EN        (* (reg8 *) sensor_9__SIO_HYST_EN)
    #define sensor_9_SIO_REG_HIFREQ     (* (reg8 *) sensor_9__SIO_REG_HIFREQ)
    #define sensor_9_SIO_CFG            (* (reg8 *) sensor_9__SIO_CFG)
    #define sensor_9_SIO_DIFF           (* (reg8 *) sensor_9__SIO_DIFF)
#endif /* (sensor_9__SIO_CFG) */

/* Interrupt Registers */
#if defined(sensor_9__INTSTAT)
    #define sensor_9_INTSTAT            (* (reg8 *) sensor_9__INTSTAT)
    #define sensor_9_SNAP               (* (reg8 *) sensor_9__SNAP)
    
	#define sensor_9_0_INTTYPE_REG 		(* (reg8 *) sensor_9__0__INTTYPE)
#endif /* (sensor_9__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_sensor_9_H */


/* [] END OF FILE */
