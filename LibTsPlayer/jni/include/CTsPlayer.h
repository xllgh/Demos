#ifndef _TSPLAYER_H_
#define _TSPLAYER_H_
#include <android/log.h>    
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <gui/Surface.h>

using namespace android;
extern "C" {
#include <amports/vformat.h>
#include <amports/aformat.h>
#include <amports/amstream.h>
#include <codec.h>
}

#include <string.h>
#include <utils/Timers.h>


#define lock_t          pthread_mutex_t
#define lp_lock_init(x,v)   pthread_mutex_init(x,v)
#define lp_lock(x)      pthread_mutex_lock(x)
#define lp_unlock(x)    pthread_mutex_unlock(x)



#define IN
#define OUT

typedef struct{
	void *(*init)(IN int flags);
  int (*read)(IN void *handle, IN unsigned char *buf, IN int size);
  int (*ready)(IN void *handle);
  int (*exit)(IN void *handle);
}wiptv_callback;


#define TRICKMODE_NONE       0x00
#define TRICKMODE_I          0x01
#define TRICKMODE_I_HEVC     0x07
#define TRICKMODE_FFFB       0x02
#define MAX_AUDIO_PARAM_SIZE 10
#define MAX_SUBTITLE_PARAM_SIZE 10

typedef struct{
	unsigned short	pid;//pid
	int				nVideoWidth;//��Ƶ���
	int				nVideoHeight;//��Ƶ�߶�
	int				nFrameRate;//֡��
	vformat_t		vFmt;//��Ƶ��ʽ
	unsigned long	cFmt;//�����ʽ
}VIDEO_PARA_T, *PVIDEO_PARA_T;
typedef struct{
	unsigned short	pid;//pid
	int				nChannels;//������
	int				nSampleRate;//������
	aformat_t		aFmt;//��Ƶ��ʽ
	int				nExtraSize;
	unsigned char*	pExtraData;	
}AUDIO_PARA_T, *PAUDIO_PARA_T;

#ifndef AVCODEC_AVCODEC_H
typedef enum {
	/* subtitle codecs */
    CODEC_ID_DVD_SUBTITLE= 0x17000,
    CODEC_ID_DVB_SUBTITLE,
    CODEC_ID_TEXT,  ///< raw UTF-8 text
    CODEC_ID_XSUB,
    CODEC_ID_SSA,
    CODEC_ID_MOV_TEXT,
    CODEC_ID_HDMV_PGS_SUBTITLE,
    CODEC_ID_DVB_TELETEXT,
    CODEC_ID_SRT,
    CODEC_ID_MICRODVD,
	}SUB_TYPE;
#endif
typedef struct{
	unsigned short	pid;//pid
	int sub_type; 
	
}SUBTITLE_PARA_T, *PSUBTITLE_PARA_T;

typedef struct ST_LPbuffer{
    unsigned char *rp;
    unsigned char *wp;
    unsigned char *buffer;
    unsigned char *bufferend;
    int valid_can_read;
    bool enlpflag;
}LPBUFFER_T;



typedef enum{    
	IPTV_PLAYER_EVT_STREAM_VALID=0,    
	IPTV_PLAYER_EVT_FIRST_PTS,     //first frame decoded event    
	IPTV_PLAYER_EVT_VOD_EOS,       //VOD EOS event    
	IPTV_PLAYER_EVT_ABEND,         //under flow event    
	IPTV_PLAYER_EVT_PLAYBACK_ERROR,// playback error event    
	IPTV_PLAYER_EVT_VID_FRAME_ERROR =0x200,// ��Ƶ�������    
	IPTV_PLAYER_EVT_VID_DISCARD_FRAME,// ��Ƶ���붪֡    
	IPTV_PLAYER_EVT_VID_DEC_UNDERFLOW,// ��Ƶ��������    
	IPTV_PLAYER_EVT_VID_PTS_ERROR,// ��Ƶ����Pts����    
	IPTV_PLAYER_EVT_AUD_FRAME_ERROR,// ��Ƶ�������    
	IPTV_PLAYER_EVT_AUD_DISCARD_FRAME,// ��Ƶ���붪��    
	IPTV_PLAYER_EVT_AUD_DEC_UNDERFLOW,//��Ƶ��������    
	IPTV_PLAYER_EVT_AUD_PTS_ERROR,// ��ƵPTS����    
	IPTV_PLAYER_EVT_BUTT    
}IPTV_PLAYER_EVT_e;

typedef enum{    
	IPTV_PLAYER_ATTR_VID_ASPECT=0,  /* ��Ƶ��߱� 0--640*480��1--720*576��2--1280*720��3--1920*1080,4--3840*2160,5--others�ȱ�ʶָ���ֱ���*/    
	IPTV_PLAYER_ATTR_VID_RATIO,     //��Ƶ��߱�, 0����4��3��1����16��9    
	IPTV_PLAYER_ATTR_VID_SAMPLETYPE,     //֡��ģʽ, 1��������Դ��0�������Դ    
	IPTV_PLAYER_ATTR_VIDAUDDIFF,     //����Ƶ����diff    
	IPTV_PLAYER_ATTR_VID_BUF_SIZE,     //��Ƶ��������С    
	IPTV_PLAYER_ATTR_VID_USED_SIZE,     //��Ƶ������ʹ�ô�С    
	IPTV_PLAYER_ATTR_AUD_BUF_SIZE,     //��Ƶ��������С    
	IPTV_PLAYER_ATTR_AUD_USED_SIZE,     //��Ƶ��������ʹ�ô�С    
	IPTV_PLAYER_ATTR_BUTT}
IPTV_ATTR_TYPE_e;


typedef void (*IPTV_PLAYER_EVT_CB)(IPTV_PLAYER_EVT_e evt, void *handler);

typedef struct {
    int abuf_size;
    int abuf_data_len;
    int abuf_free_len;
    int vbuf_size;
    int vbuf_data_len;
    int vbuf_free_len;
}AVBUF_STATUS, *PAVBUF_STATUS;

int enable_gl_2xscale(const char *);

int Active_osd_viewport(int , int );

class CTsPlayer;
class ITsPlayer{
public:
	ITsPlayer(){}
	virtual ~ITsPlayer(){}
public:
	virtual int  GetPlayMode()=0;
	//��ʾ����
	virtual int  SetVideoWindow(int x,int y,int width,int height)=0;
	//x��ʾ��Ƶ
	virtual int  VideoShow(void)=0;
	//������Ƶ
	virtual int  VideoHide(void)=0;
	//��ʼ����Ƶ����
	virtual void InitVideo(PVIDEO_PARA_T pVideoPara)=0;
	//��ʼ����Ƶ����
	virtual void InitAudio(PAUDIO_PARA_T pAudioPara)=0;
	//��ʼ����
	virtual bool StartPlay()=0;
	//��ts��д��
	virtual int WriteData(unsigned char* pBuffer, unsigned int nSize)=0;
	//��ͣ
	virtual bool Pause()=0;
	//��������
	virtual bool Resume()=0;
	//�������
	virtual bool Fast()=0;
	//ֹͣ�������
	virtual bool StopFast()=0;
	//ֹͣ
	virtual bool Stop()=0;
    //��λ
    virtual bool Seek()=0;
    //�趨����
	//�趨����
	virtual bool SetVolume(int volume)=0;
	//��ȡ����
	virtual int GetVolume()=0;
	//�趨��Ƶ��ʾ����
	virtual bool SetRatio(int nRatio)=0;
	//��ȡ��ǰ����
	virtual int GetAudioBalance()=0;
	//��������
	virtual bool SetAudioBalance(int nAudioBalance)=0;
	//��ȡ��Ƶ�ֱ���
	virtual void GetVideoPixels(int& width, int& height)=0;
	virtual bool IsSoftFit()=0;
	virtual void SetEPGSize(int w, int h)=0;
    virtual void SetSurface(Surface* pSurface)=0;
	
	//16λɫ����Ҫ����colorkey��͸����Ƶ��
     virtual void SwitchAudioTrack(int pid) = 0;
     virtual void SwitchSubtitle(int pid) = 0;
     virtual void SetProperty(int nType, int nSub, int nValue) = 0;
     virtual int64_t GetCurrentPlayTime() = 0;
     virtual void leaveChannel() = 0;
	virtual void playerback_register_evt_cb(IPTV_PLAYER_EVT_CB pfunc, void *hander) = 0;
	virtual int playerback_getStatusInfo(IPTV_ATTR_TYPE_e enAttrType, int *value)=0;
    virtual int GetRealTimeFrameRate() = 0;
    virtual int GetVideoFrameRate() = 0;
    virtual int GetVideoDropNumber() = 0;
	virtual int GetVideoTotalNumber() = 0;
	virtual void InitSubtitle(PSUBTITLE_PARA_T pSubtitlePara)=0;
	virtual bool SubtitleShowHide(bool bShow) = 0;
};

class CTsPlayer : public ITsPlayer
{
public:
	CTsPlayer();
#ifdef USE_OPTEEOS
	CTsPlayer(bool DRMMode);
	CTsPlayer(bool DRMMode, bool omx_player);
#else
	CTsPlayer(bool omx_player);
#endif
	virtual ~CTsPlayer();
public:
	//ȡ�ò���ģʽ
	virtual int  GetPlayMode();
	//��ʾ����
	virtual int  SetVideoWindow(int x,int y,int width,int height);
	//x��ʾ��Ƶ
	virtual int  VideoShow(void);
	//������Ƶ
	virtual int  VideoHide(void);
	//��ʼ����Ƶ����
	virtual void InitVideo(PVIDEO_PARA_T pVideoPara);
	//��ʼ����Ƶ����
	virtual void InitAudio(PAUDIO_PARA_T pAudioPara);
	//
	virtual void InitSubtitle(PSUBTITLE_PARA_T pSubtitlePara);
	//��ʼ����
	virtual bool StartPlay();
	//��ts��д��
	virtual int WriteData(unsigned char* pBuffer, unsigned int nSize);
	//��ͣ
	virtual bool Pause();
	//��������
	virtual bool Resume();
	//�������
	virtual bool Fast();
	//ֹͣ�������
	virtual bool StopFast();
	//ֹͣ
	virtual bool Stop();
	//��λ
    	virtual bool Seek();
        //�趨����
	//�趨����
	virtual bool SetVolume(int volume);
	//��ȡ����
	virtual int GetVolume();
	//�趨��Ƶ��ʾ����
	virtual bool SetRatio(int nRatio);
	//��ȡ��ǰ����
	virtual int GetAudioBalance();
	//��������
	virtual bool SetAudioBalance(int nAudioBalance);
	//��ȡ��Ƶ�ֱ���
	virtual void GetVideoPixels(int& width, int& height);
	virtual bool IsSoftFit();
	virtual void SetEPGSize(int w, int h);
    virtual void SetSurface(Surface* pSurface);

	//16λɫ����Ҫ����colorkey��͸����Ƶ��

    virtual void SwitchAudioTrack(int pid) ;

    virtual void SwitchSubtitle(int pid) ;
    
    virtual void SetProperty(int nType, int nSub, int nValue) ;
    
    virtual int64_t GetCurrentPlayTime() ;
    
    virtual void leaveChannel() ;
	virtual void playerback_register_evt_cb(IPTV_PLAYER_EVT_CB pfunc, void *hander);
	virtual int playerback_getStatusInfo(IPTV_ATTR_TYPE_e enAttrType, int *value);
    virtual int GetRealTimeFrameRate();
    virtual int GetVideoFrameRate();
	virtual bool SubtitleShowHide(bool bShow);
    virtual int GetVideoDropNumber();
    virtual void Report_video_paramters();
    //virtual void Report_Audio_paramters();
    virtual int GetVideoTotalNumber();
    virtual void readExtractor();
    virtual int updateCTCInfo();
    /*end add*/
    bool mIsOmxPlayer;
	
protected:
	int		m_bLeaveChannel;
	
private:
	AUDIO_PARA_T a_aPara[MAX_AUDIO_PARAM_SIZE];
	SUBTITLE_PARA_T sPara[MAX_SUBTITLE_PARAM_SIZE];
	VIDEO_PARA_T vPara;	
	int player_pid;
	codec_para_t codec;
	codec_para_t *pcodec;
	codec_para_t    *vcodec;
    codec_para_t    *acodec;
    codec_para_t    *scodec;
	bool		m_bIsPlay;
	int			m_nOsdBpp;
	int			m_nAudioBalance;
	int			m_nVolume;
	int	m_nEPGWidth;
	int 	m_nEPGHeight;
	bool	m_bFast;
	bool 	m_bSetEPGSize;
    bool    m_bWrFirstPkg;
	int	m_nMode;
    IPTV_PLAYER_EVT_CB pfunc_player_evt;
    void *player_evt_hander;
	unsigned int writecount ;
	int64_t  m_StartPlayTimePoint;
	bool    m_isSoftFit;
	FILE*	m_fp;
    lock_t mutex;
    pthread_t mThread;
	pthread_t readThread;
    virtual void checkAbend();
    virtual void checkBuffLevel();
    virtual void checkBuffstate();
    static void *threadCheckAbend(void *pthis);
	static void *threadReadPacket(void *pthis);
    bool    m_isBlackoutPolicy;
    bool m_bchangeH264to4k;
    lock_t mutex_lp;
    void checkVdecstate();
    bool        m_bIsPause;
    virtual bool  iStartPlay( );
    virtual bool  iStop( );
    int64_t m_PreviousOverflowTime;
	size_t mInputQueueSize;
};

#endif
