
#ifndef TCPF_H
#define TCPF_H

#include <QDialog>
#include <QtWidgets>
#include <QtNetwork>
#include <QFile>
#include <QByteArray>

class TcpFileServer : public QDialog
{
    Q_OBJECT

public:
    explicit TcpFileServer(QWidget *parent = nullptr);
    ~TcpFileServer() override;

private slots:
    void start();
    void acceptConnection();
    void updateServerProgress();

private:
    QProgressBar *serverProgressBar;
    QLabel *serverStatusLabel;
    QPushButton *startButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;

    QTcpServer tcpServer;
    QTcpSocket *tcpServerConnection;

    qint64 totalBytes;
    qint64 bytesReceived;
    qint64 fileNameSize;

    QString fileName;
    QFile *localFile;

    QByteArray inBlock;
};

#endif // TCPF_H
