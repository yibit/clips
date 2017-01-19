   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.40  08/25/16            */
   /*                                                     */
   /*                                                     */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.23: Correction for FalseSymbol/TrueSymbol. DR0859  */
/*                                                           */
/*            Corrected compilation errors for files         */
/*            generated by constructs-to-c. DR0861           */
/*                                                           */
/*      6.24: Loading a binary instance file from a run-time */
/*            program caused a bus error. DR0866             */
/*                                                           */
/*            Removed LOGICAL_DEPENDENCIES compilation flag. */
/*                                                           */
/*            Converted INSTANCE_PATTERN_MATCHING to         */
/*            DEFRULE_CONSTRUCT.                             */
/*                                                           */
/*            Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Removed conditional code for unsupported       */
/*            compilers/operating systems (IBM_MCW,          */
/*            MAC_MCW, and IBM_TBC).                         */
/*                                                           */
/*            Changed integer type/precision.                */
/*                                                           */
/*            Changed garbage collection algorithm.          */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Converted API macros to function calls.        */
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

#ifndef _H_inscom

#pragma once

#define _H_inscom

#include "insfun.h"
#include "object.h"

#define INSTANCE_DATA 29

struct instanceData
  {
   Instance DummyInstance;
   Instance **InstanceTable;
   bool MaintainGarbageInstances;
   bool MkInsMsgPass;
   bool ChangesToInstances;
   IGARBAGE *InstanceGarbageList;
   struct patternEntityRecord InstanceInfo;
   Instance *InstanceList;
   unsigned long GlobalNumberOfInstances;
   Instance *CurrentInstance;
   Instance *InstanceListBottom;
   bool ObjectModDupMsgValid;
  };

#define InstanceData(theEnv) ((struct instanceData *) GetEnvironmentData(theEnv,INSTANCE_DATA))

   void                           SetupInstances(Environment *);
   bool                           DeleteInstance(Instance *,Environment *);
   bool                           UnmakeInstance(Instance *,Environment *);
#if DEBUGGING_FUNCTIONS
   void                           InstancesCommand(Environment *,UDFContext *,UDFValue *);
   void                           PPInstanceCommand(Environment *,UDFContext *,UDFValue *);
   void                           Instances(Environment *,const char *,Defmodule *,const char *,bool);
#endif
   Instance                      *MakeInstance(Environment *,const char *);
   Instance                      *CreateRawInstance(Environment *,Defclass *,const char *);
   Instance                      *FindInstance(Environment *,Defmodule *,const char *,bool);
   bool                           ValidInstanceAddress(Instance *);
   void                           DirectGetSlot(Instance *,const char *,CLIPSValue *);
   bool                           DirectPutSlot(Instance *,const char *,CLIPSValue *);
   const char                    *InstanceName(Instance *);
   Defclass                      *InstanceClass(Instance *);
   unsigned long                  GetGlobalNumberOfInstances(Environment *);
   Instance                      *GetNextInstance(Environment *,Instance *);
   Instance                      *GetNextInstanceInScope(Environment *,Instance *);
   Instance                      *GetNextInstanceInClass(Defclass *,Instance *);
   Instance                      *GetNextInstanceInClassAndSubclasses(Defclass **,Instance *,UDFValue *);
   void                           InstancePPForm(Instance *,StringBuilder *);
   void                           ClassCommand(Environment *,UDFContext *,UDFValue *);
   void                           DeleteInstanceCommand(Environment *,UDFContext *,UDFValue *);
   void                           UnmakeInstanceCommand(Environment *,UDFContext *,UDFValue *);
   void                           SymbolToInstanceNameFunction(Environment *,UDFContext *,UDFValue *);
   void                           InstanceNameToSymbolFunction(Environment *,UDFContext *,UDFValue *);
   void                           InstanceAddressCommand(Environment *,UDFContext *,UDFValue *);
   void                           InstanceNameCommand(Environment *,UDFContext *,UDFValue *);
   void                           InstanceAddressPCommand(Environment *,UDFContext *,UDFValue *);
   void                           InstanceNamePCommand(Environment *,UDFContext *,UDFValue *);
   void                           InstancePCommand(Environment *,UDFContext *,UDFValue *);
   void                           InstanceExistPCommand(Environment *,UDFContext *,UDFValue *);
   void                           CreateInstanceHandler(Environment *,UDFContext *,UDFValue *);

#endif /* _H_inscom */





