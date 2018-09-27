#ifndef DESQT_H
#define DESQT_H

#include <QBitArray>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QDataStream>
#include <QMap>
#include <bitset>
#include <QVector>
#include <QElapsedTimer>

#include "tableslocation.h"

class DesQT
{
public:
    DesQT();

    QVector<quint64> encode(QString data, quint64 key);
    QVector<quint64> stringToQuint64Vector(QString data);
    QVector<QBitArray> makeKeys(quint64 key);
    QString decode(QVector<quint64> words, quint64 key);
    QBitArray doPermutation(QBitArray array, QString fileName);
    QBitArray sBox(QBitArray array, QString fileName);
    quint64 bitArrayToQuint(QBitArray array);
    QBitArray quintToArray(quint64 bits, int size=64);
    QBitArray shiftL(QBitArray array, int n);
    QBitArray shiftR(QBitArray array, int n);
    QBitArray concatBitArray(QBitArray a, QBitArray b);
    QString quintToString(quint64 valor);
    QString bitArrayToString(QBitArray a);
    QString quint64ToStringHex(quint64 a);
};

#endif // DESQT_H
