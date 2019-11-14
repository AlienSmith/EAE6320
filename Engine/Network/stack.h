#pragma once
#include <exception>
#include <memory>
#include <mutex>
#include <stack>
namespace Network {
	struct empty_stack : std::exception {
		const char* what() const throw() {
			return "Try to pop an Empty Stack";
		}
	};
	//Implementaions from C++ Concurrency Listing 3.5
	template<typename T>
	class threadsafe_stack {
	private:
		std::stack<T> data;
		//can be changed even for const memeber function of the instance 
		mutable std::mutex m;
	public:
		threadsafe_stack(){}
		threadsafe_stack(const threadsafe_stack& other) {
			//lock the other's mutex, template argument deduction and scope_lock are new features in C++17 so is the following functions
			std::scoped_lock lock(other.m);
			data = other.data;
		}
		threadsafe_stack& operator = (const threadsafe_stack&) = delete;
		void push(T new_value) {
			std::scoped_lock lock(m);
			data.push(std::move(new_value));
		}
		std::shared_ptr<T> pop() {
			std::scoped_lock lock(m);
			if (data.empty()) throw empty_stack();
			std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
			data.pop();
			return res;
		}
		void pop(T& value) {
			std::scoped_lock lock(m);
			if (data.empty()) throw empty_stack();
			value = data.top();
			data.pop();
		}
		bool empty() const {
			std::scoped_lock lock(m);
			return data.empty();
		}
	};
}