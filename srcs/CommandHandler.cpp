/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegl-in <adegl-in@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:41:09 by nakoriko          #+#    #+#             */
/*   Updated: 2026/04/03 12:40:51 by adegl-in         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/CommandHandler.hpp"
#include "../include/Commands.hpp"

#include <string>

#include "Server.hpp"
#include "Client.hpp"
#include <iostream>

std::string CommandHandler::trim(const std::string &str) {
	size_t start = str.find_first_not_of(" \t");
	if (start == std::string::npos)
		return "";
	size_t end = str.find_last_not_of(" \t");
	return str.substr(start, end - start + 1);
}


ParsedCommand CommandHandler::parse(const std::string &row) {
	ParsedCommand result;
	std::string line = row;
	size_t pos = 0;

	if(!line.empty() && line[0] == ':') {
		std::string clean_pref = trim(line.substr(1));
		pos = clean_pref.find(' ');
		if(pos == std::string::npos)
			return result;
		std::string pref = line.substr(1, pos - 1);
		result.prefix = trim(clean_pref.substr(0, pos));
		line = trim(clean_pref.substr(pos + 1));
	} 

	pos = line.find(' ');
	if(pos == std::string::npos) {
		result.command = trim(line);
		return result; 
	}
	std::string com = line.substr(0, pos);
	result.command = trim(com);
	line = trim(line.substr(pos + 1));

	while(!line.empty()) {
		if(line[0] == ':') {
			result.trailing = line.substr(1);
			break ;
		}
		pos = line.find(' ');
		if(pos == std::string::npos) {
			result.params.push_back(trim(line));
			break;
		}
		std::string param = line.substr(0, pos);
		result.params.push_back(trim(param));
		line = trim(line.substr(pos + 1));
	}
	return result;
}

void CommandHandler::execute(Server &server, Client &client, const std::string &raw) {
	ParsedCommand cmd = parse(raw);

	if(cmd.command.empty()) {
		client.sendMessage("Error: empty command\r\n");
		return ;
	}
	
	static std::map<std::string, void(*)(Server&, Client&, const std::vector<std::string>&, const std::string&)> commands;
	
	static bool init = false;
	if(!init) {
		commands["INVITE"] = cmd_invite;
		commands["JOIN"] = cmd_join;
		commands["KICK"] = cmd_kick;
		commands["MODE"] = cmd_mode;
		commands["NICK"] = cmd_nick;
		commands["NOTICE"] = cmd_notice;
		commands["PART"] = cmd_part;
		commands["PASS"] = cmd_pass;
		commands["PING"] = cmd_ping;
		commands["PRIVMSG"] = cmd_privmsg;
		commands["QUIT"] = cmd_quit;
		commands["TOPIC"] = cmd_topic;
		commands["USER"] = cmd_user;
		init = true;
	}

	std::map<std::string, void(*)(Server&, Client&, const std::vector<std::string>&, 
		const std::string&)>::iterator it = commands.find(cmd.command);
	if(it == commands.end()) {
		return;
	}
	if(it->first != "PASS" && it->first != "NICK" && it->first != "USER") {
		if(!client.isRegistered()) {
			client.sendMessage("You need to register\r\n");
			return;
		}
	}
	it->second(server, client, cmd.params, cmd.trailing);
}
