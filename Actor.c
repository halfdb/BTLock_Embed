/*--Actor.c--*/
#include "Actor.h"
#include "uid_def.h"

/* Account System */

/*********************************************************************
 * @fn      verify_account
 *
 * @brief   Verify whether the password is true for this user id.
 *
 * @return  TRUE or FALSE.
 */
bool verify_account(uint8 uid, Param password) {

}

/*********************************************************************
 * @fn      generate_guest_pwd
 *
 * @brief   Generate and register a guest password.
 *
 * @return  SUCCESS or FAILURE. The password is filled in the Param.
 */
uint8 generate_guest_pwd(uint8 uid, Param pwd) {

}

/*********************************************************************
 * @fn      destroy_guest_pwd
 *
 * @brief   Destroy and de-register a guest password given by the Param.
 *
 * @return  SUCCESS or FAILURE.
 */
uint8 destroy_guest_pwd(uint8 uid) {

}

/* Admin passwords are used by admins. */
//extern uint8 get_admin_pwd(Param);
//extern uint8 reset_admin_pwd(Param);

/* General passwords are used by users. */
uint8 * get_user_pwd(uint8 uid) {

}

/* Account management */

/**
 * @brief           Add a new user.
 * @return          Uid of the new user. Return 255 if failure.
 */
uint8 add_user() {

}

/**
 * @brief       Delete indicated user with the uid. Leave the uid blank.
 * @param uid   The uid of the user.
 * @return      SUCCESS or FAILURE.
 */
uint8 delete_user(uint8 uid) {

}

bool has_user(uint8 uid) {

}

/* Misc */
