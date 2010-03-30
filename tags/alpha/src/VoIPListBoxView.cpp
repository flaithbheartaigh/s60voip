/*
 ========================================================================
 $Id$
 $Author$
 $Date$
 $Revision$
 Copyright (c) 2010 Fandango60.  
 ========================================================================
 */

#include <aknappui.h>
#include <aknviewappui.h>
#include <eikmenub.h>
#include <avkon.hrh>
#include <barsread.h>
#include <bautils.h>
#include <stringloader.h>
#include <aknlists.h>
#include <eikenv.h>
#include <akniconarray.h>
#include <eikclbd.h>
#include <akncontext.h>
#include <akntitle.h>
#include <eikbtgpc.h>
#include <cpbkphonenumberselect.h>
#include <cpbksmsaddressselect.h> 
#include <VoIP.rsg>

#include <CPbkContactEngine.h>
#include <RPbkViewResourceFile.h>
#include <AknQueryDialog.h>
#include <aknnotewrappers.h>

#include <UTF.H>

#include "VoIP.hrh"
#include "VoIPForm.h"
#include "VoIPListBoxView.h"
#include "VoIPListBox.h"
#include "VoIPDocument.h"
#include "VoIPAppUi.h"
#include "VoIPApplication.h"
#include "PhoneBkEngine.h"
//#include "LogExEngine.h"
#include "Contact.h"
#include "PhoneHelper.h"
//#include "SIPEngine.h"

#include "symbian_ua.h"
#include "Symbian_ua_guiSettingItemListSettings.h"

// CONSTANTS
const TInt KMaxListItemTextLength = 128;

typedef TInt32 TLogContactItemId;
_LIT(KBlank,"");

/**
 * First phase of Symbian two-phase construction. Should not contain any
 * code that could leave.
 */
CVoIPListBoxView::CVoIPListBoxView()
    {
    iVoIPListBox = NULL;
    }

/** 
 * The view's destructor removes the container from the control
 * stack and destroys it.
 */
CVoIPListBoxView::~CVoIPListBoxView()
    {
    // Deletes the Container class object.
    if (iVoIPListBox != NULL)
        {
        // Removes Container from View control stack.
        AppUi()->RemoveFromViewStack(*this, iVoIPListBox);

        delete iVoIPListBox;
        iVoIPListBox = NULL;
        }
    }

/**
 * Symbian two-phase constructor.
 * This creates an instance then calls the second-phase constructor
 * without leaving the instance on the cleanup stack.
 * @return new instance of CVoIPListBoxView
 */
CVoIPListBoxView* CVoIPListBoxView::NewL()
    {
    CVoIPListBoxView* self = CVoIPListBoxView::NewLC();
    CleanupStack::Pop(self);
    return self;
    }

/**
 * Symbian two-phase constructor.
 * This creates an instance, pushes it on the cleanup stack,
 * then calls the second-phase constructor.
 * @return new instance of CVoIPListBoxView
 */
CVoIPListBoxView* CVoIPListBoxView::NewLC()
    {
    CVoIPListBoxView* self = new (ELeave) CVoIPListBoxView();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

/**
 * Second-phase constructor for view.  
 * Initialize contents from resource.
 */
void CVoIPListBoxView::ConstructL()
    {
    BaseConstructL(R_VOIPLISTBOX_VIEW);
    }

/**
 * @return The UID for this view
 */
TUid CVoIPListBoxView::Id() const
    {
    return TUid::Uid(EVoIPListBoxViewId);
    }

/**
 * Handle a command for this view (override)
 * @param aCommand command id to be handled
 */
void CVoIPListBoxView::HandleCommandL(TInt aCommand)
    {
    switch (aCommand)
        {
        case EAknSoftkeyBack:
            {
            AppUi()->HandleCommandL(aCommand);
            break;
            }
        case EEikCmdExit:
            {
            AppUi()->HandleCommandL(aCommand);
            break;
            }
        case EVoIPExitMenuItemCommand:
            {
            AppUi()->HandleCommandL(aCommand);
            break;
            }
        case EVoIPOpenMenuItemCommand:
            {
            DoEditL();
            break;
            }
        case EVoIPEditMenuItemCommand:
            {
            DoEditL();
            break;
            }
        case EVoIPVoiceCallMenuItemCommand:
            {
            DoVoiceCall();
            //bring main window back to foreground
            RWsSession& wsSession = CEikonEnv::Static()->WsSession();
            TApaTask task(wsSession);
            task.SetWgId(CEikonEnv::Static()->RootWin().Identifier());
            task.BringToForeground();
            break;
            }
        case EVoIPInternetCallMenuItemCommand:
            {
            DoInternetCall();
            //bring main window back to foreground
            RWsSession& wsSession = CEikonEnv::Static()->WsSession();
            TApaTask task(wsSession);
            task.SetWgId(CEikonEnv::Static()->RootWin().Identifier());
            task.BringToForeground();
            break;
            }
        case EVoIPTextMessageMenuItemCommand:
            {
            DoSMS();
            break;
            }
        case EVoIPMultimediaMessageMenuItemCommand:
            {

            DoMMS();
            break;
            }
        case EVoIPNewContactMenuItemCommand:
            {
            DoNewL();
            break;
            }
        case EVoIPDeleteMenuItemCommand:
            {
            DoDeleteL();
            break;
            }
        case EVoIPHelpMenuItemCommand:
            {
            break;
            }
        case EVoIPAboutMenuItemCommand:
            {
            break;
            }
        case EVoIPSIPStartMenuItemCommand:
            {
            DoSIPStart();
            break;
            }
        case EVoIPSIPStopMenuItemCommand:
            {
            DoSIPStop();
            break;
            }
        case EVoIPSIPSettingsMenuItemCommand:
            {
            AppUi()->ActivateLocalViewL(TUid::Uid(
                    ESymbian_ua_guiSettingItemListViewId));
            break;
            }
        default:
            AppUi()->HandleCommandL(aCommand);
            break;
        }
    }

/**
 *	Handles user actions during activation of the view, 
 *	such as initializing the content.
 */
void CVoIPListBoxView::DoActivateL(const TVwsViewId& /*aPrevViewId*/,
        TUid /*aCustomMessageId*/, const TDesC8& /*aCustomMessage*/)
    {
    SetupStatusPaneL();

    if (iVoIPListBox == NULL)
        {
        iVoIPListBox = CreateContainerL();
        iVoIPListBox->SetMopParent(this);

        AppUi()->AddToStackL(*this, iVoIPListBox);
        }
    }

void CVoIPListBoxView::DoDeactivate()
    {
    CleanupStatusPane();

    if (iVoIPListBox != NULL)
        {
        AppUi()->RemoveFromViewStack(*this, iVoIPListBox);
        delete iVoIPListBox;
        iVoIPListBox = NULL;
        }
    }

/** 
 * Handle status pane size change for this view (override)
 */
void CVoIPListBoxView::HandleStatusPaneSizeChange()
    {
    CAknView::HandleStatusPaneSizeChange();

    // this may fail, but we're not able to propagate exceptions here
    TVwsViewId view;
    AppUi()->GetActiveViewId(view);
    if (view.iViewUid == Id())
        {
        TInt result;
            TRAP(result, SetupStatusPaneL());
        }
    }

void CVoIPListBoxView::SetupStatusPaneL()
    {
    // reset the context pane
    TUid contextPaneUid = TUid::Uid(EEikStatusPaneUidContext);
    CEikStatusPaneBase::TPaneCapabilities subPaneContext =
            StatusPane()->PaneCapabilities(contextPaneUid);
    if (subPaneContext.IsPresent() && subPaneContext.IsAppOwned())
        {
        CAknContextPane* context =
                static_cast<CAknContextPane*> (StatusPane()->ControlL(
                        contextPaneUid));
        context->SetPictureToDefaultL();
        }

    // setup the title pane
    TUid titlePaneUid = TUid::Uid(EEikStatusPaneUidTitle);
    CEikStatusPaneBase::TPaneCapabilities subPaneTitle =
            StatusPane()->PaneCapabilities(titlePaneUid);
    if (subPaneTitle.IsPresent() && subPaneTitle.IsAppOwned())
        {
        CAknTitlePane* title =
                static_cast<CAknTitlePane*> (StatusPane()->ControlL(
                        titlePaneUid));
        TResourceReader reader;
        iEikonEnv->CreateResourceReaderLC(reader,
                R_VOIPLISTBOX_TITLE_RESOURCE );
        title->SetFromResourceL(reader);
        CleanupStack::PopAndDestroy(); // reader internal state
        }
    }

void CVoIPListBoxView::CleanupStatusPane()
    {
    }

/**
 *	Creates the top-level container for the view.  You may modify this method's
 *	contents and the CVoIPListBox::NewL() signature as needed to initialize the
 *	container, but the signature for this method is fixed.
 *	@return new initialized instance of CVoIPListBox
 */
CVoIPListBox* CVoIPListBoxView::CreateContainerL()
    {
    return CVoIPListBox::NewL(ClientRect(), NULL, this);
    }

void CVoIPListBoxView::DoEditL()
    {
    if (iVoIPListBox->ListBox()->Model()->NumberOfItems() > 0)
        {
        TInt selectedIndex = iVoIPListBox->ListBox()->CurrentItemIndex();

        CAknFilteredTextListBoxModel* Model = STATIC_CAST (CAknFilteredTextListBoxModel*, iVoIPListBox->ListBox()->Model());
        TInt currentItem = Model->Filter()->FilteredItemIndex(selectedIndex);

        CVoIPDocument* iDocument =
                static_cast<CVoIPDocument*> (AppUi()->Document());

        iDocument->SetModifyIndex(currentItem);

        CVoIPForm* form = CVoIPForm::NewLC();
        TInt err = form->ExecuteLD(R_VOIPFORM_DIALOG);
        CleanupStack::Pop(form);
        User::LeaveIfError(err);
        }
    }

void CVoIPListBoxView::DoNewL()
    {
    CVoIPDocument* iDocument =
            static_cast<CVoIPDocument*> (AppUi()->Document());
    iDocument->SetModifyIndex(-1);
    AppUi()->ActivateLocalViewL(TUid::Uid(EVoIPSettingFormViewId));
    }

void CVoIPListBoxView::DoVoiceCall()
    {
    CVoIPAppUi* iAppUi = static_cast<CVoIPAppUi*> (AppUi());

    if (iAppUi->NotifyBillableEvent())
        {
        if (iVoIPListBox->ListBox()->Model()->NumberOfItems() > 0)
            {
            TInt selectedIndex = iVoIPListBox->ListBox()->CurrentItemIndex();
            CAknFilteredTextListBoxModel* Model = STATIC_CAST (CAknFilteredTextListBoxModel*, iVoIPListBox->ListBox()->Model());
            TInt currentItem = Model->Filter()->FilteredItemIndex(
                    selectedIndex);

            CPhoneBkEngine* iPhoneEngine = iAppUi->GetPhoneBook();
            RPointerArray<CContact> contacts = iPhoneEngine->GetContactsL();
            CContact* contact = contacts[currentItem];
            TContactItemId aContactId = contact->GetEntryId();
            CPbkContactItem* iItem = iPhoneEngine->ReadContactItemLC(
                    aContactId);

            CPbkPhoneNumberSelect* sel = new (ELeave) CPbkPhoneNumberSelect();
            CPbkPhoneNumberSelect::TParams params =
                    CPbkPhoneNumberSelect::TParams::TParams(*iItem);
            TBool accepted = sel->ExecuteLD(params/**iItem, NULL, EFalse*/);

            if (accepted)
                {
                TRAPD(err, iAppUi->GetPhoneHelper()->make_callL(params.SelectedField()->PbkFieldText()));
                }
            CleanupStack::PopAndDestroy(iItem);
            }
        }
    }

void CVoIPListBoxView::DoInternetCall()
    {
    CVoIPAppUi* iAppUi = static_cast<CVoIPAppUi*> (AppUi());

    if (iAppUi->NotifyBillableEvent())
        {
        if (iVoIPListBox->ListBox()->Model()->NumberOfItems() > 0)
            {
            TInt selectedIndex = iVoIPListBox->ListBox()->CurrentItemIndex();

            CAknFilteredTextListBoxModel* Model = STATIC_CAST (CAknFilteredTextListBoxModel*, iVoIPListBox->ListBox()->Model());
            TInt currentItem = Model->Filter()->FilteredItemIndex(
                    selectedIndex);

            CPhoneBkEngine* iPhoneEngine = iAppUi->GetPhoneBook();
            RPointerArray<CContact> contacts = iPhoneEngine->GetContactsL();
            CContact* contact = contacts[currentItem];
            TContactItemId aContactId = contact->GetEntryId();
            CPbkContactItem* iItem = iPhoneEngine->ReadContactItemLC(
                    aContactId);

            CPbkPhoneNumberSelect* sel = new (ELeave) CPbkPhoneNumberSelect();
            CPbkPhoneNumberSelect::TParams params =
                    CPbkPhoneNumberSelect::TParams::TParams(*iItem);
            TBool accepted = sel->ExecuteLD(params/**iItem, NULL, EFalse*/);

            if (accepted/*no!=KNullDesC*/)
                {
                CVoIPDocument* iDocument =
                        static_cast<CVoIPDocument*> (AppUi()->Document());
                iDocument->StorePhoneNumber(
                        params.SelectedField()->PbkFieldText());
                iDocument->StoreInfoTextL(
                        params.SelectedField()->PbkFieldText());

                AppUi()->HandleCommandL(EAknSoftkeyBack); // this temp, back command will swap views
                }
            CleanupStack::PopAndDestroy(iItem);
            }
        }
    }

void CVoIPListBoxView::DoSMS()
    {
    CVoIPAppUi* iAppUi = static_cast<CVoIPAppUi*> (AppUi());

    if (iAppUi->NotifyBillableEvent())
        {
        if (iVoIPListBox->ListBox()->Model()->NumberOfItems() > 0)
            {
            TInt selectedIndex = iVoIPListBox->ListBox()->CurrentItemIndex();
            CAknFilteredTextListBoxModel* Model = STATIC_CAST (CAknFilteredTextListBoxModel*, iVoIPListBox->ListBox()->Model());
            TInt currentItem = Model->Filter()->FilteredItemIndex(
                    selectedIndex);

            CPhoneBkEngine* iPhoneEngine = iAppUi->GetPhoneBook();
            RPointerArray<CContact> contacts = iPhoneEngine->GetContactsL();
            CContact* contact = contacts[currentItem];
            TContactItemId aContactId = contact->GetEntryId();
            CPbkContactItem* iItem = iPhoneEngine->ReadContactItemLC(
                    aContactId);

            CPbkSmsAddressSelect* sel = new (ELeave) CPbkSmsAddressSelect();
            CPbkSmsAddressSelect::TParams params =
                    CPbkSmsAddressSelect::TParams::TParams(*iItem);
            TBool accepted = sel->ExecuteLD(params/**iItem, NULL, EFalse*/);

            if (accepted)
                {
                CDesCArrayFlat* recip = new CDesCArrayFlat(1);
                CleanupStack::PushL(recip);
                CDesCArrayFlat* alias = new CDesCArrayFlat(1);
                CleanupStack::PushL(alias);
                recip->AppendL(params.SelectedField()->PbkFieldText());

                const CPbkContactItem& item = params.ContactItem();

                HBufC* title = item.GetContactTitleL();
                CleanupStack::PushL(title);
                if (title == NULL)
                    {
                    HBufC* unnamed =
                            CEikonEnv::Static()->AllocReadResourceLC(
                                    R_UNNAMED);
                    title = unnamed->Des().AllocL();
                    CleanupStack::PopAndDestroy(unnamed);
                    }

                alias->AppendL(*title);
                _LIT16(KData, "");
                TRAPD(err, iAppUi->GetPhoneHelper()->send_sms(/*title*/recip, alias, /**messagetxt*/KData));

                CleanupStack::PopAndDestroy(4); // recip, alias, title, messagetxt
                }
            CleanupStack::PopAndDestroy(iItem); //iItem
            }
        }
    }

void CVoIPListBoxView::DoMMS()
    {
    CVoIPAppUi* iAppUi = static_cast<CVoIPAppUi*> (AppUi());

    if (iAppUi->NotifyBillableEvent())
        {
        if (iVoIPListBox->ListBox()->Model()->NumberOfItems() > 0)
            {
            TInt selectedIndex = iVoIPListBox->ListBox()->CurrentItemIndex();
            CAknFilteredTextListBoxModel* Model = STATIC_CAST (CAknFilteredTextListBoxModel*, iVoIPListBox->ListBox()->Model());
            TInt currentItem = Model->Filter()->FilteredItemIndex(
                    selectedIndex);

            CPhoneBkEngine* iPhoneEngine = iAppUi->GetPhoneBook();
            RPointerArray<CContact> contacts = iPhoneEngine->GetContactsL();
            CContact* contact = contacts[currentItem];
            TContactItemId aContactId = contact->GetEntryId();
            CPbkContactItem* iItem = iPhoneEngine->ReadContactItemLC(
                    aContactId);

            CPbkSmsAddressSelect* sel = new (ELeave) CPbkSmsAddressSelect();
            CPbkSmsAddressSelect::TParams params =
                    CPbkSmsAddressSelect::TParams::TParams(*iItem);
            TBool accepted = sel->ExecuteLD(params/**iItem, NULL, EFalse*/);

            if (accepted)
                {
                CDesCArrayFlat* recip = new CDesCArrayFlat(1);
                CleanupStack::PushL(recip);
                CDesCArrayFlat* alias = new CDesCArrayFlat(1);
                CleanupStack::PushL(alias);
                recip->AppendL(params.SelectedField()->PbkFieldText());

                const CPbkContactItem& item = params.ContactItem();

                HBufC* title = item.GetContactTitleL();//iItem->GetContactTitleL();
                CleanupStack::PushL(title);
                if (title == NULL)
                    {
                    HBufC* unnamed =
                            CEikonEnv::Static()->AllocReadResourceLC(
                                    R_UNNAMED);
                    title = unnamed->Des().AllocL();
                    CleanupStack::PopAndDestroy(unnamed);
                    }

                alias->AppendL(*title);
                TRAPD(err, iAppUi->GetPhoneHelper()->send_mms(recip, alias));

                CleanupStack::PopAndDestroy(3); // recip, alias, title
                }
            CleanupStack::PopAndDestroy(iItem); //iItem
            }
        }
    }

void CVoIPListBoxView::DoDeleteL()
    {
    if (iVoIPListBox->ListBox()->Model()->NumberOfItems() > 0)
        {
        CVoIPAppUi* iAppUi = static_cast<CVoIPAppUi*> (AppUi());
        CPhoneBkEngine* iPhoneEngine = iAppUi->GetPhoneBook();
        RPointerArray<CContact> contacts = iPhoneEngine->GetContactsL();
        TInt selectedIndex = iVoIPListBox->ListBox()->CurrentItemIndex();

        CAknFilteredTextListBoxModel* Model = STATIC_CAST (CAknFilteredTextListBoxModel*, iVoIPListBox->ListBox()->Model());
        TInt currentItem = Model->Filter()->FilteredItemIndex(selectedIndex);

        CContact* contact = contacts[currentItem];
        TContactItemId aContactId = contact->GetEntryId();

        HBufC* delete_confirmation =
                CEikonEnv::Static()->AllocReadResourceLC(
                        R_DELETE_CONFIRMATION);

        HBufC* prompt = HBufC::NewLC(delete_confirmation->Length()
                + contact->GetFirstName()->Length()
                + contact->GetLastName()->Length() + 2);
        _LIT(KFormat, "%S\n%S %S");
        prompt->Des().Format(KFormat(), delete_confirmation,
                contact->GetFirstName(), contact->GetLastName());

        CAknQueryDialog* query = CAknQueryDialog::NewL();
        CleanupStack::PushL(query);
        query->SetPromptL(*prompt);
        CleanupStack::Pop(query);
        if (query->ExecuteLD(R_DELETE_CONFIRMATION_QUERY))
            {
            iAppUi->GetPhoneBook()->DeleteContactItem(aContactId);
            }
        CleanupStack::PopAndDestroy(2); //delete_confirmation, prompt
        }
    }

void CVoIPListBoxView::DoSIPStart()
    {
    if (symbian_is_connected() == 0)
        {
        // Init PJSUA
        if (symbian_ua_init() != 0)
            {
            symbian_ua_destroy();
            }
        else
            {
            TSymbian_ua_guiSettingItemListSettings* iSettings =
                    TSymbian_ua_guiSettingItemListSettings::NewL();

            CVoIPDocument
                    * doc =
                            static_cast<CVoIPDocument*> (CEikonEnv::Static()->EikAppUi()->Document());

            doc->LoadSettingValuesL(*iSettings);

            TUint8 domain[256] =
                {
                0
                };
            TPtr8 cDomain(domain, sizeof(domain));
            TUint8 user[64] =
                {
                0
                };
            TPtr8 cUser(user, sizeof(user));
            TUint8 pass[64] =
                {
                0
                };
            TPtr8 cPass(pass, sizeof(pass));

            cDomain.Copy(iSettings->Ed_registrar());
            cUser.Copy(iSettings->Ed_user());
            cPass.Copy(iSettings->Ed_password());
            symbian_ua_set_account((char*) domain, (char*) user,
                    (char*) pass, false, false);
            }
        }
    }

void CVoIPListBoxView::DoSIPStop()
    {
    if (symbian_is_connected())
        {
        symbian_ua_destroy();
        }
    }

void CVoIPListBoxView::WriteToFileL(const TDesC& aFileName,
        const TDesC8& aDescription, const TDesC8& aParty,
        const TDesC8& aDirection)
    {
    RFs fileSession;
    if (KErrNone == fileSession.Connect())
        {
        CleanupClosePushL(fileSession);

        if (KErrNone == fileSession.CreatePrivatePath(EDriveC))
            {
            if (KErrNone == fileSession.SetSessionToPrivate(EDriveC))
                {
                RFile file;

                TInt err = file.Create(fileSession, aFileName, EFileWrite
                        | EFileShareAny);
                if (err == KErrAlreadyExists)
                    {
                    err = file.Open(fileSession, aFileName, EFileWrite
                            | EFileShareAny);
                    }

                if (err == KErrNone)
                    {
                    CleanupClosePushL(file);
                    // Write to current file position: start of file
                    _LIT8(KSpace, ", ");

                    file.Write(aDescription);
                    file.Write(KSpace);
                    file.Write(aParty);
                    file.Write(KSpace);
                    file.Write(aDirection);
                    file.Flush();

                    CleanupStack::PopAndDestroy(); // close file
                    }
                }
            }

        CleanupStack::PopAndDestroy(); // close fileSession
        }
    }

void CVoIPListBoxView::DesConverter16to8(TDesC16& from, TDes8& to)
    {
    TInt i = 0;
    TInt j = 0;
    to.FillZ();
    while (j < from.Length())
        {
        to[i++] |= from[j++] & 0x00ff; // lower byte
        }
    to.SetLength(from.Length());
    }

// -----------------------------------------------------------------------------
// CVoIPListBoxView::DynInitMenuPaneL
// Handles menu initialization event.
// -----------------------------------------------------------------------------
//
void CVoIPListBoxView::DynInitMenuPaneL(TInt aResourceId,
        CEikMenuPane* aMenuPane)
    {
    if (R_VOIPLISTBOX_MENU_PANE_SIP == aResourceId)
        {
        if (symbian_is_connected())
            {
            aMenuPane->SetItemDimmed(EVoIPSIPStartMenuItemCommand, ETrue);
            }
        else
            {
            aMenuPane->SetItemDimmed(EVoIPSIPStopMenuItemCommand, ETrue);
            }
        }
    }
