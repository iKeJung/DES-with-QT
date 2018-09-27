#include "desqt.h"

DesQT::DesQT()
{

}

QVector <quint64> DesQT::encode(QString data, quint64 key)
{
    QVector <QBitArray> encoded;
    QVector<quint64> words = stringToQuint64Vector(data);

    QElapsedTimer timer;
    timer.start();
    QVector<QBitArray> keys = makeKeys(key);
    qDebug() << "The slow operation took" << timer.elapsed() << "milliseconds";
    qDebug() << "The slow operation took" << timer.nsecsElapsed() << "nanoseconds";

    for (int x = 0; x < words.size(); ++x) {        
        QBitArray bitWords = quintToArray(words[x]);


        QBitArray word = doPermutation(bitWords,TablesLocation::permute1);

        QBitArray left(word.size()/2);
        for (int x = 0; x < word.size()/2; ++x) {
            left[x] = word[x];
        }
        QBitArray right(word.size()/2);
        for (int x = word.size()/2; x < word.size(); ++x) {
            right[x-word.size()/2] = word[x];
        }
        for (int c = 0; c < 16; ++c) {

            QBitArray nextLeft = right;

            QBitArray expandedRight = doPermutation(right,TablesLocation::expandWord);

            expandedRight ^= keys[c];

            QBitArray sboxconcat;
            for (int i = 0; i < 8; ++i) {
                QBitArray sboxArray(6);
                for (int j = 0; j < 6; ++j) {
                    sboxArray[j] = expandedRight[(6*i)+j];
                }

                QBitArray sboxAnswer = sBox(sboxArray, TablesLocation::sBox[i]);

                sboxconcat = concatBitArray(sboxconcat,sboxAnswer);
            }

            sboxconcat = doPermutation(sboxconcat,TablesLocation::permuteSbox);

            right = left ^ sboxconcat;
            left = nextLeft;
        }
        word = concatBitArray(right,left);

        word = doPermutation(word, TablesLocation::permute2);
        encoded << word;
    }

    QVector<quint64> encodedQuint;
    for (int x = 0; x < encoded.size(); ++x) {
        encodedQuint << bitArrayToQuint(encoded[x]);
    }

    return encodedQuint;
}



QVector<QBitArray> DesQT::makeKeys(quint64 key)
{
    QVector<QBitArray> keys;
    QBitArray keyVector = quintToArray(key);
    //qDebug() << keyVector;
    QBitArray key56 = doPermutation(keyVector,TablesLocation::permuteKey1);
    //qDebug() << key56;
    QBitArray keyLeft(key56.size()/2);
    for (int x = 0; x < key56.size()/2; ++x) {
        keyLeft[x] = key56[x];
    }
    QBitArray keyRight(key56.size()/2);
    for (int x = key56.size()/2; x < key56.size(); ++x) {
        keyRight[x-key56.size()/2] = key56[x];
    }

    QFile file(TablesLocation::keyShift);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
           return keys;
    }

    QTextStream in(&file);
    QString line = in.readLine();

    while (!line.isNull()) {
        int n = line.toInt();
        keyLeft = shiftL(keyLeft,n);
        keyRight = shiftL(keyRight,n);
        //qDebug() << keyLeft << keyRight << n;
        QBitArray concat = concatBitArray(keyLeft,keyRight);
        keys << doPermutation(concat, TablesLocation::permuteKey2);

        line = in.readLine();
    }
    //qDebug() << "\nKEYS\n" << keys << "\n";



    return keys;
}

QString DesQT::decode(QVector<quint64> words, quint64 key)
{
    QVector <QBitArray> decoded;
    qDebug() << "DECODING " << words;
    QVector<QBitArray> keys = makeKeys(key);

    for (int x = 0; x < words.size(); ++x) {
        QBitArray bitWords = quintToArray(words[x]);
        //qDebug() << "Decoding" << bitWords;
        //qDebug() << "WORD: " << bitWords;

        QBitArray word = doPermutation(bitWords,TablesLocation::permute1);
        //qDebug() << "IP: " << word;

        QBitArray left(word.size()/2);
        for (int x = 0; x < word.size()/2; ++x) {
            left[x] = word[x];
        }
        QBitArray right(word.size()/2);
        for (int x = word.size()/2; x < word.size(); ++x) {
            right[x-word.size()/2] = word[x];
        }
        for (int c = 15; c >= 0; --c) {

            QBitArray nextLeft = right;

            QBitArray expandedRight = doPermutation(right,TablesLocation::expandWord);

            //qDebug() << "E(R):" << expandedRight;

            expandedRight ^= keys[c];

            //qDebug() << "K + E(R):" << expandedRight;

            QBitArray sboxconcat;
            for (int i = 0; i < 8; ++i) {
                QBitArray sboxArray(6);
                for (int j = 0; j < 6; ++j) {
                    sboxArray[j] = expandedRight[(6*i)+j];
                }
                //qDebug() << "Sbox Array:" << sboxArray;

                QBitArray sboxAnswer = sBox(sboxArray, TablesLocation::sBox[i]);

                //qDebug() << sboxAnswer;
                sboxconcat = concatBitArray(sboxconcat,sboxAnswer);
            }

            //qDebug() << "S(B):" << sboxconcat;
            sboxconcat = doPermutation(sboxconcat,TablesLocation::permuteSbox);
            //qDebug() << "F S(B):" << sboxconcat;

            right = left ^ sboxconcat;

            //qDebug() << "L" << left;
            //qDebug() << "F" << sboxconcat;
            //qDebug() << "L + f(R,K)" << right;
            left = nextLeft;
        }
        word = concatBitArray(right,left);
        //qDebug() << "RL" << word;

        word = doPermutation(word, TablesLocation::permute2);
        //qDebug() << "IP-1" << word;
        decoded << word;
    }

    qDebug() << "DECODED ANSWER :" << decoded << "\n";
    QString decodedText = "";
    for (int x = 0; x < decoded.size(); ++x) {
        QBitArray temp = decoded[x];
        decodedText.append(quintToString(bitArrayToQuint(temp)));
    }

    return decodedText;
}

QBitArray DesQT::doPermutation(QBitArray array, QString fileName)
{
    //qDebug() << "OPENING FILE " << fileName;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug() << "ERROR - file not found.";
            return QBitArray();
    }

    QTextStream in(&file);
    QString line = in.readLine();
    QVector<int> table;
    while (!line.isNull()) {
       //qDebug() << line;
       QStringList list = line.split(" ");
       for (int x = 0; x < list.size(); ++x) {
           bool ok;
           table << list[x].toInt(&ok);
           if(!ok){
               qDebug() << "ERROR - impossible conversion to integer.";
           }
       }
       line = in.readLine();

    }
    //qDebug() << table << table.size();
    file.close();
    QBitArray newArray(table.size());
    for (int x = 0; x < table.size(); ++x) {
        if(table[x]-1>=array.size()){
            qDebug() << "ERROR - invalid values in transition table.";
        }
        newArray[x] = array[table[x]-1];
    }
    //qDebug() << newArray;
    return newArray;

}

QBitArray DesQT::sBox(QBitArray array, QString fileName)
{
    //qDebug() << "OPENING FILE " << fileName;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "ERROR - file not found.";
        return QBitArray();
    }

    QTextStream in(&file);
    QString line = in.readLine();
    QVector<QVector<int>> table;
    while (!line.isNull()) {
       //qDebug() << line;
       QStringList list = line.split(" ");
       QVector<int> row;
       for (int x = 0; x < list.size(); ++x) {
           bool ok;
           row << list[x].toInt(&ok);
           if(!ok){
               qDebug() << "ERROR - impossible conversion to integer.";
           }
       }
       table << row;
       line = in.readLine();

    }
    //qDebug()<< "STable: " << table << table.size();
    file.close();
    QBitArray i(2);
    i[0] = array[0];
    i[1] = array [5];
    QBitArray j(4);
    j[0] = array[1];
    j[1] = array[2];
    j[2] = array[3];
    j[3] = array[4];
    //qDebug() << array;
    quint64 iInt = bitArrayToQuint(i);
    //qDebug() << iInt;
    quint64 jInt = bitArrayToQuint(j);
    //qDebug() << jInt;

    quint64 newValue = table[iInt][jInt];
    //qDebug() << "new Value:" << newValue;
    QBitArray newArray = quintToArray(newValue,4);

    return newArray;
}

QVector<quint64> DesQT::stringToQuint64Vector(QString data)
{
    QVector<quint64> words;
    //qDebug() << data;

    quint64 array = 0;

    for (int x = 0; x < data.size(); x++) {
        quint64 temp;
        uchar c = data.at(x).toLatin1();
        //qDebug() << "Letra: " << c;
        temp = c;
        //qDebug() << "Decimal:" << temp;
        int shift = (7-(x%8))*8;
        //qDebug() << "SHift" << shift;
        temp <<= shift;
        //qDebug() << temp;
        array |= temp;
        if(x%8 == 7){
            words << array;
            array = 0;
        }
    }
    if(words.size()*8 < data.size()){
        words << array;
    }

    return words;
}

quint64 DesQT::bitArrayToQuint(QBitArray array)
{
    quint64 a = 0;
    for (int x = 0; x < array.size(); ++x) {
        quint64 temp = array[x];
        temp = temp << (array.size()-x-1);
        //qDebug() <<  temp;
        a+=temp;
    }

    return a;
}

QBitArray DesQT::quintToArray(quint64 bits, int size)
{
    QBitArray array(size);

    for (int x = 0; x < size; ++x) {
        quint64 temp = bits >> (size-x-1);
        temp = 0x0000000000000001 & temp;
        //qDebug() << temp;
        if(temp){
            array[x] = true;
        }else{
            array[x] = false;
        }
    }

    return array;
}

QBitArray DesQT::shiftL(QBitArray array, int n)
{
    QBitArray temp(array.size());

    for (int x = 0; x < array.size(); ++x) {
        int pos = (x-n)%array.size();
        if (pos < 0){
            pos = array.size()+pos;
        }
        //qDebug() << pos;
        temp[pos] = array[x];
    }

    return temp;
}

QBitArray DesQT::shiftR(QBitArray array, int n)
{
    QBitArray temp(array.size());

    for (int x = 0; x < array.size(); ++x) {
        int pos = (x+n)%array.size();
        //qDebug() << pos;
        temp[pos] = array[x];
    }

    return temp;
}

QBitArray DesQT::concatBitArray(QBitArray a, QBitArray b)
{
    QBitArray temp(a.size()+b.size());

    for (int x = 0; x < a.size(); ++x) {
        temp[x] = a[x];
    }
    for (int x = 0; x < b.size(); ++x) {
        temp[x+a.size()] = b[x];
    }
    return temp;
}

QString DesQT::quintToString(quint64 valor)
{
    QString text = "";
    //int max = 64-8;
    for (int y = 0; y < 8; ++y) {
        int shift = y*8;
        quint64 temp = valor << shift;
        temp >>= 56;
        //qDebug() << "Valor " << temp;
        char c = temp;
        if (c != 0){
            text.append(QChar(c));
        }
    }

    //qDebug() << text;
    return text;
}

QString DesQT::bitArrayToString(QBitArray a)
{

}

QString DesQT::quint64ToStringHex(quint64 a)
{
    QString text = QString::number(a,16);
    while(text.size()<16){
        text.push_front(QChar('0'));
    }
    for (int x = 2; x < text.size(); x=x+3) {
        text.insert(x," ");
    }
    text.push_back(" ");
    return text;
}


