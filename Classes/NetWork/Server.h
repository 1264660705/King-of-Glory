/*
	*����Server�����
*/

#ifndef _SERVER_H_
#define _SERVER_H_

#include <cstdlib>
#include <iostream>
#include <memory>
#include<vector>
#include <boost/asio.hpp>
#include "Message.h"

using boost::asio::ip::tcp;

/*---------------chat_participant----------------
*ÿһ���ͻ��˼��붼�൱��һ��chat_participant����ֻ�ǻ��࣬��Ҫ����
*���Լ̳������඼��deliver�������൱������ÿ���ͻ��˴�����Ϣ
*/
class chat_participant
{
public:
	virtual ~chat_participant() {}
	virtual void deliver(const Message& msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

/*---------------chat_room-----------------------
*�൱�������ң���vector�洢��ÿ�����������ҵĿͻ���
*��Ϊֻ��1v1�������൱��ֻ�������ͻ���
*serverֻ�Ǹ����һ���ͻ��˵���Ϣ���ݸ�����һ��
*/
class chat_room
{
public:
	void join(chat_participant_ptr participant)
	{
		_participants.push_back(participant);
		if (participants_size() == 2)
		{
			is_connected = true;
		}
	}

	void leave(chat_participant_ptr participant)
	{
		_participants.pop_back();
		is_connected = false;
	}

	size_t participants_size()
	{
		return _participants.size();
	}

	/*
	*��һ���������vector[0]���������͹�������Ϣ������������Դ��A��
	*���������������A���򴫸�B(��vector[1])����֮�򴫸�A
	*������Ϣ���õ���deliver����������������������chat_session��deliver��������
	*/
	void deliver(const Message& msg);

	bool is_connected = false;

private:
	std::vector<chat_participant_ptr> _participants;
};

/*---------------chat_session---------------------
*������chat_participant��enable_shared_from_this������
*˵���������д�����Ϣ�ĺ�����Ҳ�ܹ�ͨ��shared_from_this()�����Լ�
*����Ҫ������ǣ��������һ���ͻ��ˣ����������������������ͻ��ˣ�����
*���ϵĴ�����ͻ��˶�ȡ��Ϣ����ȡ������ͨ������������һ���ͻ��˴�����Ϣ
*/
class chat_session
	: public chat_participant, public std::enable_shared_from_this<chat_session>
{
public:
	chat_session(tcp::socket socket, chat_room& room)
		: _socket(std::move(socket)), _room(room)
	{
	}

	void start();

	void deliver(const Message& msg);

private:
	void do_read_header();

	void do_read_body();

	void do_write();

	tcp::socket _socket;
	chat_room& _room;
	Message _read_msg;
	Message _write_msg;
};

/*---------------chat_server----------------------
*���ฺ��һֱ�ȵ��ͻ������ӣ������һ���ͻ������ӣ���ô������chat_session��
*Ȼ���ּ����ȴ������ͻ��˵�����
*/
class chat_server
{
public:
	chat_server(boost::asio::io_context& io_context, const tcp::endpoint& endpoint)
		: _acceptor(io_context, endpoint)
	{
		do_accept();
	}
	const chat_room& get_room()
	{
		return _room;
	}
private:
	void do_accept();

	tcp::acceptor _acceptor;
	chat_room _room;
};

/*---------------Server---------------------------
*ͨ�������һ���߳����������������Ҷ˿ںţ�10000
*detach������ʾ���̺߳����̷߳���
*/
class Server
{
public:
	Server()
	{
		std::thread t(&Server::init_server,this);
		t.detach();
	}

	void init_server();

	bool is_connected()
	{
		return _chat_server->get_room().is_connected;
	}

	//����Ϊ�˻��is_connected
	chat_server * _chat_server;
};

#endif //_SERVER_H_

