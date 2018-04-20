#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QString>
#include <QUdpSocket>

#include "task.h"

class BackEnd;
class Connection;

class Interface : public QObject
{
    Q_OBJECT
public:
    Interface(const QString &name);
    inline void write(const QByteArray &data, const QHostAddress &address, int port) { socket_.writeDatagram(data, address, port); }
    inline const QString &name() const { return name_; }
    const QHostAddress &address() const;
    bool bind(Connection *connection, int listenPort);
    void unbind();
    static QHostAddress parseAddr(const QString &name);
private:
    const QString name_;
    const QHostAddress address_;
    QUdpSocket socket_;
    Connection *active_;
private slots:
    void read();
};

class Connection : public QObject
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QString listenIf READ listenIf WRITE setListenIf NOTIFY listenIfChanged)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(int listenPort READ listenPort WRITE setListenPort NOTIFY listenPortChanged)
    Q_PROPERTY(QStringList launchFiles READ launchFiles NOTIFY launchFilesChanged)
    Q_PROPERTY(BackEnd *backend READ backend WRITE setBackend NOTIFY backendChanged)

public:
    Q_INVOKABLE void ping();
    Q_INVOKABLE void queryList();
    Q_INVOKABLE void launch(Task *task);
    Q_INVOKABLE void stop(Task *task);
    Q_INVOKABLE void kill(Task *task);

    Connection(QObject *parent = nullptr);
    Connection(const QString &name, const QString &address, int port, Interface *listenIf, int listenPort, BackEnd *parent);
    ~Connection();

    enum State { Active, Wait, Pinged, Error, Off };
    QString name() const { return name_; }
    QString address() const { return address_; }
    int port() const { return port_; }
    int listenPort() const { return listenPort_; }
    QString listenIf() const { return interface_->name(); }
    QStringList launchFiles() const { return launchFiles_; }
    inline State state() const { return state_; }
    BackEnd *backend() const { return backend_; }

public slots:
    void setName(QString name);
    void setAddress(QString address);
    void setPort(int port);
    void setListenPort(int listenPort);
    void onRead(const QByteArray &datagram, const QHostAddress &address);
    void setListenIf(QString listenIf);
    void bind();
    void unbind();
    void scan();
    void queryTasks();
    void setBackend(BackEnd *backend);

signals:
    void nameChanged(QString name);
    void addressChanged(QString address);
    void portChanged(int port);
    void listenPortChanged(int listenPort);
    void pingOk(QString remoteIf, QString host);
    void listenIfChanged(QString listenIf);
    void launchFilesChanged(QStringList launchFiles);
    void stateChanged(State state);
    void backendChanged(BackEnd *backend);

private:
    QString name_, address_;
    Interface *interface_;
    int port_, listenPort_;
    QStringList launchFiles_;
    BackEnd * backend_;
    State state_;
    void setState(State state);
    inline void write(const QString &data) { write(data.toLocal8Bit()); }
    void write(const QByteArray &data);
    void write(const QByteArray &data, const QHostAddress &address);
};

#endif // CONNECTION_H
