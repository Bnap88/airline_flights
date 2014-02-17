//Brandon Knapp
#include <iostream>

using namespace std;

template <class T>
class PQueue
{
public:
	PQueue(int size=100);
	~PQueue();
	bool isEmpty();
	bool isFull();
	void makeEmpty();
	void enqueue(T item);
	void dequeue();
	int findDistance(int num);
	void reducePriority(int num, int x, int j);
	T top();
private:
	T * data;
	int length;
	int maxSize;
	void find(int num, bool & found, int & loc);
	void HeapifyUp(int nodeNum);
	void HeapifyDown(int nodeNum);
};

template <class T>
PQueue<T>::PQueue(int size)
{
	if (size < 1)
		size = 100;

	maxSize = size;
	data = new T[maxSize];
	length = 0;
}

template <class T>
PQueue<T>::~PQueue()
{
	delete []data;
}

template <class T>
bool PQueue<T>::isEmpty()
{
	return (length == 0);
}

template <class T>
bool PQueue<T>::isFull()
{
	return ( length == maxSize );
}

template <class T>
void PQueue<T>::makeEmpty()
{
	length = 0;
}

template <class T>
T PQueue<T>::top()
{
	return data[0];
}

template <class T>
void PQueue<T>::enqueue(T newItem)
{
	if (!isFull())
	{
		data[length] = newItem;
		length++;
		HeapifyUp(length);
	}
}

template <class T>
void PQueue<T>::dequeue()
{
	if (!isEmpty())
	{
		data[0] = data[length-1];
		length--;
		HeapifyDown(1);
	}
}

template <class T>
void PQueue<T>::HeapifyUp(int nodeNum)
{
	int childloc = nodeNum-1;
	int parentloc = nodeNum/2-1;

	if (nodeNum == 1 || data[parentloc] <= data[childloc])
		return;

	swap (data[parentloc], data[childloc]);

	HeapifyUp(parentloc+1);
}

template<class T>
void PQueue<T>::HeapifyDown(int nodeNum)
{
	int parentloc = nodeNum - 1;
	int Lchildloc = nodeNum*2-1;
	int Rchildloc = Lchildloc+1;

	if ( Lchildloc > length-1 )
		return;

	int smaller = Lchildloc;

	if (Rchildloc <= length-1 && data[Rchildloc] < data[Lchildloc])
		smaller = Rchildloc;

	if (data[parentloc] > data[smaller])
	{
		swap(data[parentloc], data[smaller]);
		HeapifyDown(smaller+1);
	}
}
template<class T>
void PQueue<T>::reducePriority(int num, int x, int j)
{
	int arrPos;
	bool found;
	find(num, found, arrPos);

	if (found)
	{
		data[arrPos].totalDistance = x;
		data[arrPos].previous = j;
		HeapifyUp(arrPos+1);
	}
}
template<class T>
int PQueue<T>::findDistance(int num)
{
	int arrPos;
	bool found;
	find(num, found, arrPos);

	if (found)
	{
		return data[arrPos].totalDistance;	
	}
}
template<class T>
void PQueue<T>::find(int num, bool & found, int & loc)
{
	found = false;
	loc = 0;
	while (loc < length && num != data[loc].number)
		loc++;

	if (loc != length)
		found = true;
}
