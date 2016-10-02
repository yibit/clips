   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.40  08/25/16            */
   /*                                                     */
   /*         DEFRULE BASIC COMMANDS HEADER FILE          */
   /*******************************************************/

/*************************************************************/
/* Purpose: Implements core commands for the defrule         */
/*   construct such as clear, reset, save, undefrule,        */
/*   ppdefrule, list-defrules, and                           */
/*   get-defrule-list.                                       */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.23: Corrected compilation errors for files         */
/*            generated by constructs-to-c. DR0861           */
/*                                                           */
/*            Changed name of variable log to logName        */
/*            because of Unix compiler warnings of shadowed  */
/*            definitions.                                   */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Removed conditional code for unsupported       */
/*            compilers/operating systems (IBM_MCW,          */
/*            MAC_MCW, and IBM_TBC).                         */
/*                                                           */
/*            Support for join network changes.              */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Converted API macros to function calls.        */
/*                                                           */
/*            Added code to prevent a clear command from     */
/*            being executed during fact assertions via      */
/*            JoinOperationInProgress mechanism.             */
/*                                                           */
/*      6.40: Removed LOCALE definition.                     */
/*                                                           */
/*            Pragma once and other inclusion changes.       */
/*                                                           */
/*            Added support for booleans with <stdbool.h>.   */
/*                                                           */
/*            Removed use of void pointers for specific      */
/*            data structures.                               */
/*                                                           */
/*            ALLOW_ENVIRONMENT_GLOBALS no longer supported. */
/*                                                           */
/*            UDF redesign.                                  */
/*                                                           */
/*************************************************************/

#ifndef _H_rulebsc

#pragma once

#define _H_rulebsc

#include "evaluatn.h"

   void                           DefruleBasicCommands(Environment *);
   void                           UndefruleCommand(Environment *,UDFContext *,CLIPSValue *);
   bool                           EnvUndefrule(Environment *,Defrule *);
   void                           GetDefruleListFunction(Environment *,UDFContext *,CLIPSValue *);
   void                           EnvGetDefruleList(Environment *,CLIPSValue *,Defmodule *);
   void                           DefruleModuleFunction(Environment *,UDFContext *,CLIPSValue *);
#if DEBUGGING_FUNCTIONS
   void                           PPDefruleCommand(Environment *,UDFContext *,CLIPSValue *);
   bool                           PPDefrule(Environment *,const char *,const char *);
   void                           ListDefrulesCommand(Environment *,UDFContext *,CLIPSValue *);
   void                           EnvListDefrules(Environment *,const char *,Defmodule *);
   bool                           EnvGetDefruleWatchFirings(Environment *,Defrule *);
   bool                           EnvGetDefruleWatchActivations(Environment *,Defrule *);
   void                           EnvSetDefruleWatchFirings(Environment *,bool,Defrule *);
   void                           EnvSetDefruleWatchActivations(Environment *,bool,Defrule *);
   bool                           DefruleWatchAccess(Environment *,int,bool,struct expr *);
   bool                           DefruleWatchPrint(Environment *,const char *,int,struct expr *);
#endif

#endif /* _H_rulebsc */

