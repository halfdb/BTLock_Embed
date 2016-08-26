/*--Actor.h--*/

#ifndef ACTOR_H
#define ACTOR_H

#include "BTLock.h"

/* Typedefs */
//typedef enum
//{
//  ADMIN,
//  USER,
//  GUEST,
//} UserType;

/*
 * Type Processors take a Param as a parameter, in which the functions read or
 * write necessary information. Their returns are usually SUCCESS or FAILURE,
 * indicating the result of the process.
 */
//typedef uint8 Processor(Param);

/* Account System */

/*********************************************************************
 * @fn      verify_account
 *
 * @brief   Verify whether the password is true for this user id.
 *
 * @return  TRUE or FALSE.
 */
extern bool verify_account(uint8 uid, Param password);

/*********************************************************************
 * @fn      generate_guest_pwd
 *
 * @brief   Generate and register a guest password.
 *
 * @return  SUCCESS or FAILURE. The password is filled in the Param.
 */
extern uint8 generate_guest_pwd(Param);

/*********************************************************************
 * @fn      destroy_guest_pwd
 *
 * @brief   Destroy and de-register a guest password given by the Param.
 *
 * @return  SUCCESS or FAILURE.
 */
extern uint8 destroy_guest_pwd(Param);

/* Admin passwords are used by admins. */
//extern uint8 get_admin_pwd(Param);
//extern uint8 reset_admin_pwd(Param);

/* General passwords are used by users. */
extern Param get_user_pwd(uint8 uid);

/* Account management */

/**
 * @brief           Add a new user.
 * @return          Uid of the new user. Return 255 if failure.
 */
extern uint8 add_user();

/**
 * @brief       Delete indicated user with the uid. Leave the uid blank.
 * @param uid   The uid of the user.
 * @return      SUCCESS or FAILURE.
 */
extern uint8 delete_user(int uid);

/* Misc */

#endif