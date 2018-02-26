#include "backend.h"
#include <QSettings>
#include <QStandardPaths>

void BackEnd::removeConnection(int idx)
{
    connections_.remove(idx);
    emit connectionsChanged(connections());
    saveSettings();
}

int BackEnd::appendConnection(const QString &name, const QString &address, int port, const QString &listenIf, int listenPort)
{
    int r = connections_.length();
    auto c = new Connection(name, address, port, listenIf, listenPort, this);
    connections_.append(c);
    emit connectionsChanged(connections());
    saveSettings();
    return r;
}

void BackEnd::removeTask(int idx)
{
    tasks_.remove(idx);
    emit tasksChanged(tasks());
    saveSettings();
}

int BackEnd::appendTask(const QString &name, bool useRos, const QString &command, const QString &launchFile)
{
    int r = tasks_.length();
    auto t = new Task(name, useRos, command, launchFile);
    tasks_.append(t);
    emit tasksChanged(tasks());
    saveSettings();
    return r;
}

BackEnd::BackEnd(QObject *parent) :
    QObject(parent), settingsSaved(true), settingsFile_(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/udp-client.ini")
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

    connect(this, SIGNAL(connectionIndexChanged(int)), SLOT(settingsChanged()));
    connect(this, SIGNAL(tasksChanged(QQmlListProperty<Task>)), SLOT(settingsChanged()));
    connect(this, SIGNAL(connectionsChanged(QQmlListProperty<Connection>)), SLOT(settingsChanged()));

    connect(&timer_, SIGNAL(timeout()), this, SLOT(updateTasks()));
    timer_.start(5000);
}

BackEnd::~BackEnd()
{
    saveSettings();
}

Task *BackEnd::taskById(const QStringRef &id)
{
    for (auto t : tasks_)
        if (t->id() == id)
            return t;
    return nullptr;
}

QString firstWord(const QString &s)
{
    int i = s.indexOf(' ');
    return (i < 0) ? s : s.mid(0, i);
}

int BackEnd::taskIdxByCommand(bool useRos, const QStringRef &command)
{
    QStringRef cmd = command;
    int i = cmd.lastIndexOf('/');
    if (i >= 0) cmd = cmd.mid(i + 1);
    for (int x = 0; x < tasks_.length(); x++) {
        Task *t = tasks_[x];
        if (useRos) {
            if (!t->useRos()) continue;
            if (firstWord(t->launchFile()) == cmd) return x;
        } else {
            if (t->useRos()) continue;
            if (firstWord(t->command()) == cmd) return x;
        }
    }
    return -1;
}

void BackEnd::activateTasks(const QStringList &ids, const QVector<int> idxs)
{
    QVector<bool> active;
    int l = tasks_.length();
    active.resize(l);
    for (int i = 0; i < l; i++)
        active[i] = tasks_[i]->state() == Task::Active;
    for (int i = 0; i < ids.length(); i++) {
        Task *task = tasks_[idxs[i]];
        task->setId(ids[i]);
        task->setState(Task::Active);
        active[idxs[i]] = false;
    }
    for (int i = 0; i < l; i++)
        if (active[i])
            tasks_[i]->setState(Task::Stop);
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

void BackEnd::setConnectionIndex(int connectionIndex)
{
    if (connectionIndex_ == connectionIndex) return;
    if (connectionIndex_ >= 0 && connectionIndex_ < connections_.size())
        connections_[connectionIndex_]->unbind();
    connectionIndex_ = connectionIndex;
    if (connectionIndex >= 0)
        connections_[connectionIndex]->bind();
    for (auto t : tasks_)
        t->setState(Task::Stop);
    emit connectionIndexChanged(connectionIndex_);
}

void BackEnd::saveSettings()
{
    if (settingsSaved) return;
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
    settingsSaved = true;
}

void BackEnd::settingsChanged()
{
    settingsSaved = false;
}

void BackEnd::updateTasks()
{
    if (connectionIndex_ < 0) return;
    Connection * c = connections_[connectionIndex_];
    if (c->state() == Connection::Error || c->state() == Connection::Off) return;
    c->queryTasks();
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
    auto &ts = reinterpret_cast<BackEnd*>(list->data)->tasks_;
    if (i >= ts.length() || i < 0) return nullptr;
    return ts.at(i);
}

int BackEnd::taskCount(QQmlListProperty<Task> *list)
{
    return reinterpret_cast<BackEnd*>(list->data)->tasks_.length() + 1;
}
