/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakoriko <nakoriko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:33:12 by nakoriko          #+#    #+#             */
/*   Updated: 2026/04/02 15:44:41 by nakoriko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//NAT
#include "../../include/Server.hpp"
#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"
#include <iostream>

//joint to channel

void cmd_join (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing) {

	(void) trailing;
//1. Check params (se vuoo - inviare il messaggio)
	if(params.empty()) {
		client.sendMessage("461 " + client.getNickname() + " JOIN :Not enough parameters\r\n");
		return;
	}
//2. Get channel name (dal params)
	std::string channel_name = params[0];
	if(channel_name.empty() || channel_name[0] != '#') {
		client.sendMessage("476 " + client.getNickname() + " " + channel_name +  " :Bad channel name\r\n");
		return;
	}
//+ key constrol
	std::string key;
	if(params.size() > 1) {
		key = params[1];
	} else {
		key = "";
	}

//3. Check o create (server._channels)
	Channel *channel = server.getChannel(channel_name);
	// std::cout << "Debug: channel pointer " << channel << std::endl; 
	bool is_new = false;///check per capre, se creator - deve diventare un operatore

	if(!channel) {
		channel = new Channel(channel_name);
		//aggiungere dentro la mappa con tutti i channels 
		server.addChannel(channel_name, channel); 
		is_new = true;
	}
	
//6. Se il canale si crea - client diventa operatore
	if(is_new) {
		channel->addOperator(&client); //il creatuore diventa operatore
	}
//4. Check modes of channel (bools in channel) e se condizioni di client e channel corrispondono
	//++check che cliente non dentro canale gia
	if(channel->isMember(&client)) {
		client.sendMessage("443 " + client.getNickname() + " " + channel_name + " :is already on channel\r\n");
		return ;
	}
	//+i
	if(channel->isInviteOnly() && !channel->isInvited(client.getNickname())) {
		client.sendMessage("473 " + client.getNickname() + " " + channel_name + " :Cannot join channel (+i)\r\n");
		return ;
	}
	//+k
	if(!channel->checkKey(key)) {
		client.sendMessage("475 " + client.getNickname() + " " + channel_name + " :Cannot join channel (+k)\r\n");
		return ;
	}
	//+l
	if(!channel->canJoin()) {
		client.sendMessage("471 " + client.getNickname() + " " + channel_name + " :Cannot join channel (+l)\r\n");
		return ;
	}

//5. Add cliente in channel channel->addMember(&client) e addOperator(&client) - se e' nuovo
	//check che client ancora non e un membro
	channel->addMember(&client);



//9. Aggiornarre tutti ( a parte client esistente), che c'e un nuovo membro (channel->broadcast())
	std::string join_msg = ":" + client.getNickname() + " JOIN " + channel_name + "\r\n";
	if(!trailing.empty()) {
		join_msg = join_msg + trailing + "\r\n";
	}
	channel->broadcast(join_msg, &client); //client stesso escluso
	client.sendMessage(join_msg);	
	
//10. Inviare tema di canale al cliente
	if(channel->getTopic().empty()) {
		client.sendMessage("331 " + client.getNickname() + " " + channel_name + " :No topic is set\r\n");
	} else {
		client.sendMessage("332 " + client.getNickname() + " " + channel_name + " :" + channel->getTopic() + "\r\n");
	}

//11. Inviare elenco di member (forse no)
	std::string names;
	const std::map<std::string, Client*> &members = channel->getMembers();
	//const_iterator per const map;
	for (std::map<std::string, Client*>::const_iterator it = members.begin(); it != members.end(); it++) {
			// operatore ha '@' davanti
			if(channel->isOperator(it->second)) {
				names = names + "@" + it->first + " ";
			} else {
				names = names + it->first + " ";
			}
	}
	if(!names.empty()) {
		names.erase(names.size() - 1); //giust oper eliminare spazio alla fine
	}
	client.sendMessage("353 " + client.getNickname() + " " + channel_name + " :" + names + "\r\n");
	client.sendMessage("366 " + client.getNickname() + " " + channel_name + " :End of NAMES list\r\n");
	//7. Eliminare dal _invited (channel->removeFromInvited(client.getNickname)) - INVITE puo essere utilizzato finche non e stato fatto JOIN
	
	//!!! RFC vuole anche che non e in active bans (non abbiamo ancora pensato di ban)
}