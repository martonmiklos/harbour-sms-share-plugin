#include "vcardserializer_plugin.h"
#include "vcardserializer.h"

#include <qqml.h>
#include <QDebug>

void vCardSerializerPlugin::registerTypes(const char *uri)
{
    qWarning() << QString("\"%1\"").arg(uri) << "VCardSerializerPlugin::registerTypes";
    // @uri hu.mm.vCardSerializer
    qmlRegisterType<vCardSerializer>(uri, 1, 0, "vCardSerializer");
}

