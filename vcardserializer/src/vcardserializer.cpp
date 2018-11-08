#include "vcardserializer.h"

#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>

vCardSerializer::vCardSerializer(QObject *parent) : QObject(parent)
{

}

QString vCardSerializer::serialize_vCard(const QString &vCARDinJSON) const
{
    /**
      /usr/share/jolla-contacts/pages/common/common.js
    ***/
    qWarning() << vCARDinJSON;
    return vCARDinJSON;
}
