/*
 ========================================================================
 $Id$
 $Author$
 $Date$
 $Revision$
 Copyright (c) 2010 Fandango60.  
 ========================================================================
 */

#ifndef VOIPLISTBOXVIEW_H
#define VOIPLISTBOXVIEW_H

#include <aknview.h>

// FORWARD DECLARATIONS
class CVoIPListBox;

/**
 * Avkon view class for VoIPListBoxView. It is register with the view server
 * by the AppUi. It owns the container control.
 * @class	CVoIPListBoxView VoIPListBoxView.h
 */
class CVoIPListBoxView : public CAknView
    {

public:
    // constructors and destructor
    CVoIPListBoxView();
    static CVoIPListBoxView* NewL();
    static CVoIPListBoxView* NewLC();
    virtual ~CVoIPListBoxView();

    // from base class CAknView
    TUid Id() const;
    void HandleCommandL(TInt aCommand);

    CVoIPListBox* CreateContainerL();

protected:
    // from base class CAknView
    void DoActivateL(const TVwsViewId& aPrevViewId, TUid aCustomMessageId,
            const TDesC8& aCustomMessage);
    void DoDeactivate();
    void HandleStatusPaneSizeChange();
    /**
     * Handle menu initialization event.
     * @param aResourceId Resource ID identifying the menu pane to initialise.
     * @param aMenuPane The in-memory representation of the menu pane.
     */
    void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);

private:
    void ConstructL();
    void SetupStatusPaneL();
    void CleanupStatusPane();

    void DoEditL();
    void DoNewL();
    void DoVoiceCall();
    void DoInternetCall();
    void DoSMS();
    void DoMMS();
    void DoDeleteL();
    void DoSIPStart();
    void DoSIPStop();

    void WriteToFileL(const TDesC& aFile, const TDesC8& aDescription,
            const TDesC8& aParty, const TDesC8& aDirection);
    void DesConverter16to8(TDesC16& from, TDes8& to);

private:
    CVoIPListBox* iVoIPListBox;
    
    };

#endif // VOIPLISTBOXVIEW_H
