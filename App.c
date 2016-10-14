#include "App.h"

#include "Actor.h"
#include "BTLock.h"
#include "command_code.h"
#include "simpleGATTprofile.h"
#include "gatt.h"

uint8 App_TaskID;

/*
 * Task Initialization
 */
void App_Init( uint8 task_id ) {
    App_TaskID = task_id;
    Actor_Init();
}

void App_ProcessOSALMsg(appEvt_t *ptr) {
    static uint8 *verification;
    uint8 *data = ptr -> data;
    uint8 *pwd = data + 1;
    uint8 code = data[0];
    uint8 response[16] = {0};
    if((code & MASK_3BIT) == CC_UNLOCK) {
        uint8 uid = (uint8) (code & ~MASK_3BIT);
        if(verify_account(uid, pwd)){
            if ((uid & 0x3) != 0) {
                // guest
                delete_guest(uid);
            }
            response[0] = (uint8) (code | 0x80);
            btl_unlock();
            osal_start_timerEx(App_TaskID, LOCK_EVT, 2000);
        } else {
            response[0] = 0xff;
            response[1] = CC_UNLOCK;
        }
    } else if ((code & MASK_3BIT) == CC_NEW_PWD) {
        uint8 uid = (uint8) (code & ~MASK_3BIT);
        uint8 u_uid = (uint8) (uid>>2<<2);
        if(verify_account(u_uid, pwd)) {
            generate_new_account_pwd(uid, response + 1);
            response[0] = (uint8) (code | 0x80);
        } else {
            response[0] = 0xff;
            response[1] = CC_NEW_PWD;
        }
    } else if (code == CC_ASK_VERI) {
        verification = start_verification();
        response[0] = (uint8) (CC_ASK_VERI | 0x80);
    } else if (code == CC_ADD_USER) {
        if (NULL != verification) {
            int i = 0;
            for (i = 0; i < 15; i++) {
                if (pwd[i] != verification[i]) {
                    break;
                }
            }
            if (i != 15) {
                response[0] = 0xff;
                response[1] = CC_ADD_USER;
            } else {
                uint8 uid = add_user();
                uint8 *p = get_user_pwd(uid);
                osal_memcpy(response + 1, p, 15);
                response[0] = (uint8) (CC_ADD_USER | uid | 0x80);
                osal_mem_free(p);
            }
            osal_mem_free(verification);
            verification = NULL;
        } else {
            response[0] = 0xff;
            response[1] = CC_ASK_VERI;
        }
    } else if ((code & MASK_3BIT) == CC_DEL_USER) {
        uint8 u = (uint8) ((code >> 2) & 0x7);
        uint8 d = (uint8) (code & 0x3);
        uint8 u_to_del = (uint8) ((u + 1 + d) % 5);
        if (verify_account(u<<2, pwd)) {
            if (delete_user(u_to_del) == SUCCESS) {
                response[0] = (uint8) (code | 0x80);
            } else {
                response[0] = 0xff;
                response[1] = code;
            }
        } else {
            response[0] = 0xff;
            response[1] = CC_UNLOCK;
        }
    } else if ((code & MASK_5BIT) == CC_INQUIRY) {
        uint8 uid = (uint8) ((code & ~MASK_5BIT) << 2);
        if (verify_account(uid, pwd)) {
            response[0] = CC_INQUIRY | 0x80;
            response[1] = get_user_bitset();
        } else {
            response[0] = 0xff;
            response[1] = CC_UNLOCK;
        }
    } else {
        osal_mem_free(data);
        return; // avoid sending response
    }

    osal_mem_free(data);
    osal_msg_deallocate((uint8*)ptr);

    SimpleProfile_SetParameter(BTLOCK_AUTH_CHAR, BTLOCK_AUTH_CHAR_LEN, response);
}

/*
 * Task Event Processor
 */
uint16 App_ProcessEvent( uint8 task_id, uint16 events ) {
    VOID task_id;
    if ( events & SYS_EVENT_MSG )
    {
        uint8 *pMsg;

        if ( (pMsg = osal_msg_receive( App_TaskID )) != NULL )
        {
            App_ProcessOSALMsg( (appEvt_t *)pMsg );

            // Release the OSAL message
            VOID osal_msg_deallocate( pMsg );
        }

        // return unprocessed events
        return (uint16) (events ^ SYS_EVENT_MSG);
    } else if (events & LOCK_EVT) {
        btl_lock();
        Actor_Save();
        return (uint16) (events ^ LOCK_EVT);
    }
}