#include <QDebug>
#include <QLocale>
#include "smsplugininfo.h"

SMSPluginInfo::SMSPluginInfo()
    : m_ready(false)
{
    TransferMethodInfo info;
    QStringList capabilities;

    // Capabilites ie. what mimetypes this plugin supports
    capabilities << QLatin1String("text/vcard");

    // TODO: Translations for 3rd party plugins is not yet supported by Sailfish OS.
    //       Adding support there later, but for now just use what ever non-translated
    //       string here. This string will be visible in the share method list.
    //: Display name for SMS share plugin, just translate as SMS
    //% "SMS"
    info.displayName     = qtTrId("sms-share-la-plugin_name");

    // Method ID is a unique identifier for this plugin. It is used to identify which share plugin should be
    // used for starting the sharing.
    info.methodId        = QLatin1String("harbour-sms-share-plugin");

    // Path to the Sharing UI which this plugin provides.
    info.shareUIPath     = QLatin1String("/usr/share/nemo-transferengine/plugins/SmsShare.qml");

    // Pass information about capabilities. This info is used for filtering share plugins
    // which don't support defined types. For SMS, this plugin won't appear in the
    // share method list, if someone tries to share content which isn't vcard or url type,
    info.capabilitities  = capabilities;

    info.accountIcon     = QStringLiteral("image://theme/icon-m-message");

    m_infoList << info;
}

SMSPluginInfo::~SMSPluginInfo()
{

}

QList<TransferMethodInfo> SMSPluginInfo::info() const
{
    return m_infoList;
}

void SMSPluginInfo::query()
{
    // Let the world know that this plugin is ready
    m_ready = true;
    emit infoReady();
}


bool SMSPluginInfo::ready() const
{
    return m_ready;
}
