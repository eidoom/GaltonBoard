/*******************************************************************************
* File Name: ScanComp_1.c
* Version 1.10
*
* Description:
*  This file contains the function prototypes and constants used in
*  the Scanning Comparator Component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_ScanComp_ScanComp_1_H)
#define CY_ScanComp_ScanComp_1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cyLib.h"
#include "ScanComp_1_Comp.h"


/***************************************
*   Conditional Compilation Parameters
***************************************/

/* Channel mode type */
#define ScanComp_1_CHANNEL_MODE_COMMON    (0u)
#define ScanComp_1_CHANNEL_MODE_UNIQUE    (1u)
#define ScanComp_1_CHANNEL_MODE_INTERN    (2u)

#define ScanComp_1_INTERNAL_REF_USED      (2u)

#define ScanComp_1_INTERNAL_REF_IS_USED   ((CY_PSOC3 || CY_PSOC5) &&  \
        (ScanComp_1_INTERNAL_REF_USED == ScanComp_1_CHANNEL_MODE_INTERN))

#if (ScanComp_1_INTERNAL_REF_IS_USED)
    #define ScanComp_1_VDAC_PER_CHANNEL_MODE_USED  (0u)
#endif /* ScanComp_1_INTERNAL_REF_IS_USED */

#if (ScanComp_1_INTERNAL_REF_IS_USED)
    #include "ScanComp_1_VDAC.h"

    #if (ScanComp_1_VDAC_PER_CHANNEL_MODE_USED)
        #include "CyDmac.h"
        #include "ScanComp_1_DMA_dma.h"
    #endif /* ScanComp_1_VDAC_PER_CHANNEL_MODE_USED */

    #define ScanComp_1_DEFAULT_VOLTAGE        (250u)
    #define ScanComp_1_DEFAULT_RANGE          (4u)
#endif /* ScanComp_1_INTERNAL_REF_IS_USED */


/*******************************************************************************
*   Data Struct Definitions
*******************************************************************************/

/* Structure to save state before go to sleep */
typedef struct
{
    uint8  enableState;
} ScanComp_1_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void ScanComp_1_Start(void) ;
void ScanComp_1_Init(void) ;
void ScanComp_1_Enable(void) ;
void ScanComp_1_Stop(void) ;
void ScanComp_1_SetSpeed(uint8 speed) ;
uint8 ScanComp_1_GetCompare(uint8 channel) ;
uint8 ScanComp_1_GetInterruptSource(uint8 inputBlock)         \
                                          ;
uint8 ScanComp_1_GetInterruptSourceMasked(uint8 inputBlock)   \
                                                ;
uint8 ScanComp_1_GetInterruptMask(uint8 inputBlock)           \
                                        ;
void ScanComp_1_SetInterruptMask(uint8 inputBlock, uint8 mask)\
                                       ;
void ScanComp_1_Sleep(void) ;
void ScanComp_1_Wakeup(void) ;

#if (ScanComp_1_INTERNAL_REF_IS_USED)
    void ScanComp_1_SetDACRange(uint8 DACRange) ;
    uint8 ScanComp_1_GetDACRange(void) ;

    #if (ScanComp_1_VDAC_PER_CHANNEL_MODE_USED)
        void ScanComp_1_SetChannelDACVoltage(uint8 channel, uint8 DACVoltage) \
                                                   ;
        uint8 ScanComp_1_GetChannelDACVoltage(uint8 channel)  \
                                                    ;
    #else
        void ScanComp_1_SetDACVoltage(uint8 DACVoltage) ;
        uint8 ScanComp_1_GetDACVoltage(void) ;
    #endif /* ScanComp_1_VDAC_PER_CHANNEL_MODE_USED */
#endif /* ScanComp_1_INTERNAL_REF_IS_USED */


/*******************************************************************************
* Variable with external linkage
*******************************************************************************/

extern uint8  ScanComp_1_initVar;


/***************************************
*           API Constants
***************************************/

#if (ScanComp_1_INTERNAL_REF_IS_USED)
    #if (ScanComp_1_VDAC_PER_CHANNEL_MODE_USED)
        /* DMA Configuration for DMA */
        #define ScanComp_1_DMA_BYTES_PER_BURST    (1u)
        #define ScanComp_1_DMA_REQUEST_PER_BURST  (1u)
        #define ScanComp_1_DMA_SRC_BASE (CYDEV_SRAM_BASE)
        #define ScanComp_1_DMA_DST_BASE (CYDEV_PERIPH_BASE)
    #endif /* ScanComp_1_VDAC_PER_CHANNEL_MODE_USED */
#endif /* ScanComp_1_INTERNAL_REF_IS_USED */

/* Shift */
#define ScanComp_1_7BIT_SHIFT                 (0x07u)

/* Registers array defines */
#define ScanComp_1_NUM_OF_REGISTERS           ((ScanComp_1_DEFAULT_CHANNELS_VAL >> 3u) + 1u)

#define ScanComp_1_NUM_OF_OUT_REGISTERS       (ScanComp_1_NUM_OF_REGISTERS)

#define ScanComp_1_NUM_OF_INT_REGISTERS       (ScanComp_1_NUM_OF_REGISTERS)

/* Num of possible channels */
#define ScanComp_1_NUM_OF_POSSIBLE_CHAN       ((uint8)(ScanComp_1_NUM_OF_REGISTERS << 3u))

/* MSB register bit shift */
#define ScanComp_1_LSB_REG_BIT_SHIFT          (0xFFu >>    \
                                                    (ScanComp_1_NUM_OF_POSSIBLE_CHAN - \
                                                     ScanComp_1_NUM_OF_CHANNELS))

#define ScanComp_1_MSB_REG_CHAN_EXIST         (ScanComp_1_LSB_REG_BIT_SHIFT >> 7u)

/* MSB-bit of the register in high */
#define ScanComp_1_MSB_REG_BIT_HIGH           (0x80u)


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* VDAC Range constants */
#if (ScanComp_1_INTERNAL_REF_IS_USED)
    #define ScanComp_1_SCANCOMP_DACRANGE_1V   (ScanComp_1_VDAC_RANGE_1V)
    #define ScanComp_1_SCANCOMP_DACRANGE_4V   (ScanComp_1_VDAC_RANGE_4V)
#endif /* ScanComp_1_INTERNAL_REF_IS_USED */

/* Constants for SetSpeed() function */
#if (CY_PSOC4)
    #define ScanComp_1_SLOWSPEED   (ScanComp_1_Comp_SLOWSPEED)
    #define ScanComp_1_MEDSPEED    (ScanComp_1_Comp_MEDSPEED)
    #define ScanComp_1_HIGHSPEED   (ScanComp_1_Comp_HIGHSPEED)
#else
    #define ScanComp_1_SLOWSPEED   (ScanComp_1_Comp_LOWPOWER)
    #define ScanComp_1_MEDSPEED    (ScanComp_1_Comp_SLOWSPEED)
    #define ScanComp_1_HIGHSPEED   (ScanComp_1_Comp_HIGHSPEED)
#endif /* CY_PSOC4 */


/***************************************
*   Initial Parameter Constants
***************************************/
#define ScanComp_1_NUM_OF_CHANNELS                (1u)
#define ScanComp_1_DEFAULT_CHANNELS_VAL           (1u - 1u)
#define ScanComp_1_DEFAULT_SPEED                  (0u)


/***************************************
*             Registers
***************************************/

/* Status Registers */
#define ScanComp_1_STATUS_INT0_REG                (* (reg8 *) ScanComp_1_bScanComp_St0_IntReg0__STATUS_REG)
#define ScanComp_1_STATUS_INT0_PTR                (  (reg8 *) ScanComp_1_bScanComp_St0_IntReg0__STATUS_REG)
#define ScanComp_1_STATUS_INT0_MASK               (* (reg8 *) ScanComp_1_bScanComp_St0_IntReg0__MASK_REG)
#define ScanComp_1_STATUS_INT0_MASK_PTR           (  (reg8 *) ScanComp_1_bScanComp_St0_IntReg0__MASK_REG)
#define ScanComp_1_STATUS_INT0_AUX_CONTROL        (* (reg8 *) \
                                                        ScanComp_1_bScanComp_St0_IntReg0__STATUS_AUX_CTL_REG)

#define ScanComp_1_STATUS_OUT0_REG                (* (reg8 *) ScanComp_1_bScanComp_St0_OutReg0__STATUS_REG)
#define ScanComp_1_STATUS_OUT0_PTR                (  (reg8 *) ScanComp_1_bScanComp_St0_OutReg0__STATUS_REG)
#define ScanComp_1_STATUS_OUT0_MASK               (* (reg8 *) ScanComp_1_bScanComp_St0_OutReg0__MASK_REG)
#define ScanComp_1_STATUS_OUT0_AUX_CONTROL        (* (reg8 *) \
                                                        ScanComp_1_bScanComp_St0_OutReg0__STATUS_AUX_CTL_REG)

#if (ScanComp_1_NUM_OF_CHANNELS >= 8u)
    #define ScanComp_1_STATUS_INT8_REG            (* (reg8 *) ScanComp_1_bScanComp_St8_IntReg8__STATUS_REG)
    #define ScanComp_1_STATUS_INT8_PTR            (  (reg8 *) ScanComp_1_bScanComp_St8_IntReg8__STATUS_REG)
    #define ScanComp_1_STATUS_INT8_MASK           (* (reg8 *) ScanComp_1_bScanComp_St8_IntReg8__MASK_REG)
    #define ScanComp_1_STATUS_INT8_MASK_PTR       (  (reg8 *) ScanComp_1_bScanComp_St8_IntReg8__MASK_REG)
    #define ScanComp_1_STATUS_INT8_AUX_CONTROL    (* (reg8 *) \
                                                        ScanComp_1_bScanComp_St8_IntReg8__STATUS_AUX_CTL_REG)
#endif /* ScanComp_1_NUM_OF_CHANNELS >= 8u */

#if (ScanComp_1_NUM_OF_CHANNELS > 8u)
    #define ScanComp_1_STATUS_INT1_REG            (* (reg8 *) ScanComp_1_bScanComp_St1_IntReg1__STATUS_REG)
    #define ScanComp_1_STATUS_INT1_PTR            (  (reg8 *) ScanComp_1_bScanComp_St1_IntReg1__STATUS_REG)
    #define ScanComp_1_STATUS_INT1_MASK           (* (reg8 *) ScanComp_1_bScanComp_St1_IntReg1__MASK_REG)
    #define ScanComp_1_STATUS_INT1_MASK_PTR       (  (reg8 *) ScanComp_1_bScanComp_St1_IntReg1__MASK_REG)
    #define ScanComp_1_STATUS_INT1_AUX_CONTROL    (* (reg8 *) \
                                                        ScanComp_1_bScanComp_St1_IntReg1__STATUS_AUX_CTL_REG)

    #define ScanComp_1_STATUS_OUT1_REG            (* (reg8 *) ScanComp_1_bScanComp_St1_OutReg1__STATUS_REG)
    #define ScanComp_1_STATUS_OUT1_PTR            (  (reg8 *) ScanComp_1_bScanComp_St1_OutReg1__STATUS_REG)
    #define ScanComp_1_STATUS_OUT1_MASK           (* (reg8 *) ScanComp_1_bScanComp_St1_OutReg1__MASK_REG)
    #define ScanComp_1_STATUS_OUT1_AUX_CONTROL    (* (reg8 *) \
                                                        ScanComp_1_bScanComp_St1_OutReg1__STATUS_AUX_CTL_REG)
#endif /* ScanComp_1_NUM_OF_CHANNELS > 8u */

#if (ScanComp_1_NUM_OF_CHANNELS > 16u)
    #define ScanComp_1_STATUS_INT2_REG            (* (reg8 *) ScanComp_1_bScanComp_St2_IntReg2__STATUS_REG)
    #define ScanComp_1_STATUS_INT2_PTR            (  (reg8 *) ScanComp_1_bScanComp_St2_IntReg2__STATUS_REG)
    #define ScanComp_1_STATUS_INT2_MASK           (* (reg8 *) ScanComp_1_bScanComp_St2_IntReg2__MASK_REG)
    #define ScanComp_1_STATUS_INT2_MASK_PTR       (  (reg8 *) ScanComp_1_bScanComp_St2_IntReg2__MASK_REG)
    #define ScanComp_1_STATUS_INT2_AUX_CONTROL    (* (reg8 *) \
                                                        ScanComp_1_bScanComp_St2_IntReg2__STATUS_AUX_CTL_REG)

    #define ScanComp_1_STATUS_OUT2_REG            (* (reg8 *) ScanComp_1_bScanComp_St2_OutReg2__STATUS_REG)
    #define ScanComp_1_STATUS_OUT2_PTR            (  (reg8 *) ScanComp_1_bScanComp_St2_OutReg2__STATUS_REG)
    #define ScanComp_1_STATUS_OUT2_MASK           (* (reg8 *) ScanComp_1_bScanComp_St2_OutReg2__MASK_REG)
    #define ScanComp_1_STATUS_OUT2_AUX_CONTROL    (* (reg8 *) \
                                                        ScanComp_1_bScanComp_St2_OutReg2__STATUS_AUX_CTL_REG)
#endif /* ScanComp_1_NUM_OF_CHANNELS > 16u */

#if (ScanComp_1_NUM_OF_CHANNELS > 24u)
    #define ScanComp_1_STATUS_INT3_REG            (* (reg8 *) ScanComp_1_bScanComp_St3_IntReg3__STATUS_REG)
    #define ScanComp_1_STATUS_INT3_PTR            (  (reg8 *) ScanComp_1_bScanComp_St3_IntReg3__STATUS_REG)
    #define ScanComp_1_STATUS_INT3_MASK           (* (reg8 *) ScanComp_1_bScanComp_St3_IntReg3__MASK_REG)
    #define ScanComp_1_STATUS_INT3_MASK_PTR       (  (reg8 *) ScanComp_1_bScanComp_St3_IntReg3__MASK_REG)
    #define ScanComp_1_STATUS_INT3_AUX_CONTROL    (* (reg8 *) \
                                                        ScanComp_1_bScanComp_St3_IntReg3__STATUS_AUX_CTL_REG)

    #define ScanComp_1_STATUS_OUT3_REG            (* (reg8 *) ScanComp_1_bScanComp_St3_OutReg3__STATUS_REG)
    #define ScanComp_1_STATUS_OUT3_PTR            (  (reg8 *) ScanComp_1_bScanComp_St3_OutReg3__STATUS_REG)
    #define ScanComp_1_STATUS_OUT3_MASK           (* (reg8 *) ScanComp_1_bScanComp_St3_OutReg3__MASK_REG)
    #define ScanComp_1_STATUS_OUT3_AUX_CONTROL    (* (reg8 *) \
                                                        ScanComp_1_bScanComp_St3_OutReg3__STATUS_AUX_CTL_REG)
#endif /* ScanComp_1_NUM_OF_CHANNELS > 24u */

#if (ScanComp_1_NUM_OF_CHANNELS > 32u)
    #define ScanComp_1_STATUS_INT4_REG            (* (reg8 *) ScanComp_1_bScanComp_St4_IntReg4__STATUS_REG)
    #define ScanComp_1_STATUS_INT4_PTR            (  (reg8 *) ScanComp_1_bScanComp_St4_IntReg4__STATUS_REG)
    #define ScanComp_1_STATUS_INT4_MASK           (* (reg8 *) ScanComp_1_bScanComp_St4_IntReg4__MASK_REG)
    #define ScanComp_1_STATUS_INT4_MASK_PTR       (  (reg8 *) ScanComp_1_bScanComp_St4_IntReg4__MASK_REG)
    #define ScanComp_1_STATUS_INT4_AUX_CONTROL    (* (reg8 *) \
                                                        ScanComp_1_bScanComp_St4_IntReg4__STATUS_AUX_CTL_REG)

    #define ScanComp_1_STATUS_OUT4_REG            (* (reg8 *) ScanComp_1_bScanComp_St4_OutReg4__STATUS_REG)
    #define ScanComp_1_STATUS_OUT4_PTR            (  (reg8 *) ScanComp_1_bScanComp_St4_OutReg4__STATUS_REG)
    #define ScanComp_1_STATUS_OUT4_MASK           (* (reg8 *) ScanComp_1_bScanComp_St4_OutReg4__MASK_REG)
    #define ScanComp_1_STATUS_OUT4_AUX_CONTROL    (* (reg8 *) \
                                                        ScanComp_1_bScanComp_St4_OutReg4__STATUS_AUX_CTL_REG)
#endif /* ScanComp_1_NUM_OF_CHANNELS > 32u */

#if (ScanComp_1_NUM_OF_CHANNELS > 40u)
    #define ScanComp_1_STATUS_INT5_REG            (* (reg8 *) ScanComp_1_bScanComp_St5_IntReg5__STATUS_REG)
    #define ScanComp_1_STATUS_INT5_PTR            (  (reg8 *) ScanComp_1_bScanComp_St5_IntReg5__STATUS_REG)
    #define ScanComp_1_STATUS_INT5_MASK           (* (reg8 *) ScanComp_1_bScanComp_St5_IntReg5__MASK_REG)
    #define ScanComp_1_STATUS_INT5_MASK_PTR       (  (reg8 *) ScanComp_1_bScanComp_St5_IntReg5__MASK_REG)
    #define ScanComp_1_STATUS_INT5_AUX_CONTROL    (* (reg8 *) \
                                                        ScanComp_1_bScanComp_St5_IntReg5__STATUS_AUX_CTL_REG)

    #define ScanComp_1_STATUS_OUT5_REG            (* (reg8 *) ScanComp_1_bScanComp_St5_OutReg5__STATUS_REG)
    #define ScanComp_1_STATUS_OUT5_PTR            (  (reg8 *) ScanComp_1_bScanComp_St5_OutReg5__STATUS_REG)
    #define ScanComp_1_STATUS_OUT5_MASK           (* (reg8 *) ScanComp_1_bScanComp_St5_OutReg5__MASK_REG)
    #define ScanComp_1_STATUS_OUT5_AUX_CONTROL    (* (reg8 *) \
                                                        ScanComp_1_bScanComp_St5_OutReg5__STATUS_AUX_CTL_REG)
#endif /* ScanComp_1_NUM_OF_CHANNELS > 40u */

#if (ScanComp_1_NUM_OF_CHANNELS > 48u)
    #define ScanComp_1_STATUS_INT6_REG            (* (reg8 *) ScanComp_1_bScanComp_St6_IntReg6__STATUS_REG)
    #define ScanComp_1_STATUS_INT6_PTR            (  (reg8 *) ScanComp_1_bScanComp_St6_IntReg6__STATUS_REG)
    #define ScanComp_1_STATUS_INT6_MASK           (* (reg8 *) ScanComp_1_bScanComp_St6_IntReg6__MASK_REG)
    #define ScanComp_1_STATUS_INT6_MASK_PTR       (  (reg8 *) ScanComp_1_bScanComp_St6_IntReg6__MASK_REG)
    #define ScanComp_1_STATUS_INT6_AUX_CONTROL    (* (reg8 *) \
                                                        ScanComp_1_bScanComp_St6_IntReg6__STATUS_AUX_CTL_REG)

    #define ScanComp_1_STATUS_OUT6_REG            (* (reg8 *) ScanComp_1_bScanComp_St6_OutReg6__STATUS_REG)
    #define ScanComp_1_STATUS_OUT6_PTR            (  (reg8 *) ScanComp_1_bScanComp_St6_OutReg6__STATUS_REG)
    #define ScanComp_1_STATUS_OUT6_MASK           (* (reg8 *) ScanComp_1_bScanComp_St6_OutReg6__MASK_REG)
    #define ScanComp_1_STATUS_OUT6_AUX_CONTROL    (* (reg8 *) \
                                                         ScanComp_1_bScanComp_St6_OutReg6__STATUS_AUX_CTL_REG)
#endif /* ScanComp_1_NUM_OF_CHANNELS > 48u */

#if (ScanComp_1_NUM_OF_CHANNELS > 56u)
    #define ScanComp_1_STATUS_INT7_REG            (* (reg8 *) ScanComp_1_bScanComp_St7_IntReg7__STATUS_REG)
    #define ScanComp_1_STATUS_INT7_PTR            (  (reg8 *) ScanComp_1_bScanComp_St7_IntReg7__STATUS_REG)
    #define ScanComp_1_STATUS_INT7_MASK           (* (reg8 *) ScanComp_1_bScanComp_St7_IntReg7__MASK_REG)
    #define ScanComp_1_STATUS_INT7_MASK_PTR       (  (reg8 *) ScanComp_1_bScanComp_St7_IntReg7__MASK_REG)
    #define ScanComp_1_STATUS_INT7_AUX_CONTROL    (* (reg8 *) \
                                                        ScanComp_1_bScanComp_St7_IntReg7__STATUS_AUX_CTL_REG)

    #define ScanComp_1_STATUS_OUT7_REG            (* (reg8 *) ScanComp_1_bScanComp_St7_OutReg7__STATUS_REG)
    #define ScanComp_1_STATUS_OUT7_PTR            (  (reg8 *) ScanComp_1_bScanComp_St7_OutReg7__STATUS_REG)
    #define ScanComp_1_STATUS_OUT7_MASK           (* (reg8 *) ScanComp_1_bScanComp_St7_OutReg7__MASK_REG)
    #define ScanComp_1_STATUS_OUT7_AUX_CONTROL    (* (reg8 *) \
                                                        ScanComp_1_bScanComp_St7_OutReg7__STATUS_AUX_CTL_REG)
#endif /* ScanComp_1_NUM_OF_CHANNELS > 56u */

#if (ScanComp_1_NUM_OF_CHANNELS > 63u)
    #define ScanComp_1_STATUS_INT9_REG            (* (reg8 *) ScanComp_1_bScanComp_St9_IntReg9__STATUS_REG)
    #define ScanComp_1_STATUS_INT9_PTR            (  (reg8 *) ScanComp_1_bScanComp_St9_IntReg9__STATUS_REG)
    #define ScanComp_1_STATUS_INT9_MASK           (* (reg8 *) ScanComp_1_bScanComp_St9_IntReg9__MASK_REG)
    #define ScanComp_1_STATUS_INT9_MASK_PTR       (  (reg8 *) ScanComp_1_bScanComp_St9_IntReg9__MASK_REG)
    #define ScanComp_1_STATUS_INT9_AUX_CONTROL    (* (reg8 *) \
                                                        ScanComp_1_bScanComp_St9_IntReg9__STATUS_AUX_CTL_REG)
#endif /* ScanComp_1_NUM_OF_CHANNELS > 63u */

/* Control register */
#define ScanComp_1_CONTROL_REG        (*(reg8 *) ScanComp_1_bScanComp_CtrlCnt__CONTROL_REG)
#define ScanComp_1_CONTROL_PTR        ( (reg8 *) ScanComp_1_bScanComp_CtrlCnt__CONTROL_REG)

/* Count 7 */
#define ScanComp_1_COUNTER_COUNT_REG          (*(reg8 *) ScanComp_1_bScanComp_ChannelCounter__COUNT_REG)
#define ScanComp_1_COUNTER_PERIOD_REG          (*(reg8 *) ScanComp_1_bScanComp_ChannelCounter__PERIOD_REG)
#define ScanComp_1_COUNTER_AUX_CONTROL_REG    (*(reg8 *) \
                                                    ScanComp_1_bScanComp_ChannelCounter__CONTROL_AUX_CTL_REG)
#define ScanComp_1_COUNTER_AUX_CONTROL_PTR    ( (reg8 *) \
                                                    ScanComp_1_bScanComp_ChannelCounter__CONTROL_AUX_CTL_REG)

/***************************************
*   Register Constants
***************************************/

#define ScanComp_1_COUNTER_ENABLE             (0x20u)
#define ScanComp_1_STATUS_INTR_ENBL           (0x10u)
#define ScanComp_1_CONTROL_ENABLE             (0x01u)
#define ScanComp_1_CONTROL_DISABLE            (0x00u)

/* Masks */
#define ScanComp_1_3BIT_MASK                  (0x07u)
#define ScanComp_1_7BIT_MASK                  (0x7Fu)
#define ScanComp_1_INT_REG_MASK               (0x7Fu)

/* INTR_MASK for each channel */
#if (ScanComp_1_NUM_OF_CHANNELS > 1u)
    #define ScanComp_1_INTR_MASK_0    (0x01u)
    #define ScanComp_1_INTR_MASK_1    (0x02u)
    #define ScanComp_1_INTR_MASK_2    (0x04u)
    #define ScanComp_1_INTR_MASK_3    (0x08u)
    #define ScanComp_1_INTR_MASK_4    (0x10u)
    #define ScanComp_1_INTR_MASK_5    (0x20u)
    #define ScanComp_1_INTR_MASK_6    (0x40u)
    #define ScanComp_1_INTR_MASK_7    (0x80u)
#endif /* ScanComp_1_NUM_OF_CHANNELS <= 8u */

#if (ScanComp_1_NUM_OF_CHANNELS > 8u)
    #define ScanComp_1_INTR_MASK_8    (0x01u)
    #define ScanComp_1_INTR_MASK_9    (0x02u)
    #define ScanComp_1_INTR_MASK_10   (0x04u)
    #define ScanComp_1_INTR_MASK_11   (0x08u)
    #define ScanComp_1_INTR_MASK_12   (0x10u)
    #define ScanComp_1_INTR_MASK_13   (0x20u)
    #define ScanComp_1_INTR_MASK_14   (0x40u)
    #define ScanComp_1_INTR_MASK_15   (0x80u)
#endif /* ScanComp_1_NUM_OF_CHANNELS > 8u */

#if (ScanComp_1_NUM_OF_CHANNELS > 16u)
    #define ScanComp_1_INTR_MASK_16   (0x01u)
    #define ScanComp_1_INTR_MASK_17   (0x02u)
    #define ScanComp_1_INTR_MASK_18   (0x04u)
    #define ScanComp_1_INTR_MASK_19   (0x08u)
    #define ScanComp_1_INTR_MASK_20   (0x10u)
    #define ScanComp_1_INTR_MASK_21   (0x20u)
    #define ScanComp_1_INTR_MASK_22   (0x40u)
    #define ScanComp_1_INTR_MASK_23   (0x80u)
#endif /* ScanComp_1_NUM_OF_CHANNELS > 16u */

#if (ScanComp_1_NUM_OF_CHANNELS > 24u)
    #define ScanComp_1_INTR_MASK_24   (0x01u)
    #define ScanComp_1_INTR_MASK_25   (0x02u)
    #define ScanComp_1_INTR_MASK_26   (0x04u)
    #define ScanComp_1_INTR_MASK_27   (0x08u)
    #define ScanComp_1_INTR_MASK_28   (0x10u)
    #define ScanComp_1_INTR_MASK_29   (0x20u)
    #define ScanComp_1_INTR_MASK_30   (0x40u)
    #define ScanComp_1_INTR_MASK_31   (0x80u)
#endif /* ScanComp_1_NUM_OF_CHANNELS > 24u */

#if (ScanComp_1_NUM_OF_CHANNELS > 32u)
    #define ScanComp_1_INTR_MASK_32   (0x01u)
    #define ScanComp_1_INTR_MASK_33   (0x02u)
    #define ScanComp_1_INTR_MASK_34   (0x04u)
    #define ScanComp_1_INTR_MASK_35   (0x08u)
    #define ScanComp_1_INTR_MASK_36   (0x10u)
    #define ScanComp_1_INTR_MASK_37   (0x20u)
    #define ScanComp_1_INTR_MASK_38   (0x40u)
    #define ScanComp_1_INTR_MASK_39   (0x80u)
#endif /* ScanComp_1_NUM_OF_CHANNELS > 32u */

#if (ScanComp_1_NUM_OF_CHANNELS > 40u)
    #define ScanComp_1_INTR_MASK_40   (0x01u)
    #define ScanComp_1_INTR_MASK_41   (0x02u)
    #define ScanComp_1_INTR_MASK_42   (0x04u)
    #define ScanComp_1_INTR_MASK_43   (0x08u)
    #define ScanComp_1_INTR_MASK_44   (0x10u)
    #define ScanComp_1_INTR_MASK_45   (0x20u)
    #define ScanComp_1_INTR_MASK_46   (0x40u)
    #define ScanComp_1_INTR_MASK_47   (0x80u)
#endif /* ScanComp_1_NUM_OF_CHANNELS > 40u */

#if (ScanComp_1_NUM_OF_CHANNELS > 48u)
    #define ScanComp_1_INTR_MASK_48   (0x01u)
    #define ScanComp_1_INTR_MASK_49   (0x02u)
    #define ScanComp_1_INTR_MASK_50   (0x04u)
    #define ScanComp_1_INTR_MASK_51   (0x08u)
    #define ScanComp_1_INTR_MASK_52   (0x10u)
    #define ScanComp_1_INTR_MASK_53   (0x20u)
    #define ScanComp_1_INTR_MASK_54   (0x40u)
    #define ScanComp_1_INTR_MASK_55   (0x80u)
#endif /* ScanComp_1_NUM_OF_CHANNELS > 48u */

#if (ScanComp_1_NUM_OF_CHANNELS > 56u)
    #define ScanComp_1_INTR_MASK_56   (0x01u)
    #define ScanComp_1_INTR_MASK_57   (0x02u)
    #define ScanComp_1_INTR_MASK_58   (0x04u)
    #define ScanComp_1_INTR_MASK_59   (0x08u)
    #define ScanComp_1_INTR_MASK_60   (0x10u)
    #define ScanComp_1_INTR_MASK_61   (0x20u)
    #define ScanComp_1_INTR_MASK_62   (0x40u)
    #define ScanComp_1_INTR_MASK_63   (0x80u)
#endif /* ScanComp_1_NUM_OF_CHANNELS > 56u */

#endif  /* CY_SCANCOMP_ScanComp_1_H */


/* [] END OF FILE */
