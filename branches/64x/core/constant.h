   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*            CLIPS Version 6.40  10/01/16             */
   /*                                                     */
   /*                CONSTANTS HEADER FILE                */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.30: Moved default type constants (NO_DEFAULT,      */
/*            STATIC_DEFAULT, and DYNAMIC_DEFAULT) to        */
/*            constant.h                                     */
/*                                                           */
/*            Added DATA_OBJECT_ARRAY primitive type.        */
/*                                                           */
/*            Added NESTED_RHS constant.                     */
/*                                                           */
/*      6.40: Pragma once and other inclusion changes.       */
/*                                                           */
/*            Added support for booleans with <stdbool.h>.   */
/*                                                           */
/*            Removed use of void pointers for specific      */
/*            data structures.                               */
/*                                                           */
/*            UDF redesign.                                  */
/*                                                           */
/*            Removed DATA_OBJECT_ARRAY primitive type.      */
/*                                                           */
/*************************************************************/

#ifndef _H_constant

#pragma once

#define _H_constant

#include <stdbool.h>

#define EXACTLY       0
#define AT_LEAST      1
#define NO_MORE_THAN  2
#define RANGE         3

#define UNBOUNDED    -1

#define LHS           0
#define RHS           1
#define NESTED_RHS    2
#define NEGATIVE      0
#define POSITIVE      1
#define EOS        '\0'

#define INSIDE        0
#define OUTSIDE       1

#define LESS_THAN     0
#define GREATER_THAN  1
#define EQUAL         2

#define GLOBAL_SAVE   0
#define LOCAL_SAVE    1
#define VISIBLE_SAVE  2

#define NO_DEFAULT      0
#define STATIC_DEFAULT  1
#define DYNAMIC_DEFAULT 2

#ifndef WPROMPT_STRING
#define WPROMPT_STRING "wclips"
#endif

#ifndef APPLICATION_NAME
#define APPLICATION_NAME "CLIPS"
#endif

#ifndef COMMAND_PROMPT
#define COMMAND_PROMPT "CLIPS> "
#endif

#ifndef VERSION_STRING
#define VERSION_STRING "6.40"
#endif

#ifndef CREATION_DATE_STRING
#define CREATION_DATE_STRING "10/01/16"
#endif

#ifndef BANNER_STRING
#define BANNER_STRING "         CLIPS (Cypher Alpha 10/01/16)\n"
#endif

/*************************/
/* TOKEN AND TYPE VALUES */
/*************************/

#define OBJECT_TYPE_NAME               "OBJECT"
#define USER_TYPE_NAME                 "USER"
#define PRIMITIVE_TYPE_NAME            "PRIMITIVE"
#define NUMBER_TYPE_NAME               "NUMBER"
#define INTEGER_TYPE_NAME              "INTEGER"
#define FLOAT_TYPE_NAME                "FLOAT"
#define SYMBOL_TYPE_NAME               "SYMBOL"
#define STRING_TYPE_NAME               "STRING"
#define MULTIFIELD_TYPE_NAME           "MULTIFIELD"
#define LEXEME_TYPE_NAME               "LEXEME"
#define ADDRESS_TYPE_NAME              "ADDRESS"
#define EXTERNAL_ADDRESS_TYPE_NAME     "EXTERNAL-ADDRESS"
#define FACT_ADDRESS_TYPE_NAME         "FACT-ADDRESS"
#define INSTANCE_TYPE_NAME             "INSTANCE"
#define INSTANCE_NAME_TYPE_NAME        "INSTANCE-NAME"
#define INSTANCE_ADDRESS_TYPE_NAME     "INSTANCE-ADDRESS"

/*************************************************************************/
/* The values of these constants should not be changed.  They are set to */
/* start after the primitive type codes in CONSTANT.H.  These codes are  */
/* used to let the generic function bsave image be used whether COOL is  */
/* present or not.                                                       */
/*************************************************************************/

#define OBJECT_TYPE_CODE                9
#define PRIMITIVE_TYPE_CODE            10
#define NUMBER_TYPE_CODE               11
#define LEXEME_TYPE_CODE               12
#define ADDRESS_TYPE_CODE              13
#define INSTANCE_TYPE_CODE             14

typedef enum
  {
   FLOAT_TYPE = (1 << 0),
   INTEGER_TYPE = (1 << 1),
   SYMBOL_TYPE = (1 << 2),
   STRING_TYPE = (1 << 3),
   MULTIFIELD_TYPE = (1 << 4),
   EXTERNAL_ADDRESS_TYPE = (1 << 5),
   FACT_ADDRESS_TYPE = (1 << 6),
   INSTANCE_ADDRESS_TYPE = (1 << 7),
   INSTANCE_NAME_TYPE = (1 << 8),
   VOID_TYPE = (1 << 9),
   BOOLEAN_TYPE = (1 << 10),
  } CLIPSType;

#define NUMBER_TYPES (INTEGER_TYPE | FLOAT_TYPE)
#define LEXEME_TYPES (SYMBOL_TYPE | STRING_TYPE)
#define ADDRESS_TYPES (EXTERNAL_ADDRESS_TYPE | FACT_ADDRESS_TYPE | INSTANCE_ADDRESS_TYPE)
#define INSTANCE_TYPES (INSTANCE_ADDRESS_TYPE | INSTANCE_NAME_TYPE)
#define SINGLEFIELD_TYPES (NUMBER_TYPES | LEXEME_TYPES | ADDRESS_TYPES | INSTANCE_NAME_TYPE)
#define ANY_TYPE (VOID_TYPE | SINGLEFIELD_TYPES | MULTIFIELD_TYPE)

/****************************************************/
/* The first 9 primitive types need to retain their */
/* values!! Sorted arrays depend on their values!!  */
/****************************************************/

#define FLOAT                           0
#define INTEGER                         1
#define SYMBOL                          2
#define STRING                          3
#define MULTIFIELD                      4
#define EXTERNAL_ADDRESS                5
#define FACT_ADDRESS                    6
#define INSTANCE_ADDRESS                7
#define INSTANCE_NAME                   8

#define RVOID                           9

#define BITMAP                         11

#define FCALL                          30
#define GCALL                          31
#define PCALL                          32
#define GBL_VARIABLE                   33
#define MF_GBL_VARIABLE                34

#define SF_VARIABLE                    35
#define MF_VARIABLE                    36
#define BITMAPARRAY                    39

#define FACT_PN_CMP1                   50
#define FACT_JN_CMP1                   51
#define FACT_JN_CMP2                   52
#define FACT_SLOT_LENGTH               53
#define FACT_PN_VAR1                   54
#define FACT_PN_VAR2                   55
#define FACT_PN_VAR3                   56
#define FACT_JN_VAR1                   57
#define FACT_JN_VAR2                   58
#define FACT_JN_VAR3                   59
#define FACT_PN_CONSTANT1              60
#define FACT_PN_CONSTANT2              61
#define FACT_STORE_MULTIFIELD          62
#define DEFTEMPLATE_PTR                63

#define OBJ_GET_SLOT_PNVAR1            70
#define OBJ_GET_SLOT_PNVAR2            71
#define OBJ_GET_SLOT_JNVAR1            72
#define OBJ_GET_SLOT_JNVAR2            73
#define OBJ_SLOT_LENGTH                74
#define OBJ_PN_CONSTANT                75
#define OBJ_PN_CMP1                    76
#define OBJ_JN_CMP1                    77
#define OBJ_PN_CMP2                    78
#define OBJ_JN_CMP2                    79
#define OBJ_PN_CMP3                    80
#define OBJ_JN_CMP3                    81
#define DEFCLASS_PTR                   82
#define HANDLER_GET                    83
#define HANDLER_PUT                    84

#define DEFGLOBAL_PTR                  90

#define PROC_PARAM                     95
#define PROC_WILD_PARAM                96
#define PROC_GET_BIND                  97
#define PROC_BIND                      98

#define UNKNOWN_VALUE                 173

#define INTEGER_OR_FLOAT              180
#define SYMBOL_OR_STRING              181
#define INSTANCE_OR_INSTANCE_NAME     182

typedef long int FACT_ID;

/*************************/
/* Macintosh Definitions */
/*************************/

#define CREATOR_STRING "CLIS"
#define CREATOR_CODE   'CLIS'

#endif





