#include "inc/mainwindow.h"
#include "ui_mainwindow.h"
#include "QtWebSockets/qwebsocket.h"

/* Main window constructor */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Build from 30.05.2019
    const QString appName = "KeyKeeper", appVersion = " v.0.0.2";
    ui->setupUi(this);

    /* Button to login input */
    loginButton = new QPushButton("Sign in", this);
    loginButton->setGeometry(QRect(QPoint(10, 20), QSize(100, 25)));
    connect(loginButton, SIGNAL(clicked()), this, SLOT(handleLoginButton()));

    /* Button to authorize input */
    authButton = new QPushButton("Register", this);
    authButton->setGeometry(QRect(QPoint(10, 50), QSize(100, 25)));
    connect(authButton, SIGNAL(clicked()), this, SLOT(handleAuthButton()));

    /* Create GUI */
    CreateGUI();

    /* Set a app name and version */
    setWindowTitle(appName + appVersion);
}

/**
* @brief Create new window
* @note Create a new window after autentification
*
* @param None
* @retval None
*/
void MainWindow::CreateNewWindow() {

    qDebug() << "New window is created.";

    /* connect button */
    loginButton->setText("Connect");
    disconnect(loginButton, SIGNAL(clicked()), this, SLOT(handleLoginButton()));
    connect(loginButton, SIGNAL(clicked()), this, SLOT(handleConnectButton()));

    /* Input IP address */
    inputLoginLineEdit->setText("");
    inputLoginLineEdit->setPlaceholderText("Enter destiny IP address");

    /* Input IP port */
    inputPassLineEdit->setText("");
    inputPassLineEdit->setPlaceholderText("Enter destiny IP port");
    inputPassLineEdit->setEchoMode(QLineEdit::Normal);    // hide an input data
}

/**
* @brief Create data base function
*
* @param None
* @retval Create data base result
*/
bool MainWindow::CreateDataBase() {

    QString dbPath = "C:\\Users\\Dmitry\\Documents\\QtProjects\\KeyKeeper\\KeyKeeper\\db\\db.db";

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);
    if(!db.open()) {
        qDebug() << "Database is not opened.";
        return false;
    }
    qDebug() << "Database is created.";
    return true;
}

/**
* @brief Autentification function
* @param sLogin Entered login by user
* @param sPass Entered password by user
* @retval Authentification result
*/
bool MainWindow::Authentification(QString sLogin, QString sPass) {

    QSqlQuery query;
    query.exec("SELECT id, login, password FROM users");

    while(query.next()) {
        QString id = query.value(0).toString();
        QString login = query.value(1).toString();
        QString password = query.value(2).toString();
        if(sLogin == login && sPass == password) {
            query.~QSqlQuery();
            return true;
        }
    }
    query.~QSqlQuery();
    return false;
}


/**
* @brief Registration function
* @param sLogin Entered login by user
* @param sPass Entered password by user
* @retval Registration result
*/
bool MainWindow::Registration(QString sLogin, QString sPass) {

    QSqlQuery query;
    query.exec("SELECT id, login, password FROM users");
    QString idStr;

    quint64 idMax = 0;
    while(query.next()) {
        quint64 Id = query.value(0).toULongLong();
        QString Login = query.value(1).toString();
        QString Password = query.value(2).toString();

        if(idMax < Id) {
            idMax = Id;
        }

        if(Login == sLogin && Password == sPass) {
            query.~QSqlQuery();
            return false;
        }
    }

    idStr = QString::number(idMax+1);

    query.exec("INSERT INTO users (id, login, password) values('"+idStr+"','"+sLogin+"','"+sPass+"')");
    query.~QSqlQuery();
    return true;
}

static QString authStatus = "Authentification status: ";

/**
* @brief Create GUI function
* @param None
* @retval None
*/
void MainWindow::CreateGUI() {

    /* Input login */
    inputLoginLineEdit = new QLineEdit(this);
    inputLoginLineEdit->setGeometry(QRect(QPoint(10, 85), QSize(200, 25)));
    inputLoginLineEdit->setPlaceholderText("Enter login please");

    /* Input password */
    inputPassLineEdit = new QLineEdit(this);
    inputPassLineEdit->setGeometry(QRect(QPoint(10, 115), QSize(200, 25)));
    inputPassLineEdit->setPlaceholderText("Enter password please");
    inputPassLineEdit->setEchoMode(QLineEdit::Password);    // hide an input data

    outputText = new QLabel(this);
    outputText->setGeometry(QRect(QPoint(120, 20), QSize(300, 25)));
    outputText->setText(authStatus);
}

void MainWindow::handleAuthButton() {

    QString login = inputLoginLineEdit->text();
    QString password = inputPassLineEdit->text();
    CreateDataBase();

    if(!Registration(login, password)) {
        outputText->setText(authStatus+"User already exists.");
        inputLoginLineEdit->setText("");
        inputPassLineEdit->setText("");
        return;
    }
    outputText->setText(authStatus+"Authentification succed");

    /* delete an autentification button */
    authButton->~QPushButton();
    this->CreateNewWindow();
}

void MainWindow::handleLoginButton() {

    QString login = inputLoginLineEdit->text();
    QString password = inputPassLineEdit->text();
    CreateDataBase();

    if(!Authentification(login, password)) {
        outputText->setText(authStatus+"Authentification failed");
        return;
    }
    outputText->setText(authStatus+"Authentification succed");

    /* delete an autentification button */
    authButton->~QPushButton();
    this->CreateNewWindow();
}

void MainWindow::handleConnectButton() {

    /* get IP address */
    QString ip = inputLoginLineEdit->text();
    /* get IP port */
    QString port = inputPassLineEdit->text();
    /* parse IP address */
    QStringList * ipList = new QStringList(ip.split("."));

    /* Uint32 presents IP address */
    unsigned int ipAddr = 0;
    /* Uint16 presents IP port */
    unsigned short ipPort = port.toUShort();

    /* convert to Uint32 */
    for(int i = 0; i < 4; i++) {
        ipAddr += (ipList->value(i).toUInt() << 8*(3-i));
    }

    /* debug info output */
    qDebug() << "IP address: " << ip;
    qDebug() << "IP port: " << ipPort;

    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newuser()));

    if(tcpServer->listen(QHostAddress::Any, ipPort)) {
        qDebug() << QString::fromUtf8("Server start");
    }

}

void MainWindow::newuser()
{
    qDebug() << QString::fromUtf8("У нас новое соединение!");
}

/* Main window destructor */
MainWindow::~MainWindow()
{
    delete ui;
}
