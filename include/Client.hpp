/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakoriko <nakoriko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 11:48:55 by nakoriko          #+#    #+#             */
/*   Updated: 2026/03/31 17:28:18 by nakoriko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <queue>

class Client {
private:
	int _fd; //use in map of clients
	std::string _nickname;
	std::string _username;
	bool _registered; // autentification
	bool _pass_checked;// autentification
	std::string _read_buffer; //for read msgs by server
	std::string _write_buffer; // for write msgs
	std::queue<std::string> _pending_messages; // "fila di messaggi", to kkep the order(subject) 

public:
	Client(int fd);
	~Client();
	void setNickname(std::string &nickname);
	void setUsername(const std::string &username);
	void setPassChecked(bool value);
	void setRegistered(bool value);
	bool isRegistered() const;
	bool isPassChecked() const;

	std::string getNickname() const;
	std::string getUsername() const;
	int getFd() const; 

//for reading messages
	void receiveMessage(std::string &row_msg); //add to _read_buffer
	void parseMessages(); //parse readbuffer by \r\n -> put in _pending_messages;
	bool hasPendingMessage();//checks_pending_messages
	void extractFromPending(); // takes out from _pending_messages
	std::string getNextMessage();
//for sending messages

	void sendMessage(const std::string &msg); // put to write_buffer
	bool hasMessageToSend();// check _write_buffer
	std::string extractToSend(); // extract for send method and removes from write_buffer;

};

#endif