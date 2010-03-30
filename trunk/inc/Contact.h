/*
 ========================================================================
 $Id$
 $Author$
 $Date$
 $Revision$
 Copyright (c) 2010 Fandango60.  
 ========================================================================
 */

#ifndef CCONTACT_H_
#define CCONTACT_H_

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
#include <PbkFields.hrh>
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

class CContact : public CBase
    {
private:
    TContactItemId iContactItemId;
    HBufC* iFirstName;
    HBufC* iLastName;

public:
    void Name(HBufC*& aFullName, TBool last_name_first) const;
    TContactItemId GetEntryId();
    void SetEntryId(TContactItemId aContactItemId);
    void SetFirstName(const TDesC& aFirstName);
    const HBufC* GetFirstName();
    void SetLastName(const TDesC& aLastName);
    const HBufC* GetLastName();

    CContact()
        {
        iContactItemId = 0;
        iFirstName = 0;
        iLastName = 0;
        }

    static CContact* NewL(TContactItemId contactItemId, const TDesC& aFirstName,
            const TDesC& aLastName);
    ~CContact();
    };

#endif /*CCONTACT_H_*/
