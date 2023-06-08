#include <iostream>
#include <asio.hpp>

#define eLOG(x) std::cout << "\033[1;31m[ERROR] \033[0m"   << x << std::endl;
#define wLOG(x) std::cout << "\033[0;33m[WARNING] \033[0m" << x << std::endl;
#define iLOG(x) std::cout << "\033[0;32m[INFO] \033[0m" << x << std::endl;
#define LOG(x)  std::cout << x << std::endl;

using asio::ip::tcp;

void hostChat(tcp::socket& sock, std::string& uname){
	asio::streambuf buf;

	std::string reply;
	std::string msg;
	while (true) {
		asio::read_until(sock, buf, '\n');
		msg = asio::buffer_cast<const char*>(buf.data());
		std::cout << msg << std::endl;

		buf.consume(buf.size());

		std::getline(std::cin, reply);
		reply = uname + " : [MSG] " + reply;

		asio::write(sock, asio::buffer(reply + '\n'));
	}
	sock.close();
}

void accept(asio::io_context& ctx, tcp::acceptor& acceptor, std::string& uname) {
	tcp::socket sock(ctx);

	acceptor.accept(sock);

	hostChat(sock, uname);
}

void host(asio::io_context& ctx) {
	short port;
	std::string uname;

	system("cls");

	iLOG("Choose PORT");
	std::cin >> port;

	iLOG("Choose username");
	std::cin >> uname;

	system("cls");

	iLOG(port);

	tcp::endpoint endpoint(tcp::v4(), port);

	tcp::acceptor acceptor(ctx, endpoint);

	accept(ctx, acceptor, uname);
}

void client(asio::io_context& ctx) {
	system("cls");

	std::string ip, uname;
	short port;
	asio::streambuf buf;

	iLOG("Choose a username");
	std::cin >> uname;

	iLOG("Connect to an IP (127.0.0.1)");
	std::cin >> ip;

	iLOG("Connect to a port (8080)");
	std::cin >> port;

	tcp::resolver resolver(ctx);
	tcp::resolver::results_type endpoint = resolver.resolve(ip, std::to_string(port));

	tcp::socket sock(ctx);
	asio::connect(sock, endpoint);

	system("cls");

	std::string reply;
	std::string msg;
	while (true) {
		std::getline(std::cin, reply);
		reply = uname + " : [MSG] " + reply;

		asio::write(sock, asio::buffer(reply + '\n'));

		asio::read_until(sock, buf, '\n');
		msg = asio::buffer_cast<const char*>(buf.data());
		
		std::cout << msg << std::endl;

		buf.consume(buf.size());
	}
	sock.close();
}

int main() {
	system("cls");

	wLOG("This application is not async");
	wLOG("This means that you can send a message and then you have to wait for the other user to send a message\n");
	iLOG("APPLICATION STYLE: (c)lient or (h)ost");
	char choice;

	asio::io_context ctx;

	std::cin >> choice;

	switch(choice)
	{
		case 'c':
			client(ctx);
			break;
		case 'h':
			host(ctx);
			break;
		default:
		eLOG("INVALID CHOICE");
			break;
	}

	return 0;
}
