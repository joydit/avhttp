#include <iostream>
#include <boost/thread.hpp>

// 异步, 
// 同步,
// 
// 
// 
// 
// 

#include "avhttp.hpp"

void test(avhttp::http_stream &h)
{
	boost::this_thread::sleep(boost::posix_time::seconds(5));
	boost::system::error_code ec;
	h.close(ec);
}

int main(int argc, char* argv[])
{
	boost::asio::io_service io;
	avhttp::http_stream h(io);
	avhttp::request_opts opt;

	opt.insert("Connection", "close");
	h.request_options(opt);
	h.open("http://w.qq.com/cgi-bin/get_group_pic?pic={64A234EE-8657-DA63-B7F4-C7718460F461}.gif");

	boost::thread(boost::bind(&test, boost::ref(h)));

	boost::array<char, 1024> buf;
	boost::system::error_code ec;
	std::size_t file_size = 0;
	while (!ec)
	{
		std::size_t bytes_transferred = 0;
		bytes_transferred = boost::asio::read(h, boost::asio::buffer(buf), ec);
		// std::size_t bytes_transferred = h.read_some(boost::asio::buffer(buf), ec);
		file_size += bytes_transferred;
		std::cout << buf.data();
		std::cout << file_size << std::endl;
	}

	h.close(ec);
	io.run();

	return 0;
}

