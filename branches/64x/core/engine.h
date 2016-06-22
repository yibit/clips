   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.40  06/20/16            */
   /*                                                     */
   /*                 ENGINE HEADER FILE                  */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides functionality primarily associated with */
/*   the run and focus commands.                             */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
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
/*      6.24: Removed DYNAMIC_SALIENCE, INCREMENTAL_RESET,   */
/*            and LOGICAL_DEPENDENCIES compilation flags.    */
/*                                                           */
/*            Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*            Added access functions to the HaltRules flag.  */
/*                                                           */
/*            Added EnvGetNextFocus, EnvGetFocusChanged, and */
/*            EnvSetFocusChanged functions.                  */
/*                                                           */
/*      6.30: Added additional developer statistics to help  */
/*            analyze join network performance.              */
/*                                                           */
/*            Removed pseudo-facts used in not CEs.          */
/*                                                           */
/*            Added context information for run functions.   */
/*                                                           */
/*            Added before rule firing callback function.    */ 
/*                                                           */
/*            Changed garbage collection algorithm.          */
/*                                                           */
/*            Changed integer type/precision.                */
/*                                                           */
/*            Added EnvHalt function.                        */
/*                                                           */
/*            Used gensprintf instead of sprintf.            */
/*                                                           */
/*            Removed conditional code for unsupported       */
/*            compilers/operating systems (IBM_MCW,          */
/*            MAC_MCW, and IBM_TBC).                         */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Converted API macros to function calls.        */
/*                                                           */
/*      6.40: Removed LOCALE definition.                     */
/*                                                           */
/*************************************************************/

#ifndef _H_engine

#define _H_engine

#ifndef _H_lgcldpnd
#include "lgcldpnd.h"
#endif
#ifndef _H_ruledef
#include "ruledef.h"
#endif
#ifndef _H_network
#include "network.h"
#endif
#ifndef _H_moduldef
#include "moduldef.h"
#endif
#ifndef _H_retract
#include "retract.h"
#endif

struct focus
  {
   struct defmodule *theModule;
   struct defruleModule *theDefruleModule;
   struct focus *next;
  };
  
#define ENGINE_DATA 18

struct engineData
  { 
   struct defrule *ExecutingRule;
   intBool HaltRules;
   struct joinNode *TheLogicalJoin;
   struct partialMatch *TheLogicalBind;
   struct dependency *UnsupportedDataEntities;
   int alreadyEntered;
   struct callFunctionItem *ListOfRunFunctions;
   struct callFunctionItemWithArg *ListOfBeforeRunFunctions;
   struct focus *CurrentFocus;
   int FocusChanged;
#if DEBUGGING_FUNCTIONS
   unsigned WatchStatistics;
   unsigned WatchFocus;
#endif
   intBool IncrementalResetInProgress;
   intBool IncrementalResetFlag;
   intBool JoinOperationInProgress;
   struct partialMatch *GlobalLHSBinds;
   struct partialMatch *GlobalRHSBinds;
   struct joinNode *GlobalJoin;
   struct partialMatch *GarbagePartialMatches;
   struct alphaMatch *GarbageAlphaMatches;
   int AlreadyRunning;
#if DEVELOPER
   long leftToRightComparisons;
   long rightToLeftComparisons;
   long leftToRightSucceeds;
   long rightToLeftSucceeds;
   long leftToRightLoops;
   long rightToLeftLoops;
   long findNextConflictingComparisons;
   long betaHashHTSkips;
   long betaHashListSkips;
   long unneededMarkerCompare;
#endif
  };

#define EngineData(theEnv) ((struct engineData *) GetEnvironmentData(theEnv,ENGINE_DATA))

#define MAX_PATTERNS_CHECKED 64

   long long               EnvRun(void *,long long);
   intBool                 EnvAddRunFunction(void *,const char *,
                                                    void (*)(void *),int);
   intBool                 EnvAddRunFunctionWithContext(void *,const char *,
                                                               void (*)(void *),int,void *);
   intBool                 EnvRemoveRunFunction(void *,const char *);
   intBool                 EnvAddBeforeRunFunction(void *,const char *,
                                                    void (*)(void *,void *),int);
   intBool                 EnvAddBeforeRunFunctionWithContext(void *,const char *,
                                                               void (*)(void *, void *),int,void *);
   intBool                 EnvRemoveBeforeRunFunction(void *,const char *);
   void                    InitializeEngine(void *);
   void                    EnvSetBreak(void *,void *);
   void                    EnvHalt(void *);
   intBool                 EnvRemoveBreak(void *,void *);
   void                    RemoveAllBreakpoints(void *);
   void                    EnvShowBreaks(void *,const char *,void *);
   intBool                 EnvDefruleHasBreakpoint(void *,void *);
   void                    RunCommand(void *);
   void                    SetBreakCommand(void *);
   void                    RemoveBreakCommand(void *);
   void                    ShowBreaksCommand(void *);
   void                    HaltCommand(void *);
   int                     FocusCommand(void *);
   void                    ClearFocusStackCommand(void *);
   void                    EnvClearFocusStack(void *);
   void                   *EnvGetNextFocus(void *,void *);
   void                    EnvFocus(void *,void *);
   int                     EnvGetFocusChanged(void *);
   void                    EnvSetFocusChanged(void *,int);
   void                    ListFocusStackCommand(void *);
   void                    EnvListFocusStack(void *,const char *);
   void                    GetFocusStackFunction(void *,DATA_OBJECT_PTR);
   void                    EnvGetFocusStack(void *,DATA_OBJECT_PTR);
   void                   *PopFocusFunction(void *);
   void                   *GetFocusFunction(void *);
   void                   *EnvPopFocus(void *);
   void                   *EnvGetFocus(void *);
   intBool                 EnvGetHaltRules(void *);
   void                    EnvSetHaltRules(void *,intBool);
   struct activation      *NextActivationToFire(void *);

#if ALLOW_ENVIRONMENT_GLOBALS

   intBool                 AddBeforeRunFunction(const char *,void (*)(void *),int);
   intBool                 AddRunFunction(const char *,void (*)(void),int);
   void                    ClearFocusStack(void);
   void                    Focus(void *);
   void                    GetFocusStack(DATA_OBJECT_PTR);
   void                   *GetFocus(void);
   int                     GetFocusChanged(void);
   void                   *GetNextFocus(void *);
   void                    Halt(void);
   void                   *PopFocus(void);
   intBool                 RemoveRunFunction(const char *);
   long long               Run(long long);
   void                    SetFocusChanged(int);
#if DEBUGGING_FUNCTIONS
   intBool                 DefruleHasBreakpoint(void *);
   void                    ListFocusStack(const char *);
   intBool                 RemoveBreak(void *);
   void                    SetBreak(void *);
   void                    ShowBreaks(const char *,void *);
#endif

#endif /* ALLOW_ENVIRONMENT_GLOBALS */

#endif /* _H_engine */






