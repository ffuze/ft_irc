//Alessandro


#include "../include/Channel.hpp"
#include <iostream>


Channel::Channel(const std::string &name) 
	:_name(name),
	_topic(""),
	_topic_creator(""),
	_topic_time(0),
	_invite_only(false),
	_top_restricted(false),
	_key(""),
	_user_limit(0)
	{}


// GETTERS
const std::string &Channel::getName() const {
	return _name;
}

//NICK
//aggiorniamo i dati di member dopo cambi di NICK
void Channel::updateNick(const std::string &old_nick, const std::string & new_nick) {
	if(_members.find(old_nick) != _members.end()) {
		Client *client = _members[old_nick];
		_members.erase(old_nick);
		_members[new_nick] = client;
	}

	if(_operators.find(old_nick) != _operators.end()) {
		Client *client = _operators[old_nick];
		_operators.erase(old_nick);
		_operators[new_nick] = client;
	}

	if(_invited.find(old_nick) != _invited.end()) {
		_invited.erase(old_nick);
		_invited.insert(new_nick);
	}
}

	
//TOPIC
void Channel::setTopic(const std::string &topic, Client *creator) {
	_topic = topic;
	//std::time ritorna time attuale (si usa con NULL, se serve solo il numero)
	_topic_time = std::time(NULL); 
	_topic_creator = creator->getNickname();
}

const std::string &Channel::getTopic()const {
	return _topic;
}

const std::string Channel::getTopicCreator() {
	return _topic_creator;
}

const std::string Channel::getTopicTime() {
	struct tm* timeinfo = localtime(&_topic_time);
	char buffer[16];
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S UTC", timeinfo);
	return buffer;
}

//MEMBERS
void Channel::addMember(Client *client) {
	_members[client->getNickname()] = client;
	// std::cout << "Debug: addMember " << client->getNickname() << " to" << _name << ", size =" << _members.size() << std::endl;
}

void Channel::removeMember(Client *client) {
	_members.erase(client->getNickname());
	_operators.erase(client->getNickname());
	_invited.erase(client->getNickname());
}

bool Channel::isMember(Client *client) const{
	// bool found = _members.find(client->getNickname()) != _members.end();
	// std::cout << "Debug isMember() " << client->getNickname() << "= " << found << std::endl;
	if(_members.find(client->getNickname()) == _members.end())
		return false;
	return true;
}

//OPERATORS
	void Channel::addOperator(Client *client) {
		_operators[client->getNickname()] = client;
	}

	void Channel::removeOperator(Client *client) {
		_operators.erase(client->getNickname());
	}
	bool Channel::isOperator(Client *client) const {
		if(_operators.find(client->getNickname()) == _operators.end())
			return false;
		return true;
	}

//INVITES
	void Channel::addInvited(const std::string &nickname)  {
		_invited.insert(nickname);
	}
	bool Channel::isInvited(const std::string &nickname) const {
		if(_invited.find(nickname) == _invited.end())
			return false;
		return true;
	}

	//BROADCAST send message to all members of channel
	void Channel::broadcast(const std::string &message, Client *exlude) {
		for(std::map<std::string, Client*>::iterator it = _members.begin(); it != _members.end(); it++) {
			if(exlude && it->second == exlude)
				continue;
			it->second->sendMessage(message);
		}
	} //+a parte  cliente appena aggiunto (Join.cpp n8.)


	//MODES
	//-i/+i
	void Channel::setInviteOnly(bool value) {
		_invite_only = value;
	}
	bool Channel::isInviteOnly() const {
		return _invite_only;
	}

	//+t /-t
	void Channel::setTopicRestricted(bool value) {
		_top_restricted = value;
	}
	bool Channel::isTopicRestricted() const {
		return _top_restricted;
	}

	//+k / -k
	void Channel::setKey(const std::string &key) {
		_key = key;
	}
	bool Channel::checkKey(const std::string &key) const {
		if(_key.empty())
			return true; //il canale senza la chiave(_key e' vuota)
		if(key == _key)
			return true;
		return false;
	}

	const std::string Channel::getKey() const {
		return _key;
	}

	

	//+l/-l
	void Channel::setUserLimit(int limit) {
		_user_limit = limit;
	}

	bool Channel::canJoin() const {
		if(_user_limit > 0 && _members.size() >= static_cast<size_t>(_user_limit))
			return false;
		return true;
	}

	int Channel::getUserLimit() const {
		return _user_limit;
	}
	
	
	//GETTERS
	const std::map<std::string, Client*> &Channel::getMembers() const {
		return _members;
	}
	const std::map<std::string, Client *> &Channel::getOperators() const {
		return _operators;
	}
