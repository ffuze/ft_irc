/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegl-in <adegl-in@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:32:48 by nakoriko          #+#    #+#             */
/*   Updated: 2026/04/03 12:09:27 by adegl-in         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"

#include <iostream>

void cmd_pass (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing) {

	(void) trailing;

	if(client.isRegistered()) {
		client.sendMessage("462 " + client.getNickname() + " :You may not reregister\r\n");
		return;
	}

	if(params.empty()) {
		client.sendMessage("461 " + client.getNickname() + " PASS :Not enough parameters\r\n");
		return;
	}

	if(params[0] == server.getPassword()) {
		client.setPassChecked(true);
		server.checkRegistration(client);
	} else {
		client.sendMessage("464 " + client.getNickname() + " :Password incorrect\r\n");
	}
}