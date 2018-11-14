#ifndef VCARDSERIALIZER_H
#define VCARDSERIALIZER_H

#include "vcardfield.h"

#include <QQuickItem>

class vCardSerializer : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(vCardSerializer)
    Q_PROPERTY(QString vCardData READ vCardData WRITE setVCardData)
public:
    vCardSerializer(QQuickItem *parent = nullptr);

    Q_INVOKABLE QString serialize_vCardFull();
    Q_INVOKABLE QString serialize_vCardShort();
    Q_INVOKABLE void setVCardData(const QString &vCardData);
    // dummy function just to make the propery valid
    QString vCardData() const;

private:
    QList <vCardField> m_fields;
    bool m_hasFullName = false;
    bool m_hasOrganisation = false;
    int m_phoneCount = 0;
};

#endif // VCARDSERIALIZER_H
