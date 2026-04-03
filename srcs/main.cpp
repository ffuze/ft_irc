/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegl-in <adegl-in@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 11:00:48 by nakoriko          #+#    #+#             */
/*   Updated: 2026/04/03 12:41:22 by adegl-in         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <cstdlib>
#include "Server.hpp"


int main(int ac, char **av) {
	std::cerr << "[DEBUG] Program started" << std::endl;

	if (ac != 3) {
		std::cerr << "Arguments error:  port and password " <<std::endl;
		return 1;
	}

	std::cerr << "[DEBUG] Arguments OK: port=" << av[1] << " pass=" << av[2] << std::endl;

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