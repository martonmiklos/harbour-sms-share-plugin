#include <QtTest>

#include "../src/vcardserializer.h"

#include <QFile>
#include <QDebug>
#include <QTranslator>

class TestVCardSerializer : public QObject
{
    Q_OBJECT

public:
    TestVCardSerializer()
    {
        m_tr = new QTranslator(this);
        if (!m_tr->load(":/sailfish_components_contacts_qt5-hu.qm"))
            QFAIL("Unable to load translation");
        if (!QCoreApplication::installTranslator(m_tr))
            QFAIL("Unable to set translation");
    }

private slots:
    void testGliga()
    {
        QFile vcfFile("://test.vcf");
        QVERIFY(vcfFile.open(QFile::ReadOnly));
        const auto vcardData = vcfFile.readAll();
        vcfFile.close();

        vCardSerializer *serializer = new vCardSerializer();
        serializer->setVCardData(vcardData);
        auto serialized =  serializer->serialize_vCardFull();

        auto awaited = QStringLiteral("P\u00E9ter Nigga\n"
                        "Munkahelyi mobil: +36904890152\n"
                       "Szem\u00E9lyes mobil: +36904445738\n"
                       "E-mail: nigga69@kmail.com\n"
                       "C\u00EDm: 1103 Kutyapest 3100 domb\n"
                       "Sz\u00FClet\u00E9snap: 1956-12-26");
        QCOMPARE(awaited, serialized);
    }

private:
    QTranslator *m_tr = nullptr;
};


QTEST_MAIN(TestVCardSerializer)

#include "tst_vcardserializer.moc"
