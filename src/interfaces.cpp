#include <QNetworkInterface>

#include "interfaces.h"

Interfaces::Interfaces(QObject *parent) : QObject(parent), list_(get())
{
    connect(&timer_, SIGNAL(timeout()), this, SLOT(update()));
    timer_.start(5000);
}

QStringList Interfaces::list() const
{
    return list_;
}

void Interfaces::update()
{
    QStringList r = get();
    if (r == list_) return;
    list_ = r;
    emit listChanged(list_);
}

QStringList Interfaces::get()
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
            r.push_back(ip.toString() + ": " + i.name());
        }
    }
    return r;
}
