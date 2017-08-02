#include "easyFunc.h"

void  EasyFunction:: SafeCloseKinect(ofxKFW2::Device &kinect)
{
	using namespace ofxKFW2;
	if (kinect.isOpen()) {
		if (kinect.hasSource<Source::Body>())
		{
			kinect.releaseBodySource();
			cout << "body released" << endl;
		}
		if (kinect.hasSource<Source::BodyIndex>())
		{
			kinect.releaseBodyIndexSource();
			cout << " bodyindex released" << endl;
		}
		if (kinect.hasSource<Source::Depth>())
		{
			kinect.releaseDepthSource();
			cout << "depth released" << endl;
		}
		if (kinect.hasSource<Source::Infrared>())
		{
			kinect.releaseInfraredSource();
			cout << "ir released" << endl;
		}
		if (kinect.hasSource<Source::Color>())
		{
			kinect.releaseColorSource();
			cout << "color released" << endl;
		}
		if (kinect.hasSource<Source::LongExposureInfrared>())
		{
			kinect.releaseLongExposureInfraredSource();
			cout << "longExpoIR released" << endl;
		}

		kinect.close();
	}
}

/*----调用Win32API绘制中文---*/

void  EasyFunction::drawCNString(const char* str) {
	int len, i;
	wchar_t* wstring;
	HDC hDC = wglGetCurrentDC();
	GLuint list = glGenLists(1);

	// 计算字符的个数
	// 如果是双字节字符的（比如中文字符），两个字节才算一个字符
	// 否则一个字节算一个字符
	len = 0;
	for (i = 0; str[i] != '\0'; ++i)
	{
		if (IsDBCSLeadByte(str[i]))
			++i;
		++len;
	}

	// 将混合字符转化为宽字符
	wstring = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len);
	wstring[len] = L'\0';

	// 逐个输出字符
	for (i = 0; i<len; i++) {
		wglUseFontBitmapsW(hDC, wstring[i], 1, list);
		glCallList(list);
	}

	// 回收所有临时资源
	free(wstring);
	glDeleteLists(list, 1);
}
#define MAX_CHAR       128
void drawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) { // 如果是第一次调用，执行初始化
					   // 为每一个ASCII字符产生一个显示列表
		isFirstCall = 0;

		// 申请MAX_CHAR个连续的显示列表编号
		lists = glGenLists(MAX_CHAR);

		// 把每个字符的绘制命令都装到对应的显示列表中
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// 调用每个字符对应的显示列表，绘制每个字符
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}

void EasyFunction::selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

void EasyFunction::display(void) {
	EasyFunction::selectFont(48, ANSI_CHARSET, "Comic Sans MS");

	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0f, 0.0f, 0.0f);
	glRasterPos2f(100.0f, 100.0f);
	drawString("Hello, World!");
	selectFont(48, GB2312_CHARSET, "楷体_GB2312");

	glColor3f(1.0f, 1.0f, 0.0f);
	glRasterPos2f(200.0f, 200.0f);
	EasyFunction::drawCNString("当代的中国汉字");

	selectFont(48, DEFAULT_CHARSET, "华文仿宋");
	glColor3f(0.0f, 1.0f, 0.0f);
	glRasterPos2f(300.0f, 300.0f);
	EasyFunction::drawCNString("傳統的中國漢字");

}

