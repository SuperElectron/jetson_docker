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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "misbparser.h"

#include <glib.h>
#include <stdio.h>

#include "klvutils.h"
#include "st0601dictionary.h"
#include "st0604dictionary.h"


struct _MisbParser
{
  MisbStd standards[MISB_NUM_STDS];
};
#define MISB_STD(parser, std) parser->standards[std]

MisbParser *
misb_parser_alloc ()
{
  guint i;
  MisbParser *my_parser;

  my_parser = g_malloc (sizeof (MisbParser));

  for (i = 0; i < MISB_NUM_STDS; i++) {
    MISB_STD (my_parser, i).validate_data = NULL;
    MISB_STD (my_parser, i).validate_key = NULL;
  }

  return my_parser;
}

void
misb_parser_release (MisbParser * parser)
{
  guint i;

  g_return_if_fail (parser != NULL);

  for (i = 0; i < MISB_NUM_STDS; i++) {
    MISB_STD (parser, i).validate_data = NULL;
    MISB_STD (parser, i).validate_key = NULL;
  }

  g_free (parser);
}

gboolean
misb_parser_add_std (MisbParser * parser, MisbParserStandards std)
{
  g_return_val_if_fail (parser != NULL, FALSE);
  g_return_val_if_fail (std < MISB_NUM_STDS, FALSE);

  switch (std) {
    case MISB_STD_0601:
      st0601_get (&MISB_STD (parser, std));
      return TRUE;

    case MISB_STD_0604:
      st0604_get (&MISB_STD (parser, std));
      return TRUE;

    default:
      return FALSE;
  }
}

MisbReturn
misb_parser_validate_packet (MisbParser * parser, Klv * klv, Klv ** new_klv,
    gboolean dump)
{
  guint i;
  Key key;

  g_return_val_if_fail (parser != NULL, MISB_RET_INVALID_ARGUMENT);
  g_return_val_if_fail (klv != NULL, MISB_RET_INVALID_ARGUMENT);
  g_return_val_if_fail (new_klv != NULL, MISB_RET_INVALID_ARGUMENT);

  /*Validate key */
  if (!klv_key (klv, &key))
    return MISB_RET_WRONG_KEY;

  for (i = 0; i < MISB_NUM_STDS; i++) {
    if (!MISB_STD (parser, i).validate_key)
      continue;

    if (MISB_STD (parser, i).validate_key (key) == MISB_RET_OK)
      break;
  }

  if (i == MISB_NUM_STDS)
    return MISB_RET_WRONG_KEY;  /* The provided key is not supported */

  /*Validate data */
  return MISB_STD (parser, i).validate_data ((gpointer) klv,
      (gpointer *) new_klv, dump);
}
