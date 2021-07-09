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

#ifndef __KLV_UTILS_H__
#define __KLV_UTILS_H__

#include <glib.h>

G_BEGIN_DECLS

/**
 * @brief Decodes the input BER-OID encoded data 
 * 
 * @param data BER-OID encoded data. The maximum
 * supported value is 0xFFFFFF7F which represents 
 * the number 0xFFFFFFF
 * @param length Returns the number of bytes in data 
 * containing the encoded value. Set to NULL if not needed
 * @return guint The decoded value
 */
  guint klv_utils_ber_oid_decode (guchar * data, guint * length);

/**
 * @brief Decodes the input BER encoded data. The maximum 
 * suppored value is 0x83FFFFFF which represents the number
 * 0xFFFFFF
 * 
 * @param data BER encoded (short or long) data
 * @param length Returns the number of bytes in data 
 * containing the encoded value. Set to NULL if not needed
 * @return guint The decoded value
 */
  guint klv_utils_ber_decode (guchar * data, guint * length);

/**
 * @brief Encodes input data in BER-OID format. 
 * 
 * @param indata  Input data. The maximum supported 
 * value is 0xFFFFFFF which will be represented as
 * 0xFFFFFF7F
 * @param outdata The output BER-OID encoded data ( it
 * has to be a valid pointer)
 * @return guint The number of bytes used to encode the value
 * or 0 on error
 */
  guint klv_utils_ber_oid_encode (guint indata, guint * outdata);

/**
 * @brief Encodes input data as BER (short or long)
 * 
 * @param indata The input data. The maximum allowed
 * value is 0xFFFFFF which will be represented as 
 * 0x83FFFFFF
 * @param outdata The output BER encoded data ( it
 * has to be a valid pointer)
 * @return guint The number of bytes used to encode the value
 * or 0 on error
 */
  guint klv_utils_ber_encode (guint indata, guint * outdata);

/**
 * @brief Calculates checksum of input data as specified
 * in ST0601.14, section 8.1.1.1
 * 
 * @param data The data whose checksum will be calculated
 * @param length The length of the input data in bytes.
 * @return guint16 The resulting checksum value or 
 * -1 on error.
 */
  guint16 klv_utils_calculate_checksum (guchar * data, guint length);

/**
 * @brief Gets the current time in microseconds
 * 
 * @return guint64 The current time in microseconds
 */
  guint64 klv_utils_get_current_timestamp ();

/**
 * @brief Conversts the input timestamp to a 
 * human readable string
 * 
 * @param timestamp_us The input timestamp in microseconds
 * @return gchar* Human readable String representing the 
 * input timestamp.
 */
  gchar *klv_utils_timestamp_to_string (guint64 timestamp_us);

G_END_DECLS
#endif                          // __KLV_UTILS_H__
