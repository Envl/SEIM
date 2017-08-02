#pragma once
#include <deque>
#include <ofMain.h>

using namespace std;
template <class  T>
class SmoothValueFilter
{
private:
	int size;
	T smoothedValue=0;
	T lastValue = 0;
public:
	deque<T> deque;
	
	SmoothValueFilter<T>( int size);
	SmoothValueFilter<T>( ) = default;
	void push(T item);
	T getValue( );
};

template <class T>
SmoothValueFilter<T>::SmoothValueFilter(int size)
{
	this->size = size;
}

template <class T>
void SmoothValueFilter<T>::push(T item)
{
	while(deque.size()>size)
	{
		deque.pop_front( );
	}
	deque.push_back(item);
	//去掉最大最小值  计算平滑后的值
	float total=0;
	int maxIndex=0 , minIndex=0;
	for(int i=0;i<deque.size();i++)
	{
		total += deque[i];
		maxIndex = deque[maxIndex] > deque[i] ? maxIndex : i;
		minIndex = deque[minIndex] < deque[i] ? minIndex : i;
	}
	//去掉最大最小值
	total -= deque[maxIndex];
	total -= deque[minIndex];
	//未加权的平均值
	if(deque.size()>2)
	{
//		lastValue = smoothedValue;
		smoothedValue = ofLerp(smoothedValue , total / (deque.size( ) - 2), 0.7f);
	}
}

template <class T>
T SmoothValueFilter<T>::getValue()
{
	return smoothedValue;
}
