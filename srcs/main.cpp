/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegl-in <adegl-in@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 11:00:48 by nakoriko          #+#    #+#             */
/*   Updated: 2026/04/02 22:15:43 by adegl-in         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Status: row

#include <iostream>
#include <string>
#include <cstdlib>
#include "Server.hpp"


int main(int ac, char **av) {

	// // disable buffering
	// std::cout.setf(std::ios::unitbuf);
	// std::cerr.setf(std::ios::unitbuf);
	
	std::cerr << "[DEBUG] Program started" << std::endl;

	if (ac != 3) {
		//1.check port e passwd
		std::cerr << "Arguments error:  port and password " <<std::endl;
		return 1;
	}

	std::cerr << "[DEBUG] Arguments OK: port=" << av[1] << " pass=" << av[2] << std::endl;

	//2.parse_args(**av: port + password);
	// TCP/IP standart (for port numbers)
	int port = std::atoi(av[1]);
	if (port <= 0 || port > 65535) {
		std::cerr << "Error: invalid port number" << std::endl;
		return 1;
	}
	
	std::string password = av[2]; 
	if (password.empty()) {
		std::cerr << "Error: password can't be empty" << std::endl;
	}

	std::cerr << "[DEBUG] Creating server..." << std::endl;

	//3. create server: socket (gets fd) + bind(set the port) + listen(starts waiting mode) - ALL NON blocks)
	//4. run server (loop + server actions + clients actions(parse commands))
	try {
		Server server(port, password);
		std::cerr << "[DEBUG] Server created, running..." << std::endl;
		server.run();
	} catch (std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}