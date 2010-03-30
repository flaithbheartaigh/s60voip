/*
 ========================================================================
 $Id$
 $Author$
 $Date$
 $Revision$
 Copyright (c) 2010 Fandango60.  
 ========================================================================
 */

#ifndef VOIP_PAN_H
#define VOIP_PAN_H

/** VoIP application panic codes */
enum TVoIPPanics
    {
    EVoIPUi = 1,
    EBirthdays3rdEditionUi,
    EOutOfEntriesArray,
    EUnSupportedSearchType,
    KInvalidEntryIndex,
    KInvalidSyncValue,
    KInvalidModifyIndex,
    EWaitNotStarted,
    EWaitAlreadyStarted,
    EViewError
    // add further panics here
    };

inline void Panic(TVoIPPanics aReason)
    {
    _LIT(applicationName, "VoIP");
    User::Panic(applicationName, aReason);
    }

#endif // VOIP_PAN_H
