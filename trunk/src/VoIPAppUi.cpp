/*
 ========================================================================
 $Id$
 $Author$
 $Date$
 $Revision$
 Copyright (c) 2010 Fandango60.  
 ========================================================================
 */
#include <eikmenub.h>
#include <akncontext.h>
#include <akntitle.h>
#include <BAUTILS.H>
#include <APGTASK.H>
#include <W32STD.H>
#include <aknmessagequerydialog.h> 
#include <VoIP.rsg>

#include "VoIPAppUi.h"
#include "VoIP.hrh"
#include "VoIPListBoxView.h"
#include "SIPExLogView.h"
#include "VoIPDocument.h"
#include "PhoneBkEngine.h"
#include "PhoneHelper.h"

#include "symbian_ua_guiSettingItemListView.h"

CVoIPAppUi::CVoIPAppUi()
    {
    }

CVoIPAppUi::~CVoIPAppUi()
    {
    if (iPhoneEngine)
        {
        delete iPhoneEngine;
        iPhoneEngine = NULL;
        }

    if (iPhoneHelper)
        {
        delete iPhoneHelper;
        iPhoneHelper = NULL;
        }

    if (iOffset)
        {
        iEikonEnv->DeleteResourceFile(iOffset);
        }
    }

void CVoIPAppUi::InitializeContainersL()
    {
    // contacts list
    iVoIPListBoxView = CVoIPListBoxView::NewL();
    AddViewL(iVoIPListBoxView);

    iSIPExLogView = CSIPExLogView::NewL();
    AddViewL(iSIPExLogView);

    iSymbian_ua_guiSettingItemListView
            = Csymbian_ua_guiSettingItemListView::NewL();
    AddViewL(iSymbian_ua_guiSettingItemListView);

    SetDefaultViewL(*iVoIPListBoxView);
    }

/**
 * Handle a command for this appui (override)
 * @param aCommand command id to be handled
 */
void CVoIPAppUi::HandleCommandL(TInt aCommand)
    {
    switch (aCommand)
        {
        case EAknSoftkeyBack:
            {
            TInt active = iTabGroup->ActiveTabIndex();
            TInt count = iTabGroup->TabCount();

            if (active > 0)
                {
                active--;

                iTabGroup->SetActiveTabByIndex(active);
                ActivateLocalViewL(TUid::Uid(
                        iTabGroup->TabIdFromIndex(active)));
                }
            else if ((active + 1) < count)
                {
                active++;

                iTabGroup->SetActiveTabByIndex(active);
                ActivateLocalViewL(TUid::Uid(
                        iTabGroup->TabIdFromIndex(active)));
                }
            break;
            }
        default:
            break;
        }

    if (aCommand == EAknSoftkeyExit || aCommand == EEikCmdExit || aCommand
            == EVoIPExitMenuItemCommand)
        {
        User::Exit(KErrNone);
        }
    }

/** 
 * Override of the HandleResourceChangeL virtual function
 */
void CVoIPAppUi::HandleResourceChangeL(TInt aType)
    {
    CAknViewAppUi::HandleResourceChangeL(aType);
    }

/** 
 * Override of the HandleKeyEventL virtual function
 * @return EKeyWasConsumed if event was handled, EKeyWasNotConsumed if not
 * @param aKeyEvent 
 * @param aType 
 */
TKeyResponse CVoIPAppUi::HandleKeyEventL(const TKeyEvent& aKeyEvent,
        TEventCode aType)
    {
    if (iTabGroup == NULL)
        {
        return EKeyWasNotConsumed;
        }

    TInt active = iTabGroup->ActiveTabIndex();
    TInt count = iTabGroup->TabCount();

    switch (aKeyEvent.iCode)
        {
        case EKeyLeftArrow:
            {
            if (active > 0)
                {
                active--;

                iTabGroup->SetActiveTabByIndex(active);
                ActivateLocalViewL(TUid::Uid(
                        iTabGroup->TabIdFromIndex(active)));
                }
            break;
            }
        case EKeyRightArrow:
            {
            if ((active + 1) < count)
                {
                active++;

                iTabGroup->SetActiveTabByIndex(active);
                ActivateLocalViewL(TUid::Uid(
                        iTabGroup->TabIdFromIndex(active)));
                }
            break;
            }
        default:
            return EKeyWasNotConsumed;
            break;
        }

    return EKeyWasConsumed;
    }

/** 
 * Override of the HandleViewDeactivation virtual function
 *
 * @param aViewIdToBeDeactivated 
 * @param aNewlyActivatedViewId 
 */
void CVoIPAppUi::HandleViewDeactivation(
        const TVwsViewId& aViewIdToBeDeactivated,
        const TVwsViewId& aNewlyActivatedViewId)
    {
    CAknViewAppUi::HandleViewDeactivation(aViewIdToBeDeactivated,
            aNewlyActivatedViewId);
    }

/**
 * @brief Completes the second phase of Symbian object construction. 
 * Put initialization code that could leave here. 
 */
void CVoIPAppUi::ConstructL()
    {

#ifdef __SERIES60_3X__
    BaseConstructL(EAknEnableSkin);
#else
    BaseConstructL(EAknEnableSkin | EAknTouchCompatible);
#endif	

    InitializeContainersL();

    // Show tabs for main views from resources
    ShowTabIcons();

    iPhoneEngine = CPhoneBkEngine::NewL();
    iPhoneEngine->LoadContactsL();
    iPhoneEngine->SortContactsL();

    //iLogEngine = CLogExEngine::NewL();

    // Construct phone helper
    iPhoneHelper = new (ELeave) CPhoneHelper();
    CleanupStack::PushL(iPhoneHelper);
    iPhoneHelper->ConstructL();
    CleanupStack::Pop(iPhoneHelper);

#ifndef __WINS__
    TFileName resfile=_L("z:\\resource\\PBKVIEW.r31");
#else
    TFileName resfile = _L("z:\\resource\\PBKVIEW.rsc");
#endif

    BaflUtils::NearestLanguageFile(CEikonEnv::Static()->FsSession(), resfile);
    iOffset = CEikonEnv::Static()->AddResourceFileL(resfile);
    }

CPhoneBkEngine* CVoIPAppUi::GetPhoneBook()
    {
    return iPhoneEngine;
    }

CPhoneHelper* CVoIPAppUi::GetPhoneHelper()
    {
    return iPhoneHelper;
    }

TBool CVoIPAppUi::NotifyBillableEvent()
    {
    RFs fileSession;
    User::LeaveIfError(fileSession.Connect());
    CleanupClosePushL(fileSession);
    _LIT(KMyTempFileName,"billableevents.cfg");
    TFileName tFile(KMyTempFileName);

    // find the drive that the application was installed on
    TFileName fileName =
            CEikonEnv::Static()->EikAppUi()->Application()->AppFullName();
    TParsePtrC parser(fileName);

    TChar ch = (parser.Drive())[0];
    TInt aDrive;
    fileSession.CharToDrive(ch, aDrive);

    User::LeaveIfError(fileSession.SetSessionToPrivate(aDrive));

    if (!BaflUtils::FileExists(fileSession, tFile))
        {
        CAknMessageQueryDialog* dlg = new (ELeave) CAknMessageQueryDialog();

        CleanupStack::PushL(dlg); //put dlg on CS

        HBufC* message = CCoeEnv::Static()->AllocReadResourceLC(
                R_BILLABLEEVENT_TEXT);

        dlg->SetMessageTextL(*message);

        CleanupStack::PopAndDestroy(message); //remove noteText from CS

        CleanupStack::Pop(dlg); //remove dlg from CS

        if (dlg->ExecuteLD(R_BILLABLEEVENT_DIALOG))
            {
            HBufC* prompt = CCoeEnv::Static()->AllocReadResourceLC(
                    R_SHOWDIALOG_QUESTION);

            CAknQueryDialog* dlgQuery = CAknQueryDialog::NewL();

            CleanupStack::PushL(dlgQuery); //put dlg on CS

            dlgQuery->SetPromptL(*prompt);

            CleanupStack::Pop(dlgQuery); //remove dlg from CS

            CleanupStack::PopAndDestroy(prompt); //remove prompt from CS

            if (dlgQuery->ExecuteLD(R_GENERAL_CONFIRMATION_QUERY))
                {
                RFile ff;
                TInt ret = ff.Create(fileSession, tFile, EFileWrite);

                if (ret != KErrNone)
                    {
                    //_LOG("Warning - Cannot create privacystatement file");
                    }
                ff.Close();
                }
            }
        else
            {
            CleanupStack::PopAndDestroy(&fileSession);
            return EFalse;
            }
        }

    CleanupStack::PopAndDestroy(&fileSession);
    return ETrue;
    }

void CVoIPAppUi::ShowTabIcons()
    {
    // Show tabs for main views from resources
    if (!sp)
        {
        sp = StatusPane();
        }

    if (!iNaviPane)
        {
        // Fetch pointer to the default navi pane control
        iNaviPane = (CAknNavigationControlContainer*) sp->ControlL(TUid::Uid(
                EEikStatusPaneUidNavi));
        }

    if (!iDecoratedTabGroup)
        {
        iDecoratedTabGroup = iNaviPane->ResourceDecorator();
        }

    if (!iTabGroup)
        {
        iTabGroup = (CAknTabGroup*) iDecoratedTabGroup->DecoratedControl();
        }

    iTabGroup->ReplaceTabTextL(EVoIPListBoxViewId, _L("Contacts"));
    iTabGroup->ReplaceTabTextL(ESIPExView2Id, _L("Log"));
    }

