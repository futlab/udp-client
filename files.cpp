#include <QUuid>
#include "files.h"
#include "backend.h"

void Files::remove(const QString &name)
{
    if (Connection *connection = backend_->activeConnection()) {
        connection->writeCmd("remove", QString("%1 %2").arg(folder_).arg(name));
    }
}

Files::Files(QObject *parent) :
    QAbstractListModel(parent), backend_(nullptr), id_(QUuid::createUuid().toString()), totalSize_(0)
{

}

Files::Files(BackEnd *backend, const QString &folder) :
    backend_(backend), id_(QUuid::createUuid().toString()), folder_(folder), totalSize_(0)
{
    connect(&timer_, SIGNAL(timeout()), this, SLOT(timeout()));
    timer_.start(5000);
    connect(&subscriber_, SIGNAL(dataReady()), this, SLOT(onData()));
}

int Files::rowCount(const QModelIndex &) const
{
    return (int)list_.size();
}

QVariant Files::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (size_t(row) > list_.size())
        return QVariant();
    const File & f = list_[row];
    switch (role) {
        case NameRole: return f.name;
        case SizeRole: return (int)f.size;
        case SizeStringRole: return sizeToString(f.size);
        default: return QVariant();
    }
}

QHash<int, QByteArray> Files::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[SizeRole] = "size";
    roles[SizeStringRole] = "sizeString";
    return roles;
}

QString Files::sizeToString(size_t size)
{
    const static std::vector<QString> units = {"B", "kB", "MB", "GB"};
    for (auto &u : units)
        if (size < 10000)
            return QString("%1 %2").arg(size).arg(u);
        else
            size /= 1024;
    return QString("%1 TB").arg(size);
}

void Files::timeout()
{
    backend_->subscribe(id_, &subscriber_);
    if (Connection *connection = backend_->activeConnection()) {
        connection->writeCmd("files", QString("%1 %2 1").arg(id_).arg(folder_));
    }
}

void Files::onData()
{
    WordParser &wp = *subscriber_.data_;
    QStringRef fileNameSize;
    beginResetModel();
    list_.clear();
    size_t listSize = list_.size(), total = 0;

    //QMap<QString, size_t> files;
    while ( (fileNameSize = wp.get()) != "") {
        auto a = fileNameSize.split(":");
        if (a.size() != 2)
            continue;
        size_t size;
        sscanf(a[1].toString().toStdString().c_str(), "%zu", &size);
        total += size;
        //files.insert(a[0].toString(), size);
        list_.emplace_back(a[0].toString(), size);
    }

    /*for (size_t i = 0; i < list_.size(); i++) {
        auto &item = list_[i];
    }*/

    endResetModel();

    if (listSize != list_.size())
        emit countChanged(list_.size());
    if (total != totalSize_) {
        totalSize_ = total;
        emit totalSizeChanged(sizeToString(totalSize_));
    }
    timer_.start(5000);
}
