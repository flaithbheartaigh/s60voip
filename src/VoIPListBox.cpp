/*
 ========================================================================
 $Id$
 $Author$
 $Date$
 $Revision$
 Copyright (c) 2010 Fandango60.  
 ========================================================================
 */

#include <barsread.h>
#include <stringloader.h>
#include <aknlists.h>
#include <eikenv.h>
#include <akniconarray.h>
#include <eikclbd.h>
#include <aknviewappui.h>
#include <eikappui.h>
#include <eiklbo.h>
#include <aknnotewrappers.h>
#include <VoIP.rsg>

#include "VoIPListBox.h"
#include "VoIPListBoxView.h"
#include "VoIP.hrh"
#include "VoIPAppUi.h"
#include "Contact.h"
#include "PhoneBkEngine.h"
#include "VoIPDocument.h"

CVoIPListBox::CVoIPListBox()
    {
    iListBox = NULL;
    iFindBox = NULL;
    }

CVoIPListBox::~CVoIPListBox()
    {
    delete iListBox;
    iListBox = NULL;

    delete iFindBox;
    iFindBox = NULL;
    }

CVoIPListBox* CVoIPListBox::NewL(const TRect& aRect,
        const CCoeControl* aParent, MEikCommandObserver* aCommandObserver)
    {
    CVoIPListBox* self =
            CVoIPListBox::NewLC(aRect, aParent, aCommandObserver);
    CleanupStack::Pop(self);
    return self;
    }

CVoIPListBox* CVoIPListBox::NewLC(const TRect& aRect,
        const CCoeControl* aParent, MEikCommandObserver* aCommandObserver)
    {
    CVoIPListBox* self = new (ELeave) CVoIPListBox();
    CleanupStack::PushL(self);
    self->ConstructL(aRect, aParent, aCommandObserver);
    return self;
    }

void CVoIPListBox::ConstructL(const TRect& aRect, const CCoeControl* aParent,
        MEikCommandObserver* aCommandObserver)
    {
    CEikonEnv::Static()->ReadResource(KUnnamedStr, R_UNNAMED);

    if (aParent == NULL)
        {
        CreateWindowL();
        }
    else
        {
        SetContainerWindowL(*aParent);
        }
    iCommandObserver = aCommandObserver;
    InitializeControlsL();
    SetRect(aRect);
    ActivateL();
    }

/**
 * Return the number of controls in the container (override)
 * @return count
 */
TInt CVoIPListBox::CountComponentControls() const
    {
    TInt count = 0;
    if (iListBox)
        count++;
    if (iFindBox)
        count++;
    return count;
    }

/**
 * Get the control with the given index (override)
 * @param aIndex Control index [0...n) (limited by #CountComponentControls)
 * @return Pointer to control
 */
CCoeControl* CVoIPListBox::ComponentControl(TInt aIndex) const
    {
    switch (aIndex)
        {
        case 0:
            return iListBox;
        case 1:
            return iFindBox;
        default:
            return NULL;
        }
    }

/**
 *	Handle resizing of the container. This implementation will lay out
 *  full-sized controls like list boxes for any screen size, and will layout
 *  labels, editors, etc. to the size they were given in the UI designer.
 *  This code will need to be modified to adjust arbitrary controls to
 *  any screen size.
 */
void CVoIPListBox::SizeChanged()
    {
    if (iListBox)
        {
        if (iFindBox)
            {
            CAknColumnListBox* aknListBox = STATIC_CAST(CAknColumnListBox*,
                    iListBox);
            AknFind::HandleFixedFindSizeChanged(this, aknListBox, iFindBox);

            }
        else
            {
            iListBox->SetRect(Rect()); // Sets rectangle of lstbox.
            }
        }
    }

TKeyResponse CVoIPListBox::OfferKeyEventL(const TKeyEvent& aKeyEvent,
        TEventCode aEventCode)
    {
    if (aKeyEvent.iCode == EKeyLeftArrow || aKeyEvent.iCode == EKeyRightArrow)
        {
        return EKeyWasNotConsumed;
        }

    if (iListBox)
        {
        if (iFindBox)
            {
            TBool needRefresh(EFalse);

            // Offers the key event to find box.
            if (AknFind::HandleFindOfferKeyEventL(aKeyEvent, aEventCode,
                    this, iListBox, iFindBox, EFalse, needRefresh)
                    == EKeyWasConsumed)
                {
                if (needRefresh)
                    {
                    SizeChanged();
                    DrawNow();
                    }

                return EKeyWasConsumed;
                }
            }

        return iListBox->OfferKeyEventL(aKeyEvent, aEventCode);
        }
    else
        {
        return EKeyWasNotConsumed;
        }
    }

/**
 *	Initialize each control upon creation.
 */
void CVoIPListBox::InitializeControlsL()
    {
    iListBox = new (ELeave) CAknSingleStyleListBox();

    iListBox->SetContainerWindowL(*this);
        {
        TResourceReader reader;
        iEikonEnv->CreateResourceReaderLC(reader, R_VOIPLISTBOX_LIST_BOX );
        iListBox->ConstructFromResourceL(reader);
        CleanupStack::PopAndDestroy(); // reader internal state
        }

    iListBox->CreateScrollBarFrameL(ETrue);
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
            CEikScrollBarFrame::EOn, CEikScrollBarFrame::EAuto);

    // the listbox owns the items in the list and will free them
    iListBox->Model()->SetOwnershipType(ELbmOwnsItemArray);

    // set up the observer to call HandleListBoxEventL().
    iListBox->SetListBoxObserver(this);

    // setup the icon array so graphics-style boxes work
    SetupListBoxIconsL();

    // create find box
    CAknSearchField::TSearchFieldStyle style(CAknSearchField::ESearch);
    iFindBox = CreateFindBoxL(iListBox, iListBox->Model(), style);
    iFindBox->MakeVisible(ETrue);

    // add list items
    PopulateListBoxL();

    SizeChanged();
    iListBox->ActivateL();
    UpdateScrollBar(iListBox);
    DrawNow();
    }

void CVoIPListBox::UpdateScrollBar(CEikListBox* aListBox)
    {
    if (aListBox)
        {
        TInt pos(aListBox->View()->CurrentItemIndex());
        if (aListBox->ScrollBarFrame())
            {
            aListBox->ScrollBarFrame()->MoveVertThumbTo(pos);
            }
        }
    }

/** 
 * Handle global resource changes, such as scalable UI or skin events (override)
 */
void CVoIPListBox::HandleResourceChange(TInt aType)
    {
    TRect rect;
    if (aType == KEikDynamicLayoutVariantSwitch)
        {
        AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane, rect);

        SetRect(rect);
        }
    CCoeControl::HandleResourceChange(aType);
    }

/**
 *	Draw container contents.
 */
void CVoIPListBox::Draw(const TRect& aRect) const
    {
    CWindowGc& gc = SystemGc();
    gc.Clear(aRect);
    }

/**
 *	Add a list box item to a list.
 */
void CVoIPListBox::AddListBoxItemL(CEikTextListBox* aListBox,
        const TDesC& aString)
    {
    CTextListBoxModel* model = aListBox->Model();
    CDesCArray* itemArray = static_cast<CDesCArray*> (model->ItemTextArray());
    itemArray->AppendL(aString);
    aListBox->HandleItemAdditionL();
    }

/**
 * Get the array of selected item indices, with respect to the list model.
 * The array is sorted in ascending order.
 * The array should be destroyed with two calls to CleanupStack::PopAndDestroy(),
 * the first with no argument (referring to the internal resource) and the
 * second with the array pointer.
 * @return newly allocated array, which is left on the cleanup stack;
 *	or NULL for empty list. 
 */
RArray<TInt>* CVoIPListBox::GetSelectedListBoxItemsLC(
        CEikTextListBox* aListBox)
    {
    CAknFilteredTextListBoxModel* model =
            static_cast<CAknFilteredTextListBoxModel *> (aListBox->Model());
    if (model->NumberOfItems() == 0)
        return NULL;

    // get currently selected indices
    const CListBoxView::CSelectionIndexArray* selectionIndexes =
            aListBox->SelectionIndexes();
    TInt selectedIndexesCount = selectionIndexes->Count();
    if (selectedIndexesCount == 0)
        return NULL;

    // copy the indices and sort numerically
    RArray<TInt>* orderedSelectedIndices = new (ELeave) RArray<TInt> (
            selectedIndexesCount);

    // push the allocated array
    CleanupStack::PushL(orderedSelectedIndices);

    // dispose the array resource
    CleanupClosePushL(*orderedSelectedIndices);

    // see if the search field is enabled
    CAknListBoxFilterItems* filter = model->Filter();
    if (filter != NULL)
        {
        // when filtering enabled, translate indices back to underlying model
        for (TInt idx = 0; idx < selectedIndexesCount; idx++)
            {
            TInt filteredItem = (*selectionIndexes)[idx];
            TInt actualItem = filter->FilteredItemIndex(filteredItem);
            orderedSelectedIndices->InsertInOrder(actualItem);
            }
        }
    else
        {
        // the selection indices refer directly to the model
        for (TInt idx = 0; idx < selectedIndexesCount; idx++)
            orderedSelectedIndices->InsertInOrder((*selectionIndexes)[idx]);
        }

    return orderedSelectedIndices;
    }

/**
 * Delete the selected item or items from the list box.
 */
void CVoIPListBox::DeleteSelectedListBoxItemsL(CEikTextListBox* aListBox)
    {
    CAknFilteredTextListBoxModel* model =
            static_cast<CAknFilteredTextListBoxModel *> (aListBox->Model());
    if (model->NumberOfItems() == 0)
        return;

    RArray<TInt>* orderedSelectedIndices =
            GetSelectedListBoxItemsLC(aListBox);
    if (!orderedSelectedIndices)
        return;

    // Delete selected items from bottom up so indices don't change on us
    CDesCArray* itemArray = static_cast<CDesCArray*> (model->ItemTextArray());
    TInt currentItem = 0;

    for (TInt idx = orderedSelectedIndices->Count(); idx-- > 0;)
        {
        currentItem = (*orderedSelectedIndices)[idx];
        itemArray->Delete(currentItem);
        }

    // dispose the array resources
    CleanupStack::PopAndDestroy();

    // dispose the array pointer
    CleanupStack::PopAndDestroy(orderedSelectedIndices);

    // refresh listbox's cursor now that items are deleted
    AknListBoxUtils::HandleItemRemovalAndPositionHighlightL(aListBox,
            currentItem, ETrue);
    }

/**
 *	Get the listbox.
 */
CAknSingleStyleListBox* CVoIPListBox::ListBox()
    {
    return iListBox;
    }

/**
 *	Set up the list's icon array.
 */
void CVoIPListBox::SetupListBoxIconsL()
    {
    CArrayPtr<CGulIcon>* icons = NULL;

    if (icons != NULL)
        {
        iListBox->ItemDrawer()->ColumnData()->SetIconArray(icons);
        }
    }

/**
 *	Set up the list's contents.
 */
void CVoIPListBox::PopulateListBoxL()
    {
    CVoIPAppUi* iAppUi = static_cast<CVoIPAppUi*> (iEikonEnv->EikAppUi());
    RPointerArray<CContact> contacts = iAppUi->GetPhoneBook()->GetContactsL();
    _LIT(KEntryFormatOneWord, "%d\t%S");
    _LIT(KEntryFormatTwoWord, "%d\t%S %S");
    const TInt bufferspace = 30; //tabs and such

    TInt size = contacts.Count();

    for (int i = 0; i < size; i++)
        {
        CContact* contact = contacts[i];

        TInt firstnamelength = contact->GetFirstName()->Length();
        TInt lastnamelength = contact->GetLastName()->Length();

        HBufC* combinedDes = NULL;

        if (firstnamelength != 0 && lastnamelength != 0)
            {
            combinedDes = HBufC::NewLC(firstnamelength + lastnamelength
                    + bufferspace);
            if (i % 2)
                {
                combinedDes->Des().Format(KEntryFormatTwoWord, 0,
                        contact->GetFirstName(), contact->GetLastName());
                }
            else
                {
                combinedDes->Des().Format(KEntryFormatTwoWord, 1,
                        contact->GetFirstName(), contact->GetLastName());
                }

            }
        else if (firstnamelength != 0)
            {
            combinedDes = HBufC::NewLC(firstnamelength + bufferspace);
            if (i % 2)
                {
                combinedDes->Des().Format(KEntryFormatOneWord(), 0,
                        contact->GetFirstName());
                }
            else
                {
                combinedDes->Des().Format(KEntryFormatOneWord(), 1,
                        contact->GetFirstName());
                }

            }
        else if (lastnamelength != 0)
            {
            combinedDes = HBufC::NewLC(lastnamelength + bufferspace);
            if (i % 2)
                {
                combinedDes->Des().Format(KEntryFormatOneWord(), 0,
                        contact->GetLastName());
                }
            else
                {
                combinedDes->Des().Format(KEntryFormatOneWord(), 1,
                        contact->GetLastName());
                }

            }
        else
            {
            combinedDes = HBufC::NewLC(KUnnamedStr.Length() + bufferspace);
            if (i % 2)
                {
                combinedDes->Des().Format(KEntryFormatOneWord(), 0,
                        &KUnnamedStr);
                }
            else
                {
                combinedDes->Des().Format(KEntryFormatOneWord(), 1,
                        &KUnnamedStr);
                }
            }

        if (combinedDes != NULL)
            {
            AddListBoxItemL(iListBox, combinedDes->Des());
            CleanupStack::PopAndDestroy(combinedDes);
            }
        }

    if (size > 0)
        {
        CVoIPDocument* iDocument =
                static_cast<CVoIPDocument*> (iAppUi->Document());

        TInt iModifyIndex = iDocument->ModifyIndex();

        if (iModifyIndex >= 0)
            {
            iListBox->SetCurrentItemIndexAndDraw(iModifyIndex);
            }
        }
    else
        {
        CAknInformationNote* note = new (ELeave) CAknInformationNote;
        HBufC* errorMsg = CCoeEnv::Static()->AllocReadResourceLC(
                R_BIRTHDAYS_NO_ENTRIES_FOUND);
        note->ExecuteLD(*errorMsg);
        CleanupStack::PopAndDestroy(errorMsg);
        }
    }

/** 
 *	Handle commands relating to markable lists.
 */
TBool CVoIPListBox::HandleMarkableListCommandL(TInt aCommand)
    {
    return EFalse;
    }

// Creates FindBox and returns it's pointer.
CAknSearchField* CVoIPListBox::CreateFindBoxL(CEikListBox* aListBox,
        CTextListBoxModel* aModel, CAknSearchField::TSearchFieldStyle aStyle)
    {
    CAknSearchField* findbox = NULL;

    if (aListBox && aModel)
        {
        // Gets pointer of CAknFilteredTextListBoxModel.
        CAknFilteredTextListBoxModel* model = STATIC_CAST( CAknFilteredTextListBoxModel*, aModel );

        // Creates FindBox.
        //CAknSearchField::TSearchFieldStyle style(CAknSearchField::ESearch);
        findbox = CAknSearchField::NewL(*this, aStyle, NULL,
                KAknExListFindBoxTextLength);
        findbox->SetSkinEnabledL(ETrue);

        CleanupStack::PushL(findbox);
        // Creates CAknListBoxFilterItems class.
        model->CreateFilterL(aListBox, findbox);
        // Filter can get by model->Filter();
        CleanupStack::Pop(findbox); // findbox
        }

    return findbox;
    }

void CVoIPListBox::HandleListBoxEventL(CEikListBox* /*aListBox*/,
        TListBoxEvent aEventType)
    {
    if (aEventType == EEventEnterKeyPressed || aEventType
            == EEventItemDoubleClicked)
        {
        iCommandObserver->ProcessCommandL(EVoIPOpenMenuItemCommand);
        }
    }
