#include "vcardserializer.h"

#include "Qt-Quoted-Printable/quotedprintable.h"

#include <QString>
#include <QStringList>

vCardSerializer::vCardSerializer(QQuickItem *parent) :
    QQuickItem(parent)
{

}

QString vCardSerializer::serialize_vCard(const QString &vCard)
{
    QList <vCardField> fields;
    QString ret;
    QStringList lines = vCard.split("\\r\\n");
    for (const QString line : lines) {
        if (line.contains(':')) {
            QStringList parts = line.split(':');
            if (parts.count() == 2) {
                fields.append(vCardField(parts.at(0), parts.at(1)));
            }
        }
    }

    bool hasName = false;
    bool hasCompanyDetails = false;
    for (vCardField field : fields) {
        if (field.fieldType() == vCardField::FullName) {
            ret.append(field.serializeShort());
            hasName = true;
        }

        if (field.fieldType() == vCardField::Organization) {
            hasCompanyDetails = true;
        }
    }

    if (!hasName) {
        for (vCardField field : fields) {
            if (field.fieldType() == vCardField::NameParts) {
                ret.append(field.serializeShort());
                break;
            }
        }
    }

    // if the company details filled output company stuff first
    if (hasCompanyDetails) {
        for (vCardField field : fields) {
            if (field.fieldType() == vCardField::Role) {
                ret.append(field.serializeShort());
                ret.append(" ");
                break;
            }
        }

        for (vCardField field : fields) {
            if (field.label() == vCardField::Work && field.fieldType() == vCardField::Organization) {
                ret.append(field.serializeShort());
                break;
            }
        }

        for (vCardField field : fields) {
            if (field.label() == vCardField::Work && field.fieldType() == vCardField::Phone) {
                ret.append(field.serializeShort());
                break;
            }
        }

        for (vCardField field : fields) {
            if (field.label() == vCardField::Work && field.fieldType() == vCardField::Email) {
                ret.append(field.serializeShort());
                break;
            }
        }
    }

    for (vCardField field : fields) {
        if (field.label() == vCardField::Work && hasCompanyDetails)
            continue;

        if (field.fieldType() == vCardField::Phone) {
            ret.append(field.serializeShort());
            break;
        }
    }

    for (vCardField field : fields) {
        if (field.label() == vCardField::Work && hasCompanyDetails)
            continue;

        if (field.fieldType() == vCardField::Email) {
            ret.append(field.serializeShort());
            break;
        }
    }

    for (vCardField field : fields) {
        if (field.fieldType() == vCardField::Address) {
            ret.append(field.serializeShort());
            break;
        }
    }

    for (vCardField field : fields) {
        if (field.fieldType() == vCardField::Url) {
            ret.append(field.serializeShort());
            break;
        }
    }

    for (vCardField field : fields) {
        if (field.fieldType() == vCardField::BirthDay) {
            ret.append(field.serializeShort());
            break;
        }
    }

    return ret;
}

