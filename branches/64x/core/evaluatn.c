   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*            CLIPS Version 6.40  10/01/16             */
   /*                                                     */
   /*                  EVALUATION MODULE                  */
   /*******************************************************/

/*************************************************************/
/* Purpose: Provides routines for evaluating expressions.    */
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
/*      6.24: Renamed BOOLEAN macro type to intBool.         */
/*                                                           */
/*            Added EvaluateAndStoreInDataObject function.   */
/*                                                           */
/*      6.30: Added support for passing context information  */
/*            to user defined functions.                     */
/*                                                           */
/*            Added support for external address hash table  */
/*            and subtyping.                                 */
/*                                                           */
/*            Changed integer type/precision.                */
/*                                                           */
/*            Support for long long integers.                */
/*                                                           */
/*            Changed garbage collection algorithm.          */
/*                                                           */
/*            Support for DATA_OBJECT_ARRAY primitive.       */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Converted API macros to function calls.        */
/*                                                           */
/*      6.40: Added Env prefix to GetEvaluationError and     */
/*            SetEvaluationError functions.                  */
/*                                                           */
/*            Added Env prefix to GetHaltExecution and       */
/*            SetHaltExecution functions.                    */
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
/*            Callbacks must be environment aware.           */
/*                                                           */
/*            UDF redesign.                                  */
/*                                                           */
/*            Removed DATA_OBJECT_ARRAY primitive type.      */
/*                                                           */
/*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "setup.h"

#include "argacces.h"
#include "commline.h"
#include "constant.h"
#include "envrnmnt.h"
#include "memalloc.h"
#include "router.h"
#include "prcdrfun.h"
#include "multifld.h"
#include "prntutil.h"
#include "exprnpsr.h"
#include "utility.h"
#include "proflfun.h"
#include "sysdep.h"

#if DEFFUNCTION_CONSTRUCT
#include "dffnxfun.h"
#endif

#if DEFGENERIC_CONSTRUCT
#include "genrccom.h"
#endif

#if OBJECT_SYSTEM
#include "object.h"
#include "inscom.h"
#endif

#include "evaluatn.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static void                    DeallocateEvaluationData(Environment *);
   static void                    PrintCAddress(Environment *,const char *,void *);
   static void                    NewCAddress(UDFContext *,CLIPSValue *);
   /*
   static bool                    DiscardCAddress(void *,void *);
   */

/**************************************************/
/* InitializeEvaluationData: Allocates environment */
/*    data for expression evaluation.             */
/**************************************************/
void InitializeEvaluationData(
  Environment *theEnv)
  {
   struct externalAddressType cPointer = { "C", PrintCAddress, PrintCAddress, NULL, NewCAddress, NULL };

   AllocateEnvironmentData(theEnv,EVALUATION_DATA,sizeof(struct evaluationData),DeallocateEvaluationData);

   InstallExternalAddressType(theEnv,&cPointer);
  }

/*****************************************************/
/* DeallocateEvaluationData: Deallocates environment */
/*    data for evaluation data.                      */
/*****************************************************/
static void DeallocateEvaluationData(
  Environment *theEnv)
  {
   int i;

   for (i = 0; i < EvaluationData(theEnv)->numberOfAddressTypes; i++)
     { rtn_struct(theEnv,externalAddressType,EvaluationData(theEnv)->ExternalAddressTypes[i]); }
  }

/**************************************************************/
/* EvaluateExpression: Evaluates an expression. Returns false */
/*   if no errors occurred during evaluation, otherwise true. */
/**************************************************************/
bool EvaluateExpression(
  Environment *theEnv,
  struct expr *problem,
  CLIPSValue *returnValue)
  {
   struct expr *oldArgument;
   void *oldContext;
   struct FunctionDefinition *fptr;
   UDFContext theUDFContext;
#if PROFILING_FUNCTIONS
   struct profileFrameInfo profileFrame;
#endif

   returnValue->voidValue = theEnv->VoidConstant;

   if (problem == NULL)
     {
      returnValue->value = theEnv->FalseSymbol;
      return(EvaluationData(theEnv)->EvaluationError);
     }

   switch (problem->type)
     {
      case STRING:
      case SYMBOL:
      case FLOAT:
      case INTEGER:
#if OBJECT_SYSTEM
      case INSTANCE_NAME:
      case INSTANCE_ADDRESS:
#endif
      case FACT_ADDRESS:
      case EXTERNAL_ADDRESS:
        returnValue->value = problem->value;
        break;

      case FCALL:
        {
         fptr = (struct FunctionDefinition *) problem->value;
         oldContext = SetEnvironmentFunctionContext(theEnv,fptr->context);

#if PROFILING_FUNCTIONS
         StartProfile(theEnv,&profileFrame,
                      &fptr->usrData,
                      ProfileFunctionData(theEnv)->ProfileUserFunctions);
#endif

         oldArgument = EvaluationData(theEnv)->CurrentExpression;
         EvaluationData(theEnv)->CurrentExpression = problem;

         theUDFContext.environment = theEnv;
         theUDFContext.theFunction = fptr;
         theUDFContext.lastArg = problem->argList;
         theUDFContext.lastPosition = 1;
         theUDFContext.returnValue = returnValue;
         fptr->functionPointer(theEnv,&theUDFContext,returnValue);

#if PROFILING_FUNCTIONS
        EndProfile(theEnv,&profileFrame);
#endif

        SetEnvironmentFunctionContext(theEnv,oldContext);
        EvaluationData(theEnv)->CurrentExpression = oldArgument;
        break;
        }

     case MULTIFIELD:
        returnValue->value = ((CLIPSValue *) (problem->value))->value;
        returnValue->begin = ((CLIPSValue *) (problem->value))->begin;
        returnValue->end = ((CLIPSValue *) (problem->value))->end;
        break;

     case MF_VARIABLE:
     case SF_VARIABLE:
        if (GetBoundVariable(theEnv,returnValue,(CLIPSLexeme *) problem->value) == false)
          {
           PrintErrorID(theEnv,"EVALUATN",1,false);
           EnvPrintRouter(theEnv,WERROR,"Variable ");
           EnvPrintRouter(theEnv,WERROR,problem->lexemeValue->contents);
           EnvPrintRouter(theEnv,WERROR," is unbound\n");
           returnValue->value = theEnv->FalseSymbol;
           EnvSetEvaluationError(theEnv,true);
          }
        break;

      default:
        if (EvaluationData(theEnv)->PrimitivesArray[problem->type] == NULL)
          {
           SystemError(theEnv,"EVALUATN",3);
           EnvExitRouter(theEnv,EXIT_FAILURE);
          }

        if (EvaluationData(theEnv)->PrimitivesArray[problem->type]->copyToEvaluate)
          {
           returnValue->value = problem->value;
           break;
          }

        if (EvaluationData(theEnv)->PrimitivesArray[problem->type]->evaluateFunction == NULL)
          {
           SystemError(theEnv,"EVALUATN",4);
           EnvExitRouter(theEnv,EXIT_FAILURE);
          }

        oldArgument = EvaluationData(theEnv)->CurrentExpression;
        EvaluationData(theEnv)->CurrentExpression = problem;

#if PROFILING_FUNCTIONS
        StartProfile(theEnv,&profileFrame,
                     &EvaluationData(theEnv)->PrimitivesArray[problem->type]->usrData,
                     ProfileFunctionData(theEnv)->ProfileUserFunctions);
#endif

        (*EvaluationData(theEnv)->PrimitivesArray[problem->type]->evaluateFunction)(theEnv,problem->value,returnValue);

#if PROFILING_FUNCTIONS
        EndProfile(theEnv,&profileFrame);
#endif

        EvaluationData(theEnv)->CurrentExpression = oldArgument;
        break;
     }

   return(EvaluationData(theEnv)->EvaluationError);
  }

/******************************************/
/* InstallPrimitive: Installs a primitive */
/*   data type in the primitives array.   */
/******************************************/
void InstallPrimitive(
  Environment *theEnv,
  struct entityRecord *thePrimitive,
  int whichPosition)
  {
   if (EvaluationData(theEnv)->PrimitivesArray[whichPosition] != NULL)
     {
      SystemError(theEnv,"EVALUATN",5);
      EnvExitRouter(theEnv,EXIT_FAILURE);
     }

   EvaluationData(theEnv)->PrimitivesArray[whichPosition] = thePrimitive;
  }

/******************************************************/
/* InstallExternalAddressType: Installs an external   */
/*   address type in the external address type array. */
/******************************************************/
int InstallExternalAddressType(
  Environment *theEnv,
  struct externalAddressType *theAddressType)
  {
   struct externalAddressType *copyEAT;

   int rv = EvaluationData(theEnv)->numberOfAddressTypes;

   if (EvaluationData(theEnv)->numberOfAddressTypes == MAXIMUM_EXTERNAL_ADDRESS_TYPES)
     {
      SystemError(theEnv,"EVALUATN",6);
      EnvExitRouter(theEnv,EXIT_FAILURE);
     }

   copyEAT = (struct externalAddressType *) genalloc(theEnv,sizeof(struct externalAddressType));
   memcpy(copyEAT,theAddressType,sizeof(struct externalAddressType));
   EvaluationData(theEnv)->ExternalAddressTypes[EvaluationData(theEnv)->numberOfAddressTypes++] = copyEAT;

   return rv;
  }

/*********************************************************/
/* EnvSetEvaluationError: Sets the EvaluationError flag. */
/*********************************************************/
void EnvSetEvaluationError(
  Environment *theEnv,
  bool value)
  {
   EvaluationData(theEnv)->EvaluationError = value;
   if (value == true)
     { EvaluationData(theEnv)->HaltExecution = true; }
  }

/************************************************************/
/* EnvGetEvaluationError: Returns the EvaluationError flag. */
/************************************************************/
bool EnvGetEvaluationError(
  Environment *theEnv)
  {
   return(EvaluationData(theEnv)->EvaluationError);
  }

/*****************************************************/
/* EnvSetHaltExecution: Sets the HaltExecution flag. */
/*****************************************************/
void EnvSetHaltExecution(
  Environment *theEnv,
  bool value)
  {
   EvaluationData(theEnv)->HaltExecution = value;
  }

/********************************************************/
/* EnvGetHaltExecution: Returns the HaltExecution flag. */
/********************************************************/
bool EnvGetHaltExecution(
  Environment *theEnv)
  {
   return(EvaluationData(theEnv)->HaltExecution);
  }

/*****************************************************/
/* ReturnValues: Returns a linked list of CLIPSValue */
/*   structures to the pool of free memory.          */
/*****************************************************/
void ReturnValues(
  Environment *theEnv,
  CLIPSValue *garbagePtr,
  bool decrementSupplementalInfo)
  {
   CLIPSValue *nextPtr;

   while (garbagePtr != NULL)
     {
      nextPtr = garbagePtr->next;
      ValueDeinstall(theEnv,garbagePtr);
      if ((garbagePtr->supplementalInfo != NULL) && decrementSupplementalInfo)
        { DecrementSymbolCount(theEnv,(struct symbolHashNode *) garbagePtr->supplementalInfo); }
      rtn_struct(theEnv,dataObject,garbagePtr);
      garbagePtr = nextPtr;
     }
  }

/**************************************************/
/* PrintDataObject: Prints a CLIPSValue structure */
/*   to the specified logical name.               */
/**************************************************/
void PrintDataObject(
  Environment *theEnv,
  const char *fileid,
  CLIPSValue *argPtr)
  {
   switch(argPtr->header->type)
     {
      case RVOID:
      case SYMBOL:
      case STRING:
      case INTEGER:
      case FLOAT:
      case EXTERNAL_ADDRESS:
      case FACT_ADDRESS:
#if OBJECT_SYSTEM
      case INSTANCE_NAME:
      case INSTANCE_ADDRESS:
#endif
        PrintAtom(theEnv,fileid,argPtr->header->type,argPtr->value);
        break;

      case MULTIFIELD:
        PrintMultifield(theEnv,fileid,(Multifield *) argPtr->value,
                        argPtr->begin,argPtr->end,true);
        break;

      default:
        EnvPrintRouter(theEnv,fileid,"<UnknownPrintType");
        PrintLongInteger(theEnv,fileid,(long int) argPtr->header->type);
        EnvPrintRouter(theEnv,fileid,">");
        EnvSetHaltExecution(theEnv,true);
        EnvSetEvaluationError(theEnv,true);
        break;
     }
  }

/****************************************************/
/* EnvSetMultifieldErrorValue: Creates a multifield */
/*   value of length zero for error returns.        */
/****************************************************/
void EnvSetMultifieldErrorValue(
  Environment *theEnv,
  CLIPSValue *returnValue)
  {
   returnValue->value = EnvCreateMultifield(theEnv,0L);
   returnValue->begin = 1;
   returnValue->end = 0;
  }

/**************************************************/
/* ValueInstall: Increments the appropriate count */
/*   (in use) values for a CLIPSValue structure.  */
/**************************************************/
void ValueInstall(
  Environment *theEnv,
  CLIPSValue *vPtr)
  {
   if (vPtr->header->type == MULTIFIELD) CVMultifieldInstall(theEnv,(Multifield *) vPtr->value);
   else CVAtomInstall(theEnv,vPtr->value);
  }

/****************************************************/
/* ValueDeinstall: Decrements the appropriate count */
/*   (in use) values for a CLIPSValue structure.    */
/****************************************************/
void ValueDeinstall(
  Environment *theEnv,
  CLIPSValue *vPtr)
  {
   if (vPtr->header->type == MULTIFIELD) CVMultifieldDeinstall(theEnv,(Multifield *) vPtr->value);
   else CVAtomDeinstall(theEnv,vPtr->value);
  }

/*******************************************/
/* CVAtomInstall: Increments the reference */
/*   count of an atomic data type.         */
/*******************************************/
void CVAtomInstall(
  Environment *theEnv,
  void *vPtr)
  {
   switch (((TypeHeader *) vPtr)->type)
     {
      case SYMBOL:
      case STRING:
#if OBJECT_SYSTEM
      case INSTANCE_NAME:
#endif
        IncrementSymbolCount(vPtr);
        break;

      case FLOAT:
        IncrementFloatCount(vPtr);
        break;

      case INTEGER:
        IncrementIntegerCount(vPtr);
        break;

      case EXTERNAL_ADDRESS:
        IncrementExternalAddressCount(vPtr);
        break;

      case MULTIFIELD:
        MultifieldInstall(theEnv,(Multifield *) vPtr);
        break;
        
      case INSTANCE_ADDRESS:
        EnvIncrementInstanceCount(theEnv,(Instance *) vPtr);
        break;
     
      case FACT_ADDRESS:
        EnvIncrementFactCount(theEnv,(Fact *) vPtr);
        break;
     
      case RVOID:
        break;

      default:
        SystemError(theEnv,"EVALUATN",7);
        EnvExitRouter(theEnv,EXIT_FAILURE);
        break;
     }
  }

/*********************************************/
/* CVAtomDeinstall: Decrements the reference */
/*   count of an atomic data type.           */
/*********************************************/
void CVAtomDeinstall(
  Environment *theEnv,
  void *vPtr)
  {
   switch (((TypeHeader *) vPtr)->type)
     {
      case SYMBOL:
      case STRING:
#if OBJECT_SYSTEM
      case INSTANCE_NAME:
#endif
        DecrementSymbolCount(theEnv,(CLIPSLexeme *) vPtr);
        break;

      case FLOAT:
        DecrementFloatCount(theEnv,(CLIPSFloat *) vPtr);
        break;

      case INTEGER:
        DecrementIntegerCount(theEnv,(CLIPSInteger *) vPtr);
        break;

      case EXTERNAL_ADDRESS:
        DecrementExternalAddressCount(theEnv,(CLIPSExternalAddress *) vPtr);
        break;

      case MULTIFIELD:
        MultifieldDeinstall(theEnv,(Multifield *) vPtr);
        break;
        
      case INSTANCE_ADDRESS:
        EnvDecrementInstanceCount(theEnv,(Instance *) vPtr);
        break;
     
      case FACT_ADDRESS:
        EnvDecrementFactCount(theEnv,(Fact *) vPtr);
        break;

      case RVOID:
        break;

      default:
        SystemError(theEnv,"EVALUATN",8);
        EnvExitRouter(theEnv,EXIT_FAILURE);
        break;
     }
  }

/*****************************************/
/* AtomInstall: Increments the reference */
/*   count of an atomic data type.       */
/*****************************************/
void AtomInstall(
  Environment *theEnv,
  int type,
  void *vPtr)
  {
   switch (type)
     {
      case SYMBOL:
      case STRING:
#if DEFGLOBAL_CONSTRUCT
      case GBL_VARIABLE:
#endif
#if OBJECT_SYSTEM
      case INSTANCE_NAME:
#endif
        IncrementSymbolCount(vPtr);
        break;

      case FLOAT:
        IncrementFloatCount(vPtr);
        break;

      case INTEGER:
        IncrementIntegerCount(vPtr);
        break;

      case EXTERNAL_ADDRESS:
        IncrementExternalAddressCount(vPtr);
        break;

      case MULTIFIELD:
        MultifieldInstall(theEnv,(Multifield *) vPtr);
        break;

      case RVOID:
        break;

      default:
        if (EvaluationData(theEnv)->PrimitivesArray[type] == NULL) break;
        if (EvaluationData(theEnv)->PrimitivesArray[type]->bitMap) IncrementBitMapCount(vPtr);
        else if (EvaluationData(theEnv)->PrimitivesArray[type]->incrementBusyCount)
          { (*EvaluationData(theEnv)->PrimitivesArray[type]->incrementBusyCount)(theEnv,vPtr); }
        break;
     }
  }

/*******************************************/
/* AtomDeinstall: Decrements the reference */
/*   count of an atomic data type.         */
/*******************************************/
void AtomDeinstall(
  Environment *theEnv,
  int type,
  void *vPtr)
  {
   switch (type)
     {
      case SYMBOL:
      case STRING:
#if DEFGLOBAL_CONSTRUCT
      case GBL_VARIABLE:
#endif
#if OBJECT_SYSTEM
      case INSTANCE_NAME:
#endif
        DecrementSymbolCount(theEnv,(CLIPSLexeme *) vPtr);
        break;

      case FLOAT:
        DecrementFloatCount(theEnv,(CLIPSFloat *) vPtr);
        break;

      case INTEGER:
        DecrementIntegerCount(theEnv,(CLIPSInteger *) vPtr);
        break;

      case EXTERNAL_ADDRESS:
        DecrementExternalAddressCount(theEnv,(CLIPSExternalAddress *) vPtr);
        break;

      case MULTIFIELD:
        MultifieldDeinstall(theEnv,(Multifield *) vPtr);
        break;

      case RVOID:
        break;

      default:
        if (EvaluationData(theEnv)->PrimitivesArray[type] == NULL) break;
        if (EvaluationData(theEnv)->PrimitivesArray[type]->bitMap) DecrementBitMapCount(theEnv,(CLIPSBitMap *) vPtr);
        else if (EvaluationData(theEnv)->PrimitivesArray[type]->decrementBusyCount)
          { (*EvaluationData(theEnv)->PrimitivesArray[type]->decrementBusyCount)(theEnv,vPtr); }
     }
  }

#if DEFFUNCTION_CONSTRUCT || DEFGENERIC_CONSTRUCT

/********************************************/
/* EnvFunctionCall: Allows Deffunctions and */
/*   Generic Functions to be called from C. */
/*   Allows only constants as arguments.    */
/********************************************/
bool EnvFunctionCall(
  Environment *theEnv,
  const char *name,
  const char *args,
  CLIPSValue *returnValue)
  {
   FUNCTION_REFERENCE theReference;

   /*=======================================*/
   /* Call the function if it can be found. */
   /*=======================================*/

   if (GetFunctionReference(theEnv,name,&theReference))
     { return(FunctionCall2(theEnv,&theReference,args,returnValue)); }

   /*=========================================================*/
   /* Otherwise signal an error if a deffunction, defgeneric, */
   /* or user defined function doesn't exist that matches     */
   /* the specified function name.                            */
   /*=========================================================*/

   PrintErrorID(theEnv,"EVALUATN",2,false);
   EnvPrintRouter(theEnv,WERROR,"No function, generic function or deffunction of name ");
   EnvPrintRouter(theEnv,WERROR,name);
   EnvPrintRouter(theEnv,WERROR," exists for external call.\n");
   return true;
  }

/********************************************/
/* FunctionCall2: Allows Deffunctions and    */
/*   Generic Functions to be called from C. */
/*   Allows only constants as arguments.    */
/********************************************/
bool FunctionCall2(
  Environment *theEnv,
  FUNCTION_REFERENCE *theReference,
  const char *args,
  CLIPSValue *returnValue)
  {
   EXPRESSION *argexps;
   bool error = false;

   /*=============================================*/
   /* Force periodic cleanup if the function call */
   /* was executed from an embedded application.  */
   /*=============================================*/

   if ((UtilityData(theEnv)->CurrentGarbageFrame->topLevel) && (! CommandLineData(theEnv)->EvaluatingTopLevelCommand) &&
       (EvaluationData(theEnv)->CurrentExpression == NULL) && (UtilityData(theEnv)->GarbageCollectionLocks == 0))
     {
      CleanCurrentGarbageFrame(theEnv,NULL);
      CallPeriodicTasks(theEnv);
     }

   /*========================*/
   /* Reset the error state. */
   /*========================*/

   if (UtilityData(theEnv)->CurrentGarbageFrame->topLevel) EnvSetHaltExecution(theEnv,false);
   EvaluationData(theEnv)->EvaluationError = false;

   /*======================================*/
   /* Initialize the default return value. */
   /*======================================*/

   returnValue->value = theEnv->FalseSymbol;

   /*============================*/
   /* Parse the argument string. */
   /*============================*/

   argexps = ParseConstantArguments(theEnv,args,&error);
   if (error == true) return true;

   /*====================*/
   /* Call the function. */
   /*====================*/

   theReference->argList = argexps;
   error = EvaluateExpression(theEnv,theReference,returnValue);

   /*========================*/
   /* Return the expression. */
   /*========================*/

   ReturnExpression(theEnv,argexps);
   theReference->argList = NULL;

   /*==========================*/
   /* Return the error status. */
   /*==========================*/

   return(error);
  }

#endif

/***************************************************/
/* CopyDataObject: Copies the values from a source */
/*   CLIPSValue to a destination CLIPSValue.       */
/***************************************************/
void CopyDataObject(
  Environment *theEnv,
  CLIPSValue *dst,
  CLIPSValue *src,
  int garbageMultifield)
  {
   if (src->header->type != MULTIFIELD)
     {
      dst->value = src->value;
     }
   else
     {
      DuplicateMultifield(theEnv,dst,src);
      if (garbageMultifield)
        { AddToMultifieldList(theEnv,(Multifield *) dst->value); }
     }
  }

/***********************************************/
/* TransferDataObjectValues: Copies the values */
/*   directly from a source CLIPSValue to a    */
/*   destination CLIPSValue.                   */
/***********************************************/
void TransferDataObjectValues(
  CLIPSValue *dst,
  CLIPSValue *src)
  {
   dst->value = src->value;
   dst->begin = src->begin;
   dst->end = src->end;
   dst->supplementalInfo = src->supplementalInfo;
   dst->next = src->next;
  }

/************************************************************************/
/* ConvertValueToExpression: Converts the value stored in a data object */
/*   into an expression. For multifield values, a chain of expressions  */
/*   is generated and the chain is linked by the nextArg field. For a   */
/*   single field value, a single expression is created.                */
/************************************************************************/
struct expr *ConvertValueToExpression(
  Environment *theEnv,
  CLIPSValue *theValue)
  {
   long i;
   struct expr *head = NULL, *last = NULL, *newItem;

   if (theValue->header->type != MULTIFIELD)
     { return(GenConstant(theEnv,theValue->header->type,theValue->value)); }

   for (i = theValue->begin; i <= theValue->end; i++)
     {
      newItem = GenConstant(theEnv,theValue->multifieldValue->theFields[i].header->type,
                                   theValue->multifieldValue->theFields[i].value);
      if (last == NULL) head = newItem;
      else last->nextArg = newItem;
      last = newItem;
     }

   if (head == NULL)
     return(GenConstant(theEnv,FCALL,FindFunction(theEnv,"create$")));

   return(head);
  }

/****************************************/
/* GetAtomicHashValue: Returns the hash */
/*   value for an atomic data type.     */
/****************************************/
unsigned long GetAtomicHashValue(
  unsigned short type,
  void *value,
  int position)
  {
   unsigned long tvalue;
   union
     {
      double fv;
      void *vv;
      unsigned long liv;
     } fis;

   switch (type)
     {
      case FLOAT:
        fis.liv = 0;
        fis.fv = ((CLIPSFloat *) value)->contents;
        tvalue = fis.liv;
        break;

      case INTEGER:
        tvalue = (unsigned long) ((CLIPSInteger *) value)->contents;
        break;

      case EXTERNAL_ADDRESS:
         fis.liv = 0;
         fis.vv = ((CLIPSExternalAddress *) value)->contents;
         tvalue = (unsigned long) fis.liv;
         break;

      case FACT_ADDRESS:
#if OBJECT_SYSTEM
      case INSTANCE_ADDRESS:
#endif
         fis.liv = 0;
         fis.vv = value;
         tvalue = (unsigned long) fis.liv;
         break;

      case STRING:
#if OBJECT_SYSTEM
      case INSTANCE_NAME:
#endif
      case SYMBOL:
        tvalue = ((CLIPSLexeme *) value)->bucket;
        break;

      default:
        tvalue = type;
     }

   if (position < 0) return(tvalue);

   return((unsigned long) (tvalue * (((unsigned long) position) + 29)));
  }

/***********************************************************/
/* FunctionReferenceExpression: Returns an expression with */
/*   an appropriate expression reference to the specified  */
/*   name if it is the name of a deffunction, defgeneric,  */
/*   or user/system defined function.                      */
/***********************************************************/
struct expr *FunctionReferenceExpression(
  Environment *theEnv,
  const char *name)
  {
#if DEFGENERIC_CONSTRUCT
   Defgeneric *gfunc;
#endif
#if DEFFUNCTION_CONSTRUCT
   Deffunction *dptr;
#endif
   struct FunctionDefinition *fptr;

   /*=====================================================*/
   /* Check to see if the function call is a deffunction. */
   /*=====================================================*/

#if DEFFUNCTION_CONSTRUCT
   if ((dptr = LookupDeffunctionInScope(theEnv,name)) != NULL)
     { return(GenConstant(theEnv,PCALL,dptr)); }
#endif

   /*====================================================*/
   /* Check to see if the function call is a defgeneric. */
   /*====================================================*/

#if DEFGENERIC_CONSTRUCT
   if ((gfunc = LookupDefgenericInScope(theEnv,name)) != NULL)
     { return(GenConstant(theEnv,GCALL,gfunc)); }
#endif

   /*======================================*/
   /* Check to see if the function call is */
   /* a system or user defined function.   */
   /*======================================*/

   if ((fptr = FindFunction(theEnv,name)) != NULL)
     { return(GenConstant(theEnv,FCALL,fptr)); }

   /*===================================================*/
   /* The specified function name is not a deffunction, */
   /* defgeneric, or user/system defined function.      */
   /*===================================================*/

   return NULL;
  }

/******************************************************************/
/* GetFunctionReference: Fills an expression with an appropriate  */
/*   expression reference to the specified name if it is the      */
/*   name of a deffunction, defgeneric, or user/system defined    */
/*   function.                                                    */
/******************************************************************/
bool GetFunctionReference(
  Environment *theEnv,
  const char *name,
  FUNCTION_REFERENCE *theReference)
  {
#if DEFGENERIC_CONSTRUCT
   Defgeneric *gfunc;
#endif
#if DEFFUNCTION_CONSTRUCT
   Deffunction *dptr;
#endif
   struct FunctionDefinition *fptr;

   theReference->nextArg = NULL;
   theReference->argList = NULL;
   theReference->type = RVOID;
   theReference->value = NULL;

   /*=====================================================*/
   /* Check to see if the function call is a deffunction. */
   /*=====================================================*/

#if DEFFUNCTION_CONSTRUCT
   if ((dptr = LookupDeffunctionInScope(theEnv,name)) != NULL)
     {
      theReference->type = PCALL;
      theReference->value = dptr;
      return true;
     }
#endif

   /*====================================================*/
   /* Check to see if the function call is a defgeneric. */
   /*====================================================*/

#if DEFGENERIC_CONSTRUCT
   if ((gfunc = LookupDefgenericInScope(theEnv,name)) != NULL)
     {
      theReference->type = GCALL;
      theReference->value = gfunc;
      return true;
     }
#endif

   /*======================================*/
   /* Check to see if the function call is */
   /* a system or user defined function.   */
   /*======================================*/

   if ((fptr = FindFunction(theEnv,name)) != NULL)
     {
      theReference->type = FCALL;
      theReference->value = fptr;
      return true;
     }

   /*===================================================*/
   /* The specified function name is not a deffunction, */
   /* defgeneric, or user/system defined function.      */
   /*===================================================*/

   return false;
  }

/*******************************************************/
/* DOsEqual: Determines if two DATA_OBJECTS are equal. */
/*******************************************************/
bool DOsEqual(
  CLIPSValue *dobj1,
  CLIPSValue *dobj2)
  {
   if (dobj1->header->type != dobj2->header->type)
     { return false; }

   if (dobj1->header->type == MULTIFIELD)
     {
      if (MultifieldDOsEqual(dobj1,dobj2) == false)
        { return false; }
     }
   else if (dobj1->value != dobj2->value)
     { return false; }

   return true;
  }

/***********************************************************
  NAME         : EvaluateAndStoreInDataObject
  DESCRIPTION  : Evaluates slot-value expressions
                   and stores the result in a
                   Kernel data object
  INPUTS       : 1) Flag indicating if multifields are OK
                 2) The value-expression
                 3) The data object structure
                 4) Flag indicating if a multifield value
                    should be placed on the garbage list.
  RETURNS      : False on errors, true otherwise
  SIDE EFFECTS : Segment allocated for storing
                 multifield values
  NOTES        : None
 ***********************************************************/
bool EvaluateAndStoreInDataObject(
  Environment *theEnv,
  bool mfp,
  EXPRESSION *theExp,
  CLIPSValue *val,
  bool garbageSegment)
  {
   val->begin = 0;
   val->end = -1;

   if (theExp == NULL)
     {
      if (garbageSegment) val->value = EnvCreateMultifield(theEnv,0L);
      else val->value = CreateMultifield2(theEnv,0L);

      return true;
     }

   if ((mfp == false) && (theExp->nextArg == NULL))
     EvaluateExpression(theEnv,theExp,val);
   else
     StoreInMultifield(theEnv,val,theExp,garbageSegment);

   return(EvaluationData(theEnv)->EvaluationError ? false : true);
  }

/******************/
/* PrintCAddress: */
/******************/
static void PrintCAddress(
  Environment *theEnv,
  const char *logicalName,
  void *theValue)
  {
   char buffer[20];

   EnvPrintRouter(theEnv,logicalName,"<Pointer-C-");

   gensprintf(buffer,"%p",((CLIPSExternalAddress *) theValue)->contents);
   EnvPrintRouter(theEnv,logicalName,buffer);
   EnvPrintRouter(theEnv,logicalName,">");
  }

/****************/
/* NewCAddress: */
/****************/
static void NewCAddress(
  UDFContext *context,
  CLIPSValue *rv)
  {
   int numberOfArguments;
   Environment *theEnv = context->environment;

   numberOfArguments = UDFArgumentCount(context);

   if (numberOfArguments != 1)
     {
      PrintErrorID(theEnv,"NEW",1,false);
      EnvPrintRouter(theEnv,WERROR,"Function new expected no additional arguments for the C external language type.\n");
      EnvSetEvaluationError(theEnv,true);
      return;
     }

   rv->value = EnvAddExternalAddress(theEnv,NULL,0);
  }

/*******************************/
/* DiscardCAddress: TBD Remove */
/*******************************/
/*
static bool DiscardCAddress(
  Environment *theEnv,
  void *theValue)
  {
   EnvPrintRouter(theEnv,WDISPLAY,"Discarding C Address\n");

   return true;
  }
*/
