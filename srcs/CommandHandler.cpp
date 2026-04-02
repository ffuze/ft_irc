/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegl-in <adegl-in@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:41:09 by nakoriko          #+#    #+#             */
/*   Updated: 2026/04/02 21:11:56 by adegl-in         ###   ########.fr       */
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
	//1. Trovare il prefix (inizia dal ':', finische con lo spazio)
	//2. TRovare command (fino lo spazio sucessivo)
	//3. Trovare paramtri (dopo spazio, ma prima di :)
	//4. Trailing - tutto il resto dopo ':' - trailing
	ParsedCommand result;
	std::string line = row;
	size_t pos = 0;

	//prefix
	if(!line.empty() && line[0] == ':') {
		std::string clean_pref = trim(line.substr(1));
		pos = clean_pref.find(' ');
		if(pos == std::string::npos)
			return result; //prefix senza spazio -  error
		std::string pref = line.substr(1, pos - 1);
		result.prefix = trim(clean_pref.substr(0, pos));//saltiamo ':', posizione fin ' ';
		line = trim(clean_pref.substr(pos + 1));// contrinene tutto dopo il spazio;	
	} 

	//command
	pos = line.find(' ');
	if(pos == std::string::npos) {  //se non ci sono i spazi - tutta la line viene definita come command
		result.command = trim(line);
		return result; 
	}
	std::string com = line.substr(0, pos);
	result.command = trim(com);
	line = trim(line.substr(pos + 1));


	//dopo command - tutto sono i parametri ( a parte se c'e trailng)
	while(!line.empty()) {
		if(line[0] == ':') {
			result.trailing = line.substr(1); // tutta la linea e' trailing dopo ':'
			break ;
		}
		pos = line.find(' ');
		if(pos == std::string::npos) {
			result.params.push_back(trim(line)); // se spazi non ci sono - tutta la linea e un parametro
			break;
		}
		std::string param = line.substr(0, pos);
		result.params.push_back(trim(param)); //agggiungamo ogni parametro (fino spazio prossimo)
		line = trim(line.substr(pos + 1));
	}
	return result;
}
//PARAMS PARSING PER OGNI COMMANDO - DA FINIRE

// bool CommandHandler::isValidCommand(Server &server, Client &client, const ParsedCommand &cmd) {
	
// 	//INVITE
// 	if(cmd.command == "INVITE") {
// 		if(cmd.params.size() != 2)
// 			return false;
// 		if(!isValidNickname(cmd.params[0]))
// 			return false;
// 		if(!isValidChannelName(cmd.params[1]))
// 			return false;
// 	//JOIN
// 	} else if (cmd.command == "JOIN") {
// 		if(cmd.params.size() < 1 || cmd.params.size() > 2)
// 			return false;
// 		std::vector<std::string> channels;
// 		std::string row_channels  = cmd.params[0];
// 		size_t start = 0;
// 		size_t end;
// 		while(end = row_channels.find(',', start) != std::string::npos) {
// 			channels.push_back(row_channels.substr(start, end - start));
// 			start = end + 1;
// 		}
// 		channels.push_back(row_channels.substr(start));
// 		for (size_t i = 0; i < channels.size(); i++) {
// 			if(!isValidChannelName(channels[i]))
// 				return false;
// 		}
// 		//controllo di keys se esistono
// 		if(cmd.params.size() == 2) {
// 			std::vector<std::string> keys;
// 			std::string row_key = cmd.params[1];
// 			start = 0;
// 			while((end = row_key.find(',',start)) != std::string::npos) {
// 				keys.push_back(row_key.substr(start, end - start));
// 				start = end + 1;
// 			}
// 			keys.push_back(row_key.substr(start));
// 			if(keys.size() != 1 && keys.size() != channels.size())
// 				return false;
// 		}
// 	//KICK
// 	} else if (cmd.command == "KICK") {
// 		if(cmd.params.size() < 2)
// 			return false;
// 		std::vector<std::string> channels;
// 		std::vector<std::string> nicks;
		
		
// 		std::string row_channnel = cmd.params[0];
// 		std::string row_niks = cmd.params[1];
// 		size_t start = 0;
// 		size_t end;
// 		while((end = row_channnel.find(',')) != std::string::npos) {
// 			channels.push_back(row_channnel.substr(start, end - start));
// 			start = end + 1;
// 		}
// 		channels.push_back(row_channnel.substr(start));

// 		start = 0;
// 		while((end = row_niks.find(',')) != std::string::npos) {
// 			nicks.push_back(row_niks.substr(start, end - start));
// 			start = end + 1;
// 		}
// 		nicks.push_back(row_niks.substr(start));

// 		if(!channels.size() == 1 || nicks.size() != channels.size())
// 			return false;


// 	} else if (cmd.command == "MODE") {

// 	} else if (cmd.command == "NICK") {

// 	} else if (cmd.command == "PART") {

// 	} else if (cmd.command == "PING") {

// 	} else if (cmd.command == "PRIVMSG") {

// 	} else if (cmd.command == "QUIT") {

// 	} else if (cmd.command == "TOPIC") {

// 	} else if (cmd.command == "USER") {

// 	} else {
// 		return false;
// 	}
	
// }


void CommandHandler::execute(Server &server, Client &client, const std::string &raw) {
	ParsedCommand cmd = parse(raw); // la struttura

	if(cmd.command.empty()) {
		client.sendMessage("Error: empty command\r\n"); //!cambiare dopo al format di RFC "Error replies"
		return ;
	}
	
	// if(!isValidCommand(server, client, cmd)) { //!is Valid deve inviare e messaggi con i messggi giusti in format RFC
	// 	return ;
	// }

	//1. Creamo la mappa dei commandi per riconoscerli: [nome] -> [metodo] (una volta solo (per la prima chiamata
	// Argomenti per i metodi: server, client, params, trailing 
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

	//2. Cerchiamo commanda dentro map -> eseguiamo
	std::map<std::string, void(*)(Server&, Client&, const std::vector<std::string>&, 
		const std::string&)>::iterator it = commands.find(cmd.command);
	if(it == commands.end()) {
		return;
	}
	//eseguiamo il metodo che corrisponde a "NOME" di commanda dentro la mappa sopra
	if(it->first != "PASS" && it->first != "NICK" && it->first != "USER") {
		if(!client.isRegistered()) {
			client.sendMessage("You need to register\r\n");
			return;
		}
	}
	it->second(server, client, cmd.params, cmd.trailing);
	

	//SPIEGAZIONE:
	//commands - la mappa (sopra)
	//cmd - struttura (struct ParsedCommand dentro CommandHandler.hpp)
	//command - nome dentro strutt cmd 
}

