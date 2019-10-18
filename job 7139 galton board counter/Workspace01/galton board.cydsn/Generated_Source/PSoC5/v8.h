/*******************************************************************************
* File Name: v8.h  
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

#if !defined(CY_PINS_v8_H) /* Pins v8_H */
#define CY_PINS_v8_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "v8_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 v8__PORT == 15 && ((v8__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    v8_Write(uint8 value);
void    v8_SetDriveMode(uint8 mode);
uint8   v8_ReadDataReg(void);
uint8   v8_Read(void);
void    v8_SetInterruptMode(uint16 position, uint16 mode);
uint8   v8_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the v8_SetDriveMode() function.
     *  @{
     */
        #define v8_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define v8_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define v8_DM_RES_UP          PIN_DM_RES_UP
        #define v8_DM_RES_DWN         PIN_DM_RES_DWN
        #define v8_DM_OD_LO           PIN_DM_OD_LO
        #define v8_DM_OD_HI           PIN_DM_OD_HI
        #define v8_DM_STRONG          PIN_DM_STRONG
        #define v8_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define v8_MASK               v8__MASK
#define v8_SHIFT              v8__SHIFT
#define v8_WIDTH              1u

/* Interrupt constants */
#if defined(v8__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in v8_SetInterruptMode() function.
     *  @{
     */
        #define v8_INTR_NONE      (uint16)(0x0000u)
        #define v8_INTR_RISING    (uint16)(0x0001u)
        #define v8_INTR_FALLING   (uint16)(0x0002u)
        #define v8_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define v8_INTR_MASK      (0x01u) 
#endif /* (v8__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define v8_PS                     (* (reg8 *) v8__PS)
/* Data Register */
#define v8_DR                     (* (reg8 *) v8__DR)
/* Port Number */
#define v8_PRT_NUM                (* (reg8 *) v8__PRT) 
/* Connect to Analog Globals */                                                  
#define v8_AG                     (* (reg8 *) v8__AG)                       
/* Analog MUX bux enable */
#define v8_AMUX                   (* (reg8 *) v8__AMUX) 
/* Bidirectional Enable */                                                        
#define v8_BIE                    (* (reg8 *) v8__BIE)
/* Bit-mask for Aliased Register Access */
#define v8_BIT_MASK               (* (reg8 *) v8__BIT_MASK)
/* Bypass Enable */
#define v8_BYP                    (* (reg8 *) v8__BYP)
/* Port wide control signals */                                                   
#define v8_CTL                    (* (reg8 *) v8__CTL)
/* Drive Modes */
#define v8_DM0                    (* (reg8 *) v8__DM0) 
#define v8_DM1                    (* (reg8 *) v8__DM1)
#define v8_DM2                    (* (reg8 *) v8__DM2) 
/* Input Buffer Disable Override */
#define v8_INP_DIS                (* (reg8 *) v8__INP_DIS)
/* LCD Common or Segment Drive */
#define v8_LCD_COM_SEG            (* (reg8 *) v8__LCD_COM_SEG)
/* Enable Segment LCD */
#define v8_LCD_EN                 (* (reg8 *) v8__LCD_EN)
/* Slew Rate Control */
#define v8_SLW                    (* (reg8 *) v8__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define v8_PRTDSI__CAPS_SEL       (* (reg8 *) v8__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define v8_PRTDSI__DBL_SYNC_IN    (* (reg8 *) v8__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define v8_PRTDSI__OE_SEL0        (* (reg8 *) v8__PRTDSI__OE_SEL0) 
#define v8_PRTDSI__OE_SEL1        (* (reg8 *) v8__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define v8_PRTDSI__OUT_SEL0       (* (reg8 *) v8__PRTDSI__OUT_SEL0) 
#define v8_PRTDSI__OUT_SEL1       (* (reg8 *) v8__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define v8_PRTDSI__SYNC_OUT       (* (reg8 *) v8__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(v8__SIO_CFG)
    #define v8_SIO_HYST_EN        (* (reg8 *) v8__SIO_HYST_EN)
    #define v8_SIO_REG_HIFREQ     (* (reg8 *) v8__SIO_REG_HIFREQ)
    #define v8_SIO_CFG            (* (reg8 *) v8__SIO_CFG)
    #define v8_SIO_DIFF           (* (reg8 *) v8__SIO_DIFF)
#endif /* (v8__SIO_CFG) */

/* Interrupt Registers */
#if defined(v8__INTSTAT)
    #define v8_INTSTAT            (* (reg8 *) v8__INTSTAT)
    #define v8_SNAP               (* (reg8 *) v8__SNAP)
    
	#define v8_0_INTTYPE_REG 		(* (reg8 *) v8__0__INTTYPE)
#endif /* (v8__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_v8_H */


/* [] END OF FILE */
