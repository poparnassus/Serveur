#include "prototype.h"



int main(void)
{
    #if defined (WIN32)
        WSADATA WSAData;
        int erreur = WSAStartup(MAKEWORD(2,2), &WSAData);
    #else
        int erreur = 0;
    #endif

    /* Socket et contexte d'adressage du serveur */
    SOCKADDR_IN sin;
    SOCKET sock;
    char buffer[32];

    socklen_t recsize = sizeof(sin);

    /* Socket et contexte d'adressage du client */
    SOCKADDR_IN csin;
    SOCKET csock;
    socklen_t crecsize = sizeof(csin);

    int sock_err;
    int nombre_de_caractere;


    /* Si les sockets Windows fonctionnent */
    if(!erreur)
    {
        /* Création d'une socket */
        sock = socket(AF_INET, SOCK_STREAM, 0);

        /* Si la socket est valide */
        if(sock != INVALID_SOCKET)
        {
            printf("La socket %d est maintenant ouverte en mode TCP/IP\n", sock);

            /* Configuration */
            sin.sin_addr.s_addr = htonl(INADDR_ANY);  /* Adresse IP automatique */
            sin.sin_family = AF_INET;                 /* Protocole familial (IP) */
            sin.sin_port = htons(PORT);               /* Listage du port */

            sock_err = bind(sock, (SOCKADDR*)&sin, recsize);
            if(sock_err!=0)
            {
                printf("Impossible d'ecouter ce port: %d \n",sock_err);
            }
            else
            {
                printf("Bind OK\n");

            }

            sock_err=99;
            while(sock_err!=0)// Boucle tant qu'une demande de session (SYN) tcp n'a pas été reçu
            {
                sock_err = listen(sock, 5);
                printf("Listen OK PORT: %d \n", PORT);


                // Acceptation de la demande
                printf("Patientez pendant que le client se connecte sur le port %d...\n", PORT);
                csock=accept(sock, (SOCKADDR*)&csin, &crecsize);
                if(sock_err==SOCKET_ERROR)
                {
                    printf("\nDesole, je ne peux pas accepter la session TCP du a l'erreur : %d \n",WSAGetLastError());
                    return EXIT_FAILURE;
                }
                else
                {
                    printf("Un client se connecte avec la socket %d de %s:%d \n", csock, inet_ntoa(csin.sin_addr), htons(csin.sin_port));

                }

                //while(1)
                //{
                    //RECEPTION DONNEES
                    nombre_de_caractere=recv(csock,buffer,32,0);
                    if(nombre_de_caractere==SOCKET_ERROR)
                    {
                        printf("Donnes non transmise \n");
                    }
                    else
                    {
                        buffer[nombre_de_caractere]=0;
                        printf("\nClient:  %s \n\n",buffer);
                    }
                //}


            }

            shutdown(csock, 2);
        }
        else
        {
            printf("erreur socket\n");
        }
             /* Fermeture de la socket client et de la socket serveur */
            printf("Fermeture de la socket client\n");
            closesocket(csock);
            printf("Fermeture de la socket serveur\n");
            closesocket(sock);
            printf("Fermeture du serveur terminee\n");
    }

    #ifdef WIN32
       WSACleanup();
    #endif
    //system("pause");
    return EXIT_SUCCESS;
}
