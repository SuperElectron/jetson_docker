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

#ifndef __ST_0604_DICTIONARY_H__
#define __ST_0604_DICTIONARY_H__

#include <glib.h>

#include "misbstd.h"

G_BEGIN_DECLS 

MisbReturn st0604_validate_key (Key key);
MisbReturn st0604_validate_data (gpointer data, gpointer * new_data,
    gboolean dump);
void st0604_get (MisbStd * std);

G_END_DECLS
#endif // __ST_0604_DICTIONARY_H__
