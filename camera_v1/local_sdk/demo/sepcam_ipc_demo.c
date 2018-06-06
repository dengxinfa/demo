#include "os_ini.h"
#include "sepcam_ipc_api.h"
#include "ipcnet_struct.h"
#include <unistd.h>
#include "convert.h"

#define IPC_BUF_LEN 4*1024
char get_buf[IPC_BUF_LEN];
char set_buf[IPC_BUF_LEN];
char *pic_path;
void savebmp(char* data, int width, int height, char *suffix);

typedef struct
{
    const int vench;
    int get_index;
    uint16_t frame_index;
}CHECK_FRAME_INDEX_t;

CHECK_FRAME_INDEX_t check_index[2] = {
    {0, 0, 0},
    {1, 0, 0},
};

//#define GET_VFRAME_TEST
#ifdef GET_VFRAME_TEST

//#define SAVE_BMP_TEST
#ifdef SAVE_BMP_TEST

static long int crv_tab[256];
static long int cbu_tab[256];
static long int cgu_tab[256];
static long int cgv_tab[256];
static long int tab_76309[256];
static unsigned char clp[1024]; //for clip in CCIR601


void init_convert_table()
{
    long int crv,cbu,cgu,cgv;
    int i,ind;

    crv = 104597; cbu = 132201; /* fra matrise i global.h */
    cgu = 25675; cgv = 53279;

    for (i = 0; i < 256; i++) {
        crv_tab[i] = (i-128) * crv;
        cbu_tab[i] = (i-128) * cbu;
        cgu_tab[i] = (i-128) * cgu;
        cgv_tab[i] = (i-128) * cgv;
        tab_76309[i] = 76309*(i-16);
    }

    for (i=0; i<384; i++)
        clp[i] =0;

    ind=384;

    for (i=0;i<256; i++)
        clp[ind++]=i;

    ind=640;

    for (i=0;i<384;i++)
        clp[ind++]=255;
}

void convert_yc2rgb(unsigned char *src0,unsigned char *src1,
                    unsigned char *dst_ori,int width,int height)
{
    int y1,y2,u,v;
    unsigned char *py1,*py2;
    int i,j, c1, c2, c3, c4;
    unsigned char *d1, *d2;
    py1=src0;
    py2=py1+width;
    d1=dst_ori;
    d2=d1+3*width;

    for (j = 0; j < height; j += 2) {
        for (i = 0; i < width; i += 2) {
            u = *src1++;
            v = *src1++;
            c1 = crv_tab[v];
            c2 = cgu_tab[u];
            c3 = cgv_tab[v];
            c4 = cbu_tab[u];
            //up-left
            y1 = tab_76309[*py1++];
            *d1++ = clp[384+((y1 + c1)>>16)];
            *d1++ = clp[384+((y1 - c2 - c3)>>16)];
            *d1++ = clp[384+((y1 + c4)>>16)];
            //down-left
            y2 = tab_76309[*py2++];
            *d2++ = clp[384+((y2 + c1)>>16)];
            *d2++ = clp[384+((y2 - c2 - c3)>>16)];
            *d2++ = clp[384+((y2 + c4)>>16)];
            //up-right
            y1 = tab_76309[*py1++];
            *d1++ = clp[384+((y1 + c1)>>16)];
            *d1++ = clp[384+((y1 - c2 - c3)>>16)];
            *d1++ = clp[384+((y1 + c4)>>16)];
            //down-right
            y2 = tab_76309[*py2++];
            *d2++ = clp[384+((y2 + c1)>>16)];
            *d2++ = clp[384+((y2 - c2 - c3)>>16)];
            *d2++ = clp[384+((y2 + c4)>>16)];
        }

        d1 += 3*width;
        d2 += 3*width;
        py1+= width;
        py2+= width;
    }
}

static int save_yc2rgb_pic(VFRAME_INFO_t *pframe)
{
    VFRAME_t vframe = pframe->frame_info;
    int height=vframe.height;
    int width=vframe.width;
    int size = vframe.addr[0].stride* vframe.height * 3 / 2;
    unsigned char *rgb = (unsigned char *)malloc(height*width*3);
    unsigned char* pVBufVirt_Y;
    unsigned char* pVBufVirt_C;
    unsigned char *pUserPageAddr;

    static int table_init = 0;

    if(!table_init)
    {
        init_convert_table();
        table_init = 1;
    }
    printf("0x%x 0x%x\n", vframe.addr[0].phy_addr, vframe.addr[1].phy_addr);
    
#if 0
    convert_yc2rgb(vframe.u32PhyAddr[0], vframe.u32PhyAddr[1], rgb, width, height);
#else
    pUserPageAddr = (unsigned char *)ipcam_sys_mmap(vframe.addr[0].phy_addr, size);
    pVBufVirt_Y = pUserPageAddr;
    pVBufVirt_C = pVBufVirt_Y + vframe.addr[0].stride* vframe.height;
    convert_yc2rgb(pVBufVirt_Y, pVBufVirt_C, rgb, width, height);
    ipcam_sys_munmap(pUserPageAddr, size);
#endif
    savebmp((char *)rgb, width, height, NULL);
    free(rgb);
    return 0;
}
#endif

int get_vframe()
{
    VFRAME_INFO_t frame;
    int getframeIndex;
    int ret;
    while(1)
    {
        ret = ipcam_get_vframe(0, &frame);
        //printf("ipcam_get_vframe sizeof(vi_frame_info) = %d ret:%d\n", sizeof(vi_frame_info), ret);
        if(ret == -1)
        {
            printf("ipcam_get_vframe FAIL\n");
            exit(0);
        }
        else if(ret > 0)
        {
#ifdef SAVE_BMP_TEST
            printf("save_yc2rgb_pic <<<<<<<<<<<<<\n");
            save_yc2rgb_pic(&frame);
#endif
            //printf("ipcam_free_viframe<<<<<<<<<<<<<<<<<\n");
            ret = ipcam_free_vframe(frame.frame_id);
            //printf("ipcam_free_viframe<<<<<<<<<<<<<<<<<ret:%d\n", ret);
        }
        usleep(10*1000);
    }
}

void *get_vframe_thread(void *p)
{
    int vchn;

    vchn = ipcam_start_vframe(0, NULL, NULL);
    if(vchn <= 0)
    {
        printf("ipcam_start_vframe FAIL\n");
        return NULL;
    }
    
    IPCNetGetUserGroupCfg_st group;
    int ret = ipcam_get_device_config(IPCNET_GET_USER_ALL_INFO_REQ, &group);
    if(ret == 0)
    {
        int i;
        printf("IPCNET_GET_USER_ALL_INFO_REQ user num:%d\n", group.__pri_UserCount);
        for(i = 0; i < group.__pri_UserCount; i++)
        {
            printf("i:%d user:%s passwd:%s\n", i, group.User[i].User, group.User[i].Passwd);
        }
    }
    else
    {
        printf("ipcam_get_device_config IPCNET_GET_USER_ALL_INFO_REQ FAIL!!\n");
    }
    get_vframe();
    
    return NULL;
}

int get_vframe_test()
{
    pthread_t tid;
    pthread_create(&tid, NULL, get_vframe_thread, NULL);
}
#endif


char video_save_file_path[162] = {"./test_video.aac"};
char audio_save_file_path[162] = {"./test_audio.aac"};
char save_file_path[162] = {0};
static FILE *fp = NULL;

int save_file(char *buf, size_t datalen)
{
    char *pfile = save_file_path;
    //printf("save_file\n");
    if(!strlen(pfile))
        return 0;
        
    if(!fp)
    {
        fp = fopen(pfile, "w");
        assert(fp);
        printf("create file save_file_path:%s\n", save_file_path);
    }
    printf("save_vile datalen:%d\n", datalen);
    fwrite(buf, datalen, 1, fp);
    return 0;
}

int close_file()
{
    if(fp)
    {
        fclose(fp);
        fp = NULL;
    }
    return 0;
}

#define frame_is_key(frame_type) (frame_type >4)

void* get_frame_task(void *p)
{
    STREAM_FRAME_INFO_t frameinfo;
    static char *buff = NULL;
    static unsigned int bufflen = 0;
    static int first_read[2] = {1, 1};
    int has_read_iframe = 0;
    int getframeIndex;
    int vench;
    int ret;

    CHECK_FRAME_INDEX_t *check_index = (CHECK_FRAME_INDEX_t *)p;
    vench = check_index->vench;
    
    while(1)
    {
        ret = ipcam_get_vencstream(vench, &frameinfo, buff, bufflen, &getframeIndex);
        //printf("ipcam_get_vencstream ret:%d\n", ret);
        if(ret < 0)
        {
            printf("ret:%d des:%s\n", ret);
            if(ret == SHM_RDWR_BUF_SIZE_ERR)
            {
                printf("need size:%d bufflen:%d\n", frameinfo.frame_size, bufflen);
                buff = (char *)realloc(buff, frameinfo.frame_size);
                bufflen = frameinfo.frame_size;
                ret = ipcam_get_vencstream(vench, &frameinfo, buff, bufflen, &getframeIndex);
            }
            else
            {
                exit(-1);
            }
        }
        
        if(ret > 0)
        {
            printf("frame_no:%d frame_type:%d\n", frameinfo.frame_no, frameinfo.frame_type);
            /*****************************************************************************************************
            * 以下代码是为了测试取帧是否正常有没有丢帧的情况
            * 实际应用的时候取完帧后按客户应用需要做一些小数据量的处理后，应即刻把数据扔给相应的业务线程去处理
            *****************************************************************************************************/
            if(first_read[vench])
            {
                first_read[vench] = 0;
                check_index->get_index = getframeIndex;
                check_index->frame_index = frameinfo.frame_no;
            }
            else
            {
                check_index->get_index++;
                check_index->frame_index++;
                if(check_index->get_index > 127)
                {
                    check_index->get_index = 0;
                }
                if(check_index->get_index != getframeIndex
                  || check_index->frame_index != frameinfo.frame_no)
                {
                    printf("err!!!check_index:%d getframeIndex:%d\n",
                        check_index->get_index, getframeIndex);
                    printf("err!!!frame_index:%d frame_no:%d\n",
                        check_index->frame_index, frameinfo.frame_no);
	                check_index->get_index = getframeIndex;
	                check_index->frame_index = frameinfo.frame_no;
                }
            }
			//if(frameinfo.frame_no%1000 == 0)
	            //printf("frame_no:%d getframeIndex:%d frame_size:%d payload:%d frame_type:%d\n",
	            //    frameinfo.frame_no, getframeIndex,
	            //    frameinfo.frame_size, frameinfo.payload, frameinfo.frame_type);
	            if(frame_is_key(frameinfo.frame_type))
                    printf("frame_no:%d %02x %02x %02x %02x %02x %02x %02x\n",
                        frameinfo.frame_no, buff[0],
                        buff[1],buff[2],
                        buff[3],buff[4],
                        buff[5],buff[6]);

	        if(has_read_iframe)
	        {
	            save_file(buff, frameinfo.frame_size);
	        }
	        else
	        {
	            if(frame_is_key(frameinfo.frame_type))
	            {
	                printf("start record.....frame_no:%d getframeIndex:%d\n", frameinfo.frame_no, getframeIndex);
	                save_file(buff, frameinfo.frame_size);
	                has_read_iframe = 1;
	            }
	        }
        }
        usleep(10*1000);
    }
    return 0;
}

char audio_buf[640];
void read_audio_frame_test()
{
    STREAM_FRAME_INFO_t frameinfo;
    int getframeIndex;
    int ret;
    while(1)
    {
        printf("ipcam_get_aencstream<<<<<<<<<<<<<<<<<\n");
        ret = ipcam_get_aencstream(&frameinfo, audio_buf, 640, &getframeIndex);
        if(ret > 0)
        {
            printf("get audio - frame_size:%d\n", frameinfo.frame_size);
            save_file(audio_buf,frameinfo.frame_size);
        }
        usleep(10*1000);
    }
}

int create_read_steam(int vench)
{
    pthread_t tid;

    pthread_create(&tid, NULL, get_frame_task, &check_index[vench]);
    return 0;
}

#if 1
#include <sys/time.h>
static pthread_t gRecThreadID = 0;
static int rec_id = -1;
static int rec_stop = 0;
static char stream_shm[32];
static char rec_buf[512*1024];

static int getTimevalDiff(struct timeval x , struct timeval y)
{
    int x_ms , y_ms , diff;

    x_ms = x.tv_sec*1000 + x.tv_usec/1000;
    y_ms = y.tv_sec*1000 + y.tv_usec/1000;

    diff = y_ms - x_ms;

    return diff;
}

void *play_thread(void*)
{
    STREAM_FRAME_INFO_t frameinfo;
    int getframeIndex;
    int ret;

    printf("play_thread rec_id:%d\n", rec_id);
    FILE *fp = fopen("/rom/nfs/record_pres.h264", "w");

    struct timeval tv, tv_bak = {0, 0};
    struct timeval atv, atv_bak = {0, 0};
    while(1)
    {
        if(rec_stop)
        {
            printf("play_thread stop\n");
            ipcam_recorder_stop(rec_id);
            fclose(fp);
            pthread_exit(0);
        }
        
        ret = get_ipcam_recorder_stream(rec_id, &frameinfo, rec_buf, 512*1024, &getframeIndex);
        if(ret > 0)
        {
            //printf("get_ipcam_recorder_stream ret:%d\n", ret);
            if(frameinfo.payload == 96)
            {
                //fwrite(rec_buf, ret, 1, fp);
                gettimeofday(&tv, NULL);
                printf("diff msec:%d payload:%d frame_no:%d timestamp:%lld\n",
                    getTimevalDiff(tv_bak, tv), frameinfo.payload, frameinfo.frame_no, frameinfo.timestamp);
                tv_bak = tv;
            }
            else
            {
                //fwrite(rec_buf, ret, 1, fp);
                gettimeofday(&atv, NULL);
                printf("diff msec:%d payload:%d frame_no:%d timestamp:%lld\n",
                    getTimevalDiff(atv_bak, atv), frameinfo.payload, frameinfo.frame_no, frameinfo.timestamp);
                atv_bak = atv;
            }
        }
        else if(ret == 0)
        {
            if(frameinfo.flag)
            {
                printf("get_ipcam_recorder_stream end\n");
                fclose(fp);
                pthread_exit(0);
            }
        }
        usleep(10);
    }
}

int record_test()
{
    IPCNET_RECORD_REQ_t request;
    char filename[512];

    printf("record request - input the filename\n");
    gets(filename);
    printf("Your input is:%s\n", filename);

    if(strlen(filename) >= 128)
    {
        printf("%s too long\n", filename);
        return 0;
    }

    memset(&request, 0, sizeof(request));
    strcpy(request.Filename, filename);
    request.Command = IPCNET_CMD_RECORD_PLAY;
    request.recorder_id = -1;
    rec_id = ipcam_recorder_play(&request);
    if(rec_id < 0)
    {
        printf("ipcam_recorder_play FAIL\n");
        return 0;
    }

    rec_stop = 0;
    pthread_create(&gRecThreadID, NULL, play_thread, NULL);
#if 0
    printf("input any key to stop\n");
    getchar();
    rec_stop = 1;
    printf("input any key again!\n");
    getchar();
#else
    int pause = 1;

    int speed = 1;
    int seek_flag = 1;
    int seek_secs;
    while(1)
    {
        printf("input any key to pause or replay\n");
        getchar();
        
    #if 0 //pause test
        ipcam_recorder_ctrl(rec_id, pause);
        pause = pause ? 0 : 1;
    #endif //pause test
    
    #if 0 //speed test
        ipcam_recorder_speed(rec_id, speed, 4);
        speed++;
        //if(speed == 0)
          //  speed = 1;
        if(speed > 8)
            speed = 1;
    #endif //speed test

    #if 1 //seek test
        if(seek_flag)
        {
            srandom(time(NULL));
            seek_flag = 0;
        }
        seek_secs = random()%600;
        printf("#####seek_secs:%d\n", seek_secs);
        ipcam_recorder_seek(rec_id, seek_secs);
    #endif
    }
#endif

    pthread_join(gRecThreadID, NULL);
    printf("pthread_join>>>>>>>>>>>>>>>>>>>>>>>\n");
    exit(0);
}
#else
int record_test()
{
    STREAM_FRAME_INFO_t frameinfo;
    static char *buff = NULL;
    static unsigned int bufflen = 0;
    int getframeIndex;
    int ret;

    IPCAM_SDK_RECORD_REQ_t rec_req;
    memset((void *)&rec_req,0,sizeof(rec_req));
    rec_req.vi_ch = 0;
    rec_req.opt = IPCAM_SDK_RECORD_PLAY;
    rec_req.rec_time.year = 2017;
    rec_req.rec_time.month = 9;
    rec_req.rec_time.day = 1;
    rec_req.rec_time.hour = 10;
    rec_req.rec_time.minute = 30;
    rec_req.rec_time.second = 50;
    ipcam_rec_play(&rec_req);
    while(1)
    {
        //printf("ipcam_get_recstream<<<<<<<<<<<<<<<<<<\n");
        ret = ipcam_get_recstream(&frameinfo, buff, bufflen, &getframeIndex);
        //printf("ipcam_get_recstream>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>ret:%d\n", ret);
        if(ret == -1)
        {
            printf("need size:%d bufflen:%d\n", frameinfo.frame_size, bufflen);
            buff = (char *)realloc(buff, frameinfo.frame_size);
            bufflen = frameinfo.frame_size;
            ret = ipcam_get_recstream(&frameinfo, buff, bufflen, &getframeIndex);
            printf("ret:%d\n", ret);
        }

        if(ret == 0)
        {
            if(frameinfo.flag)
            {
                printf("record end\n");
                close_file();
                exit(0);
            }
            usleep(10*1000);
            continue;
        }

        printf("payload:%d\n", frameinfo.payload);
        if(frameinfo.payload == 96)
        {
            save_file(buff, bufflen);
        }
        usleep(10*1000);
    }
    return 0;
}
#endif


char *print_param_test = "\n\n*********************************\n" \
    "1. Get Video Param\n" \
    "2. Set Video Param(no finish)\n" \
    "3. Get Eth param\n" \
    "4. Set Eth param\n" \
    "5. Get Cam pic param(no finish)\n" \
    "6. Set Cam pic param(no finish)\n" \
    "7. Get user all\n" \
    "8. Get one user info\n" \
    "9. Set one user info\n" \
    \
    "10. Get time\n" \
    "11. Set time\n" \
    \
    "12. Get md\n" \
    "13. Set md\n" \
    \
    "14. Get device\n"\
    "15. Set dev name\n"\
    "16. Get osd\n"\
    "17. Set osd\n"\
    \
    "18. search ap list\n"\
    \
    "19. get record info\n"\
    \
    "20. get daylight info\n"\
    "21. set daylight info\n"\
    \
    "22. request IFrame\n"\
    "23. set video quality\n"\
    "24. set audio enc cfg info\n"\
    "25. set audio dec cfg info\n"\
    "26. get ftp cfg info\n"\
    "27. set ftp cfg info\n"\
    "28. get upgrade url info\n"\
    "29. get fireware version info\n"\
    "30. set time info\n" \
    \
    "31. get wifi info\n"\
    "32. set wifi info\n" \
    "33. get pic overturn\n"\
    "34. set pic overturn\n"\
    "35. get the point num\n"\
    "36. add point \n"\
    "37. go to the point\n"\
    "38. get the record param\n"\
    "39. set the record param\n"\
    "40. Dev reboot\n"\
    "41. Dev default set\n"\
    "42. get alarm config\n"\
    "43. set alarm config\n"\
    "44. regist alarm report\n"\
    "45. storage format\n"\
    "46. request snap pic\n" \
    \
    "100. get stations profiles\n" \
    "101. opt the device of stations\n" \
    
    "*********************************\n" \
    "input the num for test:\n"; \

IPCNetVideoEncodeCfg_st video_cfg;
IPCNetEthConfig_st eth;
IPCNetTimeCfg_st time_cfg;
IPCNETMoveAlarmCfg_st video_md;
IPCNetPicOverTurn_st video_overturn;
IPCNETMoveAlarmCfg_st alarm_cfg;

void wifi_ap_list_callback(IpcnetNetworkWirelessSearch_st *ap, void* user_data)
{
    int i;
    printf("ap num:%d user_data:%s\n", ap->__pri_AplistCount, user_data);
    for(i = 0; i < ap->__pri_AplistCount; i++)
    {
        printf("ap name:%s\n", ap->Aplist[i].SSID);
    }
}

static void req_snap_callback(SNAP_FRAME_t *pFrameInfo, void* user_data)
{
   	printf("pkt_num[%d],pkt[%p]\n",pFrameInfo->pkt_num,pFrameInfo->pkt);
   	int i = 0,ret=0;
   	int frame_len = 0;
   	SNAP_PAKT_t *pack_info = NULL;
   	char *pFileName = "/tmp/nfs/test.jpeg";
	FILE *fp = NULL;
	fp = fopen(pFileName, "w+");
	if(fp == NULL)
	{
		printf("fopen %s FAIL!\n",pFileName);
		return;
	}
	
   	for(i=0; i<pFrameInfo->pkt_num; i++)
   	{
   		pack_info = pFrameInfo->pkt+i;
		printf("i=%d,addr[%p],len[%d]\n",i,pack_info->addr,pack_info->len);
		ret = fwrite(pack_info->addr, 1, pack_info->len,fp);
		printf("fwrite size=%d\n",ret);
		frame_len += pack_info->len;
   	}
	printf("frame_len=%d\n",frame_len);

	fclose(fp);
	
    return;
}

void alarm_report_callback(IPCNetAlarmMsgReport_st *alarm_msg)
{
	printf("<<<<<<<<<<<<%d-%d-%d-%d>>>>>>>>>>>>>\n", alarm_msg->AlarmDate.Year, alarm_msg->AlarmDate.Mon, alarm_msg->AlarmDate.Day, alarm_msg->AlarmDate.WDay);
	printf("<<<<<<<<<<<<%d:%d:%d>>>>>>>>>>>>>\n", alarm_msg->AlarmTime.Hour, alarm_msg->AlarmTime.Min, alarm_msg->AlarmTime.Sec);
}

void storage_format_callback(IPCNetRet_st *pFomatResult, void *user)
{
	printf("ret:%d user:%s\n", pFomatResult->ret, user);
}

void Print_motion_time(IPCNETMoveAlarmCfg_st palarm_cfg)
{	
	int i;
	
	for(i=0; i<8; i++)
	{
		printf("Week[%d].flag=%d\n", i, palarm_cfg.Week[i].Flag);
		printf("Week[%d].__pri_TimePeriodCount=%d\n", i, palarm_cfg.Week[i].__pri_TimePeriodCount);
		printf("Star0:%d-%d-%d\n", palarm_cfg.Week[i].TimePeriod[0].Start.Hour, palarm_cfg.Week[i].TimePeriod[0].Start.Min, palarm_cfg.Week[i].TimePeriod[0].Start.Sec);
		printf("End0:%d-%d-%d\n", palarm_cfg.Week[i].TimePeriod[0].End.Hour, palarm_cfg.Week[i].TimePeriod[0].End.Min, palarm_cfg.Week[i].TimePeriod[0].End.Sec);
		printf("Star1:%d-%d-%d\n", palarm_cfg.Week[i].TimePeriod[1].Start.Hour, palarm_cfg.Week[i].TimePeriod[1].Start.Min, palarm_cfg.Week[i].TimePeriod[1].Start.Sec);
		printf("End1:%d-%d-%d\n", palarm_cfg.Week[i].TimePeriod[1].End.Hour, palarm_cfg.Week[i].TimePeriod[1].End.Min, palarm_cfg.Week[i].TimePeriod[1].End.Sec);
	}
}

int param_test()
{
    int input;
    char input_str[4];
    int ret;
    int i, j;

    while(1)
    {
        printf(print_param_test);

#if 0
        while(1)
        {
            input = getchar();
            if(input != '\n' && input != '\r' && input != ' ')
                break;
        }
        getchar();
#endif
        input = 0;
        input_str[0] = '\0';
        gets(input_str);
        input = atoi(input_str);
        
        printf("Your input is:%d\n", input);
        switch(input)
        {
        case 1:
        {   //获取视频参数
            ret = ipcam_get_device_config(IPCNET_VIDEO_ENC_GET_REQ, &video_cfg);
            printf("IPCNET_VIDEO_ENC_GET_REQ ret:%d ViCh:%d VideoEncodeNum:%d\n",
                ret, video_cfg.ViCh, video_cfg.VideoEncodeNum);
            for(i = 0; i < video_cfg.__pri_VideoEncodeCount; i++)
            {
                printf("vench:%d resolution:(%d,%d) frm:%d rate:%d res option:%d\n",
                    i, video_cfg.VideoEncode[i].Witdh, video_cfg.VideoEncode[i].Height,
                    video_cfg.VideoEncode[i].FrameRate, video_cfg.VideoEncode[i].Bitrate,
                    video_cfg.VideoEncode[i].__pri_VideoResolutionOptCount);
                for(j = 0; j < video_cfg.VideoEncode[i].__pri_VideoResolutionOptCount; j++)
                {
                    printf("opton res:%d,%d\n", 
                        video_cfg.VideoEncode[i].VideoResolutionOpt[j].Witdh,
                        video_cfg.VideoEncode[i].VideoResolutionOpt[j].Height);
                }
            }
            
        }
        break;

        case 2:
        {   
        }
        break;

        case 3:
        {   //获取网卡信息
            ret = ipcam_get_device_config(IPCNET_ETH_GET_REQ, &eth);
            printf("IPCNET_ETH_GET_REQ mac:%s ip:%s\n", eth.Mac, eth.IP);
        }
        break;

        case 4:
        {   //设置网卡信息       
            IPCNetEthConfig_st eth_cof;
            memset(&eth_cof, 0, sizeof(IPCNetEthConfig_st));

            char ip[IPCNET_IP_STRING_LEN] = "";
            
            ret = ipcam_get_device_config(IPCNET_ETH_GET_REQ, &eth_cof);
            printf("the ip is :%s\n", eth_cof.IP);
            printf("please input the same network(xxx.xxx.xxx.xxx)\n");
            gets(ip);
            memcpy(eth_cof.IP, ip, sizeof(eth_cof.IP));
            eth_cof.IP[sizeof(eth_cof.IP) -1] = '\0';
            printf("ip=%s\n", eth_cof.IP);
            
            ret = ipcam_set_device_config(IPCNET_ETH_SET_REQ, &eth_cof);
        }
        break;
            
        case 7:
        {   //获取所有用户信息
            IPCNetGetUserGroupCfg_st group;
            ret = ipcam_get_device_config(IPCNET_GET_USER_ALL_INFO_REQ, &group);
            printf("IPCNET_GET_USER_ALL_INFO_REQ user num:%d\n", group.__pri_UserCount);
            for(i = 0; i < group.__pri_UserCount; i++)
            {
                printf("i:%d user:%s passwd:%s\n", i, group.User[i].User, group.User[i].Passwd);
            }
        }
        break;

        case 8:
        {   
            //输入对应的用户名得到相应的用户信息
            IPCNetUserInfoReq_st user_req;
            IPCNetUserInfo_st user_info;
            char name[64];
            printf("input the name you want to find:\n");
            gets(name);
            strncpy(user_req.User, name, sizeof(user_req.User));
            user_req.User[sizeof(user_req.User)-1] = '\0';
            printf("name:%s\n", user_req.User);
            ipcam_device_config(IPCNET_GET_USER_INFO_REQ, &user_req, &user_info);
            printf("user:%s passwd:%s\n", user_info.User, user_info.Passwd);
        }
        break;

        case 9:
        {   
            //修改用户信息
            IPCNetUserInfo_st user_info;
            char getstr[64];
            printf("input the name you want to change:\n");
            gets(getstr);
            strncpy(user_info.User, getstr, sizeof(user_info.User));
            user_info.User[sizeof(user_info.User)-1] = '\0';
            printf("input the password you want to change:\n");
            gets(getstr);
            strncpy(user_info.Passwd, getstr, sizeof(user_info.Passwd));
            user_info.Passwd[sizeof(user_info.Passwd)-1] = '\0';
            ipcam_set_device_config(IPCNET_SET_USER_INFO_REQ, &user_info);
            printf("user:%s passwd:%s\n", user_info.User, user_info.Passwd);
        }
        break;
        
        case 10:
        {
            //获取当前时间
            ipcam_get_device_config(IPCNET_GET_TIME_REQ, &time_cfg);
            printf("%d-%d-%d %d:%d:%d\n", 
                time_cfg.Date.Day, time_cfg.Date.Mon, time_cfg.Date.Year,
                time_cfg.Time.Hour, time_cfg.Time.Min, time_cfg.Time.Sec);
        }
        break;

        case 11:
        {   
            //设置时间(记得关闭电脑自动校时AdjustFlg)
            int hour;
            char getstr[64];
            printf("input the hour you want to change:\n");
            gets(getstr);
            hour = atoi(getstr);
            if(hour < 24 && hour > 0)
            {
                time_cfg.Time.Hour = hour;
                ipcam_set_device_config(IPCNET_SET_TIME_REQ, &time_cfg);
            }
        }
        break;
        
        case 12:
        {
            //获取报警设置信息
            int w;
            ret = ipcam_get_device_config(IPCNET_MOVE_ALARM_GET_REQ, &video_md);
            printf("IPCNET_MOVE_ALARM_GET_REQ>>>>>>>>>>>>>>>>>>\n");
            if(ret == 0)
            {
                printf("sensity:%d cell:%s\n", video_md.MoveCell.Sensitive, video_md.MoveCell.Cell);
                for(w = 0; w < 8; w++)
                {
                    printf("week:%d __pri_TimePeriodCount:%d ",
                        w, video_md.Week[w].__pri_TimePeriodCount);
                    for(i = 0; i < video_md.Week[w].__pri_TimePeriodCount; i++)
                    {
                        printf("%d start-%d:%d:%d end--%d:%d:%d ",
                            video_md.Week[w].TimePeriod[i].Start.Hour, video_md.Week[w].TimePeriod[i].Start.Min, video_md.Week[w].TimePeriod[i].Start.Sec,
                            video_md.Week[w].TimePeriod[i].End.Hour, video_md.Week[w].TimePeriod[i].End.Min, video_md.Week[w].TimePeriod[i].End.Sec);
                    }
                    printf("\n");
                }
            }
            else
            {
                printf("ipcam_get_device_config IPCNET_MOVE_ALARM_GET_REQ err:%d\n", ret);
            }
        }
        break;

        case 13:
        {   
            //设置报警使能
            int enable = 0;

            while(1)
            {
                printf("enable md alarm: yes or no\ninput your choose:\n");
                input_str[0] = '\0';
                gets(input_str);
                printf("\nyour input is:%s\n", input_str);
                if(strcmp(input_str, "yes") == 0)
                {
                    enable = 1;
                }
                else if(strcmp(input_str, "no") == 0)
                {
                    enable = 0;
                }
                else
                {
                    continue;
                }
                
                break;
            }

            
            ret = ipcam_get_device_config(IPCNET_MOVE_ALARM_GET_REQ, &video_md);
            if(ret == 0)
            {
                video_md.MoveCell.MdEnable = enable;
                video_md.MoveCell.Cell[0] = '\0';
                video_md.MoveCell.Sensitive = 8;
                video_md.Week[0].Flag = 1;
                video_md.Week[0].TimePeriod[0].Start.Hour = 0;
                video_md.Week[0].TimePeriod[0].Start.Min = 0;
                video_md.Week[0].TimePeriod[0].Start.Sec = 0;
                video_md.Week[0].TimePeriod[0].End.Hour = 23;
                video_md.Week[0].TimePeriod[0].End.Min = 59;
                video_md.Week[0].TimePeriod[0].End.Sec = 59;
                video_md.Week[0].__pri_TimePeriodCount = 1;
                ret = ipcam_set_device_config(IPCNET_MOVE_ALARM_SET_REQ, &video_md);
                if(ret != 0)
                {
                    printf("ipcam_set_device_config FAIL ret:%d\n", ret);
                }
            }
            else
            {
                printf("ipcam_get_device_config FAIL ret:%d\n", ret);
            }
        }
        break;

        case 14:
        {   
            //获取当前设备信息
            IPCNetCamInfo_st DeviceInfo;
            ipcam_get_device_config(IPCNET_GET_SYS_INFO,(void *)&DeviceInfo);
            printf("DevName:%s,FirmVer:%s,Model:%s,SerialNo:%s,SysVer:%s,Uuid:%s,Vendor:%s",
                DeviceInfo.DevName,DeviceInfo.FirmVer,DeviceInfo.Model,DeviceInfo.SerialNo,DeviceInfo.SysVer,DeviceInfo.Uuid,DeviceInfo.Vendor);
        }
        break;
        
        case 15:
        {   
            //设置设备名
            IPCNetDevName_st DevName;
            strcpy(DevName.DevName,"fb_ipc");
            printf("set dev name:%s",DevName.DevName);
            ipcam_set_device_config(IPCNET_SET_DEV_INFO_REQ,(void *)&DevName);
        }
        break;

        case 16:
        {   
            //得到当前OSD名
            IPCNetGetOsdCfg_st getOsdCfg;
            IPCNetOsdCfg_st osdCfg;
            getOsdCfg.Vich = 0;
            ipcam_device_config(IPCNET_GET_OSD_REQ, &getOsdCfg, &osdCfg);
            printf("IPCNET_GET_OSD_REQ name:%s", osdCfg.OsdNameInfo.NameText);
        }
        break;

        case 18:
        {   
            //当搜索到wifi信息时会调用回调wifi_ap_list_callback(在回调处理信息)
            char *user_data_test = "search wifi ap test";
            ipcam_wifi_ap_search(wifi_ap_list_callback, user_data_test, strlen(user_data_test)+1);
        }
        break;

        case 19:
        {   
            //设备自动录像设置
            IPCNetAvRecordInfoReq_st req;
            IPCNetAvRecordInfoResp_st resp;
            
            int start_date, end_date;
            int start_time, end_time;

            printf("input start date (format: yyyymmdd):\n");
            input_str[0] = '\0';
            gets(input_str);
            printf("\nyour input is:%s\n", input_str);
            start_date = atoi(input_str);

            printf("input end date (format: yyyymmdd):\n");
            input_str[0] = '\0';
            gets(input_str);
            printf("\nyour input is:%s\n", input_str);
            end_date = atoi(input_str);
            
            printf("input start time (format: hhmmss):\n");
            input_str[0] = '\0';
            gets(input_str);
            printf("\nyour input is:%s\n", input_str);
            start_time = atoi(input_str);
            
            printf("input end time (format: hhmmss):\n");
            input_str[0] = '\0';
            gets(input_str);
            printf("\nyour input is:%s\n", input_str);
            end_time = atoi(input_str);

            memset(&req, 0, sizeof(IPCNetAvRecordInfoReq_st));
            req.si = 0;
            req.m = 0;
            req.ds = start_date;
            req.de = end_date;
            req.st = start_time;
            req.e = end_time;
            ret = ipcam_device_config(IPCNET_AV_RECO_LIST_GET_REQ, &req, &resp);
            if(ret == 0)
            {
                printf("total:%d free:%d record num:%d\n",
                    resp.t, resp.u, resp.n);
            }
        }
        break;
                
        case 22:
        {
            IPCNetVideoIFrame_st ipcnet_iframe;
            ipcnet_iframe.ViCh = 0;
            ipcnet_iframe.VenCh = 0;
        	ipcam_set_device_config(IPCNET_VIDEO_IFRAME_REQ, &ipcnet_iframe);
        }
        break;

        case 23:
        {
            IPCNetVideoQuality_st videoQuality;
            static int level = 0;

            level++;
            if(level > 10)
            {
                level = 0;
            }
            
            videoQuality.ViCh = 0;
            videoQuality.VenCh = 0;
            videoQuality.Level = level;
            printf("IPCNET_SET_VIDEO_QULITY_LEVEL_REQ level:%d\n", level);
            ipcam_set_device_config(IPCNET_SET_VIDEO_QULITY_LEVEL_REQ, &videoQuality);
        }
        break;

        case 24:
        {     
            //从设备发送声音过来可听声音变化
            printf("set audio enc params\n");
             IpcnetAudioEncodeCfg_st AudioCfg;
             ret = ipcam_get_device_config(IPCNET_AUDIO_ENC_GET_REQ,(void *)&AudioCfg);
             printf("__pri_AudioEncdeCount=%d,EncCh=%d,AiCh=%d,Vol=%d,MaxVol=%d,MinVol=%d,Gain=%d,MaxGain=%d,MinGain=%d\n",
                 AudioCfg.__pri_AudioEncdeCount,AudioCfg.AudioEncde[0].EncCh,AudioCfg.AiCh,AudioCfg.AudioEncodeNum,
                 AudioCfg.AudioEncde[0].Vol,AudioCfg.AudioEncde[0].MaxVol,AudioCfg.AudioEncde[0].MinVol,
                 AudioCfg.AudioEncde[0].Gain,AudioCfg.AudioEncde[0].MaxGain,AudioCfg.AudioEncde[0].MinGain);

            printf("please set the Gian:\n");
            input_str[0] = '\0';
            gets(input_str);
            printf("\nyour input is:%d\n", atoi(input_str));
            
            AudioCfg.AudioEncde[0].Gain = atoi(input_str); //<C2><F3><BF>?<E7><D2><F4><C1><BF>
            ipcam_set_device_config(IPCNET_AUDIO_ENC_SET_REQ,(void *)&AudioCfg);
        }
        break;

        case 25:
        {   
            //传声音到设备喇叭(speaker)
            printf("set audio dec params\n");
            IpcnetAudioEncodeCfg_st AudioCfg;
            ipcam_get_device_config(IPCNET_AUDIO_ENC_GET_REQ,(void *)&AudioCfg);
            printf("__pri_AudioEncdeCount=%d,EncCh=%d,AiCh=%d,Vol=%d,MaxVol=%d,MinVol=%d,Gain=%d,MaxGain=%d,MinGain=%d\n",
                AudioCfg.__pri_AudioEncdeCount,AudioCfg.AudioEncde[0].EncCh,AudioCfg.AiCh,AudioCfg.AudioEncodeNum,AudioCfg.AudioEncde[0].DecVol,AudioCfg.AudioEncde[0].DecMaxVol,
                AudioCfg.AudioEncde[0].DecMinVol,AudioCfg.AudioEncde[0].Gain,AudioCfg.AudioEncde[0].DecMaxGain,
                AudioCfg.AudioEncde[0].DecMinGain);

           AudioCfg.AudioEncde[0].DecVol = 60; //喇叭音量
           ipcam_set_device_config(IPCNET_AUDIO_ENC_SET_REQ,(void *)&AudioCfg);
        }
        break;

        case 26:
        {   
            //获取FTP信息
            printf("get ftp params\n");
            IPCNetFtpCfg_st FtpCfg;
            ipcam_get_device_config(IPCNET_GET_FTP_CFG_REQ,&FtpCfg);
            printf("get FtpCfg FtpAddr=%s,FtpUser=%s,FtpPasswd=%s,FtpPort=%d,FtpPath=%s\n",
                FtpCfg.FtpAddr,FtpCfg.FtpUser,FtpCfg.FtpPasswd,FtpCfg.FtpPort,FtpCfg.FtpPath);
        }
        break;

        case 27:
        {   
            //设置FTP信息
            printf("set ftp params\n");
            IPCNetFtpCfg_st FtpCfg;
            memset(&FtpCfg,0,sizeof(FtpCfg));
            strcpy(FtpCfg.FtpAddr,"192.168.1.20");
            FtpCfg.FtpPort = 21;
            strcpy(FtpCfg.FtpUser,"lin");
            strcpy(FtpCfg.FtpPasswd,"linhl");
            strcpy(FtpCfg.FtpPath,"test");
            
            printf("set FtpCfg FtpAddr=%s,FtpUser=%s,FtpPasswd=%s,FtpPort=%d,FtpPath=%s\n",
                FtpCfg.FtpAddr,FtpCfg.FtpUser,FtpCfg.FtpPasswd,FtpCfg.FtpPort,FtpCfg.FtpPath);
            ipcam_set_device_config(IPCNET_SET_FTP_CFG_REQ,&FtpCfg);
        }
        break;

        case 28:
        {   
            //获取当前升级信息
            printf("get upgrade info:\n");
            IPCNetUpgradeCfg_st LcUpgradeInfo;
            memset((void *)&LcUpgradeInfo,0,sizeof(LcUpgradeInfo));
            ipcam_get_device_config(IPCNET_GET_UPGRADE_URL_REQ,&LcUpgradeInfo);
            printf("LocalUrl:%s,UpgradeUrl:%s,SystemType:%s,CustomType:%s,VendorType:%s\n",
                LcUpgradeInfo.LocalUrl,LcUpgradeInfo.UpgradeUrl,LcUpgradeInfo.SystemType,
                LcUpgradeInfo.CustomType,LcUpgradeInfo.VendorType);
        }
        break;

        case 29:
        {
            printf("start upgrade:IPCNET_UPGRADE_REQ\n");
            IPCNetUpgradeInfo_st LcUpdateCfg;
            memset(&LcUpdateCfg, 0 ,sizeof(IPCNetUpgradeInfo_st));
            
            ret = ipcam_get_device_config(IPCNET_GET_UPGRADE_URL_REQ, &LcUpdateCfg);
        }
        break;

        case 30:
        {
            printf("set time info:\n");
            IPCNetGetTimeReq_st SdkGetTimeReq;
            IPCNetTimeCfg_st SdkTime;
            IPCNetTimeCfg_st SdkSetTime;
            SdkGetTimeReq.ReqTimeType = 1;
            ipcam_device_config(IPCNET_GET_TIME_REQ,(void *)&SdkGetTimeReq,(void *)&SdkTime);
            memcpy((void *)&SdkSetTime,(void *)&SdkTime,sizeof(IPCNetTimeCfg_st));
            printf("get local time info:%d-%02d-%0d %02d:%02d:%02d\n",
                SdkTime.Date.Year,SdkTime.Date.Mon,SdkTime.Date.Day,SdkTime.Time.Hour,SdkTime.Time.Min,SdkTime.Time.Sec);

            usleep(200);
            SdkGetTimeReq.ReqTimeType = 0;
            ipcam_device_config(IPCNET_GET_TIME_REQ,(void *)&SdkGetTimeReq,(void *)&SdkTime);
            memcpy((void *)&SdkSetTime,(void *)&SdkTime,sizeof(IPCNetTimeCfg_st));
            printf("get gmt time info:%d-%02d-%0d %02d:%02d:%02d\n",
                SdkTime.Date.Year,SdkTime.Date.Mon,SdkTime.Date.Day,SdkTime.Time.Hour,SdkTime.Time.Min,SdkTime.Time.Sec);
            
            SdkSetTime.NtpEnable = 0;
            //SdkSetTime.Time.Hour += 3;
            //SdkSetTime.Time.Min += 3;
            printf("set time info:%d-%02d-%0d %02d:%02d:%02d\n",
                SdkSetTime.Date.Year,SdkSetTime.Date.Mon,SdkSetTime.Date.Day,SdkSetTime.Time.Hour,SdkSetTime.Time.Min,SdkSetTime.Time.Sec);
            ipcam_set_device_config(IPCNET_SET_TIME_REQ,&SdkSetTime);
        }
        break;

        case 31:
        {   
            //获取设备当前WIFI信息
            IPCNetWirelessConfig_st wifi_info;
            ipcam_get_device_config(IPCNET_NETWORK_WIFI_GET_REQ, &wifi_info);
            printf("wifi_info WirelessEnable:%d ssid:%s passwd:%s enc type:%s status:%d\n",
                wifi_info.WirelessEnable,
                wifi_info.SSID, wifi_info.Password, wifi_info.EncType,
                wifi_info.WirelessStatus);
        }
        break;

        case 32:
        {   
            //配置设备WIFI
            IPCNetWirelessConfig_st wifi_info;
            char getstr[64];
            
            ipcam_get_device_config(IPCNET_NETWORK_WIFI_GET_REQ, &wifi_info);

            wifi_info.WirelessEnable = 1;
            printf("input the wifi ap:\n");
            gets(getstr);
            strncpy(wifi_info.SSID, getstr, sizeof(wifi_info.SSID));
            printf("your input is:%s\n", wifi_info.SSID);
            
            printf("input the passwd:\n");
            gets(getstr);
            strncpy(wifi_info.Password, getstr, sizeof(wifi_info.Password));
            printf("Password is:%s\n", wifi_info.Password);
            
            printf("input the enc type:\n");
            gets(getstr);
            strncpy(wifi_info.EncType, getstr, sizeof(wifi_info.EncType));
            printf("Password is:%s\n", wifi_info.EncType);
            
            printf("your setting wifi_info - ssid:%s passwd:%s enc type:%s\n",
                wifi_info.SSID, wifi_info.Password, wifi_info.EncType);
            
            ipcam_set_device_config(IPCNET_NETWORK_WIFI_SET_REQ, &wifi_info);
        }
        break;

        case 33:
        {   
            //获取图像翻转状态
            ret = ipcam_get_device_config(IPCNET_GET_OVERTURN_REQ, (void *)&video_overturn); 
            if(video_overturn.Mirror)
            {
                printf("Mirror turn\n");
            }
            else
            {
                printf("Mirror false\n");
            }
            if(video_overturn.Flip)
            {
                printf("Flip turn\n");
            }
            else
            {
                printf("Flip false\n");
            }
        }
        break;

        case 34:
        {   
            //设置图像翻转
            printf("Mirror is turn ,Flip is turn\n");
            
            video_overturn.Mirror = 1;
            video_overturn.Flip = 1;
            
            ret = ipcam_set_device_config(IPCNET_SET_OVERTURN_REQ, (void *)&video_overturn); 
        }
        break;

        case 35:
        {
            //获取预置点信息
            IPCNETPrePointList_st point;
            memset(&point, 0, sizeof(IPCNETPrePointList_st));
            
            ret = ipcam_get_device_config(IPCNET_GET_PREPOINT_REQ, &point);
            if(point.PointsCount)
            {
                printf("the point num=%d\n", point.PointsCount);
            }
            else
            {
                printf("please add the point before you get point num\n");
            }
        }
        break;

        case 36:
        {
            //增加预置点
            IPCNETPointInfo_st add_point;
            memset(&add_point, 0, sizeof(IPCNETPointInfo_st));

            char str[32] = "";

            printf("please input the point name:\n");
            gets(str);

            add_point.BitID = -1;
            memcpy(add_point.Desc, str, sizeof(add_point.Desc));

            ret = ipcam_set_device_config(IPCNET_SET_PREPOINT_REQ, (void *)&add_point);
        }
        break;
        
        case 37:
        {   
            //执行预置点
            int i;
            int type;
            int bitid;
            IPCNETPointOpr_st point_opr;
            IPCNETPrePointList_st point;
            memset(&point_opr, 0, sizeof(IPCNETPointOpr_st));

            ret = ipcam_get_device_config(IPCNET_GET_PREPOINT_REQ, (void *)&point);
            if(point.PointsCount)
            {
                point_opr.BitID = 0;
                point_opr.Type = 0;

                ret = ipcam_set_device_config(IPCNET_OPERATE_PREPOINT_REQ, (void *)&point_opr);
            }
            else
            {
                printf("no point \n");
            }
        }
        break;

        case 38:
        {
            //获取摄像机设置信息
            printf("get record config\n");
            //char path[260] = "\/mnt\/s0\/media\/sensor0";
            char path[260] = "/mnt/s0/media/sensor0";//按实际情况而定
            
            IPCNetRecordCfg_st pRecordCfg;
            IPCNetRecordGetCfg_st pRecordGetCfg;
            memset(&pRecordCfg, 0, sizeof(IPCNetRecordCfg_st));
            memset(&pRecordGetCfg, 0, sizeof(IPCNetRecordGetCfg_st));

            pRecordGetCfg.ViCh = 0;
            pRecordGetCfg.RecType = 0;
            memcpy(pRecordGetCfg.Path, path, sizeof(pRecordGetCfg.Path));
    
            ret = ipcam_device_config(IPCNET_AV_RECO_CONF_GET_REQ, (void *)&pRecordGetCfg, (void *)&pRecordCfg);

            printf("pRecordCfg->RecMins=%d,pRecordCfg->AutoDel=%d\n", pRecordCfg.RecMins, pRecordCfg.AutoDel);
        }
        break;

        case 39:
        {
            //设置摄像机配置信息
            printf("set record config\n");
            //char path[260] = "\/mnt\/s0\/media\/sensor0";//按实际情况而定
            char path[260] = "/mnt/s0/media/sensor0";//按实际情况而定
            
            IPCNetRecordCfg_st pRecordCfg;
            IPCNetRecordGetCfg_st pRecordGetCfg;
            memset(&pRecordCfg, 0, sizeof(IPCNetRecordCfg_st));
            memset(&pRecordGetCfg, 0, sizeof(IPCNetRecordGetCfg_st));

            pRecordGetCfg.ViCh = 0;
            pRecordGetCfg.RecType = 0;
            memcpy(pRecordGetCfg.Path, path, sizeof(pRecordGetCfg.Path));
    
            ret = ipcam_device_config(IPCNET_AV_RECO_CONF_GET_REQ, (void *)&pRecordGetCfg, (void *)&pRecordCfg);

            if(pRecordCfg.AutoDel)
            {
                pRecordCfg.AutoDel = 0;//(close auto delete )
            }
            else
            {
                pRecordCfg.AutoDel = 1;//(open auto delete)
            }
            ret = ipcam_set_device_config(IPCNET_AV_RECO_CONF_SET_REQ, (void *)&pRecordCfg);

            ret = ipcam_device_config(IPCNET_AV_RECO_CONF_GET_REQ, (void *)&pRecordGetCfg, (void *)&pRecordCfg);
            printf("pRecordCfg->AutoDel=%d\n", pRecordCfg.AutoDel);
        }
        break;

        case 40:
        {   
            //设备重启(等待10秒左右)
            printf("Device reboot\n");
            ret = ipcam_set_device_config(IPCNET_SET_REBOOT_REQ, NULL);
        }
        break;

        case 41:
        {
            //回复出厂设置(等待10秒左右)
            printf(" restore factory settings reset to defaults \n");
            ret = ipcam_set_device_config(IPCNET_SET_DEFAULT_REQ, NULL);
        }
        break;

        case 42:
        {   
		//获取移动报警设置
		ret = ipcam_get_device_config(IPCNET_MOVE_ALARM_GET_REQ, &alarm_cfg);

		printf("alarm_cfg.MoveInfo.MdEnable=%d\n", alarm_cfg.MoveCell.MdEnable);
		printf("alarm_cfg.MoveInfo.Sensitive=%d\n", alarm_cfg.MoveCell.Sensitive);
		printf("alarm_cfg.MoveInfo.Columns=%d\n", alarm_cfg.MoveCell.Columns);
		printf("alarm_cfg.MoveInfo.Rows=%d\n", alarm_cfg.MoveCell.Rows);

              //移动侦测时间段
		Print_motion_time(alarm_cfg);	
        }
        break;

        case 43:
        {   
		if(alarm_cfg.MoveCell.MdEnable)
		{
			alarm_cfg.MoveCell.MdEnable = 0;//关闭移动侦测
		}
		else
		{
			alarm_cfg.MoveCell.MdEnable = 1;//开启移动侦测
			char str[4];
			int volum = 0;
			str[0] = '\0';

			printf("input senstive level:");
			gets(str);
       		volum = atoi(str);
			alarm_cfg.MoveCell.Sensitive = volum;//移动侦测灵敏度

			for(i=0; i<8; i++)//i=0 every day  i=1~7 since Sunday to saturday
			{
				alarm_cfg.Week[i].Flag = 1;
				for(j=0;j<2;j++)//两个时间段
				{     //demo中设置固定全天时间段侦测
					alarm_cfg.Week[i].TimePeriod[j].Start.Hour = 0;
					alarm_cfg.Week[i].TimePeriod[j].Start.Min = 0;
					alarm_cfg.Week[i].TimePeriod[j].Start.Sec =0;
					alarm_cfg.Week[i].TimePeriod[j].End.Hour = 23;
					alarm_cfg.Week[i].TimePeriod[j].End.Min = 59;
					alarm_cfg.Week[i].TimePeriod[j].End.Sec = 59;
				}
			}
		}

		alarm_cfg.MoveCell.Cell[0] = '\0';

		ret = ipcam_set_device_config(IPCNET_MOVE_ALARM_SET_REQ, &alarm_cfg);
        }
        break;

		case 44:
		{
			printf("<--------------------get alarm report---------------------------->\n");
			ipcam_regist_alarm_event_callback(alarm_report_callback);
		}
		break;

		case 45:
		{
			char *usr_date = "storage format";
			ipcam_storage_format(storage_format_callback, usr_date, strlen(usr_date) + 1);
		}
		break;

	    case 46:
	    {
	        printf("snap request\n");
	        ret = ipcam_snap_shoot(req_snap_callback, NULL, 0);
	        printf("ipcam_snap_shoot ret=%d",ret);
	    }
	    break;

		//人脸识别接口
	    case 47:
	    {
	    	//获取已经设置的用户
	    	int i = 0;
	    	IPCNetFrGetUserInfo_t FrUserInfo;
	    	IPCNetFrUserInfo_st *pUserInfo = NULL;
	    	memset((void *)&FrUserInfo, 0, sizeof(FrUserInfo));
			printf("IPCNET_FR_GET_USER_REQ\n");
			ipcam_get_device_config(IPCNET_FR_GET_USER_REQ, &FrUserInfo);
			printf("FrUserListCount=%d\n",FrUserInfo.__pri_FrUserListCount);
			for(i=0; i<FrUserInfo.__pri_FrUserListCount; i++)
			{
				pUserInfo = &FrUserInfo.FrUserList[i];
				printf("i=%d,Name=%s,UserID=%d\n",i,pUserInfo->Name,pUserInfo->UserID);
			}
	    }
	    break;

	    case 48:
	    {
	    	//采集人脸信息
			printf("IPCNET_FR_COLLECTION_REQ\n");
			IPCNetFrCollectionReq_st FrCollect;
			FrCollect.Angle = IPCNET_FR_FRONT_FACE_E;
			//采集到的人脸信息，会在回调中返回
			ipcam_set_device_config(IPCNET_FR_COLLECTION_REQ, &FrCollect);
	    }
	    break;

		case 49:
	    {
	    	//根据采集人脸设置用户信息
			printf("IPCNET_FR_ADD_USER_BY_COLLECT_REQ\n");
			IPCNetFrAddCollectUser_st CollectUserInfo;
			memset((void *)&CollectUserInfo, 0, sizeof(IPCNetFrAddCollectUser_st));
			CollectUserInfo.FaceId = 1;
			CollectUserInfo.ImgNum = 4;
			strcpy(CollectUserInfo.Name,"xiaoming");
			ipcam_set_device_config(IPCNET_FR_ADD_USER_BY_COLLECT_REQ, &CollectUserInfo);
	    }
	    break;

	    case 50:
	    {
	    	//根据采集人脸设置用户信息
			printf("IPCNET_FR_ADD_USER_BY_COLLECT_REQ\n");
			IPCNetFrImgName_st FrUserInfo;
			memset((void *)&FrUserInfo, 0, sizeof(FrUserInfo));
			strcpy(FrUserInfo.ImgName, "/tmp/user_face.jeg");
			ipcam_set_device_config(IPCNET_FR_ADD_USER_BY_VISIT_HISTORY_REQ, &FrUserInfo);
	    }
	    break;

		case 51:
	    {
	    	//修改用户信息
			printf("IPCNET_FR_MODIFY_USER_INFO_REQ\n");
			IPCNetFrModifyUserInfo_st ModifyUserInfo;
			ModifyUserInfo.UserID = 1;
			strcpy(ModifyUserInfo.Name, "xiaoming");
			strcpy(ModifyUserInfo.ImgName,"ChangPicName");
			ipcam_set_device_config(IPCNET_FR_MODIFY_USER_INFO_REQ, &ModifyUserInfo);
	    }
	    break;

	    case 52:
	    {
	    	printf("IPCNET_FR_DEL_USER_INFO_REQ\n");
	    	IPCNetDelUserInfo_st DelUserInfo;
	    	DelUserInfo.UserId = 1;
			ipcam_set_device_config(IPCNET_FR_DEL_USER_INFO_REQ, &DelUserInfo);
	    }
	    break;

	    case 53:
	    {
	    	printf("IPCNET_FR_GET_VISIT_HISTORY_REQ\n");
			IPCNETGetVisitHistoryReq_st GetVisitInfo;
			GetVisitInfo.GetNum = 10;
			//返回结果在回调里去接收
			ipcam_set_device_config(IPCNET_FR_GET_VISIT_HISTORY_REQ, &GetVisitInfo);
	    }
	    break;

	    case 54:
	    {
	    	printf("IPCNET_FR_FUNC_ENABLE_REQ");
	    	IPCNETNetFrFuncEnabe_st FrFuncInfo;
	    	FrFuncInfo.Enable = TRUE;
	    	ipcam_set_device_config(IPCNET_FR_FUNC_ENABLE_REQ, &FrFuncInfo);
	    }
	    break;

	    case 55:	//结果会在回调中返回
	    {
			printf("IPCNET_FR_VISITOR_PER_MIN_REQ");
			ipcam_get_device_config(IPCNET_FR_VISITOR_PER_MIN_REQ, NULL);
	    }
	    break;
			
        //---------------station------------------------------------
#if 0
        case 100:
        {
            IPCNetStaProfiles_st profiles;
            int i;
            ipcam_get_device_config(IPCNET_GET_STATION_PROFILES_REQ, &profiles);
            printf("StaType:%s __pri_DevStatusCount:%d\n", profiles.StaType, profiles.__pri_DevStatusCount);
            for(i = 0; i < profiles.__pri_DevStatusCount; i++)
            {
                printf("DevType:%s UId:%s Status:%s BatteryCap:%d\n", 
                    profiles.DevStatus[i].DevType, profiles.DevStatus[i].UId,
                    profiles.DevStatus[i].Status, profiles.DevStatus[i].BatteryCap);
            }
        }
        break;
#endif        
        case 101:
        {
            IPCNetSta_OptDev_st opt;
            int i;

            printf("input the device uid:\n");
            gets(opt.UId);
            printf("your input is:%s\n", opt.UId);

            strcpy(opt.Opt, IPCNET_STATION_DEV_OPT_WAKEUP);
            ret = ipcam_set_device_config(IPCNET_SET_STATION_OPT_DEV_REQ, &opt);
            if(ret != IPCNET_RET_OK)
            {
                printf("IPCNET_SET_STATION_OPT_DEV_REQ ERR:%d\n", ret);
            }
        }
        break;


        /*case 102:
            ret = ipcam_set_device_config(IPCNET_START_RECORD_REQ, NULL);
        break;*/

        }
    }
    return 0;
}

static void alarm_report_test(IPCNetAlarmMsgReport_st *alarm_msg)
{
    printf("AlarmType:%d AlarmSta:%d date:%d-%d-%d %d:%d:%d\n", 
            alarm_msg->AlarmType, alarm_msg->AlarmSta,
            alarm_msg->AlarmDate.Year, alarm_msg->AlarmDate.Mon, alarm_msg->AlarmDate.Day,
            alarm_msg->AlarmTime.Hour, alarm_msg->AlarmTime.Min, alarm_msg->AlarmTime.Sec);
}


/*int utf2gbk(char *buf, size_t len)  
{  
    iconv_t cd = iconv_open("GBK", "UTF-8");  
    if (cd == (iconv_t)-1) {  
        perror("iconc_open failed\n");  
        return -1;  
    }  
    size_t sz = 32*8;  
    char *tmp_str = (char *)malloc(sz);  
    // 靠靠靠靠靠靠靠靠靠靠靠  
    size_t inlen = len;  
    size_t outlen = sz;  
    char *in = buf;  
    char *out = tmp_str;  
    if (tmp_str == NULL) {  
        iconv_close(cd);  
        fprintf(stderr, "malloc failed\n");  
        return -1;  
    }  
    memset(tmp_str, 0, sz);  
    if (iconv(cd, &in, &inlen, &out, &outlen) == (size_t)-1) {  
        iconv_close(cd);  
        return -1;  
    }  
    iconv_close(cd);  
    return 0;  
}  
*/
int save_fr_pic(char *path,char *pbuf,int len)
{
	int ret = 0;
	FILE *fp = fopen(path,"w+");
	if(!fp)
	{
		printf("open %s fail!\n",path);
		return -1;
	}

	ret = fwrite(pbuf, len, 1, fp);
	printf("ret=%d,len=%d\n",ret,len);

	fclose(fp);
	return 0;	
}

//pPicInfo:图片对应信息，pPicBuf:图片数据
static void face_recognize_callback(int type, void *pPicInfo, char *pPicBuf, int PicLen)
{
	static int CollectCnt = 0;
	static int VisitCnt = 0;
	int ret;
	char FileName[96] = {0};
	char OldFileName[96] = {0};
	char charout[32] = {0};
	char shell_cmd[128] = {0};

	printf("type=%d,pPicBuf=%p,PicLen=%d\n",type,pPicBuf,PicLen);

	if(IPCNET_FR_COLLECTION_RESP == type)	//人脸采集的图片
	{
		IPCNetFrCollectResp_st *pFrCollect = (IPCNetFrCollectResp_st *)pPicInfo;
		printf("Angle=%d,ImgPath=%s\n",pFrCollect->Angle,pFrCollect->ImgPath);
		sprintf(FileName,"%s/pic_collect_%d.jpeg",pic_path,CollectCnt++);
		save_fr_pic(FileName,pPicBuf,PicLen);
		sprintf(shell_cmd,"echo %s >> %s/collect.txt",FileName,pic_path);
                system(shell_cmd);
	}
	else if(IPCNET_FR_DETECT_PIC == type)	//检测到人脸信息，这个没有图片
	{
		IPCNetDetectFaceInfo_st *pDetectFaceInfo = (IPCNetDetectFaceInfo_st *)pPicInfo;
		printf("Name=%s,Cnt=%d\n",pDetectFaceInfo->Name,pDetectFaceInfo->Cnt);
	}
	else if(IPCNET_FR_GET_VISIT_HISTORY_RESP == type)	//返回历史访问者的人脸图片
	{
		IPCNetFrGetVisitHistoryResp_st *pVisitInfo = (IPCNetFrGetVisitHistoryResp_st *)pPicInfo;
		
		ret = Utf8ToUnicode(pVisitInfo->Name, charout);
		if( ret < 0 )
			printf("Utf8ToUnicode failed!\n");
		
		ret = UnicodeToGbk((unsigned short *)charout, pVisitInfo->Name , 32);
		if( ret < 0 )
                        printf("UnicodeToGbk failed!\n");
		if( ret > 0 )
			pVisitInfo->Name[ret]='\0';
							
		printf("Name=%s,ImgPath=%s,Sex=%s,Age=%d,Cnt=%d\n",
			pVisitInfo->Name,pVisitInfo->ImgPath,pVisitInfo->Sex,pVisitInfo->Age,pVisitInfo->Cnt);
		sprintf(FileName,"%s/%d-%d-%d_%d-%d-%d_%s_%d.jpeg",pic_path,pVisitInfo->Date.Year,\
					pVisitInfo->Date.Mon,pVisitInfo->Date.Day,\
					pVisitInfo->Time.Hour,pVisitInfo->Time.Min,\
					pVisitInfo->Time.Sec,pVisitInfo->Name,\
					pVisitInfo->Cnt);
		if(!strcmp(OldFileName,FileName))
		{
			sprintf(FileName,"%s/%d-%d-%d_%d-%d-%d_%s_%d.jpeg",pic_path,pVisitInfo->Date.Year,\
                                        pVisitInfo->Date.Mon,pVisitInfo->Date.Day,\
                                        pVisitInfo->Time.Hour,pVisitInfo->Time.Min,\
                                        pVisitInfo->Time.Sec,pVisitInfo->Name,\
                                        pVisitInfo->Cnt+(VisitCnt++));
		}
		else
		{
			VisitCnt=0;
		}
		if(save_fr_pic(FileName,pPicBuf,PicLen))
		{
			if(save_fr_pic(FileName,pPicBuf,PicLen))
				return;
		}
		sprintf(shell_cmd,"echo %s >> %s/tftp_list.txt",FileName,pic_path);
		system(shell_cmd);
		strcpy(OldFileName,FileName);
	}
	else if(IPCNET_FR_VISITOR_PER_MIN_RESP == type)	//这个只返回数据，没有图片
	{
		int VisitorPerMin = *((int *)pPicInfo);
		printf("IPCNET_FR_VISITOR_PER_MIN_RESP VisitorPerMin=%d\n",VisitorPerMin);
		ipcam_get_device_config(IPCNET_GET_TIME_REQ, &time_cfg);
            	sprintf(shell_cmd,"echo \"%d-%d-%d %d:%d	VisitorPerMin=%d\" >> %s/passenger_flow.txt",\
		time_cfg.Date.Year, time_cfg.Date.Mon, time_cfg.Date.Day,\
		time_cfg.Time.Hour, time_cfg.Time.Min,VisitorPerMin,pic_path);
		system(shell_cmd);
		sprintf(shell_cmd,"echo \"update\" >> %s/passenger_flow.txt",pic_path);
		system(shell_cmd);
	}
	return;
}

int main( int argc, char *argv[] )
{
    int test_aframe = 0;
    int test_vframe = 0;
    int test_record = 0;
    int test_face_recognize = 0;
    int ret;
    int opt = -1;
    int i;

    while( (opt = getopt( argc, argv, "s:avrf:" )) != -1 )
    {
        printf("opt:%c\n",opt);
        switch( opt )
        {
            case 's':
                printf("optarg:%s\n", optarg);
                strcpy(save_file_path, optarg);
                printf("save file: %s\n", save_file_path);
                break;
                
            case 'a':
                test_aframe = 1;
                break;

            case 'r':
                test_record = 1;
                break;
                
            case 'v':
                test_vframe = 1;
                break;
            case 'f':
            	test_face_recognize = 1;
		pic_path = optarg;
            	break;
            	
        }
    }
    
    ret = sepcam_ipc_init("sepcam_ip_demo");
    if(ret != 0)
    {
        printf("sepcam_ipc_init FAIL\n");
        return -1;
    }

    ret = sepcam_stream_init();
    if(ret != 0)
    {
        printf("sepcam_stream_init FAIL\n");
        return -1;
    }
    
    IPCNetCamInfo_st cam_info;
    ipcam_get_device_config(IPCNET_GET_SYS_INFO, &cam_info);
    printf("IPCNET_GET_SYS_INFO sdk version:%s\n", cam_info.IpcSdk);
    
    ipcam_regist_alarm_event_callback(alarm_report_test);


    IPCNetCamColorCfg_st cam_ipc;
    //ret = ipcam_get_device_config(IPCNET_GET_CAM_PIC_CFG_REQ, &cam_ipc);
    //printf("IPCNET_GET_CAM_PIC_CFG_REQ ViCh:%d Brightness:%d\n", video_cfg.ViCh, cam_ipc.Brightness);

    if(test_record)
    {
        record_test();
        exit(0);
    }
    
    if(test_vframe)
    {
        int vench = 0;
        printf(">>>>>>>>>>>>>>>>>video test\n");
#ifdef GET_VFRAME_TEST
        //get_vframe_test();
#else
        sepcam_stream_init();
        create_read_steam(vench);

        while(1)
        {
            printf("enter any key to request iframe\n");
            getchar();
            
            IPCNetVideoIFrame_st ipcnet_iframe;
            ipcnet_iframe.ViCh = 0;
            ipcnet_iframe.VenCh = vench;
        	ipcam_set_device_config(IPCNET_VIDEO_IFRAME_REQ, &ipcnet_iframe);
        }
        exit(0);
#endif
    }
    else if(test_aframe)
    {
        printf(">>>>>>>>>>>>>>>>>audio test\n");
        while(1)
        {
            read_audio_frame_test();
        }
    }
    else if(test_face_recognize)
    {
		//注册人脸识别的回调处理函数
    	sepcam_fr_init(face_recognize_callback);
    }
    else
    {
    	sepcam_fr_init(face_recognize_callback);
        param_test();
    }
    
    while(1)
    {
        sleep(10);
    }
    return 0;
}

