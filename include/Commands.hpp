/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegl-in <adegl-in@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 14:40:10 by nakoriko          #+#    #+#             */
/*   Updated: 2026/04/03 12:26:24 by adegl-in         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

class Server;
class Client;

#include <string>
#include <vector>
#include <algorithm>

void cmd_nick (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);
void cmd_pass (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);
void cmd_user (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);
void cmd_quit (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);
void cmd_ping (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);

void cmd_join (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);
void cmd_part (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);
void cmd_privmsg (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);
void cmd_notice(Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);

void cmd_invite (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);
void cmd_kick (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);
void cmd_mode (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);
void cmd_topic (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);

#endif