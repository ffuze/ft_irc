*This project has been created as part of the 42 curriculum by adegl-in, nakoriko, lemarino*

# IRC Server - C++98

## Description

This project implements a fully functional **IRC (Internet Relay Chat) server** compliant with RFC 1459, entirely written in C++98.

### What does the project teach?

- To implement a real-time communication server following the IRC protocol standard
- Adapt better with C++98 network programming and socket handling
- Understand multi-client connection management using `poll()`

### Top features

- **Authentication System**: PASS + NICK + USER authentication workflow
- **Channel Management**: support for JOIN, PART, TOPIC, and channel-specific operations
- **Real-time Messaging**: PRIVMSG for private messages and channel broadcasts
- **Operator Privileges**: INVITE, KICK, MODE commands for channel control
- **Non-blocking I/O**: more efficient connection handling using `poll()` system call
- **RFC 1459 Compliance**: full adherence to Internet Relay Chat (IRC) protocol standards
- **Channel Modes**: support for +i (invite-only), +t (topic restricted), +k (key protected), +l (user limit), and +o (operator)

## INSTRUCTIONS

### Requirements

- **Language**: C++98
- **Compiler**: g++
- **Operating System**: Linux/Unix

### Compilation

```bash
make                # Compile the server
make clean          # Remove object files
make fclean         # Remove all compiled files and objects
make re             # Full recompilation
```

**Output**: Executable `ircserv`

### EXECUTION

#### Starting the server

```bash
./ircserv <port> <password>
```

**Parameters:**
- `<port>`: the server listening port (must be between 1 and 65535)
- `<password>`: the password required for client authentication

**Example:**
```bash
./ircserv 6667 myPassword
```

#### Connecting with a client

**Option 1: netcat (simple terminal client)**
```bash
bash -c 'nc -C localhost 6667'
```

**Option 2: irssi (GUI client)**
```bash
irssi -c localhost -p 6667 -w myPassword -n nickname
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
- `+l <limit> / -l`: user limit for channel
- `+o <nick> / -o <nick>`: grant/revoke operator

## Project structure

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

## RESOURCES

### Where we mostly got inspired

- **RFC 1459**: [Internet Relay Chat Protocol](https://tools.ietf.org/html/rfc1459) - Official IRC protocol specification and standards
- **POSIX poll()**: [System Call Documentation](https://pubs.opengroup.org/onlinepubs/9699919799/functions/poll.html)
- **C++98 Standard**: Reference documentation for C++ language features and standard library

### Was AI used for this project?

AI was indeed used to develop the project, but only for learning purposes, specifically as follows:

- **Protocol Compliance**: to ensure we used the correct IRC message formats and response codes
- **Code Documentation**: to fasten the development of this README structure
- **Debugging & Optimization**: to idenifty and resolve issues regarding the main aspects, such as socket handling, buffer management, and protocol compliance
