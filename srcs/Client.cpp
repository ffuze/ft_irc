/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alek <alek@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:51:18 by nakoriko          #+#    #+#             */
/*   Updated: 2026/03/27 18:26:03 by alek             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client(int fd) : _fd(fd), _nickname(""), _username(""), _registered(false), _pass_checked(false), _read_buffer("") {}

Client::~Client() {}

void Client::setNickname(std::string &nickname) { _nickname = nickname; }
void Client::setUsername(std::string &username) { _username = username; }
void Client::setPassChecked(bool value) { _pass_checked = value; }
void Client::setRegistered(bool value) { _registered = value; }

std::string Client::getName() const { return _nickname; }
std::string Client::getUsername() const { return _username; }
int Client::getFd() const { return _fd; }

void Client::receiveMessage(std::string &row_msg) { _read_buffer += row_msg; }
void Client::parseMessages() {}
bool Client::hasPendingMessage() { return !_pending_messages.empty(); }
void Client::extractFromPending() { if (!_pending_messages.empty()) _pending_messages.pop(); }

void Client::sendMessage(std::string &msg) { _write_buffer += msg; }
bool Client::hasMessageToSend() { return !_write_buffer.empty(); }
void Client::extractToSend() { _write_buffer.clear(); }
