TEMPLATE = lib
TARGET = $$qtLibraryTarget(smsshareplugin)
DEPENDPATH += .

CONFIG += plugin link_pkgconfig
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
    harbour-sms-share-plugin.spec

shareui.files = qml/SmsShare.qml
shareui.path = /usr/share/nemo-transferengine/plugins

target.path = /usr/lib/nemo-transferengine/plugins
INSTALLS += target shareui

OTHER_FILES += \
    rpm/* \
    translations/*

DISTFILES += \
    translations/harbour_sms_plugin_hu.ts
