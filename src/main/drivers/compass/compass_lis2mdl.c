/*
 * This file is part of Betaflight.
 *
 * Betaflight is free software. You can redistribute this software
 * and/or modify this software under the terms of the GNU General
 * Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later
 * version.
 *
 * Betaflight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this software.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdbool.h>
#include <stdint.h>

#include <math.h>

#include "platform.h"

#if defined(USE_MAG_LIS2MDL)

// LIS2MDL, IIS2MDC, LSM303AGR and LSM303AH are firmware and pin-to-pin compatible solutions
// https://www.st.com/resource/en/design_tip/dt0131-digital-magnetometer-and-ecompass-efficient-design-tips--stmicroelectronics.pdf

#include "common/axis.h"

#include "drivers/time.h"
#include "drivers/sensor.h"

#include "drivers/compass/compass.h"

#define LIS2MDL_MAG_I2C_ADDRESS 0x1E

// LIS2MDL Registers
// Hard-iron Registers
#define LIS2MDL_ADDR_OFFSET_X_REG_L 0x45
#define LIS2MDL_ADDR_OFFSET_X_REG_H 0x46
#define LIS2MDL_ADDR_OFFSET_Y_REG_L 0x47
#define LIS2MDL_ADDR_OFFSET_Y_REG_H 0x48
#define LIS2MDL_ADDR_OFFSET_Z_REG_L 0x49
#define LIS2MDL_ADDR_OFFSET_Z_REG_H 0x4A
// WHO_AM_I
#define LIS2MDL_ADDR_WHO_AM_I   0x4F
// Configuration Registers
#define LIS2MDL_ADDR_CFG_REG_A  0x60
#define LIS2MDL_ADDR_CFG_REG_B  0x61
#define LIS2MDL_ADDR_CFG_REG_C  0x62
// Interrupt Configuration Registers
#define LIS2MDL_ADDR_INT_CTRL_REG   0x63
#define LIS2MDL_ADDR_INT_SOURCE_REG   0x64
#define LIS2MDL_ADDR_INT_THS_L_REG   0x65
#define LIS2MDL_ADDR_INT_THS_H_REG   0x66
// Status Register
#define LIS2MDL_ADDR_STATUS_REG 0x67
// Output Registers
#define LIS2MDL_ADDR_OUTX_L_REG 0x68
#define LIS2MDL_ADDR_OUTX_H_REG 0x69
#define LIS2MDL_ADDR_OUTY_L_REG 0x6A
#define LIS2MDL_ADDR_OUTY_H_REG 0x6B
#define LIS2MDL_ADDR_OUTZ_L_REG 0x6C
#define LIS2MDL_ADDR_OUTZ_H_REG 0x6D
// Tmperature Sensor Registers
#define LIS2MDL_ADDR_TEMP_OUT_L_REG 0x6E
#define LIS2MDL_ADDR_TEMP_OUT_H_REG 0x6F

// LIS2MDL Definitions
#define LIS2MDL_WHO_AM_I                0x40
#define LIS2MDL_STATUS_REG_READY        0x0F
#define LIS2MDL_STATUS_REG_XYZ_READY    0x08
#define CFGA_MD_CONTINUOUS   (0 << 0)
#define CFGA_ODR_100         ((1 << 3) | (1 << 2))
#define CFGA_COMP_TEMP_EN    (1 << 7)
#define CFGB_OFF_CANC        (1 << 1)
#define CFGC_BDU             (1 << 4)

static bool lis2mdlInit(magDev_t *mag)
{
    bool ack = true;

    // busDeviceRegister(dev);

    ack = ack && busWrite(mag->busDev, LIS2MDL_ADDR_CFG_REG_A, CFGA_MD_CONTINUOUS | CFGA_ODR_100 | CFGA_COMP_TEMP_EN);
    ack = ack && busWrite(mag->busDev, LIS2MDL_ADDR_CFG_REG_B, CFGB_OFF_CANC);
    ack = ack && busWrite(mag->busDev, LIS2MDL_ADDR_CFG_REG_C, CFGC_BDU);

    if (!ack) {
        return false;
    }

    // mag->magOdrHz = 100;
    return true;
}

static bool lis2mdlRead(magDev_t *mag)
{
    bool ack;
	uint8_t status = 0;

	ack = busRead(mag->busDev, LIS2MDL_ADDR_STATUS_REG, &status);

	if ( !( ack && ( ( status & LIS2MDL_STATUS_REG_XYZ_READY) >> 3 ) ) ) {
		return false;
	}

	static uint8_t buf[6];

    ack = busReadBuf(mag->busDev, LIS2MDL_ADDR_OUTX_L_REG, (uint8_t *)buf, sizeof(buf));

    if (!ack) {
        mag->magADCRaw[X] = 0;
        mag->magADCRaw[Y] = 0;
        mag->magADCRaw[Z] = 0;
        return false;
    }


    // Sensitivity is +/- 50,000 milligauss, 16bit
    // e.g. gauss = val * (100.f / 65.535f)

    int16_t x = (int16_t)(buf[1] << 8 | buf[0]);
    int16_t y = (int16_t)(buf[3] << 8 | buf[2]);
    int16_t z = (int16_t)(buf[5] << 8 | buf[4]);

    // adapt LIS2MDL left-handed frame to common sensor axis orientation  (match LIS2MDL)
    // pin 1 mark becomes +X -Y
    mag->magADCRaw[X] = -x;
    mag->magADCRaw[Y] = y;
    mag->magADCRaw[Z] = z;

    return true;
}

#define DETECTION_MAX_RETRY_COUNT   5
static bool deviceDetect(magDev_t * mag)
{
    for (int retryCount = 0; retryCount < DETECTION_MAX_RETRY_COUNT; retryCount++) {
        delay(10);

        uint8_t sig = 0;
        bool ack = busRead(mag->busDev, LIS2MDL_ADDR_WHO_AM_I, &sig);

        if (ack && sig == LIS2MDL_WHO_AM_I) {
            return true;
        }
    }

    return false;
}

bool lis2mdlDetect(magDev_t *mag)
{
    mag->busDev = busDeviceInit(BUSTYPE_I2C, DEVHW_LIS2MDL, mag->magSensorToUse, OWNER_COMPASS);
    
    if (mag->busDev == NULL) {
        return false;
    }

    if (!deviceDetect(mag)) {
        busDeviceDeInit(mag->busDev);
        return false;
    }

    mag->init = lis2mdlInit;
    mag->read = lis2mdlRead;

    return true;
}
#endif