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
    QVector<int> readTable(QString fileName);
    QVector<QVector<int> > readSBoxTable(QString fileName);
    QVector<quint64> encode(QString data, quint64 key);
    QVector<quint64> stringToQuint64Vector(QString data);
    QVector<QBitArray> makeKeys(quint64 key);
    QString decode(QVector<quint64> words, quint64 key);
    QBitArray doPermutation(QBitArray array, QVector<int> table);
    QBitArray doSBox(QBitArray array, QVector<QVector<int> > fileName);
    quint64 bitArrayToQuint(QBitArray array);
    QBitArray quintToArray(quint64 bits, int size=64);
    QBitArray shiftL(QBitArray array, int n);
    QBitArray shiftR(QBitArray array, int n);
    QBitArray concatBitArray(QBitArray a, QBitArray b);
    QString quintToString(quint64 valor);
    QString bitArrayToString(QBitArray a);
    QString quint64ToStringHex(quint64 a);


private:

    QVector<int> keyShift;
    QVector<int> expandWord;
    QVector<int> permute1;
    QVector<int> permute2;
    QVector<int> permuteKey1;
    QVector<int> permuteKey2;
    QVector<int> permuteSbox;
    QVector<QVector<QVector<int> > > sBox;
};

#endif // DESQT_H
