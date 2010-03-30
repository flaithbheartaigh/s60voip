/*
 ========================================================================
 $Id$
 $Author$
 $Date$
 $Revision$
 Copyright (c) 2010 Fandango60.  
 ========================================================================
 */

#ifndef VOIPLISTBOX_H
#define VOIPLISTBOX_H

#include <coecntrl.h>
#include <aknsfld.h>
#include <logwrap.h>

class MEikCommandObserver;
class CAknSingleGraphicStyleListBox;
class CEikTextListBox;

const TInt KAknExListFindBoxTextLength = 20;

/**
 * Container class for VoIPListBox
 * 
 * @class	CVoIPListBox VoIPListBox.h
 */
class CVoIPListBox : public CCoeControl, public MEikListBoxObserver
    {
public:
    // constructors and destructor
    CVoIPListBox();
    static CVoIPListBox* NewL(const TRect& aRect, const CCoeControl* aParent,
            MEikCommandObserver* aCommandObserver);
    static CVoIPListBox
            * NewLC(const TRect& aRect, const CCoeControl* aParent,
                    MEikCommandObserver* aCommandObserver);
    void ConstructL(const TRect& aRect, const CCoeControl* aParent,
            MEikCommandObserver* aCommandObserver);
    virtual ~CVoIPListBox();

public:
    // from base class CCoeControl
    TInt CountComponentControls() const;
    CCoeControl* ComponentControl(TInt aIndex) const;
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,
            TEventCode aEventCode);
    void HandleResourceChange(TInt aType);

protected:
    // from base class CCoeControl
    void SizeChanged();

private:
    // from base class CCoeControl
    void Draw(const TRect& aRect) const;
    /*!
     * From MEikListBoxObserver,HandleListBoxEventL.
     */
    void HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType);

    void UpdateScrollBar(CEikListBox* aListBox);

private:
    void InitializeControlsL();
    MEikCommandObserver* iCommandObserver;

public:
    static void AddListBoxItemL(CEikTextListBox* aListBox,
            const TDesC& aString);
    static RArray<TInt>* GetSelectedListBoxItemsLC(CEikTextListBox* aListBox);
    static void DeleteSelectedListBoxItemsL(CEikTextListBox* aListBox);
    CAknSingleStyleListBox* ListBox();
    void SetupListBoxIconsL();
    void PopulateListBoxL();
    CAknSearchField* CreateFindBoxL(CEikListBox* aListBox,
            CTextListBoxModel* aModel,
            CAknSearchField::TSearchFieldStyle aStyle);
    TBool HandleMarkableListCommandL(TInt aCommand);

private:
    CAknSingleStyleListBox* iListBox;
    CAknSearchField* iFindBox;
    TBuf<30> KUnnamedStr;

    };

#endif // VOIPLISTBOX_H
