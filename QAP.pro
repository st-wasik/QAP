QT -= gui
QT += core

CONFIG += c++11 console
CONFIG -= app_bundle
QMAKE_CXXFLAGS += -O3

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    two_opt.cpp \
    input.cpp \
    cost.cpp \
    common.cpp \
    greedy.cpp \
    steepest.cpp \
    randomwalk.cpp \
    heuristic.cpp \
    random.cpp \
    globaloutput.cpp \
    irunnable.cpp \
    tabu.cpp \
    annealing.cpp

HEADERS += \
    two_opt.h \
    common.h \
    input.h \
    cost.h \
    greedy.h \
    steepest.h \
    randomwalk.h \
    heuristic.h \
    random.h \
    irunnable.hpp \
    globaloutput.h \
    tabu.hpp \
    annealing.h
