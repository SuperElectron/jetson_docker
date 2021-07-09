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
#include "st0604dictionary.h"

#include <string.h>

#include "klv.h"
#include "misbstd.h"

static const Key st_0604_mpeg2_h264_key = {
  .byte = {
        0x4D, 0x49, 0x53, 0x50,
        0x6D, 0x69, 0x63, 0x72,
        0x6F, 0x73, 0x65, 0x63,
      0x74, 0x69, 0x6D, 0x65}
};

/*
ST0604 packet format

BYTE:     0  1  2  3  4  5  6  7  8  9  10  11  12  13  14  15
Content:      Identifier (Ascii value for MISPmicrosectime) 

Byte        Content
16          Status Byte
17,18       Byte 0 and 1 of TimeStamp (MSB)
19          0xFF
20,21       Byte 2 and 3 of TimeStamp
22          0xFF
23,24       Byte 4 and 5 of TimeStamp
25          0xFF
26,27       Byte 6 and 7 of TimeStamp (LSB)
*/

#define ST0604_PREC_TIMESTAMP_STATUS_BYTE 16
#define ST0604_START_CODE_EMUL_PREVENT_VAL 0xFF
#define ST0604_START_CODE_EMUL_PREVENT_BYTE0 19
#define ST0604_START_CODE_EMUL_PREVENT_BYTE1 22
#define ST0604_START_CODE_EMUL_PREVENT_BYTE2 25

MisbReturn
st0604_validate_key (Key key)
{
  if (memcmp (&key, &st_0604_mpeg2_h264_key,
          sizeof (st_0604_mpeg2_h264_key)) != 0) {
    GST_DEBUG ("Provided key is not ST0604 key");
    return MISB_RET_WRONG_KEY;
  }
  return MISB_RET_OK;
}

static void
st0604_dump (Klv * klv)
{
  guchar *tmp_data;
  Key key;
  guint i;

  g_return_if_fail (klv != NULL);

  tmp_data = klv_raw_data (klv);
  klv_key (klv, &key);

  GST_LOG ("Showing ST0604 content");

  g_print ("=========================\n");

  g_print ("key:\t");
  for (i = 0; i < MISB_KEY_SIZE; i++)
    g_print ("%01X ", key.byte[i]);

  g_print ("\nStatus:\t%02X\n", tmp_data[ST0604_PREC_TIMESTAMP_STATUS_BYTE]);

  g_print ("Value:\t");
  g_print ("%02X %02X %02X %02X %02X %02X %02X %02X\n",
      tmp_data[ST0604_PREC_TIMESTAMP_STATUS_BYTE + 1],
      tmp_data[ST0604_PREC_TIMESTAMP_STATUS_BYTE + 2],
      tmp_data[ST0604_START_CODE_EMUL_PREVENT_BYTE0 + 1],
      tmp_data[ST0604_START_CODE_EMUL_PREVENT_BYTE0 + 2],
      tmp_data[ST0604_START_CODE_EMUL_PREVENT_BYTE1 + 1],
      tmp_data[ST0604_START_CODE_EMUL_PREVENT_BYTE1 + 2],
      tmp_data[ST0604_START_CODE_EMUL_PREVENT_BYTE2 + 1],
      tmp_data[ST0604_START_CODE_EMUL_PREVENT_BYTE2 + 2]);

  g_print ("=========================\n");
}

MisbReturn
st0604_validate_data (gpointer data, gpointer * new_data, gboolean dump)
{
  Klv *klv;
  guchar *tmp_data;

  g_return_val_if_fail (data != NULL, MISB_RET_INVALID_ARGUMENT);

  klv = (Klv *) data;
  tmp_data = klv_raw_data (klv);

  /*Validate
     We will just check for the Start Code Emulation Prevention Bytes */
  if ((tmp_data[ST0604_START_CODE_EMUL_PREVENT_BYTE0] !=
          ST0604_START_CODE_EMUL_PREVENT_VAL)
      || (tmp_data[ST0604_START_CODE_EMUL_PREVENT_BYTE1] !=
          ST0604_START_CODE_EMUL_PREVENT_VAL)
      || (tmp_data[ST0604_START_CODE_EMUL_PREVENT_BYTE2] !=
          ST0604_START_CODE_EMUL_PREVENT_VAL))
    return MISB_RET_WRONG_TIMESTAMP;

  if (dump)
    st0604_dump (klv);

  return MISB_RET_OK;
}

void
st0604_get (MisbStd * std)
{
  g_return_if_fail (std != NULL);

  std->validate_key = st0604_validate_key;
  std->validate_data = st0604_validate_data;
}
