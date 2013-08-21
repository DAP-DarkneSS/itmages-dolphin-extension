QT       += core gui dbus

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
                   trans \
                   images

  desktop.path   = $$PREFIX/share/kde4/services/ServiceMenus
  desktop.files += itmages-dolphin-extension.desktop
  images.path    = $$PREFIX/share/icons/hicolor/scalable/apps
  images.files  += itmages.svg
  trans.path     = $$PREFIX/share/itmages
  trans.files   += itmages-dolphin-extension_ru.qm
  target.path    = $$PREFIX/bin
}
