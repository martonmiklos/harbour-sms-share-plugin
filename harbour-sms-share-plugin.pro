TEMPLATE = lib
TARGET = $$qtLibraryTarget(smsshareplugin)
DEPENDPATH += .

CONFIG += plugin link_pkgconfig
CONFIG += sailfishapp_i18n_idbased
PKGCONFIG += nemotransferengine-qt5

QT += qml

# Input
HEADERS += \
    src/smsplugininfo.h \
    src/smsshareplugin.h \
    src/smsuploader.h

SOURCES += \
    src/smsplugininfo.cpp \
    src/smsshareplugin.cpp \
    src/smsuploader.cpp
OTHER_FILES += \
    qml/SmsShare.qml \
    harbour-sms-share-plugin.spec \
    rpm/* \
    translations/*.ts

TRANSLATIONS += translations/harbour-sms-share-plugin-hu_HU.ts

shareui.files = qml/SmsShare.qml
shareui.path = /usr/share/nemo-transferengine/plugins

target.path = /usr/lib/nemo-transferengine/plugins

translations.files = translations/*.qm
translations.path = $$[QT_INSTALL_DATA]/../translations/nemotransferengine

INSTALLS += target shareui translations

DISTFILES += \
    translations/harbour-sms-plugin-hu.ts \
    rpm/harbour-sms-share-plugin.yaml
