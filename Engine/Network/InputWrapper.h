#pragma once
#include <time.h>
namespace Network {
	template<class T>
	struct InputWrapper{
		T t;
		int Socket_id;
	};
	template<class U>
	struct UpdateWrapper {
		U u;
		time_t time;
	};
}