QT += quick quickcontrols2
QT += 3dcore 3drender 3dinput 3dquick 3dlogic 3dquickextras
CONFIG += c++14

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
    backend.cpp \
    task.cpp \
    connection.cpp \
    interfaces.cpp \
    scan.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    backend.h \
    task.h \
    connection.h \
    interfaces.h \
    scan.h

DISTFILES +=

FORMS +=

INCLUDEPATH += ../boost_1_65_1
#QMAKE_LFLAGS += /LIBPATH:d:/src/boost_1_65_1/stage/lib
#LIBS += d:/src/boost_1_65_1/stage/lib/libboost_thread-vc140-mt-1_65_1.lib
#LIBS += d:/src/boost_1_65_1/stage/lib/libboost_system-vc140-mt-1_65_1.lib
#LIBS += d:/src/boost_1_65_1/stage/lib/libboost_date_time-vc140-mt-1_65_1.lib
#LIBS += d:/src/boost_1_65_1/stage/lib/libboost_chrono-vc140-mt-1_65_1.lib
#LIBS += d:/src/boost_1_65_1/stage/lib/libboost_filesystem-vc140-mt-1_65_1.lib
#LIBS += d:/src/boost_1_65_1/stage/lib/libboost_iostreams-vc140-mt-1_65_1.lib
LIBS += "-Lc:/Boost/lib/"

#

# PCL
INCLUDEPATH += ../pcl/io/include ../pcl/common/include d:\src\build\pcl\include d:\\src\\eigen
#SOURCES += ../pcl/io/src/pcd_io.cpp

#LIBS += "-Ld:\src\build\pcl\lib"
LIBS += D:/src/build/pcl/lib/pcl_io_debug.lib
LIBS += D:/src/build/pcl/lib/pcl_common_debug.lib

# Qt3DPointCloudRenderer

DEFINES += WITH_PCL
INCLUDEPATH += ../Qt3DPointcloudRenderer/include
HEADERS += \
    ../Qt3DPointcloudRenderer/include/qpointcloud.h \
    ../Qt3DPointcloudRenderer/include/qpointcloudgeometry.h \
    ../Qt3DPointcloudRenderer/include/qpointcloudreader.h \
    ../Qt3DPointcloudRenderer/include/qpointfield.h

SOURCES += \
    ../Qt3DPointcloudRenderer/src/qpointcloud.cpp \
    ../Qt3DPointcloudRenderer/src/qpointcloudgeometry.cpp \
    ../Qt3DPointcloudRenderer/src/qpointcloudreader.cpp \
    ../Qt3DPointcloudRenderer/src/qpointfield.cpp
