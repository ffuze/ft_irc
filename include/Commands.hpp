/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegl-in <adegl-in@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 14:40:10 by nakoriko          #+#    #+#             */
/*   Updated: 2026/04/02 20:47:52 by adegl-in         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

class Server;
class Client;
#include <string>
#include <vector>
#include <algorithm>

//prototipo uguale per tutti i commandi:
// Server, Client, vector di strings params, string trailing("messaggio"); 


//NAT (basic server commands)
void cmd_nick (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);
void cmd_pass (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);
void cmd_user (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);
void cmd_quit (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);
void cmd_ping (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);
// void cmd_cap (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);

//NAT (basic channel commands)
void cmd_join (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);
void cmd_part (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);
void cmd_privmsg (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);
void cmd_notice(Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);

//LEO (operator commands)
void cmd_invite (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);
void cmd_kick (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);
void cmd_mode (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);
void cmd_topic (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing);




//subject: You must be able to authenticate, set a nickname, a username, 
//join a channel,send and receive private messages using your reference client 
//---->commands from RFC 1459 (OFFICIAL IRC server)

//Basic commands (regular user can do) 
// NICK - set a nickName (goes first)
// PASS - authentication (secont)
// USER - set a username (just after frist 2)
// JOIN - join a channel;
// PRIVMSG - send and receive private message to channel or another client;

//Operator commands FROM SUBJECTS (just operator of channel can do) - LEO ()

//KICK  - cancel user from channel
//INVITE - invite user to channel
//TOPIC - change/read channel topic
//MODE - change channel modes(i, t, k,o,l)

//OTHER COMMANDS (basic ones) - NAT o LEO
// PART - logout of channel
// QUIT - logout of server(leave all channels)
//PING (+PONG?) - check the connection between client and server


#endif