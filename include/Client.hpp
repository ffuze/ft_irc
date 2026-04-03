/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegl-in <adegl-in@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 11:48:55 by nakoriko          #+#    #+#             */
/*   Updated: 2026/04/03 12:25:25 by adegl-in         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <queue>

class Client {
	private:
		int _fd;
		std::string _nickname;
		std::string _username;
		bool _registered;
		bool _pass_checked;
		std::string _read_buffer;
		std::string _write_buffer;
		std::queue<std::string> _pending_messages;

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

		void receiveMessage(std::string &row_msg);
		void parseMessages();
		bool hasPendingMessage();
		void extractFromPending();
		std::string getNextMessage();

		void sendMessage(const std::string &msg);
		bool hasMessageToSend();
		std::string extractToSend();
};

#endif