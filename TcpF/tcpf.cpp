#include "tcpf.h"

TcpFileServer::TcpFileServer(QWidget *parent)
    : QDialog(parent)
{
    totalBytes = 0;
    bytesReceived = 0;
    fileNameSize = 0;

    tcpServerConnection = nullptr;
    localFile = nullptr;

    serverProgressBar = new QProgressBar;
    serverProgressBar->setValue(0);

    serverStatusLabel = new QLabel(tr("伺服器就緒"));

    startButton = new QPushButton("接收");
    quitButton = new QPushButton("退出");

    buttonBox = new QDialogButtonBox;

    buttonBox->addButton(
        startButton,
        QDialogButtonBox::ActionRole
        );

    buttonBox->addButton(
        quitButton,
        QDialogButtonBox::RejectRole
        );

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(serverProgressBar);
    mainLayout->addWidget(serverStatusLabel);
    mainLayout->addStretch();
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

    setWindowTitle("接收檔案");

    connect(startButton, SIGNAL(clicked()),
            this, SLOT(start()));

    connect(quitButton, SIGNAL(clicked()),
            this, SLOT(close()));

    connect(&tcpServer, SIGNAL(newConnection()),
            this, SLOT(acceptConnection()));
}

TcpFileServer::~TcpFileServer()
{
    tcpServer.close();

    if (localFile != nullptr) {

        if (localFile->isOpen()) {
            localFile->close();
        }

        delete localFile;
        localFile = nullptr;
    }
}
void TcpFileServer::start() //開始監聽
{
    bytesReceived = 0;
    fileNameSize = 0;

    if (tcpServer.listen(QHostAddress::AnyIPv4, 16998)) {

        startButton->setEnabled(false);

        serverStatusLabel->setText(
            "監聽中....");

    } else {

        serverStatusLabel->setText(
            "監聽失敗：" + tcpServer.errorString());
    }
}
void TcpFileServer::acceptConnection() //接受新連線
{
    tcpServerConnection = tcpServer.nextPendingConnection();

    if (tcpServerConnection != nullptr) {

        serverStatusLabel->setText(
            "用戶端已連線");

        connect(tcpServerConnection, SIGNAL(disconnected()),
                tcpServerConnection, SLOT(deleteLater()));
    }
}

void TcpFileServer::updateServerProgress() //更新接收進度
{
    if (totalBytes > 0) {

        int progress = static_cast<int>(
            bytesReceived * 100 / totalBytes);

        serverProgressBar->setValue(progress);
    }
}
