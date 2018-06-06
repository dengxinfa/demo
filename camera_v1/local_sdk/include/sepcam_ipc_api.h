#ifndef __SEPCAM_IPC_API_H__
#define __SEPCAM_IPC_API_H__

#include "ipcnet_struct.h"
#include "stream_frame_info.h"

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif
#ifndef INOUT
#define INOUT
#endif

//������������Ҫ���˳�
#define LOCAL_UPDATE_FLAG_FILE "/tmp/local_update_flag.txt"
//������Ҫ���˳�
#define RECORDER_EXITS_FLAG_FILE "/tmp/recorder_exist_flag.txt"
//Զ������Ҫ���˳�
#define REMOVE_UPDATE_FLAG_FILE "/tmp/UpgradeFlg.txt"

#ifdef __cplusplus
extern "C"
{
#endif

//-----------------���·���Ϊ�߼�����, ���ڻ�ȡYUV\RGB֡����-----------------------------------------------------------
int ipcam_start_vi_data(int data_type);
int ipcam_get_viframe(STREAM_FRAME_INFO_t *frameinfo, char *buffer, unsigned int bufflen, int *getframeIndex);
int ipcam_free_viframe(char *buffer);

/*
* @data_type: 0 - YUV420sp, 1-rgb_u3c3
* @info: ����
* @void: ����
*/
int ipcam_start_vframe(int data_type, void *info, void *info_ext);
int ipcam_get_vframe(int vchn, VFRAME_INFO_t *frame);
int ipcam_free_vframe(uint32_t frame_id);

void *ipcam_sys_mmap(uint32_t phy_addr, uint32_t size);
int ipcam_sys_munmap(void* phy_addr, uint32_t size);

//-----------------���Ϸ����߼�����, ���ڻ�ȡYUV\RGB֡����-----------------------------------------------------------


int sepcam_ipc_init(char *pocess_name);
int sepcam_stream_init();

/*
* ��Ƶ����
*/
int ipcam_get_vencstream(int venc_ch, STREAM_FRAME_INFO_t *frameinfo, char *buffer, unsigned int bufflen, int *getframeIndex);
int ipcam_query_vencstream_info(int venc_ch, STREAM_FRAME_INFO_t *frameinfo, int *getframeIndex);
int ipcam_flush_vencstream(int venc_ch);

/*
* ��Ƶ����
*/
int ipcam_get_aencstream(STREAM_FRAME_INFO_t *frameinfo, char *buffer, unsigned int bufflen, int *getframeIndex);
int ipcam_flush_aencstream();
int ipcam_get_pcmstream(STREAM_FRAME_INFO_t *frameinfo, char *buffer, unsigned int bufflen, int *getframeIndex);


#if 0 /**����ʹ��*/
/*
* ¼��ط�
*/
typedef enum
{
    IPCAM_SDK_RECORD_PLAY,
    IPCAM_SDK_RECORD_STOP,
    IPCAM_SDK_RECORD_PAUSE,
    IPCAM_SDK_RECORD_REPLAY,
}IPCAM_SDK_RECORD_OPT_e;

typedef struct
{
	unsigned short year;	// The number of year.
	unsigned char month;	// The number of months since January, in the range 1 to 12.
	unsigned char day;		// The day of the month, in the range 1 to 31.
	unsigned char wday;		// The number of days since Sunday, in the range 0 to 6. (Sunday = 0, Monday = 1, ...)
	unsigned char hour;     // The number of hours past midnight, in the range 0 to 23.
	unsigned char minute;   // The number of minutes after the hour, in the range 0 to 59.
	unsigned char second;   // The number of seconds after the minute, in the range 0 to 59.
}IPCAM_SDK_TIME_INFO_t;

typedef struct
{
    int opt;            //recorder req ope,refer to IPCAM_SDK_RECORD_OPT_e
    unsigned int vi_ch;         //sensor index
    char filename[128];     //���ŵ��ļ�����ΪNULL��ʹ��rec_timeȷ�������ĸ��ļ�
    IPCAM_SDK_TIME_INFO_t rec_time; //¼���ļ��Ŀ�ʼʱ�䣬����ȷ����Ӧʱ���¼���ļ�
}IPCAM_SDK_RECORD_REQ_t;

typedef struct
{
    int opt;
}IPCAM_SDK_RECORD_RESP_t;

int ipcam_rec_play(IPCAM_SDK_RECORD_REQ_t *RecReq);
int ipcam_rec_stop();
int ipcam_rec_pause(int pause); //1: ��ͣ 0: �ָ�
int ipcam_get_recstream(STREAM_FRAME_INFO_t *frameinfo, char *buffer, unsigned int bufflen, int *getframeIndex);
#endif

/**
 * \brief ipcam_recorder_play
 *
 * \param req [in] playback request info.
 *
 * \return playback id
 *
 */
int ipcam_recorder_play(IPCNET_RECORD_REQ_t *req);
int ipcam_recorder_ctrl(int id, int ctrl); //0: (�ָ�)���� 1: ��ͣ 2:����
int ipcam_recorder_speed(int id, int speed, int slow);
int ipcam_recorder_seek(int id, int seconds);
int ipcam_recorder_stop(int id);
int get_ipcam_recorder_stream(int id, STREAM_FRAME_INFO_t *frameinfo,
        char *buffer, unsigned int bufflen, int *getframeIndex);


int ipcam_get_device_config(IN uint16_t msg_type, IN void *out);
int ipcam_set_device_config(IN uint16_t msg_type, IN void *in);
int ipcam_device_config(uint16_t msg_type, void *in, void *out);
int ipcam_device_config_waittime(uint16_t msg_type, void *in, void *out, int waittime);
HANDLE_t ipcam_device_config_handle(uint16_t msg_type, void *in, void *out, int waittime);
int ipcam_close_device_config_handle(HANDLE_t handle);
 
/*
* ֱ��ʹ��json���ƣ�����ֵҲ��json����Ҫʹ��free_ipcam_json_config���ͷ�,
* @waittime: �ȴ�����ֵ, ����
*/
char *ipcam_device_json_config(uint16_t msg_type, char *in_json, int waittime);
void free_ipcam_json_config(char *ipc_json);


/*
* @user: ipcam_wifi_ap_search�Ĳ���user_data��һ�ݿ���
*/
typedef void (*SEARCH_WIFI_AP_CB)(IpcnetNetworkWirelessSearch_st *ap, void *user);
/*
* @cb:          Wifi�����ӿ����첽�ģ�����������������cb�ᱻ����
* @user_data��   Ϊ�����û�ʹ�ã�user_dataΪ���ص�������ʱ�������û�ϣ���������������ݵĿ���
* @data_len:    user_data�ĳ���
*/
int ipcam_wifi_ap_search(SEARCH_WIFI_AP_CB cb, void *user_data, size_t data_len);

//storage format callback
typedef void (*STORAGE_FORMAT_CB)(IPCNetRet_st *pFomatResult, void *user);
int ipcam_storage_format(STORAGE_FORMAT_CB cb, void *user_data, size_t data_len);

typedef enum
{
    IPCAM_SPEAKER_STATUS_NORMAL,
    IPCAM_SPEAKER_STATUS_ABNORMAL_DISCONNECT,
}IPCAM_SPEAKER_STATUS_e;
typedef void (*IPCAM_SPEAKER_STATUS_CB_FN)(void *, IPCAM_SPEAKER_STATUS_e status);

/*
* warning: ipcam_speaker_start,ipcam_send_speaker_data,ipcam_speaker_end are obsolescent,
* use ipcam_adec_start,ipcam_send_adec_data,ipcam_adec_end instead
*/
int ipcam_speaker_start(IPCAM_SPEAKER_STATUS_CB_FN callback, void *);
int ipcam_send_speaker_data(char *data, size_t data_size, void *reserve);
int ipcam_speaker_end();

/*
* @ipcam_adec_start: return a HANDLE if success. The HANDLE MUST be closed by ipcam_adec_end when the audio decode is over.
*/
HANDLE_t ipcam_adec_start(void *reserve);
int ipcam_send_adec_data(HANDLE_t handle, char *data, size_t data_size, void *reserve);
int ipcam_adec_end(HANDLE_t handle);

typedef void (*IPCAM_ALARM_EVENT_CB)(IPCNetAlarmMsgReport_st *alarm_msg);
int ipcam_regist_alarm_event_callback(IPCAM_ALARM_EVENT_CB callback);

//�������Ȼص�
typedef void (*IPCAM_UPGRADE_STATUS_CB)(IPCNetUgradeResp_st *pUpgradeSta);
int ipcam_regist_upgrade_status_callback(IPCAM_UPGRADE_STATUS_CB callback);

//�������Ȼص�
typedef void (*IPCAM_UPDATE_WIFI_STATUS_CB)(IPCNetUpdateWirelessInfo_st *pUpdateWifiSta);
int ipcam_regist_upgrade_wifi_status_callback(IPCAM_UPDATE_WIFI_STATUS_CB callback);

//����ʶ��ص�
typedef void (*IPCAM_FR_EVENT_CB)(int type, void *pPicInfo, char *pPicBuf,int PicLen);
int sepcam_fr_init(IPCAM_FR_EVENT_CB callback);


typedef enum
{
    IPCAM_STATUS_NORMAL,
    IPCAM_STATUS_ABNORMAL_DISCONNECT,
    IPCAM_STATUS_RECONNECT,
}IPCAM_STATUS_e;
typedef void (*IPCAM_STATUS_CB_FN)(void *, IPCAM_SPEAKER_STATUS_e status);
int sepcam_ipc_init_ex(IPCAM_STATUS_CB_FN callback, void *);

//ץ�Ļص�
typedef struct
{
	unsigned char *addr;	//��Ƭ�ĵ�ַ
	unsigned int len;		//��Ƭ�ĳ���
}SNAP_PAKT_t;

typedef struct
{
	int frame_size;						//֡��С
	unsigned long long int time_stamp;	//��ȷ��΢�뼶��ʱ��� 
	int pkt_num;						//��Ƭ�ĸ���
	SNAP_PAKT_t *pkt;					//��Ƭ�ĵ�ַ�ͳ��ȵ�����
}SNAP_FRAME_t;

typedef void (*IPCAM_SNAP_PIC_CB)(SNAP_FRAME_t *,void *);
int ipcam_snap_shoot(IPCAM_SNAP_PIC_CB cb, void *user_data, int data_len);

#ifdef __cplusplus
}
#endif

#endif

