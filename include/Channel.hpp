/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alek <alek@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 18:01:54 by alek              #+#    #+#             */
/*   Updated: 2026/03/28 16:53:26 by alek             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <set>
#include <map>
#include "Client.hpp"

class Channel {
    private:
        std::string _name;
        std::string _topic; //command TOPIC

        //"Databases":
        std::map<std::string, Client*> _members;
        std::map<std::string, Client*> _operators;
        std::set<std::string> _invited;

        //Modes:
        bool _invite_only;     // mode i (invite-only)
        bool _top_restricted;  // mode t (topic restricted to operators)
        std::string _key;      // mode k (channel password)
        int _user_limit;       // mode l (user limit, -1 if no limit)

    public:

        Channel(const std::string &name);
        ~Channel();

        // member management
        void addMember(Client *client);
        void removeMember(Client *client);
        bool isMember(Client *client) const;

        // operator management
        void addOperator(Client *client);
        void removeOperator(Client *client);
        bool isOperator(Client *client) const;

        // inviting methods
        void addInvited(const std::string &nickname);
        void removeInvited(const std::string &nickname);
        bool isInvited(const std::string &nickname) const;

        // broadcasting (general)
        void broadcast(const std::string &message);

        //broadcasting (except exclude)
        void broadcast(const std::string &message, Client *exclude);

        // get() methods
        const std::string& getName() const;
        const std::string& getTopic() const;
        const std::string& getKey() const;
        int getUserLimit() const;
        size_t getMemberCount() const;
        const std::map<std::string, Client*>& getMembers() const;

        // set() methods
        void setTopic(const std::string &topic);
        void setKey(const std::string &key);
        void setUserLimit(int limit);
        
        bool isInviteOnly() const;
        void setInviteOnly(bool value);

        bool isTopicRestricted() const;
        void setTopicRestricted(bool value);
};

#endif
