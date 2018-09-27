#ifndef TABLESLOCATION_H
#define TABLESLOCATION_H

#include <QString>
#include <QStringList>

class TablesLocation
{
    public:
        TablesLocation();

        static const QString path;
        static const QString expandWord;
        static const QString keyShift;
        static const QString permute1;
        static const QString permute2;
        static const QString permuteKey1;
        static const QString permuteKey2;
        static const QString permuteSbox;
        static const QStringList sBox;
};

#endif // TABLESLOCATION_H
