TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.c

# Default rules for deployment.
target.path = /usr/bin/$${TARGET}
!isEmpty(target.path): INSTALLS += target
