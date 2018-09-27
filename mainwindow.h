#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBitArray>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QVector>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>
#include <QElapsedTimer>

#include "desqt.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    QString stringHexToString(QString);
    ~MainWindow();

private slots:
    void on_actionAbout_triggered();
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void on_pushButton_Encrypt_clicked();

    void on_pushButton_Decrypt_clicked();

private:
    void putDecryptedText(QString a);
    void putDecryptedText(QVector<quint64> a);
    void putEncryptedText(QString a);
    void putEncryptedText(QVector<quint64> a);


    Ui::MainWindow *ui;
    DesQT *cipher;
    int dado;
};

#endif // MAINWINDOW_H
