/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegl-in <adegl-in@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:41:55 by nakoriko          #+#    #+#             */
/*   Updated: 2026/04/03 12:25:47 by adegl-in         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDHANDLER_HPP
# define COMMANDHANDLER_HPP


# include <string>
# include <vector>
# include "../include/ParamValidator.hpp"

class Server;
class Client;

struct ParsedCommand {
	std::string prefix;
	std::string command;
	std::vector<std::string> params;
	std::string trailing;
};

class CommandHandler {
	public:
		static std::string trim(const std::string &str);
		static ParsedCommand parse(const std::string &row);
		static bool isValidCommand(const ParsedCommand &cmd);
		static void execute(Server &server, Client &client, const std::string &raw);
};

#endif
