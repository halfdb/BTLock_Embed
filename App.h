#ifndef BTLOCK_APP_H
#define BTLOCK_APP_H

#include "hal_types.h"
#include "OSAL.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define LOCK_EVT 0x0001

extern uint8 App_TaskID;

typedef struct {
    osal_event_hdr_t hdr;
    uint8 *data;
} appEvt_t;

/*
 * Task Initialization
 */
extern void App_Init( uint8 task_id );

/*
 * Task Event Processor
 */
extern uint16 App_ProcessEvent( uint8 task_id, uint16 events );

#ifdef __cplusplus
}
#endif

#endif //BTLOCK_APP_H
