!exists($$PWD/Qt-Quoted-Printable) {
    error("Please do git submodule update --init --recursive")
}

TEMPLATE = lib
TARGET = vCardSerializer
QT += qml quick
CONFIG += plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)
uri = hu.mm.vCardSerializer

include(Qt-Quoted-Printable/quotedprintable.pri)

# Input
SOURCES += \
        src/vcardserializer_plugin.cpp \
        src/vcardserializer.cpp \
        src/vcardfield.cpp

HEADERS += \
        src/vcardserializer_plugin.h \
        src/vcardserializer.h \
        src/vcardfield.h

DISTFILES += \
    rpm/vcardserializer.changes.run.in \
    rpm/vcardserializer.yaml \
    qmldir \
    rpm/vcardserializer.changes

target.path = $$[QT_INSTALL_QML]/$$PLUGIN_IMPORT_PATH/$$replace(uri, \\., /)
INSTALLS += target

qmldir.files += $$PWD/qmldir
qmldir.path +=  $$target.path
INSTALLS += qmldir
