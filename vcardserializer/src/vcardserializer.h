#ifndef VCARDSERIALIZER_H
#define VCARDSERIALIZER_H

#include "vcardfield.h"

#include <QQuickItem>

class vCardSerializer : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(vCardSerializer)

public:
    vCardSerializer(QQuickItem *parent = nullptr);
    Q_INVOKABLE static QString serialize_vCardFull(const QString & vCard);
    Q_INVOKABLE static QString serialize_vCardShort(const QString & vCard);
};

#endif // VCARDSERIALIZER_H
