#ifndef FILES_H
#define FILES_H

#include <vector>
#include <memory>
#include <QObject>
#include <QTimer>
#include <QAbstractListModel>
#include "subscriber.h"

class BackEnd;

class Files : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QString totalSize READ totalSize NOTIFY totalSizeChanged)
public:
    Q_INVOKABLE void removeAll() { remove("*.bag"); }
    Q_INVOKABLE void remove(const QString &name);
    enum FileRoles {
        NameRole = Qt::UserRole + 1,
        SizeRole,
        SizeStringRole
    };
    struct File {
        File(const QString &name, size_t size) : name(name), size(size) { }
        const QString name;
        const size_t size;
    };
    explicit Files(QObject *parent = nullptr);
    Files(BackEnd *backend, const QString &folder);
    int rowCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;
    int count() const { return list_.size(); }
    QString totalSize() const { return sizeToString(totalSize_); }
    static QString sizeToString(size_t size);
signals:
    void countChanged(int count);
    void totalSizeChanged(QString totalSize);

private slots:
    void timeout();
    void onData();
private:
    std::vector<File> list_;
    BackEnd *backend_;
    QTimer timer_;
    const QString id_, folder_;
    Subscriber subscriber_;
    size_t totalSize_;
};

#endif // FILES_H
