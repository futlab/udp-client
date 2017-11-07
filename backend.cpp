#include "backend.h"
#include <QNetworkInterface>
#include <QSettings>
#include <QUuid>

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

BackEnd::BackEnd(QObject *parent) : QObject(parent)
{
    QSettings settings("Futlab", "UDP client");
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
    QSettings settings("Futlab", "UDP client");
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

Task::Task(QObject *parent) : QObject(parent)
{
}

Task::Task(const QString &name, bool useRos, const QString &command, const QString &launchFile, QObject *parent) :
    QObject(parent), name_(name), command_(command), launchFile_(launchFile), state_(Stop), useRos_(useRos)
{
}

Task::~Task()
{
}

void Task::logLine(const QStringRef &ref)
{
    log_.append(ref);
    emit logChanged(log_);
}

void Task::logLine(const QString &ref)
{
    log_.append(ref);
    emit logChanged(log_);
}

void Task::setState(State s) {
    if (state_ == s) return;
    state_ = s;
    emit stateChanged(s);
}

void Task::setName(QString name)
{
    if (name_ == name) return;
    name_ = name;
    emit nameChanged(name_);
}

void Task::setCommand(QString command)
{
    if (command_ == command) return;
    command_ = command;
    emit commandChanged(command_);
}

void Task::setUseRos(bool useRos)
{
    if (useRos_ == useRos) return;
    useRos_ = useRos;
    emit useRosChanged(useRos_);
}

void Task::setId(QString id)
{
    if (id_ == id) return;
    id_ = id;
    emit idChanged(id_);
}

void Task::setLaunchFile(QString launchFile)
{
    if (launchFile_ == launchFile) return;
    launchFile_ = launchFile;
    emit launchFileChanged(launchFile_);
}

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
