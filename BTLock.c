/*--BTLock.c--*/
#include "BTLock.h"
#include "hal_led.h"
#include "hal_lcd.h"
#include "hal_lcd.c"
#include "OSAL.h"
#include "osal_snv.h"

bool in_range(uint16 position) {
    if (position >= 0x80 && position < 0xF0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/* Hardware Manipulation */
/*********************************************************************
 * @fn      read_flash
 *
 * @brief   Read specific blocks in the flash.
 *
 * @param   position - the offset of the intended block.
 * @param   size - the size of the data.
 * @param   p_buffer - data will be placed in this array.
 *
 * @return  SUCCESS if successful. FAILURE, otherwise.
 */
uint8 read_flash(uint8 position, uint8 size, void *p_buffer) {
    if (!in_range(position)) {
        return FAILURE;
    }
    if(osal_snv_read(position, size, p_buffer) == SUCCESS) {
        return SUCCESS;
    } else {
        return FAILURE;
    }
}

/*********************************************************************
 * @fn      write_flash
 *
 * @brief   Write specific blocks in the flash.
 *
 * @param   position - the offset of the intended block.
 * @param   size - the size of the data.
 * @param   p_buffer - data will be placed in this array.
 *
 * @return  SUCCESS if successful. FAILURE, otherwise.
 */
uint8 write_flash(uint8 position, uint8 size, void *p_buffer) {
    if (!in_range(position)) {
        return FAILURE;
    }
    if(osal_snv_write(position, size, p_buffer) == SUCCESS) {
        return SUCCESS;
    } else {
        return FAILURE;
    }
}

/*********************************************************************
 * @fn      btl_rand
 *
 * @brief   A random byte.
 *
 * @return  The random number.
 */
uint8 btl_rand() {
    return (uint8) btl_rand16() ;
}

uint16 btl_rand16() {
    return osal_rand();
}

/* Lock Manipulations */

void btl_unlock()
{
#if (defined HAL_LCD) && (HAL_LCD == TRUE)
//    HalLcdWriteString("Door Unlocked", HAL_LCD_LINE_4);
    LCD_CLS();
    LCD_P6x8Str(0, 3, "Door Unlocked");
#endif // (defined HAL_LCD) && (HAL_LCD == TRUE)
    HalLedSet(HAL_LED_1, HAL_LED_MODE_ON);
    HalLedSet(HAL_LED_2, HAL_LED_MODE_OFF);
}

void btl_lock()
{
#if (defined HAL_LCD) && (HAL_LCD == TRUE)
    LCD_CLS();
//    HalLcdWriteString("Door Locked", HAL_LCD_LINE_4);
    LCD_P6x8Str(0, 3, "Door Lockde");
#endif // (defined HAL_LCD) && (HAL_LCD == TRUE)
    HalLedSet(HAL_LED_1, HAL_LED_MODE_OFF);
    HalLedSet(HAL_LED_2, HAL_LED_MODE_ON);
}