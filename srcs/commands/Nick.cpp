/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegl-in <adegl-in@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:32:51 by nakoriko          #+#    #+#             */
/*   Updated: 2026/04/03 12:32:53 by adegl-in         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"
#include "../../include/Channel.hpp"

void cmd_nick (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing) {

	(void) trailing;
	if(params.empty()) {
		client.sendMessage("461 " + client.getNickname() + " NICK :Not enough parameters\r\n");
		return;
	}

	std::string new_nick = params[0];
	if(new_nick.empty()) {
		client.sendMessage("432 " + client.getNickname() + " " + new_nick + " :Erroneus nickname\r\n");
	}
	if(server.isNickTaken(new_nick)) {
		client.sendMessage("433 " + client.getNickname() + " " + new_nick + " :Nickname is already in use\r\n");
		return ;
	}
	std::string old_nick = client.getNickname();
	client.setNickname(new_nick);
	if(!old_nick.empty()) {
		const std::map<std::string, Channel*> &channels = server.getAllChannels();
		for(std::map<std::string, Channel*>::const_iterator it = channels.begin(); it != channels.end(); it++) {
			Channel *channel = it->second;
			if(channel->isMember(&client)) {
				channel->updateNick(old_nick, new_nick);
				channel->broadcast(":" + old_nick + " NICK " + new_nick + "\r\n", &client);
			}
		}
	}
	server.checkRegistration(client);
}