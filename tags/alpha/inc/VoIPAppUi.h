/*
 ========================================================================
 $Id$
 $Author$
 $Date$
 $Revision$
 Copyright (c) 2010 Fandango60.  
 ========================================================================
 */

#ifndef VOIPAPPUI_H
#define VOIPAPPUI_H

#include <aknviewappui.h>
#include <cpbkcontactitem.h>

#include <aknnavi.h> 
#include <akntabgrp.h>
#include <aknnavide.h>
#include <eikspane.h> 

class CVoIPListBoxView;
class CSIPExLogView;
class Csymbian_ua_guiSettingItemListView;
class CVoIPSettingFormView;
class CVoIPDocument;
class CPhoneBkEngine;
class CContact;
class CPhoneHelper;

/**
 * @class	CVoIPAppUi VoIPAppUi.h
 * @brief The AppUi class handles application-wide aspects of the user interface, including
 *        view management and the default menu, control pane, and status pane.
 */
class CVoIPAppUi : public CAknViewAppUi
    {
public:
    // constructor and destructor
    CVoIPAppUi();
    virtual ~CVoIPAppUi();
    void ConstructL();

public:
    // from CCoeAppUi
    TKeyResponse
            HandleKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);

    // from CEikAppUi
    void HandleCommandL(TInt aCommand);
    void HandleResourceChangeL(TInt aType);

    // from CAknAppUi
    void HandleViewDeactivation(const TVwsViewId& aViewIdToBeDeactivated,
            const TVwsViewId& aNewlyActivatedViewId);

private:
    void InitializeContainersL();
    void ShowTabIcons();

public:
    CPhoneBkEngine* GetPhoneBook();
    CPhoneHelper* GetPhoneHelper();
    TBool NotifyBillableEvent();

private:
    CVoIPListBoxView* iVoIPListBoxView;
    CSIPExLogView* iSIPExLogView;
    CVoIPSettingFormView* iVoIPSettingFormView;
    Csymbian_ua_guiSettingItemListView* iSymbian_ua_guiSettingItemListView;
    CPhoneBkEngine* iPhoneEngine;
    CPhoneHelper* iPhoneHelper;
    TInt iOffset;
    CAknTabGroup* iTabGroup;
    CAknNavigationDecorator* iDecoratedTabGroup;
    CAknNavigationControlContainer* iNaviPane;
    CEikStatusPane* sp;
    };

#endif // VOIPAPPUI_H
