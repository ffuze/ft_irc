/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegl-in <adegl-in@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 13:43:28 by adegl-in          #+#    #+#             */
/*   Updated: 2026/04/03 13:43:29 by adegl-in         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"
#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"

void cmd_notice(Server &server, Client &client, const std::vector<std::string> &params, const std::string &trailing)
{
    if (params.empty() || trailing.empty())
        return;
    std::string target = params[0];
    std::string message = ":" + client.getNickname() + " NOTICE " + target + " :" + trailing;
    if (target[0] == '#')
    {
        Channel *channel = server.getChannel(target);
        if (!channel)
            return;
        const std::map<std::string, Client*> &members = channel->getMembers();
        for (std::map<std::string, Client*>::const_iterator it = members.begin(); it != members.end(); ++it)
        {
            if (it->second->getNickname() != client.getNickname())
                it->second->sendMessage(message + "\r\n");
        }
    }
    else
    {
        Client *target_client = server.getClient(target);
        if (target_client == NULL)
            return;
        target_client->sendMessage(message + "\r\n");
    }
}