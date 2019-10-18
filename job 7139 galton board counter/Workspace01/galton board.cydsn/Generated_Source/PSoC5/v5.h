/*******************************************************************************
* File Name: v5.h  
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

#if !defined(CY_PINS_v5_H) /* Pins v5_H */
#define CY_PINS_v5_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "v5_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 v5__PORT == 15 && ((v5__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    v5_Write(uint8 value);
void    v5_SetDriveMode(uint8 mode);
uint8   v5_ReadDataReg(void);
uint8   v5_Read(void);
void    v5_SetInterruptMode(uint16 position, uint16 mode);
uint8   v5_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the v5_SetDriveMode() function.
     *  @{
     */
        #define v5_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define v5_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define v5_DM_RES_UP          PIN_DM_RES_UP
        #define v5_DM_RES_DWN         PIN_DM_RES_DWN
        #define v5_DM_OD_LO           PIN_DM_OD_LO
        #define v5_DM_OD_HI           PIN_DM_OD_HI
        #define v5_DM_STRONG          PIN_DM_STRONG
        #define v5_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define v5_MASK               v5__MASK
#define v5_SHIFT              v5__SHIFT
#define v5_WIDTH              1u

/* Interrupt constants */
#if defined(v5__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in v5_SetInterruptMode() function.
     *  @{
     */
        #define v5_INTR_NONE      (uint16)(0x0000u)
        #define v5_INTR_RISING    (uint16)(0x0001u)
        #define v5_INTR_FALLING   (uint16)(0x0002u)
        #define v5_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define v5_INTR_MASK      (0x01u) 
#endif /* (v5__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define v5_PS                     (* (reg8 *) v5__PS)
/* Data Register */
#define v5_DR                     (* (reg8 *) v5__DR)
/* Port Number */
#define v5_PRT_NUM                (* (reg8 *) v5__PRT) 
/* Connect to Analog Globals */                                                  
#define v5_AG                     (* (reg8 *) v5__AG)                       
/* Analog MUX bux enable */
#define v5_AMUX                   (* (reg8 *) v5__AMUX) 
/* Bidirectional Enable */                                                        
#define v5_BIE                    (* (reg8 *) v5__BIE)
/* Bit-mask for Aliased Register Access */
#define v5_BIT_MASK               (* (reg8 *) v5__BIT_MASK)
/* Bypass Enable */
#define v5_BYP                    (* (reg8 *) v5__BYP)
/* Port wide control signals */                                                   
#define v5_CTL                    (* (reg8 *) v5__CTL)
/* Drive Modes */
#define v5_DM0                    (* (reg8 *) v5__DM0) 
#define v5_DM1                    (* (reg8 *) v5__DM1)
#define v5_DM2                    (* (reg8 *) v5__DM2) 
/* Input Buffer Disable Override */
#define v5_INP_DIS                (* (reg8 *) v5__INP_DIS)
/* LCD Common or Segment Drive */
#define v5_LCD_COM_SEG            (* (reg8 *) v5__LCD_COM_SEG)
/* Enable Segment LCD */
#define v5_LCD_EN                 (* (reg8 *) v5__LCD_EN)
/* Slew Rate Control */
#define v5_SLW                    (* (reg8 *) v5__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define v5_PRTDSI__CAPS_SEL       (* (reg8 *) v5__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define v5_PRTDSI__DBL_SYNC_IN    (* (reg8 *) v5__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define v5_PRTDSI__OE_SEL0        (* (reg8 *) v5__PRTDSI__OE_SEL0) 
#define v5_PRTDSI__OE_SEL1        (* (reg8 *) v5__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define v5_PRTDSI__OUT_SEL0       (* (reg8 *) v5__PRTDSI__OUT_SEL0) 
#define v5_PRTDSI__OUT_SEL1       (* (reg8 *) v5__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define v5_PRTDSI__SYNC_OUT       (* (reg8 *) v5__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(v5__SIO_CFG)
    #define v5_SIO_HYST_EN        (* (reg8 *) v5__SIO_HYST_EN)
    #define v5_SIO_REG_HIFREQ     (* (reg8 *) v5__SIO_REG_HIFREQ)
    #define v5_SIO_CFG            (* (reg8 *) v5__SIO_CFG)
    #define v5_SIO_DIFF           (* (reg8 *) v5__SIO_DIFF)
#endif /* (v5__SIO_CFG) */

/* Interrupt Registers */
#if defined(v5__INTSTAT)
    #define v5_INTSTAT            (* (reg8 *) v5__INTSTAT)
    #define v5_SNAP               (* (reg8 *) v5__SNAP)
    
	#define v5_0_INTTYPE_REG 		(* (reg8 *) v5__0__INTTYPE)
#endif /* (v5__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_v5_H */


/* [] END OF FILE */
