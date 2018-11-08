#ifndef PLUGIN_H
#define PLUGIN_H

#include <QQmlExtensionPlugin>

#define VCARDSERIALIZER_PLUGIN_PATH   "hu.mm.vCardSerializer"
#define VCARDSERIALIZER_PLUGIN_URI    QStringLiteral(VCARDSERIALIZER_PLUGIN_PATH)

class Plugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID VCARDSERIALIZER_PLUGIN_PATH)

public:
    void initializeEngine(QQmlEngine *engine, const char *uri);
    void registerTypes(const char *uri);
};

#endif // PLUGIN_H
