/*!****************************************************************************
	@file   	receive_order.h
	
	@brief		�ṩ���ն�������
				�ṹ��qt_node_order���ڼ�¼����������Ҫ�õ��������ֶ�
				ͨ�����մ�����on_receive_message���յ������ݼ�¼�Ļ���ṹ����
				�ڳ����˳�ʱ����close������log_quote�����������¼���ļ���
******************************************************************************/
#pragma once
#include "socket_multicast.h"

#define QT_ORDER_COUNT				(8000000)

#pragma pack(push, 1)

struct qt_node_order
{
	unsigned int		m_seq;						/// ʢ���������
	char				m_symbol[9];				/// ��Լ
	short				m_channel_no;				/// Ƶ����
	long long			m_ll_seq_no;				/// ��Ϣ��¼��
	long long			m_ll_update_time;			/// �������ʱ��
	unsigned long long	m_local_time;				///	���ؽ���ʱ��
	double				m_i_px;						///	�۸�
	long long			m_ll_qty;					/// ����
	char				m_i_side;					/// ����
	char				m_i_order_type;				/// ��������
};

#pragma pack(pop)

class udp_quote_order:public socket_event
{
public:
	udp_quote_order(void);
	~udp_quote_order(void);

	bool init( const char* remote_ip, unsigned short remote_port,const char* local_ip, unsigned short local_port );
	void close();

	/// \brief ���յ��鲥���ݵĻ�Ԙ�¼�
	virtual void on_receive_message(int id, const char* buff, unsigned int len);
private:
	/// ��¼��־����
	void write_log(const char* msg);
	/// ��¼��������
	void log_quote();
private:
	socket_multicast	m_sock;
	/// ���������Դ
	long long			m_ll_order_count;
	qt_node_order		m_order[QT_ORDER_COUNT];
};

