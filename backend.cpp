#include "backend.h"
#include <QNetworkInterface>
#include <QSettings>
#include <QStandardPaths>

void BackEnd::removeConnection(int idx)
{
    connections_.remove(idx);
    emit connectionsChanged(connections());
}

int BackEnd::appendConnection(const QString &name, const QString &address, int port, const QString &listenIf, int listenPort)
{
    int r = connections_.length();
    auto c = new Connection(name, address, port, listenIf, listenPort, this);
    connections_.append(c);
    emit connectionsChanged(connections());
    return r;
}

void BackEnd::removeTask(int idx)
{
    tasks_.remove(idx);
    emit tasksChanged(tasks());
}

int BackEnd::appendTask(const QString &name, bool useRos, const QString &command, const QString &launchFile)
{
    int r = tasks_.length();
    auto t = new Task(name, useRos, command, launchFile);
    tasks_.append(t);
    emit tasksChanged(tasks());
    return r;
}

BackEnd::BackEnd(QObject *parent) : QObject(parent), settingsFile_(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/udp-client.ini")
{    
    QSettings settings(settingsFile_, QSettings::IniFormat);
    connectionIndex_ = settings.value("connectionIndex", QVariant(-1)).toInt();
    int l = settings.beginReadArray("connections");
    for (int i = 0; i < l; ++i) {
        settings.setArrayIndex(i);
        Connection * c = new Connection(
                    settings.value("name").toString(),
                    settings.value("address").toString(),
                    settings.value("port").toInt(),
                    settings.value("listenIf").toString(),
                    settings.value("listenPort").toInt(),
                    this);
        connections_.append(c);
    }
    settings.endArray();

    l = settings.beginReadArray("tasks");
    for (int i = 0; i < l; ++i) {
        settings.setArrayIndex(i);
        Task * t = new Task(
                    settings.value("name").toString(),
                    settings.value("useRos").toBool(),
                    settings.value("command").toString(),
                    settings.value("launchFile").toString(),
                    this);
        tasks_.append(t);
    }
    settings.endArray();
}

BackEnd::~BackEnd()
{
    saveState();
}

Task *BackEnd::taskById(const QStringRef &id)
{
    for (auto t : tasks_)
        if (t->id() == id)
            return t;
    return nullptr;
}

QQmlListProperty<Task> BackEnd::tasks()
{
    return QQmlListProperty<Task>(this, this,
                                        &BackEnd::appendTask,
                                        &BackEnd::taskCount,
                                        &BackEnd::task,
                                        &BackEnd::clearTasks);
}

QQmlListProperty<Connection> BackEnd::connections()
{
    return QQmlListProperty<Connection>(this, this,
                                        &BackEnd::appendConnection,
                                        &BackEnd::connectionCount,
                                        &BackEnd::connection,
                                        &BackEnd::clearConnections);
}

QStringList BackEnd::interfaces() const
{
    QStringList r;
    for (auto i : QNetworkInterface::allInterfaces()) {
        if (!i.isValid()) continue;
        int f = i.flags();
        if (f & (QNetworkInterface::IsLoopBack | QNetworkInterface::IsPointToPoint)) continue;
        if (!(f & QNetworkInterface::IsUp)) continue;
        for (auto a : i.addressEntries()) {
            QHostAddress ip = a.ip();
            if (ip.isNull() || ip.protocol() != QAbstractSocket::IPv4Protocol) continue;
            r.push_back(ip.toString());
        }
    }
    return r;
}

void BackEnd::setConnectionIndex(int connectionIndex)
{
    if (connectionIndex_ == connectionIndex) return;
    connectionIndex_ = connectionIndex;
    emit connectionIndexChanged(connectionIndex_);
}

void BackEnd::saveState()
{
    QSettings settings(settingsFile_, QSettings::IniFormat);
    settings.setValue("connectionIndex", connectionIndex_);
    int l = connections_.length();
    settings.beginWriteArray("connections", l);
    for (int i = 0; i < l ; ++i) {
        const Connection *c = connections_[i];
        settings.setArrayIndex(i);
        settings.setValue("name",       c->name());
        settings.setValue("address",    c->address());
        settings.setValue("port",       c->port());
        settings.setValue("listenIf",   c->listenIf());
        settings.setValue("listenPort", c->listenPort());
    }
    settings.endArray();

    l = tasks_.length();
    settings.beginWriteArray("tasks", l);
    for (int i = 0; i < l ; ++i) {
        const Task *t = tasks_[i];
        settings.setArrayIndex(i);
        settings.setValue("name",       t->name());
        settings.setValue("useRos",     t->useRos());
        settings.setValue("command",    t->command());
        settings.setValue("launchFile", t->launchFile());
    }
    settings.endArray();
    settings.sync();
}

// static Connections handlers

void BackEnd::appendConnection(QQmlListProperty<Connection> *list, Connection *c) {
    reinterpret_cast<BackEnd*>(list->data)->connections_.append(c);
}

void BackEnd::clearConnections(QQmlListProperty<Connection> *list)
{
    reinterpret_cast<BackEnd*>(list->data)->connections_.clear();
}

Connection *BackEnd::connection(QQmlListProperty<Connection> *list, int i)
{
    return reinterpret_cast<BackEnd*>(list->data)->connections_.at(i);
}

int BackEnd::connectionCount(QQmlListProperty<Connection> *list)
{
    return reinterpret_cast<BackEnd*>(list->data)->connections_.length();
}

// static Tasks handlers

void BackEnd::appendTask(QQmlListProperty<Task> *list, Task *c) {
    //reinterpret_cast<BackEnd*>(list->data)->tasks_.append(*c);
}

void BackEnd::clearTasks(QQmlListProperty<Task> *list)
{
    reinterpret_cast<BackEnd*>(list->data)->tasks_.clear();
}

Task *BackEnd::task(QQmlListProperty<Task> *list, int i)
{
    return reinterpret_cast<BackEnd*>(list->data)->tasks_.at(i);
}

int BackEnd::taskCount(QQmlListProperty<Task> *list)
{
    return reinterpret_cast<BackEnd*>(list->data)->tasks_.length();
}
