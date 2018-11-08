#include "plugin.h"

#include "vcardserializer.h"

#include <QtQml>

void Plugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_ASSERT(uri == VCARDSERIALIZER_PLUGIN_URI);
    qWarning() << "initializeEngine" << uri;
    Q_UNUSED(uri);
    Q_UNUSED(engine);
}

void Plugin::registerTypes(const char *uri)
{
    qWarning() << "register Types" << uri << qmlRegisterType<vCardSerializer>(uri, 1, 0, "vCardSerializer");
}
