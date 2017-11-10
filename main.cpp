#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#include "backend.h"
#include "interfaces.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

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
