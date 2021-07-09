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

#ifndef __MISB_PARSER_H__
#define __MISB_PARSER_H__

#include <gio/gio.h>
#include <gst/gst.h>

#include "klv.h"
#include "lds.h"

G_BEGIN_DECLS

typedef struct _MisbParser MisbParser;

/**
 * @brief Supported MISB standards
 * 
 */
typedef enum{
  MISB_STD_0601,
  MISB_STD_0604,  
  MISB_NUM_STDS
}MisbParserStandards;

/**
 * @brief Allocates a \ref MisbParser struct
 * 
 * @return MisbParser* The allocated struct
 */
MisbParser *misb_parser_alloc();

/**
 * @brief Releases the memory associated with a \ref MisbParser
 * 
 * @param parser The \ref MisbParser to be released
 */
void misb_parser_release(MisbParser *parser);

/**
 * @brief Adds a new standard to the \ref MisbParser
 * 
 * @param parser The \ref MisbParser
 * @param std The standard to be added (Has to be one of \ref MisbParserStandards)
 * @return gboolean TRUE if succesfully added
 */
gboolean misb_parser_add_std(MisbParser *parser, MisbParserStandards std);

/**
 * @brief Evaluates if the input Klv packet is valid.
 * A packet is considered to be valid if the the key
 * corresponds to the UAS key, the  first element
 * is the timestam and the last element is a valid 
 * checksum. 
 * 
 * @param parser The parser instance to use.
 * @param klv The Klv to be evaluated 
 * @param lds Lds representation of the Klv value
 * @param new_klv If defined and the input klv is wrong, 
 * a fixed version (if possible) will be returned here. \n
 * Set to NULL if you just want to validate the packet.
 * @param dump Whether or not to dump the content of the packet
 * @return MisbReturn Return code (One of \ref MisbParserReturn)
 */
    MisbReturn misb_parser_validate_packet (MisbParser *parser, Klv * klv, Klv ** new_klv,
    gboolean dump);

G_END_DECLS
#endif /* __MISB_PARSER_H__ */
