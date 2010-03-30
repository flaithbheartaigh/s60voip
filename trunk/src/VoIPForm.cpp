/*
 ========================================================================
 $Id$
 $Author$
 $Date$
 $Revision$
 Copyright (c) 2010 Fandango60.  
 ========================================================================
 */
#include <avkon.rsg>
#include <EIKMENUP.H>
#include <eikcapc.h>
#include <eikmfne.h> 
#include <cpbkfieldinfo.h>
#include <cpbkfieldsinfo.h> 
#include <aknmessagequerydialog.h>
#include <VoIP.rsg>
#include <AknSelectionList.h>
#include "VoIPForm.h"
#include "VoIP.hrh"
#include "VoIPAppUi.h"
#include "VoIPDocument.h"
#include "VoIPListBoxView.h"
#include "PhoneBkEngine.h"
#include "Contact.h"
// Constants
_LIT(mindateTime, "19000101:");
_LIT(maxdateTime, "21000101:");
_LIT(nowdateTime, "20090101:");

CVoIPForm::CVoIPForm()
    {
    }

CVoIPForm::~CVoIPForm()
    {
    }

CVoIPForm* CVoIPForm::NewL()
    {
    CVoIPForm * self = CVoIPForm::NewLC();
    CleanupStack::Pop(self);
    return new (ELeave) CVoIPForm();
    }

CVoIPForm* CVoIPForm::NewLC()
    {
    CVoIPForm * self = new (ELeave) CVoIPForm();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

TBool CVoIPForm::SaveFormDataL()
    {
    CVoIPAppUi* iAppUi = static_cast<CVoIPAppUi*> (iEikonEnv->EikAppUi());
    CVoIPDocument* iDocument =
            static_cast<CVoIPDocument*> (iAppUi->Document());

    TInt iModifyIndex = iDocument->ModifyIndex();
    RPointerArray<CContact> contacts = iAppUi->GetPhoneBook()->GetContactsL();

    if (iModifyIndex >= 0)
        {
        CContact* contact = contacts[iModifyIndex];

        if (contact)
            {
            iAppUi->GetPhoneBook()->DeleteContactItem(contact->GetEntryId());

            TContactItemId contactItemId =
                    iAppUi->GetPhoneBook()->CreateDefaultContactItem();

            contact->SetEntryId(contactItemId);

            CPbkContactItem* contactItem =
                    iAppUi->GetPhoneBook()->OpenContactItemLCX(contactItemId);

            TInt pageId = ActivePageId();

            RArray<CEikCaptionedControl*> aControls;

            ControlsOnPage(aControls, pageId);

            for (TInt i = 0; i < aControls.Count(); i++)
                {
                CEikCaptionedControl* control =
                        ((CEikCaptionedControl*) aControls[i]);

                TPbkContactItemField* field = contactItem->FindField(
                        control->iId);

                if (field == NULL)
                    {
                    CPbkFieldInfo* fieldInfo =
                            iAppUi->GetPhoneBook()->GetFieldsInfo().Find(
                                    control->iId);
                    contactItem->AddFieldL(*fieldInfo);
                    field = contactItem->FindField(control->iId);
                    }

                field->SetLabelL(control->GetFullCaptionText());

                CPbkFieldInfo& fieldInfo = field->FieldInfo();
                if (fieldInfo.CtrlType() == EPbkFieldCtrlTypeDateEditor)
                    {
                    CEikDateEditor* nEditor =
                            static_cast<CEikDateEditor*> (ControlOrNull(
                                    control->iId));
                    TTime time = nEditor->Date();
                    field->DateTimeStorage()->SetTime(time);
                    }
                else
                    {
                    CEikEdwin* nEditor =
                            static_cast<CEikEdwin*> (ControlOrNull(
                                    control->iId));
                    HBufC* text = nEditor->GetTextInHBufL();
                    field->TextStorage()->SetText(text);
                    }
                }

            aControls.Reset();

            iAppUi->GetPhoneBook()->CommitContactItem(contactItem);
            CleanupStack::PopAndDestroy(2); //OpenContactItemLCX
            }
        }
    return ETrue;
    }

void CVoIPForm::LoadFormValuesFromDataL()
    {
    TInt pageId = ActivePageId();

    // Delete all controls not saved.

    RArray<CEikCaptionedControl*> aControls;

    ControlsOnPage(aControls, pageId);

    for (TInt i = 0; i < aControls.Count(); i++)
        {
        CEikCaptionedControl* control =
                ((CEikCaptionedControl*) aControls[i]);
        DeleteLine(control->iId, EFalse);
        }

    aControls.Reset();

    // Create controls based on PhoneBook contact item.

    CVoIPAppUi* iAppUi = static_cast<CVoIPAppUi*> (iEikonEnv->EikAppUi());
    CVoIPDocument* iDocument =
            static_cast<CVoIPDocument*> (iAppUi->Document());

    TInt iModifyIndex = iDocument->ModifyIndex();
    RPointerArray<CContact> contacts = iAppUi->GetPhoneBook()->GetContactsL();

    if (iModifyIndex >= 0)
        {
        CContact* contact = contacts[iModifyIndex];

        if (contact)
            {
            CPbkContactItem* contactItem =
                    iAppUi->GetPhoneBook()->ReadContactItemLC(
                            contact->GetEntryId());

            CPbkFieldArray& fieldArray = contactItem->CardFields();

            for (TInt i = 0; i < fieldArray.Count(); i++)
                {
                TPbkContactItemField field = fieldArray[i];
                if (field.IsEmpty() == EFalse)
                    {
                    CPbkFieldInfo& fieldInfo = field.FieldInfo();
                    TInt id = fieldInfo.FieldId();
                    TAny* unused = 0;

                    if (fieldInfo.CtrlType() == EPbkFieldCtrlTypeTextEditor)
                        {
                        TInt type = EEikCtEdwin;

                        CEikEdwin * nEditor =
                                static_cast<CEikEdwin*> (ControlOrNull(id));
                        if (!nEditor)
                            {
                            CEikEdwin* edwin =
                                    (CEikEdwin*) CreateLineByTypeL(
                                            field.Label(), pageId, id, type,
                                            unused);

                            edwin->ConstructL(EEikEdwinNoHorizScrolling
                                    | EEikEdwinResizable, 10, 100, 1);

                            edwin->SetTextL(&(field.Text()));
                            }
                        }
                    else if (fieldInfo.CtrlType()
                            == EPbkFieldCtrlTypeNumberEditor)
                        {
                        TInt type = EAknCtIntegerEdwin;

                        CAknNumericEdwin
                                * nEditor =
                                        static_cast<CAknNumericEdwin*> (ControlOrNull(
                                                id));
                        if (!nEditor)
                            {
                            CAknNumericEdwin* edwin =
                                    (CAknNumericEdwin*) CreateLineByTypeL(
                                            field.Label(), pageId, id, type,
                                            unused);

                            edwin->ConstructL(EEikEdwinNoHorizScrolling
                                    | EEikEdwinResizable, 10, 100, 1);

                            edwin->SetTextL(&(field.Text()));
                            }
                        }
                    else if (fieldInfo.CtrlType()
                            == EPbkFieldCtrlTypeDateEditor)
                        {
                        TInt type = EEikCtDateEditor;

                        CEikDateEditor * nEditor =
                                static_cast<CEikDateEditor*> (ControlOrNull(
                                        id));
                        if (!nEditor)
                            {
                            CEikDateEditor* editor =
                                    (CEikDateEditor*) CreateLineByTypeL(
                                            field.Label(), pageId, id, type,
                                            unused);

                            editor->ConstructL(TTime(mindateTime), TTime(
                                    maxdateTime),
                                    field.DateTimeStorage()->Time(), ETrue);
                            }
                        Line(id)->ActivateL();
                        }
                    }
                }
            SetChangesPending(ETrue);
            UpdatePageL(ETrue);
            CleanupStack::PopAndDestroy(); //contactItem
            }
        }
    }

/**
 * Called by QuerySaveChangeL when the user chooses to discard changes made to 
 * the form. Loads the form values from iOpponent
 */
void CVoIPForm::DoNotSaveFormDataL()
    {
    LoadFormValuesFromDataL();
    }

/**
 * Called by the framework before the form is initialised
 * Loads the form values from iOpponent ready for execution of the form
 */
void CVoIPForm::PreLayoutDynInitL()
    {
    CAknForm::PreLayoutDynInitL();
    LoadFormValuesFromDataL();
    }

/**
 * Called by the framework when a menu is displayed.
 * Removes the default items from the options menu of the form for editing a
 * fields label, adding a field and deleting a field
 */
void CVoIPForm::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane)
    {
    CAknForm::DynInitMenuPaneL(aResourceId, aMenuPane);

    if (aResourceId == R_AVKON_FORM_MENUPANE)
        {
        aMenuPane->SetItemDimmed(EAknFormCmdEdit, TRUE);
        aMenuPane->SetItemDimmed(EAknFormCmdAdd, TRUE); // <-- these are the form's default menu, I choose to hide them

        if (IsEditable())
            {
            aMenuPane->SetItemDimmed(EVoIPCallMenuItemCommand, TRUE);
            aMenuPane->SetItemDimmed(EVoIPMessageMenuItemCommand, TRUE);
            aMenuPane->SetItemDimmed(EVoIPDeleteMenuItemCommand, TRUE); // <-- these are the form's default menu, I choose to hide them
            aMenuPane->SetItemDimmed(EVoIPAboutMenuItemCommand, TRUE);
            aMenuPane->SetItemDimmed(EVoIPHelpMenuItemCommand, TRUE);
            aMenuPane->SetItemDimmed(EVoIPExitMenuItemCommand, TRUE);
            aMenuPane->SetItemDimmed(EVoIPAboutMenuItemCommand, TRUE);
            aMenuPane->SetItemDimmed(EVoIPMarkingMenuItemCommand, TRUE);
            aMenuPane->SetItemDimmed(EVoIPEditMenuItemCommand, TRUE);

            TInt aPos;
            if (aMenuPane->MenuItemExists(EVoIPAddMenuItemCommand, aPos))
                {
                aMenuPane->InsertMenuItemL(aMenuPane->ItemData(
                        EVoIPAddMenuItemCommand), 0);
                aMenuPane->DeleteBetweenMenuItems((aPos + 1), (aPos + 1));
                }
            }
        else
            {
            aMenuPane->SetItemDimmed(EVoIPAddMenuItemCommand, TRUE);
            }
        }
    }

void CVoIPForm::ConstructL()
    {
    CAknForm::ConstructL(R_VOIPFORM_OPTIONS_MENU);
    }

void CVoIPForm::ProcessCommandL(TInt aCommandId)
    {
    CAknForm::ProcessCommandL(aCommandId);

    switch (aCommandId)
        {
        case EVoIPEditMenuItemCommand:
            {
            CAknForm::ProcessCommandL(EAknFormCmdEdit);
            break;
            }
        case EVoIPAddMenuItemCommand:
            {
            CAknForm::ProcessCommandL(EAknFormCmdAdd);

            CVoIPAppUi* iAppUi =
                    static_cast<CVoIPAppUi*> (iEikonEnv->EikAppUi());
            const CPbkFieldsInfo& fieldsInfo =
                    iAppUi->GetPhoneBook()->GetFieldsInfo();

            const TInt count = fieldsInfo.Count();

            CDesCArray* list = new (ELeave) CDesCArrayFlat(count);
            CleanupStack::PushL(list);

            CArrayFixFlat<TInt>* correspondence = new (ELeave) CArrayFixFlat<
                    TInt> (count);
            CleanupStack::PushL(correspondence);

            for (TInt i = 0; i < count; ++i)
                {
                CPbkFieldInfo* fieldInfo = fieldsInfo[i];
                const TDesC* fieldName = &(fieldInfo->FieldName());

                if (fieldName->Length() > 0)
                    {
                    _LIT(KFormat, "\t%S");
                    TBuf<256> fieldStr;
                    fieldStr.Format(KFormat, fieldName);

                    list->AppendL(fieldStr);
                    correspondence->AppendL(i);
                    }
                }

            TInt selectedIndex;
            CAknSelectionListDialog* dialog = CAknSelectionListDialog::NewL(
                    selectedIndex, list, 0);

            if (dialog->ExecuteLD(R_LIST_DIALOG))
                {
                CPbkFieldInfo* fieldInfo =
                        fieldsInfo[(*correspondence)[selectedIndex]];
                const TDesC& fieldName = fieldInfo->FieldName();

                TInt id = fieldInfo->FieldId();
                TInt pageId = ActivePageId();
                TAny* unused = 0;

                if (fieldInfo->EditMode() == EPbkFieldEditModeAlpha)
                    {
                    TInt type = EEikCtEdwin;

                    CEikEdwin* editor = (CEikEdwin*) CreateLineByTypeL(
                            fieldName, pageId, id, type, unused);

                    editor->ConstructL(EEikEdwinNoHorizScrolling
                            | EEikEdwinResizable, 10, 100, 1);

                    }
                else if (fieldInfo->EditMode() == EPbkFieldEditModeNumeric)
                    {
                    TInt type = EEikCtEdwin;

                    CEikEdwin* editor = (CEikEdwin*) CreateLineByTypeL(
                            fieldName, pageId, id, type, unused);

                    editor->SetAknEditorInputMode(EAknEditorNumericInputMode);
                    editor->SetAknEditorAllowedInputModes(
                            EAknEditorNumericInputMode);

                    editor->ConstructL(EEikEdwinNoHorizScrolling
                            | EEikEdwinResizable, 10, 100, 1);
                    }
                else if (fieldInfo->EditMode() == EPbkFieldEditModeDate)
                    {
                    TInt type = EEikCtDateEditor;

                    CEikDateEditor* editor =
                            (CEikDateEditor*) CreateLineByTypeL(fieldName,
                                    pageId, id, type, unused);

                    editor->ConstructL(TTime(mindateTime),
                            TTime(maxdateTime), TTime(nowdateTime), ETrue);
                    }

                Line(id)->ActivateL();
                SetChangesPending(ETrue);
                UpdatePageL(ETrue);
                }

            CleanupStack::PopAndDestroy(2); // list, correspondence
            break;
            }
        default:
            {
            CVoIPAppUi* iAppUi =
                    static_cast<CVoIPAppUi*> (iEikonEnv->EikAppUi());

            CVoIPListBoxView* iView =
                    static_cast<CVoIPListBoxView*> (iAppUi->View(TUid::Uid(
                            EVoIPListBoxViewId)));

            iView->HandleCommandL(aCommandId);

            break;
            }
        }
    }

void CVoIPForm::SetInitialCurrentLine() //virtual func from base class
    {
    CEikDialog::SetInitialCurrentLine();

    CEikCaptionedControl* control = GetFirstLineOnFirstPageOrNull();
    if (control)
        {
        TryChangeFocusToL(control->iId);//Id of focused control
        }
    }
