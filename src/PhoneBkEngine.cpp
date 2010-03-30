/*
 ========================================================================
 $Id$
 $Author$
 $Date$
 $Revision$
 Copyright (c) 2010 Fandango60.  
 ========================================================================
 */

#include "PhoneBkEngine.h"
#include "Contact.h"
#include "VoIP.hrh"
#include <VoIP.rsg>
#include <CPbkContactEngine.h>		// CPbkContactEngine 
#include <cpbkfieldinfo.h> 
#include <cpbkfieldsinfo.h> 
#include <f32file.h>
#include <eikenv.h>
#include <eikmenup.h>
#include <aknglobalnote.h>
#include <AknCommonDialogs.h> 
#include <aknnotewrappers.h>	//CAknInformationNote
#include <CPbkMultipleEntryFetchDlg.h>
#include <aknselectionlist.h>
#include <stringloader.h> 
#include <cpbkcontactitem.h> 
#include <bcardeng.h>
#include <cpbkcontactchangenotifier.h> 
#include <rpbkviewresourcefile.h>
#include <sendui.h>  
#include <cpbkphonenumberselect.h>  
#include <cpbkidlefinder.h>
#include <aknviewappui.h>
#include <avkon.rsg>
#include <avkon.hrh>
#include <avkon.mbg>
#include <txtrich.h>                // for CRichText
#include <eikenv.h>                 // for CEikonEnv
#include <smut.h>                   // for message type UIDs
#include <msvuids.h>                // for Message type UIDs
#include <miutset.h>                // for KUidMsgTypeSMTP
#include <AknQueryDialog.h>         // for CAknTextQueryDialog
#include <mtmuids.h>                // for KUidMtmQueryCanSendMsg etc. capability query UIDs
#include <txtfmlyr.h>               // for CParaFormatLayer, CCharFormatLayer
#include <sendui.h>                 // for CSendAppUi
#include <mmsconst.h>               // for KUidMsgTypeMultimedia
#include <e32debug.h>
#include <e32base.h>
#include <e32cons.h>
_LIT(KSpace,"");

class contacts_key : public TKey
    {
public:
    contacts_key(CArrayFixFlat<CContact*> & i_arr, HBufC*& aname1,
            HBufC*& aname2, TBool alast_name_first = false) :
        arr(i_arr), name1(aname1), name2(aname2), last_name_first(
                alast_name_first)
        {
        }
    TInt Compare(TInt aLeft, TInt aRight) const
        {
        CContact* left = arr[aLeft];
        CContact* right = arr[aRight];

        left->Name(name1, last_name_first);
        right->Name(name2, last_name_first);

        //TCollationMethod m = *Mem::CollationMethodByIndex(0); // get the standard method
        //m.iFlags |= TCollationMethod::EIgnoreNone; // dont ignore punctuation and spaces

        return name1->CompareC(*name2, 3 /* string level */, NULL);
        }
private:
    CArrayFixFlat<CContact*>& arr;
    HBufC*& name1;
    HBufC*& name2;
    TBool last_name_first;
    };

class contacts_swap : public TSwap
    {
public:
    contacts_swap(CArrayFixFlat<CContact*> & i_arr) :
        arr(i_arr)
        {
        }
    void Swap(TInt aLeft, TInt aRight) const
        {
        CContact* tmp = arr[aLeft];
        arr[aLeft] = arr[aRight];
        arr[aRight] = tmp;
        }
private:
    CArrayFixFlat<CContact*>& arr;
    };

CPhoneBkEngine::CPhoneBkEngine()
    {
    }

CPhoneBkEngine* CPhoneBkEngine::NewL()
    {
    CPhoneBkEngine* self = CPhoneBkEngine::NewLC();
    CleanupStack::Pop(self);
    return self;
    }

CPhoneBkEngine* CPhoneBkEngine::NewLC()
    {
    CPhoneBkEngine* self = new (ELeave) CPhoneBkEngine();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CPhoneBkEngine::~CPhoneBkEngine()
    {
    if (iNotifier)
        {
        delete iNotifier;
        iNotifier = NULL;
        }

    if (iPbkContactEngine)
        {
        delete iPbkContactEngine;
        iPbkContactEngine = NULL;
        }

    contacts.ResetAndDestroy();

    }

void CPhoneBkEngine::ConstructL()
    {
    //Create a new Phonebook engine object and connects to the default contact database
    //Should trap this :- KErrCorrupt 
    iPbkContactEngine = CPbkContactEngine::NewL();

    // register ourselves as an observer
    iNotifier = iPbkContactEngine->CreateContactChangeNotifierL(this);
    }

TContactItemId CPhoneBkEngine::CreateDefaultContactItem()
    {
    CPbkContactItem* item = iPbkContactEngine->CreateEmptyContactL();
    CleanupStack::PushL(item);
    TInt newContactID = iPbkContactEngine->AddNewContactL(*item, EFalse);
    CleanupStack::PopAndDestroy(item);
    return newContactID;
    }

void CPhoneBkEngine::CommitContactItem(CPbkContactItem* item)
    {
    TRAPD(error2, iPbkContactEngine->CommitContactL( *item , EFalse));
    _LIT(KMsgDebug,"Error value: %d");
    RDebug::Print(KMsgDebug, error2);
    }

void CPhoneBkEngine::LoadContactsL()
    {
    iter = iPbkContactEngine->CreateContactIteratorLC(ETrue);

    contacts.ResetAndDestroy();

    iReady = EFalse;

    // CAknWaitNoteWrapper is privately derived from CActive/CBase
    // therefore a reinterpret_cast must be performed here
    CAknWaitNoteWrapper* iWaitNote = CAknWaitNoteWrapper::NewL();
    CleanupStack::PushL(reinterpret_cast<CBase*> (iWaitNote));
    iWaitNote->ExecuteL(R_CONNECTING_NOTE, *this, EFalse);
    CleanupStack::PopAndDestroy(iWaitNote);

    CleanupStack::PopAndDestroy(iter);

    //TODO: do we need to do this
    //contacts->Compress();
    }

void CPhoneBkEngine::SortContactsL()
    {
    if (contacts.Count() > 0)
        {
        TLinearOrder<CContact>* order = new TLinearOrder<CContact> (
                compareName);
        contacts.Sort(*order);
        delete (order);
        }
    }

TInt CPhoneBkEngine::compareName(const CContact& aFirst,
        const CContact& aSecond)
    {
    HBufC* name1 = HBufC::NewL(128);
    CleanupStack::PushL(name1);
    HBufC* name2 = HBufC::NewL(128);
    CleanupStack::PushL(name2);

    aFirst.Name(name1, false);
    aSecond.Name(name2, false);

    TInt comparison = name1->CompareC(*name2, 3 /* string level */, NULL);
    CleanupStack::PopAndDestroy(2);

    return comparison;
    }

RPointerArray<CContact> CPhoneBkEngine::GetContactsL()
    {
    return contacts;
    }

CPbkContactItem* CPhoneBkEngine::ReadContactItemLC(TContactItemId iContactId)
    {
    return iPbkContactEngine->ReadMinimalContactLC(iContactId);
    }

CPbkContactItem* CPhoneBkEngine::OpenContactItemLCX(TContactItemId iContactId)
    {
    return iPbkContactEngine->OpenContactLCX(iContactId);
    }

void CPhoneBkEngine::HandleDatabaseEventL(TContactDbObserverEvent aEvent)
    {
//    if (aEvent.iType == EContactDbObserverEventContactAdded)
//        {
//        // debug message
//        HBufC* noteText = StringLoader::LoadLC(R_CONTACTADDED_TEXT);
//        (CEikonEnv::Static())->InfoMsg(*noteText);
//        CleanupStack::PopAndDestroy();
//        }
    }

// ----------------------------------------------------------------------------
// CPhoneBkEngine::DialogDismissedL(TInt aButtonId)
// It is overridden function from interface MAknBackgroundProcess
// and is called by the active scheduler to dismiss the dialog
// ----------------------------------------------------------------------------
//
void CPhoneBkEngine::DialogDismissedL(TInt aButtonId)
    {
    if (aButtonId == EAknSoftkeyCancel)
        {
        // Do something to stop.
        iReady = ETrue;
        }
    }

// ----------------------------------------------------------------------------
// CPhoneBkEngine::IsProcessDone() const
// It is overridden function from interface MAknBackgroundProcess
// and is called by the active scheduler to check if is process already done 
// ----------------------------------------------------------------------------
//
TBool CPhoneBkEngine::IsProcessDone() const
    {
    return iReady;
    }

// ----------------------------------------------------------------------------
// CPhoneBkEngine::ProcessFinished() 
// It is overridden function from interface MAknBackgroundProcess
// and is called by the active scheduler to finish the process 
// ----------------------------------------------------------------------------
//
void CPhoneBkEngine::ProcessFinished()
    {
    }

// ----------------------------------------------------------------------------
// CPhoneBkEngine::StepL() 
// It is overridden function from interface MAknBackgroundProcess
// and is called by the active scheduler to do one step of processing
// ----------------------------------------------------------------------------
//
void CPhoneBkEngine::StepL()
    {
    CPbkContactItem *item = 0;
    TBuf<KMaxNameLength> first_name;
    TBuf<KMaxNameLength> last_name;
    TDateTime birth_date;

    for (iter->FirstL(); (item = iter->CurrentL()); iter->NextL())
        {
        TPbkContactItemField* field;

        field = item->FindField(EPbkFieldIdLastName);
        if (field)
            {
            last_name = field->Text();
            }
        else
            {
            last_name = KSpace;
            }

        field = item->FindField(EPbkFieldIdFirstName);
        if (field)
            {
            first_name = field->Text();
            }
        else
            {
            first_name = KSpace;
            }

        CContact* contact = CContact::NewL(item->Id(), first_name, last_name);
        contacts.AppendL(contact);
        }
    iReady = ETrue;
    }

const CPbkFieldsInfo& CPhoneBkEngine::GetFieldsInfo()
    {
    return iPbkContactEngine->FieldsInfo();
    }

void CPhoneBkEngine::DeleteContactItem(TContactItemId iContactId)
    {
    iPbkContactEngine->DeleteContactL(iContactId);
    }
