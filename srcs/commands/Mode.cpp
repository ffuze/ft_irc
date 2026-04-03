#include "../../include/Server.hpp"
#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"
#include "../../include/Channel.hpp"
#include <cstdlib>

void handleModeI(Channel *channel, bool adding, const std::string &target, Client &client) {
	channel->setInviteOnly(adding);
	channel->broadcast(":" + client.getNickname() + " MODE " + target + " " + (adding ? "+i" : "-i") + "\r\n");
}

void handleModeT(Channel *channel, bool adding, const std::string &target, Client &client) {
	channel->setTopicRestricted(adding);
	channel->broadcast(":" + client.getNickname() + " MODE " + target + " " + (adding ? "+t" : "-t") + "\r\n");
}

void handleModeK(Channel *channel, bool adding, const std::string &target, Client &client, const std::string &key) {
	if(adding) {
		channel->setKey(key);
	} else {
		channel->setKey("");
	}
	channel->broadcast(":" + client.getNickname() + " MODE " + target + " " + (adding ? "+k" : "-k") + "\r\n");
}

void handleModeO(Channel *channel, bool adding, const std::string &target, Client &client, const std::string &nick, Server &server) {
	if(nick.empty()) {
		client.sendMessage("461 " + client.getNickname() + " MODE :Not enough parameters\r\n");
		return;
	}
	Client *target_client = server.getClient(nick);
	if(!target_client || !channel->isMember(target_client)) {
		client.sendMessage("441 " + client.getNickname() + " " + nick + " " + target + " :They aren't on that channel\r\n");
		return ;
	}
	if(adding) {
		channel->addOperator(target_client);
	} else {
		channel->removeOperator(target_client);
	}
	channel->broadcast(":" + client.getNickname() + " MODE " + target + " " + (adding ? "+o" : "-o") + " " + nick + "\r\n");
}

void handleModeL(Channel *channel, bool adding, const std::string &target, Client &client, const std::string &limit_str) {
	if(adding) {
		int limit = atoi(limit_str.c_str());
		channel->setUserLimit(limit);
	} else
		channel->setUserLimit(0);
	channel->broadcast(":" + client.getNickname() + " MODE " + target + " " + (adding ? "+l" : "-l") + "\r\n");
}


void cmd_mode (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing) {
	(void)	trailing;
	if(params.empty()) {
		client.sendMessage("461 " + client.getNickname() + " MODE :Not enough parameters\r\n");
		return ;
	}
	std::string target = params[0];
	if(target[0] != '#') {
		if(target == client.getNickname()) {
			if(params.size() < 2) {
				client.sendMessage("221 " + client.getNickname() + " +\r\n");
				return ;
			}
		}
		return ;
	}

	Channel *channel = server.getChannel(target);
	if(!channel) {
		client.sendMessage("403 " + client.getNickname() + " " + target + " :No such channel\r\n");
		return ;
	}

	if(!channel->isOperator(&client)) {
		client.sendMessage("482 " + client.getNickname() + " " + target + " :You're not channel operator\r\n");
		return;
	}

	if(params.size() < 2) {	
		std::string modes = "+";
		if(channel->isInviteOnly()) modes = modes + "i";
		if(channel->isTopicRestricted()) modes = modes + "t";
		if(!channel->getKey().empty()) modes = modes + "k";
		if(channel->getUserLimit() > 0) modes = modes + "l";
		
		client.sendMessage("324 " + client.getNickname() + " " + target + " " + modes + "\r\n");
		return ;
	}

	std::string mode_str = params[1];
	bool adding;
	if(mode_str[0] == '+')
		adding = true;
	else
		adding = false;

	char mode_char = mode_str[1];
	switch(mode_char) {
		case 'i':
			handleModeI(channel, adding, target, client);
			break;
		case 't':
			handleModeT(channel, adding, target, client);
			break;
		case 'k':
			if(adding && params.size() < 3) {
				client.sendMessage("461 " + client.getNickname() + " MODE :Not enough parameters\n");
				return;
			}
			handleModeK(channel, adding, target, client, adding ? params[2] : "");
			break;
		case 'o':
			if(params.size() < 3) {
				client.sendMessage("461 " + client.getNickname() + " MODE :Not enough parameters\r\n");
				return;
			}
			handleModeO(channel, adding, target, client, params[2], server);
			break;
		case 'l':
			if(adding && params.size() < 3) {
				client.sendMessage("461 " + client.getNickname() + " MODE :Not enough parameters\n");
				return;
			}
			handleModeL(channel, adding, target, client, adding ? params[2] : "");
			break;
		default:
			client.sendMessage("472 " + client.getNickname() + " MODE :is unknown mode char to me\r\n");
			break;
	}
}