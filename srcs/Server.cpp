/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegl-in <adegl-in@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 12:27:39 by nakoriko          #+#    #+#             */
/*   Updated: 2026/04/03 12:58:58 by adegl-in         ###   ########.fr       */
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
#include <algorithm>
#include <cctype>

static std::string toLower(const std::string &str) {
	std::string result = str;
	std::transform(result.begin(), result.end(), result.begin(), ::tolower);
	return result;
}

Server::Server (int port, std::string password) : _port(port), _password(password), _running(false) {
	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(_server_fd < 0)
		throw std::runtime_error("socket() failed");

	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_port); 
	addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(_server_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0)
		throw std::runtime_error("bind() failed");

	if(listen(_server_fd, SOMAXCONN) < 0)
		throw std::runtime_error("listen() failed");
	
	if(fcntl(_server_fd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("fcntl() failed");
	
	struct pollfd pfd;
	pfd.fd = _server_fd;
	pfd.events = POLLIN | POLLHUP;
	pfd.revents = 0;
	_pollfds.push_back(pfd);
	std::cout << "Server flawlessly started on port " << port << std::endl;
}

void Server::run() {
	_running = true;

	while(_running) {
		int fd_count = poll(_pollfds.data(), _pollfds.size(), -1); 
		if (fd_count < 0) {
			if(errno == EINTR)
			{
				continue;
			}
			break;
		}
		for(size_t i = 0; i < _pollfds.size(); i++) {
			if(_pollfds[i].revents & POLLHUP) {
				removeClient(_pollfds[i].fd);
				continue;
			}
			
			if(_pollfds[i].revents & POLLIN) {
				if(_pollfds[i].fd == _server_fd) {
					acceptNewClient();
				}
				else {
					handleClientRead(_pollfds[i].fd);
				}
			}
			
			if(_pollfds[i].revents & POLLOUT) {
				handleClientWrite(_pollfds[i].fd);
			}
		}
	}
}


Server::~Server() {
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		close(it->first);
		delete it->second;
	}
	_clients.clear();

	if(_server_fd >= 0)
		close(_server_fd);

	for(std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++) {
		delete it->second;
	}
	_channels.clear();
}

void Server::acceptNewClient() {
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(client_addr);
	int client_fd = accept(_server_fd, (struct sockaddr*)&client_addr, &addr_len);
	if(client_fd < 0)
		return ;
	fcntl(client_fd, F_SETFL, O_NONBLOCK);

	std::cout << "[+] New client connected (fd: " << client_fd << ")" << std::endl;

	Client* client = new Client(client_fd);
	
	_clients[client_fd] = client;

	struct pollfd pfd;
	pfd.fd = client_fd;
	pfd.events = POLLIN | POLLOUT;
	pfd.revents = 0;
	_pollfds.push_back(pfd);
}

void Server::addChannel(const std::string &name, Channel *channel) {
	_channels[name] = channel;
}
void Server::removeChannel(const std::string &name) {
	_channels.erase(name);
}




void Server::removeClient(int fd) {
	std::map<int, Client *>::iterator it = _clients.find(fd);
	if(it == _clients.end())
		return ;
	Client *client = it->second;
	std::cout <<  "Debug: client:" << client->getNickname() << std::endl;
	for (std::map<std::string, Channel*>::iterator chan_it = _channels.begin();
		chan_it != _channels.end(); chan_it++) {
	}
	for(size_t i = 0; i < _pollfds.size(); i++) {
		if(_pollfds[i].fd == fd) {
			_pollfds.erase(_pollfds.begin() + i);
			break ;
		}
	}
	delete client;
	_clients.erase(it);
	close(fd);
}

void Server::handleClientRead(int fd) {
	std::map<int, Client*>::iterator it = _clients.find(fd);
	if(it == _clients.end())
		return ;

	Client *client = it->second;
	char buffer[1024]; 
	int bytes = recv(fd, buffer, sizeof(buffer) - 1, 0); 
	if(bytes <= 0) {
		removeClient(fd);
		return ;
	}
	buffer[bytes] = '\0';

	std::string data(buffer);
	client->receiveMessage(data);
	client->parseMessages();

	while(client->hasPendingMessage()) {
		std::string msg = client->getNextMessage();	
		std::cout << "[MSG] " << client->getNickname() << ": " << msg << std::endl;
		CommandHandler::execute(*this, *client, msg); 
	}
}

void Server::handleClientWrite(int fd) {
	std::map<int, Client*>::iterator it = _clients.find(fd);
	if(it == _clients.end())
		return ;
	Client *client = it->second;
	if(client->hasMessageToSend()) {
		std::string msg = client->extractToSend();
		send(fd, msg.c_str(), msg.size(), 0);
	}
}

Client* Server::getClient(const std::string &nick) {
	std::string nickLower = toLower(nick);
	for(std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		if(toLower(it->second->getNickname()) == nickLower)
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
	std::string nickLower = toLower(nick);
	for (std::map<int, Client*>::iterator it  = _clients.begin(); it != _clients.end(); it++) {
		if(toLower(it->second->getNickname()) == nickLower)
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



