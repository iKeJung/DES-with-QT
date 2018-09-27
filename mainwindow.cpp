#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qApp->setStyle("Fusion");

    cipher = new DesQT();
}

MainWindow::~MainWindow()
{
    delete cipher;

    delete ui;

}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "DES Encryption program details.","Program made as part of an assignment for\n"
                                                               "the \"Segurança em Tecnologia de Informação\"\n"
                                                               "course at Unisc.\n\n"
                                                               "Authors: Alessandra Helena Jandrey and Henrique Jung.\n"
                                                               "Professor: Ricardo M. Czekster.");
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open File"), "", tr("Text Files (*.txt)"));
    if(!fileName.isEmpty()){
        QFile file(fileName);
        if(!file.open(QFile::ReadOnly | QFile::Text)){
            QMessageBox::warning(this, "Warning", "File could not be oppened!");
        }
        QTextStream stream(&file);
        QString message = "";
        while(!stream.atEnd()){
            QString line = stream.readLine();
            message.append(line).append("\n");
        }

        putDecryptedText(message);

    }
}

void MainWindow::on_actionClose_triggered()
{
    this->close();
}

void MainWindow::putDecryptedText(QString a)
{
    ui->textDecrypted->selectAll();
    ui->textDecrypted->textCursor().removeSelectedText();

    if(ui->comboBox_Decrypted->currentIndex()==0){
        ui->textDecrypted->append(a);
        //qDebug() << a;
    }else{
        QVector<quint64> v = cipher->stringToQuint64Vector(a);
        QString text = "";
        for (int x = 0; x < v.size(); ++x) {
            text.append(cipher->quint64ToStringHex(v[x]));
        }
        ui->textDecrypted->append(text);
        //qDebug() << text;
    }

}

void MainWindow::putDecryptedText(QVector<quint64> a)
{
    ui->textDecrypted->selectAll();
    ui->textDecrypted->textCursor().removeSelectedText();

    if(ui->comboBox_Decrypted->currentIndex()==0){
        QString text = "";
        for (int x = 0; x < a.size(); ++x) {
            text.append(cipher->quintToString(a[x]));
        }
        ui->textDecrypted->append(text);
        //qDebug() << text;
    }else{
        QString text = "";
        for (int x = 0; x < a.size(); ++x) {
            text.append(cipher->quint64ToStringHex(a[x]));
        }
        ui->textDecrypted->append(text);
        //qDebug() << text;
    }
}

void MainWindow::putEncryptedText(QString a)
{
    ui->textEncrypted->selectAll();
    ui->textEncrypted->textCursor().removeSelectedText();

    if(ui->comboBox_Encrypted->currentIndex()==0){
        ui->textEncrypted->append(a);
        //qDebug() << a;
    }else{
        QVector<quint64> v = cipher->stringToQuint64Vector(a);
        QString text = "";
        for (int x = 0; x < v.size(); ++x) {
            text.append(cipher->quint64ToStringHex(v[x]));
        }
        ui->textEncrypted->append(text);
        //qDebug() << text;
    }
}

void MainWindow::putEncryptedText(QVector<quint64> a)
{
    ui->textEncrypted->selectAll();
    ui->textEncrypted->textCursor().removeSelectedText();
    QString text = "";
    if(ui->comboBox_Encrypted->currentIndex()==0){
        for (int x = 0; x < a.size(); ++x) {
            text.append(cipher->quintToString(a[x]));
            //qDebug() << cipher->quintToString(a[x]);
            //qDebug() << text << "\n";
        }
    }else{
        for (int x = 0; x < a.size(); ++x) {
            text.append(cipher->quint64ToStringHex(a[x]));
        }
    }
    //qDebug() << text;
    ui->textEncrypted->append(text);
}

void MainWindow::on_pushButton_Encrypt_clicked()
{
    QString t = ui->textDecrypted->toPlainText();
    QString k = ui->lineKey->text();

    quint64 key = 0;
    QString text;
    if(t.size()>0 && k.size()>0){
        if(ui->comboBox_Decrypted->currentIndex()==0){
            text = t;
        }else{
            text = stringHexToString(t.replace(" ",""));
        }
        if(ui->comboBox_Key->currentIndex()==0){
            key = cipher->stringToQuint64Vector(k)[0];
        }else{
            k.truncate(16);
            bool ok;
            qDebug() << k;
            key = k.toULongLong(&ok,16);
            if(!ok){
                QMessageBox::warning(this,"Warning","Conversion from hex to integer not possible with this key.");
            }
        }

        //qDebug() << text;
        QString report;
        QTextStream a(&report);
        QElapsedTimer timer;
        timer.start();
        ui->textBrowser->append("Encrypting text...");
        QVector<quint64> encoded = cipher->encode(text,key);
        a << "The encryption operation took " << timer.elapsed() << " milliseconds.";

        ui->textBrowser->append(report);
        putEncryptedText(encoded);


        QVector<quint64> tVector = cipher->stringToQuint64Vector(text);
        //qDebug() << encoded.size() << tVector.size();
        quint64 c = 0;
        if(encoded.size() != tVector.size()){
            return;
        }
        for (int x = 0; x < encoded.size(); ++x) {
            quint64 vXor = encoded[x]^tVector[x];
            QBitArray bits = cipher->quintToArray(vXor);
            for (int y = 0; y < bits.size(); y++) {
                if(bits[y]){
                    c++;
                }
            }
        }

        report = "";
        double value = c/(double)(encoded.size()*64);
        a << "Encryption strengh: " << value*100 <<"%.\n";
        ui->textBrowser->append(report);
    }
}

QString MainWindow::stringHexToString(QString t)
{
    QString text;
    for (int x = 0; x < t.size(); x=x+2) {
        QString aux = t.at(x);
        if(x+1<t.size()){
            aux.append(t.at(x+1));
        }
        bool ok;
        text.append(aux.toInt(&ok,16));
        if(!ok){
            QMessageBox::warning(this,"Warning","Conversion from hex to string not possible with this text.");
        }
    }
    return text;
}

void MainWindow::on_pushButton_Decrypt_clicked()
{
    QString t = ui->textEncrypted->toPlainText();
    QStringList lista = ui->textEncrypted->toHtml().split(";");
    QString a;
    for (int x = 11; x < lista.size()-1; ++x) {
        a.append(lista[x]);
    }
    a = a.remove(0,2);
    t = a;
    t = ui->textEncrypted->toPlainText();
    //qDebug() << lista << "\n";
    //qDebug() << t << "outro " << ui->textEncrypted->toPlainText();
    QString k = ui->lineKey->text();
    quint64 key = 0;
    QString text;
    if(t.size()>0 && k.size()>0){
        if(ui->comboBox_Encrypted->currentIndex()==0){
            text = t;
        }else{
            text = stringHexToString(t.replace(" ",""));
        }
        if(ui->comboBox_Key->currentIndex()==0){
            key = cipher->stringToQuint64Vector(k)[0];
        }else{
            k.truncate(16);
            bool ok;
            key = k.toULongLong(&ok,16);
            if(!ok){
                QMessageBox::warning(this,"Warning","Conversion from hex to integer not possible with this key.");
            }
        }
        QString report;
        QTextStream a(&report);
        QElapsedTimer timer;
        timer.start();
        ui->textBrowser->append("Decrypting text...");
        QString decripted = cipher->decode(cipher->stringToQuint64Vector(text),key);
        putDecryptedText(decripted);
        a << "The decrypting operation took " << timer.elapsed() << " milliseconds.\n";
        ui->textBrowser->append(report);


    }
}
