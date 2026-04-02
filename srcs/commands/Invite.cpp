//LEO

#include "../../include/Server.hpp"
#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"

//invite client to channel
void cmd_invite (Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing) {


(void) trailing;// non necessario


	//1. Controllare che il canale passato (dentro params)
	if(params.size() < 2) {
		client.sendMessage("461 " + client.getNickname() + " INVITE :Not enough parameters\r\n");
		return;
	}

	std::string target_nick = params[0];
	std::string chanel_name = params[1];
	//2. Trovare o creare il canale (dentro channels - Alessando)
	Channel *channel = server.getChannel(chanel_name);
	if(!channel) {
		client.sendMessage("403 " + client.getNickname() + " " + chanel_name +  " :No such channel\r\n");
		return;
	}
	
	if(!channel->isMember(&client)) {
		client.sendMessage("442 " + client.getNickname() + " " + chanel_name +  " :You're not on that channel\r\n");
		return;
	}
	//3. Controllare i regimi di canale:
	//	 (+i invited only, 
	if(channel->isInviteOnly() && !channel->isOperator(&client)) {
		client.sendMessage("482 " + client.getNickname() + " " + chanel_name +  " :You're not channel operator\r\n");
		return;
	}
	//		+k - il password deve corrispondere
	//		+l - quantita d membri dentro canale permette di aggiungere ancora uno)
	
	//4.Aggiungere nuovo membro dentro canale (channel->addMember())
	Client *target = server.getClient(target_nick);
	if( !target) {
		client.sendMessage("401 " + client.getNickname() + " " + target_nick +  " :No such nick\r\n");
		return;
	}
	if(channel->isMember(target)) {
		client.sendMessage("443 " + client.getNickname() + " " + target_nick + " " + chanel_name +  " :is already on channel\r\n");
		return;
	}
	//5. Se canale e nuovo - in teoria il singolo membro deve essere  operatore (channel._operatprs) - non sono sicura pero'
	channel->addInvited(target_nick);
	//6. Inviare la conferma al client (сlient.senMessage() e target
	target->sendMessage(":" + client.getNickname() + " INVITE " + target_nick + " " + chanel_name + "\r\n");
	client.sendMessage("341 " + client.getNickname() + " INVITE " + target_nick + " " + chanel_name +  "\r\n");

	//7. Ogni messaggio dal client al channel devono essere inviati a tutti i membri di channel ()
	//	(la richiesta JOIN - e' un messsagio, allora bisogna di mandare notifica a tuttti)
	//	 broadcast();

	//[https://modern.ircdocs.horse/#invite-message]
	//params[0] = nickname; params[1] = channel
	/*
	Channel *channel = server.getChannel(params[1]);

	//invia messaggio 461 (ERR_NEEDMOREPARAMS)
	//"<client> <command> :Not enough parameters"
	if (params.size() < 2) {
		client.sendMessage("461 " + client.getNickname() + " INVITE" +  + " :not enough parameters\r\n");
	}

	//invia messaggio 403 (ERR_NOSUCHCHANNEL)
	//"<client> <channel> :No such channel"
	else if (!channel) {
		client.sendMessage("403 " + client.getNickname() + " #" + params[1] + ": no such channel\r\n");
	}

	//invia messaggio 442 (ERR_NOTONCHANNEL)
	//"<client> <channel> :You're not on that channel"
	else if (channel->isMember(&client) == false) {
		client.sendMessage("442 " + client.getNickname() + " #" + channel->getName() + ": You're not on that channel\r\n");
	}

	//invia messaggio 482 (ERR_CHANOPRIVSNEEDED)
	//"<client> <channel> :You're not channel operator"
	else if (channel->isInviteOnly() && channel->isOperator(&client) == false) {
		client.sendMessage("482 " + client.getNickname() + " #" + channel->getName() + ": You're not channel operator\r\n");
	}

	//invia messaggio 443 (ERR_USERONCHANNEL)
	//"<client> <nick> <channel> :is already on channel"
	else if (channel->isMember(server.getClient(params[0]))) {
		client.sendMessage("442 " + client.getNickname() + " " + params[0] + " #" + channel->getName() + \
			": is already on channel\r\n");
	}

	//invia messaggio 341 (RPL_INVITED))
	// INVITE reply example dove alice invita bob nel canale general:
	//  ":server 341 alice bob #general" to alice(requesting client)
	//  ":alice!alice@hostname INVITE bob :#general" to bob(target client)
	//   -> ":" + sender->nickname + "!" + sender->username + "@" + sender->hostname + " INVITE ..."
	else {
		;
	}
	*/
}



	
	//1. Controllare che il canale passato (dentro params)
	//2. Trovare o creare il canale (dentro channels - Alessando)
	//3. Controllare i regimi di canale:
	//	 (+i invited only, 
	//		+k - il password deve corrispondere
	//		+l - quantita d membri dentro canale permette di aggiungere ancora uno)
	//4.Aggiungere nuovo membro dentro canale (channel->addMember())
	//5. Se canale e nuovo - in teoria il singolo membro deve essere  operatore (channel._operatprs) - non sono sicura pero'
	//6. Inviare la conferma al client (сlient.senMessage())
	//7. Ogni messaggio dal client al channel devono essere inviati a tutti i membri di channel ()
	//	(la richiesta JOIN - e' un messsagio, allora bisogna di mandare notifica a tuttti)
	//	 broadcast();
	
