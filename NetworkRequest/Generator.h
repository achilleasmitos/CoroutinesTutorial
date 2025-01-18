#pragma once

#include <coroutine>
#include <future>
#include <iostream>
#include <vector>

namespace networkrequest
{

// Custom awaiter for a std::future
template<typename T>
struct FutureAwaiter
{
	std::future<T>& future;

	bool await_ready() const noexcept { return false; }

	void await_suspend(std::coroutine_handle<> handle)
	{
		// Spawn a new thread that awaits the provider notification
		// Return void for the control to be passed back to the caller of the current coroutine
		std::thread coroutineThread(
			[this, handle]() mutable
			{
				future.wait();
				handle.resume();
			});
		std::cout << "Created coroutine thread: " << coroutineThread.get_id() << std::endl;
		coroutineThread.detach();
	}

	T await_resume()
	{
		std::cout << "Coroutine thread: " << std::this_thread::get_id() << std::endl;
		return future.get();
	}
};

// Wrapper around the 'promise_type' class
// This is the interface for the coroutine calls
class Generator
{
public:
#pragma region Define/Setup the necessary types for the underlying promise_type
	class promise_type;
	using HandleType = std::coroutine_handle<promise_type>;

	// The name 'promise_type' is required to be exactly so by the compiler
	class promise_type
	{
	public:
		Generator get_return_object()
		{
			return Generator{HandleType::from_promise(*this)};
		}
		std::suspend_always initial_suspend() noexcept { return {}; }
		std::suspend_always final_suspend() noexcept { return {}; }

		void unhandled_exception() { std::exit(1); }
		void return_void() noexcept {}
	};
#pragma endregion

#pragma region Special member functions
	explicit Generator(HandleType h);
	Generator(Generator&& source) noexcept;
	~Generator() noexcept;
#pragma endregion

	// User-defined methods with which to interface
	// with the uderlying promise_type methods
	void resume() const;

private:
	// The underlying promise_type
	HandleType m_handle{};
};

} // namespace networkrequest
