#include "VideoOutput.hpp"

#include <linux/videodev2.h>
#include <assert.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

VideoOutput::VideoOutput(const char *outputevice) {
	unsigned int outputSize = strlen(outputDevice);
	this->outputDevice = new char[outputSize];
	sprintf(this->outputDevice, "%s", outputDevice);
	
	this->fdrw = open(outputDevice, O_RDWR);
	assert(this->fdrw >= 0);
	
	this->format_properties(&(this->vid_format));
	int ret_code = ioctl(this->fdrw, VIDIOC_S_FMT, &vid_format);
	assert(ret_code >=0);
}
		
VideoOutput::~VideoOutput() {
	if (this->fdrw >= 0) {
		close(this->fdrw);
	}
}
		
void VideoOutput::print_format(struct v4l2_format* vid_format) {
	printf("	vid_format->type                =%d\n",	vid_format->type );
	printf("	vid_format->fmt.pix.width       =%d\n",	vid_format->fmt.pix.width );
	printf("	vid_format->fmt.pix.height      =%d\n",	vid_format->fmt.pix.height );
	printf("	vid_format->fmt.pix.pixelformat =%d\n",	vid_format->fmt.pix.pixelformat);
	printf("	vid_format->fmt.pix.sizeimage   =%d\n",	vid_format->fmt.pix.sizeimage );
	printf("	vid_format->fmt.pix.field       =%d\n",	vid_format->fmt.pix.field );
	printf("	vid_format->fmt.pix.bytesperline=%d\n",	vid_format->fmt.pix.bytesperline );
	printf("	vid_format->fmt.pix.colorspace  =%d\n",	vid_format->fmt.pix.colorspace );
}

void VideoOutput::format_properties(struct v4l2_format* vid_format) {
	vid_format->type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	vid_format->fmt.pix.width = FRAME_WIDTH;
	vid_format->fmt.pix.height = FRAME_HEIGHT;
	vid_format->fmt.pix.pixelformat = FRAME_FORMAT;
	vid_format->fmt.pix.sizeimage = FRAME_SIZE;
	vid_format->fmt.pix.field = V4L2_FIELD_NONE;
	vid_format->fmt.pix.bytesperline = BYTES_PER_LINE;
	vid_format->fmt.pix.colorspace = V4L2_COLORSPACE_SRGB;
}

void VideoOutput::write(cv::Mat &frame) {
	cv::Mat dst;
	cv::resize(frame, dst, cv::Size(FRAME_WIDTH,FRAME_HEIGHT),0,0,CV_INTER_LINEAR );
	::write(this->fdrw, dst.data, FRAME_SIZE);
}

