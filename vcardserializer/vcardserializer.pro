TEMPLATE = lib
TARGET = vCardSerializer
QT += qml quick
CONFIG += plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)
uri = hu.mm.vCardSerializer

# Input
SOURCES += \
        src/vcardserializer_plugin.cpp \
        src/vcardserializer.cpp

HEADERS += \
        src/vcardserializer_plugin.h \
        src/vcardserializer.h

DISTFILES += \
    rpm/vcardserializer.changes.in \
    rpm/vcardserializer.changes.run.in \
    rpm/vcardserializer.spec \
    rpm/vcardserializer.yaml \
    translations/*.ts \
    qmldir

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/vcardserializer-de.ts


target.path = $$[QT_INSTALL_QML]/$$PLUGIN_IMPORT_PATH/$$replace(uri, \\., /)
INSTALLS += target

qmldir.files += $$PWD/qmldir
qmldir.path +=  $$target.path
INSTALLS += qmldir

