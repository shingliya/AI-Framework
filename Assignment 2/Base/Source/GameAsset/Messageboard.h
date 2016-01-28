#ifndef MESSAGEBOARD_H
#define MESSAGEBOARD_H

#include <String>
#include <vector>

using std::vector;
using std::string;

class Messageboard {
private:
	vector <string> from;
	vector <string> to;
	vector <string> messages;
	vector <bool> completed;

public:
	Messageboard();
	~Messageboard();

	string getMessage(string forWho);
	void setMessage(string from, string to, string message);
	void setComplete(string message);

	int getTotalMassage();
	string getFrom(int pos);
	string getTo(int pos);
	string getMessage(int pos);
	string getComplete(int pos);
};

#endif