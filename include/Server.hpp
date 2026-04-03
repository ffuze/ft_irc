/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegl-in <adegl-in@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 11:35:58 by nakoriko          #+#    #+#             */
/*   Updated: 2026/04/03 12:27:26 by adegl-in         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <map>
# include <vector>

# include "Client.hpp"
# include "Channel.hpp"

class Server {
	private:
		int _port;
		std::string _password;
		int _server_fd;
		std::map<int, Client*> _clients;
		std::map<std::string, Channel*> _channels;
		bool _running; //for stop()
		std::vector<struct pollfd> _pollfds;

	public:
		Server(int port, std::string password);
		~Server();
		
		void run();
		void acceptNewClient();
		void addChannel(const std::string &name, Channel *channel);
		void removeChannel(const std::string &name);
		void removeClient(int fd);
		void handleClientRead(int fd);
		void handleClientWrite(int fd);
		Client *getClient(const std::string &nick);
		Channel *getChannel(const std::string &name);
		const std::map<std::string, Channel*> &getAllChannels () const;
		std::string getPassword() const;
		bool isNickTaken(const std::string &nick);
		void checkRegistration(Client &client);
};

#endif