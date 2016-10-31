   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.40  08/25/16            */
   /*                                                     */
   /*                DEFMODULE HEADER FILE                */
   /*******************************************************/

/*************************************************************/
/* Purpose: Defines basic defmodule primitive functions such */
/*   as allocating and deallocating, traversing, and finding */
/*   defmodule data structures.                              */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.23: Correction for FalseSymbol/TrueSymbol. DR0859  */
/*                                                           */
/*            Corrected compilation errors for files         */
/*            generated by constructs-to-c. DR0861           */
/*                                                           */
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*      6.30: Changed integer type/precision.                */
/*                                                           */
/*            Removed conditional code for unsupported       */
/*            compilers/operating systems (IBM_MCW,          */
/*            MAC_MCW, and IBM_TBC).                         */
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

#ifndef _H_moduldef

#pragma once

#define _H_moduldef

typedef struct defmodule Defmodule;
typedef struct portItem PortItem;
typedef struct defmoduleItemHeader DefmoduleItemHeader;
typedef struct moduleItem ModuleItem;
typedef struct constructHeader ConstructHeader;
typedef struct moduleStackItem ModuleStackItem;

typedef void *AllocateModuleFunction(Environment *);
typedef void FreeModuleFunction(Environment *,void *);

typedef enum
  {
   DEFMODULE,
   DEFRULE,
   DEFTEMPLATE,
   DEFFACTS,
   DEFGLOBAL,
   DEFFUNCTION,
   DEFGENERIC,
   DEFMETHOD,
   DEFCLASS,
   DEFMESSAGE_HANDLER,
   DEFINSTANCES
  } ConstructType;

#include <stdio.h>
#include "entities.h"
#include "userdata.h"

struct constructHeader
  {
   ConstructType constructType;
   CLIPSLexeme *name;
   const char *ppForm;
   DefmoduleItemHeader *whichModule;
   long bsaveID;
   ConstructHeader *next;
   struct userData *usrData;
   Environment *env;
  };

struct defmoduleItemHeader
  {
   Defmodule *theModule;
   ConstructHeader *firstItem;
   ConstructHeader *lastItem;
  };

typedef ConstructHeader *FindConstructFunction(Environment *,const char *);
typedef ConstructHeader *GetNextConstructFunction(Environment *,ConstructHeader *);
typedef bool *IsConstructDeletableFunction(ConstructHeader *);
typedef bool *DeleteConstructFunction(ConstructHeader *,Environment *);
typedef void *FreeConstructFunction(Environment *,ConstructHeader *);

/**********************************************************************/
/* defmodule                                                          */
/* ----------                                                         */
/* name: The name of the defmodule (stored as a reference in the      */
/*   table).                                                          */
/*                                                                    */
/* ppForm: The pretty print representation of the defmodule (used by  */
/*   the save and ppdefmodule commands).                              */
/*                                                                    */
/* itemsArray: An array of pointers to the module specific data used  */
/*   by each construct specified with the RegisterModuleItem          */
/*   function. The data pointer stored in the array is allocated by   */
/*   the allocateFunction in moduleItem data structure.               */
/*                                                                    */
/* importList: The list of items which are being imported by this     */
/*   module from other modules.                                       */
/*                                                                    */
/* next: A pointer to the next defmodule data structure.              */
/**********************************************************************/

struct defmodule
  {
   ConstructHeader header;
   DefmoduleItemHeader **itemsArray;
   PortItem *importList;
   PortItem *exportList;
   bool visitedFlag;
  };

struct portItem
  {
   CLIPSLexeme *moduleName;
   CLIPSLexeme *constructType;
   CLIPSLexeme *constructName;
   PortItem *next;
  };

#define MIHS (DefmoduleItemHeader *)

/**********************************************************************/
/* moduleItem                                                         */
/* ----------                                                         */
/* name: The name of the construct which can be placed in a module.   */
/*   For example, "defrule".                                          */
/*                                                                    */
/* allocateFunction: Used to allocate a data structure containing all */
/*   pertinent information related to a specific construct for a      */
/*   given module. For example, the deffacts construct stores a       */
/*   pointer to the first and last deffacts for each each module.     */
/*                                                                    */
/* freeFunction: Used to deallocate a data structure allocated by     */
/*   the allocateFunction. In addition, the freeFunction deletes      */
/*   all constructs of the specified type in the given module.        */
/*                                                                    */
/* bloadModuleReference: Used during a binary load to establish a     */
/*   link between the defmodule data structure and the data structure */
/*   containing all pertinent module information for a specific       */
/*   construct.                                                       */
/*                                                                    */
/* findFunction: Used to determine if a specified construct is in a   */
/*   specific module. The name is the specific construct is passed as */
/*   a string and the function returns a pointer to the specified     */
/*   construct if it exists.                                          */
/*                                                                    */
/* exportable: If true, then the specified construct type can be      */
/*   exported (and hence imported). If false, it can't be exported.   */
/*                                                                    */
/* next: A pointer to the next moduleItem data structure.             */
/**********************************************************************/

struct moduleItem
  {
   const char *name;
   int moduleIndex;
   void *(*allocateFunction)(Environment *);
   void  (*freeFunction)(Environment *,void *);
   void *(*bloadModuleReference)(Environment *,int);
   void  (*constructsToCModuleReference)(Environment *,FILE *,int,int,int);
   FindConstructFunction *findFunction;
   ModuleItem *next;
  };

struct moduleStackItem
  {
   bool changeFlag;
   Defmodule *theModule;
   ModuleStackItem *next;
  };

#define DEFMODULE_DATA 4

struct defmoduleData
  {
   struct moduleItem *LastModuleItem;
   struct callFunctionItem *AfterModuleChangeFunctions;
   ModuleStackItem *ModuleStack;
   bool CallModuleChangeFunctions;
   Defmodule *ListOfDefmodules;
   Defmodule *CurrentModule;
   Defmodule *LastDefmodule;
   int NumberOfModuleItems;
   struct moduleItem *ListOfModuleItems;
   long ModuleChangeIndex;
   bool MainModuleRedefinable;
#if (! RUN_TIME) && (! BLOAD_ONLY)
   struct portConstructItem *ListOfPortConstructItems;
   long NumberOfDefmodules;
   struct callFunctionItem *AfterModuleDefinedFunctions;
#endif
#if CONSTRUCT_COMPILER && (! RUN_TIME)
   struct CodeGeneratorItem *DefmoduleCodeItem;
#endif
#if (BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE) && (! RUN_TIME)
   long BNumberOfDefmodules;
   long NumberOfPortItems;
   struct portItem *PortItemArray;
   Defmodule *DefmoduleArray;
#endif
  };

#define DefmoduleData(theEnv) ((struct defmoduleData *) GetEnvironmentData(theEnv,DEFMODULE_DATA))

   void                           InitializeDefmodules(Environment *);
   Defmodule                     *EnvFindDefmodule(Environment *,const char *);
   const char                    *DefmoduleName(Defmodule *);
   const char                    *DefmodulePPForm(Defmodule *);
   Defmodule                     *EnvGetNextDefmodule(Environment *,Defmodule *);
   void                           RemoveAllDefmodules(Environment *);
   int                            AllocateModuleStorage(void);
   int                            RegisterModuleItem(Environment *,const char *,
                                                     AllocateModuleFunction *,
                                                     FreeModuleFunction *,
                                                     void *(*)(Environment *,int),
                                                     void (*)(Environment *,FILE *,int,int,int),
                                                     FindConstructFunction *);
   void                          *GetModuleItem(Environment *,Defmodule *,int);
   void                           SetModuleItem(Environment *,Defmodule *,int,void *);
   Defmodule                     *EnvGetCurrentModule(Environment *);
   Defmodule                     *EnvSetCurrentModule(Environment *,Defmodule *);
   void                           GetCurrentModuleCommand(Environment *,UDFContext *,UDFValue *);
   void                           SetCurrentModuleCommand(Environment *,UDFContext *,UDFValue *);
   int                            GetNumberOfModuleItems(Environment *);
   void                           CreateMainModule(Environment *);
   void                           SetListOfDefmodules(Environment *,Defmodule *);
   struct moduleItem             *GetListOfModuleItems(Environment *);
   struct moduleItem             *FindModuleItem(Environment *,const char *);
   void                           SaveCurrentModule(Environment *);
   void                           RestoreCurrentModule(Environment *);
   void                           AddAfterModuleChangeFunction(Environment *,const char *,void (*)(Environment *),int);
   void                           IllegalModuleSpecifierMessage(Environment *);
   void                           AllocateDefmoduleGlobals(Environment *);
   long                           GetNumberOfDefmodules(Environment *);

#endif /* _H_moduldef */


