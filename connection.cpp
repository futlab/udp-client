#include <QUuid>

#include "connection.h"
#include "backend.h"
#include "subscriber.h"

void Connection::ping()
{
    setState(Wait);
    write(QString("ping %1").arg(listenPort_));
}

void Connection::queryList()
{
    write(QString("list %1").arg(listenPort_));
}

void Connection::launch(Task *task)
{
    task->setState(Task::Wait);
    QString id = "#" + QUuid::createUuid().toString();
    task->setId(id);
    QString param = task->useRos() ? task->launchFile() : task->command();
    param.replace("#remote", address_);
    param.replace("#local", interface_->address().toString());
    write(QString(task->useRos() ? "roslaunch %1 %2 %3" : "launch %1 %2 %3").arg(listenPort_).arg(id).arg(param));
}

void Connection::stop(Task *task)
{
    write(QString("stop %1 %2").arg(listenPort_).arg(task->id()));
}

void Connection::kill(Task *task)
{
    write(QString("kill %1 %2").arg(listenPort_).arg(task->id()));
}

Connection::Connection(QObject *parent) : QObject(parent), interface_(nullptr), backend_(nullptr), state_(Active) {
}

Connection::Connection(const QString &name, const QString &address, int port, Interface *listenIf, int listenPort, BackEnd *parent) :
    QObject(parent), name_(name), address_(address), interface_(listenIf), port_(port), listenPort_(listenPort), backend_(parent)
{
    bind();
}

Connection::~Connection()
{
}

void Connection::setName(QString name)
{
    if (name_ == name) return;
    name_ = name;
    emit nameChanged(name_);
    if (backend_) emit backend_->settingsChanged();
}

void Connection::setAddress(QString address)
{
    if (address_ == address) return;
    address_ = address;
    emit addressChanged(address_);
    if (backend_) emit backend_->settingsChanged();
}

void Connection::setPort(int port)
{
    if (port_ == port) return;
    port_ = port;
    emit portChanged(port_);
    if (backend_) emit backend_->settingsChanged();
}

void Connection::setListenPort(int listenPort)
{
    if (listenPort_ == listenPort) return;
    listenPort_ = listenPort;
    emit listenPortChanged(listenPort_);
    if (backend_) emit backend_->settingsChanged();
    bind();
}

void Connection::setState(State state)
{
    if (state == state_) return;
    state_ = state;
    emit stateChanged(state);
}

void Connection::writeCmd(const QString &cmd, const QString &body)
{
    write(QString("%1 %2 %3").arg(cmd).arg(listenPort_).arg(body));
}

void Connection::write(const QByteArray &data)
{
    QHostAddress address(address_);
    interface_->write(data, address, port_);
}

void Connection::write(const QByteArray &data, const QHostAddress &address)
{
    interface_->write(data, address, port_);
}

void Connection::onRead(const QByteArray &datagram, const QHostAddress &)
{
    WordParser args(datagram);
    QStringRef head = args.get();
    if (head.isEmpty()) return;

    if (head == "OK") {
        QString remoteIf = args.get().toString(), host = args.get().toString();
        if (state_ == Wait)
            setState(Pinged);
        emit pingOk(remoteIf, host);
    } else if (head == "LIST") {
        launchFiles_.clear();
        QStringRef ref;
        while ((ref = args.get()) != "")
             launchFiles_.append(ref.toString());
        emit launchFilesChanged(launchFiles_);
    } else if (backend_) {
        if (head == "START") {
            if (Task * task = backend_->taskById(args.get())) {
                task->setState(Task::Active);
            }
        } else if (head == "QUIT") {
            QStringRef id = args.get();
            if (Task *task = backend_->taskById(id)) {
                task->logLine(QString("Exit with code %1").arg(args.get().toString()));
                task->setState(Task::Stop);
            }
        } else if (head == "ERROR") {
            if (Task *task = backend_->taskById(args.get()))
            {
                task->logLine(QString("Exception: %1").arg(args.all().toString()));
                task->setState(Task::Error);
                //task->setError(true);
            }
        } else if (head == "TASKS") {
            QStringList ids;
            QVector<int> idxs;
            while (true) {
                QStringRef id = args.get();
                if (id == "") break;
                QStringRef cmd = args.get('\n');
                if (cmd == "") break;
                WordParser cp(cmd.toString());
                QStringRef c1 = cp.get();
                if (c1 == "") continue;
                bool useRos = c1 == "roslaunch";
                int i = backend_->taskIdxByCommand(useRos, useRos ? cp.get() : c1);
                if (i < 0) continue;
                ids.append(id.toString());
                idxs.append(i);
            }
            backend_->activateTasks(ids, idxs);
        } else if (head.at(0) == '#') {
            if (Task *task = backend_->taskById(head))
                task->logLine(args.all());
        } else if (head.at(0) == '@') {
            if (Subscriber *subscriber = backend_->findSubscriber(head))
                subscriber->pushData(args);
        }
    }
}

void Connection::setListenIf(QString listenIf)
{
    if (interface_ && interface_->name() == listenIf)
        return;
    if (interface_)
        interface_->unbind();
    if (listenIf == "") {
        interface_ = nullptr;
        emit listenIfChanged("");
        return;
    }
    interface_ = backend_->interfaceByName(listenIf);
    emit listenIfChanged(interface_->name());
    bind();
}

void Connection::bind()
{
    if (interface_)
        setState(interface_->bind(this, listenPort_) ? Active : Error);
}

void Connection::unbind()
{
    interface_->unbind();
    setState(Off);
}

void Connection::scan()
{
    setState(Active);
    QByteArray datagram = QString("ping %1").arg(listenPort_).toLocal8Bit();
    qint32 local = interface_->address().toIPv4Address();
    for (qint32 ip = (local & 0xFFFFFF00) + 1; ip < (local | 0xFF); ip++)
        if (ip != local) {
            QHostAddress address(ip);
            write(datagram, address);
        }
}

void Connection::queryTasks()
{
    write(QString("tasks %1").arg(listenPort_));
}

void Connection::setBackend(BackEnd *backend)
{
    if (backend_ == backend)
        return;

    backend_ = backend;
    emit backendChanged(backend_);
}

QHostAddress Interface::parseAddr(const QString &name)
{
    if (name == "Any")
        return QHostAddress::Any;
    int i = name.indexOf(':');
    if (i < 0) return QHostAddress(name);
    return QHostAddress(name.mid(0, i));
}

Interface::Interface(const QString &name) : name_(name), address_(parseAddr(name)), active_(nullptr)
{
    connect(&socket_, SIGNAL(readyRead()), SLOT(read()));
}

const QHostAddress &Interface::address() const
{
    return address_;
}

bool Interface::bind(Connection *connection, int listenPort)
{
    active_ = connection;
    auto state = socket_.state();
    if (state != QAbstractSocket::UnconnectedState)
        socket_.abort();
    return socket_.bind(address_, listenPort);
}

void Interface::unbind()
{
    if (socket_.state() != QAbstractSocket::UnconnectedState)
        socket_.abort();
}

void Interface::read()
{
    QByteArray datagram;
    datagram.resize(socket_.pendingDatagramSize());
    QHostAddress address;
    socket_.readDatagram(datagram.data(), datagram.size(), &address);
    if (active_)
        active_->onRead(datagram, address);
}
