#include "vcardfield.h"

#include "Qt-Quoted-Printable/quotedprintable.h"

#include <qglobal.h>
#include <QString>
#include <QStringList>

vCardField::vCardField(const QString &key, const QString & value) :
    m_key(key),
    m_value(value)
{
    QStringList keyParts = key.split(';');

    if (keyParts.contains(QStringLiteral("ENCODING=QUOTED-PRINTABLE")))
        m_value = QuotedPrintable::decode(m_value.toLocal8Bit());

    if (keyParts.contains(QStringLiteral("HOME")))
        m_label = Home;
    else if (keyParts.contains(QStringLiteral("WORK")))
        m_label = Work;
    else if (keyParts.contains(QStringLiteral("OTHER")))
        m_label = Other;

    if (keyParts.first() == QStringLiteral("FN")) {
        m_fieldType = FullName;
    } else if (keyParts.first() == QStringLiteral("N")) {
        m_fieldType = NameParts;
        QStringList nameParts = m_value.split(';');
        m_value = nameParts.join(' ');
    } else if (keyParts.first() == "TEL") {
        m_fieldType = Phone;
        if (keyParts.contains("CELL"))
            m_phoneType = Mobile;
        else if (keyParts.contains("VIDEO"))
            m_phoneType = Video;
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
        QString displayFormat = qtTrId("contacts-la-address_display_format");
        // key format:
        // 0     1             2   3    4      5          6
        // Pobox;Extended addr;Str;City;Region;Postalcode;Country
        QStringList vCardValueParts = m_value.split(';');
        QStringList fieldsToReplace;
        fieldsToReplace
                << "pobox"
                << "extaddr"
                << "street"
                << "city"
                << "zipcode"
                << "region"
                << "country";
        int i = 0;
        for (const QString & currentField : fieldsToReplace) {
            int placeholderStart = displayFormat.indexOf(currentField);
            if (placeholderStart == -1)
                continue;

            if (vCardValueParts.count() <= i
                    || vCardValueParts.at(i).isEmpty()
                    || vCardValueParts.at(i) == QStringLiteral("=")) { // HACK for malformed? vCard fields
                int placeHolderEnd = placeholderStart + 1;
                while (placeHolderEnd < displayFormat.length()) {
                    if (displayFormat.at(placeHolderEnd) == '<')
                        break;
                    placeHolderEnd++;
                }
                displayFormat = displayFormat.remove(placeholderStart, placeHolderEnd - placeholderStart);
            } else {
                displayFormat = displayFormat.replace(currentField, vCardValueParts.at(i));
            }
            i++;
        }

        int firstOpen = displayFormat.indexOf('<');
        displayFormat = displayFormat.mid(firstOpen, displayFormat.lastIndexOf('>') - firstOpen);
        displayFormat = displayFormat.remove('<').remove('>');
        m_value = displayFormat;
    } else if (keyParts.first() == "URL") {
        m_fieldType = Url;
    } else if (keyParts.first() == "BDAY") {
        m_fieldType = BirthDay;
    } else {
        m_fieldType = UnknownFieldType;
    }


}

bool vCardField::operator ==(const vCardField &other)
{
    return m_value == other.serializeShort()
            && m_label == other.label()
            && m_phoneType == other.phoneType()
            && m_fieldType == other.fieldType();
}

bool vCardField::operator !=(const vCardField &other)
{
    return !(m_value == other.serializeShort()
            && m_label == other.label()
            && m_phoneType == other.phoneType()
            && m_fieldType == other.fieldType());
}

QString vCardField::serializeFull() const
{
    switch (m_fieldType) {
    case vCardField::FullName:
    case vCardField::NameParts:
        return m_value;
        break;
    case vCardField::Phone: {
        QString phoneTypeString;
        switch (m_phoneType) {
        case vCardField::Assistant:
            phoneTypeString = qtTrId("contacts-la-detail_type_phone_assistant");
            break;
        case vCardField::Fax:
            phoneTypeString = qtTrId("contacts-la-detail_type_phone_fax");
            break;
        case vCardField::Pager:
            phoneTypeString = qtTrId("contacts-la-detail_type_phone_pager");
            break;
        case vCardField::Video:
            phoneTypeString = qtTrId("contacts-la-detail_type_phone_video");
            break;
        case vCardField::Mobile:
            phoneTypeString = qtTrId("contacts-la-detail_type_phone_mobile");
            break;
        default:
            break;
        }

        if (m_label != UnknownLabel && !phoneTypeString.isEmpty()) {
            phoneTypeString = phoneTypeString.prepend(" ").toLower();
        }

        switch (m_label) {
        case vCardField::Home:
            phoneTypeString = QString("%1%2")
                    .arg(qtTrId("contacts-la-short_detail_label_personal"), phoneTypeString);
            break;
        case vCardField::Work:
            phoneTypeString = QString("%1%2")
                    .arg(qtTrId("contacts-la-short_detail_label_work"), phoneTypeString);
            break;
        case vCardField::Other:
            phoneTypeString = QString("%1%2")
                    .arg(qtTrId("contacts-la-short_detail_label_other"), phoneTypeString);
            break;
        case UnknownLabel:
        default:
            break;
        }

        if (phoneTypeString.isEmpty())
            return m_value;
        return QString("%1: %2\n").arg(phoneTypeString, m_value);
        break;
    }

    case vCardField::Email:
        switch (m_label) {
        case vCardField::Home:
            return QString("%1 %2: %3\n")
                    .arg(qtTrId("contacts-la-short_detail_label_personal"),
                         qtTrId("contacts-la-detail_type_email").toLower(),
                         m_value);
            break;
        case vCardField::Work:
            return QString("%1 %2: %3\n")
                    .arg(qtTrId("contacts-la-short_detail_label_work"),
                         qtTrId("contacts-la-detail_type_email").toLower(),
                         m_value);
            break;
        case vCardField::Other:
            return QString("%1 %2: %3\n")
                    .arg(qtTrId("contacts-la-short_detail_label_other"),
                         qtTrId("contacts-la-detail_type_email").toLower(),
                         m_value);
            break;
        case UnknownLabel:
        default:
            break;
        }
        return QString("%1: %2\n")
                .arg(qtTrId("contacts-la-detail_type_emai"), m_value);
        break;
    case vCardField::Organization:
        return QString("%1: %2\n")
                .arg(qtTrId("contacts-la-detail_type-company"), m_value);
        break;
    case vCardField::Address:
        return QString("%1: %2\n")
                .arg(qtTrId("contacts-la-detail_type_address"), m_value);
        break;
    case vCardField::Url:
        return QString("%1: %2\n")
                .arg(qtTrId("contacts-la-detail_type_website_homepage"), m_value);
        break;
    case vCardField::BirthDay:
        return QString("%1: %2\n")
                .arg(qtTrId("contacts-la-detail_type_birthday"), m_value);
        break;
    case vCardField::Role:
        return m_value;
    case vCardField::UnknownFieldType:
    default:
        return QString("");
        break;
    }
}

QString vCardField::serializeShort() const
{
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

vCardField::PhoneType vCardField::phoneType() const
{
    return m_phoneType;
}
