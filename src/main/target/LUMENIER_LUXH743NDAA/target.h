/*
 * This file is part of Cleanflight.
 *
 * Cleanflight is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cleanflight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cleanflight.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#define TARGET_BOARD_IDENTIFIER "LLH7"

#define USBD_PRODUCT_STRING "LUXH743NDAA"

#define USE_HARDWARE_PREBOOT_SETUP

// *************** LED **********************
#define LED0   PE3

// *************** Beeper **********************
// ** NOT ACTIVE, not enough timers available **
#define BEEPER   PA8
#define BEEPER_INVERTED

// *************** I2C: BARO & MAG ****************************
// ** DPS310 and LIS2MDL on internal I2C1 **
// ** LIS2MDL not yet supported by INAV! **
#define USE_I2C

#define USE_I2C_DEVICE_1
#define I2C1_SCL                PB8
#define I2C1_SDA                PB9

#define USE_I2C_DEVICE_4
#define USE_I2C4_AF6
#define I2C4_SCL                PB6
#define I2C4_SDA                PB7

#define USE_BARO
#define BARO_I2C_BUS            BUS_I2C1
#define USE_BARO_DPS310

#define USE_MAG
#define MAG_I2C_BUS             BUS_I2C4
#define USE_MAG_ALL

#define TEMPERATURE_I2C_BUS     BUS_I2C4

#define PITOT_I2C_BUS           BUS_I2C4

// *************** ENABLE OPTICAL FLOW & RANGEFINDER *****************************
#define USE_RANGEFINDER
#define USE_RANGEFINDER_MSP
#define RANGEFINDER_I2C_BUS     BUS_I2C4
#define USE_OPFLOW
#define USE_OPFLOW_MSP

// *************** UART *****************************
#define USE_VCP
#define VBUS_SENSING_PIN    PD2

#define USE_UART1
#define UART1_RX_PIN PA10
#define UART1_TX_PIN PA9

#define USE_UART2
#define UART2_RX_PIN PD6
#define UART2_TX_PIN PD5

#define USE_UART3
#define UART3_RX_PIN PD9
#define UART3_TX_PIN PD8

#define USE_UART4
#define UART4_RX_PIN PA1
#define UART4_TX_PIN PA0

#define USE_UART5
#define UART5_RX_PIN PB12
#define UART5_TX_PIN PB13

#define USE_UART7
#define UART7_RX_PIN PE7
#define UART7_TX_PIN PE8

#define USE_UART8
#define UART8_RX_PIN PE0
#define UART8_TX_PIN PE1

#define SERIAL_PORT_COUNT 8 //VCP, USART1, USART2, USART3, UART4, UART5, USART7, USART8

// *************** SPI: Flash, SD Card, OSD, IMU *****************************
// ** Board has ICM-45686 IMU on SPI3, but INAV does not support it yet **
// ** ICM-42688-P IMU on SPI6, but code was added to activate SPI6 **
// ** Default to BLACKBOX on SPI Flash, but enable SD in hadware_setup.c **
#define USE_SPI
#define USE_SPI_DEVICE_1
#define USE_SPI_DEVICE_2
#define USE_SPI_DEVICE_3
#define USE_SPI_DEVICE_4
#define USE_SPI_DEVICE_6

// Flash
#define SPI1_SCK_PIN            PA5
#define SPI1_MISO_PIN           PA6
#define SPI1_MOSI_PIN           PA7

#define USE_FLASHFS
#define USE_FLASH_M25P16
#define M25P16_SPI_BUS  BUS_SPI1
#define M25P16_CS_PIN   PA4
#define ENABLE_BLACKBOX_LOGGING_ON_SPIFLASH_BY_DEFAULT

// SD Card
#define SPI2_SCK_PIN            PB10
#define SPI2_MISO_PIN           PC2
#define SPI2_MOSI_PIN           PC3

#define USE_SDCARD
#define USE_SDCARD_SPI
#define SDCARD_DETECT_INVERTED
#define SDCARD_DETECT_PIN       PE15
#define SDCARD_SPI_BUS          BUS_SPI2
#define SDCARD_CS_PIN           PB11

// ICM-45686 Gyro (No driver yet)
#define SPI3_SCK_PIN            PC10
#define SPI3_MISO_PIN           PC11
#define SPI3_MOSI_PIN           PC12

// MAX7456 OSD
#define SPI4_SCK_PIN            PE2
#define SPI4_MISO_PIN           PE5
#define SPI4_MOSI_PIN           PE6

#define USE_MAX7456
#define MAX7456_SPI_BUS         BUS_SPI4
#define MAX7456_CS_PIN          PE4

// ICM-42688-P Gyro
#define SPI6_SCK_PIN            PB3
#define SPI6_MISO_PIN           PB4
#define SPI6_MOSI_PIN           PB5

#define ICM42605_CS_PIN         PD7
#define ICM42605_SPI_BUS        BUS_SPI6

#define USE_IMU_ICM42605
#define IMU_ICM42605_ALIGN      CW270_DEG_FLIP

// *************** Sensors & ADC *****************************
#define SENSORS_SET (SENSOR_ACC|SENSOR_MAG|SENSOR_BARO)

#define USE_ADC
#define ADC_CHANNEL_1_PIN               PC0
#define ADC_CHANNEL_2_PIN               PC1
#define ADC_CHANNEL_3_PIN               PC4
#define ADC_CHANNEL_4_PIN               PC5
#define VBAT_ADC_CHANNEL                ADC_CHN_1
#define CURRENT_METER_ADC_CHANNEL       ADC_CHN_2
#define AIRSPEED_ADC_CHANNEL            ADC_CHN_3
#define RSSI_ADC_CHANNEL                ADC_CHN_4

// *************** LED Strip *****************************
#define USE_LED_STRIP
#define WS2811_PIN                      PB0

// *************** RC Input *****************************
#define DEFAULT_FEATURES        (FEATURE_TX_PROF_SEL | FEATURE_BLACKBOX)
#define DEFAULT_RX_TYPE         RX_TYPE_SERIAL
#define SERIALRX_PROVIDER       SERIALRX_CRSF

#define USE_SERIAL_4WAY_BLHELI_INTERFACE

// *************** GPIO & PWM Outputs *****************************
#define USE_PINIO
#define USE_PINIOBOX
// PINIO to Switch Camera
#define PINIO1_PIN              PB1
// PINIO to Disable VTX Power
#define PINIO2_PIN              PB2

#define MAX_PWM_OUTPUT_PORTS    15
#define USE_DSHOT
#define USE_ESC_SENSOR

#define TARGET_IO_PORTA 0xffff
#define TARGET_IO_PORTB 0xffff
#define TARGET_IO_PORTC 0xffff
#define TARGET_IO_PORTD 0xffff
#define TARGET_IO_PORTE 0xffff