/*!****************************************************************************
	@file   	efvi_receive_ts.h
	
	@brief		�ṩ����timesale����
				�ṹ��qt_node_exe���ڼ�¼timesale������Ҫ�õ��������ֶ�
				ͨ�����մ�����handle_rx���յ������ݼ�¼�Ļ���ṹ����
				�ڳ����˳�ʱ����close������log_quote�����������¼���ļ���
******************************************************************************/
#pragma once
#include "efvi_receive_depend.h"

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

class udp_quote_ts
{
public:
	udp_quote_ts(void);
	~udp_quote_ts(void);

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
	sock_udp_param		m_udp_param;			///<�鲥����
	bool				m_receive_quit_flag;	///<�˳��̵߳ı�־	
	struct resources*	m_res;					///<������Դ

	/// ������ռ�¼�����ֶ�
	long long			m_ll_exe_count;
	qt_node_exe			m_exe[QT_TS_COUNT];	
};

