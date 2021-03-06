/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

/*! @name PORTB17 (number 40), PUSH_BUTTON1
  @{ */
#define BOOT_PIN_SW3_GPIO GPIOB /*!<@brief GPIO device name: GPIOB */
#define BOOT_PIN_SW3_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOOT_PIN_SW3_PIN 17U    /*!<@brief PORTB pin index: 17 */
                                /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitBootPin(void);

/*! @name PORTE1 (number 2), J2[20]/UART1_RX_TGTMCU
  @{ */
#define UART1_POLLFORACTIVITY_RX_GPIO_GPIO GPIOE /*!<@brief GPIO device name: GPIOE */
#define UART1_POLLFORACTIVITY_RX_GPIO_PORT PORTE /*!<@brief PORT device name: PORTE */
#define UART1_POLLFORACTIVITY_RX_GPIO_PIN 1U     /*!<@brief PORTE pin index: 1 */
                                                 /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void UART1_PollForActivity(void);

#define SOPT5_UART1TXSRC_UART_TX 0x00u /*!<@brief UART 1 transmit data source select: UART1_TX pin */

/*! @name PORTE1 (number 2), J2[20]/UART1_RX_TGTMCU
  @{ */
#define UART1_RX_PORT PORTE /*!<@brief PORT device name: PORTE */
#define UART1_RX_PIN 1U     /*!<@brief PORTE pin index: 1 */
                            /* @} */

/*! @name PORTE0 (number 1), J2[18]/UART1_TX_TGTMCU
  @{ */
#define UART1_TX_PORT PORTE /*!<@brief PORT device name: PORTE */
#define UART1_TX_PIN 0U     /*!<@brief PORTE pin index: 0 */
                            /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void UART1_InitPins(void);

/*! @name PORTE1 (number 2), J2[20]/UART1_RX_TGTMCU
  @{ */
#define UART1_RESTOREDEFAULT_RX_PORT PORTE /*!<@brief PORT device name: PORTE */
#define UART1_RESTOREDEFAULT_RX_PIN 1U     /*!<@brief PORTE pin index: 1 */
                                           /* @} */

/*! @name PORTE0 (number 1), J2[18]/UART1_TX_TGTMCU
  @{ */
#define UART1_RESTOREDEFAULT_TX_PORT PORTE /*!<@brief PORT device name: PORTE */
#define UART1_RESTOREDEFAULT_TX_PIN 0U     /*!<@brief PORTE pin index: 0 */
                                           /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void UART1_RestoreDefault(void);

/*! @name PORTC10 (number 55), J1[13]/I2C1_SCL/I2S0_RX_FS
  @{ */
#define I2C1_SCL_PORT PORTC /*!<@brief PORT device name: PORTC */
#define I2C1_SCL_PIN 10U    /*!<@brief PORTC pin index: 10 */
                            /* @} */

/*! @name PORTC11 (number 56), J2[7]/I2C1_SDA
  @{ */
#define I2C1_SDA_PORT PORTC /*!<@brief PORT device name: PORTC */
#define I2C1_SDA_PIN 11U    /*!<@brief PORTC pin index: 11 */
                            /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void I2C1_InitPins(void);

/*! @name PORTC10 (number 55), J1[13]/I2C1_SCL/I2S0_RX_FS
  @{ */
#define I2C1_RESTOREDEFAULT_SCL_PORT PORTC /*!<@brief PORT device name: PORTC */
#define I2C1_RESTOREDEFAULT_SCL_PIN 10U    /*!<@brief PORTC pin index: 10 */
                                           /* @} */

/*! @name PORTC11 (number 56), J2[7]/I2C1_SDA
  @{ */
#define I2C1_RESTOREDEFAULT_SDA_PORT PORTC /*!<@brief PORT device name: PORTC */
#define I2C1_RESTOREDEFAULT_SDA_PIN 11U    /*!<@brief PORTC pin index: 11 */
                                           /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void I2C1_RestoreDefault(void);

/*! @name PORTD4 (number 61), J2[6]/SPI0_PCS1/LLWU_P14
  @{ */
#define SPI1_PCS_PORT PORTD /*!<@brief PORT device name: PORTD */
#define SPI1_PCS_PIN 4U     /*!<@brief PORTD pin index: 4 */
                            /* @} */

/*! @name PORTD5 (number 62), J2[12]/BLUE_LED
  @{ */
#define SPI1_SCK_PORT PORTD /*!<@brief PORT device name: PORTD */
#define SPI1_SCK_PIN 5U     /*!<@brief PORTD pin index: 5 */
                            /* @} */

/*! @name PORTD6 (number 63), J2[8]
  @{ */
#define SPI1_SOUT_PORT PORTD /*!<@brief PORT device name: PORTD */
#define SPI1_SOUT_PIN 6U     /*!<@brief PORTD pin index: 6 */
                             /* @} */

/*! @name PORTD7 (number 64), J2[10]
  @{ */
#define SPI1_SIN_PORT PORTD /*!<@brief PORT device name: PORTD */
#define SPI1_SIN_PIN 7U     /*!<@brief PORTD pin index: 7 */
                            /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void SPI1_InitPins(void);

/*! @name PORTD4 (number 61), J2[6]/SPI0_PCS1/LLWU_P14
  @{ */
#define SPI1_RESTOREDEFAULT_PCS_PORT PORTD /*!<@brief PORT device name: PORTD */
#define SPI1_RESTOREDEFAULT_PCS_PIN 4U     /*!<@brief PORTD pin index: 4 */
                                           /* @} */

/*! @name PORTD5 (number 62), J2[12]/BLUE_LED
  @{ */
#define SPI1_RESTOREDEFAULT_SCK_PORT PORTD /*!<@brief PORT device name: PORTD */
#define SPI1_RESTOREDEFAULT_SCK_PIN 5U     /*!<@brief PORTD pin index: 5 */
                                           /* @} */

/*! @name PORTD6 (number 63), J2[8]
  @{ */
#define SPI1_RESTOREDEFAULT_SOUT_PORT PORTD /*!<@brief PORT device name: PORTD */
#define SPI1_RESTOREDEFAULT_SOUT_PIN 6U     /*!<@brief PORTD pin index: 6 */
                                            /* @} */

/*! @name PORTD7 (number 64), J2[10]
  @{ */
#define SPI1_RESTOREDEFAULT_SIN_PORT PORTD /*!<@brief PORT device name: PORTD */
#define SPI1_RESTOREDEFAULT_SIN_PIN 7U     /*!<@brief PORTD pin index: 7 */
                                           /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void SPI1_RestoreDefault(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
