#pragma once

#include <algorithm>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <set>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
//#include <boost/asio/io_context.hpp>
#include "string_message.hpp"

//----------------------------------------------------------------------

typedef std::deque<string_message> string_message_queue;

//----------------------------------------------------------------------

class chat_participant
{
public:
	virtual ~chat_participant() {}
	virtual void deliver(const string_message& msg) = 0;
};

typedef boost::shared_ptr<chat_participant> chat_participant_ptr;

//----------------------------------------------------------------------

class chat_room
{
public:
	void join(chat_participant_ptr participant);

	void leave(chat_participant_ptr participant);

	void deliver(const string_message& msg);

	std::set<chat_participant_ptr> * get_participants(){return &participants_;}
private:
	std::set<chat_participant_ptr> participants_;
	enum { max_recent_msgs = 50 };
	string_message_queue recent_msgs_;
};