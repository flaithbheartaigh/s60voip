/*
 ========================================================================
 $Id$
 $Author$
 $Date$
 $Revision$
 Copyright (c) 2010 Fandango60.  
 ========================================================================
 */

#include "Contact.h"
#include "VoIP.hrh"
#include <VoIP.rsg>
_LIT(KSpace," ");

CContact* CContact::NewL(TContactItemId aContactItemId, const TDesC& aFirstName,
        const TDesC& aLastName)
    {
    CContact* contact = new (ELeave) CContact;
    CleanupStack::PushL(contact);

    contact->iContactItemId = aContactItemId;

    contact->iFirstName = HBufC::NewL(aFirstName.Length());
    *(contact->iFirstName) = aFirstName;

    contact->iLastName = HBufC::NewL(aLastName.Length());
    *(contact->iLastName) = aLastName;

    CleanupStack::Pop();
    return contact;
    }

void CContact::Name(HBufC*& aFullName, TBool last_name_first) const
    {
    const CContact *contact = this;
    TInt totalStrLength = contact->iFirstName->Length() + contact->iLastName->Length()
            + 50;
    
    if (totalStrLength > aFullName->Des().MaxLength())
        {
        delete aFullName;
        aFullName = HBufC::NewL(totalStrLength);
        }

    if (last_name_first)
        {
        *(aFullName) = *(contact->iLastName);
        if (contact->iLastName->Length() > 0 && contact->iFirstName->Length()
                > 0)
            {
            aFullName->Des().Append(KSpace);
            }
        aFullName->Des().Append(*(contact->iFirstName));
        }
    else
        {
        *(aFullName) = *(contact->iFirstName);
        if (contact->iFirstName->Length() > 0 && contact->iLastName->Length()
                > 0)
            {
            aFullName->Des().Append(KSpace);
            }
        aFullName->Des().Append(*(contact->iLastName));
        }
    
    if (aFullName->Des().Length() == 0)
        {
        HBufC* KUnnamedStr = CEikonEnv::Static()->AllocReadResourceLC(
                R_UNNAMED);
        aFullName->Des().Append(*KUnnamedStr);
        CleanupStack::PopAndDestroy(KUnnamedStr);
        }
    }

TContactItemId CContact::GetEntryId()
    {
    return iContactItemId;
    }

void CContact::SetEntryId(TContactItemId aContactItemId)
    {
    iContactItemId = aContactItemId;
    }

CContact::~CContact()
    {
    if (iFirstName)
        {
        delete iFirstName;
        iFirstName = NULL;
        }

    if (iLastName)
        {
        delete iLastName;
        iLastName = NULL;
        }
    }

void CContact::SetFirstName(const TDesC& aFirstName)
    {
    if (aFirstName.Compare(*iFirstName) != 0)
        {
        TInt newLength = aFirstName.Length();

        if (newLength > iFirstName->Des().MaxLength())
            {
            iFirstName = iFirstName->ReAllocL(newLength);
            }
        iFirstName->Des().Copy(aFirstName);
        }
    }

const HBufC* CContact::GetFirstName()
    {
    return iFirstName;
    }

void CContact::SetLastName(const TDesC& aLastName)
    {
    if (aLastName.Compare(*iLastName) != 0)
        {
        TInt newLength = aLastName.Length();

        if (newLength > iLastName->Des().MaxLength())
            {
            iLastName = iLastName->ReAllocL(newLength);
            }
        iLastName->Des().Copy(aLastName);
        }
    }

const HBufC* CContact::GetLastName()
    {
    return iLastName;
    }
