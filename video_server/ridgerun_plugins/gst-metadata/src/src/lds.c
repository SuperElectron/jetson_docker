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

#include "lds.h"

#include <glib.h>
#include <string.h>

#include "klv.h"
#include "klvutils.h"
#include "st0601dictionary.h"


struct _Lde
{
  guint tag;
  guint length;
  guchar *value;
  gboolean value_is_allocated;
};
typedef struct _Lde Lde;

typedef enum
{
  STATE_TAG,
  STATE_LENGTH,
  STATE_VALUE
} states;

static void
_lde_set_allocated (Lde * lde, gboolean allocated)
{
  g_return_if_fail (lde != NULL);

  lde->value_is_allocated = allocated;
}

gboolean
lds_get (guchar * data, guint length, Lds ** lds)
{
  guint i;
  guint length_size;
  guint tags;
  states state;
  Lds *my_lds;
  Lde *element;

  g_return_val_if_fail (data != NULL, 0);
  g_return_val_if_fail (lds != NULL, 0);

  state = STATE_TAG;
  my_lds = *lds;

  i = 0;
  tags = 0;

  while (i < length) {

    switch (state) {
      case STATE_TAG:
        element = g_malloc(sizeof(Lde));
        element->tag = klv_utils_ber_oid_decode (&data[i], &length_size);

        if (element->tag == 0)
          return FALSE;

        i += length_size;
        tags++;

        state = STATE_LENGTH;
        break;

      case STATE_LENGTH:
        element->length = klv_utils_ber_decode (&data[i], &length_size);
        i += length_size;

        state = STATE_VALUE;
        break;

      case STATE_VALUE:
        element->value = &data[i];
        _lde_set_allocated (element, FALSE);
        i += element->length;

        my_lds = g_slist_append(my_lds, (gpointer)element);

        state = STATE_TAG;
        break;

      default:
        return FALSE;
    };
  }

  *lds = my_lds;
  return TRUE;
}

Lds *
lds_first (Lds * lds)
{
  return lds;
}

Lds *
lds_last (Lds * lds)
{
  GSList *list;

  g_return_val_if_fail (lds != NULL, NULL);

  list = g_slist_last(lds);

  if(!list)
    return NULL;

  return list;
}

Lds *
lds_next (Lds * lds)
{
  g_return_val_if_fail (lds != NULL, NULL);
  return g_slist_next(lds);
}

guint
lds_tag (Lds * lds)
{
  Lde *element;

  g_return_val_if_fail (lds != NULL, 0);

  element = (Lde*)lds->data;

  if(!element)
    return 0;

  return element->tag;
}

guint
lds_length (Lds * lds)
{
  Lde *element;

  g_return_val_if_fail (lds != NULL, 0);

  element = (Lde*)lds->data;

  if(!element)
    return 0;

  return element->length;
}

guchar *
lds_value (Lds * lds)
{
  Lde *element;

  g_return_val_if_fail (lds != NULL, NULL);

  element = (Lde*)lds->data;

  if(!element)
    return NULL;

  return element->value;
}

static void
_lde_release(gpointer data)
{
  Lde *element;
  g_return_if_fail(data != NULL);

  element = (Lde*)data;

  if (element->value_is_allocated)
    g_free (element->value);

  g_free(element);
}

void
lds_release (Lds * lds)
{
  g_return_if_fail(lds != NULL);

  g_slist_free_full(lds, _lde_release);
}

static Lde *
_lde_create (guint tag, guint length, guchar * value)
{
  Lde *element;
  
  g_return_val_if_fail(value != NULL, NULL);

  element = g_malloc (sizeof (Lde));

  element->tag = tag;
  element->length = length;
  element->value = value;
  element->value_is_allocated = FALSE;

  return element;
}

gboolean
lds_add_first (Lds ** lds, guint tag, guint length, guchar * value)
{
  Lds *my_lds;
  Lde *element;

  g_return_val_if_fail (value != NULL, FALSE);
  g_return_val_if_fail (lds != NULL, FALSE);

  element = _lde_create (tag, length, value);

  my_lds = *lds;

  my_lds = g_slist_prepend(my_lds, (gpointer)element);

  *lds = my_lds;
  return TRUE;
}

gboolean
lds_add_first_as_copy (Lds ** lds, guint tag, guint length, guchar * value)
{
  guchar *value_copy;
  Lds *my_lds;
  Lde *element;

  g_return_val_if_fail (value != NULL, FALSE);
  g_return_val_if_fail (lds != NULL, FALSE);

  value_copy = g_memdup (value, length);

  my_lds = *lds;
  element = _lde_create (tag, length, value_copy);
  _lde_set_allocated(element, TRUE);

  my_lds = g_slist_prepend(my_lds, (gpointer)element);

  *lds = my_lds;
  return TRUE;
}

gboolean
lds_add_last (Lds ** lds, guint tag, guint length, guchar * value)
{
  Lds *my_lds;
  Lde *element;

  g_return_val_if_fail (value != NULL, FALSE);
  g_return_val_if_fail (lds != NULL, FALSE);

  element = _lde_create (tag, length, value);
  my_lds = *lds;

  my_lds = g_slist_append(my_lds, (gpointer)element);

  *lds = my_lds;
  return TRUE;
}

gboolean
lds_add_last_as_copy (Lds ** lds, guint tag, guint length, guchar * value)
{
  guchar *value_copy;
  Lds *my_lds;
  Lde* element;

  g_return_val_if_fail (value != NULL, FALSE);
  g_return_val_if_fail (lds != NULL, FALSE);

  value_copy = g_memdup (value, length);
  element = _lde_create (tag, length, value_copy);
  _lde_set_allocated(element, TRUE);
  my_lds = *lds;

  my_lds = g_slist_append(my_lds, (gpointer)element);

  *lds = my_lds;
  return TRUE;
}

gboolean
lds_remove_last (Lds * lds)
{
  Lds *tmp_lds;
  Lde *element;

  g_return_val_if_fail (lds != NULL, FALSE);

  if (lds_next (lds) == NULL) {
    lds_release (lds);
    return TRUE;
  }

  tmp_lds = lds_last (lds);
  element = (Lde*)tmp_lds->data;

  _lde_release(element);
  g_slist_delete_link(lds, tmp_lds);
  
  return TRUE;
}

guint
lds_raw_data (Lds * lds, guchar ** data)
{
  Lds *tmp_lds;
  guchar *tmp_data;
  guint tmp_value;
  guchar *tmp_array;
  guint tmp_length;
  guint data_size;
  guint i;

  g_return_val_if_fail (lds != NULL, 0);
  g_return_val_if_fail (data != NULL, 0);

  tmp_lds = lds;
  tmp_data = NULL;
  data_size = 0;

  while (tmp_lds) {
    /*Save tag */
    tmp_length = klv_utils_ber_oid_encode (lds_tag (tmp_lds), &tmp_value);
    tmp_array = (guchar *) & tmp_value;
    tmp_data = g_realloc (tmp_data, data_size + tmp_length);
    for (i = 0; i < tmp_length; i++)
      tmp_data[data_size + i] = tmp_array[i];

    data_size += tmp_length;

    /*Save length */
    tmp_length = klv_utils_ber_encode (lds_length (tmp_lds), &tmp_value);
    tmp_array = (guchar *) & tmp_value;
    tmp_data = g_realloc (tmp_data, data_size + tmp_length);
    for (i = 0; i < tmp_length; i++)
      tmp_data[data_size + i] = tmp_array[i];

    data_size += tmp_length;

    /*Save value */
    tmp_length = lds_length (tmp_lds);
    tmp_array = lds_value (tmp_lds);
    tmp_data = g_realloc (tmp_data, data_size + tmp_length);
    for (i = 0; i < tmp_length; i++)
      tmp_data[data_size + i] = tmp_array[i];

    data_size += tmp_length;

    tmp_lds = lds_next (tmp_lds);
  }

  *data = tmp_data;

  return data_size;
}

gboolean
lds_add_checksum (Lds * lds, Key key)
{
  guint i;
  guchar *data, *lds_data, *checksum_ptr;
  guint encoded_length;
  guchar *length_array;
  guint lds_length, length_length;
  guint data_size;
  guint16 checksum;

  g_return_val_if_fail (lds != NULL, FALSE);

  /*Get LDS Raw data */
  lds_length = lds_raw_data (lds, &lds_data);

  /*Move key to data pointer */
  data_size = 16;
  data = g_malloc (data_size);
  for (i = 0; i < 16; i++)
    data[i] = key.byte[i];

  /*Calculate total length = LDS length + checksum tag (1 byte)
     + checksum length (1 byte) + checksum (2 bytes) */
  length_length =
      klv_utils_ber_encode (lds_length + 1 + 1 + 2, &encoded_length);
  length_array = (guchar *) & encoded_length;

  /*Move length to data pointer */
  data = g_realloc (data, data_size + length_length);
  for (i = 0; i < length_length; i++)
    data[data_size + i] = length_array[i];

  data_size += length_length;

  /*Move LDS data to pointer */
  data = g_realloc (data, data_size + lds_length);
  memcpy (&data[data_size], lds_data, lds_length);
  data_size += lds_length;

  /*Add Checksum tag */
  data = g_realloc (data, data_size + 1);
  data[data_size] = UAS_TAG_CHECKSUM;
  data_size++;

  /*Add Checksum length */
  data = g_realloc (data, data_size + 1);
  data[data_size] = 2;
  data_size++;

  /*Calculate checksum */
  checksum = klv_utils_calculate_checksum (data, data_size);

  checksum_ptr = (guchar *) g_memdup (&checksum, 2);
  lds_add_last_as_copy (&lds, UAS_TAG_CHECKSUM, 2, checksum_ptr);

  g_free (checksum_ptr);
  g_free (data);
  g_free (lds_data);

  return TRUE;

}
