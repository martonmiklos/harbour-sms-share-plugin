TEMPLATE = lib
TARGET = $$qtLibraryTarget(smsshareplugin)
DEPENDPATH += .

CONFIG += plugin link_pkgconfig
PKGCONFIG += nemotransferengine-qt5

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

TS_FILE = $$OUT_PWD/harbour_sms_share_plugin.ts
EE_QM = $$OUT_PWD/harbour_sms_share_plugin-eng_en.qm

ts.commands += mkdir -p $$OUT_PWD/translations;
ts.commands += lupdate . -ts $$TS_FILE
ts.CONFIG += no_check_exist no_link
ts.output = $$TS_FILE
ts.input = ..

ts_install.files = $$TS_FILE
ts_install.path = /usr/share/translations/source
ts_install.CONFIG += no_check_exist

# should add -markuntranslated "-" when proper translations are in place (or for testing)
engineering_english.commands += lupdate . -ts $$TS_FILE; lrelease -idbased $$TS_FILE -qm $$EE_QM
engineering_english.CONFIG += no_check_exist no_link
engineering_english.depends = ts
engineering_english.input = $$TS_FILE
engineering_english.output = $$EE_QM

engineering_english_install.path = /usr/share/translations/nemotransferengine
engineering_english_install.files = $$EE_QM
engineering_english_install.CONFIG += no_check_exist

TS_HU_FILE = translations/harbour_sms_share_plugin-hu.ts
QM_HU_FILE = harbour_sms_share_plugin-hu.qm

hungarian.commands += lupdate . -ts $$TS_HU_FILE; lrelease -idbased $$TS_HU_FILE -qm $$QM_HU_FILE
hungarian.CONFIG += no_check_exist no_link
hungarian.depends = ts
hungarian.input = $$TS_HU_FILE
hungarian.output = $$QM_HU_FILE

hungarian_install.path = /usr/share/translations/nemotransferengine
hungarian_install.files = $$QM_HU_FILE
hungarian_install.CONFIG += no_check_exist

QMAKE_EXTRA_TARGETS += ts engineering_english hungarian

PRE_TARGETDEPS += ts engineering_english hungarian

INSTALLS += ts_install engineering_english_install hungarian_install
# engineering_english_install

OTHER_FILES += \
    rpm/* \
    translations/*

DISTFILES += \
    translations/harbour_sms_plugin_hu.ts
