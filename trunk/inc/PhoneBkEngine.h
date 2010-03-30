/*
 ========================================================================
 $Id$
 $Author$
 $Date$
 $Revision$
 Copyright (c) 2010 Fandango60.  
 ========================================================================
 */

#ifndef PHONEBKENGINE_H
#define PHONEBKENGINE_H

#include "e32base.h"				// CBase
#include <txtfmlyr.h>
#include <txtrich.h>
#include <smut.h>
#include <sendui.h> 
#include <cntdb.h>
#include <cntfield.h>
#include <etel.h>
#include <cpbkidlefinder.h> 
#include <mpbkcontactdbobserver.h>
#include <mpbkcontactdbobserver.h>
#include <cpbkcontactiter.h> 
#include <PbkFields.hrh>
#include <CPbkFieldsInfo.h>
#include <eiktxlbx.h>
#include <eiklbx.h>
#include <eiktxlbm.h>
#include <aknlists.h>
#include <aknview.h>
#include <aknpopup.h>
#include <aknpopuplayout.h>
#include <aknnotedialog.h>
#include <akniconarray.h>
#include <gulicon.h>

#include <logcli.h>
#include <logview.h>
#include <logwrap.h>

#if !defined(__AKNVIEW_H__)
#include <aknview.h>                        // for CAknView
#endif

#if !defined(__AKNVIEWAPPUI_H__)
#include <aknviewappui.h>                   // for CAknViewAppUi
#endif

#if !defined(__EIKAPP_H__) 
#include <eikapp.h>                         // for CEikApplication
#endif

#if !defined(__EIKDOC_H__)
#include <eikdoc.h>                         // for CEikDocument
#endif

#if !defined(__EIKDIALG_H__)
#include <eikdialg.h>                       // for CEikDialog, CCoeControl
#endif

#include <aknwaitnotewrapper.h>

// Own constants
const TInt KMaxTelephoneNumberLength = 30; // maximum length for a gsm number


//Forward Declaration
class CPbkContactEngine;
class CPbkContactChangeNotifier;
class MObjectProvider;
class CContact;

class CPhoneBkEngine : public CBase,
        public MPbkContactDbObserver,
        public MAknBackgroundProcess
    {
public:
    // constructors and destructor
    static CPhoneBkEngine* NewL();
    static CPhoneBkEngine* NewLC();
    virtual ~CPhoneBkEngine();

public:
    void LoadContactsL();
    void SortContactsL();
    RPointerArray<CContact> GetContactsL();
    TContactItemId CreateDefaultContactItem();
    CPbkContactItem* ReadContactItemLC(TContactItemId iContactId);
    CPbkContactItem* OpenContactItemLCX(TContactItemId iContactId);
    void CommitContactItem(CPbkContactItem* item);
    void DeleteContactItem(TContactItemId iContactId);
    const CPbkFieldsInfo& GetFieldsInfo();
    static TInt compareName(const CContact& aFirst, const CContact& aSecond);

    //from MPbkContactDbObserver
    virtual void HandleDatabaseEventL(TContactDbObserverEvent aEvent);

private:
    // constructors
    CPhoneBkEngine();
    void ConstructL();
    void ResetContacts();

    /** From MAknBackgroundProcess,
     * is called by the active scheduler to dismiss the dialog
     *@param aButtonId specifies button that was pressed
     */
    void DialogDismissedL(TInt aButtonId);

    /** From MAknBackgroundProcess, 
     * is called by the active scheduler to check if is process already done 
     */
    TBool IsProcessDone() const;

    /** From MAknBackgroundProcess
     * is called by the active scheduler to finish the process 
     */
    void ProcessFinished();

    /** From MAknBackgroundProcess,
     * and is called by the active scheduler to do one step of processing
     * Purpose this function in AddressBook to run AddressBookEngine::ConnectL() 
     * if connection is not established yet
     */
    void StepL();

public:
    RPointerArray<CContact> contacts;

private:
    CPbkContactEngine* iPbkContactEngine;
    CPbkContactChangeNotifier* iNotifier;
    CPbkContactIter* iter;
    TBool iReady;
    TInt i;
    };

#endif

// End of File
