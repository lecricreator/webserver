#include "webserv.hpp"

//handle_client receives and sends a limited nbr of bytes
//TODO: creer une classe Connection qui contiendra toutes les informations dont chaque client a besoin (le fd du client, la requete http, un eventuel body, etc..)
int main() 
{
	//call une fonction qui va parse la config file dans une classe, return 1 si la fonction catch une erreur
	//
	//while(true)
	//	accepte les nouvelles connection (cree un objet Connection pour chaque nouveau client)
	//	read la data recue par chaque client et handle accordingly. les clients envoient leur data morceau par morceau, a chaque iteration de la loop tu regardes si t'as recu de la data, tu check si ya quelque chose a parser dans la data que t'as recu, si oui tu parses sinon tu mets la data de cote et en attendant la prochaine iteration ou tu verifieras a nouveau si t'as recu le bout de data manquant
	//	cree et envoie les reponses aux clients qui sont prets a recevoir une reponse (c a dire les clients dont le serveur a fini de parser la requete http)
	//	reset ou close les connections en fonction du parametre keepalive de la config file (copie de deepseek jsp ce que c'est reset la connection et jss pas sur de ce que le parametre keepalive fait)
	//	close les connections idle/timed out (copie de deepseek egalement)
	int port = 8080;
	int server_fd = create_listening_socket(port);
	if (server_fd == ERROR)
	  return FAILURE;
	while (true) {
	  int client_fd = accept_client(server_fd);
	  if (client_fd == ERROR)
	    return FAILURE;
	  handle_client(client_fd);
	}
	return SUCCESS;
}
