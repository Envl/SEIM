#pragma once
#include <ofxKinectForWindows2.h>

namespace EasyFunction {
	//删除指针   
	template <typename  T>
	inline void DelPTR(T* ptr)//模版函数必须在头文件中实现  参见知乎回答 https://www.zhihu.com/question/20630104
	{
		delete ptr;
		ptr = NULL;
	}

	//关闭Kinect
	void SafeCloseKinect(ofxKFW2::Device &kinect);

	//调用 Win32API 绘制中文
	void  drawCNString(const char* str);
	void  display(void);
	void  selectFont(int size, int charset, const char* face);

}
