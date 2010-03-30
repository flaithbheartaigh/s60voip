/*
* ==============================================================================
*  Name        : SIPExLogView.h
*  Part of     : SIPEx
*  Interface   : 
*  Description : 
*  Version     : 
*
*  Copyright (c) 2004-2006 Nokia Corporation.
*  This material, including documentation and any related 
*  computer programs, is protected by copyright controlled by 
*  Nokia Corporation.
* ==============================================================================
*/

#ifndef __SIPEXLOGVIEW_H__
#define __SIPEXLOGVIEW_H__

// INCLUDES
#include <aknview.h>

#include "SIPEx.hrh"
#include "VoIP.hrh" // modification: $Author$

// CONSTANTS
// UID of view
const TUid KLogViewId = { ESIPExView2Id };

// FORWARD DECLARATIONS
//class CSIPExEngine; 
class CSIPExLogContainer;

// CLASS DECLARATION

/**
*  CSIPExLogView view class.
* 
*/
class CSIPExLogView : public CAknView
    {
    public: // Constructors and destructor

        /**
        * First phase contruction
        */
    	static CSIPExLogView* NewL(); // modification: $Author$
        static CSIPExLogView* NewLC( /*CSIPExEngine& aEngine*/ );
  
        /**
        * Destructor.
        */
        ~CSIPExLogView();
        
    private:
    
        /**
        * C++ default constructor
        * @param aEngine The reference to the application engine.
        */
        CSIPExLogView( /*CSIPExEngine& aEngine*/ );
        
        /**
        * EPOC default constructor.
        */
        void ConstructL();

    public: // Functions from base classes
        
        /**
        * Return Uid
        */
        TUid Id() const;

        /**
        * Handle Commands
        */
        void HandleCommandL( TInt aCommand );
        TBool CallSoftKeyPressedL(TInt aCommand); // modification: $Author$

        /**
        * Handle size change
        */
        void HandleViewRectChange();
        
        /**
        * Handle menu initialization event.
        * @param aResourceId Resource ID identifying the menu pane to initialise.
        * @param aMenuPane The in-memory representation of the menu pane.
        */
        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane ); 

    private:

        /**
        * From AknView, activates view
        */
        void DoActivateL( const TVwsViewId& aPrevViewId,TUid aCustomMessageId,
            const TDesC8& aCustomMessage );

        /**
        * From AknView, deactivates view
        */
        void DoDeactivate();
        
    public:
        
        /**
        * Return pointer to container, ownership is not transferred
        */
        CSIPExLogContainer* Container();

    private: // Data
        CSIPExLogContainer* iContainer;
        
        //CSIPExEngine& iEngine;
    
    /* modification: $Author$ */   
    public: 
    	static void RunNote_errorL( const TDesC* aOverrideText = NULL );
    	static void RunNote_infoL( const TDesC* aOverrideText = NULL );
    	static void RunNote_warningL( const TDesC* aOverrideText = NULL );
    	static TInt RunQry_accept_callL( const TDesC* aOverrideText = NULL );
        void PutMessage(const TDesC &msg); 
        
    };

#endif // __SIPEXLOGVIEW_H__

// End of File
