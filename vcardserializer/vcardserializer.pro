# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

TEMPLATE = lib

# The name of your application
LIBNAME = vCardSerializer
TARGET = $$qtLibraryTarget($$LIBNAME)
uri = hu.mm.$$LIBNAME

DEFINES += PLUGIN_URI=$$uri

QT += qml quick

CONFIG += qt plugin c++11

SOURCES += src/vcardserializer.cpp \
    src/plugin.cpp

DISTFILES += \
    rpm/vcardserializer.changes.in \
    rpm/vcardserializer.changes.run.in \
    rpm/vcardserializer.spec \
    rpm/vcardserializer.yaml \
    translations/*.ts \
    qmldir


# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/vcardserializer-de.ts

HEADERS += \
    src/vcardserializer.h \
    src/plugin.h

target.path = $$[QT_INSTALL_QML]/$$PLUGIN_IMPORT_PATH/$$replace(uri, \\., /)
INSTALLS += target

qmldir.files += $$PWD/qmldir
qmldir.path +=  $$target.path
INSTALLS += qmldir

#translations.files = translations/*.qm
#translations.path = $$[QT_HOST_DATA]/translations
#INSTALLS += translations
