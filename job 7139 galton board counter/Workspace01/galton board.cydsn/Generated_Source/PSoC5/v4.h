/*******************************************************************************
* File Name: v4.h  
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

#if !defined(CY_PINS_v4_H) /* Pins v4_H */
#define CY_PINS_v4_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "v4_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 v4__PORT == 15 && ((v4__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    v4_Write(uint8 value);
void    v4_SetDriveMode(uint8 mode);
uint8   v4_ReadDataReg(void);
uint8   v4_Read(void);
void    v4_SetInterruptMode(uint16 position, uint16 mode);
uint8   v4_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the v4_SetDriveMode() function.
     *  @{
     */
        #define v4_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define v4_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define v4_DM_RES_UP          PIN_DM_RES_UP
        #define v4_DM_RES_DWN         PIN_DM_RES_DWN
        #define v4_DM_OD_LO           PIN_DM_OD_LO
        #define v4_DM_OD_HI           PIN_DM_OD_HI
        #define v4_DM_STRONG          PIN_DM_STRONG
        #define v4_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define v4_MASK               v4__MASK
#define v4_SHIFT              v4__SHIFT
#define v4_WIDTH              1u

/* Interrupt constants */
#if defined(v4__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in v4_SetInterruptMode() function.
     *  @{
     */
        #define v4_INTR_NONE      (uint16)(0x0000u)
        #define v4_INTR_RISING    (uint16)(0x0001u)
        #define v4_INTR_FALLING   (uint16)(0x0002u)
        #define v4_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define v4_INTR_MASK      (0x01u) 
#endif /* (v4__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define v4_PS                     (* (reg8 *) v4__PS)
/* Data Register */
#define v4_DR                     (* (reg8 *) v4__DR)
/* Port Number */
#define v4_PRT_NUM                (* (reg8 *) v4__PRT) 
/* Connect to Analog Globals */                                                  
#define v4_AG                     (* (reg8 *) v4__AG)                       
/* Analog MUX bux enable */
#define v4_AMUX                   (* (reg8 *) v4__AMUX) 
/* Bidirectional Enable */                                                        
#define v4_BIE                    (* (reg8 *) v4__BIE)
/* Bit-mask for Aliased Register Access */
#define v4_BIT_MASK               (* (reg8 *) v4__BIT_MASK)
/* Bypass Enable */
#define v4_BYP                    (* (reg8 *) v4__BYP)
/* Port wide control signals */                                                   
#define v4_CTL                    (* (reg8 *) v4__CTL)
/* Drive Modes */
#define v4_DM0                    (* (reg8 *) v4__DM0) 
#define v4_DM1                    (* (reg8 *) v4__DM1)
#define v4_DM2                    (* (reg8 *) v4__DM2) 
/* Input Buffer Disable Override */
#define v4_INP_DIS                (* (reg8 *) v4__INP_DIS)
/* LCD Common or Segment Drive */
#define v4_LCD_COM_SEG            (* (reg8 *) v4__LCD_COM_SEG)
/* Enable Segment LCD */
#define v4_LCD_EN                 (* (reg8 *) v4__LCD_EN)
/* Slew Rate Control */
#define v4_SLW                    (* (reg8 *) v4__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define v4_PRTDSI__CAPS_SEL       (* (reg8 *) v4__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define v4_PRTDSI__DBL_SYNC_IN    (* (reg8 *) v4__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define v4_PRTDSI__OE_SEL0        (* (reg8 *) v4__PRTDSI__OE_SEL0) 
#define v4_PRTDSI__OE_SEL1        (* (reg8 *) v4__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define v4_PRTDSI__OUT_SEL0       (* (reg8 *) v4__PRTDSI__OUT_SEL0) 
#define v4_PRTDSI__OUT_SEL1       (* (reg8 *) v4__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define v4_PRTDSI__SYNC_OUT       (* (reg8 *) v4__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(v4__SIO_CFG)
    #define v4_SIO_HYST_EN        (* (reg8 *) v4__SIO_HYST_EN)
    #define v4_SIO_REG_HIFREQ     (* (reg8 *) v4__SIO_REG_HIFREQ)
    #define v4_SIO_CFG            (* (reg8 *) v4__SIO_CFG)
    #define v4_SIO_DIFF           (* (reg8 *) v4__SIO_DIFF)
#endif /* (v4__SIO_CFG) */

/* Interrupt Registers */
#if defined(v4__INTSTAT)
    #define v4_INTSTAT            (* (reg8 *) v4__INTSTAT)
    #define v4_SNAP               (* (reg8 *) v4__SNAP)
    
	#define v4_0_INTTYPE_REG 		(* (reg8 *) v4__0__INTTYPE)
#endif /* (v4__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_v4_H */


/* [] END OF FILE */
