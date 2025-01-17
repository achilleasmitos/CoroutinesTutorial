#pragma once

#include <coroutine>
#include <iostream>

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
#pragma region Necessary methods for "promise_type"
		Generator get_return_object();
		std::suspend_never initial_suspend() noexcept;
		std::suspend_always final_suspend() noexcept;

		// Method required for 'co_yield' to work
		std::suspend_always yield_value(int value);

		void unhandled_exception();
		void return_void() noexcept;
#pragma endregion

		// Access the held value
		int GetStoredValue() const;

	private:
		int m_value{ 15 };
	};
#pragma endregion

#pragma region Special member functions
	explicit Generator(HandleType h);
	Generator(Generator&& source) noexcept;
	~Generator() noexcept;
#pragma endregion

	// User-defined methods with which to interface
	// with the uderlying promise_type methods
	int get_value() const;
	bool is_finished() const;
	void resume() const;

private:
	// The underlying promise_type
	HandleType m_handle{};
};
