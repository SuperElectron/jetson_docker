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

#ifndef __LDS_H__
#define __LDS_H__

#include <gst/gst.h>

#include "klvutils.h"
#include "misbstd.h"

G_BEGIN_DECLS

/**
 * @brief List representing a Local Data Set (Lds)
 * 
 */
  typedef GSList Lds;

/**
 * @brief Get Lds struct from the given data
 * 
 * @param data Raw data representing an Lds. It has to be 
 * released by the user using g_free()
 * @param length The length of the data in bytes
 * @param lds The returned Lds struct. The user is responsible
 * for releasing this memory by calling \ref lds_release()
 * @return gboolean TRUE if the Lds was successfully obtained. 
 */
  gboolean lds_get (guchar * data, guint length, Lds ** lds);

/**
 * @brief Releases the memory associated with lds
 * 
 * @param lds The object to be released
 */
  void lds_release (Lds * lds);

/**
 * @brief Gets the first element of the Lds
 * 
 * @param lds The lds object
 * @return Lds* The first element or NULL on error
 */
  Lds *lds_first (Lds * lds);

/**
 * @brief Gets the last element of the Lds
 * 
 * @param lds The lds object.
 * @return Lds* The last element or NULL on error.
 */
  Lds *lds_last (Lds * lds);

/**
 * @brief Gets the Next element on the Lds
 * 
 * @param lds The Lds object
 * @return Lds* The NEXT element or NULL if there
 * is no next element.
 */
  Lds *lds_next (Lds * lds);

/**
 * @brief Gets the tag for the given Lds
 * 
 * @param lds Lds object
 * @return guint The elelemt tag or 0 o error
 */
  guint lds_tag (Lds * lds);

/**
 * @brief Gets the length of the given Lds
 * 
 * @param lds Lds object
 * @return guint The element length or 0 on error
 */
  guint lds_length (Lds * lds);

/**
 * @brief Get the value field for the given Lds
 * 
 * @param lds The Lds object
 * @return guchar* The value field (use \ref lds_length to get 
 * its length)
 */
  guchar *lds_value (Lds * lds);

/**
 * @brief Adds the given tag, length, value as a new element
 * to the beginning of the given Lds. If lds doesn't exist, 
 * it will be created.
 * 
 * @param lds Lds object in which the new element will be added
 * @param tag The new tag
 * @param length The new length
 * @param value The new value. The memory associated with value has to 
 * be released by the user.
 * @return gboolean TRUE on success
 */
  gboolean lds_add_first (Lds ** lds, guint tag, guint length, guchar * value);

/**
 * @brief Same as \ref lds_add_first but value is internally copied
 * 
 * @param lds Lds object in which the new element will be added
 * @param tag The new tag
 * @param length The new length
 * @param value The new value
 * @return gboolean TRUE on success
 */
  gboolean lds_add_first_as_copy (Lds ** lds, guint tag, guint length,
      guchar * value);

/**
 * @brief Adds the given tag, length, value as a new element
 * to the end of the given Lds. If lds doesn't exist, 
 * it will be created.
 * 
 * @param lds Lds object in which the new element will be added
 * @param tag The new tag
 * @param length The new length
 * @param value The new value. The memory associated with value has to 
 * be released by the user.
 * @return gboolean TRUE on success
 */
  gboolean lds_add_last (Lds ** lds, guint tag, guint length, guchar * value);

/**
 * @brief Same as \ref lds_add_last but value is internally copied.
 * 
 * @param lds Lds object in which the new element will be added
 * @param tag The new tag
 * @param length The new length
 * @param value The new value.
 * @return gboolean TRUE on success
 */
  gboolean lds_add_last_as_copy (Lds ** lds, guint tag, guint length,
      guchar * value);

/**
 * @brief Removes the last element on the given Lds.
 * If lds contains only one element, it will be released
 * 
 * @param lds The Lds whose last element is going to be removed
 * @return gboolean TRUE on success
 */
  gboolean lds_remove_last (Lds * lds);

/**
 * @brief Gets the given lds as Raw (byte) data
 * 
 * @param lds The Lds
 * @param data The output data. It has to be released
 * by the user.
 * @return guint The size of the data in bytes
 */
  guint lds_raw_data (Lds * lds, guchar ** data);

/**
 * @brief Calculates and adds a checksum to the 
 * given Lds
 * 
 * @param lds  The Lds object
 * @param key The key
 * @return gboolean TRUE on success
 */
  gboolean lds_add_checksum (Lds * lds, Key key);

G_END_DECLS
#endif                          // __LDS_H__
