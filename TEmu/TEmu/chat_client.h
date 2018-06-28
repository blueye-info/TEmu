#pragma once
#include <cstdlib>
#include <deque>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <openssl/x509v3.h>
#include <boost/thread/thread.hpp>
#include "string_message.hpp"

using boost::asio::ip::tcp;

typedef std::deque<string_message> string_message_queue;

class chat_client
{
public:
	chat_client(boost::asio::io_service& io_service,
		boost::asio::ssl::context& context,
		tcp::resolver::iterator endpoint_iterator)
		: io_service_(io_service),
		socket_(io_service, context)
	{
		socket_.set_verify_mode(boost::asio::ssl::verify_peer);
		socket_.set_verify_callback(
			boost::bind(&chat_client::verify_certificate, this, _1, _2));

		boost::asio::async_connect(socket_.lowest_layer(), endpoint_iterator,
			boost::bind(&chat_client::handle_connect, this,
			boost::asio::placeholders::error));
	}

	bool verify_certificate(bool preverified,
		boost::asio::ssl::verify_context& ctx)
	{
		// The verify callback can be used to check whether the certificate that is  
		// being presented is valid for the peer. For example, RFC 2818 describes  
		// the steps involved in doing this for HTTPS. Consult the OpenSSL  
		// documentation for more details. Note that the callback is called once  
		// for each certificate in the certificate chain, starting from the root  
		// certificate authority.  

		// In this example we will simply print the certificate's subject name.  
		char subject_name[256];
		
		X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
		X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
		std::cout << "Verifying " << subject_name << "\n";

		return preverified;
	}

	void write(const string_message& msg)
	{
		//boost::asio::post(io_service_,
		//	boost::bind(&chat_client::do_write, this, msg));
		io_service_.post(boost::bind(&chat_client::do_write, this, msg));
	}

	void close()
	{
		//boost::asio::post(io_service_,
		//	boost::bind(&chat_client::do_close, this));

		io_service_.post(boost::bind(&chat_client::do_close, this));
	}

private:

	void handle_connect(const boost::system::error_code& error)
	{
		if (!error)
		{
			socket_.async_handshake(boost::asio::ssl::stream_base::client,
				boost::bind(&chat_client::handle_handshake, this,
					boost::asio::placeholders::error));
		}
		else
		{
			std::cout << "Connect failed: " << error.message() << "\n";
		}


	}

	void handle_handshake(const boost::system::error_code& error)
	{
		if (!error)
		{
			std::cout << "Handshake ok! " <<std::endl;

			boost::asio::async_read(socket_,
				boost::asio::buffer(read_msg_.data(), string_message::header_length),
				boost::bind(&chat_client::handle_read_header, this,
				boost::asio::placeholders::error));

		}
		else
		{
			std::cout << "Handshake failed: " << error.message() << "\n";
		}
	}

	void handle_read_header(const boost::system::error_code& error)
	{
		if (!error && read_msg_.decode_header())
		{
//			std::cout << " chat client::handle_read_header ok" << std::endl;
			boost::asio::async_read(socket_,
				boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
				boost::bind(&chat_client::handle_read_body, this,
				boost::asio::placeholders::error));
		}
		else
		{
			std::cout << " chat client::handle_read_header error" << std::endl;
			do_close();
		}
	}

	void handle_read_body(const boost::system::error_code& error)
	{
		if (!error)
		{
			//DEBUG
//			std::cout << "char_client::handle_read_body() ok -- " << read_msg_.to_string() << std::endl;
			std::cout.write(read_msg_.body(), read_msg_.body_length());
//			std::cout << "\n\n"<<std::endl;
//			std::cout << "pause!!!" << std::endl;
		
			read_msg_.from_string("");

			boost::asio::async_read(socket_,
				boost::asio::buffer(read_msg_.data(), string_message::header_length),
				boost::bind(&chat_client::handle_read_header, this,
				boost::asio::placeholders::error));
		}
		else
		{
			std::cout << "char_client::handle_read_body() error -- " << std::endl;
			do_close();
		}
	}

	void do_write(string_message msg)
	{
//		std::cout << "char_client::do_write() " << std::endl;
		bool write_in_progress = !write_msgs_.empty();
		write_msgs_.push_back(msg);
		if (!write_in_progress)
		{
			boost::asio::async_write(socket_,
				boost::asio::buffer(write_msgs_.front().data(),
				write_msgs_.front().length()),
				boost::bind(&chat_client::handle_write, this,
				boost::asio::placeholders::error));
		}
	}

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
					boost::bind(&chat_client::handle_write, this,
					boost::asio::placeholders::error));
			}
		}
		else
		{
			do_close();
		}
	}

	void do_close()
	{
		std::cout << "char_client::do_close()" << std::endl;
		socket_.lowest_layer().close();//.shutdown();// close();
	}

private:
	boost::asio::io_service& io_service_;
	//tcp::socket socket_;
	boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
	string_message read_msg_;
	string_message_queue write_msgs_;
};
