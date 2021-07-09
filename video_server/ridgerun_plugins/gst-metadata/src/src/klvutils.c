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

#include "klvutils.h"

#include <gst/gst.h>
#include <sys/time.h>

#define BER_OID_CONTINUATION_BIT_MASK 0x80
#define BER_SHORT_LONG_INDICATOR_BIT_MASK 0x80
#define BER_SHORT_MASK 0xFFFFFF80
#define BER_DATA_MASK 0x7F
#define BYTE_MASK 0xFF
#define BER_MAX_VALUE 0xFFFFFF
#define BER_OID_MAX_VALUE 0xFFFFFFF

guint
klv_utils_ber_oid_decode (guchar * data, guint * length)
{
  guint i;
  guint j;
  guint bytes;
  guchar *tmp_data;
  guint val;

  g_return_val_if_fail (data != NULL, 0);

  tmp_data = data;
  bytes = 0;

  /*Get number of bytes in value */
  i = 0;
  while (tmp_data) {
    if ((*tmp_data & BER_OID_CONTINUATION_BIT_MASK) == 0) {
      bytes = i + 1;
      break;
    }
    i++;
    tmp_data++;
  }

  if (bytes > 4) {
    GST_ERROR ("We don't support values of more than 4 bytes");
    return 0;
  }

  /*Calculate value */
  val = 0;
  for (i = bytes, j = 0; i > 0; i--, j++)
    val += (guint) ((data[i - 1] & BER_DATA_MASK) << (j * 7));

  if (length)
    *length = bytes;

  return val;
}

guint
klv_utils_ber_decode (guchar * data, guint * length)
{
  guint val;
  guint tmp;

  g_return_val_if_fail (data != NULL, 0);

  val = 0;
  tmp = 0;

  if ((data[0] & BER_SHORT_LONG_INDICATOR_BIT_MASK) == 0) {     /* BER Short */
    val = (guint) (data[0] & BYTE_MASK);
    tmp = 1;
  } else {                      /* BER Long */

    guint j, k;
    tmp = (guint) (data[0] & BER_DATA_MASK);    /* Number of bytes in length */

    for (j = tmp, k = 0; j > 0; j--, k++)
      val += (guint) ((data[j] & BYTE_MASK) << (k * 8));

    /* We need to add the first byte to the count */
    tmp++;
  }

  if (length)
    *length = tmp;

  return val;
}

guint
klv_utils_ber_encode (guint indata, guint * outdata)
{
  gboolean ber_short;
  guint i, bytes_needed;
  guchar *out, *in;

  g_return_val_if_fail (outdata != NULL, 0);

  ber_short = ((indata & BER_SHORT_MASK) == 0);

  if (ber_short) {
    *outdata = indata;
    return 1;
  }

  for (i = 3; i > 0; i--) {
    if ((indata & (BYTE_MASK << (8 * i))) != 0)
      break;
  }
  bytes_needed = i + 2;

  if (bytes_needed > 4) {
    GST_ERROR ("Can't encode %u, max allowed value is %u", indata,
        BER_MAX_VALUE);
    return 0;
  }

  *outdata = 0;
  out = (guchar *) outdata;
  in = (guchar *) & indata;

  out[0] = (BER_SHORT_LONG_INDICATOR_BIT_MASK + bytes_needed - 1);

  for (i = 1; i < bytes_needed; i++)
    out[i] = in[bytes_needed - 1 - i];

  return bytes_needed;
}

guint
klv_utils_ber_oid_encode (guint indata, guint * outdata)
{
  guint i, bytes_needed;
  guint tmp;
  guchar *out;

  g_return_val_if_fail (outdata != NULL, 0);

  if ((indata & 0xF0000000) != 0) {
    GST_ERROR ("Can't encode %u, max allowed value is %u", indata,
        BER_OID_MAX_VALUE);
    return 0;
  }

  for (i = 3; i > 0; i--) {
    if ((indata & (BER_DATA_MASK << (7 * i))) != 0)
      break;
  }
  bytes_needed = i + 1;

  *outdata = 0;
  out = (guchar *) outdata;
  tmp = indata;
  for (i = bytes_needed; i > 0; i--) {
    out[i - 1] = (tmp & BER_DATA_MASK);
    tmp = tmp >> 7;

    if (i < bytes_needed)
      out[i - 1] |= BER_OID_CONTINUATION_BIT_MASK;
  }

  return bytes_needed;
}

guint16
klv_utils_calculate_checksum (guchar * data, guint length)
{
  guint16 bcc;
  guint i;

  g_return_val_if_fail (data != NULL, -1);

  bcc = 0;
  for (i = 0; i < length; i++)
    bcc += data[i] << (8 * ((i + 1) % 2));

  GST_LOG ("bcc: 0x%04X", bcc);

  return bcc;
}

guint64
klv_utils_get_current_timestamp ()
{
  struct timeval currentTime;
  gettimeofday (&currentTime, NULL);

  return (guint64) (currentTime.tv_sec * (guint) 1e6 + currentTime.tv_usec);
}

gchar *
klv_utils_timestamp_to_string (guint64 timestamp_us)
{
  /*We need the time in seconds */
  time_t rawtime = timestamp_us / 1000000;
  struct tm *timeinfo = localtime (&rawtime);

  return asctime (timeinfo);
}
