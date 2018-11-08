#ifndef VCARDSERIALIZER_H
#define VCARDSERIALIZER_H

#include <QObject>

class vCardSerializer : public QObject
{
    Q_OBJECT
public:
    explicit vCardSerializer(QObject *parent = nullptr);
    Q_INVOKABLE QString serialize_vCard(const QString & vCARDinJSON) const;
signals:

public slots:
};

#endif // VCARDSERIALIZER_H
