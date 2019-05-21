#include "localkiwixserver.h"
#include "ui_localkiwixserver.h"
#include "kiwixapp.h"
#include <QDesktopServices>

LocalKiwixServer::LocalKiwixServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LocalKiwixServer)
{
    ui->setupUi(this);

    QFile styleFile(":/css/localServer.css");
    styleFile.open(QIODevice::ReadOnly);
    auto byteContent = styleFile.readAll();
    styleFile.close();
    QString style(byteContent);
    setStyleSheet(style);

    connect(ui->KiwixServerButton, SIGNAL(clicked()), this, SLOT(runOrStopServer()));
    connect(ui->OpenInBrowserButton, SIGNAL(clicked()), this, SLOT(openInBrowser()));
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost) {
            qInfo() << address.toString();
            m_ipAddress = address.toString();
            ui->IpAddress->setText(m_ipAddress);
            break;
        }
    }
    ui->OpenInBrowserButton->setVisible(false);
    ui->IpAddress->setVisible(false);
}

LocalKiwixServer::~LocalKiwixServer()
{
    delete ui;
}

void LocalKiwixServer::openInBrowser()
{
    QUrl url;
    url.setScheme("http");
    url.setHost(m_ipAddress);
    url.setPort(8181);
    QDesktopServices::openUrl(url);
}

void LocalKiwixServer::runOrStopServer()
{
    m_active = !m_active;
    if (m_active) {
        KiwixApp::instance()->runServer();
        ui->KiwixServerButton->setText("Stop Kiwix Server");
        QUrl url("qrc:/texts/kiwixServerRunningText.html");
        ui->KiwixServerText->setSource(url);
        ui->OpenInBrowserButton->setVisible(true);
        ui->IpAddress->setVisible(true);
    } else {
        KiwixApp::instance()->shutdownServer();
        ui->KiwixServerButton->setText("Start Kiwix Server");
        QUrl url("qrc:/texts/kiwixServerText.html");
        ui->KiwixServerText->setSource(url);
        ui->OpenInBrowserButton->setVisible(false);
        ui->IpAddress->setVisible(false);
    }
}