#include "socket_io.h"
#include "tcp_client_async.h"
#include "epoll_io_loop.h"
#include "TCPHandler.h"
#include "hpr/HPR_Utils.h"
int main(int argc, char **argv)
{
	CIOLoop io;
	io.Start();
	
	CTCPClientAsync client(&io);

	HPR_Sleep(100);
	//×¢²á¼àÌýÊÂ¼þ
	CTCPHandler handle;
	client.DoConnect.connect(&handle, &CTCPHandler::OnConnect);
	client.DoRecv.connect(&handle, &CTCPHandler::OnRecv);
	client.DoException.connect(&handle, &CTCPHandler::OnException);
	client.DoClose.connect(&handle, &CTCPHandler::OnClose);

	//io.Add_Handler(&client);
	//client.ConnectAsync("172.7.3.252", 8655);
	//client.ConnectAsync("172.7.3.11", 8655);
	client.ConnectAsync("223.232.233.4", 8655);
	//client.ConnectAsync("172.7.3.11", 8656);

	
	getchar();
	return 0;
}