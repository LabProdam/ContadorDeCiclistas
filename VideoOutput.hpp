#include <opencv2/opencv.hpp>
#include <linux/videodev2.h>

#define FRAME_WIDTH  640
#define FRAME_HEIGHT 480
#define FRAME_FORMAT V4L2_PIX_FMT_BGR24
#define FRAME_SIZE 3*FRAME_WIDTH*FRAME_HEIGHT
#define BYTES_PER_LINE 640//240

class VideoOutput {
private:
		int fdrw;
		char *outputDevice;		
		struct v4l2_format vid_format;
public:
		VideoOutput(const char *outputevice);
		~VideoOutput();
		
		void print_format(struct v4l2_format* vid_format);
		void format_properties(struct v4l2_format* vid_format);		
		void write(cv::Mat &frame);
};
