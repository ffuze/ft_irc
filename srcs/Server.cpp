/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alek <alek@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 12:27:39 by nakoriko          #+#    #+#             */
/*   Updated: 2026/03/27 18:15:02 by alek             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <poll.h>


Server::Server (int port, std::string password) : _port(port), _password(password), _running(false) {
	// 1.socket()
	// 2.bind()
	// 3. listen()
	// NON blockin(?)

	struct pollfd pfd; // standart structure from poll дшикфкн
	pfd.fd = _server_fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_pollfds.push_back(pfd);
	
	

}





// void Server::run() {
// 	while(_running) {
// 		int events_count; // = poll(struct pollfd *fds, nfds_t nfds, int timeout )- waiting for events
// 		if (events_count < 0) //error or signal to stop
// 			break;
// 		//something happpend - >start to work {
// 		//check every fd in our array of structures of fd
// 	// 	for (int i = 0; i < nfds; i++) {
// 	// 		if(events for reading (POLLIN)) {

// 	// 		}
// 	// 		if(event for sending (POLLOUT)) {

// 	// 		}
// 	// 	}
// 	// }
// 