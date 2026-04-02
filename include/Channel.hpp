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
#include <ctime>
#include "Client.hpp"

class Channel {
private:
	std::string _name;
	std::string _topic; //command TOPIC
	std::string _topic_creator;
	std::time_t  _topic_time;

	//Modes from subject:
	bool _invite_only; // mode i: invite-only channel
	bool _top_restricted; //mode t: just operator can change TOPIC
	std::string _key; // mode k: channel password
	int _user_limit; //mode l: limit of clients
	
	//"Databases":
	std::map<std::string, Client*> _members; //needs for JOIN, PART,KICK,QUIT, PRIVMSG
	std::map<std::string, Client*> _operators; //needs to check operator rights for KICK, INVITE, TOPIC, MODE
	std::set<std::string> _invited; // to keep nicknames. Needs for INVTE, JOIN (if mode +i);



public:
	Channel(const std::string &name);//creates channel with name


	//MEMBERS (for JOIN, PART, KICK, OUT)
	void addMember(Client *client);//agiungere to channel
	void removeMember(Client *client); //from channel (tutti elenci)
	bool isMember(Client *client) const; //check se presente dentro _members

	//OPERATORS(need to check the rights for commands and for MODE +o / -o)
	void addOperator(Client *client);
	void removeOperator(Client *client); 
	bool isOperator(Client *client) const;

	//INVITES (needs for INVITe and Join if mode +i)
	void addInvited(const std::string &nickname);
	bool isInvited(const std::string &nickname) const;

	//BROADCAST send message to all members of channel
	void broadcast(const std::string &message, Client *exlude = NULL); //+a parte  cliente appena aggiunto (Join.cpp n8.)

	//NICK
	void updateNick(const std::string &old_nick, const std::string & new_nick);
	
	//TOPIC
	void setTopic(const std::string &topic, Client *creator);
	const std::string &getTopic()const;
	const std::string getTopicCreator();
	const std::string getTopicTime();// long timestamp convertito in string

	//MODES
	//-i/+i
	void setInviteOnly(bool value);
	bool isInviteOnly() const;

	//+t /-t
	void setTopicRestricted(bool value);
	bool isTopicRestricted() const;

	//+k / -k
	void setKey(const std::string &key);
	bool checkKey(const std::string &key) const;
	const std::string getKey() const;

	//+l/-l
	void setUserLimit(int limit);
	bool canJoin() const;
	int getUserLimit() const;
	
	
	//GETTERS
	const std::string &getName() const;
	const std::map<std::string, Client*> &getMembers() const;
	const std::map<std::string, Client *> &getOperators() const;
};

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
