/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegl-in <adegl-in@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:33:01 by nakoriko          #+#    #+#             */
/*   Updated: 2026/04/03 12:36:09 by adegl-in         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"

void cmd_user (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing) {

	(void) trailing;
	if(client.isRegistered()) {
		client.sendMessage("462 " + client.getNickname() + " :You may not reregister\r\n");
		return;
	}

	if(params.size() < 3) {
		client.sendMessage("461 " + client.getNickname() + " USER :Not enough parameters\r\n");
		return;
	}
	client.setUsername(params[0]);
	server.checkRegistration(client);
	
}