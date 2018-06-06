#ifndef _IPCAM_SDK_STREAM_INFO_
#define _IPCAM_SDK_STREAM_INFO_

/** 读的buf太小或写的数据太大*/
#define SHM_RDWR_BUF_SIZE_ERR (-1)

/** 读写传入的参数有问题*/
#define SHM_RDWR_PARAM_ERR (-2)

/** 读写内存没有找到*/
#define SHM_MEM_NAME_ERR (-3)

/** 读写共享内存可创建数用完*/
#define SHM_MEM_MGR_ERR_INSUFF (-4)


typedef enum
{
    PAYLOAD_PCMU          = 0,
    PAYLOAD_1016          = 1,
    PAYLOAD_G721          = 2,
    PAYLOAD_GSM           = 3,
    PAYLOAD_G723          = 4,
    PAYLOAD_DVI4_8K       = 5,
    PAYLOAD_DVI4_16K      = 6,
    PAYLOAD_LPC           = 7,
    PAYLOAD_PCMA          = 8,
    PAYLOAD_G722          = 9,
    PAYLOAD_S16BE_STEREO  = 10,
    PAYLOAD_S16BE_MONO    = 11,
    PAYLOAD_QCELP         = 12,
    PAYLOAD_CN            = 13,
    PAYLOAD_MPEGAUDIO     = 14,
    PAYLOAD_G728          = 15,
    PAYLOAD_DVI4_3        = 16,
    PAYLOAD_DVI4_4        = 17,
    PAYLOAD_G729          = 18,
    PAYLOAD_G711A         = 19,
    PAYLOAD_G711U         = 20,
    PAYLOAD_G726          = 21,
    PAYLOAD_G729A         = 22,
    PAYLOAD_LPCM          = 23,
    PAYLOAD_CelB          = 25,
    PAYLOAD_JPEG          = 26,
    PAYLOAD_CUSM          = 27,
    PAYLOAD_NV            = 28,
    PAYLOAD_PICW          = 29,
    PAYLOAD_CPV           = 30,
    PAYLOAD_H261          = 31,
    PAYLOAD_MPEGVIDEO     = 32,
    PAYLOAD_MPEG2TS       = 33,
    PAYLOAD_H263          = 34,
    PAYLOAD_SPEG          = 35,
    PAYLOAD_MPEG2VIDEO    = 36,
    PAYLOAD_AAC           = 37,
    PAYLOAD_WMA9STD       = 38,
    PAYLOAD_HEAAC         = 39,
    PAYLOAD_PCM_VOICE     = 40,
    PAYLOAD_PCM_AUDIO     = 41,
    PAYLOAD_AACLC         = 42,
    PAYLOAD_MP3           = 43,
    PAYLOAD_ADPCMA        = 49,
    PAYLOAD_AEC           = 50,
    PAYLOAD_X_LD          = 95,
    PAYLOAD_H264          = 96,
    PAYLOAD_D_GSM_HR      = 200,
    PAYLOAD_D_GSM_EFR     = 201,
    PAYLOAD_D_L8          = 202,
    PAYLOAD_D_RED         = 203,
    PAYLOAD_D_VDVI        = 204,
    PAYLOAD_D_BT656       = 220,
    PAYLOAD_D_H263_1998   = 221,
    PAYLOAD_D_MP1S        = 222,
    PAYLOAD_D_MP2P        = 223,
    PAYLOAD_D_BMPEG       = 224,
    PAYLOAD_MP4VIDEO      = 230,
    PAYLOAD_MP4AUDIO      = 237,
    PAYLOAD_VC1           = 238,
    PAYLOAD_JVC_ASF       = 255,
    PAYLOAD_D_AVI         = 256,
    PAYLOAD_DIVX3		 = 257,
    PAYLOAD_AVS			 = 258,
    PAYLOAD_REAL8		 = 259,
    PAYLOAD_REAL9		 = 260,
    PAYLOAD_VP6			 = 261,
    PAYLOAD_VP6F			 = 262,
    PAYLOAD_VP6A			 = 263,
    PAYLOAD_SORENSON	 	 = 264,
    PAYLOAD_H265          = 265,
    PAYLOAD_MAX           = 266,
    /* add by hisilicon */
    PAYLOAD_AMR           = 1001,
    PAYLOAD_MJPEG         = 1002,
    PAYLOAD_AMRWB         = 1003,
    PAYLOAD_BUTT
}ComPayLoad_e;

#define MAX_NAL_NUM     8
struct tagNALinfo
{
    int nal_num;        //一帧含有nal 的个数
    int nal_len[MAX_NAL_NUM];  //每个nal的长度
};
typedef struct tagNALinfo nal_info_t;

typedef enum
{
    ENC_H264=0,
    ENC_MJPEG,
    ENC_H265,
    ENC_VIDEO_BUTT, // 3
    
    ENC_JPEG,
    ENC_FD_JPG, //5-人脸
    ENC_FD_REG_JPG,
    ENC_MUL_JPG,    //多张抓拍

    ENC_ALARM_JPG,
    ENC_PICTURE_BUTT,
    
    ENC_G726,
    ENC_PCM_ALAW,
    ENC_PCM_ULAW,
    ENC_AUDIO_BUTT,
}AVENC_FORMAT_E;


static INLINE_t BOOL enc_is_video(int enc_type)
{
    return (enc_type < ENC_VIDEO_BUTT);
}

static INLINE_t BOOL enc_is_audio(int enc_type)
{
    return (enc_type > ENC_PICTURE_BUTT && enc_type < ENC_AUDIO_BUTT);
}

static INLINE_t BOOL enc_is_picture(int enc_type)
{
    return (enc_type > ENC_VIDEO_BUTT && enc_type < ENC_PICTURE_BUTT);
}

static INLINE_t BOOL frame_is_key(int frame_type)
{
    return(frame_type >4);
}

#define FRAME_TYPE_IS_VIDEO(frame_type) \
        ((((frame_type) >> 8)&0xff) < ENC_VIDEO_BUTT)

#define FRAME_TYPE_IS_AUDIO(frame_type) \
        ((((frame_type) >> 8)&0xff) > ENC_PICTURE_BUTT && (((frame_type) >> 8)&0xff) < ENC_AUDIO_BUTT)

#define FRAME_TYPE_IS_SNAP(frame_type) \
        ((((frame_type) >> 8)&0xff) > ENC_VIDEO_BUTT && (((frame_type) >> 8)&0xff) < ENC_PICTURE_BUTT)

#define FRAME_TYPE_IS_V_KEY(frame_type) \
        (FRAME_TYPE_IS_VIDEO(frame_type) && (((frame_type)&0Xff) > 4))


typedef struct
{
    uint16_t vench;
    uint16_t payload;
    uint16_t frame_type; //帧类型
    uint16_t frame_no;  //帧序号
    uint32_t frame_size; //帧长
    uint32_t flag:1; //目前录像回放用到这个flag，flag=1为录像文件全部回放完了
    uint32_t reserve:31;
    uint64_t timestamp; //精确到微秒级时间戳
}STREAM_FRAME_INFO_t;

typedef struct
{
    uint8_t *addr;
    uint32_t pkt_size;
}STREAM_FRAME_PACK_t;

typedef struct
{
    uint8_t pkt_num;
    uint8_t reserve[3];
    STREAM_FRAME_PACK_t *pkt;
}STREAM_FRAME_DATA_t;



//-----------------以下结构体用于获取YUV\RGB帧数据-----------------------------------------------------------
typedef struct
{
    uint32_t enFlashType;
    uint32_t   u32JpegDcfPhyAddr;
    void* pJpegDcfVirAddr;
} VIDEO_SUPPLEMENT_t;

typedef struct
{
    uint32_t u32Width;
    uint32_t u32Height;
    uint32_t u32Field;
    uint32_t enPixelFormat;

    uint32_t enVideoFormat;
    uint32_t enCompressMode;

    uint32_t u32PhyAddr[3];
    void*   pVirAddr[3];
    uint32_t u32Stride[3];

    uint32_t u32HeaderPhyAddr[3];
    void*   pHeaderVirAddr[3];
    uint32_t u32HeaderStride[3];

    int16_t s16OffsetTop;		/* top offset of show area */
    int16_t s16OffsetBottom;	/* bottom offset of show area */
    int16_t s16OffsetLeft;		/* left offset of show area */
    int16_t s16OffsetRight;		/* right offset of show area */

    uint64_t u64pts;
    uint32_t u32TimeRef;

    uint32_t u32PrivateData;
    VIDEO_SUPPLEMENT_t stSupplement;
} VI_FRAME_t;

typedef struct
{
    VI_FRAME_t stVFrame;
    uint32_t u32PoolId;
} VI_FRAME_INFO_t;

typedef struct
{
    uint32_t phy_addr;
    uint32_t stride;
    uint32_t reserve[3];
    uint32_t data_len;
}VFRAME_ADDR_t;

typedef struct
{
    uint32_t width;
    uint32_t height;
    uint32_t pixel_format; // 0 - YUV420sp, 1-rgb_u3c3
    uint64_t pts;
    uint32_t ref;
    VFRAME_ADDR_t addr[6];
    char reserve[64];
}VFRAME_t;

typedef struct
{
    uint32_t frame_id;
    VFRAME_t frame_info;
}VFRAME_INFO_t;

//-----------------以上结构体用于获取YUV\RGB帧数据-----------------------------------------------------------

#endif

