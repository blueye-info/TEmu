//
// chat_server.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

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
#include "chat_room.h"

using boost::asio::ip::tcp;

//----------------------------------------------------------------------

class chat_session
	: public chat_participant,
	public boost::enable_shared_from_this<chat_session>
{
public:
	chat_session(boost::asio::io_service& io_service, chat_room& room)
		: socket_(io_service),
		room_(room)
		//m_is_started(false)
	{
	}

	tcp::socket& socket()
	{
		return socket_;
	}
	//bool is_started()
	//{
	//	return m_is_started;
	//};

	void start()
	{
		//m_is_started = true;
		room_.join(shared_from_this());

		//DEBUG
		//std::string str = "TEST:1 string 123456 END";
		//deliver(str);
		//deliver("TEST:2 char AAAAAA END");

		boost::asio::async_read(socket_,
			boost::asio::buffer(read_msg_.data(), string_message::header_length),
			boost::bind(
			&chat_session::handle_read_header, shared_from_this(),
			boost::asio::placeholders::error));
	}

	void deliver(const char* str)
	{
		deliver(std::string(str));
	}

	void deliver(const std::string& str)
	{
		string_message msg;
		msg.from_string(str);
		deliver(msg);
	}

	void deliver(const string_message& msg)
	{
		bool write_in_progress = !write_msgs_.empty();
		write_msgs_.push_back(msg);
		if (!write_in_progress)
		{
			boost::asio::async_write(socket_,
				boost::asio::buffer(write_msgs_.front().data(),
				write_msgs_.front().length()),
				boost::bind(&chat_session::handle_write, shared_from_this(),
				boost::asio::placeholders::error));
		}
	}

	void handle_read_header(const boost::system::error_code& error)
	{
		if (!error && read_msg_.decode_header())
		{
			std::cout << "handle_read_header" << std::endl;
			boost::asio::async_read(socket_,
				boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
				boost::bind(&chat_session::handle_read_body, shared_from_this(),
				boost::asio::placeholders::error));
		}
		else
		{
			room_.leave(shared_from_this());
		}
	}
	void handle_read_body(const boost::system::error_code& error);
	//void handle_read_body(const boost::system::error_code& error)
	//{
	//	if (!error)
	//	{
	//		//DEBUG
	//		std::cout << read_msg_.to_string() << std::endl;
	//		std::cout.write(read_msg_.body(), read_msg_.body_length());
	//		std::cout << "\n";

	//		room_.deliver(read_msg_);
	//		boost::asio::async_read(socket_,
	//			boost::asio::buffer(read_msg_.data(), string_message::header_length),
	//			boost::bind(&chat_session::handle_read_header, shared_from_this(),
	//			boost::asio::placeholders::error));
	//	}
	//	else
	//	{
	//		room_.leave(shared_from_this());
	//	}
	//}

	void handle_write(const boost::system::error_code& error)
	{
		if (!error)
		{
			write_msgs_.pop_front();
			if (!write_msgs_.empty())
			{
				boost::asio::async_write(socket_,
					boost::asio::buffer(write_msgs_.front().data(),
					write_msgs_.front().length()),
					boost::bind(&chat_session::handle_write, shared_from_this(),
					boost::asio::placeholders::error));
			}
		}
		else
		{
			room_.leave(shared_from_this());
		}
	}

private:
	tcp::socket socket_;
	chat_room& room_;
	string_message read_msg_;
	string_message_queue write_msgs_;
	//bool m_is_started ;
};

typedef boost::shared_ptr<chat_session> chat_session_ptr;

//----------------------------------------------------------------------

class chat_server
{
public:
	chat_server(boost::asio::io_service& io_service,
		const tcp::endpoint& endpoint)
		: io_service_(io_service),
		acceptor_(io_service, endpoint)
	{
		start_accept();
	}

	void start_accept()
	{
		chat_session_ptr new_session(new chat_session(io_service_, room_));
		acceptor_.async_accept(new_session->socket(),
			boost::bind(&chat_server::handle_accept, this, new_session,
			boost::asio::placeholders::error));
	}

	void handle_accept(chat_session_ptr session,
		const boost::system::error_code& error)
	{
		if (!error)
		{
			std::cout << "new session start!" << std::endl;
			session->start();
		}

		start_accept();
	}
	chat_room * get_room(){return &room_;}
private:
	boost::asio::io_service& io_service_;
	tcp::acceptor acceptor_;
	chat_room room_;
};

typedef boost::shared_ptr<chat_server> chat_server_ptr;
typedef std::list<chat_server_ptr> chat_server_list;

//----------------------------------------------------------------------

/*int main(int argc, char* argv[])
{
	try
	{
		if (argc < 2)
		{
			std::cerr << "Usage: chat_server <port> [<port> ...]\n";
			return 1;
		}

		boost::asio::io_context io_context;

		chat_server_list servers;
		for (int i = 1; i < argc; ++i)
		{
			using namespace std; // For atoi.
			tcp::endpoint endpoint(tcp::v4(), atoi(argv[i]));
			chat_server_ptr server(new chat_server(io_context, endpoint));
			servers.push_back(server);
		}

		io_context.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}*/
