/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alek <alek@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:51:18 by nakoriko          #+#    #+#             */
/*   Updated: 2026/03/31 18:00:35 by nakoriko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"


Client::Client(int fd) : _fd(fd), _registered(false), _pass_checked(false) {}

Client::~Client() {

}

void Client::setNickname(std::string &nickname) {
	_nickname = nickname;
}

void Client:: setUsername(const std::string &username) {
	_username = username;
}

void Client::setPassChecked(bool value) {
	_pass_checked = value;
}

void Client::setRegistered(bool value) {
	_registered = value;
}

bool Client::isRegistered() const {
	return _registered;
}

bool Client::isPassChecked() const {
	return _pass_checked;
}


std::string Client::getNickname() const{
	return _nickname;
}

std::string Client::getUsername() const {
	return _username;
}

int Client::getFd() const {
	return _fd;
}

void Client::receiveMessage(std::string &row_msg) {
	_read_buffer = _read_buffer + row_msg;
}

void Client::parseMessages() {
	//il protocollo IRC prevede \r\n come il divider dei messaggi
	//senza \r\n - il messaggio rimane dentro buffer, finche non va completato
	
	//check dentro buffer si trovano \r \n (il messaggio completo)
	//se trovato msg - lo mettiamo in fila, eliminando dal buffer
	//continuamo
	size_t pos;
	pos = _read_buffer.find("\r\n");
	while(pos != std::string::npos) {
		std::string msg = _read_buffer.substr(0, pos);
		_pending_messages.push(msg);
		_read_buffer.erase(0, pos+2); //eliminiamo insieme con \r\n
		pos = _read_buffer.find("\r\n");
	}
}

bool Client::hasPendingMessage() {
	//check inside our std::queue
	return !_pending_messages.empty();
}

void Client::extractFromPending() {
	if(!_pending_messages.empty())
		_pending_messages.pop();
}


//mettiamo il messaggio dentro buffer_write
void Client::sendMessage(const std::string &msg) {
	_write_buffer = _write_buffer + msg;
}

//se ci sono - li aggingiamo in POLLOUT dopo
bool Client::hasMessageToSend() {
	return!_write_buffer.empty();
}

//prendiamo il massaggio per inviare, ed eliminiamo dal buffer _write
std::string Client::extractToSend () {
	std::string data_to_send = _write_buffer;
	_write_buffer.clear();
	return data_to_send;
}

std::string Client::getNextMessage() {
	if(_pending_messages.empty())
		return "";
	std::string msg = _pending_messages.front();
	_pending_messages.pop();
	return msg; 
}
