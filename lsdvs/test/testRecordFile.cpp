#if 0
// 读取录像文約，检查该文約录像的完整G�
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define REC_FILENAME "./rec.h264"

typedef struct _PackHead_
{
	unsigned char 	packType;    // 包类G�, 0:视频包; 1:音频包; 2:智能包
	unsigned char	frameHeadLen;    // 帧头长度,该长度根綢packType 的值变化而取 sizeof(不同的结构体)    
	char	reserved[2];
} PACK_HEAD_T;

// 音视频帧头描述
typedef struct _FrameHead_
{
    unsigned int  frameLen;            // 帧裸流长度
    unsigned int  frameNo;            // 帧号,单调礗增
    unsigned char videoStandard;    // 视频:标准 0,PAL; 1,NTSC. // 音频:通道模式, 0:单通道, 1:立体声, 默认值 0
    unsigned char videoResolution;    // 视频:分辨率 0,qcif; 1,cif; 2,hd1; 3,d1. // 音频:采样率, 0:8k, 1:16k, 默认值 0
    unsigned char frameRate;          // 视频:帧率. // 音频:位宽,0:8bits, 1:16bits, 默认值 1
    unsigned char frameType;         // 视频:帧类G� 5,I帧; 1,P帧 . // 音频::编码方式,0:adpcm, 1:g711, 2:g726, 默认值 0
    unsigned int  sec;                // 从本地获取的时间,秒
    unsigned int  usec;                // 从本地获取的时间,微妙
    unsigned long long  pts;                // 从海思底层获取的时间戳,(微秒)
} FRAME_HEAD_ST;

 
 typedef struct _StreamHead_
 {
 	PACK_HEAD_T 	packHead;
 	FRAME_HEAD_ST	frameHead;
 } STREAM_HEAD_T;

static void PrintFrameHead( STREAM_HEAD_T *streamHead )
{
	printf( "pt(%u), fhl(%u), fl(%u), fn(%u), vs(%u), vr(%u), fr(%u), ft(%u)!\r\n",
    	streamHead->packHead.packType,
    	streamHead->packHead.frameHeadLen,
    	streamHead->frameHead.frameLen,
    	streamHead->frameHead.frameNo,
    	streamHead->frameHead.videoStandard,
    	streamHead->frameHead.videoResolution,
    	streamHead->frameHead.frameRate,
    	streamHead->frameHead.frameType
        );
}
 
 int main()
 {
	int fd, ret;
	unsigned int lastFrameNo, totalReadSize = 0;
	static char buf[1024*1024];
	STREAM_HEAD_T streamHead;
	fd = open( REC_FILENAME, O_RDONLY );

	ret = read( fd, &streamHead, sizeof(streamHead) );
	if( ret <= 0 )
    {
    	return 0;
    }
	totalReadSize += ret;
    
	printf( "totalReadSize = %u\r\n", totalReadSize );
	ret = read( fd, buf, streamHead.frameHead.frameLen );
	if( ret <= 0 )
    {
    	return 0;
    }
	totalReadSize += ret;
    
	printf( "totalReadSize = %u\r\n", totalReadSize );
	lastFrameNo = streamHead.frameHead.frameNo;
	printf( "start test\r\n" );
	while( 1 )
    {        
    	ret = read( fd, &streamHead, sizeof(streamHead) );
    	if( ret <= 0 )
        {
        	printf( "ret(%d) = read(streamHead)\r\n", ret );
        	break;
        }
    	totalReadSize += ret;
        
    	printf( "totalReadSize = %u\r\n", totalReadSize );
    	PrintFrameHead( &streamHead );
    	ret = read( fd, buf, streamHead.frameHead.frameLen );
    	if( ret <= 0 )
        {            
        	printf( "ret(%d) = read(buf)\r\n", ret );
        	break;
        }
    	totalReadSize += ret;
        
    	printf( "totalReadSize = %u\r\n", totalReadSize );
        #if 0
    	if( 1 != (streamHead.frameHead.frameNo - lastFrameNo) )
        {
        	printf( "1 != (streamHead.frameHead.frameNo - lastFrameNo)\r\n" );
        	break;
        }
        #endif
    	lastFrameNo = streamHead.frameHead.frameNo;
    }
    
	printf( "stop test\r\n" );

	return 0;
}

#endif //

