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

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "platform.h"

#include "build/build_config.h"

#include "drivers/time.h"
#include "drivers/bus_spi.h"
#include "drivers/io.h"
#include "drivers/io_impl.h"

#include "drivers/sdcard/sdcard.h"
#include "io/asyncfatfs/asyncfatfs.h"

#ifdef USE_HARDWARE_PREBOOT_SETUP

void initialisePreBootHardware(void)
{
/**
* Initialize the SD card
*
* SD card is not initialized by default if ENABLE_BLACKBOX_LOGGING_ON_SPIFLASH_BY_DEFAULT
*
* Without this, the board will hardfault when user tries 
* to access Blackbox settings in the Configurator.
*/
    sdcardInsertionDetectInit();
    sdcard_init();
    afatfs_init();
}

#endif