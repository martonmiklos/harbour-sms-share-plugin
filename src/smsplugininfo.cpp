#include <QDebug>
#include <QLocale>
#include "smsplugininfo.h"

SMSPluginInfo::SMSPluginInfo()
    : m_ready(false)
{
    TransferMethodInfo info;
    QStringList capabilities;

    // Load translations
    QTranslator* tr = new QTranslator(this);
    const QString filename(SMSSHARE_TRANSLATIONS_FILE);
    const QString directory(SMSSHARE_TRANSLATIONS_DIR);
    const QString prefix("-");
    const QString suffix(".qm");
    if (tr->load(QLocale(), filename, prefix, directory, suffix) ||
        tr->load(QLocale("en"), filename, prefix, directory, suffix)) {
        qApp->installTranslator(tr);
    } else {
        qWarning() << "Failed to load SMS Share translations from "
            SMSSHARE_TRANSLATIONS_DIR "/" SMSSHARE_TRANSLATIONS_FILE "*.qm";
        delete tr;
    }

    // Capabilites ie. what mimetypes this plugin supports
    capabilities << QLatin1String("text/vcard")  // contacts
                 << QLatin1String("text/x-url")  // bookmarked Urls, links
                 << QLatin1String("text/plain"); // notes for e.g.

    //: Display name for SMS share plugin, just translate as SMS
    //% "SMS"
    info.displayName     = qtTrId("harbour-sms-share-plugin-id");

    // Method ID is a unique identifier for this plugin. It is used to identify which share plugin should be
    // used for starting the sharing.
    info.methodId        = QLatin1String("harbour-sms-share-plugin");

    // Path to the Sharing UI which this plugin provides.
    info.shareUIPath     = SMSSHARE_UI_PATH;

    // Pass information about capabilities. This info is used for filtering share plugins
    // which don't support defined types. For SMS, this plugin won't appear in the
    // share method list, if someone tries to share content which isn't vcard or url type,
    info.capabilitities  = capabilities;

    info.accountIcon     = SMSSHARE_ICON_SVG;

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
