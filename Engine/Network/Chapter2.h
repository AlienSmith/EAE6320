#pragma once
#include <iostream>
#include <External/Asio/asio.hpp>
int CreateOutputBuffer() {
	//allocate buffer
	const std::string buf = "Hello";
	//convert to constBufferSequence like item
	asio::const_buffers_1 output_buf = asio::buffer(buf);
	return 0;
}
int CreateInputBuffer() {
	const size_t BUF_SIZE_BYTES = 20;
	//allocate 
	std::unique_ptr<char[]> buf(new char[BUF_SIZE_BYTES]);
	//convert
	asio::mutable_buffers_1 input_buf = asio::buffer(static_cast<void*>(buf.get()), BUF_SIZE_BYTES);
	return 0;
}