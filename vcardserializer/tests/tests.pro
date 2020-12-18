QT -= gui
QT += testlib quick

INCLUDEPATH += ../

include(../Qt-Quoted-Printable/quotedprintable.pri)
SOURCES += \
  ../src/vcardfield.cpp \
  ../src/vcardserializer.cpp \
  tst_vcardserializer.cpp

HEADERS += \
  ../src/vcardfield.h \
  ../src/vcardserializer.h

RESOURCES += \
  assets.qrc

QMAKE_RESOURCE_FLAGS += -no-compress
