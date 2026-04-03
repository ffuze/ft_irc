#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <set>
# include <map>
# include <ctime>
# include "Client.hpp"

class Channel {
	private:
		std::string _name;
		std::string _topic;
		std::string _topic_creator;
		std::time_t  _topic_time;

		bool _invite_only;
		bool _top_restricted;
		std::string _key;
		int _user_limit;

		std::map<std::string, Client*> _members;
		std::map<std::string, Client*> _operators;
		std::set<std::string> _invited;

	public:
		Channel(const std::string &name);

		void addMember(Client *client);
		void removeMember(Client *client);
		bool isMember(Client *client) const;

		void addOperator(Client *client);
		void removeOperator(Client *client); 
		bool isOperator(Client *client) const;

		void addInvited(const std::string &nickname);
		bool isInvited(const std::string &nickname) const;

		void broadcast(const std::string &message, Client *exlude = NULL);

		void updateNick(const std::string &old_nick, const std::string & new_nick);

		void setTopic(const std::string &topic, Client *creator);
		const std::string &getTopic()const;
		const std::string getTopicCreator();
		const std::string getTopicTime();

		void setInviteOnly(bool value);
		bool isInviteOnly() const;

		void setTopicRestricted(bool value);
		bool isTopicRestricted() const;

		void setKey(const std::string &key);
		bool checkKey(const std::string &key) const;
		const std::string getKey() const;

		void setUserLimit(int limit);
		bool canJoin() const;
		int getUserLimit() const;

		const std::string &getName() const;
		const std::map<std::string, Client*> &getMembers() const;
		const std::map<std::string, Client *> &getOperators() const;
};

#endif