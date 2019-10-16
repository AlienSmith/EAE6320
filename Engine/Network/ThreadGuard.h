#pragma once
#include<thread>
namespace Networking {
	class thread_guard {
		std::thread& t;
	public:
		explicit thread_guard(std::thread& t_):t(t_){}
		~thread_guard() {
			if (t.joinable()) {
				t.join();
			}
		}
	};
}