#include "vcardserializer.h"

#include "Qt-Quoted-Printable/quotedprintable.h"

#include <QString>
#include <QStringList>

vCardSerializer::vCardSerializer(QQuickItem *parent) :
    QQuickItem(parent)
{

}

QString vCardSerializer::serialize_vCardFull(const QString &vCard)
{
    /**
     * Output strategy:
     * Export shall start with the name
     *
     * Genera export order (priority) similar to the contact edit UI:
     * - Phone number(s)
     * - Email address(es)
     * - Rest (birthday, website, etc.)
     *
     * If company details present those should be serialzed first (likely a company contact?)
     *
     */
    QList <vCardField> fields;
    QString ret;
    QStringList lines = vCard.split("\\r\\n");
    for (const QString & line : lines) {
        if (line.contains(':')) {
            QStringList parts = line.split(':');
            if (parts.count() == 2) {
                fields.append(vCardField(parts.at(0).toUpper(), parts.at(1)));
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
        bool hasRole = false;
        for (vCardField field : fields) {
            if (field.fieldType() == vCardField::Role) {
                hasRole = true;
                // if role and organization is present
                // serialize in as a CTO at ACME Ltd. form
                ret.append(field.serializeFull());
                ret.append(tr(" at "));
                break;
            }
        }

        for (vCardField field : fields) {
            if (field.fieldType() == vCardField::Organization) {
                if (hasRole)
                    ret.append(field.serializeShort().append('\n'));
                else
                    ret.append(field.serializeFull());
                break;
            }
        }

        for (vCardField field : fields) {
            if (field.label() == vCardField::Work && field.fieldType() == vCardField::Phone) {
                ret.append(field.serializeFull());
                break;
            }
        }

        for (vCardField field : fields) {
            if (field.label() == vCardField::Work && field.fieldType() == vCardField::Email) {
                ret.append(field.serializeFull());
                break;
            }
        }
    }

    for (vCardField field : fields) {
        // work related fields were serialized already if the organization was set
        if (field.label() == vCardField::Work)
            continue;

        if (field.fieldType() == vCardField::Phone) {
            ret.append(field.serializeFull());
        }
    }

    if (!hasCompanyDetails) {
        // if work phone was not serialized at the organization serialize it after the other phones
        for (vCardField field : fields) {
            if (field.fieldType() == vCardField::Phone
                    && field.label() == vCardField::Work) {
                ret.append(field.serializeFull());
            }
        }
    }

    for (vCardField field : fields) {
        if (field.label() == vCardField::Work)
            continue;

        if (field.fieldType() == vCardField::Email) {
            ret.append(field.serializeFull());
            break;
        }
    }

    if (!hasCompanyDetails) {
        // if work email was not serialized at the organization serialize
        // it after the other email addresses
        for (vCardField field : fields) {
            if (field.fieldType() == vCardField::Email
                    && field.label() == vCardField::Work) {
                ret.append(field.serializeFull());
            }
        }
    }

    for (vCardField field : fields) {
        if (field.fieldType() == vCardField::Address) {
            ret.append(field.serializeFull());
            break;
        }
    }

    for (vCardField field : fields) {
        if (field.fieldType() == vCardField::Url) {
            ret.append(field.serializeFull());
            break;
        }
    }

    for (vCardField field : fields) {
        if (field.fieldType() == vCardField::BirthDay) {
            ret.append(field.serializeFull());
            break;
        }
    }

    return ret;
}

QString vCardSerializer::serialize_vCardShort(const QString &vCard)
{
    // short SMS: only names and phone numbers in random order
    QList <vCardField> fields;
    QString ret;
    QStringList lines = vCard.split("\\r\\n");
    for (const QString & line : lines) {
        if (line.contains(':')) {
            QStringList parts = line.split(':');
            if (parts.count() == 2) {
                fields.append(vCardField(parts.at(0).toUpper(), parts.at(1)));
            }
        }
    }

    bool hasName = false;
    for (vCardField field : fields) {
        if (field.fieldType() == vCardField::FullName) {
            ret.append(field.serializeShort());
            hasName = true;
            break;
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

    for (vCardField field : fields) {
        if (field.fieldType() == vCardField::Phone) {
            ret.append(field.serializeFull());
        }
    }

    return ret;
}

