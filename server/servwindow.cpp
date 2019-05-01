#include "servwindow.h"
#include "error.h"
#include <QTcpServer>
#include <iostream>

using namespace std;

servWindow::servWindow()
{
    //Gestion server

    cout << "Lancement du server ... " << endl;

    serv = new QTcpServer(this);

 //   serv->listen(QHostAddress::Any, 5088);

    if (!serv->listen(QHostAddress::Any, 50887)) //démarrage server
    {
        cout << "ERROR while starting server at port 50887" << endl;
        exit(0);
    }
    else
    {
        //si le serveur démarre correctement

        cout << "Server démarré sur le port : 50887" << endl;
        cout << "initialisation des SLOTS ..." << endl;
        connect(serv, SIGNAL(newConnection()), this, SLOT(newConnexion()));
        cout << "SLOTS initialisés. " << endl;
    }

    lengthMessage = 0;
}

void servWindow::newConnexion()
{
    cout << "Connection d'un nouveau client..." << endl;
    sendToAll(tr("<em>Un nouveau client vient de se connecter</em>"));
    cout << "Création d'un tcp Socket..." << endl;
    QTcpSocket *newClient = serv->nextPendingConnection();
    cout << "Affectation du nouveau client à la liste. " << endl;
    clients << newClient; //tableau de clients hérité de Qlist

    cout << "Création des signaux asociés..." << endl;

    connect(newClient, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    connect(newClient, SIGNAL(disconnected()), this, SLOT(disconnectedClient()));

    cout << "Client connecté." << endl;
}

void    servWindow::dataReceived()
{
    cout << "Paquet(sous-paquet) reçu." << endl;
    cout << "Recherche du socket client..." << endl;
    //on détermine quel client envoie le paquet
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if(socket == 0)
    {
        cout << "socket non trouvé, annulation." << endl;
        return;
    }
    cout << "socket valide. analyse du paquet (sous-paquet)... " << endl;
    QDataStream in(socket);

    if(lengthMessage == 0)
    {
        if(socket->bytesAvailable() < (int)sizeof (quint16))
        {
            cout << "taille du message invalide. annulation." << endl;
            return;
         }

        in >> lengthMessage;
        cout << "taille du message trouvée." << endl;
    }

    if(socket->bytesAvailable() < lengthMessage)
    {
        cout << "message partiel, arrêt de la méthode." << endl;
        return; //si le message n'est pas entier, on stop la méthode
    }

    cout << "récupération du message ..." << endl;
    QString message;
    in >> message;
    cout << "message récupéré." << endl << "envoi du message aux utilisateurs..." << endl;

    sendToAll(message);

    cout << "message envoyé." << endl;

    lengthMessage = 0;
}

void    servWindow::disconnectClient()
{
    cout << "déconnetion d'un des clients." << endl;
    sendToAll(tr("<em>Un client vient de se déconnecter</em>"));

    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if(socket == 0)
    {
        cout << "socket invalide." << endl;
        return;
    }

    cout << "Suppression du client de la QList..." << endl;
    clients.removeOne(socket);
    cout << "suppression du socket..." << endl;
    socket->deleteLater();

    cout <<"Client déconnecté avec succès." << endl;
}

void    servWindow::sendToAll(const QString &message)
{
    //préparation du paquet
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0; //on réserve une place à la taille du paquet en écrivant 0
    out << message; //on ajoute le message à la suite
    out.device()->seek(0); //on se replace au début du paquet
    out << (quint16) (paquet.size() - sizeof (quint16)); //on écrase le 0

    //envoi du paquet
    for(int i=0; i< clients.size(); i++)
    {
        clients[i] -> write(paquet);
    }
}

servWindow::~servWindow()
{

}
