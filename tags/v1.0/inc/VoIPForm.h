/*
 ========================================================================
 $Id$
 $Author$
 $Date$
 $Revision$
 Copyright (c) 2010 Fandango60.  
 ========================================================================
 */

#ifndef VOIPFORM_H_
#define VOIPFORM_H_

#include <aknform.h>
#include <eikedwin.h>
#include <e32cmn.h>

// Derive the class from CAknForm
class CVoIPForm : public CAknForm
    {
public:
    static CVoIPForm* NewL();
    static CVoIPForm* NewLC();
    virtual ~CVoIPForm();
    
private:
    CVoIPForm();
    void ConstructL();    

public:
    /**
     * From CAknForm, ExecuteLD
     * @return CAknForm::ExecuteLD return value
     * @param aResourceId resource ID
     */
    //TInt ExecuteLD(TInt aResourceId);

    /**
     * From CAknForm, PrepareLC
     * @param aResourceId resource ID
     */
    //void PrepareLC(TInt aResourceId);

private:
    // Functions from base class
    /**
     * From CEikDialog, PostLayoutDynInitL 
     * Set default field value to member data.
     */
    void PreLayoutDynInitL();

    /**
     * From CAknForm , QuerySaveChangesL
     * Show save query. If user answers "No" to this query.
     * return field value to the value which is before editing.
     */
    //TBool QuerySaveChangesL();

    /**
     * From CAknForm , SaveFormDataL
     * Save the contents of the form.
     */
    TBool SaveFormDataL();

    /**
     * From CAknForm, DoNotSaveFormDataL
     * Does not save the contents of the form.
     */
    void DoNotSaveFormDataL();

    void LoadFormValuesFromDataL();
    void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);
    void ProcessCommandL(TInt aCommandId);
    void SetInitialCurrentLine();

private:
    /**
     * For holding controls value
     */
    TBuf<100/*EAknExFormEdwinMaxLength*/> iDataName;
    TInt iAddedItemId;

    };

#endif /* VOIPFORM_H_*/
