/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParamValidator.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegl-in <adegl-in@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 19:54:17 by nakoriko          #+#    #+#             */
/*   Updated: 2026/04/03 12:41:37 by adegl-in         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParamValidator.hpp"

bool isValidNickname(const std::string &nick) {
	if(nick.empty())
		return false;
	if(!std::isalpha(nick[0]))
		return false;
	const std::string ok_chars = "-_[]\\'^{}";
	for (size_t i = 1; i < nick.size(); i++) {
		char c = nick[i];
		if (!std::isdigit(c) && !std::isalpha(c) && ok_chars.find(c) == std::string::npos)
			return false;
	}
	return true;
}

bool isValidChannelName(const std::string &channel) {
	if(channel.empty())
		return false;
	const std::string pref_ok = "#&+!";
	if(pref_ok.find(channel[0]) == std::string::npos)
		return false;
	const std::string ko_chars = " ,\a\r\n\t";
	for(size_t i = 1; i < channel.size(); i++) {
		if(ko_chars.find(channel[i]) != std::string::npos)
			return false;
	}
	return true;
}

bool isValidFlag(const std::string &flag) {
	if(flag.empty())
		return false;
	if(flag[0] != '+' && flag[0] != '-')
		return false;
	const std::string ok_chars = "+-itkol";
	for (size_t i = 0; i < flag.size(); i++) {
		if(ok_chars.find(flag[i]) == std::string::npos)
			return false;
	}
	return true;
}