/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegl-in <adegl-in@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:33:54 by nakoriko          #+#    #+#             */
/*   Updated: 2026/04/03 12:35:04 by adegl-in         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"

void cmd_quit (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing) {
	
	(void) params;
	std::string msg = "Quit";
	if(!trailing.empty())
		msg = trailing;
	std::map<std::string, Channel*> channels = server.getAllChannels();
	for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); it++) {
		Channel *channel = it->second;
		if(channel->isMember(&client)) {
			channel->broadcast(":" + client.getNickname() + " QUIT :" + msg + "\r\n");
			channel->removeMember(&client);
		}
	}
	server.removeClient(client.getFd());
}