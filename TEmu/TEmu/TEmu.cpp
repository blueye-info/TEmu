// TEmu.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "chat_client.h"

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		std::cerr << "Usage: client <host> <port>\n";
		std::string host = "127.0.0.1";
		std::string port = "1819"; 
		if (argc >= 2)
		{
			host = argv[1];
		}
		if (argc >= 3)
		{
			port = argv[2];
		}
		std::cerr << "Running: client " << host << " " << port << "\n";

		boost::asio::io_service io_service;

		tcp::resolver resolver(io_service);
		tcp::resolver::query query(host, port);
		tcp::resolver::iterator iterator = resolver.resolve(query);

		boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
//		ctx.set_verify_mode(boost::asio::ssl::context::verify_peer);
		ctx.load_verify_file("server.crt");

		chat_client c(io_service, ctx, iterator);

		boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));

		char line[string_message::max_body_length + 1];
		while (std::cin.getline(line, string_message::max_body_length + 1))
		{
			using namespace std; // For strlen and memcpy.
			string_message msg;
			msg.body_length(strlen(line));
			memcpy(msg.body(), line, msg.body_length());
			msg.encode_header();
			c.write(msg);
		}

		c.close();
		t.join();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	
	return 0;
}

