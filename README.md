# IRC Server - C++98

A fully functional IRC server entirely implemented in C++98, with support for channels, private messages, and commands.

## What can it do?

This project implements an IRC (Internet Relay Chat) server compliant with RFC 1459 with the following features:

+ **Authentication**: PASS + NICK + USER system
+ **Channels**: support for JOIN, PART, TOPIC
+ **Messaging**: PRIVMSG (can also broadcast to other channels)
+ **Operator Management**: INVITE, KICK, MODE
+ **Connection**: non-blocking input and output thanks to poll()

## Machine and IDE requirements

- C++98 or higher
- g++
- Linux/Unix

## Compilation commands

```bash
make                # Compile the server
make clean          # Remove object files
make fclean         # Remove everything
make re             # Full recompilation
```

### Output

- **ircserv**: IRC server executable

## USAGE

### Starting the server

```bash
./ircserv <port> <password>
```

**Example:**
```bash
./ircserv 6667 myPassword
```

Remember that the port number must be between 1 and 65535, while the password is required for client authentication.

### Connecting with a client

#### Tool 1: netcat (NC)

```bash
bash -c 'nc localhost 6667'
```

#### Tool 2: irssi (graphical)

```bash
irssi -c localhost -p 6667 -n nickname
# Inside irssi: /quote PASS mypassword
```

## SUPPORTED COMMANDS

### Basic commands

| Command | Syntax | Description |
|---------|--------|-------------|
| PASS | `PASS <password>` | Authentication (must be first) |
| NICK | `NICK <nickname>` | Set nickname |
| USER | `USER <username> 0 * :<realname>` | Set username |
| JOIN | `JOIN <#channel>` | Join a channel |
| PART | `PART <#channel>` | Leave a channel |
| PRIVMSG | `PRIVMSG <target> :<message>` | Send private message or broadcast |
| QUIT | `QUIT` | Disconnect from server |
| PING | `PING` | Test connection |

### Operator commands

| Command | Syntax | Description |
|---------|--------|-------------|
| INVITE | `INVITE <nick> <#channel>` | Invite user to channel |
| KICK | `KICK <#channel> <nick>` | Remove user from channel |
| MODE | `MODE <#channel> <+/-mode> [params]` | Change channel modes |
| TOPIC | `TOPIC <#channel> :<topic>` | Set channel topic |

### Channel modes

- `+i / -i`: invite-only channel
- `+t / -t`: topic restricted to operators
- `+k / -k`: channel protected by password
- `+l <limit> / -l`: sser limit for channel
- `+o <nick> / -o <nick>`: grant/revoke operator

## Project Structure

```
.
├── include/              # Header files
│   ├── Channel.hpp
│   ├── Client.hpp
│   ├── Server.hpp
│   ├── Commands.hpp
│   ├── CommandHandler.hpp
│   └── ParamValidator.hpp
├── srcs/                 # Source files
│   ├── main.cpp
│   ├── Server.cpp
│   ├── Client.cpp
│   ├── Channel.cpp
│   ├── CommandHandler.cpp
│   └── commands/         # IRC Commands
│       ├── Join.cpp
│       ├── Nick.cpp
│       ├── Pass.cpp
│       ├── User.cpp
│       ├── PrivMsg.cpp
│       ├── Quit.cpp
│       ├── Part.cpp
│       ├── Ping.cpp
│       ├── Invite.cpp
│       ├── Kick.cpp
│       ├── Mode.cpp
│       └── Topic.cpp
├── Makefile
└── README.md
```
