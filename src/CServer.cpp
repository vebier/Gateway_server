#include "CServer.h"
#include "HttpConnection.h"
CServer::CServer(net::io_context& ioc,unsigned short& port):ioc_(ioc), 
acceptor_(ioc_,tcp::endpoint(tcp::v4(), port)), socket_(ioc)
{

}

void CServer::run()
{
	auto self(shared_from_this());
	acceptor_.async_accept(socket_,[self](boost::system::error_code ec){//�������Ӻ�Ļص�����
		try{//������try�����쳣����Ϊ��ֹacceptʧ���׳��쳣���±����޷��ջ�
			if (ec) {
				std::cerr << "accept Failed: " << ec.message() << std::endl;
				self->run();
				return;
			}
			std::make_shared<HttpConncation>(std::move(self->socket_))->run();
			//������һ���첽���ܲ���
			self->run();
		}
		catch (const std::exception& e) {
			std::cerr << "Exception in accept: " << e.what() << std::endl;
			self->run();
		}
	});
}