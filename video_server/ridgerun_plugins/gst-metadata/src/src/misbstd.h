/*  
 *  Copyright (C) 2019 RidgeRun, LLC (http://www.ridgerun.com)
 *  All Rights Reserved. 
 *
 *  The contents of this software are proprietary and confidential to RidgeRun, 
 *  LLC.  No part of this program may be photocopied, reproduced or translated 
 *  into another programming language without prior written consent of
 *  RidgeRun, LLC.  The user is free to modify the source code after obtaining 
 *  a software license from RidgeRun.  All source code changes must be provided 
 *  back to RidgeRun without any encumbrance. 
 */

#ifndef __MISB_STD__
#define __MISB_STD__

#include <glib.h>

G_BEGIN_DECLS

#define MISB_KEY_SIZE 16

  typedef struct _MisbStd MisbStd;

/**
 * @brief 16 byte Klv  key
 * 
 */
  typedef struct _Key Key;

  struct _Key
  {
    union
    {
      guint word[4];
      guchar byte[16];
    };
  };

  typedef enum
  {
    MISB_RET_OK = 0,
    MISB_RET_NO_TIMESTAMP = 1,
    MISB_RET_WRONG_TIMESTAMP = 2,
    MISB_RET_NO_CHECKSUM = 3,
    MISB_RET_WRONG_CHECKSUM = 4,
    MISB_RET_WRONG_KEY = 5,
    MISB_RET_NO_LDS = 6,
    MISB_RET_INVALID_ARGUMENT = 7,
    MISB_RET_FIXED = 8
  } MisbReturn;

  struct _MisbStd
  {
    /**
     * @brief Validates the input key is correct
     * @param key The key to be validated
     * @return MisbReturn Return code (One of \ref MisbReturn)
     * 
     */
    MisbReturn (*validate_key) (Key key);
    /**
     * @brief Validates the input data is correct
     * @param data The data to be validated
     * @param new_data If defined and input data is wrong, 
     * a fixed version (if possible) of data should be returned
     * here. Set to NULL if you only want to check input data.
     * @param dump Whether or not to dump data
     * @return MisbReturn Return code (One of \ref MisbReturn)
     * 
     */
    MisbReturn (*validate_data) (gpointer data, gpointer * new_data,
        gboolean dump);
  };


G_END_DECLS
#endif                          // __MISB_STD__
