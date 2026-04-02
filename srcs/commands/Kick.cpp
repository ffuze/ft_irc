//LEO

#include "../../include/Server.hpp"
#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"


//kikc client from channel
void cmd_kick (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing) {
	(void) server;
	(void) client;
	(void) params;
	(void) trailing;

	//1. Controllare che il cliente - e un opertore (dentro channel._operators;)
//2. Check if canale esiste (params)
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

//3. Check target (params) esiste (channel._members)

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

	//4. Eliminare (da ovunque _members, _operatrorsmm _invited)
	//5. Inviare KICK a tutti (channel->broadcast())
	channel->broadcast(kick_msg);
	channel->removeMember(target);
}



//1. Controllare che il cliente - e un opertore (dentro channel._operators;)
//2. Check if canale esiste (params)
//3. Check target (params) esiste (channel._members)
//4. Eliminare (da ovunque _members, _operatrorsmm _invited)
//5. Inviare KICK a tutti (channel->broadcast())
