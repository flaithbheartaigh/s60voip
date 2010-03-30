/*
 ========================================================================
 $Id$
 $Author$
 $Date$
 $Revision$
 Copyright (c) 2010 Fandango60.  
 ========================================================================
 */

#ifndef VOIPAPPLICATION_H
#define VOIPAPPLICATION_H

#include <aknapp.h>
#include "VoIP.hrh"

// UID for the application - this should correspond to the uid defined in the mmp file
const TUid KUidVoIPApplication =
    {_UID3};

/**
 *
 * @class	CVoIPApplication VoIPApplication.h
 * @brief	A CAknApplication-derived class is required by the S60 application 
 *          framework. It is subclassed to create the application's document 
 *          object.
 */
class CVoIPApplication : public CAknApplication
    {
private:
    TUid AppDllUid() const;
    CApaDocument* CreateDocumentL();

    };

#endif // VOIPAPPLICATION_H
