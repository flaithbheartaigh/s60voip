/*
* ==============================================================================
*  Name        : SIPExLogView.cpp
*  Part of     : SIPEx
*  Interface   : 
*  Description : 
*  Version     : 
*
*  Copyright (c) 2004-2006 Nokia Corporation.
*  This material, including documentation and any related 
*  computer programs, is protected by copyright controlled by 
*  Nokia Corporation.
* ==============================================================================
*/

// INCLUDE FILES
#include <aknviewappui.h>
#include <eikmenup.h> // CEikMenupane
#include <avkon.hrh>
#include <AknNoteWrappers.h>
#include <StringLoader.h>
#include <AknQueryDialog.h>
#include <UTF.H>

//#include <SIPEx.rsg>
#include <VoIP.rsg> // modification: $Author$
#include <VoIPDocument.h> // modification: $Author$

#include "SIPExLogView.h"
#include "SIPExLogContainer.h" 

#include "symbian_ua.h" // modification: $Author$
#include "Symbian_ua_guiSettingItemListSettings.h" // modification: $Author$

/* modification: $Author$ */
CSIPExLogView *myinstance = NULL;
_LIT(KStCall, "Call");
_LIT(KStHangUp, "Hang Up");
_LIT(KStBack, "Exit");
_LIT(KBlank,"");

/* modification: $Author$ */
void on_info(const wchar_t* buf)
{
	TPtrC aBuf((const TUint16*) buf);

	if (myinstance)
		myinstance->PutMessage(aBuf);
}

/* modification: $Author$ */
void on_incoming_call(const wchar_t* caller_disp, const wchar_t* caller_uri)
{
	TBuf<512> buf;
	TPtrC aDisp((const TUint16*) caller_disp);
	TPtrC aUri((const TUint16*) caller_uri);
	_LIT(KFormat, "Incoming call from %S, accept?");

	buf.Format(KFormat, &aDisp);
	if (CSIPExLogView::RunQry_accept_callL(&buf) == EAknSoftkeyYes)
	{
		CEikButtonGroupContainer* cba = CEikButtonGroupContainer::Current();
		if (cba != NULL)
		{
			TRAPD(result, cba->SetCommandL(
					ESIPExLogViewControlPaneRightId, KStHangUp));
			cba->DrawDeferred();
		}
		symbian_ua_answercall();
	}
	else
	{
		symbian_ua_endcall();
	}
}

/* modification: $Author$ */
void on_call_end(const wchar_t* reason)
{	
	TPtrC aReason((const TUint16*) reason);

	CEikButtonGroupContainer* cba = CEikButtonGroupContainer::Current();
	if (cba != NULL)
	{
		TRAPD(result, cba->SetCommandL(
				ESIPExLogViewControlPaneRightId, KStBack));
		cba->DrawDeferred();
	}

	CSIPExLogView::RunNote_infoL(&aReason);
}

/* modification: $Author$ */
void on_reg_state(bool success)
{
	if (success)
		CSIPExLogView::RunNote_infoL();
	else
		CSIPExLogView::RunNote_warningL();
}

/* modification: $Author$ */
void on_unreg_state(bool success)
{
	TPtrC st_success(_L("Unregistration Success!"));
	TPtrC st_failed(_L("Unregistration Failed!"));

	if (success)
		CSIPExLogView::RunNote_infoL(&st_success);
	else
		CSIPExLogView::RunNote_warningL(&st_failed);
}

/* modification: $Author$ */
void CSIPExLogView::PutMessage(const TDesC &msg)
{
	//	if (!iSymbian_ua_guiContainer)
	//		return;
	//
	//	CEikEdwin *obj_info =
	//			(CEikEdwin*) iSymbian_ua_guiContainer->ComponentControl(
	//					iSymbian_ua_guiContainer->EEd_info);
	//
	//	obj_info->SetTextL(&msg);
	//	obj_info->DrawDeferred();

	CVoIPDocument* iDocument = static_cast<CVoIPDocument*> (AppUi()->Document());
	iDocument->StoreInfoTextL(msg);
	if(iContainer)
	{
		iContainer->ShowInfoL();
	}
	
}

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSIPExLogView::NewL
// EPOC two-phase construction
// -----------------------------------------------------------------------------
//
/* modification: $Author$ */
CSIPExLogView* CSIPExLogView::NewL()
{
	CSIPExLogView* self = CSIPExLogView::NewLC();
	CleanupStack::Pop(self);
	return self;
}

// -----------------------------------------------------------------------------
// CSIPExLogView::NewLC
// EPOC two-phase construction
// -----------------------------------------------------------------------------
//
CSIPExLogView* CSIPExLogView::NewLC( /*CSIPExEngine& aEngine*/ )
    {
    CSIPExLogView* self = new ( ELeave ) CSIPExLogView( /*aEngine*/ );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
    
// -----------------------------------------------------------------------------
// CSIPExLogView::CSIPExLogView
// C++ default constructor
// -----------------------------------------------------------------------------
//
CSIPExLogView::CSIPExLogView( /*CSIPExEngine& aEngine*/ )
    /*: iEngine( aEngine )*/
    {
    // no implementation required
    }
    
// -----------------------------------------------------------------------------
// CSIPExLogView::ConstructL
// EPOC second-phased constructor
// -----------------------------------------------------------------------------
//
void CSIPExLogView::ConstructL()
    {
    BaseConstructL( R_SIP_EX_VIEW2 );
    
	/* modification: $Author$ */
	symbian_ua_info_cb_t cb;
	Mem::FillZ(&cb, sizeof(cb));

	cb.on_info = &on_info;
	cb.on_incoming_call = &on_incoming_call;
	cb.on_reg_state = &on_reg_state;
	cb.on_unreg_state = &on_unreg_state;
	cb.on_call_end = &on_call_end;

	symbian_ua_set_info_callback(&cb);
	myinstance = this;
    }

// -----------------------------------------------------------------------------
// CSIPExLogView::~CSIPExLogView
// Default destructor
// -----------------------------------------------------------------------------
//
CSIPExLogView::~CSIPExLogView()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }

    delete iContainer;
    }

// -----------------------------------------------------------------------------
// TUid CSIPExLogView::Id
// This returns the view ID
// -----------------------------------------------------------------------------
//
TUid CSIPExLogView::Id() const
    {
    return KLogViewId;
    }

// -----------------------------------------------------------------------------
// CSIPExLogView::HandleCommandL
// Here we handle commands for this view. 
// Each view has their own HandleCommandL()
// -----------------------------------------------------------------------------
//
/* modification: $Author$ */
void CSIPExLogView::HandleCommandL(TInt aCommand)
{
	TBool commandHandled = EFalse;
	switch (aCommand)
	{
		case ESIPExLogViewControlPaneRightId:
		{
			commandHandled = CallSoftKeyPressedL(aCommand);
			break;
		}
		default:
			break;
	}

	if (!commandHandled)
	{
		if (aCommand == EVoIPSIPSettingsMenuItemCommand)
		{
			AppUi()->ActivateLocalViewL(TUid::Uid(ESymbian_ua_guiSettingItemListViewId));
		}
		else if (aCommand == ESIPExLogViewControlPaneRightId)
		{
			AppUi()->HandleCommandL(EEikCmdExit/*EAknSoftkeyBack*/); // this temp, back command will swap
		}
		else
		{
			AppUi()->HandleCommandL(aCommand);
		}
	}
}

// -----------------------------------------------------------------------------
// CSIPExLogView::HandleViewRectChange
// -----------------------------------------------------------------------------
//
void CSIPExLogView::HandleViewRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }
    
// -----------------------------------------------------------------------------
// CSIPExLogView::DynInitMenuPaneL
// Handles menu initialization event.
// -----------------------------------------------------------------------------
//
void CSIPExLogView::DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane )
    {
    }

// -----------------------------------------------------------------------------
// CSIPExLogView::DoActivateL
// This is called when a view needs to be activated. 
// This creates container with its controls.
// -----------------------------------------------------------------------------
//
void CSIPExLogView::DoActivateL(
   const TVwsViewId& /*aPrevViewId*/,TUid /*aCustomMessageId*/,
   const TDesC8& /*aCustomMessage*/ )
    {
   
    // Create Container
    if (!iContainer)
        {
        iContainer = new (ELeave) CSIPExLogContainer( /*iEngine*/ );
        iContainer->SetMopParent( this );
        iContainer->ConstructL( ClientRect() );
        AppUi()->AddToStackL( *this, iContainer );
        }
    
    /* modification: $Author$ */
	CVoIPDocument* doc = static_cast<CVoIPDocument*> (CEikonEnv::Static()->EikAppUi()->Document());
	CEikButtonGroupContainer* cba = CEikButtonGroupContainer::Current();
	if (cba != NULL)
	{
		// we haven't registered pjsip_ua or we don't have phone number then just return to contatcs view
		if(/*doc->IsSIPStarted() && */doc->GetPhoneNumber().Compare(KBlank) != 0)
		{
			TRAPD(result, cba->SetCommandL(
					ESIPExLogViewControlPaneRightId, KStCall));
			cba->DrawDeferred();
		}
		else
		{
			TRAPD(result, cba->SetCommandL(
					ESIPExLogViewControlPaneRightId, KStBack));
			cba->DrawDeferred();
		}
    }
    
   }

// -----------------------------------------------------------------------------
// CSIPExLogView::HandleCommandL
// This is called when a view needs to be deactivated. 
// This destroys container with its controls.
// -----------------------------------------------------------------------------
//
void CSIPExLogView::DoDeactivate()
    {
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    
    delete iContainer;
    iContainer = NULL;
    }
    
// -----------------------------------------------------------------------------
// CSIPExLogView::Container
// -----------------------------------------------------------------------------
//
CSIPExLogContainer* CSIPExLogView::Container()
    {
    return iContainer;
    }

/**
 * Show the popup note for note_error
 * @param aOverrideText optional override text
 */
 /* modification: $Author$ */
void CSIPExLogView::RunNote_errorL(const TDesC* aOverrideText)
{
	CAknErrorNote* note = new (ELeave) CAknErrorNote();
	if (aOverrideText == NULL)
	{
		HBufC* noteText = StringLoader::LoadLC(R_SYMBIAN_UA_GUI_CONTAINER_NOTE_ERROR);
		note->ExecuteLD(*noteText);
		CleanupStack::PopAndDestroy(noteText);
	}
	else
	{
		note->ExecuteLD(*aOverrideText);
	}
}

/**
 * Show the popup note for note_info
 * @param aOverrideText optional override text
 */
 /* modification: $Author$ */
void CSIPExLogView::RunNote_infoL(const TDesC* aOverrideText)
{
	CAknInformationNote* note = new (ELeave) CAknInformationNote();
	if (aOverrideText == NULL)
	{
		HBufC* noteText = StringLoader::LoadLC(R_SYMBIAN_UA_GUI_CONTAINER_NOTE_INFO);
		note->ExecuteLD(*noteText);
		CleanupStack::PopAndDestroy(noteText);
	}
	else
	{
		note->ExecuteLD(*aOverrideText);
	}
}

/**
 * Show the popup note for note_warning
 * @param aOverrideText optional override text
 */
 /* modification: $Author$ */
void CSIPExLogView::RunNote_warningL(const TDesC* aOverrideText)
{
	CAknWarningNote* note = new (ELeave) CAknWarningNote();
	if (aOverrideText == NULL)
	{
		HBufC* noteText = StringLoader::LoadLC(R_SYMBIAN_UA_GUI_CONTAINER_NOTE_WARNING);
		note->ExecuteLD(*noteText);
		CleanupStack::PopAndDestroy(noteText);
	}
	else
	{
		note->ExecuteLD(*aOverrideText);
	}
}

/**
 * Show the popup dialog for qry_accept_call
 * @param aOverrideText optional override text
 * @return EAknSoftkeyYes (left soft key id) or 0
 */
 /* modification: $Author$ */
TInt CSIPExLogView::RunQry_accept_callL(
		const TDesC* aOverrideText)
{

	CAknQueryDialog* queryDialog = CAknQueryDialog::NewL();

	if (aOverrideText != NULL)
	{
		queryDialog->SetPromptL(*aOverrideText);
	}
	return queryDialog->ExecuteLD(R_SYMBIAN_UA_GUI_CONTAINER_QRY_ACCEPT_CALL);
}

/** 
 * Handle the rightSoftKeyPressed event.
 * @return ETrue if the command was handled, EFalse if not
 */
 /* modification: $Author$ */
TBool CSIPExLogView::CallSoftKeyPressedL(TInt aCommand)
{
	CEikButtonGroupContainer* cba = CEikButtonGroupContainer::Current();

	CVoIPDocument* doc = static_cast<CVoIPDocument*> (CEikonEnv::Static()->EikAppUi()->Document());
	
	// CASE 1: we are already in call
	
	// we are in call, then end the call
	if (symbian_ua_anycall())
	{
		symbian_ua_endcall();
		return ETrue;
	}
	
	// CASE 2: no phone number to call
	 
	// we haven't registered pjsip_ua or we don't have phone number then just return to contatcs view
	if(/*!doc->IsSIPStarted() || */doc->GetPhoneNumber().Compare(KBlank) == 0)
	{
		return EFalse;
	}

	// CASE 3: we want make a call
	
	// Init PJSUA
	TSymbian_ua_guiSettingItemListSettings* iSettings = TSymbian_ua_guiSettingItemListSettings::NewL();
	
	if(symbian_is_connected() == 0)
	{
		if (symbian_ua_init() != 0)
		{
			symbian_ua_destroy();
			return ETrue;
		}
	}
	
	doc->LoadSettingValuesL(*iSettings);

	TUint8 domain[256] =
	{ 0 };
	TPtr8 cDomain(domain, sizeof(domain));
	TUint8 user[64] =
	{ 0 };
	TPtr8 cUser(user, sizeof(user));
	TUint8 pass[64] =
	{ 0 };
	TPtr8 cPass(pass, sizeof(pass));

	cDomain.Copy(iSettings->Ed_registrar());
	cUser.Copy(iSettings->Ed_user());
	cPass.Copy(iSettings->Ed_password());
	symbian_ua_set_account((char*) domain, (char*) user, (char*) pass, false, false);

	TUint8 url[256];
	TPtr8 aUrl(url, 256);
				
	HBufC *number = HBufC::NewL(doc->GetPhoneNumber().Length());
	TPtr numberPtr(number->Des());
	numberPtr.Copy(doc->GetPhoneNumber());
	
	HBufC *domainBuf = HBufC::NewL(iSettings->Ed_registrar().Length());

	TInt startingPos = iSettings->Ed_registrar().Find(_L(":"));
	if(startingPos == KErrNotFound)
	{		
		TPtr domainPtr(domainBuf->Des());
		domainPtr.Copy(iSettings->Ed_registrar());
	}
	else
	{
		TPtrC subDomain(iSettings->Ed_registrar().Mid(startingPos+1)); 
		TPtr ptext(domainBuf->Des());
		ptext.Copy(subDomain);
	}
	
	_LIT(KSIPURLFormat, "sip:%S@%S");
	
	HBufC* sipURL = HBufC::NewLC(6 + number->Length() + domainBuf->Length());
	sipURL->Des().Format(KSIPURLFormat, number, domainBuf);
	
	doc->StorePhoneNumber(KBlank);
	
	//HBufC *buf = heapbasedFred;/*obj_url->GetTextInHBufL();*/
	CnvUtfConverter::ConvertFromUnicodeToUtf8(aUrl, *sipURL);

	PutMessage(_L("Calling: "));
	PutMessage(*sipURL);
	delete sipURL;
	delete number;
	delete domainBuf;
	
	
	if (cba != NULL)
	{
		cba->SetCommandL(aCommand, KStHangUp);
		cba->DrawDeferred();
	}
	
	
	if (symbian_ua_makecall((char *) aUrl.PtrZ()) != 0)
	{
		PutMessage(_L("Making call failed!"));
		if (cba != NULL)
		{
			cba->SetCommandL(aCommand, KStBack);
			cba->DrawDeferred();
		}
	}
	
	return ETrue;
}

// End of File

