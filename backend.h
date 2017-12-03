#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QQmlListProperty>
#include <QTimer>

#include "task.h"
#include "connection.h"

class BackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int connectionIndex READ connectionIndex WRITE setConnectionIndex NOTIFY connectionIndexChanged)
    Q_PROPERTY(QQmlListProperty<Task> tasks READ tasks NOTIFY tasksChanged)
    Q_PROPERTY(QQmlListProperty<Connection> connections READ connections NOTIFY connectionsChanged)
public:
    Q_INVOKABLE void removeConnection(int idx);
    Q_INVOKABLE int appendConnection(const QString &name, const QString &address, int port, const QString &listenIf, int listenPort);
    Q_INVOKABLE void removeTask(int idx);
    Q_INVOKABLE int appendTask(const QString &name, bool userRos, const QString &command, const QString &launchFile);
    explicit BackEnd(QObject *parent = nullptr);
    ~BackEnd();

    Task *taskById(const QStringRef &id);
    int taskIdxByCommand(bool useRos, const QStringRef &command);
    void activateTasks(const QStringList &ids, const QVector<int> idxs);
    QQmlListProperty<Task> tasks();
    QQmlListProperty<Connection> connections();
    int connectionIndex() const { return connectionIndex_; }
    QVector<bool> activeTasks() const;

public slots:
    void setConnectionIndex(int connectionIndex);
    void saveSettings();
    void settingsChanged();

signals:
    void tasksChanged(QQmlListProperty<Task> tasks);
    void connectionsChanged(QQmlListProperty<Connection> connections);
    void connectionIndexChanged(int connectionIndex);

private slots:
    void updateTasks();

private:
    static void appendConnection(QQmlListProperty<Connection>* list, Connection *c);
    static void clearConnections(QQmlListProperty<Connection>* list);
    static Connection *connection(QQmlListProperty<Connection>* list, int i);
    static int connectionCount(QQmlListProperty<Connection>* list);

    static void appendTask(QQmlListProperty<Task>*, Task *);
    static void clearTasks(QQmlListProperty<Task>* list);
    static Task *task(QQmlListProperty<Task>* list, int i);
    static int taskCount(QQmlListProperty<Task>* list);

    QVector<Task *> tasks_;
    QVector<Connection *> connections_;
    int connectionIndex_;
    bool settingsSaved;
    const QString settingsFile_;
    QTimer timer_;
};

#endif // BACKEND_H
