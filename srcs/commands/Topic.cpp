//LEO

#include "../../include/Server.hpp"
#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"
#include "../../include/Channel.hpp"

static void giveTopic(Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing) {
	(void) trailing;
	Channel *channel = server.getChannel(params[0]);
	if (channel->getTopic().empty()) {
			client.sendMessage("331 " + client.getNickname() + " " + channel->getName() + " :No topic is set\r\n");
	}
	else {
		client.sendMessage(":server 332 " + client.getNickname() + ": the topic of #" + \
			channel->getName() + " is " + channel->getTopic() + "\r\n");
		client.sendMessage(":server 332 " + client.getNickname() + ": topic last changed by " + \
			channel->getTopicCreator() + " on " + channel->getTopicTime() + "\r\n");
	}
}

static void	changeTopic(Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing) {
	Channel *channel = server.getChannel(params[0]);

	channel->setTopic(trailing, &client);
	if (channel->getTopic() == "") {
		client.sendMessage(":server 331 " + client.getNickname() + ": has deleted the channel #" + channel->getName() + "'s topic\r\n");
	}
	else {
		client.sendMessage(":server 332 " + client.getNickname() + ": the topic of #" + \
			channel->getName() + " is " + channel->getTopic() + "\r\n");
	}
	client.sendMessage(":server 332 " + client.getNickname() + ": topic last changed by " + \
		channel->getTopicCreator() + " on " + channel->getTopicTime() + "\r\n");
}

void cmd_topic (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing) {
	if (params.empty()) {
		client.sendMessage("461 " + client.getNickname() + ": not enough parameters\r\n");
		return;
	}
	Channel *channel = server.getChannel(params[0]);
	if(!channel) {
		client.sendMessage("403 " + client.getNickname() + " " + params[0] + " :No such channel\r\n");
		return ;
	}
	
	if (channel->isMember(&client) == false) {
		client.sendMessage("442 " + client.getNickname() + " " + channel->getName() + " :You're not on that channel\r\n"); //sistemato messaggio secondo 6.1. Error replies
		return ;
	}
	if(!trailing.empty()) {
		if (channel->isTopicRestricted() == true && \
			channel->isOperator(&client) == false) {
				client.sendMessage("482 " + client.getNickname() + " " + channel->getName() +  " :You're not channel operator\r\n"); //6.1. Error replies
				return ;
		}
		changeTopic(server, client, params, trailing);
	}
	else
		giveTopic(server, client, params, trailing);
	return ;
}
