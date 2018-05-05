#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QQmlListProperty>
#include <QTimer>
#include <map>
#include <memory>

#include "task.h"
#include "connection.h"
#include "files.h"

using namespace std;

class Subscriber;

class BackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int connectionIndex READ connectionIndex WRITE setConnectionIndex NOTIFY connectionIndexChanged)
    Q_PROPERTY(QQmlListProperty<Task> tasks READ tasks NOTIFY tasksChanged)
    Q_PROPERTY(QQmlListProperty<Connection> connections READ connections NOTIFY connectionsChanged)
    Q_PROPERTY(QString buildInfo READ buildInfo NOTIFY buildInfoChanged)
    Q_PROPERTY(Files* bags READ bags NOTIFY bagsChanged)
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
    QString buildInfo();
    QQmlListProperty<Task> tasks();
    QQmlListProperty<Connection> connections();
    int connectionIndex() const { return connectionIndex_; }
    QVector<bool> activeTasks() const;
    Interface *interfaceByName(const QString &name);
    void subscribe(const QString &id, Subscriber *subscriber);
    Subscriber *findSubscriber(const QStringRef &id);
    Connection *activeConnection();
    Files* bags() { return &bags_; }

public slots:
    void setConnectionIndex(int connectionIndex);
    void saveSettings();
    void settingsChanged();

signals:
    void tasksChanged(QQmlListProperty<Task> tasks);
    void connectionsChanged(QQmlListProperty<Connection> connections);
    void connectionIndexChanged(int connectionIndex);
    void bagsChanged(Files *bags);  
    void buildInfoChanged(QString buildInfo);

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

    //struct Subscriber
    QVector<Task *> tasks_;
    QVector<Connection *> connections_;
    map<string, unique_ptr<Interface>> interfaces_;
    map<string, Subscriber*> subscribers_;
    int connectionIndex_;
    bool settingsSaved;
    const QString settingsFile_;
    QTimer timer_;
    Files bags_;
    QString m_buildInfo;
};

#endif // BACKEND_H
