#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#include "backend.h"
#include "interfaces.h"
#include "scan.h"

#include "qpointfield.h"
#include "qpointcloud.h"
#include "qpointcloudreader.h"
#include "qpointcloudgeometry.h"

int main(int argc, char *argv[])
{
    Scan scan;
    scan.loadPCD("d:/bag/result.pcd");

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<QPointCloudReader>("pcl", 1, 0, "PointcloudReader");
    qmlRegisterType<QPointcloud>("pcl", 1, 0, "Pointcloud");
    qmlRegisterType<QPointcloudGeometry>("pcl", 1, 0, "PointcloudGeometry");
    qmlRegisterUncreatableType<QPointfield>("pcl", 1, 0, "Pointfield", "Can not yet be created in qml, use PointcloudReader.");

    qmlRegisterType<BackEnd>("udp_client.backend", 1, 0, "BackEnd");
    qmlRegisterType<Connection>("udp_client.backend", 1, 0, "Connection");
    qmlRegisterType<Task>("udp_client.backend", 1, 0, "Task");
    qmlRegisterType<Interfaces>("udp_client.backend", 1, 0, "Interfaces");

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
