#include "tableslocation.h"


const QString TablesLocation::path = QString("Tables\\");
const QString TablesLocation::expandWord = QString(path).append(QString("expandword.txt"));
const QString TablesLocation::keyShift= QString(path).append(QString("keyshift.txt"));
const QString TablesLocation::permute1= QString(path).append(QString("permute1.txt"));
const QString TablesLocation::permute2= QString(path).append(QString("permute2.txt"));
const QString TablesLocation::permuteKey1= QString(path).append(QString("permutekey1.txt"));
const QString TablesLocation::permuteKey2= QString(path).append(QString("permutekey2.txt"));
const QString TablesLocation::permuteSbox= QString(path).append(QString("permutesbox.txt"));
const QStringList TablesLocation::sBox = QStringList(std::initializer_list<QString> {
                                                     QString(path).append(QString("s1.txt")),
                                                     QString(path).append(QString("s2.txt")),
                                                     QString(path).append(QString("s3.txt")),
                                                     QString(path).append(QString("s4.txt")),
                                                     QString(path).append(QString("s5.txt")),
                                                     QString(path).append(QString("s6.txt")),
                                                     QString(path).append(QString("s7.txt")),
                                                     QString(path).append(QString("s8.txt"))
                                                     });

/*const QString TablesLocation::s1= QString(path).append(QString("s1.txt"));
const QString TablesLocation::s2= QString(path).append(QString("s2.txt"));
const QString TablesLocation::s3= QString(path).append(QString("s3.txt"));
const QString TablesLocation::s4= QString(path).append(QString("s4.txt"));
const QString TablesLocation::s5= QString(path).append(QString("s5.txt"));
const QString TablesLocation::s6= QString(path).append(QString("s6.txt"));
const QString TablesLocation::s7= QString(path).append(QString("s7.txt"));
const QString TablesLocation::s8= QString(path).append(QString("s8.txt"));
*/
TablesLocation::TablesLocation()
{

}
