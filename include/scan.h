#ifndef SCAN_H
#define SCAN_H

#include <QObject>

class Scan : public QObject
{
    Q_OBJECT
public:
    explicit Scan(QObject *parent = nullptr);
    void loadPCD(const QString &fileName);
signals:

public slots:
};

#endif // SCAN_H
