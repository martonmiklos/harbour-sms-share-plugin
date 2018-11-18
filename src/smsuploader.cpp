#include "smsuploader.h"

#include <QString>
#include <TransferEngine-qt5/mediaitem.h>

SMSUploader::SMSUploader(QObject *parent):
    MediaTransferInterface(parent)
{

}

SMSUploader::~SMSUploader()
{
}

QString SMSUploader::displayName() const
{
    //% "SMS"
    return qtTrId("harbour-sms-share-plugin-id");
}

QUrl SMSUploader::serviceIcon() const
{
    // Url to the icon which should be shown in the transfer UI
    return QUrl("/usr/share/nemo-transferengine/plugins/harbour-sms-share-plugin/icon.svg");
}

bool SMSUploader::cancelEnabled() const
{
    // Return true if cancelling ongoing upload is supported
    // Return false if cancelling ongoing upload is not supported
    return false;
}

bool SMSUploader::restartEnabled() const
{
    // Return true, if restart is  supported.
    // Return false, if restart is not supported
    return false;
}


void SMSUploader::start()
{
    // This is called by the sharing framework to start sharing

    // TODO: Add your code here to start uploading
}

void SMSUploader::cancel()
{
    // This is called by the sharing framework to cancel on going transfer

    // TODO: Add your code here to cancel ongoing upload
}

