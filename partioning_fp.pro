#-------------------------------------------------
#
# Project created by QtCreator 2019-01-23T23:05:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = partioning_fp
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        csv_data_manipulator.cpp \
        pt_fp.cpp \
    zynq_model.cpp \
    fpga.cpp \
    task_set.cpp \
    generate_fp.cpp

LIBS += \
       -lgurobi_g++5.2\
       -lgurobi_c++\
       -lgurobi70

HEADERS += \
        pt_fp.h \
        csv_data_manipulator.hpp \
        fpga.h \
    zynq_model.h \
    partition.h \
    generate_fp.h

FORMS += \
    fp.ui
