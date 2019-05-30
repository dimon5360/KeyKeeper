#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <QLabel>

#include <QDebug>

#include <QtSql/QSqlDatabase>
#include <QSqlQuery>


#include <QtNetwork/QtNetwork>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QObject>

#include "iostream"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleAuthButton();
    void handleLoginButton();
    void handleConnectButton();
    void newuser();

private:
    void CreateGUI();
    bool CreateDataBase();
    bool Authentification(QString login, QString pass);
    bool Registration(QString login, QString pass);
    void CreateNewWindow();

private:
    Ui::MainWindow *ui;
    QPushButton *confirmButton,*loginButton,*authButton;                         // confirm input
    QLineEdit *inputPassLineEdit, *inputLoginLineEdit;                           // for password, login input
    QLabel *outputText;

    QPushButton * connectButton;
    QLineEdit * getIpAddress, * getIpPort;

    QSqlDatabase db;
    QTcpServer *tcpServer;
};


#endif // MAINWINDOW_H
