#include "smsshareplugin.h"
#include "smsuploader.h"
#include "smsplugininfo.h"
#include <QtPlugin>

SMSSharePlugin::SMSSharePlugin()
{
}

SMSSharePlugin::~SMSSharePlugin()
{
}

MediaTransferInterface * SMSSharePlugin::transferObject()
{
    return new SMSUploader;
}

TransferPluginInfo *SMSSharePlugin::infoObject()
{
    return new SMSPluginInfo;
}

QString SMSSharePlugin::pluginId() const
{
    return "harbour-sms-share-plugin";
}

bool SMSSharePlugin::enabled() const
{
    return true;
}

