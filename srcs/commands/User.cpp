/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegl-in <adegl-in@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:33:01 by nakoriko          #+#    #+#             */
/*   Updated: 2026/04/02 20:55:44 by adegl-in         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//NAT
#include "../../include/Server.hpp"
#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"

//Set username e nickname

void cmd_user (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing) {

	(void) trailing;
	// server saves username + nickname -> send wecome message
// 	Command: USER
//                 [params] [ignored '0'] [ignored '*'] [non e obbligatorio]
//    Parameters: <username> <hostname>   <servername>   <realname> 
	if(client.isRegistered()) {
		client.sendMessage("462 " + client.getNickname() + " :You may not reregister\r\n");
		return;
	}

	if(params.size() < 3) {
		client.sendMessage("461 " + client.getNickname() + " USER :Not enough parameters\r\n");
		return;
	}
	client.setUsername(params[0]);
	server.checkRegistration(client); // se c'e il pass, nick e user - > inviare welcome message)
	
}