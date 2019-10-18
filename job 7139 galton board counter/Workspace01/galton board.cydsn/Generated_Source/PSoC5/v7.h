/*******************************************************************************
* File Name: v7.h  
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

#if !defined(CY_PINS_v7_H) /* Pins v7_H */
#define CY_PINS_v7_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "v7_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 v7__PORT == 15 && ((v7__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    v7_Write(uint8 value);
void    v7_SetDriveMode(uint8 mode);
uint8   v7_ReadDataReg(void);
uint8   v7_Read(void);
void    v7_SetInterruptMode(uint16 position, uint16 mode);
uint8   v7_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the v7_SetDriveMode() function.
     *  @{
     */
        #define v7_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define v7_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define v7_DM_RES_UP          PIN_DM_RES_UP
        #define v7_DM_RES_DWN         PIN_DM_RES_DWN
        #define v7_DM_OD_LO           PIN_DM_OD_LO
        #define v7_DM_OD_HI           PIN_DM_OD_HI
        #define v7_DM_STRONG          PIN_DM_STRONG
        #define v7_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define v7_MASK               v7__MASK
#define v7_SHIFT              v7__SHIFT
#define v7_WIDTH              1u

/* Interrupt constants */
#if defined(v7__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in v7_SetInterruptMode() function.
     *  @{
     */
        #define v7_INTR_NONE      (uint16)(0x0000u)
        #define v7_INTR_RISING    (uint16)(0x0001u)
        #define v7_INTR_FALLING   (uint16)(0x0002u)
        #define v7_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define v7_INTR_MASK      (0x01u) 
#endif /* (v7__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define v7_PS                     (* (reg8 *) v7__PS)
/* Data Register */
#define v7_DR                     (* (reg8 *) v7__DR)
/* Port Number */
#define v7_PRT_NUM                (* (reg8 *) v7__PRT) 
/* Connect to Analog Globals */                                                  
#define v7_AG                     (* (reg8 *) v7__AG)                       
/* Analog MUX bux enable */
#define v7_AMUX                   (* (reg8 *) v7__AMUX) 
/* Bidirectional Enable */                                                        
#define v7_BIE                    (* (reg8 *) v7__BIE)
/* Bit-mask for Aliased Register Access */
#define v7_BIT_MASK               (* (reg8 *) v7__BIT_MASK)
/* Bypass Enable */
#define v7_BYP                    (* (reg8 *) v7__BYP)
/* Port wide control signals */                                                   
#define v7_CTL                    (* (reg8 *) v7__CTL)
/* Drive Modes */
#define v7_DM0                    (* (reg8 *) v7__DM0) 
#define v7_DM1                    (* (reg8 *) v7__DM1)
#define v7_DM2                    (* (reg8 *) v7__DM2) 
/* Input Buffer Disable Override */
#define v7_INP_DIS                (* (reg8 *) v7__INP_DIS)
/* LCD Common or Segment Drive */
#define v7_LCD_COM_SEG            (* (reg8 *) v7__LCD_COM_SEG)
/* Enable Segment LCD */
#define v7_LCD_EN                 (* (reg8 *) v7__LCD_EN)
/* Slew Rate Control */
#define v7_SLW                    (* (reg8 *) v7__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define v7_PRTDSI__CAPS_SEL       (* (reg8 *) v7__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define v7_PRTDSI__DBL_SYNC_IN    (* (reg8 *) v7__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define v7_PRTDSI__OE_SEL0        (* (reg8 *) v7__PRTDSI__OE_SEL0) 
#define v7_PRTDSI__OE_SEL1        (* (reg8 *) v7__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define v7_PRTDSI__OUT_SEL0       (* (reg8 *) v7__PRTDSI__OUT_SEL0) 
#define v7_PRTDSI__OUT_SEL1       (* (reg8 *) v7__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define v7_PRTDSI__SYNC_OUT       (* (reg8 *) v7__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(v7__SIO_CFG)
    #define v7_SIO_HYST_EN        (* (reg8 *) v7__SIO_HYST_EN)
    #define v7_SIO_REG_HIFREQ     (* (reg8 *) v7__SIO_REG_HIFREQ)
    #define v7_SIO_CFG            (* (reg8 *) v7__SIO_CFG)
    #define v7_SIO_DIFF           (* (reg8 *) v7__SIO_DIFF)
#endif /* (v7__SIO_CFG) */

/* Interrupt Registers */
#if defined(v7__INTSTAT)
    #define v7_INTSTAT            (* (reg8 *) v7__INTSTAT)
    #define v7_SNAP               (* (reg8 *) v7__SNAP)
    
	#define v7_0_INTTYPE_REG 		(* (reg8 *) v7__0__INTTYPE)
#endif /* (v7__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_v7_H */


/* [] END OF FILE */
