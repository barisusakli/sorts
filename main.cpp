// bubble sort,heap sort,quick sort and std::sort 
// with comparisons

#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <windows.h>
#include <algorithm>

#include "Profiler.h"
#include "rdesort.h"

using std::cout;

template<class T>
void swap(T& a,T& b)
{
	T c = a;
	a = b;
	b = c;
}


template<typename T>
struct myLess
{
	bool operator()(const T& lhs, const T& rhs) const
	{
		return lhs < rhs;
	}
};


// Bubble Sort

template<class T,class TPredicate>
void bubbleSort(std::vector<T>& data,TPredicate pred)
{
	// number of swaps made in each pass
	int swapCount = 0;


	for(unsigned int i=1; i<data.size(); ++i)
	{
		swapCount = 0;
		for(unsigned int k=0; k<data.size() - i; ++k)
		{
			if( pred(data[k+1],data[k]))
			{
				swap(data[k],data[k+1]);
				++swapCount;
			}
		}

		//if there are no swaps in a pass the array is sorted
		if (swapCount == 0)
			return;	
	}
}

// Heap Sort

// converts an array into a heap
template<class T,class TPredicate>
void heapify(std::vector<T>& data,unsigned int current,unsigned int size,TPredicate pred)
{
	while(current < size)
	{
		unsigned int left = current * 2 + 1;
		unsigned int right = left + 1;

		if(left < size)
		{
			unsigned int index = current;

			if(pred( data[index], data[left] ))
				index = left;

			if( right < size )
			{
				if(pred( data[index], data[right] ))
					index = right;
			}

			if(index!=current)
			{
				swap(data[current],data[index]);
				current = index;
			}
			else
				break;
		}
		else 
			break;
	}
}


template<class T,class TPredicate>
void heapSort(std::vector<T>& data,TPredicate pred)
{
	int start = data.size()/2 - 1;

	for(int i=start; i>=0; --i)
	{
		heapify(data,i,data.size(),pred);
	}

	for(unsigned int i=0; i < data.size() -1 ; ++i)
	{
		swap(data[0],data[data.size()-i-1]);
		heapify(data,0,data.size()- i -1,pred);
	}
}


// Quick Sort


template<class T,class TPredicate>
void quickSortRecurse(std::vector<T>& data,int first,int size,TPredicate pred)
{
	if(size > 1)
	{
		int pivot = 0;
		int last = first + size - 1;
		int mid = 0;

		int lower = first;
		int higher = last;

	
		mid = (lower+higher)>>1;
		pivot = data[mid];
		data[mid] = data[first];

		while(lower<higher)
		{
			while(pred(pivot,data[higher]) && lower < higher)
				higher--;

			if(higher!=lower)
			{
				data[lower] = data[higher];
				lower++;
			}

			while(pred(data[lower],pivot) && lower < higher)
				lower++;

			if(higher!=lower)
			{
				data[higher] = data[lower];
				higher--;
			}
		}

		data[lower] = pivot;
		quickSortRecurse(data,first,lower-first,pred);
		quickSortRecurse(data,lower+1,last-lower,pred);

	}
}

template<class T,class TPredicate>
void quickSort(std::vector<T>& data,TPredicate pred)
{
	quickSortRecurse(data,0,data.size(),pred);
}

// for printing the data
template<class T>
void printData(std::vector<T>& data)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	std::wostringstream oss;
	DWORD dwChars;
	for(unsigned int i=0; i<data.size(); ++i)
	{
		oss<<data[i]<<" ";
	}
	oss<<"\n";
	std::wstring text = oss.str();
	WriteConsole(hStdout,text.c_str(), (DWORD)text.length(), &dwChars, NULL);
}


DWORD WINAPI thread1(LPVOID args)
{

	DWORD dwChars;

	// Make sure there is a console to receive output results. 
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if( hStdout == INVALID_HANDLE_VALUE )
		return 1;

	std::vector<int>* data = reinterpret_cast<std::vector<int>*>(args);

	//make copy of data
	std::vector<int> bubbleData = *data;
	Profiler profiler;
	profiler.beginProfile("bubbleSort");
	bubbleSort<int>(bubbleData,myLess<int>());
	profiler.endProfile("bubbleSort");

	std::wostringstream oss;

	oss<<"Bubble sort : "<<std::fixed<<std::showpoint<<std::setprecision(10)<<profiler.getData("bubbleSort")<<" seconds\n";
	std::wstring text  = oss.str();

	WriteConsole(hStdout,text.c_str(), (DWORD)text.length(), &dwChars, NULL);

	return 0;
}

DWORD WINAPI thread2(LPVOID args)
{
	DWORD dwChars;

	// Make sure there is a console to receive output results. 
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if( hStdout == INVALID_HANDLE_VALUE )
		return 1;

	std::vector<int>* data = reinterpret_cast<std::vector<int>*>(args);

	//make copy of data
	std::vector<int> heapData = *data;
	Profiler profiler;
	profiler.beginProfile("heapSort");
	heapSort<int>(heapData,myLess<int>());
	profiler.endProfile("heapSort");

	std::wostringstream oss;

	oss<<"Heap sort   : "<<std::fixed<<std::showpoint<<std::setprecision(10)<<profiler.getData("heapSort")<<" seconds\n";
	std::wstring text  = oss.str();

	WriteConsole(hStdout,text.c_str(), (DWORD)text.length(), &dwChars, NULL);

	return 0;
}

DWORD WINAPI thread3(LPVOID args)
{
	DWORD dwChars;

	// Make sure there is a console to receive output results. 
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if( hStdout == INVALID_HANDLE_VALUE )
		return 1;

	std::vector<int>* data = reinterpret_cast<std::vector<int>*>(args);

	//make copy of data
	std::vector<int> quickSortData = *data;

	Profiler profiler;
	profiler.beginProfile("quickSort");
	quickSort<int>(quickSortData,myLess<int>());
	profiler.endProfile("quickSort");

	std::wostringstream oss;

	oss<<"Quick sort  : "<<std::fixed<<std::showpoint<<std::setprecision(10)<<profiler.getData("quickSort")<<" seconds\n";
	std::wstring text  = oss.str();

	WriteConsole(hStdout,text.c_str(), (DWORD)text.length(), &dwChars, NULL);

	return 0;
}

template<class T>
void withThreads(std::vector<T> data)
{
	HANDLE threads[3];
	DWORD threadIds[3];

	threads[0] = CreateThread( NULL, 0, thread1, &data, 0, &threadIds[0]);
	threads[1] = CreateThread( NULL, 0, thread2, &data, 0, &threadIds[1]);
	threads[2] = CreateThread( NULL, 0, thread3, &data, 0, &threadIds[2]);

	WaitForMultipleObjects(3, threads, TRUE, INFINITE);

	for(int i=0; i<3; i++)
	{
		CloseHandle(threads[i]);
	}

}

template<class T>
void withouthThreads(std::vector<T>& data)
{
	Profiler profiler;
	//make copy of data
	std::vector<int> bubbleData = data;

	profiler.beginProfile("bubbleSort");
	bubbleSort<int>(bubbleData,myLess<int>());
	profiler.endProfile("bubbleSort");
		
	
	cout<<"Bubble sort : "<<std::fixed<<std::showpoint<<std::setprecision(10)<<profiler.getData("bubbleSort")<<" seconds\n";
	
	//heap sort
	std::vector<int> heapData = data;

	profiler.beginProfile("heapSort");
	heapSort<int>(heapData,myLess<int>());
	profiler.endProfile("heapSort");

	cout<<"Heap sort   : "<<std::fixed<<std::showpoint<<std::setprecision(10)<<profiler.getData("heapSort")<<" seconds\n";
		
	//quick sort
	std::vector<int> quickSortData = data;


	profiler.beginProfile("quickSort");
	quickSort<int>(quickSortData,myLess<int>());
	profiler.endProfile("quickSort");

	cout<<"Quick sort  : "<<std::fixed<<std::showpoint<<std::setprecision(10)<<profiler.getData("quickSort")<<" seconds\n";
	
	//std::sort
	std::vector<int> stdsortData = data;
	
	profiler.beginProfile("std::sort");
	std::sort(stdsortData.begin(),stdsortData.end(),myLess<int>());
	profiler.endProfile("std::sort");


	cout<<"std::sort   : "<<std::fixed<<std::showpoint<<std::setprecision(10)<<profiler.getData("std::sort")<<" seconds\n";

	int* rdedata = new int[data.size()];
	std::copy(data.begin(),data.end(),rdedata);

	profiler.beginProfile("rde::sort");
	rde::quick_sort(rdedata,rdedata+data.size());
	profiler.endProfile("rde::sort");
	delete[] rdedata;

	cout<<"rde::sort   : "<<std::fixed<<std::showpoint<<std::setprecision(10)<<profiler.getData("rde::sort")<<" seconds\n";

}



int main(int argc, char* argv[])
{
	// create array and fill with random values
	int arraysStartingSize = 100;
	unsigned int runs = 4;


	for(unsigned int n=0; n<runs; ++n)
	{
		cout<<"Sorting "<<arraysStartingSize<<" elements\n";
		std::vector<int> data;

		for(int i=0; i<arraysStartingSize; ++i)
		{
			data.push_back(rand()%1000);	
		}

		//withThreads(data);
		withouthThreads(data);

		// enlarge the array size
		arraysStartingSize = arraysStartingSize * 10;

		cout<<"\n\n";

	}
	int stop;
	std::cin>>stop;
	return 0;
}

