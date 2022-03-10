/*!****************************************************************************
	@file   	efvi_receive_snap.h
	
	@brief		�ṩ����lev2����
				�ṹ��qt_node_snap���ڼ�¼lev2������Ҫ�õ��������ֶ�
				ͨ�����մ�����handle_rx���յ������ݼ�¼�Ļ���ṹ����
				�ڳ����˳�ʱ����close������log_quote�����������¼���ļ���
******************************************************************************/
#pragma once
#include "efvi_receive_depend.h"

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

class udp_quote_snap
{
public:
	udp_quote_snap(void);
	~udp_quote_snap(void);

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

	/// ���������Դ
	long long			m_ll_snap_count;
	qt_node_snap			m_snap[QT_SNAP_COUNT];
};

