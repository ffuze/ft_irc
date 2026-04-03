/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegl-in <adegl-in@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:33:12 by nakoriko          #+#    #+#             */
/*   Updated: 2026/04/03 12:29:31 by adegl-in         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"
#include <iostream>

void cmd_join (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing) {

	(void) trailing;
	
	if(params.empty()) {
		client.sendMessage("461 " + client.getNickname() + " JOIN :Not enough parameters\r\n");
		return;
	}
	std::string channel_name = params[0];
	if(channel_name.empty() || channel_name[0] != '#') {
		client.sendMessage("476 " + client.getNickname() + " " + channel_name +  " :Bad channel name\r\n");
		return;
	}
	
	std::string key;
	if(params.size() > 1) {
		key = params[1];
	} else {
		key = "";
	}

	Channel *channel = server.getChannel(channel_name);
	bool is_new = false;
	if(!channel) {
		channel = new Channel(channel_name);
		server.addChannel(channel_name, channel); 
		is_new = true;
	}
	if(is_new) {
		channel->addOperator(&client);
	}
	if(channel->isMember(&client)) {
		client.sendMessage("443 " + client.getNickname() + " " + channel_name + " :is already on channel\r\n");
		return ;
	}
	if(channel->isInviteOnly() && !channel->isInvited(client.getNickname())) {
		client.sendMessage("473 " + client.getNickname() + " " + channel_name + " :Cannot join channel (+i)\r\n");
		return ;
	}
	if(!channel->checkKey(key)) {
		client.sendMessage("475 " + client.getNickname() + " " + channel_name + " :Cannot join channel (+k)\r\n");
		return ;
	}
	if(!channel->canJoin()) {
		client.sendMessage("471 " + client.getNickname() + " " + channel_name + " :Cannot join channel (+l)\r\n");
		return ;
	}
	channel->addMember(&client);

	std::string join_msg = ":" + client.getNickname() + " JOIN " + channel_name + "\r\n";
	if(!trailing.empty()) {
		join_msg = join_msg + trailing + "\r\n";
	}
	channel->broadcast(join_msg, &client);
	client.sendMessage(join_msg);	

	if(channel->getTopic().empty()) {
		client.sendMessage("331 " + client.getNickname() + " " + channel_name + " :No topic is set\r\n");
	} else {
		client.sendMessage("332 " + client.getNickname() + " " + channel_name + " :" + channel->getTopic() + "\r\n");
	}
	
	std::string names;
	const std::map<std::string, Client*> &members = channel->getMembers();
	for (std::map<std::string, Client*>::const_iterator it = members.begin(); it != members.end(); it++) {
			if(channel->isOperator(it->second)) {
				names = names + "@" + it->first + " ";
			} else {
				names = names + it->first + " ";
			}
	}
	if(!names.empty()) {
		names.erase(names.size() - 1);
	}
	client.sendMessage("353 " + client.getNickname() + " " + channel_name + " :" + names + "\r\n");
	client.sendMessage("366 " + client.getNickname() + " " + channel_name + " :End of NAMES list\r\n");
}
