#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QString>

class Task : public QObject
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(bool useRos READ useRos WRITE setUseRos NOTIFY useRosChanged)
    Q_PROPERTY(QString command READ command WRITE setCommand NOTIFY commandChanged)
    Q_PROPERTY(QString launchFile READ launchFile WRITE setLaunchFile NOTIFY launchFileChanged)
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)

public:
    Task(QObject *parent = nullptr);
    Task(const QString &name, bool userRos, const QString &command,const QString &launchFile, QObject *parent = nullptr);
    ~Task();

    enum State { Stop, Wait, Active, Error };
    QString name() const { return name_; }
    void logLine(const QStringRef &ref);
    void logLine(const QString &line);
    State state() const { return state_; }
    void setState(State a);
    QString command() const { return command_; }
    bool useRos() const { return useRos_; }
    QString id() const { return id_; }
    QString launchFile() const { return launchFile_; }

public slots:
    void setName(QString name);
    void setCommand(QString command);
    void setUseRos(bool useRos);
    void setId(QString id);
    void setLaunchFile(QString launchFile);

signals:
    void nameChanged(QString name);
    void stateChanged(bool state);
    void commandChanged(QString command);
    void useRosChanged(bool useRos);
    void idChanged(QString id);
    void launchFileChanged(QString launchFile);
    void log(QString line);

private:
    QString name_, command_, launchFile_, id_;
    QStringList log_;
    State state_;
    bool useRos_;
};

#endif // TASK_H
