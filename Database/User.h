//
// Created by Adi Iman on 25/04/2020.
//

#ifndef WORKSHOP1_USER_H
#define WORKSHOP1_USER_H
#include <string>
#include <boost/optional.hpp>
using namespace std;

class User
{
public:
	long Id;
	string Username, Password;
	bool IsActive;
	boost::optional<long> AddedBy;

	User()
	{
		Id = 0;
	};
	User(long Id, string Username, string Password, bool IsActive,boost::optional<long> AddedBy)
	{
		this->Id = Id;
		this->Username = Username;
		this->Password = Password;
		this->IsActive = IsActive;
		this->AddedBy = AddedBy;
	};
};
#endif //WORKSHOP1_USER_H
