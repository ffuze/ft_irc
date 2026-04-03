/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegl-in <adegl-in@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:33:28 by nakoriko          #+#    #+#             */
/*   Updated: 2026/04/03 12:34:40 by adegl-in         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"
#include "../../include/Channel.hpp"

void cmd_privmsg (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing) {
	
	if (params.empty() || trailing.empty()) {
		client.sendMessage("461 " + client.getNickname() + " PIVMSG :Not enough parameters\n\r");
		return;
	}
	std::string target = params[0];
	std::string message = trailing;

	if(target[0] == '#') {
		Channel* channel = server.getChannel(target);
		if(!channel) {
			client.sendMessage("403 " + client.getNickname() + " " + target + " :No such channel\n\r");
			return;
		}
		if(!channel->isMember(&client)) {
			client.sendMessage("404 " + client.getNickname() + " " + target + " :Cannot send to channel\n\r");
			return;
		}
		channel->broadcast(":" + client.getNickname() 
			+ " PRIVMSG " + target + " :" + message + "\r\n", &client);
	}

	else {
		Client *target_client = server.getClient(target);
		if(!target_client) {
			client.sendMessage("401 " + client.getNickname() + " " + target + " :No such nick/channel\n\r");
			return;
		}
		target_client->sendMessage(":" + client.getNickname() + " PRIVMSG " + target + " :" + message + "\r\n");
	}
}