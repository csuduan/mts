/*!****************************************************************************
	@file   	receive_snap.h
	
	@brief		�ṩ����lev2����
				�ṹ��qt_node_snap���ڼ�¼lev2������Ҫ�õ��������ֶ�
				ͨ�����մ�����on_receive_message���յ������ݼ�¼�Ļ���ṹ����
				�ڳ����˳�ʱ����close������log_quote�����������¼���ļ���
******************************************************************************/
#pragma once
#include "socket_multicast.h"

#define QT_SNAP_COUNT				(8000000)

#pragma pack(push, 1)
struct qt_px_qty_unit
{
	double				m_price;				/// �۸�
	long long			m_qty;					/// ����
};

struct qt_node_snap
{
	unsigned int		m_seq;						/// ʢ���������
	char				m_symbol[9];			/// ��Լ
	long long			m_quote_update_time;	/// �������ʱ��
	unsigned long long	m_local_time;			/// ���ؽ���ʱ��
	// snp
	qt_px_qty_unit		m_bid_lev1;				/// �����һ��
	qt_px_qty_unit		m_bid_lev10;			/// �����ʮ��
	qt_px_qty_unit		m_ask_lev1;				/// �����һ��
	qt_px_qty_unit		m_ask_lev10;			/// �����ʮ��
	double				m_open_price;			/// ���̼�
	double				m_day_high;				/// ��߼�
	double				m_day_low;				/// ��ͼ�
	double				m_last_price;			/// ���¼�
	long long			m_total_qty;			/// �ɽ���
	long long			m_total_bid_qty;		/// �������
	long long			m_total_ask_qty;		/// ��������
};

#pragma pack(pop)

class udp_quote_snap:public socket_event
{
public:
	udp_quote_snap(void);
	~udp_quote_snap(void);

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
	socket_multicast		m_sock;
	/// ���������Դ
	long long				m_ll_snap_count;
	qt_node_snap			m_snap[QT_SNAP_COUNT];
};

