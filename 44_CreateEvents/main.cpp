#include <windows.h>
#include <process.h>
#include <iostream>

HANDLE g_CountEvent;
int g_Data = 0;

unsigned WINAPI GenerateNumbers(PVOID arg_param)
{
	for (size_t i = 0; i < 1000; i++)
	{
		g_Data++;
		std::cout << "Producer Thread - Write Item: " << g_Data << std::endl;
	}

	SetEvent(g_CountEvent);
	return 0;
}

unsigned WINAPI ReadNumbersReverse(PVOID arg_param)
{
	WaitForSingleObject(g_CountEvent, INFINITE);
	for (size_t i = 0; i < 1000; i++)
	{
		g_Data--;
		std::cout << "Consumer Thread - Read Item: " << g_Data << std::endl;
	}
	
	return 0;
}

int main(int argc, char* argv[])
{
	g_CountEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	HANDLE handle_producer = (HANDLE)_beginthreadex(NULL, 0, GenerateNumbers, NULL, 0, NULL);
	HANDLE handle_consumer = (HANDLE)_beginthreadex(NULL, 0, ReadNumbersReverse, NULL, 0, NULL);

	WaitForSingleObject(handle_producer, INFINITE);
	WaitForSingleObject(handle_consumer, INFINITE);

	CloseHandle(g_CountEvent);
	CloseHandle(handle_producer);
	CloseHandle(handle_consumer);

	return 0;
}





//
//#include <mutex>
//#include <condition_variable>
//#include <thread>
//#include <stdio.h>
//
//namespace event
//{
//	class auto_reset
//	{
//	public:
//		explicit auto_reset(bool initial = false);
//
//		void set();
//		void reset();
//
//		bool wait_one();
//
//	private:
//		bool m_flag;
//		std::mutex m_protect;
//		std::condition_variable m_signal;
//	};
//};
//
//event::auto_reset::auto_reset(bool initial) : m_flag(initial)
//{
//}
//
//void event::auto_reset::set()
//{
//	std::lock_guard<std::mutex> lock(m_protect);
//	m_flag = true;
//	m_signal.notify_one();
//}
//
//void event::auto_reset::reset()
//{
//	std::lock_guard<std::mutex> lock(m_protect);
//	m_flag = false;
//}
//
//bool event::auto_reset::wait_one()
//{
//	std::unique_lock<std::mutex> lock(m_protect);
//	while (!m_flag)
//	{
//		m_signal.wait(lock);
//	}
//	m_flag = false;
//	return true;
//}
//
//
//event::auto_reset g_event;
//
//void Task()
//{
//	g_event.wait_one();
//	printf("Hello from other thread!\n");
//}
//
//
//int main()
//{
//	std::thread worker(Task);
//	printf("Hello from the first thread\n");
//	g_event.set();
//
//	worker.join();
//}