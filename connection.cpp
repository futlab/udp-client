#include <QUuid>

#include "connection.h"
#include "backend.h"


void Connection::ping()
{
    setState(Wait);
    QString data = QString("ping %1").arg(listenPort_);
    QHostAddress address(address_);
    socket_.writeDatagram(data.toLocal8Bit(), address, port_);
}

void Connection::queryList()
{
    QString data = QString("list %1").arg(listenPort_);
    QHostAddress address(address_);
    socket_.writeDatagram(data.toLocal8Bit(), address, port_);
}

void Connection::launch(Task *task)
{
    task->clearLog();
    task->setState(Task::Wait);
    QString id = "#" + QUuid::createUuid().toString();
    task->setId(id);
    QString param = task->useRos() ? task->launchFile() : task->command();
    param.replace("#remote", address_);
    param.replace("#local", this->listenIf_);
    QString data = QString(task->useRos() ? "roslaunch %1 %2 %3" : "launch %1 %2 %3").arg(listenPort_).arg(id).arg(param);
    socket_.writeDatagram(data.toLocal8Bit(), QHostAddress(address_), port_);
}

void Connection::stop(Task *task)
{
    QString data = QString("stop %1").arg(task->id());
    socket_.writeDatagram(data.toLocal8Bit(), QHostAddress(address_), port_);
}

void Connection::kill(Task *task)
{
    QString data = QString("kill %1").arg(task->id());
    socket_.writeDatagram(data.toLocal8Bit(), QHostAddress(address_), port_);
}

Connection::Connection(QObject *parent) : QObject(parent), backend_(nullptr), state_(Active) {
}

Connection::Connection(const QString &name, const QString &address, int port, const QString &listenIf, int listenPort, BackEnd *parent) :
    QObject(parent), name_(name), address_(address), listenIf_(listenIf), port_(port), listenPort_(listenPort), backend_(parent)
{
    connect(&socket_, SIGNAL(readyRead()), SLOT(read()));
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
}

void Connection::setAddress(QString address)
{
    if (address_ == address) return;
    address_ = address;
    emit addressChanged(address_);
}

void Connection::setPort(int port)
{
    if (port_ == port) return;
    port_ = port;
    emit portChanged(port_);
}

void Connection::setListenPort(int listenPort)
{
    if (listenPort_ == listenPort) return;
    listenPort_ = listenPort;
    emit listenPortChanged(listenPort_);
    bind();
}

void Connection::bind()
{
    QHostAddress a = (listenIf_ == "Any") ? QHostAddress::Any : QHostAddress(listenIf_);
    if (socket_.state() != QAbstractSocket::UnconnectedState) socket_.abort();
    setState(socket_.bind(a, listenPort_) ? Active : Error);
}

void Connection::setState(State state)
{
    if (state == state_) return;
    state_ = state;
    emit stateChanged(state);
}

class WordParser
{
    const QString data;
    const QChar *d, *e;
public:
    WordParser(const QByteArray &bytes) : data(bytes), d(data.data()), e(d + data.length()) {}
    QStringRef get() {
        while (d < e && *d <= ' ') d++;
        if (d >= e) return QStringRef();
        const QChar *f = d;
        while (d < e && *d > ' ') d++;
        return QStringRef(&data, f - data.data(), d - f);
    }
    QStringRef all() {
        if (d < e && *d <= ' ') d++;
        if (d >= e) return QStringRef();
        return QStringRef(&data, d - data.data(), e - d);
    }
};

void Connection::read()
{
    QByteArray datagram;
    datagram.resize(socket_.pendingDatagramSize());
    QHostAddress address;
    socket_.readDatagram(datagram.data(), datagram.size(), &address);
    WordParser args(datagram);
    QStringRef head = args.get();
    if (head.isEmpty()) return;

    if (head == "OK") {
        setState(Pinged);
        emit pingOk(args.get().toString(), args.get().toString());
    } else if (head == "LIST") {
        launchFiles_.clear();
        QStringRef ref;
        while ((ref = args.get()) != "")
             launchFiles_.append(ref.toString());
        emit launchFilesChanged(launchFiles_);
    } else if (head == "START") {
        if (Task * task = backend_->taskById(args.get())) {
            task->clearLog();
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
    } else if (head.at(0) == '#') {
        if (Task *task = backend_->taskById(head))
            task->logLine(args.all());
    }
}

void Connection::setListenIf(QString listenIf)
{
    if (listenIf_ == listenIf)
        return;

    listenIf_ = listenIf;
    emit listenIfChanged(listenIf_);
    bind();
}
