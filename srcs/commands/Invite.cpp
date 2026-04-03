#include "../../include/Server.hpp"
#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"

void cmd_invite (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing) {
	(void) trailing;

	if(params.size() < 2) {
		client.sendMessage("461 " + client.getNickname() + " INVITE :Not enough parameters\r\n");
		return;
	}

	std::string target_nick = params[0];
	std::string chanel_name = params[1];
	Channel *channel = server.getChannel(chanel_name);
	if(!channel) {
		client.sendMessage("403 " + client.getNickname() + " " + chanel_name +  " :No such channel\r\n");
		return;
	}
	if(!channel->isMember(&client)) {
		client.sendMessage("442 " + client.getNickname() + " " + chanel_name +  " :You're not on that channel\r\n");
		return;
	}
	if(channel->isInviteOnly() && !channel->isOperator(&client)) {
		client.sendMessage("482 " + client.getNickname() + " " + chanel_name +  " :You're not channel operator\r\n");
		return;
	}
	Client *target = server.getClient(target_nick);
	if( !target) {
		client.sendMessage("401 " + client.getNickname() + " " + target_nick +  " :No such nick\r\n");
		return;
	}
	if(channel->isMember(target)) {
		client.sendMessage("443 " + client.getNickname() + " " + target_nick + " " + chanel_name +  " :is already on channel\r\n");
		return;
	}
	channel->addInvited(target_nick);
	target->sendMessage(":" + client.getNickname() + " INVITE " + target_nick + " " + chanel_name + "\r\n");
	client.sendMessage("341 " + client.getNickname() + " INVITE " + target_nick + " " + chanel_name +  "\r\n");
}
	
