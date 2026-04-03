#include "../../include/Server.hpp"
#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"

void cmd_kick (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing) {
	(void) server;
	(void) client;
	(void) params;
	(void) trailing;

	if(params.size() < 2) {
		client.sendMessage("461 " + client.getNickname() + " KICK :Not enough parameters\r\n");
		return ;
	}

	std::string channel_name = params[0];
	std::string target_nick = params[1];
	Channel *channel = server.getChannel(channel_name);
	if(!channel) {
		client.sendMessage("403 " + client.getNickname() + " " + channel_name + " :No such channel\r\n");
		return ;
	}

	if(!channel->isMember(&client)) {
		client.sendMessage("442 " + client.getNickname() + " " + channel_name +  " :You're not on that channel\r\n");
		return;
	}

	if(!channel->isOperator(&client)) {
		client.sendMessage("482 " + client.getNickname() + " " + channel_name +  " :You're not channel operator\r\n");
		return;
	}

	Client *target = server.getClient(target_nick);
	if( !target) {
		client.sendMessage("401 " + client.getNickname() + " " + target_nick +  " :No such nick\r\n");
		return;
	}
	if(!channel->isMember(target)) {
		client.sendMessage("441 " + client.getNickname() + " " + target_nick + " " + channel_name +  " :They aren't on that channel\r\n");
		return;
	}

	std::string kick_msg = ":" + client.getNickname() + " KICK " + channel_name + " " + target_nick;
	if(!trailing.empty()) 
		kick_msg = kick_msg + " : " + trailing;
	kick_msg = kick_msg + "\r\n";
	channel->broadcast(kick_msg);
	channel->removeMember(target);
}
