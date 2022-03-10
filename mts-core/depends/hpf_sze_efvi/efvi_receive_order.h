/*!****************************************************************************
	@file   	efvi_receive_order.h
	
	@brief		�ṩ���ն�������
				�ṹ��qt_node_order���ڼ�¼����������Ҫ�õ��������ֶ�
				ͨ�����մ�����handle_rx���յ������ݼ�¼�Ļ���ṹ����
				�ڳ����˳�ʱ����close������log_quote�����������¼���ļ���
******************************************************************************/
#pragma once
#include "efvi_receive_depend.h"

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

class udp_quote_order
{
public:
	udp_quote_order(void);
	~udp_quote_order(void);

	bool init( sock_udp_param &shfe );
	void close();
private:
	int sl_parse(ef_filter_spec* fs, const char* ip,  unsigned short port);
	struct pkt_buf* pkt_buf_from_id(struct resources* res, int pkt_buf_i);
	void pkt_buf_free(struct resources* res, struct pkt_buf* pkt_buf);
	void handle_rx(struct resources* res, int pkt_buf_i, int len);
	void handle_rx_discard(struct resources* res, int pkt_buf_i, int len, int discard_type);
	void refill_rx_ring(struct resources* res);

	/// �����߳�
	bool start_recv_thread(); 
	static void* func_work_thread(void* param);
	int on_work_thread();
	void thread_main_loop(struct resources* res);
	/// ��¼��־����
	void write_log(const char* msg);
	/// ��CPU
	bool bind_cpu(int cpu_id, pthread_t thd_id);
	/// ��¼��������
	void log_quote();
private:
	bool				m_receive_quit_flag;	///<�˳��̵߳ı�־	
	struct resources*	m_res;					///<������Դ
	sock_udp_param		m_udp_param;			///<�鲥����
	
	/// ���������Դ
	long long			m_ll_order_count;
	qt_node_order		m_order[QT_ORDER_COUNT];
};

