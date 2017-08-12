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
#include <surfaceflinger/Surface.h>
#include <surfaceflinger/ISurface.h>
#include <gui/ISurfaceTexture.h>
#include <gui/SurfaceTextureClient.h>
#include <surfaceflinger/ISurfaceComposer.h>
using namespace android;


extern "C" {
#include <amports/vformat.h>
#include <amports/aformat.h>
#include <codec.h>
}
#include <string.h>
#include <utils/Timers.h>




#define WF

#define TRICKMODE_NONE       0x00
#define TRICKMODE_I          0x01
#define TRICKMODE_FFFB       0x02
#define MAX_AUDIO_PARAM_SIZE 10

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
typedef enum
{
    IPTV_PLAYER_EVT_STREAM_VALID=0,
    IPTV_PLAYER_EVT_FIRST_PTS,   //�����һ֡
    IPTV_PLAYER_EVT_VOD_EOS,    //VOD�������
    IPTV_PLAYER_EVT_ABEND,         //Ϊ�ϱ������¼������ӵ�����
    IPTV_PLAYER_EVT_PLAYBACK_ERROR,	// ���Ŵ���
}IPTV_PLAYER_EVT_e;

typedef void (*IPTV_PLAYER_EVT_CB)(IPTV_PLAYER_EVT_e evt, void *handler);


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
	
	virtual void SetSurface(Surface* pSurface);
	//16λɫ����Ҫ����colorkey��͸����Ƶ��
     virtual void SwitchAudioTrack(int pid) = 0;
     virtual void SwitchSubtitle(int pid) = 0;
     virtual void SetProperty(int nType, int nSub, int nValue) = 0;
     virtual long GetCurrentPlayTime() = 0;
     virtual void leaveChannel() = 0;
	virtual void playerback_register_evt_cb(IPTV_PLAYER_EVT_CB pfunc, void *hander) = 0;
};

class CTsPlayer : public ITsPlayer
{
public:
	CTsPlayer();
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
    
    virtual long GetCurrentPlayTime() ;
    
    virtual void leaveChannel() ;
	virtual void playerback_register_evt_cb(IPTV_PLAYER_EVT_CB pfunc, void *hander);
	
protected:
	int		m_bLeaveChannel;
	
private:
	AUDIO_PARA_T a_aPara[MAX_AUDIO_PARAM_SIZE];
	VIDEO_PARA_T vPara;	
	int player_pid;
	codec_para_t codec;
	codec_para_t *pcodec;
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
#ifdef WF
	FILE*	m_fp;
#endif

};

#endif
