/*
 ========================================================================
 $Id$
 $Author$
 $Date$
 $Revision$
 Copyright (c) 2010 Fandango60. 
 ========================================================================
 */

#ifndef CC_CHECKECACTIVE_H_INCLUDED
#define CC_CHECKECACTIVE_H_INCLUDED

#include <e32base.h>

class CCheckedActive : public CActive
    {
public:
    ~CCheckedActive();
protected:
    CCheckedActive(TInt aPriority);
    void SetActive();
    virtual void CheckedRunL() = 0;
private:
    void RunL();
    TInt RunError(TInt aError);
    };

#endif
