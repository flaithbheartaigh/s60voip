/*
 ========================================================================
 $Id$
 $Author$
 $Date$
 $Revision$
 Copyright (c) 2010 Fandango60.  
 ========================================================================
 */

#include <txtrich.h>
#include <SenduiMtmUids.h>
#include <CMessageData.h> 
#include <Etel3rdParty.h>

#include "PhoneHelper.h"

void CPhoneHelper::ConstructL()
    {
    iSendAppUi = CSendUi::NewL();
    iTelephony = CTelephony::NewL();

    CActiveScheduler::Add(this);
    }

void CPhoneHelper::CheckedRunL()
    {
    if (iStatus == KErrNone)
        {
        // The call has been dialled successfully;
        // iCallId contains the call's ID, needed when controlling the call.
        }
    }

void CPhoneHelper::DoCancel()
    {
    iTelephony->CancelAsync(CTelephony::EDialNewCallCancel);
    }

CPhoneHelper::~CPhoneHelper()
    {
    Cancel();

    delete iTelephony;
    delete iSendAppUi;
    }

void CPhoneHelper::make_callL(const TDesC& number)
    {
    CTelephony::TTelNumber telNumber(number);

    CTelephony::TCallParamsV1 callParams;
    callParams.iIdRestrict = CTelephony::ESendMyId;
    CTelephony::TCallParamsV1Pckg callParamsPckg(callParams);

    iTelephony->DialNewCall(iStatus, callParamsPckg, telNumber, iCallId);
    SetActive();
    }

void CPhoneHelper::send_sms(CDesCArrayFlat* recip, CDesCArrayFlat* alias,
        const TDesC& aMessage)
    {
    CParaFormatLayer* paraf = CParaFormatLayer::NewL();
    CleanupStack::PushL(paraf);

    CCharFormatLayer* charf = CCharFormatLayer::NewL();
    CleanupStack::PushL(charf);

    CRichText* body;
    body = CRichText::NewL(paraf, charf);
    CleanupStack::PushL(body);
    if (aMessage.Length() > 0)
        body->InsertL(0, aMessage);

    // create empty message 
    CMessageData* message = CMessageData::NewLC();
    message->AppendToAddressL((*recip)[0], (*alias)[0]);
    message->SetBodyTextL(body);
    // start message editor through SendUI 
    iSendAppUi->CreateAndSendMessageL(KSenduiMtmSmsUid, message, KNullUid,
            EFalse);
    CleanupStack::PopAndDestroy(message);

    CleanupStack::PopAndDestroy(3); // body, paraf, charf
    }

void CPhoneHelper::send_mms(CDesCArrayFlat* recip, CDesCArrayFlat* alias)
    {
    CParaFormatLayer* paraf = CParaFormatLayer::NewL();
    CleanupStack::PushL(paraf);

    CCharFormatLayer* charf = CCharFormatLayer::NewL();
    CleanupStack::PushL(charf);

    CRichText* body;
    body = CRichText::NewL(paraf, charf);
    CleanupStack::PushL(body);

    // create empty message 
    CMessageData* message = CMessageData::NewLC();
    message->AppendToAddressL((*recip)[0], (*alias)[0]);
    message->SetBodyTextL(body);
    // start message editor through SendUI 
    iSendAppUi->CreateAndSendMessageL(KSenduiMtmSmsUid, message, KNullUid,
            EFalse);
    CleanupStack::PopAndDestroy(message);

    CleanupStack::PopAndDestroy(3); // body, paraf, charf
    }
