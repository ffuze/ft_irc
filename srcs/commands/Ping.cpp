/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegl-in <adegl-in@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:34:07 by nakoriko          #+#    #+#             */
/*   Updated: 2026/04/03 12:34:17 by adegl-in         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"

void cmd_ping (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing) {
	(void) server;

	if(params.empty()) {
		client.sendMessage("PONG\r\n");
		return ;
	}
	std::string msg = "PONG";
	for(size_t i = 0; i < params.size(); i++) {
		msg = msg + " " + params[i];
	}
	if(!trailing.empty()) {
		msg = msg + " :" + trailing;
	}
	client.sendMessage(msg + "\r\n");
}