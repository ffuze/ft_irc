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
//3. Check target (params) esiste (channel._members)
//4. Eliminare (da ovunque _members, _operatrorsmm _invited)
//5. Inviare KICK a tutti (channel->broadcast())

}