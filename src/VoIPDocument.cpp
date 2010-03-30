/*
 ========================================================================
 $Id$
 $Author$
 $Date$
 $Revision$
 Copyright (c) 2010 Fandango60.  
 ========================================================================
 */

#include "VoIPApplication.h"
#include "VoIPDocument.h"
#include "VoIPAppUi.h"
#include "PhoneBkEngine.h"
#include "Symbian_ua_guiSettingItemListSettings.h"

// CONSTANTS
const TInt KMaxInfoLength = 1024;
const TInt KMaxPhoneNumberLength = 40;
_LIT(KtxDicFileName ,"\\private\\20025455\\VoIP.ini" );
_LIT(KBlank,"");

const TInt KRegistrar = 2;
const TInt KUsername = 3;
const TInt KPassword = 4;
const TInt KStunServer = 5;
const TInt KSrtp = 6;
const TInt KIce = 7;

/**
 * @brief Constructs the document class for the application.
 * @param anApplication the application instance
 */
CVoIPDocument::CVoIPDocument(CEikApplication& anApplication) :
    CAknDocument(anApplication)
    {
    }

/**
 * @brief Destructs the document class for the application.
 */
CVoIPDocument::~CVoIPDocument()
    {
    }

/**
 * @brief Completes the second phase of Symbian object construction. 
 * Put initialization code that could leave here.  
 */
void CVoIPDocument::ConstructL()
    {
    iInfoText = HBufC::NewL(KMaxInfoLength);
    iPhoneNumber = HBufC::NewL(KMaxPhoneNumberLength);
    StorePhoneNumber(KBlank);
    }

CVoIPDocument* CVoIPDocument::NewL(CEikApplication& aApp)
    {
    CVoIPDocument* self = new (ELeave) CVoIPDocument(aApp);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

/**
 * @brief Creates the application UI object for this document.
 * @return the new instance
 */
CEikAppUi* CVoIPDocument::CreateAppUiL()
    {
    return new (ELeave) CVoIPAppUi;
    }

void CVoIPDocument::SetModifyIndex(const TInt& aIndex)
    {
    iModifyIndex = aIndex;
    }

TInt CVoIPDocument::ModifyIndex() const
    {
    return iModifyIndex;
    }
 
void CVoIPDocument::StoreInfoTextL(const TDesC& aText)
    {
    __ASSERT_ALWAYS((aText.Length() + 1) < KMaxInfoLength, User::Leave(
                    KErrArgument));

    TPtr ptext(iInfoText->Des());

    TInt appendLen = aText.Length() + 1; // plus one is for line break 

    if ((appendLen + iInfoText->Length()) > KMaxInfoLength)
        {
        ptext.Delete(0, appendLen);
        }

    ptext.Append(aText);
    ptext.Append(CEditableText::ELineBreak);
    }
    
const TDesC& CVoIPDocument::InfoText()
    {
    return *iInfoText;
    }

void CVoIPDocument::LoadSettingValuesL(
        TSymbian_ua_guiSettingItemListSettings& aSettings)
    {
    // load values into iSettings
    TFindFile AufFolder(CCoeEnv::Static()->FsSession());
    if (KErrNone == AufFolder.FindByDir(KtxDicFileName, KNullDesC))
        {
        CDictionaryFileStore* MyDStore = CDictionaryFileStore::OpenLC(
                CCoeEnv::Static()->FsSession(), AufFolder.File(),
                KUidVoIPApplication);
        TUid FileUid;

        FileUid.iUid = KRegistrar;
        if (MyDStore->IsPresentL(FileUid))
            {
            RDictionaryReadStream in;
            in.OpenLC(*MyDStore, FileUid);
            in >> aSettings.Ed_registrar();
            CleanupStack::PopAndDestroy(1);
            }

        FileUid.iUid = KUsername;
        if (MyDStore->IsPresentL(FileUid))
            {
            RDictionaryReadStream in;
            in.OpenLC(*MyDStore, FileUid);
            in >> aSettings.Ed_user();
            CleanupStack::PopAndDestroy(1);
            }

        FileUid.iUid = KPassword;
        if (MyDStore->IsPresentL(FileUid))
            {
            RDictionaryReadStream in;
            in.OpenLC(*MyDStore, FileUid);
            in >> aSettings.Ed_password();
            CleanupStack::PopAndDestroy(1);
            }

        FileUid.iUid = KStunServer;
        if (MyDStore->IsPresentL(FileUid))
            {
            RDictionaryReadStream in;
            in.OpenLC(*MyDStore, FileUid);
            in >> aSettings.Ed_stun_server();
            CleanupStack::PopAndDestroy(1);
            }

        FileUid.iUid = KSrtp;
        if (MyDStore->IsPresentL(FileUid))
            {
            RDictionaryReadStream in;
            in.OpenLC(*MyDStore, FileUid);
            aSettings.SetB_srtp((TBool) in.ReadInt32L());
            CleanupStack::PopAndDestroy(1);
            }

        FileUid.iUid = KIce;
        if (MyDStore->IsPresentL(FileUid))
            {
            RDictionaryReadStream in;
            in.OpenLC(*MyDStore, FileUid);
            aSettings.SetB_ice((TBool) in.ReadInt32L());
            CleanupStack::PopAndDestroy(1);
            }

        CleanupStack::PopAndDestroy(1);	
        }

    }

void CVoIPDocument::SaveSettingValuesL(
        TSymbian_ua_guiSettingItemListSettings& aSettings)
    {
    // store values from iSettings
    TFindFile AufFolder(CCoeEnv::Static()->FsSession());
    if (KErrNone == AufFolder.FindByDir(KtxDicFileName, KNullDesC))
        {
        User::LeaveIfError(CCoeEnv::Static()->FsSession().Delete(
                AufFolder.File()));

        CDictionaryFileStore* MyDStore = CDictionaryFileStore::OpenLC(
                CCoeEnv::Static()->FsSession(), AufFolder.File(),
                KUidVoIPApplication);

        TUid FileUid =
            {
            0x0
            };

        FileUid.iUid = KRegistrar;
        RDictionaryWriteStream out1;
        out1.AssignLC(*MyDStore, FileUid);
        out1 << aSettings.Ed_registrar();
        out1.CommitL();
        CleanupStack::PopAndDestroy(1);	

        FileUid.iUid = KUsername;
        RDictionaryWriteStream out2;
        out2.AssignLC(*MyDStore, FileUid);
        out2 << aSettings.Ed_user();
        out2.CommitL();
        CleanupStack::PopAndDestroy(1);	

        FileUid.iUid = KPassword;
        RDictionaryWriteStream out3;
        out3.AssignLC(*MyDStore, FileUid);
        out3 << aSettings.Ed_password();
        out3.CommitL();
        CleanupStack::PopAndDestroy(1);	

        FileUid.iUid = KStunServer;
        RDictionaryWriteStream out4;
        out4.AssignLC(*MyDStore, FileUid);
        out4 << aSettings.Ed_stun_server();
        out4.CommitL();
        CleanupStack::PopAndDestroy(1);	

        FileUid.iUid = KSrtp;
        RDictionaryWriteStream out5;
        out5.AssignLC(*MyDStore, FileUid);
        out5.WriteInt32L(aSettings.B_srtp());
        out5.CommitL();
        CleanupStack::PopAndDestroy(1);

        FileUid.iUid = KIce;
        RDictionaryWriteStream out6;
        out6.AssignLC(*MyDStore, FileUid);
        out6.WriteInt32L(aSettings.B_ice());
        out6.CommitL();
        CleanupStack::PopAndDestroy(1);

        MyDStore->CommitL();
        CleanupStack::PopAndDestroy(1);
        }

    }

const TDesC& CVoIPDocument::GetPhoneNumber()
    {
    return *iPhoneNumber;
    }

void CVoIPDocument::StorePhoneNumber(const TDesC& aPhoneNumber)
    {
    __ASSERT_ALWAYS((aPhoneNumber.Length() + 1) < KMaxPhoneNumberLength, User::Leave(
                    KErrArgument));

    TInt startingPos = aPhoneNumber.Find(_L("+"));
    if (startingPos == KErrNotFound)
        {
        TPtr ptext(iPhoneNumber->Des());
        ptext.Delete(0, KMaxPhoneNumberLength);
        ptext.Append(aPhoneNumber);
        }
    else
        {
        TPtrC subDes(aPhoneNumber.Mid(startingPos + 1));
        TPtr ptext(iPhoneNumber->Des());
        ptext.Delete(0, KMaxPhoneNumberLength);
        ptext.Append(subDes);
        }
    }
