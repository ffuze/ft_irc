#include "../include/Channel.hpp"

// Constructor
Channel::Channel(const std::string &name) 
    : _name(name), _topic(""), _invite_only(false), _top_restricted(false), _key(""), _user_limit(-1) {}

// Destructor
Channel::~Channel() {}

// ============ MEMBER MANAGEMENT ============

void Channel::addMember(Client *client)
{
    if (client)
        _members[client->getName()] = client;
}

void Channel::removeMember(Client *client)
{
    if (client)
    {
        _members.erase(client->getName());
        _operators.erase(client->getName());
    }
}

bool Channel::isMember(Client *client) const
{
    if (!client)
        return (false);
    return (_members.find(client->getName()) != _members.end());
}

// ============ OPERATOR MANAGEMENT ============

void Channel::addOperator(Client *client)
{
    if (client)
        _operators[client->getName()] = client;
}

void Channel::removeOperator(Client *client)
{
    if (client)
        _operators.erase(client->getName());
}

bool Channel::isOperator(Client *client) const
{
    if (!client)
        return (false);
    return (_operators.find(client->getName()) != _operators.end());
}

// ============ INVITE MANAGEMENT ============

void Channel::addInvited(const std::string &nickname)
{
    _invited.insert(nickname);
}

void Channel::removeInvited(const std::string &nickname)
{
    _invited.erase(nickname);
}

bool Channel::isInvited(const std::string &nickname) const
{
    return (_invited.find(nickname) != _invited.end());
}

// ============ BROADCASTING ============

void Channel::broadcast(const std::string &message)
{
    for (std::map<std::string, Client*>::iterator it = _members.begin(); it != _members.end(); ++it)
    {
        std::string msg = message;
        it->second->sendMessage(msg);
    }
}

void Channel::broadcast(const std::string &message, Client *exclude)
{
    for (std::map<std::string, Client*>::iterator it = _members.begin(); it != _members.end(); ++it)
    {
        if (exclude && it->second->getFd() == exclude->getFd())
            continue;
        std::string msg = message;
        it->second->sendMessage(msg);
    }
}

// ============ GETTERS ============

const std::string& Channel::getName() const
{
    return (_name);
}

const std::string& Channel::getTopic() const
{
    return (_topic);
}

const std::string& Channel::getKey() const
{
    return (_key);
}

int Channel::getUserLimit() const
{
    return (_user_limit);
}

size_t Channel::getMemberCount() const
{
    return (_members.size());
}

const std::map<std::string, Client*>& Channel::getMembers() const
{
    return (_members);
}

// ============ SETTERS ============

void Channel::setTopic(const std::string &topic)
{
    _topic = topic;
}

void Channel::setKey(const std::string &key)
{
    _key = key;
}

void Channel::setUserLimit(int limit)
{
    _user_limit = limit;
}

// ============ MODE GETTERS / SETTERS ============

bool Channel::isInviteOnly() const
{
    return (_invite_only);
}

void Channel::setInviteOnly(bool value)
{
    _invite_only = value;
}

bool Channel::isTopicRestricted() const
{
    return (_top_restricted);
}

void Channel::setTopicRestricted(bool value)
{
    _top_restricted = value;
}
