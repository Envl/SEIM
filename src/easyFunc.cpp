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

/*----����Win32API��������---*/

void  EasyFunction::drawCNString(const char* str) {
	int len, i;
	wchar_t* wstring;
	HDC hDC = wglGetCurrentDC();
	GLuint list = glGenLists(1);

	// �����ַ��ĸ���
	// �����˫�ֽ��ַ��ģ����������ַ����������ֽڲ���һ���ַ�
	// ����һ���ֽ���һ���ַ�
	len = 0;
	for (i = 0; str[i] != '\0'; ++i)
	{
		if (IsDBCSLeadByte(str[i]))
			++i;
		++len;
	}

	// ������ַ�ת��Ϊ���ַ�
	wstring = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len);
	wstring[len] = L'\0';

	// �������ַ�
	for (i = 0; i<len; i++) {
		wglUseFontBitmapsW(hDC, wstring[i], 1, list);
		glCallList(list);
	}

	// ����������ʱ��Դ
	free(wstring);
	glDeleteLists(list, 1);
}
#define MAX_CHAR       128
void drawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) { // ����ǵ�һ�ε��ã�ִ�г�ʼ��
					   // Ϊÿһ��ASCII�ַ�����һ����ʾ�б�
		isFirstCall = 0;

		// ����MAX_CHAR����������ʾ�б���
		lists = glGenLists(MAX_CHAR);

		// ��ÿ���ַ��Ļ������װ����Ӧ����ʾ�б���
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// ����ÿ���ַ���Ӧ����ʾ�б�����ÿ���ַ�
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
	selectFont(48, GB2312_CHARSET, "����_GB2312");

	glColor3f(1.0f, 1.0f, 0.0f);
	glRasterPos2f(200.0f, 200.0f);
	EasyFunction::drawCNString("�������й�����");

	selectFont(48, DEFAULT_CHARSET, "���ķ���");
	glColor3f(0.0f, 1.0f, 0.0f);
	glRasterPos2f(300.0f, 300.0f);
	EasyFunction::drawCNString("���y���Ї��h��");

}

