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

#ifndef __KLV_H__
#define __KLV_H__

#include "lds.h"

G_BEGIN_DECLS
/**
 * @brief Struct representing a Key Length Value (KLV) object
 * 
 */
typedef struct _Klv Klv;

/**
 * @brief Gets the KLV as raw data
 * 
 * @param klv The Klv object
 * @return guchar* The raw data
 */
guchar *klv_raw_data (Klv * klv);

/**
 * @brief Gets Klv length
 * 
 * @param klv The Klv object
 * @return guint The length field or 0 on error
 */
guint klv_packet_length (Klv * klv);

/**
 * @brief Gets Klv key
 * 
 * @param klv The Klv object
 * @param key The key 
 * @return gboolean TRUE on success
 */
gboolean klv_key (Klv * klv, Key * key);

/**
 * @brief Gets Klv length
 * 
 * @param klv The Klv object
 * @return guint The length field or 0 on error
 */
guint klv_length (Klv * klv);

/**
 * @brief Gets Klv value
 * 
 * @param klv The Klv object
 * @return guchar* The value field (use \ref klv_length to 
 * know its length)
 */
guchar *klv_value (Klv * klv);

/**
 * @brief Gets a Klv object from the giving data
 * 
 * @param data Byte data representing a Klv object
 * @param klv The returned Klv object (its memory has to 
 * be released by the user using \ref klv_release)
 * @return gboolean TRUE on success
 */
gboolean klv_get (gpointer data, Klv ** klv);

/**
 * @brief Releases the memory associated with a Klv object
 * 
 * @param klv The Klv object to be released
 */
void klv_release (Klv * klv);

/**
 * @brief Creates a Klv object from the given Key and 
 * Lds
 * 
 * @param key The Klv key
 * @param lds The Lds
 * @param klv The returned Klv object (its memory has to 
 * be released by the user using \ref klv_release)
 * @return gboolean TRUE on success
 */
gboolean klv_from_lds (Key key, Lds * lds, Klv ** klv);

G_END_DECLS
#endif // __KLV_H__
