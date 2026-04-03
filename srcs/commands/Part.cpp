/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegl-in <adegl-in@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:33:45 by nakoriko          #+#    #+#             */
/*   Updated: 2026/04/03 13:43:31 by adegl-in         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"

void cmd_part (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing) {
	(void) server;
	(void) client;
	(void) params;
	(void) trailing;

	if(params.empty()) {
		client.sendMessage("461 " + client.getNickname() + " PART :Not enough parameters\n\r");
		return;
	}
	std::string channel_name = params[0];
	std::string msg = "Leaving";
	if(!trailing.empty())
		msg = trailing;
	
	Channel *channel = server.getChannel(channel_name);
	if(!channel) {
		client.sendMessage("403 " + client.getNickname() + " " + channel_name + " :No such channel\r\n");
		return;
	}
	if(!channel->isMember(&client)) {
		client.sendMessage("442 " + client.getNickname() + " " + channel_name + " :You're not on that channel\n\r");
		return;
	}
	std::string channel_msg = ":" + client.getNickname() + " PART " + channel_name + " :" + msg + "\r\n";
	channel->broadcast(channel_msg);
	channel->removeMember(&client);

	if(channel->getMembers().empty()) {
		server.removeChannel(channel_name);
		delete channel;
	}
}