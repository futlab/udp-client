#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QQmlListProperty>

#include "task.h"
#include "connection.h"

class BackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int connectionIndex READ connectionIndex WRITE setConnectionIndex NOTIFY connectionIndexChanged)
    Q_PROPERTY(QQmlListProperty<Task> tasks READ tasks NOTIFY tasksChanged)
    Q_PROPERTY(QQmlListProperty<Connection> connections READ connections NOTIFY connectionsChanged)
    Q_PROPERTY(QStringList interfaces READ interfaces NOTIFY interfacesChanged)
public:
    Q_INVOKABLE void removeConnection(int idx);
    Q_INVOKABLE int appendConnection(const QString &name, const QString &address, int port, const QString &listenIf, int listenPort);
    Q_INVOKABLE void removeTask(int idx);
    Q_INVOKABLE int appendTask(const QString &name, bool userRos, const QString &command, const QString &launchFile);
    explicit BackEnd(QObject *parent = nullptr);
    ~BackEnd();

    Task *taskById(const QStringRef &id);
    QQmlListProperty<Task> tasks();
    QQmlListProperty<Connection> connections();
    QStringList interfaces() const;
    int connectionIndex() const { return connectionIndex_; }

public slots:
    void setConnectionIndex(int connectionIndex);
    void saveState();

signals:
    void tasksChanged(QQmlListProperty<Task> tasks);
    void connectionsChanged(QQmlListProperty<Connection> connections);
    void interfacesChanged(QStringList interfaces);
    void connectionIndexChanged(int connectionIndex);

private:
    static void appendConnection(QQmlListProperty<Connection>* list, Connection *c);
    static void clearConnections(QQmlListProperty<Connection>* list);
    static Connection *connection(QQmlListProperty<Connection>* list, int i);
    static int connectionCount(QQmlListProperty<Connection>* list);

    static void appendTask(QQmlListProperty<Task>* list, Task *c);
    static void clearTasks(QQmlListProperty<Task>* list);
    static Task *task(QQmlListProperty<Task>* list, int i);
    static int taskCount(QQmlListProperty<Task>* list);

    QVector<Task *> tasks_;
    QVector<Connection *> connections_;
    QStringList m_interfaces;
    int connectionIndex_;
    const QString settingsFile_;
};

#endif // BACKEND_H
