/*
 ========================================================================
 $Id$
 $Author$
 $Date$
 $Revision$
 Copyright (c) 2010 Fandango60.  
 ========================================================================
 */

#ifndef VOIPDOCUMENT_H
#define VOIPDOCUMENT_H

#include <akndoc.h>

class CEikAppUi;
class CPhoneBkEngine;
class Contact;
class TSymbian_ua_guiSettingItemListSettings;

/**
 * @class	CVoIPDocument VoIPDocument.h
 * @brief	A CAknDocument-derived class is required by the S60 application 
 *           framework. It is responsible for creating the AppUi object. 
 */
class CVoIPDocument : public CAknDocument
    {
public:
    // constructor
    static CVoIPDocument* NewL(CEikApplication& aApp);

private:
    // constructors
    CVoIPDocument(CEikApplication& aApp);
    void ConstructL();
    // destructors
    ~CVoIPDocument();

public:
    // from base class CEikDocument
    CEikAppUi* CreateAppUiL();
    TInt ModifyIndex() const;
    void SetModifyIndex(const TInt& aIndex);
    /**
     * Info text is always stored in document. Text will be appended
     * to existing info text unless max size is reached. In such situation
     * oldest text will be removed.
     * @param aText info text
     */
    void StoreInfoTextL(const TDesC& aText);
    /**
     * Returns reference to current info text
     */
    const TDesC& InfoText();

    void
            SaveSettingValuesL(
                    TSymbian_ua_guiSettingItemListSettings& aSettings);
    void
            LoadSettingValuesL(
                    TSymbian_ua_guiSettingItemListSettings& aSettings);

    void StorePhoneNumber(const TDesC& aText);
    const TDesC& GetPhoneNumber();

private:
    CPhoneBkEngine* iPhoneEngine;
    TInt iModifyIndex;
    HBufC* iInfoText; // Owned: Current info text
    HBufC* iPhoneNumber;
    };
#endif // VOIPDOCUMENT_H
