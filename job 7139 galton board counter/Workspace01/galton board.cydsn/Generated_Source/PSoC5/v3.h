/*******************************************************************************
* File Name: v3.h  
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

#if !defined(CY_PINS_v3_H) /* Pins v3_H */
#define CY_PINS_v3_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "v3_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 v3__PORT == 15 && ((v3__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    v3_Write(uint8 value);
void    v3_SetDriveMode(uint8 mode);
uint8   v3_ReadDataReg(void);
uint8   v3_Read(void);
void    v3_SetInterruptMode(uint16 position, uint16 mode);
uint8   v3_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the v3_SetDriveMode() function.
     *  @{
     */
        #define v3_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define v3_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define v3_DM_RES_UP          PIN_DM_RES_UP
        #define v3_DM_RES_DWN         PIN_DM_RES_DWN
        #define v3_DM_OD_LO           PIN_DM_OD_LO
        #define v3_DM_OD_HI           PIN_DM_OD_HI
        #define v3_DM_STRONG          PIN_DM_STRONG
        #define v3_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define v3_MASK               v3__MASK
#define v3_SHIFT              v3__SHIFT
#define v3_WIDTH              1u

/* Interrupt constants */
#if defined(v3__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in v3_SetInterruptMode() function.
     *  @{
     */
        #define v3_INTR_NONE      (uint16)(0x0000u)
        #define v3_INTR_RISING    (uint16)(0x0001u)
        #define v3_INTR_FALLING   (uint16)(0x0002u)
        #define v3_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define v3_INTR_MASK      (0x01u) 
#endif /* (v3__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define v3_PS                     (* (reg8 *) v3__PS)
/* Data Register */
#define v3_DR                     (* (reg8 *) v3__DR)
/* Port Number */
#define v3_PRT_NUM                (* (reg8 *) v3__PRT) 
/* Connect to Analog Globals */                                                  
#define v3_AG                     (* (reg8 *) v3__AG)                       
/* Analog MUX bux enable */
#define v3_AMUX                   (* (reg8 *) v3__AMUX) 
/* Bidirectional Enable */                                                        
#define v3_BIE                    (* (reg8 *) v3__BIE)
/* Bit-mask for Aliased Register Access */
#define v3_BIT_MASK               (* (reg8 *) v3__BIT_MASK)
/* Bypass Enable */
#define v3_BYP                    (* (reg8 *) v3__BYP)
/* Port wide control signals */                                                   
#define v3_CTL                    (* (reg8 *) v3__CTL)
/* Drive Modes */
#define v3_DM0                    (* (reg8 *) v3__DM0) 
#define v3_DM1                    (* (reg8 *) v3__DM1)
#define v3_DM2                    (* (reg8 *) v3__DM2) 
/* Input Buffer Disable Override */
#define v3_INP_DIS                (* (reg8 *) v3__INP_DIS)
/* LCD Common or Segment Drive */
#define v3_LCD_COM_SEG            (* (reg8 *) v3__LCD_COM_SEG)
/* Enable Segment LCD */
#define v3_LCD_EN                 (* (reg8 *) v3__LCD_EN)
/* Slew Rate Control */
#define v3_SLW                    (* (reg8 *) v3__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define v3_PRTDSI__CAPS_SEL       (* (reg8 *) v3__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define v3_PRTDSI__DBL_SYNC_IN    (* (reg8 *) v3__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define v3_PRTDSI__OE_SEL0        (* (reg8 *) v3__PRTDSI__OE_SEL0) 
#define v3_PRTDSI__OE_SEL1        (* (reg8 *) v3__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define v3_PRTDSI__OUT_SEL0       (* (reg8 *) v3__PRTDSI__OUT_SEL0) 
#define v3_PRTDSI__OUT_SEL1       (* (reg8 *) v3__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define v3_PRTDSI__SYNC_OUT       (* (reg8 *) v3__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(v3__SIO_CFG)
    #define v3_SIO_HYST_EN        (* (reg8 *) v3__SIO_HYST_EN)
    #define v3_SIO_REG_HIFREQ     (* (reg8 *) v3__SIO_REG_HIFREQ)
    #define v3_SIO_CFG            (* (reg8 *) v3__SIO_CFG)
    #define v3_SIO_DIFF           (* (reg8 *) v3__SIO_DIFF)
#endif /* (v3__SIO_CFG) */

/* Interrupt Registers */
#if defined(v3__INTSTAT)
    #define v3_INTSTAT            (* (reg8 *) v3__INTSTAT)
    #define v3_SNAP               (* (reg8 *) v3__SNAP)
    
	#define v3_0_INTTYPE_REG 		(* (reg8 *) v3__0__INTTYPE)
#endif /* (v3__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_v3_H */


/* [] END OF FILE */
