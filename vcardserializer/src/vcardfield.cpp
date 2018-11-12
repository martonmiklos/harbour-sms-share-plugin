#include "vcardfield.h"

#include "Qt-Quoted-Printable/quotedprintable.h"

#include <QString>
#include <QStringList>

vCardField::vCardField(const QString &key, const QString value) :
    m_key(key),
    m_value(value)
{
    QStringList keyParts = key.split(';');

    if (keyParts.contains("ENCODING=QUOTED-PRINTABLE"))
        m_value = QuotedPrintable::decode(m_value.toLocal8Bit());

    if (keyParts.contains("HOME"))
        m_label = Home;
    else if (keyParts.contains("WORK"))
        m_label = Work;
    else if (keyParts.contains("OTHER"))
        m_label = Other;

    if (keyParts.first() == QStringLiteral("FN")) {
        m_fieldType = FullName;
    } else if (keyParts.first() == QStringLiteral("N")) {
        m_fieldType = NameParts;
        QStringList nameParts = m_value.split(';');
        m_value = nameParts.join(' ');
    } else if (keyParts.first() == "TEL") {
        m_fieldType = Phone;
    } else if (keyParts.first() == "EMAIL") {
        m_fieldType = Email;
    } else if (keyParts.first() == "ROLE") {
        m_fieldType = Role;
    } else if (keyParts.first() == "ORG") {
        m_fieldType = Organization;
        QStringList nameParts = m_value.split(';');
        m_value = nameParts.join(' ');
    } else if (keyParts.first() == "ADR") {
        m_fieldType = Address;
        QStringList nameParts = m_value.split(';');
        m_value = nameParts.join(' ');
        // TODO add ordering support
    } else if (keyParts.first() == "URL") {
        m_fieldType = Url;
    } else if (keyParts.first() == "BDAY") {
        m_fieldType = BirthDay;
    } else {
        m_fieldType = UnknownFieldType;
    }


}

QString vCardField::serializeFull()
{
    switch (m_fieldType) {
    case vCardField::FullName:
    case vCardField::NameParts:
        return QObject::tr("Name: %1\n").arg(m_value);
        break;
    case vCardField::Phone: {
        QString phoneTypeString;
        switch (m_phoneType) {
        case vCardField::Assistant:
            phoneTypeString = QObject::tr("Assistant");
            break;
        case vCardField::Fax:
            phoneTypeString = QObject::tr("Fax");
            break;
        case vCardField::Pager:
            phoneTypeString = QObject::tr("Pager");
            break;
        case vCardField::Video:
            phoneTypeString = QObject::tr("Video");
            break;
        case vCardField::Mobile:
            phoneTypeString = QObject::tr("Mobile");
            break;
        default:
            break;
        }

        switch (m_label) {
        case vCardField::Home:
            phoneTypeString = QObject::tr("Home %1").arg(phoneTypeString.toLower());
            break;
        case vCardField::Work:
            phoneTypeString = QObject::tr("Work %1").arg(phoneTypeString.toLower());
            break;
        case vCardField::Other:
            phoneTypeString = QObject::tr("Other %1").arg(phoneTypeString.toLower());
            break;
        case UnknownLabel:
        default:
            break;
        }
        if (phoneTypeString.isEmpty())
            return m_value;
        return QString("%1: %2\n").arg(phoneTypeString).arg(m_value);
        break;
    }

    case vCardField::Email:
        switch (m_label) {
        case vCardField::Home:
            return QObject::tr("Home email: %1\n").arg(m_value);
            break;
        case vCardField::Work:
            return QObject::tr("Work email: %1\n").arg(m_value);
            break;
        case vCardField::Other:
            return QObject::tr("Other email: %1\n").arg(m_value);
            break;
        case UnknownLabel:
        default:
            break;
        }
        return QObject::tr("Email: %1\n").arg(m_value);
        break;
    case vCardField::Organization:
        return QObject::tr("Company: %1\n").arg(m_value);
        break;
    case vCardField::Address:
        return QObject::tr("Address: %1\n").arg(m_value);
        break;
    case vCardField::Url:
        return QObject::tr("Website: %1\n").arg(m_value);
        break;
    case vCardField::BirthDay:
        return QObject::tr("Birthday: %1\n").arg(m_value);
        break;
    case vCardField::Role:
        return m_value;
    case vCardField::UnknownFieldType:
    default:
        return QString("");
        break;
    }
}

QString vCardField::serializeShort()
{
    return serializeFull();
    switch (m_fieldType) {
    case vCardField::FullName:
        return m_value;
        break;
    case vCardField::NameParts:
        return m_value;
        break;
    case vCardField::Phone:
        return m_value;
        break;
    case vCardField::Email:
        return m_value;
        break;
    case vCardField::Organization:
        return m_value;
        break;
    case vCardField::Address:
        return m_value;
        break;
    case vCardField::Url:
        return m_value;
        break;
    case vCardField::BirthDay:
        return m_value;
        break;
    case vCardField::UnknownFieldType:
    default:
        return QString("");
        break;
    }
}

vCardField::Label vCardField::label() const
{
    return m_label;
}

vCardField::FieldType vCardField::fieldType() const
{
    return m_fieldType;
}
