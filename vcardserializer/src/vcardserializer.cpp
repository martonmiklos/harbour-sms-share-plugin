#include "vcardserializer.h"

#include <QDebug>

vCardSerializer::vCardSerializer(QQuickItem *parent):
    QQuickItem(parent)
{
    qWarning() << "vCardSerializer::vCardSerializer(QObject *parent)";
}

vCardSerializer::~vCardSerializer()
{
    qWarning() << "vCardSerializer::~vCardSerializer()";
}

QString vCardSerializer::serialize_vCard(const QString &vCardJSON) const
{
    return vCardJSON;
}
