#ifndef VCARDSERIALIZER_H
#define VCARDSERIALIZER_H

#include <QQuickItem>

class vCardSerializer : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(vCardSerializer)

public:
    vCardSerializer(QQuickItem *parent = nullptr);
    ~vCardSerializer();

    Q_INVOKABLE QString serialize_vCard(const QString & vCardJSON) const;
};

#endif // VCARDSERIALIZER_H
