#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <QObject>
#include <QHostAddress>

class WordParser
{
    const QString data;
    const QChar *d, *e;
public:
    WordParser(const QByteArray &bytes) : data(bytes), d(data.data()), e(d + data.length()) {}
    WordParser(const QString &str) : data(str), d(data.data()), e(d + data.length()) {}
    QStringRef get(const QChar c = ' ') {
        if (d < e && *d <= ' ') d++;
        if (d >= e) return QStringRef();
        const QChar *f = d;
        while (d < e && *d > c) d++;
        return QStringRef(&data, f - data.data(), d - f);
    }
    QStringRef all() {
        if (d < e && *d <= ' ') d++;
        if (e > d && e[-1] < ' ') e--;
        if (d >= e) return QStringRef();
        return QStringRef(&data, d - data.data(), e - d);
    }
};

class Subscriber : public QObject
{
    Q_OBJECT
public:
    explicit Subscriber(QObject *parent = nullptr);
    void pushData(WordParser &data) { data_ = &data; emit dataReady();}
signals:
    void dataReady();
public:
    WordParser *data_;
    //inline
};

#endif // SUBSCRIBER_H
