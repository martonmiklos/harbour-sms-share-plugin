#include "vcardserializer.h"

#include "Qt-Quoted-Printable/quotedprintable.h"

#include <QLocale>
#include <QString>
#include <QStringList>
#include <QTranslator>

vCardSerializer::vCardSerializer(QQuickItem *parent) :
    QQuickItem(parent)
{
    QTranslator translator;
    const QString filename("sailfish_components_contacts_qt5");
    const QString directory("/usr/share/translations");
    const QString prefix("-");
    const QString suffix(".qm");
    if (translator.load(QLocale(), filename, prefix, directory, suffix)
            || translator.load(QLocale("en"), filename, prefix, directory, suffix))
        QCoreApplication::installTranslator(&translator);
}

QString vCardSerializer::serialize_vCardFull()
{
    QStringList lines;
    QString ret;
    /**
     * Output strategy:
     * Export shall start with the name
     *
     * General export order (priority) similar to the contact edit UI:
     * - Phone number(s)
     * - Email address(es)
     * - Rest (birthday, website, etc.)
     *
     * If company details present those should be serialzed first (likely a company contact?)
     *
     */
    for (const auto & field : m_fields) {
        if (field.fieldType() == vCardField::FullName) {
            ret.append(field.serializeShort() + "\n");
            break;
        }
    }

    if (!m_hasFullName) {
        for (const auto & field : m_fields) {
            if (field.fieldType() == vCardField::NameParts) {
                ret.append(field.serializeShort() + "\n");
                break;
            }
        }
    }

    // if the company details filled output company stuff first
    if (m_hasOrganisation) {
        bool hasRole = false;
        for (const auto & field : m_fields) {
            if (field.fieldType() == vCardField::Role) {
                hasRole = true;
                // if role and organization is present
                // serialize in as a CTO at ACME Ltd. form
                ret.append(field.serializeFull());
                ret.append(" - ");
                break;
            }
        }

        for (const auto & field : m_fields) {
            if (field.fieldType() == vCardField::Organization) {
                if (hasRole)
                    ret.append(field.serializeShort() + "\n");
                else
                    ret.append(field.serializeFull());
                break;
            }
        }

        for (const auto & field : m_fields) {
            if (field.label() == vCardField::Work && field.fieldType() == vCardField::Phone) {
                ret.append(field.serializeFull());
                break;
            }
        }

        for (const auto & field : m_fields) {
            if (field.label() == vCardField::Work && field.fieldType() == vCardField::Email) {
                ret.append(field.serializeFull());
                break;
            }
        }
    }

    for (const auto & field : m_fields) {
        // work related fields were serialized already if the organization was set
        if (field.label() == vCardField::Work)
            continue;

        if (field.fieldType() == vCardField::Phone) {
            const auto line = field.serializeFull();
            if (!lines.contains(line)) {
                ret.append(line);
                lines.append(line);
            }
        }
    }

    if (!m_hasOrganisation) {
        // if work phone was not serialized at the organization serialize it after the other phones
        for (const auto & field : m_fields) {
            if (field.fieldType() == vCardField::Phone
                    && field.label() == vCardField::Work) {
                const auto line = field.serializeFull();
                if (!lines.contains(line)) {
                    ret.append(line);
                    lines.append(line);
                }
            }
        }
    }

    for (const auto & field : m_fields) {
        if (field.label() == vCardField::Work)
            continue;

        if (field.fieldType() == vCardField::Email) {
            const auto line = field.serializeFull();
            if (!lines.contains(line)) {
                ret.append(line);
                lines.append(line);
            }
        }
    }

    if (!m_hasOrganisation) {
        // if work email was not serialized at the organization serialize
        // it after the other email addresses
        for (const auto & field : m_fields) {
            if (field.fieldType() == vCardField::Email
                    && field.label() == vCardField::Work) {
                const auto line = field.serializeFull();
                if (!lines.contains(line)) {
                    ret.append(line);
                    lines.append(line);
                }
            }
        }
    }

    for (const auto & field : m_fields) {
        if (field.fieldType() == vCardField::Address) {
            const auto line = field.serializeFull();
            if (!lines.contains(line)) {
                ret.append(line);
                lines.append(line);
            }
        }
    }

    for (const auto & field : m_fields) {
        if (field.fieldType() == vCardField::Url) {
            const auto line = field.serializeFull();
            if (!lines.contains(line)) {
                ret.append(line);
                lines.append(line);
            }
        }
    }

    for (const auto & field : m_fields) {
        if (field.fieldType() == vCardField::BirthDay) {
            const auto line = field.serializeFull();
            if (!lines.contains(line)) {
                ret.append(line);
                lines.append(line);
            }
        }
    }

    return ret.trimmed();
}

QString vCardSerializer::serialize_vCardShort()
{
    QString ret;
    // short serialization strategy:
    // only the name and phone numbers in
    // the same order as they present in the vCard
    for (const auto & field : m_fields) {
        if (field.fieldType() == vCardField::FullName) {
            ret.append(field.serializeShort() + "\n");
            break;
        }
    }

    if (!m_hasFullName) {
        for (const auto & field : m_fields) {
            if (field.fieldType() == vCardField::NameParts) {
                ret.append(field.serializeShort() + "\n");
                break;
            }
        }
    }

    for (const auto & field : m_fields) {
        if (field.fieldType() == vCardField::Phone) {
            // if having a single phone number just serialize that
            // do not serialize the type
            if (m_phoneCount == 1)
                ret.append(field.serializeShort() + "\n");
            else
                ret.append(field.serializeFull());
        }
    }

    return ret.trimmed();
}

void vCardSerializer::setVCardData(const QString &vCardData)
{
    QStringList lines = vCardData.split("\\r\\n");
    m_fields.clear();
    m_hasFullName = false;
    m_hasOrganisation = true;
    m_phoneCount = 0;
    for (const QString & line : lines) {
        if (line.contains(':')) {
            QStringList parts = line.split(':');
            if (parts.count() == 2) {
                vCardField field = vCardField(parts.at(0).toUpper(), parts.at(1));
                if (field.fieldType() == vCardField::FullName)
                    m_hasFullName = true;
                else if (field.fieldType() == vCardField::Organization)
                    m_hasOrganisation = true;
                else if (field.fieldType() == vCardField::Phone) {
                    m_phoneCount++;
                    // some phone numbers appears twice
                    // one without type one with
                    // remove the typeless
                    int fieldI = 0;
                    for (const vCardField & existingField : m_fields) {
                        if (existingField.fieldType() == vCardField::Phone
                                && existingField.serializeShort() == field.serializeShort()) {
                            // we have two identical phone numbers
                            if ((existingField.label() == vCardField::UnknownLabel
                                 && existingField.phoneType() == vCardField::UnknownPhoneType)
                                    || field == existingField) {
                                // no additional info from the first one -> remove it
                                // the current one should have more or the same add. info
                                m_fields.removeAt(fieldI);
                                m_phoneCount--;
                            }
                        }
                        fieldI++;
                    }
                }
                m_fields.append(field);
            }
        }
    }
}

QString vCardSerializer::vCardData() const
{
    return QString();
}

