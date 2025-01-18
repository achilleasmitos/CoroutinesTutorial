#include "Generator.h"

namespace networkrequest
{

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

void Generator::resume() const
{
	if (!m_handle.done())
	{
		m_handle.resume();
	}
}
#pragma endregion

} // namespace networkrequest
