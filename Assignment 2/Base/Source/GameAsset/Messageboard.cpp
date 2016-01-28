#include "Messageboard.h"

Messageboard::Messageboard() 
{
}

Messageboard::~Messageboard() 
{
}

string Messageboard::getMessage(string forWho) 
{
	if (messages.size() == 0) 
	{
		return "no msg";
	}
	else {
		for (int i = messages.size() - 1; i > -1; --i)
		{
			if (to[i] == forWho && completed[i] == false)
			{
				return messages[i];
			}
		}
	}
	return "no msg";
}

void Messageboard::setMessage(string from, string to, string message) {
	if (messages.size() >= 10) {
		this->from.erase(this->from.begin());
		this->to.erase(this->to.begin());
		messages.erase(messages.begin());
		completed.erase(completed.begin());
	}

	this->from.push_back(from);
	this->to.push_back(to);
	this->messages.push_back(message);
	this->completed.push_back(false);
}

void Messageboard::setComplete(string message)
{
	for (int i = messages.size() - 1; i > -1; --i)
	{
		if (messages[i] == message && completed[i] == false)
		{
			completed[i] = true; 
			break;
		}
	}
}

int Messageboard::getTotalMassage()
{
	return messages.size();
}

string Messageboard::getFrom(int pos)
{
	return from[pos];
}

string Messageboard::getTo(int pos)
{
	return to[pos];
}

string Messageboard::getMessage(int pos)
{
	return messages[pos];
}

string Messageboard::getComplete(int pos)
{
	if (completed[pos] == true)
		return "yes";
	else
		return "no";
}