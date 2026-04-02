/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegl-in <adegl-in@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:32:51 by nakoriko          #+#    #+#             */
/*   Updated: 2026/04/02 21:17:19 by adegl-in         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//NAT
#include "../../include/Server.hpp"
#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"
#include "../../include/Channel.hpp"

//Set o change nickName of client

void cmd_nick (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing) {

	(void) trailing;

//1. Controllare che c'e il nick dentro params
	if(params.empty()) {
	client.sendMessage("461 " + client.getNickname() + " NICK :Not enough parameters\r\n");
	return;
	}

	//2. Check, che non e vuoto (parsing?)
	std::string new_nick = params[0];
	if(new_nick.empty()) {
		client.sendMessage("432 " + client.getNickname() + " " + new_nick + " :Erroneus nickname\r\n");
	}
//3. Check se disponibile (server.clients()), inviare un errore se si
	if(server.isNickTaken(new_nick)) {
		client.sendMessage("433 " + client.getNickname() + " " + new_nick + " :Nickname is already in use\r\n");
		return ;
	}
//4. SetNickName() (se lo cambia)
	std::string old_nick = client.getNickname();
	client.setNickname(new_nick);
//5. Inviar ela conferma (se lo cambiato + messagio per channels (se dentro channels))
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

//6. Check registrazione + username + nickname -> iviare welcome (RFC)
	server.checkRegistration(client); // se c'e il pass, nick e user - > inviare welcome message)


}