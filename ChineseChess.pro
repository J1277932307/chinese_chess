QT += widgets gui \
    widgets

HEADERS += \
    board.h \
    stone.h

SOURCES += \
    board.cpp \
    main.cpp \
    stone.cpp

QMAKE_CXXFLAGS += "-std=c++11"
