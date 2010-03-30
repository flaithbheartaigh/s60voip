/*
 ========================================================================
 $Id$
 $Author$
 $Date$
 $Revision$
 Copyright (c) 2010 Fandango60.  
 ========================================================================
 */

#ifndef _PHONEHELPER_H_
#define _PHONEHELPER_H_

#include <Etel3rdParty.h>
#include <sendui.h>
#include <badesca.h>

#include "CheckedActive.h"

class CPhoneHelper : public CCheckedActive
    {
public:
    CPhoneHelper() :
        CCheckedActive(EPriorityIdle)
        {
        }
    void ConstructL();
    ~CPhoneHelper();

public:
    void send_sms(CDesCArrayFlat* recip, CDesCArrayFlat* alias,
            const TDesC& aMessage = KNullDesC);
    void send_mms(CDesCArrayFlat* recip, CDesCArrayFlat* alias);
    void make_callL(const TDesC& number);

private:
    virtual void DoCancel();
    virtual void CheckedRunL();

protected:
    CTelephony::TCallId iCallId;
    CTelephony* iTelephony;
    CSendUi* iSendAppUi;
    };

#endif
