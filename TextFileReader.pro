#-------------------------------------------------
#
# Project created by QtCreator 2014-08-18T18:17:33
#
#-------------------------------------------------

QT       += core gui\
    printsupport


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TextFileReader
TEMPLATE = app

INCLUDEPATH += C:/Users/Staples/Downloads/boost_1_56_0/boost_1_56_0 \
    /Users/bren/Downloads/boost_1_56_0

SOURCES += main.cpp\
        mainwindow.cpp \
    textfilereader.cpp \
    workorderscroller.cpp \
    woscrollread.cpp \
    tabdatadisplay.cpp \
    qcustomplot.cpp \
    workorderdatum.cpp \
    thicknessfilereader.cpp \
    hmifilereader.cpp \
    workorderfilereader.cpp \
    spectralfilereader.cpp \
    rgafilereader.cpp \
    dropbox.cpp \
    specmtx.cpp \
    stdmtx.cpp \
    chartdataview.cpp \
    reportfilereader.cpp \
    pyrometerfilereader.cpp \
    notepage.cpp

HEADERS  += mainwindow.h \
    textfilereader.h \
    workorderscroller.h \
    woscrollread.h \
    tabdatadisplay.h \
    qcustomplot.h \
    workorderdatum.h \
    thicknessfilereader.h \
    hmifilereader.h \
    workorderfilereader.h \
    spectralfilereader.h \
    rgafilereader.h \
    dropbox.h \
    specmtx.h \
    stdmtx.h \
    chartdataview.h \
    reportfilereader.h \
    pyrometerfilereader.h \
    notepage.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    MathematicaSrc.qrc \
    AlStd.qrc

OTHER_FILES +=

