#ifndef INTERFACES_H
#define INTERFACES_H

#include <QObject>
#include <QString>
#include <QTimer>

class Interfaces: public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList list READ list NOTIFY listChanged)
public:
    Interfaces(QObject *parent = nullptr);
    QStringList list() const;
private slots:
    void update();
signals:
    void listChanged(QStringList list);
private:
    QTimer timer_;
    QStringList list_;
    QStringList get();
};

#endif // INTERFACES_H
