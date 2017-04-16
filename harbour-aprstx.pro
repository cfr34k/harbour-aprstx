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

# The name of your application
TARGET = harbour-aprstx

CONFIG += sailfishapp

QT += multimedia positioning

SOURCES += src/harbour-aprstx.cpp \
    src/settings.cpp \
    src/aprsctrl.cpp \
    src/aprs.cpp \
    src/afskmod.cpp

OTHER_FILES += qml/harbour-aprstx.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    qml/pages/SecondPage.qml \
    rpm/harbour-aprstx.changes.in \
    rpm/harbour-aprstx.spec \
    rpm/harbour-aprstx.yaml \
    translations/*.ts \
    harbour-aprstx.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 256x256

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/harbour-aprstx-de.ts

DISTFILES += \
    qml/pages/SettingsPage.qml

HEADERS += \
    src/settings.h \
    src/aprs.h \
    src/aprsctrl.h \
    src/afskmod.h
