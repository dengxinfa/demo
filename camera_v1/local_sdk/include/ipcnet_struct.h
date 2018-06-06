#ifndef _IPCNET_NET_STRUCT_H
#define _IPCNET_NET_STRUCT_H

#include "os_ini.h"

#define IPCNET_STRING_LEN 32
#define IPCNET_IP_STRING_LEN 32
#define IPCNET_NET_MAX_AREA 256   //ÿ��ֵһ��λ һ����12*16=192�� 1����ѡ�� 0 ûѡ��
#define IPCNET_VIDEO_CHANNEL_NUM 2
#define IPCNET_AUDIO_CHANNEL_NUM 1
#define IPCNET_WIFI_AP_MAX_LEN 40

#ifndef MAX_PATH
#define MAX_PATH	260
#endif

typedef struct IPCNET_CMD_MSG_HEAD
{
    char magic[4];
    
    T_U16  msg_type;
    //T_U16  ext_msg_type; // ��Э�鲻ʹ�����
    T_U8 pkt_num;
    T_U8 pkt_id;
    
    T_U32  guid;
    T_U32  result;//0��ʾ�ɹ��������Ĵ����1��ʼ
    T_U32  version;//(V 1.0.0)
    
    T_U32  msg_head_len;//sizeof(msg_head) + ��չ���ȣ�;
    T_U32  msg_len;//(msg_head_len + ʵ�����ݳ��� )��
    T_U32  timestamp;
    T_U32  sequence;//���к�

    T_S8  res[40];
}IPCNET_CMD_MSG_HEAD_t;

typedef enum
{
	IPCNET_LOGIN_REQ = 1000,
	IPCNET_LOGIN_RESP,

	IPCNET_KEEPALIVE_REQ,
	IPCNET_KEEPALIVE_RESP,

	IPCNET_STREAM_REQ,
	IPCNET_STREAM_RESP,

	IPCNET_TALK_REQ,
	IPCNET_TALK_RESP,

	IPCNET_FORCELOGOUT_REQ,
	IPCNET_FORCELOGOUT_RESP,

    IPCNET_STREAM_EX_REQ,
    IPCNET_STREAM_EX_RESP,
    
	IPCNET_NET_SYSINFO_REQ = 1020,
	IPCNET_NET_SYSINFO_RESP,

	IPCNET_USER_SET_REQ,
	IPCNET_USER_SET_RESP,

	IPCNET_MCU_INFO_REQ,
	IPCNET_MCU_INFO_RESP,
	
	IPCNET_ETH_SET_REQ = 1040,  //IPCNetEthConfig_st
	IPCNET_ETH_SET_RESP,
	
    IPCNET_ETH_GET_REQ = 1042,
    IPCNET_ETH_GET_RESP,    //IPCNetEthConfig_st
    
    IPCNET_NETWORK_MOBILE_SET_REQ = 1044, //Ipcnet3GInfo_st
    IPCNET_NETWORK_MOBILE_SET_RESP,

    IPCNET_NETWORK_MOBILE_GET_REQ = 1046,
    IPCNET_NETWORK_MOBILE_GET_RESP, //Ipcnet3GInfo_st

    IPCNET_NETWORK_WIFI_SET_REQ = 1048, //IPCNetWirelessConfig_st
    IPCNET_NETWORK_WIFI_SET_RESP,
    
    IPCNET_NETWORK_WIFI_GET_REQ = 1050,
    IPCNET_NETWORK_WIFI_GET_RESP,   //IPCNetWirelessConfig_st
    
    IPCNET_NETWORK_WIFI_SEARCH_GET_REQ = 1052,
    IPCNET_NETWORK_WIFI_SEARCH_GET_RESP,  //IpcnetNetworkWirelessSearch_st

    IPCNET_VIDEO_ENC_SET_REQ = 1054,    //IPCNetVideoEncodeCfg_st
    IPCNET_VIDEO_ENC_SET_RESP,

    IPCNET_VIDEO_ENC_GET_REQ = 1056,
    IPCNET_VIDEO_ENC_GET_RESP,  //IPCNetVideoEncodeCfg_st
    
    IPCNET_AUDIO_ENC_SET_REQ = 1058,   //IpcnetAudioEncodeCfg_st
    IPCNET_AUDIO_ENC_SET_RESP,

    IPCNET_AUDIO_ENC_GET_REQ = 1060,
    IPCNET_AUDIO_ENC_GET_RESP,  //IpcnetAudioEncodeCfg_st

    IPCNET_PTZ_GET_REQ = 1062,
    IPCNET_PTZ_GET_RESP,

    IPCNET_PTZ_SET_REQ = 1064,     //IPCPtzCtrlMsg_st
    IPCNET_PTZ_SET_RESP,
    
    IPCNET_MOVE_ALARM_SET_REQ = 1066, //IPCNETMoveCellAlarmCfg_st
    IPCNET_MOVE_ALARM_SET_RESP,

    IPCNET_MOVE_ALARM_GET_REQ = 1068,
    IPCNET_MOVE_ALARM_GET_RESP,

	IPCNET_AV_RECO_CONF_SET_REQ = 1070, //IPCNetRecordCfg_st
    IPCNET_AV_RECO_CONF_SET_RESP,

    IPCNET_AV_RECO_CONF_GET_REQ = 1072,
    IPCNET_AV_RECO_CONF_GET_RESP, //IPCNetRecordCfg_st

	IPCNET_AV_RECO_LIST_GET_REQ = 1074,
    IPCNET_AV_RECO_LIST_GET_RESP,

	IPCNET_AV_RECO_LIST_PAGE_GET_REQ = 1076,
    IPCNET_AV_RECO_LIST_PAGE_GET_RESP,

	IPCNET_AV_RECO_OP_REQ = 1078,
    IPCNET_AV_RECO_OP_RESP,     // 1079

	IPCNET_UPGRADE_REQ = 1080,
	IPCNET_UPGRADE_RESP,

	IPCNET_GET_GPIO_REQ = 1082,
	IPCNET_GET_GPIO_RESP,
	
	IPCNET_SET_GPIO_REQ = 1084,
	IPCNET_SET_GPIO_RESP,

	IPCNET_GET_SUBDEV_REQ = 1086,
	IPCNET_GET_SUBDEV_RESP,
	
	IPCNET_SET_SUBDEV_REQ = 1088,
	IPCNET_SET_SUBDEV_RESP,

	IPCNET_GET_BUS_REQ = 1090,
	IPCNET_GET_BUS_RESP,
	
	IPCNET_SET_BUS_REQ = 1092, //����͸��
	IPCNET_SET_BUS_RESP,    // 1093

	IPCNET_GET_DEV_INFO_REQ = 1094,
	IPCNET_GET_DEV_INFO_RESP,

	IPCNET_SET_DEV_INFO_REQ = 1096,
	IPCNET_SET_DEV_INFO_RESP,
	
	IPCNET_UPGRADE_CFG_REQ = 1098,
	IPCNET_UPGRADE_CFG_RESP,
	
	IPCNET_UPGRADE_AUTO_SET_REQ = 1100,
	IPCNET_UPGRADE_AUTO_SET_RESP,
	
	IPCNET_SET_DEFAULT_REQ = 1102,
    IPCNET_SET_DEFAULT_RESP,

	IPCNET_SET_REBOOT_REQ = 1104,
    IPCNET_SET_REBOOT_RESP,
    
    IPCNET_GET_NET_PORT_REQ = 1106,
    IPCNET_GET_NET_PORT_RESP,
    
    IPCNET_SET_NET_PORT_REQ = 1108,
    IPCNET_SET_NET_PORT_RESP,

	IPCNET_GET_GPIO_ARRAY_REQ = 1110,
	IPCNET_GET_GPIO_ARRAY_RESP,
	
	IPCNET_SET_GPIO_ARRAY_REQ = 1112,
	IPCNET_SET_GPIO_ARRAY_RESP,

    IPCNET_GET_MOTION_DETECT_REQ = 1114,
    IPCNET_GET_MOTION_DETECT_RESP,
    
    IPCNET_SET_MOTION_DETECT_REQ = 1116,
    IPCNET_SET_MOTION_DETECT_RESP,

	IPCNET_WIFI_STATUS_REPORT_RESP = 1201,       // 1201

    IPCNET_VIDEO_IFRAME_REQ = 1202,   //IPCNetVideoIFrame_st
    IPCNET_VIDEO_IFRAME_RESP,

    IPCNET_GET_VIDEO_QULITY_LEVEL_REQ = 1204,
    IPCNET_GET_VIDEO_QULITY_LEVEL_RESP,
    
    IPCNET_SET_VIDEO_QULITY_LEVEL_REQ = 1206,
    IPCNET_SET_VIDEO_QULITY_LEVEL_RESP,
    
    IPCNET_GET_NETWORKS_STATUS_REQ = 1208,
    IPCNET_GET_NETWORKS_STATUS_RESP,
	
	IPCNET_NET_CFG_PTZ_REQ = 1400,
	IPCNET_NET_CFG_PTZ_RESP,

	IPCNET_GET_TIME_REQ = 1402, //IPCNetGetTimeReq_st
	IPCNET_GET_TIME_RESP, //IPCNetTimeCfg_st
	
	IPCNET_SET_TIME_REQ = 1404, //IPCNetTimeCfg_st
	IPCNET_SET_TIME_RESP,   //1405

    IPCNET_NET_PRESET_SET_PTZ_REQ = 1406,
	IPCNET_NET_PRESET_SET_PTZ_RESP, //IPCNetTimeCfg_st

    IPCNET_NET_PRESET_GET_PTZ_REQ = 1408,      
	IPCNET_NET_PRESET_GET_PTZ_RESP,

	//---����PC�ƹ�(��ͨ����) 
	IPCNET_GET_LAMP_REQ = 1410,
	IPCNET_GET_LAMP_RESP,
	
	IPCNET_SET_LAMP_REQ = 1412,
	IPCNET_SET_LAMP_RESP,

	//----ͼ��ת
	IPCNET_GET_OVERTURN_REQ = 1414,
	IPCNET_GET_OVERTURN_RESP,       //1415
	
	IPCNET_SET_OVERTURN_REQ = 1416,	
	IPCNET_SET_OVERTURN_RESQ,	
	
	/**�����ع�����**/
	IPCNET_GET_EXPOSURE_TYPE_REQ = 1418,
	IPCNET_GET_EXPOSURE_TYPE_RESP,
	
	IPCNET_SET_EXPOSURE_TYPE_REQ = 1420,
	IPCNET_SET_EXPOSURE_TYPE_RESP,

	/**�Զ��ع�**/
	IPCNET_GET_AUTO_EXPOSURE_REQ = 1422,
	IPCNET_GET_AUTO_EXPOSURE_RESP,

	IPCNET_SET_AUTO_EXPOSURE_REQ = 1424,
	IPCNET_SET_AUTO_EXPOSURE_RESP,      //1425

	/**�ֶ��ع�**/
	IPCNET_GET_MANUAL_EXPOSURE_REQ = 1426,
	IPCNET_GET_MANUAL_EXPOSURE_RESP,

	IPCNET_SET_MANUAL_EXPOSURE_REQ = 1428,
	IPCNET_SET_MANUAL_EXPOSURE_RESP,

	//----��ת�ڷ�ʽ
	IPCNET_GET_PICOLOR_REQ = 1430,	 /*��ȡ��ת��*/         //1430
	IPCNET_GET_PICOLOR_RESP,
	
	IPCNET_SET_PICOLOR_REQ = 1432, /*���ò�ת��*/
	IPCNET_SET_PICOLOR_RESP, 
	
	//----��������
	IPCNET_GET_ENVIRONMENT_REQ = 1434,		/*��ȡ����*///1--outdoorģʽ 0--indoorģʽ
	IPCNET_GET_ENVIRONMENT_RESP,
	
	IPCNET_SET_ENVIRONMENT_REQ = 1436,		/*���û���*///1--outdoorģʽ 0--indoorģʽ
	IPCNET_SET_ENVIRONMENT_RESP,

	//---2D/3D����
	IPCNET_GET_DENOISE_REQ = 1438,		/*1438 ��ȡ2D/3D ����*/	
	IPCNET_GET_DENOISE_RESP,
	
	IPCNET_SET_DENOISE_REQ = 1440,	/*����2D/3D ����*/          //1440
	IPCNET_SET_DENOISE_RESP,        

	//v1---��̬����
	IPCNET_GET_WDR_REQ = 1442,		/* 1442 ��ȡWDR*/
	IPCNET_GET_WDR_RESP,
	
	IPCNET_SET_WDR_REQ = 1444,		/*����WDR*/
	IPCNET_SET_WDR_RESP,		//1445

	//--��ƽ��
	IPCNET_GET_WH_BLANCE_REQ = 1446,
	IPCNET_GET_WH_BLANCE_RESP,
	
	IPCNET_SET_WH_BLANCE_REQ = 1448,
	IPCNET_SET_WH_BLANCE_RESP,

	//--��֡
	IPCNET_GET_SLOW_FRAME_RATE_REQ = 1450,
	IPCNET_GET_SLOW_FRAME_RATE_RESP,     
	
	IPCNET_SET_SLOW_FRAME_RATE_REQ = 1452,
	IPCNET_SET_SLOW_FRAME_RATE_RESP,
	
	//--����Ĭ��ֵ
	IPCNET_GET_EXPOSURE_DEFAULT_REQ = 1454,		//δ�õ�
	IPCNET_GET_EXPOSURE_DEFAULT_RESP,       //1455
	
	IPCNET_SET_EXPOSURE_DEFAULT_REQ = 1456,      
	IPCNET_SET_EXPOSURE_DEFAULT_RESP,

	//--����У��
	IPCNET_GET_BADPIXEL_DETECT_REQ = 1458, 	//δ�õ�
	IPCNET_GET_BADPIXEL_DETECT_RESP,
	
	IPCNET_SET_BADPIXEL_DETECT_REQ = 1460,
	IPCNET_SET_BADPIXEL_DETECT_RESP,       
	
	IPCNET_GET_ANTIFLICKER_REQ = 1462,	 	/*1462 ��ȡ������*/	
	IPCNET_GET_ANTIFLICKER_RESP,
	
	IPCNET_SET_ANTIFLICKER_REQ = 1464,		 /*���ÿ�����*/
	IPCNET_SET_ANTIFLICKER_RESP,        //1465
	
	//wdr v2---����������������ɾ��������Ӱ����������
	IPCNET_GET_RESERVE_REQ = 1466,		
	IPCNET_GET_RESERVE_RESP,
	
	IPCNET_SET_RESERVE_REQ = 1468,		
	IPCNET_SET_RESERVE_RESP,

	//----�Զ���ת���ֶ����ù���ֵ
	IPCNET_GET_PICOLOR4MANUlUAM_REQ = 1470,
	IPCNET_GET_PICOLOR4MANUlUAM_RESP,   
	
	IPCNET_SET_PICOLOR4MANUlUAM_REQ = 1472, 
	IPCNET_SET_PICOLOR4MANUlUAM_RESP,

    IPCNET_SNAP_SHOOT_REQ = 1474,
	IPCNET_SNAP_SHOOT_RESP,     //1475

    //Ԥ�õ�
	IPCNET_GET_PREPOINT_REQ = 1476,
	IPCNET_GET_PREPOINT_RESP,       //1477    

	IPCNET_SET_PREPOINT_REQ = 1478,
	IPCNET_SET_PREPOINT_RESP,       //1479
    
	IPCNET_OPERATE_PREPOINT_REQ = 1480,
	IPCNET_OPERATE_PREPOINT_RESP,       //1481

    //ftp��������
	IPCNET_SET_FTP_CFG_REQ = 1482, //IPCNetFtpCfg_st
	IPCNET_SET_FTP_CFG_RESP,

	IPCNET_GET_FTP_CFG_REQ = 1484,
	IPCNET_GET_FTP_CFG_RESP,     //1485 //IPCNetFtpCfg_st

    /***Email cfg***/
	IPCNET_SET_EMAIL_CFG_REQ = 1486,
	IPCNET_SET_EMAIL_CFG_RESP,

	IPCNET_GET_EMAIL_CFG_REQ = 1488,
	IPCNET_GET_EMAIL_CFG_RESP,

    /***Ddns cfg***/
	IPCNET_SET_DDNS_CFG_REQ = 1490,
	IPCNET_SET_DDNS_CFG_RESP,

	IPCNET_GET_DDNS_CFG_REQ,
	IPCNET_GET_DDNS_CFG_RESP,

    /***�������ȣ��Աȶȣ����Ͷ�***/
	IPCNET_SET_CAM_PIC_CFG_REQ = 1494,
	IPCNET_SET_CAM_PIC_CFG_RESP,        //1495

	IPCNET_GET_CAM_PIC_CFG_REQ = 1496,
	IPCNET_GET_CAM_PIC_CFG_RESP,

	/****IRCUT�л�****/
	IPCNET_SET_IRCUT_REQ = 1498,
	IPCNET_SET_IRCUT_RESP,

	IPCNET_GET_IRCUT_REQ = 1500,
	IPCNET_GET_IRCUT_RESP,

	IPCNET_ALARM_REPORT_RESP = 1502,       // 1502�����ϴ�������Ϣ

    /****OSD cfg****/
	IPCNET_SET_OSD_REQ = 1503,
	IPCNET_SET_OSD_RESP = 1504,
    
	IPCNET_GET_OSD_REQ = 1505,
	IPCNET_GET_OSD_RESP = 1506,

	IPCNET_FORMAT_EXFAT_REQ = 1507,
	IPCNET_FORMAT_EXFAT_RESP,

	IPCNET_BINARY_DATA_MSG_REQ =   1509, //������������Ϣ
	IPCNET_BINARY_DATA_MSG_RESP,

	IPCNET_START_REMOTE_DEBUG_MSG_REQ = 1511, //����Զ�̵���
	IPCNET_START_REMOTE_DEBUG_MSG_RESP,

	IPCNET_END_REMOTE_DEBUG_MSG_REQ = 1513, //�ر�Զ�̵���
	IPCNET_END_REMOTE_DEBUG_MSG_RESP,

	IPCNET_GET_FUNC_CFG_REQ = 1515, //���ܲ�����ȡ
	IPCNET_GET_FUNC_CFG_RESP,    

    IPCNET_SET_FUNC_CFG_REQ = 1517, //���ܲ�������
	IPCNET_SET_FUNC_CFG_RESP,    

	IPCNET_GET_STA_LED_CFG_REQ = 1519, //��ȡ�Ƶ�ʹ��״̬
	IPCNET_GET_STA_LED_CFG_RESP,

    IPCNET_SET_STA_LED_CFG_REQ = 1521, //���õƵ�ʹ��
	IPCNET_SET_STA_LED_CFG_RESP,

    IPCNET_GET_DET_LUM_AREA_CFG_REQ = 1523, //��ȡ�������ȼ�����
    IPCNET_GET_DET_LUM_AREA_CFG_RESP,       

    IPCNET_SET_DET_LUM_AREA_CFG_REQ = 1525, //�����������ȼ�����
    IPCNET_SET_DET_LUM_AREA_CFG_RESP,  

    IPCNET_DISK_TEST_REQ = 1527, //disk����
    IPCNET_DISK_TEST_RESP,

    IPCNET_SET_P2P_SER_CODE_REQ = 1529, //����p2p����������
    IPCNET_SET_P2P_SER_CODE_RESP,

    IPCNET_GET_P2P_SER_CODE_REQ = 1531,        // 1531 ��ȡp2p����������
    IPCNET_GET_P2P_SER_CODE_RESP,

#if 0
    IPCNET_GET_NET_MAN_CFG_REQ,         // 1533 --����ѡ����Ϣ
    IPCNET_GET_NET_MAN_CFG_RESQ,
    
    IPCNET_SET_NET_MAN_CFG_REQ,         // 1535 --����ʹ���ĸ�����
    IPCNET_SET_NET_MAN_CFG_RESQ,
#endif  

    IPCNET_GET_TIMER_CFG_REQ = 1537,       //��ȡ��ʱ������
    IPCNET_GET_TIMER_CFG_RESP,

    IPCNET_SET_TIMER_CFG_REQ = 1539,       //��ȡ��ʱ������
    IPCNET_SET_TIMER_CFG_RESP,

    IPCNET_GET_DISK_CFG_REQ = 1541,        //��ȡ���̲���
    IPCNET_GET_DISK_CFG_RESP,

	//����ʶ���������
    IPCNET_FR_GET_USER_REQ = 1543, //����ʶ���ȡ��ǰ���õ��û���Ϣ
    IPCNET_FR_GET_USER_RESP,

	IPCNET_FR_COLLECTION_REQ = 1545, //����ʶ�𣬲ɼ�������Ϣ
    IPCNET_FR_COLLECTION_RESP,

	IPCNET_FR_ADD_USER_BY_COLLECT_REQ = 1547, //���ݲɼ���������Ϣ����û�
    IPCNET_FR_ADD_USER_BY_COLLECT_RESP,

	IPCNET_FR_ADD_USER_BY_VISIT_HISTORY_REQ = 1549, //��������ʷ��¼������û�
    IPCNET_FR_ADD_USER_BY_VISIT_HISTORY_RESP,

	IPCNET_FR_MODIFY_USER_INFO_REQ = 1551, //�޸��û���Ϣ
    IPCNET_FR_MODIFY_USER_INFO_RESP,

	IPCNET_FR_DEL_USER_INFO_REQ = 1553, //ɾ���û���Ϣ
    IPCNET_FR_DEL_USER_INFO_RESP,

	IPCNET_FR_GET_VISIT_HISTORY_REQ = 1555, //��ȡ���ü�¼
    IPCNET_FR_GET_VISIT_HISTORY_RESP,

	IPCNET_FR_FUNC_ENABLE_REQ = 1557, //������⹦�ܿ���
    IPCNET_FR_FUNC_ENABLE_RESP,

	IPCNET_FR_CANCEL_COLLECTION_REQ = 1559, //ȡ�������ɼ�����
    IPCNET_FR_CANCEL_COLLECTION_RESP,

    IPCNET_FR_DETECT_PIC = 1561,			//��⵽����

    IPCNET_FR_VISITOR_PER_MIN_REQ = 1562,			//ÿ���ӵ�������
    IPCNET_FR_VISITOR_PER_MIN_RESP = 1563,

    //------------------------Station(���塢nvr)Э��------------------------
    IPCNET_GET_STATION_PROFILES_REQ = 2000,
    IPCNET_GET_STATION_PROFILES_RESP, //IPCNetDevProfiles_st
    
    IPCNET_SET_STATION_OPT_DEV_REQ = 2002, //IPCNetSta_OptDev_st
    IPCNET_SET_STATION_OPT_DEV_RESP,

    IPCNET_GET_P2P_WAKEUP_SETTING_REQ = 2004,        //IPCNetP2PWakeUpSetting_st
    IPCNET_GET_P2P_WAKEUP_SETTING_RESP,

    
    
    //!!!---------------!!!����������Ҫ��sdkЭ�����չ����������Э��----------
    IPCNET_INTERNEL_MSG_TYPE = 6000,

    IPCNET_GET_USER_ALL_INFO_REQ = 6002,
    IPCNET_GET_USER_ALL_INFO_RESP,  //IPCNetGetUserGroupCfg_st

    IPCNET_GET_USER_INFO_REQ = 6004,    //IPCNetUserInfoReq_st
    IPCNET_GET_USER_INFO_RESP,  //IPCNetUserInfo_st
    
    IPCNET_SET_USER_INFO_REQ = 6006,    //IPCNetUserInfo_st
    IPCNET_SET_USER_INFO_RESP,

    IPCNET_GET_SYS_INFO = 6008,      //IPCNetCamInfo_st    
    IPCNET_GET_SYS_INFO_RESP,

    IPCNET_GET_UPGRADE_URL_REQ = 6010,      
    IPCNET_GET_UPGRADE_URL_RESP,

    IPCNET_UPDATE_WIRELESS_STATUS = 6012, //����wifi��״̬

    //--------recorder app--------------------
    IPCNET_PLAY_RECORD_REQ = 6100, //IPCNET_RECORD_REQ_t
    IPCNET_PLAY_RECORD_RESP, //IPCNET_RECORD_RESP_t

    IPCNET_RECORD_OPT_REQ = 6101, //IPCNET_RECORD_OPT_t
    IPCNET_RECORD_OPT_RESP,
    
    IPCNET_MSG_TYPE_BUTT = 10000,
    /*
    * �������µ������Ǹ�ֱ�Ӷ����ṩ�ӿڵ���ʽʹ�ã�
    * ��Щ������п��ܻ�䶯�Ƚ϶�
    * ��sepcam_ipc_internal.h IPC_CFG_INTERNEL_COMMAND_e
    */
}IPCNET_NET_MSG_TYPE_e;


typedef enum
{
    IPCNET_RET_OK,      //0�ɹ�
    
	_IPCNET_RET_OK_ = 100,     //����ǲ��õ�
	IPCNET_NET_RET_UNKNOWN,          //�����������101��ʼ
	IPCNET_NET_RET_VERSION_NOT_SUPPORT,
	IPCNET_RET_REQ_ILLEGAL,
	IPCNET_NET_RET_LOGIN_EXSIST,

	IPCNET_RET_UNLOGIN = 105, //5
	IPCNET_RET_PASSWARD_ERR,
	IPCNET_RET_UNAUTHORISED,
	IPCNET_RET_TIMEOUT,
	IPCNET_RET_NOT_FOUND,

    IPCNET_RET_USER_NOT_FOUND = 110,
	IPCNET_RET_PASSWARD_NULL,
	IPCNET_NET_RET_FIND_AND_SEND_OVER = 112,
	IPCNET_NET_RET_FIND_AND_SEND_APART,
	IPCNET_RET_SENDBUF_MEM_NOTENOUGH = 114,

    IPCNET_RET_PARSE_FAIL = 115,
    
	IPCNET_NET_RET_NEED_RESTART_APP = 602,
	IPCNET_NET_RET_NEED_RESTART_SYS,
	IPCNET_NET_RET_WRITE_FILE_ERR,
	
	IPCNET_NET_RET_REQ_NOT_SUPPORT = 605,
	IPCNET_NET_RET_NEED_VERIFY_ERR,
	IPCNET_NET_RET_NEED_CONFIG_UNEXIST,
	IPCNET_NET_RET_NEED_PASER_ERR,
	IPCNET_RET_NO_MEM,

	IPC_NET_RET_UPDATING = 610,
	IPC_NET_RET_OPERATE_BUSYING,
	IPCNET_RET_USER_NAME_TOO_LONG,
	IPCNET_RET_PASSWD_TOO_LONG,
	IPCNET_RET_RESOURCE_ERROR,
	
	IPCNET_NET_RET_REQ_ID_UNKNOWN = 1001,
	IPCNET_NET_RET_REQ_UNKNOWN,

	IPCNET_RET_FORMAT_FAIL =1050,
	
	IPCNET_NET_RET_NULL = 0xFFFFFF, //���ֵ������Э�鷶Χ����Ϊ���ô�����on_ipcnet_msg_process()֪������������첽�ģ����������ظ���
}IPCNET_RET_CODE_e;

typedef enum
{
	DEV_TYPE_SWITCH_AND_OUTLET = 0,
	DEV_TYPE_SWITCH = 1,
	DEV_TYPE_OUTLET = 2,//����
	DEV_TYPE_SCENE_PANEL = 3,
	DEV_TYPE_IR_CONTROL_UNIT = 4,//����ת����(ң����)
	DEV_TYPE_ROUTER = 5,   
	DEV_TYPE_LIGHT = 6,
	DEV_TYPE_LIGHT_SENSOR = 7,
	DEV_TYPE_CURTAIN_CONTROLER = 8,//����������
	DEV_TYPE_TEMPERATURE_SENSOR = 9,
	DEV_TYPE_PRESSURE_SENSOR = 10,
	DEV_TYPE_FLOW_RATE_SENSOR = 11,
	DEV_TYPE_CAMERA = 12,
	DEV_TYPE_COMPUTER = 13,
	DEV_TYPE_SPEAKER = 14,
	DEV_TYPE_AIR_CONDITIONER_CONTROLLER = 15,
	DEV_TYPE_AIR_CONDITIONER_REMOTE_CONTROLLER = 16,
	DEV_TYPE_AIR_CONDITIONER_REMOTE_CONTROLLER_WITH_CAMERA = 17,
	DEV_TYPE_INVALID,
}DEV_TYPE_e;


typedef enum
{
    IPCNET_BINARY_MSG_UNKNOW = 0,
    IPCNET_BINARY_MSG_SERIAL_DATA_REQ,       // 1-͸����������
	IPCNET_BINARY_MSG_SERIAL_DATA_RESP,	    
    IPCNET_BINARY_MSG_REMOTE_DEBUG_REQ,         // 3-Զ�̵�������
    IPCNET_BINARY_MSG_REMOTE_DEBUG_RESQ,
    IPCNET_BINARY_MSG_QUNDA_REQ,         // 5-Ⱥ�ﴮ��Э��
    IPCNET_BINARY_MSG_QUNDA_RESP,
}IPCNET_BINARY_TYPE_e;

typedef struct
{
    T_U16  flag;    //��ʶͷ
    T_U16  type;    //��Ϣ����IPCNET_BINARY_TYPE_e
    T_U16  len;    //head len+data len
    T_S16  result; //����ʱ��������
}IPCNET_CMD_BIN_MSG_HEAD_t;	

/*
//------����
typedef struct
{
	char Name[IPCNET_STRING_LEN];
	char SessionID[IPCNET_STRING_LEN];
}IpcnetGenerlReq_st;

typedef struct
{
	char Name[IPCNET_STRING_LEN];
	T_S32 Ret;
	char SessionID[IPCNET_STRING_LEN];
}IpcnetGenerlResp_st;
*/

typedef struct
{
	T_S32 dump;
}IPCNetNULL_st;

typedef struct
{
    T_S32 ret;
}IPCNetRet_st;

typedef struct  //ʱ��ṹ
{
    int8_t Hour;
    int8_t Min;
    int8_t Sec;
}IPCNetTime_st;

typedef struct  //ʱ��ṹ
{
    int16_t Year;
    int8_t Mon;
    int8_t Day;
    int8_t WDay;
}IPCNetDate_st;

//------------------------�豸��Ϣ����------------------------
#define IPCNET_SYSINFO_NAME "SystemInfo"
#define IPCNET_WORKSTATE_NAME "WorkState"

//------SystemInfo------
typedef struct
{
	T_S32 AlarmInChannel;
	T_S32 AlarmOutChannel;
	char BuildTime[IPCNET_STRING_LEN];
	char EncryptVersion[IPCNET_STRING_LEN];
	char HardWareVersion[IPCNET_STRING_LEN];
	char SerialNo[IPCNET_STRING_LEN];
	char SoftWareVersion[IPCNET_STRING_LEN];
	T_S32 TalkInChannel;
	T_S32 TalkOutChannel;
	T_S32 VideoInChannel;
	T_S32 VideoOutChannel;
	T_S32 ExtraChannel;
	T_S32 AudioInChannel;
	char DeviceRunTime[IPCNET_STRING_LEN];
}IpcnetSystemInfo_st;

typedef struct
{
	char Name[IPCNET_STRING_LEN];
	T_S32 Ret;
	char SessionID[IPCNET_STRING_LEN];
	IpcnetSystemInfo_st SystemInfo;
}IpcnetSystemInfoResp_st;


//------WorkState------
typedef struct
{
	T_S32 Channel;
	T_S32 Width;
	T_S32 Height;
}IpcnetVideoChannelState_st;
typedef struct
{
	T_S32 VideoChannelNum;
	IpcnetVideoChannelState_st VideoChannelState[IPCNET_VIDEO_CHANNEL_NUM];
}IpcnetVideoState_st;

typedef struct
{
	T_S32 Channel;
	char Codec[IPCNET_AUDIO_CHANNEL_NUM];
}IpcnetAudioChannelState_st;
typedef struct
{
	T_S32 AudioChannelNum;
	IpcnetAudioChannelState_st AudioChannelState[IPCNET_AUDIO_CHANNEL_NUM];
}IpcnetAudioState_st;

typedef struct
{
	IpcnetVideoState_st VideoState;
	IpcnetAudioState_st AudioState;
}IpcnetWorkState_st;

typedef struct
{
	char Name[IPCNET_STRING_LEN];
	T_S32 Ret;
	char SessionID[IPCNET_STRING_LEN];
	IpcnetWorkState_st WorkState;
}IpcnetWorkStateResp_st;

typedef enum{
	AV_RECO_OP_PLAY_REQ = 0,    //��ʼ��������
	AV_RECO_OP_PLAY_RESP,

	AV_RECO_OP_FAST_FORWARD_REQ,        //���--2
	AV_RECO_OP_FAST_FORWARD_RESP,       

	AV_RECO_OP_FAST_BACKWARD_REQ,        //����--4
	AV_RECO_OP_FAST_BACKWARD_RESP,

	AV_RECO_OP_STOP_REQ,
	AV_RECO_OP_STOP_RESP,

	AV_RECO_OP_SEEK_REQ,
	AV_RECO_OP_SEEK_RESP,
	
	AV_RECO_OP_DOWNLOAD_FILE_REQ, // 10
	AV_RECO_OP_DOWNLOAD_FILE_RESP,
	
	AV_RECO_OP_FILE_ATTRIBUTE_REQ,
	AV_RECO_OP_FILE_ATTRIBUTE_RESP,
	
	AV_RECO_OP_DELETE_FILE_REQ,
	AV_RECO_OP_DELETE_FILE_RESP,

	AV_RECO_OP_SET_RECORDER_CONFIG_REQ,
	AV_RECO_OP_SET_RECORDER_CONFIG_RESP,

	AV_RECO_OP_GET_RECORDER_CONFIG_REQ,
	AV_RECO_OP_GET_RECORDER_CONFIG_RESP,

	AV_RECO_OP_PLAY_PAUSE_REQ,      //20-��ͣ
	AV_RECO_OP_PLAY_PAUSE_RESP

}AV_RECO_OP_et;



//--------------------��Ƶ����---------------------------
#define IPCNET_STREAM_TYPE_VIDEO (0x01)      //������Ƶ
#define IPCNET_STREAM_TYPE_AUDIO (0x01<<1)      //������Ƶ
#define IPCNET_STREAM_TYPE_BMP (0x01<<2) 

typedef struct IPCNET_STREAM_REQ
{
	u_char   channel;        //�����ͨ��0
	u_char	 flag;			 //0����������1��������
	u_char   stream_type;    //֡���ͣ���Ƶ��.��Ƶ������������1
	u_char   cmd_type;       //�������ͣ���ʼ����,��������1
	u_int    reserve;        //�����ֶ�
}IPCNET_STREAM_REQ_t;



//---------------------�û�����----------------------------
#define IPCNET_USER_CFG "User.Cfg"
#define IPCNET_USER_ADD "Add"
#define IPCNET_USER_DEL "Del"
#define IPCNET_USER_CHANGE "Change"
typedef struct
{
    char Op[32];
    char Passwd[64];
}IPCNetUserOptReq_st;


//-----------------------��������-------------------------
//-------------------��������------------------------
#define IPCNET_NETWORKETH_CFG "NetWork.Eth"
#define IPCNET_NETWORKDNS_CFG "NetWork.DNS"
#define IPCNET_NETWORKWIRELESS_CFG "NetWork.Wireless"
#define IPCNET_NETWORK_MOBILE_CFG "NetWork.3G4G"
#define IPCNET_NETWORKWIRELESS_SEARCH "NetWork.WirelessSearch"


//----------��������ETH---------------
/*
/IPCNET_ETH_GET_REQ
/IPCNET_ETH_SET_REQ
*/
typedef struct
{
	T_S32 Valid;
	T_S32 DhcpEnble;
	char Mac[20]; //not effective for setting
	char IP[IPCNET_IP_STRING_LEN];
	char Netmask[IPCNET_IP_STRING_LEN];
	char Getway[IPCNET_IP_STRING_LEN];
	char DNS1[IPCNET_IP_STRING_LEN];
	char DNS2[IPCNET_IP_STRING_LEN];
}IPCNetEthConfig_st;


//----------��������Wireless---------------
#define IPCNET_WIFI_NONE_ENC "WEP-NONE"
#define IPCNET_WIFI_WEP_ENC "WEP"
#define IPCNET_WIFI_WPA_AES_ENC "WPA-AES"
#define IPCNET_WIFI_WPA_TKIP_ENC "WPA-TKIP"
#define IPCNET_WIFI_WPA2_AES_ENC "WPA2-AES"
#define IPCNET_WIFI_WPA2_TKIP_ENC "WPA2-TKIP"
//#define JL_WIFI_WPA_WPA2_ENC "WPA2-PSK"
#define IPCNET_WIFI_UNKNOWN_ENC "UNKNOWN"
//#define IPCNET_WIFI_AUTO_ENC "AUTO"

#define IPCNET_SSID_MODE_SCAN_SETTING "SCAN"
#define IPCNET_SSID_MODE_MANUAL_SETTING "MANUAL"

typedef enum
{
    //�����������
    IPCNET_WIRELESS_UNINITIALIZED,
    IPCNET_WIRELESS_INITIALIZING,
    IPCNET_WIRELESS_INTIALIZ_FAIL, // ��״̬�������豸�����ڻ�������������������ζwireless�豸������
    IPCNET_WIRELESS_INTIALIZED,
    IPCNET_WIRELESS_CONNECTING, // 4-�����ȵ�������
    IPCNET_WIRELESS_IP_SETTING, // 5-�ȵ����ӳɹ�����������IP�У����ʹ�õ��Ǿ�̬IP����ֱ������IPȻ��״̬��ΪWIRELESS_SETTING_OK
    IPCNET_WIRELESS_SETTING_OK,
    IPCNET_WIRELESS_SETTING_FAIL, //�豸���ӻ�����IP�Ȳ���ʧ��
    IPCNET_WIRELESS_PASSWD_ERROR, //�������
}IPCNET_WIRELESS_STATUS_e;

//----------��������:����---------------
/*
/IPCNET_NETWORK_WIFI_GET_REQ
/IPCNET_NETWORK_WIFI_SET_REQ
*/
typedef struct
{
	T_S32 ValidNetDev;	//��ǰʹ�������豸��0-eth0,1-wireless,2-mobile //not effective for setting
	T_S32 WirelessEnable;
	T_S32 WirelessStatus; //IPCNET_WIRELESS_STATUS_e  //not effective for setting
	char SsidSetMode[IPCNET_STRING_LEN];  //not effective for setting
	char EncType[IPCNET_STRING_LEN];
	char SSID[IPCNET_WIFI_AP_MAX_LEN];
	char Password[IPCNET_STRING_LEN];
	T_S32 DhcpEnble;
	char IP[IPCNET_IP_STRING_LEN];
	char Netmask[IPCNET_IP_STRING_LEN];
	char Getway[IPCNET_IP_STRING_LEN];
	T_S32 RSSI ;  //not effective for setting
}IPCNetWirelessConfig_st;


//----------WIFI�ȵ�����---------------
/*
/IPCNET_NETWORK_WIFI_SEARCH_GET_REQ
*/
typedef struct
{
	char SSID[IPCNET_WIFI_AP_MAX_LEN];
	char EncType[IPCNET_STRING_LEN];
	T_S32  RSSI;//�ź�ǿ�ȼ���60%�������60
}IpcnetNetworkWirelessAp_st;

typedef struct
{
	IpcnetNetworkWirelessAp_st Aplist[30];
	T_S32  __pri_AplistCount;//ApCount
}IpcnetNetworkWirelessSearch_st;



//-------------------����������------------------------
#define IPCNET_VIDEO_REQ_CFG "AVEnc.StreamReq"
#define IPCNET_STREAM_REQ_VIDEO "Video"
#define IPCNET_STREAM_REQ_AUDIO "Audio"
#define IPCNET_STREAM_REQ_COMP "COMP"
#define IPCNET_STREAM_REQ_START "Start"
#define IPCNET_STREAM_REQ_STOP "Stop"
typedef struct
{
	T_U8   ViCh;        //�����ͨ��0
	T_U8   EncCh;        //�����ͨ��0
	char   StreamType[16];    //֡���ͣ���Ƶ��.��Ƶ������������1
	char   Opt[16];
}IPCNetStreamReq_t;



//-------------------��Ƶ��������------------------------
/*
/IPCNET_VIDEO_ENC_GET_REQ
/IPCNET_VIDEO_ENC_SET_REQ
*/
#define IPCNET_VIDEO_ENC_CFG "AVEnc.VideoEncode"

#define IPCNET_ENCODE_H264 "ENC_H264"
#define IPCNET_ENCODE_MJPEG "ENC_MJPEG"
#define IPCNET_ENCODE_H265 "ENC_H265"

#define ENCODE_BASELINE "ENC_BASELINE"
#define ENCODE_MAINPROFILE "ENC_MAINPRO"
#define ENCODE_HEIGHTPROFILE "ENC_HEIGHTPRO"

typedef struct
{
	int Witdh;
	int Height;
}IPCNetVideoResolutionOpt_st;

typedef struct
{
	T_S32 EncCh;
	
	char Encode[IPCNET_STRING_LEN];
	//@OpionEncode, @__pri_OpionEncodeCount NOT effective for SETTING
	char OpionEncode[3][IPCNET_STRING_LEN];
	T_S32 __pri_OpionEncodeCount;
	
	int Witdh;
	int Height;
	//@VideoResolutionOpt, @__pri_VideoResolutionOptCount NOT effective for SETTING
	IPCNetVideoResolutionOpt_st VideoResolutionOpt[4];
	int __pri_VideoResolutionOptCount;
	
	T_S32 IFrame;
	T_S32 MaxIFrame;
	T_S32 MinIFrame;
	
	T_S32 FrameRate;
	T_S32 MaxFrameRate;
	T_S32 MinFrameRate;

    T_S32 RateCtrl; // CBR:1���ɱ�����,0:VBR�ɱ�����
    T_S32 Bitrate;  //���� 16K-20M BIT/SΪ��λ

	//char ProfileType[IPCNET_STRING_LEN];
	//char ProfileTypeOption[3][IPCNET_STRING_LEN];
	//T_S32 ProfileTypeOptionCount;
}IPCNetVideoEncode_st;

typedef struct
{
	T_S32 ViCh;
	T_S32 VideoEncodeNum;
	IPCNetVideoEncode_st VideoEncode[2];
	int __pri_VideoEncodeCount;
}IPCNetVideoEncodeCfg_st;


#if 0
typedef struct
{
	T_S32 EncCh;
	char Encode[IPCNET_STRING_LEN];
	
	int Witdh;
	int Height;
	
	T_S32 IFrame;
	T_S32 FrameRate;
    T_S32 RateCtrl; // CBR:1���ɱ�����,0:VBR�ɱ�����
    T_S32 Bitrate;  //���� 16K-20M BIT/SΪ��λ

	char ProfileType[IPCNET_STRING_LEN];
}IPCNetSetVideoEncodeReq;
typedef struct
{
	T_S32 ViCh;
	T_S32 VideoEncodeNum;
	IPCNetSetVideoEncodeReq VideoEncode[2];
	T_U32 __pri_VideoEncodeCount;
}IPCNetSetVideoEncodeCfgReq;
#endif

#define IPCNET_VIDEO_IFRAME_CFG "AVEnc.VideoIFrame"
typedef struct
{
	T_S32 ViCh;
	T_S32 VenCh;
}IPCNetVideoIFrame_st;

#define IPCNET_VIDEO_QUALITY_CFG "AVEnc.VideoQuality"
/*****
Level:
DEV_ADJUST_VIDEO_LEVEL_MID-�����м�֡�ʺ�����
DEV_ADJUST_VIDEO_LEVEL_REDUCE-�𲽵ݼ�����֡�ʺ�����
DEV_ADJUST_VIDEO_LEVEL_PROMOTE-�𲽵�������֡�ʺ�����
DEV_ADJUST_VIDEO_LEVEL_RECOVER-�ָ����ֵ
ֵΪ1-10ʱ��dst=src*(10-opt)/10,ֵԽС��֡�ʺ�����Խ��
******/
typedef struct
{
	T_S32 ViCh;
	T_S32 VenCh;
	T_S32 Level;	//DEV_ADJUST_VIDEO_LEVEL_e	
	T_S32 LevelMax; //not effective for setting
	T_S32 LevelMin; //not effective for setting
}IPCNetVideoQuality_st;


//-------------------����������ظ�------------------------
#define IPCNET_STREAM_RESP_CFG "Stream.StreamInfo"

typedef struct
{
	T_S32 ViCh;
	T_S32 EncCh;
	
	char Encode[IPCNET_STRING_LEN];
    
	T_S32 Width;
	T_S32 Height;
	
	T_S32 IFrame;
	T_S32 FrameRate;
	//char ProfileType[IPCNET_STRING_LEN];
}IPCNetVideoStreamResp_st;
typedef struct
{
    IPCNetVideoStreamResp_st VideoStream;
}IPCNetStreamResp_st;



//-------------------��Ƶ��������------------------------
/*
/IPCNET_AUDIO_ENC_GET_REQ
/IPCNET_AUDIO_ENC_SET_REQ
*/
#define IPCNET_AUDIO_ENC_CFG "AVEnc.AudioEncode"

#define IPCNET_AUDIO_ENCODE_PCM "PCM"
#define IPCNET_AUDIO_ENCODE_G711A "G711A"
#define IPCNET_AUDIO_ENCODE_G711U "G711U"
#define IPCNET_AUDIO_ENCODE_G726 "G726"
#define IPCNET_AUDIO_ENCODE_AMR "AMR"


#define IPCNET_AUDIO_ENCODE_8K "8k"
#define IPCNET_AUDIO_ENCODE_11P025K "11P025K"
#define IPCNET_AUDIO_ENCODE_24K "11P025K"
#define IPCNET_AUDIO_ENCODE_16K   "16k" 
#define IPCNET_AUDIO_ENCODE_22P05K "22P025K"

#define IPCNET_AUDIO_ENCODE_MONO "MONO"
#define IPCNET_AUDIO_ENCODE_STEREO "STEREO"

typedef struct
{
	T_S32 EncCh;
	
	char Encode[IPCNET_STRING_LEN];
	char EncodeOption[4][IPCNET_STRING_LEN];
	T_S32 EncodeOptionCount;
	
	char MicType[IPCNET_STRING_LEN];
	char MicTypeOption[4][IPCNET_STRING_LEN];
	T_S32 MicTypeOptionCount;
	
	char SampleFreq[IPCNET_STRING_LEN];
	char SampleFreqOption[4][IPCNET_STRING_LEN];
	T_S32 SampleFreqOptionCount;

	//audio aenc vol and gain
	T_S32 Vol;
	T_S32 MaxVol;
	T_S32 MinVol;

	T_S32 Gain;
	T_S32 MaxGain;
	T_S32 MinGain;

    //audio dec vol and gain
    T_S32 DecVol;
	T_S32 DecMaxVol;
	T_S32 DecMinVol;

	T_S32 DecGain;
	T_S32 DecMaxGain;
	T_S32 DecMinGain;
}IpcnetAudioEncode_st;
typedef struct
{
	T_S32 AiCh;
	T_S32 AudioEncodeNum;
	IpcnetAudioEncode_st AudioEncde[1];
	T_S32 __pri_AudioEncdeCount;
}IpcnetAudioEncodeCfg_st;

#define IPCNET_TALK_REQ_INFO "Talk.Req"

typedef struct
{
    int TalkEnable;
}IPCNetTalkReq_t;

//------------------------------------PTZ--------------------------------------
typedef struct
{
	T_S32 Channel;
	char Command[IPCNET_STRING_LEN];
	T_S32 Param1;
	T_S32 Param2;
}IpcnetPTZControl_st;
typedef struct
{
	char Name[IPCNET_STRING_LEN];
	char SessionID[IPCNET_STRING_LEN];
	IpcnetPTZControl_st OPPTZControl;
}IpcnetPTZControl_Req_st;


//------------------------------------MD ALRAM--------------------------------------
#define IPCNET_MOVE_ALARM_CFG "Alarm.MoveAlarm"

typedef struct  //ʱ��ṹ
{
    IPCNetTime_st Start;
    IPCNetTime_st End;
}IPCNetTimePeriod_st;
typedef struct  //����ʱ���
{
    T_S32  Flag; //1:��, 0:��
    IPCNetTimePeriod_st TimePeriod[2];
    T_S32 __pri_TimePeriodCount;
}IPCNetDefendPeriodInfo_st;

typedef struct  //�ƶ������������
{
    T_S32  MdEnable; //1:��, 0:��
    T_S32  Sensitive;  //������1--9����9��������
    T_S32  Columns;
    T_S32  Rows;
    T_S32  Area[IPCNET_NET_MAX_AREA];
    T_S32  __pri_AreaCount;
}IPCNetVideoMoveInfo_st;

typedef struct  //�ƶ������������
{
    T_S32  MdEnable;
    T_S32  Sensitive;  //������1--9����9��������
    T_S32  Columns; //not effective for setting
    T_S32  Rows; //not effective for setting
    char  Cell[64]; //����base64�����Columns*Rows��λͼ
}IPCNetMotionCellInfo_st;

typedef struct //�澯����IO��Ϣ
{
    T_S32   Delay;
    T_S32   EmailEnable;
}IpcnetAlarmIoOutPutInfo_st;

typedef struct  //����ץ��
{
    T_S32 PictureNum;
    T_S32 Sec;
    T_S32 Msec;
    T_S32 FtpEnable;
    T_S32 EmailEnable;
    T_S32 CloudEnable;
    T_S32 PushEnable;   //�ϴ�����ͼƬ
}IPCNetAlarmSnapInfo_st;
typedef struct //�澯����IO��Ϣ
{
    int AlarmLevel;     //0-��⵽�͵�ƽ������1-�ߵ�ƽ����
    int Delay;
    int EmailEnable;
}IPCNetAlarmIoOutputInfo_st;
typedef struct //����¼��
{
    T_S32 RecEnable;
    T_S32 RecordTime;
    T_S32 FtpEnable;
    T_S32 EmailEnable;
    T_S32 CloudEnable;
}IPCNetAlarmRecordInfo_st;

typedef struct //������Ԥ��λ����
{
    T_S32 PresetId;		//-1:��ʾû�����ù���Ԥ�õ㣬>=0,�����˶�Ӧ��Ԥ�õ�
    char PresetName[IPCNET_STRING_LEN];
}IPCNetAlarmPresetInfo_st;
typedef struct  
{
    IPCNetAlarmIoOutputInfo_st IoOutputInfo;
    IPCNetAlarmRecordInfo_st RecordInfo;
    IPCNetAlarmSnapInfo_st SnapInfo;
    IPCNetAlarmPresetInfo_st PresetInfo;
}IPCNetAlarmLinkagePolicyInfo_st;

/*
/IPCNET_MOVE_ALARM_GET_REQ
*/
typedef struct //�ƶ����
{
	T_S32 ViCh;
    IPCNetDefendPeriodInfo_st Week[8]; //0:everyday 1-7:since Sunday to saturday
    IPCNetVideoMoveInfo_st MoveInfo; //�����Э������̫���ˣ�������ʹ����
    IPCNetMotionCellInfo_st MoveCell;
    IPCNetAlarmLinkagePolicyInfo_st PolicyInfo;
}IPCNETMoveAlarmCfg_st;

#if 0
typedef struct //�ƶ����
{
	T_S32 ViCh;
    IPCNetDefendPeriodInfo_st Week[8]; //0:everyday 1-7:since Sunday to saturday
    IPCNetMotionCellInfo_st MoveCell;
    IPCNetAlarmLinkagePolicyInfo_st PolicyInfo;
}IPCNetSetMoveAlarmCfgReqV2_st;
#endif

/*
/IPCNET_MOVE_ALARM_SET_REQ
*/
typedef struct //�ƶ����
{
	T_S32 ViCh;
    IPCNetDefendPeriodInfo_st Week[8]; //0:everyday 1-7:since Sunday to saturday
    IPCNetMotionCellInfo_st MoveCell;
    IPCNetAlarmLinkagePolicyInfo_st PolicyInfo;
}IPCNETMoveCellAlarmCfg_st;


#define IPCNET_MOVE_TYPE_CELL_INFO "MoveCell"
#define IPCNET_MOVE_TYPE_MOVE_INFO "MoveInfo"

typedef struct
{
    char MoveInfoType[IPCNET_STRING_LEN];
}IPCNETMoveAlarmCfgReq_st;

//--------------------PTZ INFO------------------------
typedef struct //��̨����
{
    
}IpcnetPtzCfg_st;



#define IPCNET_LOGIN_RET_INFO "Login.info"

typedef struct IPCNetLoginRet
{
    uint32_t Tick;
}IPCNetLoginRet_t;


//------------------------------------Upgrade info--------------------------------------
#define IPC_UPGRADE_INFO "Upgrade.info"
/*
typedef enum
{
    PARTITION_ROOTFS = 1,
    PARTITION_KERNEL,
    IMAGE_USR,
    IMAGE_WEB,
    IMAGE_VENDOR,
    IMAGE_CUSTOM,
    IMAGE_SYSTEM,
}UPGRADE_TYPE_t;

typedef struct 
{
    T_S32 type;//UPGRADE_TYPE_t     �����ļ������ͺ�upgrade_name��Ӧ 
    char upgrade_name[128];//�����ļ������ƻ��߽���ͨ�����Ź�����Ϣ
    char upgrade_url[256];
    char version[64];
    char check[64];
}IpcnetUpgradeInfo_st;

typedef struct 
{
    IpcnetUpgradeInfo_st UpgradeInfo[7];
    T_S32 UpgradeInfoCount;
}IPCNetUpgradeInfo_st;
*/


typedef struct 
{
    char version[32];
    char usrcheck[32];
    char systemcheck[32];
    char webcheck[32];
}IpcnetUpgradeSystemInfo_st;
typedef struct 
{
    char version[32];
    char customcheck[32];
}IpcnetUpgradeCustomInfo_st;
typedef struct 
{
    char version[32];
    char vendorcheck[32];
}IpcnetUpgradeVendorInfo_st;

typedef struct 
{
    T_S32 SerType;   //0-Զ�̷�������1-���ط�����
    IpcnetUpgradeSystemInfo_st UpgradeSystemInfo;
    IpcnetUpgradeCustomInfo_st UpgradeCustomInfo;
    IpcnetUpgradeVendorInfo_st UpgradeVendorInfo;
}IPCNetUpgradeInfo_st;

typedef struct
{
    int ret;    //�������̣���δ�õ���Ŀǰ����IPC_NET_RET_UPDATING
    int p;      //percent
}IPCNetUgradeResp_st;

#define IPCNET_SET_AUTO_UPGRADE_INFO "AutoUpgradeSet.info"

typedef struct 
{
    BOOL AutoUpgrade;
}IPCNetAutoUpgrade_st;


typedef struct 
{
    char  LocalUrl[128];     //������
    char  UpgradeUrl[128];
    char  SystemType[128];
    char  CustomType[128];
    char  VendorType[128];
    IPCNetAutoUpgrade_st AutoUpgrade;
}IPCNetUpgradeCfg_st;


#define IPCNET_3G_INFORM "Net.Mobile"
typedef struct 
{
	T_S32 enable;	//ʹ��3gģ�飬0-����Ч��1-��Ч
	T_S32 type;		// 3Gģ�����ͣ�0-����evdo,1-��ͨwcdma
	char ip[32];	// 3Gģ���ȡ��IP��ַ
	char vpn[32];
	char name[IPCNET_STRING_LEN];
}Ipcnet3GInfo_st;


//------------recorder config setting-------------
#define IPCNET_RECORDER_CONFIG "Rec.Conf"
typedef struct//ʱ��ṹ
{
    T_U8 h;
    T_U8 m;
    T_U8 s;
}IPCNetTimeSimple_t;
typedef struct
{
    T_S32 En;
    char St1[IPCNET_STRING_LEN];
    char Ed1[IPCNET_STRING_LEN];
    char St2[IPCNET_STRING_LEN];
    char Ed2[IPCNET_STRING_LEN];
}IPCNetRecordTiming_st;
typedef struct
{
	char Path[MAX_PATH];//media file path the recorder to store.
    BOOL isValid;
	T_S32 Type;//sdcard,hard disk,net disk or ?
    unsigned Total;//mega bytes.
	unsigned Free;//mega bytes.
}IPCNetDiskInfo_st;

typedef struct
{
    T_S32 ViCh;
    T_S32 VeCh;     //0-������,1-������
    BOOL AutoDel;
    T_S32 RecMins;
    T_S32 RecMinsOption[8];
    IPCNetRecordTiming_st RecTime[8];
	IPCNetDiskInfo_st DiskInfo;
	
	T_S32 PackageType;//0:avi,1:mjpg(vi.encode_type == 1 is needed.),2:mkv
	short Mode;//0:length means time, 1:length means file size.	
	//T_S32 Duration;//if mode is 0,then 1000 means 00:10:00.	
	unsigned long long  ReserveSize;
//	#define RECYCLE_MODE_OVER_WRITE 0
//	#define RECYCLE_MODE_KEEP 1	
	//short RecycleMode;
}IPCNetRecordCfg_st;

typedef struct
{
    T_S32 ViCh;     //sensor index.
    char Path[MAX_PATH];
    T_S32 RecType;  //¼���ļ�����0-��ʱ¼��1-����¼��
}IPCNetRecordGetCfg_st;



//--------------------��ѯ¼����Ϣ--------------------------
#define IPCNET_AVRECORD_REQ_INFO "lir"
typedef struct
{
	int si; //vich
	int m; //mode �������pʹ�ã�0:ֻ�鿴�ļ���1:�鿴�ļ����ļ���
	char p[128]; //path
	int ds; //yyyymmdd Ϊ����¼���ǰ�ʱ����Ҷ�����·�����ң���pΪ��ʱʹ�á����Դ���ʼʱ��:������
	int de; //yyyymmdd Ϊ����¼���ǰ�ʱ����Ҷ�����·�����ң���pΪ��ʱʹ�á����Դ������ʱ��:������
	int et;  //event type: 0-time; 1-alarm
	int st; //start:hhmmss ��ʼʱ��:ʱ����
	int e;  //end:hhmmss ����ʱ��:ʱ����
}IPCNetAvRecordInfoReq_st;

#define IPCNET_AVRECORD_RESP_INFO "li"
typedef struct
{
	int n; //num
	int t; //total space
	int u; //free space
}IPCNetAvRecordInfoResp_st;


//--------------------��ѯ¼���ļ��б�--------------------------
#define IPCNET_AV_REC_NUM 20 //sdk

#define IPCNET_AVRECORDLIST_REQ_INFO "lp"
typedef struct
{
    char p[80];
    int s; //start_index
    int c; //count //sdk�����˲��ܳ���IPCNET_AV_REC_NUM��
    
    //���� 20170829
    int si; //vich
    int re; //recursive_num
    int m; //mode, //0:�������ļ��� 1�������ļ���
    int et; //event type //0: ��ʱ¼�� 1��Ϊ����¼��
}IPCNetAvRecListPageReq_t;

#define IPCNET_AVRECORDLIST_RESP_INFO "dir"
typedef struct
{
    int t; //DT_DIR:�ļ��У�DT_REG: ��ͨ�ļ�
    char n[24]; //filename
    int sl; // size ��8λ
    int sh; // size ��8λ
}IPCNetAvRecFileInfo_t;
typedef struct
{
    int t; //DT_DIR:�ļ��У�DT_REG: ��ͨ�ļ�
    char n[80]; //dir name
    IPCNetAvRecFileInfo_t l[IPCNET_AV_REC_NUM];
    int __pri_lCount;
}IPCNetAvRecListPageResp_t;



//------------recorder config setting-------------
#define IPCNET_GPIO_INFO "GPIO"

typedef struct
{
    T_S32 Index;
    T_S32 Mode;     //0-input,1-output,2-adc,3-dac,4-pwd
    T_S32 Val;
	T_S32 Tr;
	T_S32 Gate;
}IPCNetGpioInfo_st;
typedef struct
{
    T_S32 Index;
    T_S32 Mode;
    T_S32 Val;
//	T_S32 Tr;
//	T_S32 Gate;
}IPCNetGpioArrayInfo_st;

typedef struct
{
    T_S32 Index;
}IPCNetGetGpioInfo_st;
typedef struct
{
    T_S32 Index;
}IPCNetGetGpioArrayInfo_st;


//------------alarm event-------------
#define IPCNET_MD_INFORM "MotionDetect.AlarmInform"
typedef struct
{
    T_S32 ViCh;//sensor index.
    IPCNetDate_st Day;
    IPCNetTime_st Time;
}IPCNetMdAlarmInfo_st;

#define IPCNET_IO_INFORM "MotionDetect.AlarmInform"
typedef struct
{
    T_S32 Id;
    T_S32 IOIndex;//sensor index.
    IPCNetDate_st Day;
    IPCNetTime_st Time;
}IPCNetIOAlarmInfo_st;

//------------Device Information-------------
#define IPCNET_DEV_INFO "Dev.Info"
#define DEV_INFO_STR_LEN 64
typedef struct
{
    char DevName[128];
	char Model[DEV_INFO_STR_LEN];
    char CPU[DEV_INFO_STR_LEN];
	char DSP[DEV_INFO_STR_LEN];
	char Sensor[DEV_INFO_STR_LEN];
	char Wifi[DEV_INFO_STR_LEN];
	char Eth[DEV_INFO_STR_LEN];
	char Mobile[DEV_INFO_STR_LEN];
	IPCNetDate_st Date;
    IPCNetTime_st Time;
	T_S32 TimeZone;
	//T_S32 Subdev;
	//T_S32 GPIO;
	char Onvif[DEV_INFO_STR_LEN];
	char GB28181[DEV_INFO_STR_LEN];
	char Mail[DEV_INFO_STR_LEN];
	char FTP[DEV_INFO_STR_LEN];
	char Storage[DEV_INFO_STR_LEN];
	char Alarm[DEV_INFO_STR_LEN];
	char OSver[DEV_INFO_STR_LEN];
	char Kern[DEV_INFO_STR_LEN];
	char Firm[DEV_INFO_STR_LEN];
}IPCNetDevInfo_st;

typedef struct
{
    char DevName[128];
}IPCNetDevName_st;

//------------set time-------------
typedef enum
{
    IPCNET_GMT_NEGA_12 = 0,
    IPCNET_GMT_NEGA_11_30,
    IPCNET_GMT_NEGA_11,
    IPCNET_GMT_NEGA_10_30,
    IPCNET_GMT_NEGA_10,
    IPCNET_GMT_NEGA_9_30,	//5
    
    IPCNET_GMT_NEGA_9,
    IPCNET_GMT_NEGA_8_30,
    IPCNET_GMT_NEGA_8,
    IPCNET_GMT_NEGA_7_30,
    IPCNET_GMT_NEGA_7,		//10
    
    IPCNET_GMT_NEGA_6_30,
    IPCNET_GMT_NEGA_6,
    IPCNET_GMT_NEGA_5_30,
    IPCNET_GMT_NEGA_5,
    IPCNET_GMT_NEGA_4_30,	//15
    
    IPCNET_GMT_NEGA_4,
    IPCNET_GMT_NEGA_3_30,
    IPCNET_GMT_NEGA_3,
    IPCNET_GMT_NEGA_2_30,
    IPCNET_GMT_NEGA_2,		//20
    
    IPCNET_GMT_NEGA_1_30,
    IPCNET_GMT_NEGA_1,
    IPCNET_GMT_NEGA_0_30,
    IPCNET_GMT_0,
    IPCNET_GMT_PLUS_0_30, //25
    IPCNET_GMT_PLUS_1,
    IPCNET_GMT_PLUS_1_30,
    IPCNET_GMT_PLUS_2,
    IPCNET_GMT_PLUS_2_30,
    IPCNET_GMT_PLUS_3,		//30
    
    IPCNET_GMT_PLUS_3_30,
    IPCNET_GMT_PLUS_4,
    IPCNET_GMT_PLUS_4_30,
    IPCNET_GMT_PLUS_5,
    IPCNET_GMT_PLUS_5_30,	//35
    IPCNET_GMT_PLUS_6,
    IPCNET_GMT_PLUS_6_30,
    IPCNET_GMT_PLUS_7,
    IPCNET_GMT_PLUS_7_30,
    IPCNET_GMT_PLUS_8,//����ʱ��,Ĭ��ֵ	//40
    IPCNET_GMT_PLUS_8_30,
    IPCNET_GMT_PLUS_9,
    IPCNET_GMT_PLUS_9_30,
    IPCNET_GMT_PLUS_10,
    IPCNET_GMT_PLUS_10_30,
    IPCNET_GMT_PLUS_11,
    IPCNET_GMT_PLUS_11_30,
    IPCNET_GMT_PLUS_12,		//48
    IPCNET_GMT_PLUS_12_30,
    IPCNET_GMT_PLUS_13,
}IPCNET_GMT_e;

/*
/IPCNET_GET_TIME_REQ = 1402,
/IPCNET_SET_TIME_REQ = 1404,
*/
#define IPCNET_GET_TIME_REQ_CONF "GetTime.Conf"
typedef struct
{
    int ReqTimeType;    //0-gmt time,1-local time
}IPCNetGetTimeReq_st;

#define IPCNET_TIME_CONF "Time.Conf"
typedef struct
{
    IPCNetDate_st Date;
    IPCNetTime_st Time;
	T_S32 NtpEnable;
	char NtpServ[128];
    T_S32 TimeZone;     //IPCNET_GMT_e //��Э��ʹ�ø����Э��ʹ��DstDistIdʱ���뽫������-1

    //new attr added
    char DstDistId[64];    //����ʱ����ID�����ַ���ƥ��
    char ZoneDesc[32];	//��Э��ʹ���ַ�������ʾʱ������Ϊ��ȡʱչʾ������ʱֻʹ��DstDistId��ȷ��
    T_S32 IsDst;        //��ǰ�õ����Ƿ��������ʱ
    T_S32 AdjustFlg;    //�Ƿ��Ѿ�У��ʱ�����ڲ�����̨�豸ʱ���������⡣
    T_S32 DstEnable;    //�Զ���������ʱ����
    int TimeType;       //0-��������ʱ�䣬1-����ʱ��
}IPCNetTimeCfg_st;


//------------ptz set-------------

#define IPCNET_PTZ_INFO "PTZInfo"
#define IPCNET_PTZ_CTRL_MSG_INFO "PtzCtrlMsg.info"
#define IPCNET_PTZ_PREBIT_MSG_INFO "PtzPreBit.info"
#define IPCNET_PTZ_PREBIT_OPERATE_MSG "PtzOperateBit.info"

//��̨��ϢҪ�Ȼ�ȡ������
typedef struct {
    T_S32 PtzEnable;//ptz �Ƿ�����
    T_S32 BitEnable;//Ԥ��λ�Ƿ�����
    T_S32 StartCenterEnable;//�������Զ�����
    T_S32 Speed;
    T_S32 RunTimes;
    T_S32 LevelMaxTimes;
    T_S32 LevelMidTimes;
    T_S32 VertMaxTimes;
    T_S32 VertMidTimes;             
} IPCPtzBaseMsg_st;

typedef struct {
    T_S32 UseNum;     
} IPCPtzBitMsg_st;

typedef enum
{
    IPCNET_PTZ_UP = 0,
    IPCNET_PTZ_UP_STOP,
    IPCNET_PTZ_DOWN,    // 2
    IPCNET_PTZ_DOWN_STOP,
    IPCNET_PTZ_LEFT,    // 4
    IPCNET_PTZ_LEFT_STOP,
    IPCNET_PTZ_RIGHT,   // 6
    IPCNET_PTZ_RIGHT_STOP,
    IPCNET_PTZ_ZOOM_IN,     //8 �Ŵ�
    IPCNET_PTZ_ZOOM_OUT,    //9--��С
    IPCNET_PTZ_ZOOM_STOP,       // 10
    IPCNET_PTZ_FOCUS_FAR,       // 11--Զ��
    IPCNET_PTZ_FOCUS_NEAR,      // 12 --����
    IPCNET_PTZ_FOCUS_STOP,		// 13
    IPCNET_PTZ_SELF_CHECK,      //14--��̨�Լ�
    IPCNET_PTZ_UP_DOWN,			//15--����Ѳ��
    IPCNET_PTZ_LEFT_RIGHT,		//16--����Ѳ��
    IPCNET_AF_ZOOM_POSITION,	//17--AF�䱶��λ
}IPCNETPtzCmd_e;

typedef struct {
    IPCNETPtzCmd_e MotoCmd;            
    T_S32 OneStep;
    IPCPtzBaseMsg_st PtzBaseMsg;
    IPCPtzBitMsg_st  PtzBitMsg;
} IPCPtzCmd_st;

/***ptz��������****/
typedef struct
{
    IPCNETPtzCmd_e CtrlCmd;    //��������
    int Speed;      //�ٶ�
    int RunTimes;	//Ѳ��������Ѳ���������ʹ��
    int Position;	//�����λʹ��
}IPCPtzCtrlMsg_st;

//��ȡ���״̬��Ϣ�ṹ��
/*****
IPCNET_PTZ_GET_REQ = 1062,
IPCNET_PTZ_GET_RESP,
*****/
#define IPCNET_PTZ_STATUS_INFO "PtzStatus.info"
typedef struct
{
	int Min;		//��Сֵ0
	int Max;		//���ֵ100
	int CurSteps;	//��ǰ����
}IPCNETPtzStatus_st;

/***��ȡԤ�õ���Ϣ***/
/*
**IPCNET_SET_PREPOINT_REQ = 1478,
** @struct IPCNETPointInfo_st
*/
typedef struct
{
    int BitID;  //ֵΪ-1ʱ���Զ�����ID��>=0 ָ��ID
    char Desc[32];
}IPCNETPointInfo_st;

/***����Ԥ�õ㣬���Ԥ�õ�***/
/*
**IPCNET_OPERATE_PREPOINT_REQ = 1480,
** @struct IPCNETPointOpr_st
*/
typedef struct
{
    unsigned int Type; //0-����Ԥ�õ㣬1-�������Ԥ�õ㣬2-�������Ԥ�õ�
    unsigned int BitID; //Ԥ�õ����
}IPCNETPointOpr_st;


/*
IPCNET_GET_PREPOINT_REQ = 1476,
** @struct IPCNETPrePointList_st
*/
#define IPCNET_PTZ_PREPOINT_LIST_INFO "PrePointList.info"
typedef struct
{
    unsigned int PointsCount;       //ʵ�ʵ�Ԥ�õ����
    IPCNETPointInfo_st Points[32];  //���32��Ԥ�õ�
    int __pri_PointsCount;          //�ڲ�תjsonʹ�ã����ù�
}IPCNETPrePointList_st;



/**�ع�����**/
#define IPCNET_EXP_TYPE_INFO "IspExpType.info"
typedef struct
{
	T_S32 type;
}IPCNetExpType_st;


typedef enum
{
	IPCNET_NOISE_RATE_PRIO,	//�������
	IPCNET_FRAME_RATE_PRIO	//֡������
}IPCNetExpTypeEnum_t;

/***�Զ��ع�***/
#define IPCNET_AUTO_EXP_INFO "IspExpAuto.info"
typedef struct
{
	T_U32 compansation;	//����
	T_U32 AgMin;
	T_U32 AgMax;
	T_U32 DgMin;
	T_U32 DgMax;
	T_U32 ExpoType;	//֡�����ȡ��������	
}IPCNetAutoExp_st;

//--�ֶ��ع�
#define IPCNET_MANUAL_EXP_INFO "IspExpMan.info"
typedef struct 
{
	T_U32 MeLineEnable;//�ֶ����ÿ���
	T_U32 MeLineEnum;	//shutter_list����

	T_U32 MeAgEnable;	//ģ������ʹ�ܿ���
	T_U32 MeAgVal;		//ģ����������//0x0~0xFF
	
	T_U32 MeDgEnable;	//������������ʹ�ܿ���
	T_U32 MeDgVal;		//������������//0x0~0xFF
	T_U32 ShutterList[32];
	T_U32 ShutterListCount;
}IPCNetManualExpInfo_st;


typedef struct
{
    char Exptype[IPCNET_STRING_LEN];
    union
    {
        
    }ExposureInfo_u;
}IPCNetExposureInfo_st;


//----ͼ��ת
#define IPCNET_VIDEO_OVERTURN_INFO "IspOverTurn.info"

typedef struct
{
    T_S32 ViCh;
	BOOL Mirror;    //true-����false-����
	BOOL Flip;      //true-��ת��false-����
}IPCNetPicOverTurn_st;

/***��ת��***/
#define IPCNET_PIC_COLOR_INFO "IspPicColor.info"
//----��ת�ڷ�ʽ

/*****ֵ��DEV_PICCOLOR_eö��һ��*****/
typedef enum
{
	IPCNET_PICCOLOR_SETBLACK,	//�ֶ�ת�ڰ�
	IPCNET_PICCOLOR_SETCOLOR,		//�ֶ�ת��ɫ
	IPCNET_PICCOLOR_IRIS_AUTO,		//�Զ��ڰ�-��ɫת��
	IPCNET_PICCOLOR_EXV0L,	//ʹ����ӵ�ѹ����ڰ�-��ɫת��
	IPCNET_PICCOLOR_IO,         //�ⲿ�����л���û�õ�������
	IPCNET_PICCOLOR_TIMER_OPT,	//��ʱ����
	IPCNET_PICCOLOR_INFRARED_OPEN, //�ֶ��򿪺����
	IPCNET_PICCOLOR_INFRARED_CLOSE, //�ֶ��رպ����
	IPCNET_PICCOLOR_END
}IPCNetPicColorEnum_st;

typedef enum
{
	IPCNET_PICCOLOR_LEVEL_0,
	IPCNET_PICCOLOR_LEVEL_1,
	IPCNET_PICCOLOR_LEVEL_2,
}IPCNetPicColorLevelEnum_st;

typedef struct
{
	T_S32 Type;	//��ת������

	//�Զ��ڰ�-��ɫת����Ч
	T_S32 SwitchLevel;  //�л������ȣ�0-�ͣ�1-�У�2-��
	T_S32 SwitchTime;	//�л�ʱ�䣬��λs

	//���²������ڶ�ʱ��ת����Ч
	T_S32 TimerOpera;	//�趨ʱ���ڵĲ�����0-��ɫģʽ��1-�ڰ�ģʽ
	IPCNetTime_st Start;
	IPCNetTime_st End;
}IPCNetPicColorInfo_st;


//----��������
#define IPCNET_EVN_INFO "IspExpEnv.info"

typedef enum
{
	IPCNET_ENV_MODE_INDOOR = 0,
	IPCNET_ENV_MODE_OUTDOOR
}IPCNetEnvModeEnum_st;

typedef enum
{
	IPCNET_ENV_LIGHT_MODE_MANUAL = 0,
	IPCNET_ENV_LIGHT_MODE_BRIGHT,
	IPCNET_ENV_LIGHT_MODE_NORMAL,
	IPCNET_ENV_LIGHT_MODE_BLACK
}IPCNetEnvLightModeEnum_st;

typedef struct
{
	T_S32 Mode; // 1-outdoorģʽ 0--indoorģʽ
	T_S32 LightMode; //����
	T_S32 Level;	//�ȼ�[1~15]���ֶ���Ч
}IPCNetEnvInfo_st;

//----2D/3D����
#define IPCNET_DENOISE_INFO "IspDenoise.info"

typedef struct
{
	BOOL En2DEntiNoise;
	BOOL En3DEntiNoise;
	T_S32 Val3D;
}IPCNetDeNoiseInfo_st;


/**��ƽ��**/
#define IPCNET_WHBALANCE_INFO "IspWhBalance.info"
typedef struct
{
	T_U32 Type;	//0-�Զ�, 1-�ֶ������Զ����²�����Ч��
	T_U32 AutoRG;
	T_U32 AutoBG; //0-255
	T_U32 ManualRG;
	T_U32 ManualBG;//0-65535
	T_U32 Reserve;
}IPCNetWhBalance_st;

/**��֡**/
#define IPCNET_SLOW_FRAME_INFO "IspSlowFrm.info"
typedef enum
{
	IPCNetSlowFrameRate_1 = 1,	// 1
	IPCNetSlowFrameRate_1_2,	// 1/2
	IPCNetSlowFrameRate_1_4,	// 1/4
	IPCNetSlowFrameRate_1_8,	// 1/8
	IPCNetSlowFrameRate_1_16	// 1/16
}IPCNetFrameRateEnum_st;

typedef struct
{
	T_S32 FrameRate;
}IPCNetSlowFrameInfo_st;

//--����˸����
#define IPCNET_ANTI_FLICKER_INFO "IspAntiFlk.info"
typedef struct
{
	BOOL Enable; 	  /**�Ƿ�������˸ģʽ**/ 
	T_U32 Frequency; /****0-60hz,1-50hz*****/
    T_U32 Mode;     /***0-�ƹ⻷����1-�޵ƹ�����Ȼ���****/
}IPCNetAntiFlickerInfo_st;

//---��̬����
#define IPCNET_WDR_INFO "IspWdr.info"
typedef struct
{
	BOOL WdrEn;
	BOOL WdrManEn;//�ֶ���̬ʹ��
	T_U32 WdrVal;   //0-255
}IPCNetWdrInfo_st;

//�Զ���-��ת���ֶ�ģʽ
#define IPCNET_PIC_COLOR4_MANUAL_INFO "IspPicColorMan.info"
typedef struct
{
	T_U32 IsPicolor4ManuLuam; //trueʱΪ�Զ���-��ת��ģʽ���ֶ��������ֵ
	T_U32 OnLux;
	T_U32 OffLux;
	T_U32 CheckTime;
	T_U64 Reserve;
}IPCNetPicColor4Manual_st;

/**ץ������**/
#define IPCNET_SNAP_SHOOT_REQ_INFO "SnapShoot.info"
#define IPCNET_SNAP_TYPE_ORG "Org" //��ͨץ��
#define IPCNET_SNAP_TYPE_FD "FD" //�������ץ��
#define IPCNET_SNAP_TYPE_MULTI "MULTI" //����ץ��

typedef struct
{
    T_U32 ViCh;     //ץ��ͨ��
    BOOL SnapEnable;
    BOOL Permanent;
    char SnapType[IPCNET_STRING_LEN];
}IPCNetSnapShoot_st;

/**ftp cfg**/

//ftp��Ϣ
/*
/IPCNET_GET_FTP_CFG_REQ = 1484,
/IPCNET_SET_FTP_CFG_REQ = 1482,
** @struct IPCNetFtpCfg_st
*/
#define IPCNET_FTP_CFG_INFO "FtpCfg.info"
typedef struct
{
    char FtpAddr[IPCNET_STRING_LEN];
    T_U32 FtpPort;
    char FtpUser[IPCNET_STRING_LEN];
    char FtpPasswd[IPCNET_STRING_LEN];
    char FtpPath[IPCNET_STRING_LEN];
}IPCNetFtpCfg_st;

/***email cfg*****/
#define IPCNET_EMAIL_CFG_INFO "EmailCfg.info"
typedef enum
{
	EMAIL_ENC_NONE = 0,
	EMAIL_ENC_SSL,
	EMAIL_ENC_TSL,
}IPCNetEmailEncrytTypeEnum_st;
typedef struct  
{
    T_U32 SmtpPort;
    char SmtpServer[IPCNET_STRING_LEN];
//	int EnVerify;//enable verify the smtp user name and password.
	int EncType;//encrypt type
    char SmtpUser[IPCNET_STRING_LEN];
    char SmtpPasswd[IPCNET_STRING_LEN];
    char SmtpSender[IPCNET_STRING_LEN];
    char MailTitle[IPCNET_STRING_LEN];
    char MailContext[IPCNET_STRING_LEN*2];
	char SmtpReceiver1[IPCNET_STRING_LEN];
    char SmtpReceiver2[IPCNET_STRING_LEN];
    char SmtpReceiver3[IPCNET_STRING_LEN];
    char SmtpReceiver4[IPCNET_STRING_LEN];
}IPCNetEmailCfg_st;

/****ddns��������****/
#define IPCNET_DDNS_CFG_INFO "DdnsCfg.info"

typedef struct 
{
    T_U16 DdnsEnable;
    T_U16 DdnsPort;
    char DdnsUser[IPCNET_STRING_LEN];
    char DdnsPasswd[IPCNET_STRING_LEN];
    char DdnsAddr[IPCNET_STRING_LEN];
}IPCNetDdnsCfg_st;

/****���ȣ����Ͷȣ��Աȶ�****/
#define IPCNET_CAM_PIC_CFG_INFO "CamCfg.info"

typedef struct
{
    BOOL SetDefault;    //�Ƿ�����Ĭ��ֵ
    T_U8 ViCh;//max=3
    T_U8 Brightness;//[0-255] def:128
    T_U8 Chroma;//[0-255] def:128
    T_U8 Contrast;//[0-255] def:128
    
    T_U8 Saturtion;//[0-255] def:128
    T_U8 Acutance;//[0-255] def:128
}IPCNetCamColorCfg_st;

/****IRCUT CFG****/
#define IPCNET_IRCUT_CFG_INFO "IRCutCfg.info"
typedef enum
{
	IPCNET_IRCUT_OFF = 0,
    IPCNET_IRCUT_ON,
    IPCNET_IRCUT_OPPOSE,    //���÷���
}IPCNetIRcutOptEnum_st;

typedef struct
{
    T_U32 IRCutVal;     //����IRCUTʱ����Ϊ���������ȡIRCUT״̬ʱ����Ϊ״ֵ̬
    BOOL SetOppose;     //IRCUT�Ƿ����÷���FALSE-����,TRUE-����
}IPCNetIRCutCfg_st;

/***����������Ϣ�ϱ����ͻ���***/
#define IPCNET_ALARM_REPORT_INFO "AlarmReport.info"
typedef enum
{   
	IPCNET_ALARM_IO_INPUT = 0,    //IO���뱨��
    IPCNET_ALARM_LOST_VIDEO,      //��Ƶ��ʧ����
    IPCNET_ALARM_MOVE_DETECT,  //�ƶ���ⱨ�� 
    IPCNET_ALARM_HW_IO_RECORD,  //Ӳ��¼�񿪹�
    IPCNET_ALARM_HW_IO_SNAP,//Ӳ��ץ�Ŀ���
    IPCNET_KEY_EVENT,           //�����¼�
    IPCNET_SYSTEM_REBOOT,		//����
    IPCNET_MMC_FORMAT,			//��ʽ��
    IPCNET_ALARM_REC_ON_OFF,	//����¼�񿪹�
    IPCNET_ALARM_CHANGE_IP,		//IP�仯
    IPCNET_ALARM_CHANGE_UUID,	//UUID�仯
}IPCNetAlarmTypeEnum_st;

typedef struct
{
    int AlarmType;      //�� IPCNetAlarmTypeEnum_st ��Ӧ
	int Idx;            //IPCNET_ALARM_IO_INPUT����ʱ��idxΪ�������������
	int Val;            //IPCNET_ALARM_IO_INPUT����ʱ��ValΪ����������IO�ڵĵ�ƽ
	int AlarmSta;    //����״̬0-off,1-on
    IPCNetDate_st AlarmDate;
    IPCNetTime_st AlarmTime;
    time_t tm;		//����������ͼƬ�ϵ�tm��Ӧ����
}IPCNetAlarmMsgReport_st;

typedef struct
{
    int AlarmType;
    int ViCh;
    IPCNetDate_st AlarmDate;
    IPCNetTime_st AlarmTime;
    char WirelessIP[IPCNET_STRING_LEN];
    char DeviceID[IPCNET_STRING_LEN];
}IPCNetWirelessConnStaReport_st;

/***osd cfg***/
#define IPCNET_GET_OSD_CFG_INFO "OsdGetCfg.info"
#define IPCNET_OSD_CFG_INFO "OsdCfg.info"
typedef struct  //ͨ��ʱ����ʾ������Ϣ
{
    BOOL Enable;       //�Ƿ�����
    T_U16 DisplayMode;  //��ʾ�ĸ�ʽ����2016/08/ ����2016-08��
    T_U16 Xcord;        //x����0-704
    T_U16 Ycord;        //y����0-576
}IPCNetOsdConf_st;

typedef struct
{
    char NameText[IPCNET_STRING_LEN];
    IPCNetOsdConf_st NameConf;
}IPCNetOsdName_st;

typedef struct
{
    BOOL SetDefault;
    T_U32 Vich;
    IPCNetOsdName_st OsdNameInfo;
    IPCNetOsdConf_st OsdDateInfo;
    IPCNetOsdConf_st OsdRateInfo;
    IPCNetOsdConf_st OsdSignalInfo;
}IPCNetOsdCfg_st;

typedef struct
{
    int Vich;
}IPCNetGetOsdCfg_st;

#define IPCNET_UPGRADE_CFG_INFO "UpgradeCfg.info"

#define IPCNET_FUNC_CFG_INFO "FuncCfg.info"
typedef struct
{
    T_S32 enable;
    T_S32 Idx;          //0-set gpio,1-relay ctrl,2-alarm out
    T_U32 Mask;         //ʹ��IO�ڵ�����
    T_U32 Val;          //��ӦIO�ڵ�ֵ
}IPCNetFuncCfg_st;

#define IPCNET_STA_LED_CFG "StaLedCfg.info"
typedef struct
{
    T_S32 Enable;
}IPCNetStatusLedCfg_st;

//�������ȼ��
#define IPCNET_AREA_DET_LUM_CFG "LumAreaCfg.info"

typedef struct
{
    T_S32 X;
    T_S32 Y;
    T_U32 Width;
    T_U32 Height;
}IPCNetRectCfg_st;

typedef struct
{
    BOOL Enable;
    T_U32 VideoWidth;    //�ֱ��ʳ�
    T_U32 VideoHeight;     //�ֱ��ʿ�
    IPCNetRectCfg_st RectInfo;
}IPCNetLumAreaCfg_st;

//disk test info
#define IPCNET_DISK_TEST_CFG "DiskTestCfg.info"
typedef struct
{
    T_U8 Type;  //0-sd,1-sata
    T_U8 DiskNo;    //���̺�
    T_U8 TestOp;    //0-��д���ԣ�1-��ʽ������
    //�������0-���Գɹ���1-���̲����ڣ�2-��дʧ��
    T_S32 ErrorNo;  
}IPCNetDiskTest_st;

//p2p server cfg
#define IPCNET_P2P_SER_CFG "P2pSerCfg.info"
typedef struct 
{
    char P2pSerCode[256];
    char Md5Chk[64];
}IPCNetP2pSerCfg_st;

//ʹ������ѡ��
#define IPCNET_NET_DEV_MAN_CFG "NetManCfg.info"
typedef struct
{
    T_U32 NetMask;  //bit[0]-eth,bit[1]-wifi,bit[2]-mobile
    T_S32 DefDev;   //0-eth,1-wifi,2-mobile
}IPCNetNetworkDevCfg_st;

//Ⱥ�ﶨʱ������
#define IPCNet_TIMER_CFG "TimerCfg.info"
typedef struct
{
    BOOL Enable;    
    T_U32 WeekMask;//bit[0]:sunday,bit[1]:monday,bit[2]:tuesday....
    T_S32 Hour;
    T_S32 Min;
    T_S32 Opt;  //0-off,1-on  
}IPCNetTimeManCfg_st;

typedef struct
{
    T_S32 __pri_TimerListCount;
    IPCNetTimeManCfg_st TimerList[8];
}IPCNetTimerInfo_st;



//------------------------Station(���塢nvr)Э��------------------------
#define IPCNET_STATION_DEV_BELL "BellSta"
#define IPCNET_STATION_DEV_CAM "BellCam"

#define IPCNET_STATION_DEV_OPT_WAKEUP "Wakeup"
#define IPCNET_STATION_DEV_OPT_Del "Del"
#define IPCNET_STATION_DEV_OPT_SLEEP "Sleep"

#define IPCNET_STATION_CAM_WAKEUP "OptDev.sta"
typedef struct
{
    char UId[32];
    char Opt[16];
}IPCNetSta_OptDev_st;

#define IPCNET_STATION_CAM_STATUS_ONLINE "on"
#define IPCNET_STATION_CAM_STATUS_OFFLINE "offline"
typedef struct
{
    char UId[32];
    char Status[32]; 
    int BatCap; //����
}IPCNetBellCamProfile_st;
typedef struct
{
    char Type[32];
    IPCNetBellCamProfile_st Profile;
}IPCNetDevProfile_st;

#define IPCNET_STATION_PROFILES "DevProfiles"
struct IPCNetDevProfiles
{
    char Type[32];
    IPCNetDevProfile_st Profiles[4];
    int __pri_ProfilesCount;
};
typedef struct IPCNetDevProfiles IPCNetDevProfiles_st;
typedef struct IPCNetDevProfiles IPCNetSysProfiles_st;


#define IPCNET_P2P_WAKEUP_SETTING "P2PWakeUp.Setting"
#define IPCNET_P2P_WAKEUP_KEEPALIVE_TYPE "KeepAlivePkt"
typedef struct
{
    char LoginAddr[32]; // Login Address
    uint16_t LoginPort;  // Login Port
    
    char LoginPkt[128];  // Login packet length //base64
    //size_t LoginPktLen;  // Login packet data buffer
    uint32_t LoginInterval;  // Send login time interval
    
    char WakeupPkt[128]; // WakeUp pattern length //base64
    //size_t WakeupPktLen;  // WakeUp pattern
}IPCNetP2PKeepAliveInfo_st;

typedef struct
{
    //char SettingType[32];
    IPCNetP2PKeepAliveInfo_st KeepAliveInfo[4];
    int __pri_KeepAliveInfoCount;
}IPCNetP2PWakeUpSetting_st;



//------------------------------------dev info--------------------------------------

typedef struct IpcnetAvInfo
{
	T_U16 ch_no;	//ͨ����
	char audio_code_type; //��Ƶ���ͣ�ö�����ͼ���NPE_AUDIO_TYPE�� Ŀǰʹ�õ���Ƶ������G726��G711A��G711U
	char audio_sample_freq; //��Ƶ����Ƶ�� ,ö�����ͼ���NPE_AUDIO_FREQ��Ŀǰ�̶�ʹ��8K
	T_U16 width[2];    //��������ͨ���ķֱ��ʣ�width[0]��heigh[0]Ϊ�������� width[1]��heigh[1]Ϊ������
	T_U16 heigh[2];
	char chann_name[32];	//ͨ����
}IpcnetAvInfo_st;
/*
typedef struct DEV_SYS_INFO
{
    char name[32];//�豸����
    unsigned char alarm_input_num;//��������·��
    unsigned char alarm_output_num; //�������·��
    unsigned char tos;//�豸����
    char dev_ssr[64];//�豸���к�
	char p2p_uuid[32];
	char dev_ver[32];
	unsigned char subdev_num;
	unsigned char video_inputCount;//����·��
	IpcnetAvInfo_st video_input[4];	//��Ӧ4��ͨ�������ݣ�����ǹ��ֻʹ��һ��ͨ�����ظ��ĳ��Ȼ��ǰ���4��chann_info����ֻchann_info[0]��Ч
	//struct av_info 	video_input[4];	//��Ӧ4��ͨ�������ݣ�����ǹ��ֻʹ��һ��ͨ�����ظ��ĳ��Ȼ��ǰ���4��chann_info����ֻchann_info[0]��Ч
}DEV_SYS_INFO_t;
*/

typedef struct IPCNetSysInfo
{
    char name[32];//�豸����
    unsigned char alarm_input_num;//��������·��
    unsigned char alarm_output_num; //�������·��
    unsigned char tos;//�豸����a
    char dev_ssr[64];//�豸���к�
	char p2p_uuid[32];
	char dev_ver[32];
	unsigned char hw_num;
	unsigned char subdev_num;
	unsigned char __pri_video_inputCount;//����·��
	IpcnetAvInfo_st video_input[4];	//��Ӧ4��ͨ�������ݣ�����ǹ��ֻʹ��һ��ͨ�����ظ��ĳ��Ȼ��ǰ���4��chann_info����ֻchann_info[0]��Ч

	IPCNetSysProfiles_st DevProfiles;
	int __pri_DevProfilesExist;
}IPCNetSysInfo_st;




//!!!------------------------------!!!����������Ҫ��sdkЭ�����չ����������Э��--------------------------------------

//��ȡ�û���Ϣ
/*
/IPCNET_GET_USER_INFO_REQ = 3004,
** @struct IPCNetUserInfoReq_st
** @struct IPCNetUserInfo_st
*/
#define IPCNET_Get_USER_REQ "GetUserReq.info"
typedef struct
{
    char User[IPCNET_STRING_LEN];
}IPCNetUserInfoReq_st;

//���á���ȡ�û���Ϣ
/*
/IPCNET_SET_USER_INFO_REQ = 3006,
*/
#define IPCNET_USER_INFO_CFG "User.info"
typedef struct
{
    char User[IPCNET_STRING_LEN];
    char Passwd[IPCNET_STRING_LEN];
}IPCNetUserInfo_st;


//��ȡȫ���û���Ϣ
/*
/IPCNET_GET_USER_ALL_INFO_REQ = 3002
** @struct IPCNetGetUserGroupCfg_st
*/
#define IPCNET_GET_USER_GROUP_RESP "GroupUserp.info"
typedef struct
{
    char User[IPCNET_STRING_LEN];
    char Passwd[IPCNET_STRING_LEN];
}IPCNetGetUserInfo_st;
typedef struct
{
    IPCNetGetUserInfo_st User[10];
    int __pri_UserCount;
}IPCNetGetUserGroupCfg_st;


//��ȡ�������Ϣ
/*
/IPCNET_GET_SYS_INFO = 3008,
/IPCNET_SET_SYS_INFO = 3010,
** @struct IPCNetCamInfo_st
*/
#define IPCNET_CAM_INFO_CFG "CamCfg.info"

typedef struct
{
    char DevName[64];       //�豸����
    char Vendor[32];        //������Ϣ
    char Model[32];         //�豸�ͺ�
    char Uuid[64];          //�豸ID
    char SysVer[32];        //ϵͳ�汾��
    char FirmVer[32];       //�̼��汾��custom.vendor.web.system.usr
    char SerialNo[64];      //�豸���к�
    char IpcSdk[32];      //sdk�汾��
    char License[16];		//У����
    T_U8 alarm_input_num;
    T_U8 alarm_output_num;
    T_U8 tos;
    T_U8 hw_num;
    T_U8 subdev_num;
    //����·����0-��ʾû������ƵƵ����
    T_U8 __pri_video_inputCount;	
    //��Ӧ4��ͨ�������ݣ�����ǹ��ֻʹ��һ��ͨ�����ظ��ĳ��Ȼ��ǰ���4��chann_info����ֻchann_info[0]��Ч
	IpcnetAvInfo_st video_input[4];	
}IPCNetCamInfo_st;

//����wifi״̬
/***
IPCNET_UPDATE_WIRELESS_STATUS = 3011
***/
#define IPCNET_CAM_UPDATE_WIFI_INFO "UpdateWifiInfo.info"
typedef struct 
{
	char ssid[IPCNET_STRING_LEN];	//��Ӧ��ssid����
	int status;						//IPCNET_WIRELESS_STATUS_e
}IPCNetUpdateWirelessInfo_st;


#define IPCNET_RECORD_COMMAND_INFO "PlayBack.Info"

/***
¼����������
IPCNET_PLAY_RECORD_REQ = 2014
***/
typedef enum
{
    IPCNET_CMD_RECORD_PLAY,
    IPCNET_CMD_RECORD_STOP,
    IPCNET_CMD_RECORD_PAUSE,
    IPCNET_CMD_RECORD_REPLAY,
    IPCNET_CMD_RECORD_SPEED,
    IPCNET_CMD_RECORD_SEEK,
}IPCNET_RECORD_COMMAND_e;

typedef struct
{
    int ViCh;
    int Command; //IPCNET_RECORD_COMMAND_e
    int recorder_id; //IPCNET_CMD_RECORD_STOP���õ�
    char Filename[128];     //���ŵ��ļ�����ΪNULL��ʹ��Date Timeȷ�������ĸ��ļ�
	IPCNetDate_st Date;
    IPCNetTime_st Time;

    /*
    * @Args:����������Command����
    *       IPCNET_CMD_RECORD_SEEK:
                Arg[0]Ϊ���ŵ���ʼʱ��(��Ե�һ֡������)����ʡ�ԣ�Ĭ��Ϊ��ʼ������
    *       IPCNET_CMD_RECORD_SPEED:
                Arg[0]Ϊ�����ٶȣ���ʡ�ԣ�Ĭ��Ϊ����(1)
    *
    */
    int Args[2];
    int __pri_ArgsCount;
}IPCNET_RECORD_REQ_t;

typedef struct
{
    int recorder_id; //server�˵�id
    char stream_shm[32];
}IPCNET_RECORD_RESP_t;

#define IPCNET_RECORD_OPT_INFO "Record.Opt"
#define IPCNET_RECORD_OPT_START "Start"
#define IPCNET_RECORD_OPT_END "End"
typedef struct
{
    char Opt[16];
    int Duration; //seconds
}IPCNET_RECORD_OPT_t;


/****����ʶ��****/
/****
IPCNET_FR_GET_USER_REQ = 1543, //����ʶ���ȡ��ǰ���õ��û���Ϣ
@struct IPCNetFrGetUserInfo_t
IPCNET_FR_GET_USER_RESP,
***/
#define IPCNET_FR_GET_USER_INFO "FrUserInfo.info"
#define IPCNET_FR_MAX_USER 10	//�������Ŀǰ���֧��10��

typedef struct 
{
	int UserID;
	char Name[32];
	char Sex[16];
	int Age;
	char ImgPath[64];
}IPCNetFrUserInfo_st;

typedef struct 
{
	int __pri_FrUserListCount;
	IPCNetFrUserInfo_st FrUserList[IPCNET_FR_MAX_USER];
}IPCNetFrGetUserInfo_t;

/*****
IPCNET_FR_COLLECTION_REQ = 1545, //����ʶ�𣬲ɼ�������Ϣ
@struct IPCNetFrCollectionReq_st
IPCNET_FR_COLLECTION_RESP,
@struct IPCNetFrCollectResp_st
*****/
#define IPCNET_FR_COLLECT_USER_INFO "FrCollectFace.info"

typedef enum
{
	IPCNET_FR_FRONT_FACE_E,			//0-����         
	IPCNET_FR_LEFT_FACE_E,			//1-����
	IPCNET_FR_RIGHT_FACE_E,			//2-����
	IPCNET_FR_TILTING_LEFT_FACE_E,	//3-����б��
	IPCNET_FR_TILTING_RIGHT_FACE_E,	//4-����б��
	IPCNET_FR_LOOKING_UP_E,			//5-̧ͷ
	IPCNET_FR_LOOKING_DOWN_E,		//6-��ͷ
	IPCNET_FR_FACE_UNKOW_E			//7-δ֪
}IPCNET_FACE_ANGLE_E;

typedef struct
{
	IPCNET_FACE_ANGLE_E Angle;	//ץ�ĽǶ�
}IPCNetFrCollectionReq_st;

typedef struct 
{
	IPCNET_FACE_ANGLE_E Angle;	//��ǰ��Ƭ��ץ�ĽǶ�
	char ImgPath[64];
}IPCNetFrCollectResp_st;

/***
IPCNET_FR_ADD_USER_BY_COLLECT_REQ = 1547, //���ݲɼ���������Ϣ����û�
@struct IPCNetFrAddCollectUser_st

IPCNET_FR_ADD_USER_BY_COLLECT_RESP,
@struct IPCNetNULL_st
********
IPCNET_FR_ADD_USER_BY_VISIT_HISTORY_REQ = 1549, //��������ʷ��¼������û�
@struct IPCNetFrImgName_st

IPCNET_FR_ADD_USER_BY_VISIT_HISTORY_RESP,
@struct IPCNetNULL_st
***/

#define IPCNET_FR_USER_BY_COLLECT_INFO "AddCollectUser.info"

typedef struct 
{
	char ImgName[64];
}IPCNetFrImgName_st;

typedef struct 
{
	char Name[32];	//�û�����
	int FaceId;		//����ID
	int ImgNum;		//ͼƬ���
	int __pri_ImgNameListCount;
	IPCNetFrImgName_st ImgNameList[9];	//ͼƬ����
}IPCNetFrAddCollectUser_st;

/****
IPCNET_FR_MODIFY_USER_INFO_REQ = 1551, //�޸��û���Ϣ
@struct IPCNetFrModifyUserInfo_st

IPCNET_FR_MODIFY_USER_INFO_RESP,
@struct IPCNetNULL_st
****/
#define IPCNET_MODIFY_USER_INFO "FrModifyUser.info"
typedef struct 
{
	int UserID;
	char Name[64];
	int Age;
	char Sex[16];
	char ImgName[64];
}IPCNetFrModifyUserInfo_st;

/******
IPCNET_FR_DEL_USER_INFO_REQ = 1553, //ɾ���û���Ϣ
@struct IPCNetDelUserInfo_st
IPCNET_FR_DEL_USER_INFO_RESP,
@struct IPCNetNULL_st
******/
#define IPCNET_DEL_USER_INFO "frDelUser.info"

typedef struct
{
	int UserId;//>=0,��Ӧ���û�ID��-1-ɾ�����л�
}IPCNetDelUserInfo_st;

/*****
IPCNET_FR_GET_VISIT_HISTORY_REQ = 1555, //��ȡ���ü�¼
@struct IPCNETGetVisitHistoryReq_st
IPCNET_FR_GET_VISIT_HISTORY_RESP,
@struct IPCNetFrGetVisitHistoryResp_st
*****/
#define IPCNET_GET_VISIT_HISTORY_INFO "FrGetVisityHistroy.info"

typedef struct 
{
	int GetNum;//�����¼�ĸ���
}IPCNETGetVisitHistoryReq_st;

typedef struct 
{
	char Name[32];//�ݷ��ߵ�����
	char ImgPath[64];//ͼƬ��·��
	char Sex[16];//�Ա�
	int Age;//����
	int Cnt;//�ݷõĴ���
	IPCNetDate_st Date;
    IPCNetTime_st Time;
}IPCNetFrGetVisitHistoryResp_st;


/*******
IPCNET_FR_FUNC_ENABLE_REQ = 1557, //������⹦�ܿ���
@IPCNETNetFrFuncEnabe_st 
IPCNET_FR_FUNC_ENABLE_RESP,
*******/
#define IPCNET_FR_FUNC_ENABLE_INFO "FrFucEnable.info"
typedef struct 
{
	BOOL Enable;	//true-ʹ�ܣ�false-��ʹ��
}IPCNETNetFrFuncEnabe_st;

typedef enum
{
	PIC_FR_COLLECT,			//�����ɼ�ͼƬ
	PIC_FR_DETECT,			//��⵽����ͼƬ
	PIC_FR_VISIT_HISTORY,	//������ʷ��¼ͼƬ
}IPCNetFrPicType_e;

typedef struct 
{
	IPCNetFrPicType_e Type;	//�������Ļص�����
	int PicName[32];
	int PicRemotePath[32];
	int PicLen;
}IPCNetFrPicInfo_st;

/*********
IPCNET_FR_DETECT_PIC = 1561
@struct IPCNetDetectFaceInfo_st
**********/
typedef struct 
{
	//ʶ���ʱ��
	IPCNetDate_st Date;
    IPCNetTime_st Time;
	char Name[32];//��ʶ���ߵ�����
	int Cnt;//��ʶ��Ĵ���
}IPCNetDetectFaceInfo_st;//��ʶ���ߵ���Ϣ


//!!!------------------------------!!!����������Ҫ��sdkЭ�����չ����������Э��--------------------------------------


#endif

