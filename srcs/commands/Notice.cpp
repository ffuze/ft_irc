#include "../../include/Server.hpp"
#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"

// send a message, in a channel, ONLY to all of the users that are in that cbannel
void cmd_notice(Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing)
{
    // Check if target and message are provided
    if (params.empty() || trailing.empty())
        return;
    std::string target = params[0];
    std::string message = ":" + client.getNickname() + " NOTICE " + target + " :" + trailing;
    // check if the target is a channel
    if (target[0] == '#')
    {
        Channel *channel = server.getChannel(target);
        // check if channel does not exist
        if (!channel)
            return;
        const std::map<std::string, Client*> &members = channel->getMembers();
        // send the notice message to all members except the sender 
        for (std::map<std::string, Client*>::const_iterator it = members.begin(); it != members.end(); ++it)
        {
            if (it->second->getNickname() != client.getNickname())
                it->second->sendMessage(message + "\r\n");
        }
    }
    // check if the target is instead a client
    else
    {
        Client *target_client = server.getClient(target);
        // check if client does not exist
        if (target_client == NULL)
            return;
        // send the message 
        target_client->sendMessage(message + "\r\n");
    }
}