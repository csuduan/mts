/*!****************************************************************************
	@file   	receive_ts.h
	
	@brief		�ṩ����timesale����
				�ṹ��qt_node_exe���ڼ�¼timesale������Ҫ�õ��������ֶ�
				ͨ�����մ�����on_receive_message���յ������ݼ�¼�Ļ���ṹ����
				�ڳ����˳�ʱ����close������log_quote�����������¼���ļ���
******************************************************************************/
#pragma once
#include "socket_multicast.h"

#define QT_TS_COUNT				(8000000)

#pragma pack(push, 1)

struct qt_node_exe
{
	unsigned int			m_seq;					/// ʢ���������
	char					m_symbol[9];			/// ��Լ
	short					m_channel_no;			/// Ƶ����
	long long				m_ll_seq_no;			/// ��Ϣ��¼��
	long long				m_ll_update_time;		/// ����ʱ��
	unsigned long long		m_local_time;			/// ���ؽ���ʱ��
	double					m_i_exe_px;				/// �ɽ��۸�
	long long				m_ll_exe_qty;			/// �ɽ���
	char					m_i_exe_type;			/// �ɽ�����
};

#pragma pack(pop)

class udp_quote_ts:public socket_event
{
public:
	udp_quote_ts(void);
	~udp_quote_ts(void);

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
	/// ������ռ�¼�����ֶ�
	long long			m_ll_exe_count;
	qt_node_exe			m_exe[QT_TS_COUNT];	
};

