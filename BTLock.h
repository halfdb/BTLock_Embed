/*--BTLock.h--*/
#ifndef BTLOCK_H
#define BTLOCK_H

#include "hal_types.h"
#include "comdef.h"

/* Macros */
#define PARAM_LEN 15

/* Typedefs */
typedef uint8 Param[PARAM_LEN];

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
extern uint8 read_flash(uint8 position, uint8 size, void *p_buffer);

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
extern uint8 write_flash(uint8 position, uint8 size, void *p_buffer);

extern /* Param */ uint8* start_verification();

/*********************************************************************
 * @fn      btl_rand
 *
 * @brief   A random byte.
 *
 * @return  The random number.
 */
extern uint8 btl_rand();

/**
 * @brief   A random 16-bit number.
 * @return  The random number.
 */
extern uint16 btl_rand16();

/* Lock Manipulations */

extern void btl_unlock();
extern void btl_lock();

#endif