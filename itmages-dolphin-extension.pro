QT       += core gui dbus
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET    = itmages-dolphin-extension
TEMPLATE  = app

SOURCES  += src/main.cpp \
            src/itmagesitem.cpp \
            src/links.cpp \
            src/itmagesuploader.cpp
		
HEADERS  += src/itmagesitem.h \
            src/links.h \
            src/itmagesuploader.h

FORMS    += src/itmagesitem.ui \
            src/itmagesuploader.ui

TRANSLATIONS = itmages-dolphin-extension_ru.ts
CODECFORSRC  = UTF-8

unix {
  isEmpty(PREFIX):PREFIX = /usr

  INSTALLS      += target \
                   desktop \
                   desktop1 \
                   man \
                   trans \
                   images

  desktop.path   = $$PREFIX/share/kde4/services/ServiceMenus
  desktop.files += itmages-dolphin-extension.desktop
  desktop1.path   = $$PREFIX/share/applications
  desktop1.files += itmages-uploader.desktop
  man.path   = $$PREFIX/share/man/man1
  man.files += itmages-dolphin-extension.1
  images.path    = $$PREFIX/share/icons/hicolor/scalable/apps
  images.files  += itmages.svg
  trans.path     = $$PREFIX/share/itmages
  trans.files   += itmages-dolphin-extension_ru.qm
  target.path    = $$PREFIX/bin
}
