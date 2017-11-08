#include "task.h"

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
