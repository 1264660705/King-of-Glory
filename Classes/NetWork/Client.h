/*
	*Client�����
*/
#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <cstdlib>
#include <deque>
#include<vector>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <string>
#include "Message.h"

using boost::asio::ip::tcp;

typedef std::deque<Message> Message_queue;

/*---------------chat_client---------------------------
*������client�ĵײ��࣬���𲻶ϵĴӷ������ж�ȡ��Ϣ������
*����Ϣѹ��deque���棬Client��get_message()���Ƕ�ȡ����
*�� _message_deque����ȡ��Ϣ��
*/
class chat_client
{
public:
	chat_client(boost::asio::io_context& io_context, const tcp::resolver::results_type& endpoints)
		: _io_context(io_context), _socket(io_context)
	{
		do_connect(endpoints);
	}

	void write(const Message& msg);

	void close();

	//std::deque<std::string>     _message_deque;             //���ܵ�����Ϣ�ļ���
	std::vector<std::string>     _message_deque;             //���ܵ�����Ϣ�ļ���

private:
	void do_connect(const tcp::resolver::results_type& endpoints);

	void do_read_header();

	void do_read_body();

	void do_write();

private:
	boost::asio::io_context& _io_context;
	tcp::socket _socket;
	Message _read_msg;
	Message_queue _write_msgs;
};

/*---------------Client---------------------------
*ͨ�������һ���߳�����client�����Ҷ˿ںţ�10000
*detach������ʾ���̺߳����̷߳���
*��ʼ��ʱҪ���client�ı�ʶ���Ա㷢����Ϣʱ��������
*send_message������get_message()�Ǹ������Ҫ����
*��������ʵ��������ʱ������λ���շ���Ϣ
*get_message()һ��ֻ��ȡһ�����Ҷ�deque��������
*/
class Client
{
public:
	Client(const std::string& head) :_head(head)
	{
		std::thread t(&Client::init_client, this);
		t.detach();
	}

	void init_client();

	void send_message(std::string message);

	//std::string get_message();

	std::vector<std::string> get_messages();

	chat_client* _chat_client;
	std::string _head;
};

#endif //_CLIENT_H_


