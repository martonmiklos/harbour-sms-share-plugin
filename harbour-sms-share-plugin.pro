!exists($$PWD/vcardserializer) {
    error("Please do git submodule update --init --recursive")
}

TEMPLATE = lib
TARGET = $$qtLibraryTarget(harboursmsshareplugin)
CONFIG += plugin link_pkgconfig
PKGCONFIG += nemotransferengine-qt5

QT += qml

NAME = harbour-sms-share-plugin
SMSSHARE_UI_DIR = /usr/share/$$NAME
SMSSHARE_TRANSLATIONS_DIR = $${SMSSHARE_UI_DIR}/translations

QMAKE_CXXFLAGS += -Wno-unused-parameter -fvisibility=hidden
QMAKE_CFLAGS += -fvisibility=hidden
QMAKE_LFLAGS += -fvisibility=hidden

INCLUDEPATH += \
    transferengine

DEFINES += \
    SMSSHARE_UI_DIR=\\\"$$SMSSHARE_UI_DIR\\\" \
    SMSSHARE_TRANSLATIONS_FILE=\\\"$$NAME\\\" \
    SMSSHARE_TRANSLATIONS_DIR=\\\"$$SMSSHARE_TRANSLATIONS_DIR\\\"

CONFIG(debug, debug|release) {
    DEFINES += DEBUG HARBOUR_DEBUG
}

OTHER_FILES += \
    transferengine/mediatransferinterface.h \
    transferengine/transfermethodinfo.h \
    transferengine/transferplugininfo.h \
    transferengine/transferplugininterface.h

# Input
HEADERS += \
    src/smsplugininfo.h \
    src/smsshareplugin.h \
    src/smsuploader.h

SOURCES += \
    src/smsplugininfo.cpp \
    src/smsshareplugin.cpp \
    src/smsuploader.cpp


SMSSHARE_UI_FILES = \
    images/smsshare.svg \
    qml/SmsShare.qml

OTHER_FILES += $$SMSSHARE_UI_FILES

target.path = $$[QT_INSTALL_LIBS]/nemo-transferengine/plugins
INSTALLS += target

shareui.files = $$SMSSHARE_UI_FILES
shareui.path = $$SMSSHARE_UI_DIR
INSTALLS += shareui

OTHER_FILES += \
    translations/*

# Translations
TRANSLATION_SOURCES = \
  $${_PRO_FILE_PWD_}/qml \
  $${_PRO_FILE_PWD_}/src

defineTest(addTrFile) {
    in = $${_PRO_FILE_PWD_}/translations/$$1
    out = $${OUT_PWD}/translations/$$1

    s = $$replace(1,-,_)
    lupdate_target = lupdate_$$s
    lrelease_target = lrelease_$$s

    $${lupdate_target}.commands = lupdate -noobsolete -locations none $${TRANSLATION_SOURCES} -ts \"$${in}.ts\" && \
        mkdir -p \"$${OUT_PWD}/translations\" &&  [ \"$${in}.ts\" != \"$${out}.ts\" ] && \
        cp -af \"$${in}.ts\" \"$${out}.ts\" || :

    $${lrelease_target}.target = $${out}.qm
    $${lrelease_target}.depends = $${lupdate_target}
    $${lrelease_target}.commands = lrelease -idbased \"$${out}.ts\"

    QMAKE_EXTRA_TARGETS += $${lrelease_target} $${lupdate_target}
    PRE_TARGETDEPS += $${out}.qm
    qm.files += $${out}.qm

    export($${lupdate_target}.commands)
    export($${lrelease_target}.target)
    export($${lrelease_target}.depends)
    export($${lrelease_target}.commands)
    export(QMAKE_EXTRA_TARGETS)
    export(PRE_TARGETDEPS)
    export(qm.files)
}

LANGUAGES = hu pl sv es

addTrFile($$NAME)
for(l, LANGUAGES) {
    addTrFile($${NAME}-$$l)
}

qm.path = $$SMSSHARE_TRANSLATIONS_DIR
qm.CONFIG += no_check_exist
INSTALLS += qm
