/*--Actor.c--*/
#include "Actor.h"
#include "uid_def.h"
#include "OSAL.h"

/* Shared variables, typedefs and macros */

uint8 account_bitsets[4] = {0, 0, 0, 0};
uint8 pwd_modified[4] = {0, 0, 0, 0};
#define BITSET_POSITION 0xA0
Param passwords[20] = {0};
#define INDEX_FOR_UID(I) ((I)-UID_10)
#define POSITION_FOR_UID(I) ((uint8)(INDEX_FOR_UID(I)+BITSET_POSITION+1))

/* Account System */

void read_pwd(uint8 uid) {
    read_flash(POSITION_FOR_UID(uid), PARAM_LEN, passwords[INDEX_FOR_UID(uid)]);
}

void save_pwd(uint8 uid) {
    write_flash(POSITION_FOR_UID(uid), PARAM_LEN, passwords[INDEX_FOR_UID(uid)]);
}

void Actor_Init() {
    read_flash(BITSET_POSITION, 4, account_bitsets);
    uint8 u;
    for (u = 1; u <= 5; u++) {
        uint8 u_uid = UID_OF(u, 0);
        if (has_account(u_uid)) {
            read_pwd(u_uid);
            uint8 uid;
            for (uid = u_uid; uid <= u_uid+3; uid++) {
                if (has_account(uid)) {
                    read_pwd(uid);
                }
            }
        }
    }
    osal_memset(pwd_modified, 0, 4);
}

void Actor_Save() {
    write_flash(BITSET_POSITION, 4, account_bitsets);
    uint8 g, u;
    for (g = 0; g < 4; g++) {
        uint8 m = pwd_modified[g];
        for (u = 1; u <= 5; u++) {
            if (m & (1<<u)) {
                save_pwd(UID_OF(u, g));
            }
        }
        pwd_modified[g] = 0;
    }
}

/*********************************************************************
 * @fn      verify_account
 *
 * @brief   Verify whether the password is true for this user id.
 *
 * @return  TRUE or FALSE.
 */
bool verify_account(uint8 uid, Param password) {
    if (has_account(uid)) {
        int idx = INDEX_FOR_UID(uid);
        return osal_memcmp(passwords[idx], password, PARAM_LEN);
    }
    return FALSE;
}

void generate_pwd(Param pwd) {
    int i;
    uint16 *p = (uint16 *) pwd;
    for(i=0; i<PARAM_LEN/2; i++) {
        p[i] = btl_rand16();
    }
    if(PARAM_LEN & 1) {
        pwd[PARAM_LEN - 1] = btl_rand();
    }
}

/*********************************************************************
 * @brief   Generate and register a password.
 *
 * @return  SUCCESS or FAILURE. The password is filled in the Param.
 */
uint8 generate_new_account_pwd(uint8 uid, Param pwd) {
    uint8 u = U_FROM_UID(uid);
    if((account_bitsets[0] & (1<<u)) == 0) {
        return FAILURE;
    }
    uint8 g = G_FROM_UID(uid);
    generate_pwd(pwd);
    osal_memcpy(passwords[INDEX_FOR_UID(uid)], pwd, PARAM_LEN);
    account_bitsets[g] |= (1<<u);
    pwd_modified[g] |= (1<<u);
    return SUCCESS;
}

/*********************************************************************
 * @brief   Destroy and de-register a guest with the uid.
 *
 * @return  SUCCESS or FAILURE.
 */
uint8 delete_guest(uint8 uid) {
    uint8 u = U_FROM_UID(uid), g = G_FROM_UID(uid);
    if (g == 0) return FAILURE;
    account_bitsets[g] &= (uint8) ~(1<<u);
    osal_memset(passwords[INDEX_FOR_UID(uid)], 0, PARAM_LEN);
    pwd_modified[g] |= 1<<u;
    return SUCCESS;
}

/* Admin passwords are used by admins. */
//extern uint8 get_admin_pwd(Param);
//extern uint8 reset_admin_pwd(Param);

/* General passwords are used by users. */
uint8 *get_user_pwd(uint8 uid) {
    uint8 *ret = osal_mem_alloc(PARAM_LEN);
    if (uid < UID_10 || uid > UID_53){
        osal_memset(ret, 0, PARAM_LEN);
    } else {
        osal_memcpy(ret, passwords[INDEX_FOR_UID(uid)], PARAM_LEN);
    }
    return ret;
}

/* Account management */

/**
 * @brief           Add a new user.
 * @return          Uid of the new user. Return 255 if failure.
 */
uint8 add_user() {
    uint8 u = 1;
    for (; u <= 5; u++) {
        uint8 mask = (uint8) (1<<u);
        if ((uint8) (mask & account_bitsets[0]) == 0) {
            account_bitsets[0] |= mask;
            generate_pwd(passwords[INDEX_FOR_UID(UID_OF(u, 0))]);
            pwd_modified[0] |= mask;
            return UID_OF(u, 0);
        }
    }
    return 0xff;
}

/**
 * @brief       Delete indicated user with the uid. Leave the uid blank.
 * @param u     The u of the user uid.
 * @return      SUCCESS or FAILURE.
 */
uint8 delete_user(uint8 u) {
    account_bitsets[0] &= (uint8) ~(1<<u);
    osal_memset(passwords[INDEX_FOR_UID(UID_OF(u, 0))], 0, PARAM_LEN);
    pwd_modified[0] |= 1<<u;
    uint8 g;
    bool ret = TRUE;
    for (g=1; g<4; g++) {
        ret &= (delete_guest(UID_OF(u, g)) == SUCCESS);
    }
    if (ret) {
        return SUCCESS;
    } else {
        return FAILURE;
    }
}

uint8 get_user_bitset() {
    return account_bitsets[0];
}

bool has_account(uint8 uid){
    if (uid & 3) {
        if (!has_account(uid & 0x1C)) { // 0x1C == 0b11100
            return FALSE;
        }
    }
    uint8 g = G_FROM_UID(uid);
    uint8 u = U_FROM_UID(uid);
    if ((account_bitsets[g] & (1 << u)) != 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/* Misc */
