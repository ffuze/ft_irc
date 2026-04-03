/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParamValidator.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adegl-in <adegl-in@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/29 19:53:11 by nakoriko          #+#    #+#             */
/*   Updated: 2026/04/03 12:26:37 by adegl-in         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARAMVALIDATOR_HPP
# define PARAMVALIDATOR_HPP

# include <string>

bool isValidNickname(const std::string &nick);
bool isValidChannelName(const std::string &channel);
bool isValidFlag(const std::string &flag);

#endif