#ifndef VCARDFIELD_H
#define VCARDFIELD_H

#include <QString>

class vCardField
{
public:
    vCardField(const QString & key, const QString &value);
    bool operator ==(const vCardField & other);
    bool operator !=(const vCardField & other);

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

    QString serializeFull() const;
    QString serializeShort() const;
    Label label() const;
    FieldType fieldType() const;
    PhoneType phoneType() const;

private:
    FieldType m_fieldType;
    Label m_label = UnknownLabel;
    PhoneType m_phoneType = UnknownPhoneType;
    QString m_key, m_value;
};

#endif // VCARDFIELD_H
