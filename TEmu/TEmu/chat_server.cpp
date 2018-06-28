#include "chat_server.h"


void chat_session::handle_read_body(const boost::system::error_code& error)
{
	if (!error)
	{
		//DEBUG
		std::cout << "handle_read_body" << std::endl;
		std::cout << read_msg_.to_string() << std::endl;
		std::cout.write(read_msg_.body(), read_msg_.body_length());
		std::cout << "\n####\n";

		room_.deliver(read_msg_);
		boost::asio::async_read(socket_,
			boost::asio::buffer(read_msg_.data(), string_message::header_length),
			boost::bind(&chat_session::handle_read_header, shared_from_this(),
				boost::asio::placeholders::error));
	}
	else
	{
		room_.leave(shared_from_this());
	}
}