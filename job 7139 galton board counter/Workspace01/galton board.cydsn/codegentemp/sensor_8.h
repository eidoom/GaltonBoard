/*******************************************************************************
* File Name: sensor_8.h  
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

#if !defined(CY_PINS_sensor_8_H) /* Pins sensor_8_H */
#define CY_PINS_sensor_8_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "sensor_8_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 sensor_8__PORT == 15 && ((sensor_8__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    sensor_8_Write(uint8 value);
void    sensor_8_SetDriveMode(uint8 mode);
uint8   sensor_8_ReadDataReg(void);
uint8   sensor_8_Read(void);
void    sensor_8_SetInterruptMode(uint16 position, uint16 mode);
uint8   sensor_8_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the sensor_8_SetDriveMode() function.
     *  @{
     */
        #define sensor_8_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define sensor_8_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define sensor_8_DM_RES_UP          PIN_DM_RES_UP
        #define sensor_8_DM_RES_DWN         PIN_DM_RES_DWN
        #define sensor_8_DM_OD_LO           PIN_DM_OD_LO
        #define sensor_8_DM_OD_HI           PIN_DM_OD_HI
        #define sensor_8_DM_STRONG          PIN_DM_STRONG
        #define sensor_8_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define sensor_8_MASK               sensor_8__MASK
#define sensor_8_SHIFT              sensor_8__SHIFT
#define sensor_8_WIDTH              1u

/* Interrupt constants */
#if defined(sensor_8__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in sensor_8_SetInterruptMode() function.
     *  @{
     */
        #define sensor_8_INTR_NONE      (uint16)(0x0000u)
        #define sensor_8_INTR_RISING    (uint16)(0x0001u)
        #define sensor_8_INTR_FALLING   (uint16)(0x0002u)
        #define sensor_8_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define sensor_8_INTR_MASK      (0x01u) 
#endif /* (sensor_8__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define sensor_8_PS                     (* (reg8 *) sensor_8__PS)
/* Data Register */
#define sensor_8_DR                     (* (reg8 *) sensor_8__DR)
/* Port Number */
#define sensor_8_PRT_NUM                (* (reg8 *) sensor_8__PRT) 
/* Connect to Analog Globals */                                                  
#define sensor_8_AG                     (* (reg8 *) sensor_8__AG)                       
/* Analog MUX bux enable */
#define sensor_8_AMUX                   (* (reg8 *) sensor_8__AMUX) 
/* Bidirectional Enable */                                                        
#define sensor_8_BIE                    (* (reg8 *) sensor_8__BIE)
/* Bit-mask for Aliased Register Access */
#define sensor_8_BIT_MASK               (* (reg8 *) sensor_8__BIT_MASK)
/* Bypass Enable */
#define sensor_8_BYP                    (* (reg8 *) sensor_8__BYP)
/* Port wide control signals */                                                   
#define sensor_8_CTL                    (* (reg8 *) sensor_8__CTL)
/* Drive Modes */
#define sensor_8_DM0                    (* (reg8 *) sensor_8__DM0) 
#define sensor_8_DM1                    (* (reg8 *) sensor_8__DM1)
#define sensor_8_DM2                    (* (reg8 *) sensor_8__DM2) 
/* Input Buffer Disable Override */
#define sensor_8_INP_DIS                (* (reg8 *) sensor_8__INP_DIS)
/* LCD Common or Segment Drive */
#define sensor_8_LCD_COM_SEG            (* (reg8 *) sensor_8__LCD_COM_SEG)
/* Enable Segment LCD */
#define sensor_8_LCD_EN                 (* (reg8 *) sensor_8__LCD_EN)
/* Slew Rate Control */
#define sensor_8_SLW                    (* (reg8 *) sensor_8__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define sensor_8_PRTDSI__CAPS_SEL       (* (reg8 *) sensor_8__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define sensor_8_PRTDSI__DBL_SYNC_IN    (* (reg8 *) sensor_8__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define sensor_8_PRTDSI__OE_SEL0        (* (reg8 *) sensor_8__PRTDSI__OE_SEL0) 
#define sensor_8_PRTDSI__OE_SEL1        (* (reg8 *) sensor_8__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define sensor_8_PRTDSI__OUT_SEL0       (* (reg8 *) sensor_8__PRTDSI__OUT_SEL0) 
#define sensor_8_PRTDSI__OUT_SEL1       (* (reg8 *) sensor_8__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define sensor_8_PRTDSI__SYNC_OUT       (* (reg8 *) sensor_8__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(sensor_8__SIO_CFG)
    #define sensor_8_SIO_HYST_EN        (* (reg8 *) sensor_8__SIO_HYST_EN)
    #define sensor_8_SIO_REG_HIFREQ     (* (reg8 *) sensor_8__SIO_REG_HIFREQ)
    #define sensor_8_SIO_CFG            (* (reg8 *) sensor_8__SIO_CFG)
    #define sensor_8_SIO_DIFF           (* (reg8 *) sensor_8__SIO_DIFF)
#endif /* (sensor_8__SIO_CFG) */

/* Interrupt Registers */
#if defined(sensor_8__INTSTAT)
    #define sensor_8_INTSTAT            (* (reg8 *) sensor_8__INTSTAT)
    #define sensor_8_SNAP               (* (reg8 *) sensor_8__SNAP)
    
	#define sensor_8_0_INTTYPE_REG 		(* (reg8 *) sensor_8__0__INTTYPE)
#endif /* (sensor_8__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_sensor_8_H */


/* [] END OF FILE */
