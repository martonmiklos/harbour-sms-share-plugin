#include "vcardserializer_plugin.h"
#include "vcardserializer.h"

#include <qqml.h>
#include <QDebug>

void vCardSerializerPlugin::registerTypes(const char *uri)
{
    // @uri hu.mm.VCardSerializer
    // types always must start with a capital letter!
    // Thanks to raven-worx: https://forum.qt.io/topic/91989/qmlregistertype-dont-recogized-into-qml-file/4
    qmlRegisterType<vCardSerializer>(uri, 1, 0, "VCardSerializer");
}

