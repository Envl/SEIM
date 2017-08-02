#include "VideoWrapper.h"

void VideoWrapper::Activate( )
{
	bIsActive = true;
}

void  VideoWrapper::Deactivate( )
{
	bIsActive = false;
}

void VideoWrapper::load(string path)
{
	player.loadMovie(path);
}

void VideoWrapper::draw(int x , int y , int w , int h)
{
	if(!bIsActive)
	{
		return;
	}
	if(bHideWhenFinish && player.isStopped())
	{
		return;
	}
	player.draw(x , y , w , h);
}

void VideoWrapper::update( )
{
	auto a = player.getDuration( ) - player.getPosition( );
	if(!bFirst && a<=0.5f )  // 视频最后留一秒黑后 改成0.5
	{
		player.pause( );
		return;
	}
	bFirst = false;
	player.update( );
}

void VideoWrapper::setHideWhenFinish(bool flag)
{
	bHideWhenFinish = flag;
}

