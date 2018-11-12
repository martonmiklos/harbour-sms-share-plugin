#ifndef VCARDFIELD_H
#define VCARDFIELD_H

#include <QString>

class vCardField
{
public:
    vCardField(const QString & key, const QString value);

    enum FieldType {
        FullName,
        NameParts,
        Phone,
        Email,
        Organization,
        Role,
        Address,
        Url,
        BirthDay,
        UnknownFieldType
    };

    enum Label {
        Home,
        Work,
        Other,
        UnknownLabel
    };

    enum PhoneType {
        None,
        Assistant,
        Fax,
        Pager,
        Video,
        Mobile,
        UnknownPhoneType
    };

    QString serializeFull();
    QString serializeShort();
    Label label() const;

    FieldType fieldType() const;

private:
    FieldType m_fieldType;
    Label m_label = UnknownLabel;
    PhoneType m_phoneType = UnknownPhoneType;
    QString m_key, m_value;
};

#endif // VCARDFIELD_H
