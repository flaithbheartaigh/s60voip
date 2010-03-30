/*
 ========================================================================
 $Id$
 $Author$
 $Date$
 $Revision$
 Copyright (c) 2010 Fandango60.  
 ========================================================================
 */

#include "checkedactive.h"

CCheckedActive::~CCheckedActive()
    {
    }

CCheckedActive::CCheckedActive(TInt aPriority) :
    CActive(aPriority)
    {
    }

TInt CCheckedActive::RunError(TInt aError)
    {
    return aError;
    }

void CCheckedActive::SetActive()
    {
    CActive::SetActive();
    }

void CCheckedActive::RunL()
    {
    TRAPD(err, CheckedRunL());

    if (err != KErrNone)
        {
        User::Leave(err);
        }
    }
