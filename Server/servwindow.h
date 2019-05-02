#ifndef SERVWINDOW_H
#define SERVWINDOW_H

#include <QtWidgets>
#include <QObject>
#include <QtNetwork>

class servWindow   : public QObject
{
    Q_OBJECT

    public:
        servWindow();
        void sendToAll(const QString &message);
        void shutdown();
        ~servWindow();

    private slots:
        void newConnexion();
        void dataReceived();
        void disconnectClient();

    private:
       // QLabel *stateServ;
      //  QPushButton *leaveButton;

        QTcpServer *serv;
        QList<QTcpSocket *> clients;
        quint16 lengthMessage;
};

#endif // SERVWINDOW_H
