/*******************************************************************************
* File Name: v6.h  
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

#if !defined(CY_PINS_v6_H) /* Pins v6_H */
#define CY_PINS_v6_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "v6_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 v6__PORT == 15 && ((v6__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    v6_Write(uint8 value);
void    v6_SetDriveMode(uint8 mode);
uint8   v6_ReadDataReg(void);
uint8   v6_Read(void);
void    v6_SetInterruptMode(uint16 position, uint16 mode);
uint8   v6_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the v6_SetDriveMode() function.
     *  @{
     */
        #define v6_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define v6_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define v6_DM_RES_UP          PIN_DM_RES_UP
        #define v6_DM_RES_DWN         PIN_DM_RES_DWN
        #define v6_DM_OD_LO           PIN_DM_OD_LO
        #define v6_DM_OD_HI           PIN_DM_OD_HI
        #define v6_DM_STRONG          PIN_DM_STRONG
        #define v6_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define v6_MASK               v6__MASK
#define v6_SHIFT              v6__SHIFT
#define v6_WIDTH              1u

/* Interrupt constants */
#if defined(v6__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in v6_SetInterruptMode() function.
     *  @{
     */
        #define v6_INTR_NONE      (uint16)(0x0000u)
        #define v6_INTR_RISING    (uint16)(0x0001u)
        #define v6_INTR_FALLING   (uint16)(0x0002u)
        #define v6_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define v6_INTR_MASK      (0x01u) 
#endif /* (v6__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define v6_PS                     (* (reg8 *) v6__PS)
/* Data Register */
#define v6_DR                     (* (reg8 *) v6__DR)
/* Port Number */
#define v6_PRT_NUM                (* (reg8 *) v6__PRT) 
/* Connect to Analog Globals */                                                  
#define v6_AG                     (* (reg8 *) v6__AG)                       
/* Analog MUX bux enable */
#define v6_AMUX                   (* (reg8 *) v6__AMUX) 
/* Bidirectional Enable */                                                        
#define v6_BIE                    (* (reg8 *) v6__BIE)
/* Bit-mask for Aliased Register Access */
#define v6_BIT_MASK               (* (reg8 *) v6__BIT_MASK)
/* Bypass Enable */
#define v6_BYP                    (* (reg8 *) v6__BYP)
/* Port wide control signals */                                                   
#define v6_CTL                    (* (reg8 *) v6__CTL)
/* Drive Modes */
#define v6_DM0                    (* (reg8 *) v6__DM0) 
#define v6_DM1                    (* (reg8 *) v6__DM1)
#define v6_DM2                    (* (reg8 *) v6__DM2) 
/* Input Buffer Disable Override */
#define v6_INP_DIS                (* (reg8 *) v6__INP_DIS)
/* LCD Common or Segment Drive */
#define v6_LCD_COM_SEG            (* (reg8 *) v6__LCD_COM_SEG)
/* Enable Segment LCD */
#define v6_LCD_EN                 (* (reg8 *) v6__LCD_EN)
/* Slew Rate Control */
#define v6_SLW                    (* (reg8 *) v6__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define v6_PRTDSI__CAPS_SEL       (* (reg8 *) v6__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define v6_PRTDSI__DBL_SYNC_IN    (* (reg8 *) v6__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define v6_PRTDSI__OE_SEL0        (* (reg8 *) v6__PRTDSI__OE_SEL0) 
#define v6_PRTDSI__OE_SEL1        (* (reg8 *) v6__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define v6_PRTDSI__OUT_SEL0       (* (reg8 *) v6__PRTDSI__OUT_SEL0) 
#define v6_PRTDSI__OUT_SEL1       (* (reg8 *) v6__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define v6_PRTDSI__SYNC_OUT       (* (reg8 *) v6__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(v6__SIO_CFG)
    #define v6_SIO_HYST_EN        (* (reg8 *) v6__SIO_HYST_EN)
    #define v6_SIO_REG_HIFREQ     (* (reg8 *) v6__SIO_REG_HIFREQ)
    #define v6_SIO_CFG            (* (reg8 *) v6__SIO_CFG)
    #define v6_SIO_DIFF           (* (reg8 *) v6__SIO_DIFF)
#endif /* (v6__SIO_CFG) */

/* Interrupt Registers */
#if defined(v6__INTSTAT)
    #define v6_INTSTAT            (* (reg8 *) v6__INTSTAT)
    #define v6_SNAP               (* (reg8 *) v6__SNAP)
    
	#define v6_0_INTTYPE_REG 		(* (reg8 *) v6__0__INTTYPE)
#endif /* (v6__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_v6_H */


/* [] END OF FILE */
