/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakoriko <nakoriko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 12:27:39 by nakoriko          #+#    #+#             */
/*   Updated: 2026/04/01 19:10:04 by nakoriko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "CommandHandler.hpp"
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>



Server::Server (int port, std::string password) : _port(port), _password(password), _running(false) {
// 1.socket(). "Communication between client and server has to be done via TCP/IP (v4 or v6)"
//AF_INET(Address Family Internet (IPv4), SOCK_STREAM(TCP), 0 (default protocol(TCP nel nostro caso))
	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(_server_fd < 0)
		throw std::runtime_error("socket() failed");

// 2.bind() //set socket to port and choose where to listen 
	struct sockaddr_in addr = {}; //standart structure for socket, mettiamo tutto a zero dentro la struttura
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_port); //host to network short, converte il numero dell porta al formato standart (unsigned short  di 16 bit)
	addr.sin_addr.s_addr = INADDR_ANY; // diciamo che deve ascoltare qualsiasi ip, cioe accesibile da local e internet

	if(bind(_server_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0) //2nd argument: bind vuole sockaddr, ma noi dobbiamo passaare IPv4 (sockaddr_in)
		throw std::runtime_error("bind() failed");

// 3. listen()  - mette il socket in waiting mode, mette tutti conessioni in fila, finche accept() non li accept
	if(listen(_server_fd, SOMAXCONN) < 0) //se socket gia chiuso  - fa throw; SOMAXCONN - quantita massima di "aspettatori" in fila (128-4096)
		throw std::runtime_error("listen() failed");
	
// NON blockin - fcntl() serve per far funzionare poll(), senza aspettare accept(), recv() e send()
	if(fcntl(_server_fd, F_SETFL, O_NONBLOCK) < 0) //cambiamo il flag per nostro socket per O_NONBLOCK(F_SETFL - constanta/commanda per settare flags)
		throw std::runtime_error("fcntl() failed");
	
	struct pollfd pfd; // standart structure from poll, we just fill fields,
	pfd.fd = _server_fd; // identifichiamo il nostro pfd by fd
	pfd.events = POLLIN; //solo ascoltiamo, perche il server socket fd solo acceta connesioni, non manda i dati. POLLOUT non serve
	pfd.revents = 0; //valore iniziale
	_pollfds.push_back(pfd); // push dentro nostro vector di pollfds

}



void Server::run() {
	_running = true;

	while(_running) {
	//poll() - si attiva se succede qualcosa(aggiugendo di nuovo fd o eventi)
	//prende il punattore su primo elemento di vector e controlla quantita di eventi
	//ritorna quantita di eventi (> 0 o < 0)
		int fd_count = poll(_pollfds.data(), _pollfds.size(), -1); //data() punta al primo elemento di vector, -1 - aspetta finche non succede qualcosa.  
		if (fd_count < 0) { //error or signal to stop
			if(errno == EINTR) // k di variabile globale errno. Se EINTR - chiamata interrota con un signale(CTR+C), possiamo continuare
				continue;
			break; // se error - usciamo
		}
		//se fd_count > 0 - facciamo check di all pollfds, interpretando eventi 
		for(size_t i = 0; i < _pollfds.size(); i++) {
			//check di real events (o 1 = POLLIN, o 4 = POLLOUT, o tuti i due)
			//per questo controlliamo i bit concrete (0 e 2), tramite &;
			if(_pollfds[i].revents & POLLIN) {
				//se real event su server fd - allora e una richiesta dal client
				if(_pollfds[i].fd == _server_fd) {
					acceptNewClient();
				}
				// altrimenti - dati dal Client (per leggere)
				else {
					handleClientRead(_pollfds[i].fd);
				}
			}
			//POLLOUT - la richiesta dal cliente per invio di dati
			if(_pollfds[i].revents & POLLOUT) {
				handleClientWrite(_pollfds[i].fd);
			}
		}
	}
}


Server::~Server() {
//1. Chiudere tutti i clients sockets ed eliiminare clienti
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		close(it->first);// chiudiamo fd di ogni client
		delete it->second; //chiama destructor del client 
	}
	_clients.clear(); // eliminiamo tuti i puntatori dentro map

//2. Chiudere server socket
	if(_server_fd >= 0) // controllo nel caso, secostruttore ha messo dentro exception prima di creare socket
		close(_server_fd);

//3. Chiudere tutti i canali
	for(std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++) {
		delete it->second; //*it->first si puliscono dentro destruttore channel
	}
	_channels.clear();
//4. Pulire _pollfds (si puliscono autmaticamente, perche e' vector)
}

void Server::acceptNewClient() {
//1.Sewrve struttura per tenere indirizio di client
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(client_addr);
//2. Riempiamo la struttura e prendiamo fd. accept() acetta conessione, riempie la struttura e restituisce il nuovo fd per comunicare)
	int client_fd = accept(_server_fd, (struct sockaddr*)&client_addr, &addr_len);
	if(client_fd < 0)
		return ;
	// NON blockin - fcntl() serve per far funzionare poll(), senza aspettare accept(), recv() e send()
	fcntl(client_fd, F_SETFL, O_NONBLOCK);

//3. Creamo un ogeto di cliente con il sup fd
	Client* client = new Client(client_fd);
	
	//4. Mettiamolo dentro la mappa di clienti con fd
	_clients[client_fd] = client;

//5. Mettiamo fd dentro pollfds
	struct pollfd pfd;
	pfd.fd = client_fd;
	pfd.events = POLLIN | POLLOUT; //essegniamo i bits di lettura e scrittura per poll ()
	pfd.revents = 0;
	_pollfds.push_back(pfd);
}

void Server::addChannel(const std::string &name, Channel *channel) {
	// std::cout << "Debug: addchannel()" << name << " ->" << channel << std::endl;
	_channels[name] = channel;
}
void Server::removeChannel(const std::string &name) {
	_channels.erase(name);
}




void Server::removeClient(int fd) {
//Client puo cdisconettersi in qualsiasi momento 
//-> bisogna di chiudere  corettamnte il socket e liberare il suo fd
	std::map<int, Client *>::iterator it = _clients.find(fd);
	if(it == _clients.end())
		return ;
	Client *client = it->second;
//1. Bisogna di eliminare anche da tutti i canali, in quale e presente
	// for (std::map<std::string, Channel*>::iterator chan_it = _channels.begin();
	// 	chan_it != _channels.end(); chan_it++) {
	// 		chan_it->second->removeMember(client); //Channel.cpp
	//}
//2. Dal polfds
	for(size_t i = 0; i < _pollfds.size(); i++) {
		if(_pollfds[i].fd == fd) {
			_pollfds.erase(_pollfds.begin() + i);
			break ;
		}
	}
//2.Dalla mappa
	delete client; //eliminiamo client*;
	_clients.erase(it); // canceliamo la row intera;

//3. Chiudiamo il socket
	close(fd);
}

//(poll() found un evento dal cliente)
//predniamo il messagio da leggere
void Server::handleClientRead(int fd) {
// std::cout << "Debug:handle client resd fd = : " << fd << std::endl;

//1. Identifichiamo il cliente
	std::map<int, Client*>::iterator it = _clients.find(fd);
	if(it == _clients.end()) //se fd di cliente e' stato gia chiuso, per sempio
		return ;

	Client *client = it->second; //puntatore su cliente che corrisponde ad fd dentro la mappa
	
//2. Estriamo il messaggop dal socket
	char buffer[1024]; 
//2.1 Usiamo recv, per leggere dal socket (con standart flag per lettura - 0)
	int bytes = recv(fd, buffer, sizeof(buffer) - 1, 0); 
	if(bytes <= 0) { //se nel caso non c'e niente, vuol dire che fd gia' chiuso e bisogna di cancellarlo dalla mappa
		removeClient(fd); //remove fd from poll
		return ;
	}
	buffer[bytes] = '\0';

//3. Usiamo i metodi di class Client, per elaborare e eseguire
	std::string data(buffer); // salviamo per invio a medodi di CLient
	client->receiveMessage(data); // per mettere la string dentro _read_buffer
	client->parseMessages(); //parse by \r\n -> manda al _pending_messages()
	// std::cout << "Debug: pending messagges = " << client->hasPendingMessage() << std::endl;

//4. Elaboriamo (eseguiamo) ogni messaggio
	while(client->hasPendingMessage()) {
		std::string msg = client->getNextMessage();
		//La magia di eseguzione viene fatta qui:
		CommandHandler::execute(*this, *client, msg); 
	}
}


//write to client
void Server::handleClientWrite(int fd) {
	std::map<int, Client*>::iterator it = _clients.find(fd);
	if(it == _clients.end())
		return ;
	Client *client = it->second;
	if(client->hasMessageToSend()) {
		std::string msg = client->extractToSend();
		//send()tramite socket: prende client fd, puntatore e quantita di bytes
		send(fd, msg.c_str(), msg.size(), 0);
	}
}

Client* Server::getClient(const std::string &nick) {
	
	for(std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		if(it->second->getNickname() == nick)
			return it->second;
	} 
	return NULL;
}

Channel* Server::getChannel(const std::string &name) {
	std::map<std::string, Channel*>::iterator it = _channels.find(name);
	if(it == _channels.end())
		return NULL;
	return it->second;
}

const std::map<std::string, Channel*> &Server::getAllChannels () const {
	return _channels;
}


std::string Server::getPassword() const {
	return _password;
}

bool Server::isNickTaken(const std::string &nick) {
	for (std::map<int, Client*>::iterator it  = _clients.begin(); it != _clients.end(); it++) {
		if(it->second->getNickname() == nick)
			return true;
	}
	return false;
}

void Server::checkRegistration(Client &client) {
	if(client.isPassChecked() && !client.getNickname().empty() && !client.getUsername().empty()) 
	{
		client.setRegistered(true);
		client.sendMessage(":server 001 " + client.getNickname() + " :Welcome to the IRC server\r\n");
	}
}



