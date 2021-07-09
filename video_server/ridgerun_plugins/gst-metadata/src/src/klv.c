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

#include "klv.h"

#include <glib.h>
#include <string.h>

#include "lds.h"
#include "klvutils.h"

struct _Klv
{
  Key key;
  guint length;

  guchar *value;
  gboolean value_is_allocated;

  guint length_length;

  guchar *data;
  gboolean data_is_allocated;
};

guchar *
klv_raw_data (Klv * klv)
{
  g_return_val_if_fail (klv != NULL, NULL);
  return klv->data;
}

guint
klv_packet_length (Klv * klv)
{
  g_return_val_if_fail (klv != NULL, 0);

  /*Key length (16) + length field length + value length */
  return (klv->length + klv->length_length + 16);
}

guint
klv_length (Klv * klv)
{
  g_return_val_if_fail (klv != NULL, 0);
  return klv->length;
}

guchar *
klv_value (Klv * klv)
{
  g_return_val_if_fail (klv != NULL, NULL);
  return klv->value;
}

gboolean
klv_key (Klv * klv, Key * key)
{
  g_return_val_if_fail (klv != NULL, FALSE);
  g_return_val_if_fail (key != NULL, FALSE);

  *key = klv->key;

  return TRUE;
}

gboolean
klv_get (gpointer data, Klv ** klv)
{
  Key *key;
  Klv *my_klv;
  guchar *byte_data;
  guint length_size;

  g_return_val_if_fail (klv != NULL, FALSE);
  g_return_val_if_fail (data != NULL, FALSE);

  key = (Key *) data;
  *klv = NULL;

  my_klv = g_malloc (sizeof (Klv));
  byte_data = (guchar *) data;
  length_size = 0;

  my_klv->key = *key;
  my_klv->length =
      klv_utils_ber_decode (&byte_data[MISB_KEY_SIZE], &length_size);
  my_klv->value = &byte_data[MISB_KEY_SIZE + length_size];
  my_klv->value_is_allocated = FALSE;
  my_klv->length_length = length_size;
  my_klv->data = byte_data;
  my_klv->data_is_allocated = FALSE;

  *klv = my_klv;

  return TRUE;
}

gboolean
klv_from_lds (Key key, Lds * lds, Klv ** klv)
{
  Klv *my_klv;
  guchar *value, *data;
  guint length;
  guint i;
  guchar *array;

  *klv = NULL;

  g_return_val_if_fail (lds != NULL, FALSE);

  my_klv = g_malloc (sizeof (Klv));

  /*Get LDS raw data */
  length = lds_raw_data (lds, &value);

  my_klv->key = key;
  my_klv->length = length;
  my_klv->value = value;
  my_klv->value_is_allocated = TRUE;
  my_klv->length_length = klv_utils_ber_encode (my_klv->length, &length);
  array = (guchar *) & length;

  /*Copy key */
  data = g_malloc (MISB_KEY_SIZE + my_klv->length_length + my_klv->length);
  for (i = 0; i < MISB_KEY_SIZE; i++)
    data[i] = key.byte[i];

  /*Copy length */
  for (i = 0; i < my_klv->length_length; i++)
    data[MISB_KEY_SIZE + i] = array[i];

  /*copy value */
  memcpy (&data[MISB_KEY_SIZE + my_klv->length_length], value, my_klv->length);
  my_klv->data = data;
  my_klv->data_is_allocated = TRUE;

  *klv = my_klv;

  return TRUE;
}

void
klv_release (Klv * klv)
{
  if (!klv)
    return;

  if (klv->value_is_allocated)
    g_free (klv->value);

  if (klv->data_is_allocated)
    g_free (klv->data);

  klv->value = NULL;
  klv->data = NULL;

  g_free (klv);
  klv = NULL;
}
