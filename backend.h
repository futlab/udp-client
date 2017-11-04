#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QQmlListProperty>
#include <QUdpSocket>


class Task : public QObject
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString log READ log NOTIFY logChanged)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(bool useRos READ useRos WRITE setUseRos NOTIFY useRosChanged)
    Q_PROPERTY(QString command READ command WRITE setCommand NOTIFY commandChanged)
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)

public:
    Task(QObject *parent = nullptr) : QObject(parent) {}
    Task(const QString &name, bool userRos, const QString &command, QObject *parent = nullptr);
    enum State { Stop, Wait, Active, Error };
    QString name() const { return name_; }
    QString log() const { return log_; }
    void clearLog() { log_ = ""; }
    void logLine(const QStringRef &ref);
    void logLine(const QString &ref);
    State state() const { return state_; }
    void setState(State a);
    QString command() const { return command_; }
    bool useRos() const { return useRos_; }
    QString id() const { return id_; }

public slots:
    void setName(QString name);
    void setCommand(QString command);
    void setUseRos(bool useRos);
    void setId(QString id);

signals:
    void nameChanged(QString name);
    void logChanged(QString log);
    void stateChanged(bool state);
    void commandChanged(QString command);
    void useRosChanged(bool useRos);

    void idChanged(QString id);

private:
    QString name_, log_, command_, id_;
    State state_;
    bool useRos_;
};

class BackEnd;

class Connection : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QString listenIf READ listenIf WRITE setListenIf NOTIFY listenIfChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(int listenPort READ listenPort WRITE setListenPort NOTIFY listenPortChanged)
    Q_PROPERTY(QStringList launchFiles READ launchFiles NOTIFY launchFilesChanged)
    QString name_, address_, listenIf_;
    int port_, listenPort_;
    QStringList launchFiles_;
    QUdpSocket socket_;
    BackEnd * const backend_;
    void bind();

public:
    Q_INVOKABLE void ping();
    Q_INVOKABLE void queryList();
    Q_INVOKABLE void launch(Task *task);
    Connection(QObject *parent = nullptr);
    Connection(const QString &name, const QString &address, int port, const QString &listenIf, int listenPort, BackEnd *parent);
    QString name() const { return name_; }
    QString address() const { return address_; }
    int port() const { return port_; }
    int listenPort() const { return listenPort_; }
    QString listenIf() const { return listenIf_; }
    QStringList launchFiles() const { return launchFiles_; }

public slots:
    void setName(QString name);
    void setAddress(QString address);
    void setPort(int port);
    void setListenPort(int listenPort);
    void read();
    void setListenIf(QString listenIf);

signals:
    void nameChanged(QString name);
    void addressChanged(QString address);
    void portChanged(int port);
    void listenPortChanged(int listenPort);
    void pingOk(QString remotIf, QString host);
    void listenIfChanged(QString listenIf);
    void launchFilesChanged(QStringList launchFiles);
};


class BackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString targetAddress READ targetAddress WRITE setTargetAddress NOTIFY targetAddressChanged)
    Q_PROPERTY(QQmlListProperty<Task> tasks READ tasks NOTIFY tasksChanged)
    Q_PROPERTY(QQmlListProperty<Connection> connections READ connections NOTIFY connectionsChanged)
    Q_PROPERTY(QStringList interfaces READ interfaces NOTIFY interfacesChanged)
public:
    Q_INVOKABLE void removeConnection(int idx);
    Q_INVOKABLE int appendConnection(const QString &name, const QString &address, int port, const QString &listenIf, int listenPort);
    Q_INVOKABLE void removeTask(int idx);
    Q_INVOKABLE int appendTask(const QString &name, bool userRos, const QString &command);
    explicit BackEnd(QObject *parent = nullptr);
    ~BackEnd();

    QString targetAddress();
    Task *taskById(const QStringRef &id);
    QQmlListProperty<Task> tasks();
    QQmlListProperty<Connection> connections();
    QStringList interfaces() const;

public slots:
    void setTargetAddress(const QString &targetAddress);

signals:
    void targetAddressChanged();    
    void tasksChanged(QQmlListProperty<Task> tasks);
    void connectionsChanged(QQmlListProperty<Connection> connections);
    void interfacesChanged(QStringList interfaces);
private:
    static void appendConnection(QQmlListProperty<Connection>* list, Connection *c);
    static void clearConnections(QQmlListProperty<Connection>* list);
    static Connection *connection(QQmlListProperty<Connection>* list, int i);
    static int connectionCount(QQmlListProperty<Connection>* list);

    static void appendTask(QQmlListProperty<Task>* list, Task *c);
    static void clearTasks(QQmlListProperty<Task>* list);
    static Task *task(QQmlListProperty<Task>* list, int i);
    static int taskCount(QQmlListProperty<Task>* list);

    QString targetAddress_;
    QVector<Task *> tasks_;
    QVector<Connection *> connections_;
    QStringList m_interfaces;
};

#endif // BACKEND_H
