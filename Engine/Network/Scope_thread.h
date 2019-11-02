#pragma once
#include<thread>
#include<stdexcept>
class scope_thread {
	std::thread t;
public:
	explicit scope_thread(std::thread _t) :t(std::move(_t)) {
		if (!t.joinable()) {
			throw std::logic_error("No binded thread");
		}
	}
	explicit scope_thread(std::thread _t, std::exception& exception) {
		if (!t.joinable()) {
			exception = std::logic_error("No binded thread");
		}
	}
	~scope_thread() {
		t.join();
	}
	scope_thread(const scope_thread&) = delete;
	scope_thread& operator = (const scope_thread&) = delete;
};