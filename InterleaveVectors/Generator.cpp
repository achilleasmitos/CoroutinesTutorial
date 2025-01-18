#include "Generator.h"

namespace interleavevectors
{

#pragma region promise_type implementation
std::suspend_always Generator::promise_type::yield_value(int value)
{
	m_value = value;
	return {};
}

Generator Generator::promise_type::get_return_object()
{
	return Generator{HandleType::from_promise(*this)};
}

std::suspend_never Generator::promise_type::initial_suspend() noexcept
{
	return {};
}

std::suspend_always Generator::promise_type::final_suspend() noexcept
{
	return {};
}

void Generator::promise_type::unhandled_exception() {}

void Generator::promise_type::return_void() noexcept {}

int Generator::promise_type::GetStoredValue() const { return m_value; }
#pragma endregion

#pragma region Generator implementation
Generator::Generator(HandleType h) : m_handle{h} {}
Generator::Generator(Generator&& source) noexcept
	: m_handle{std::exchange(source.m_handle, nullptr)}
{
}
Generator::~Generator() noexcept
{
	if (m_handle)
	{
		m_handle.destroy();
	}
}

int Generator::get_value() const { return m_handle.promise().GetStoredValue(); }
bool Generator::is_finished() const { return m_handle.done(); }
void Generator::resume() const
{
	if (!is_finished())
	{
		m_handle.resume();
	}
}
#pragma endregion

} // namespace interleavevectors
