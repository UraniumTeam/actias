#pragma once
#include <Actias/Memory/IAllocator.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <utility>

class MockConstructors
{
public:
    MOCK_METHOD(void, Construct, (), (noexcept));
    MOCK_METHOD(void, Destruct, (), (noexcept));
    MOCK_METHOD(void, Copy, (), (noexcept));
    MOCK_METHOD(void, Move, (), (noexcept));
};

class MockObject
{
    std::shared_ptr<MockConstructors> m_Mock;

public:
    MockObject(std::shared_ptr<MockConstructors> mock) noexcept
    {
        m_Mock = std::move(mock);
        m_Mock->Construct();
    }

    MockObject(const MockObject& other) noexcept
    {
        m_Mock = other.m_Mock;
        m_Mock->Copy();
    }

    MockObject(MockObject&& other) noexcept
    {
        m_Mock = other.m_Mock;
        other.m_Mock.reset();
        m_Mock->Move();
    }

    ~MockObject() noexcept
    {
        if (m_Mock)
        {
            m_Mock->Destruct();
        }
    }

    USize GetHash() const
    {
        return reinterpret_cast<USize>(m_Mock.get());
    }

    inline friend bool operator==(const MockObject& lhs, const MockObject& rhs)
    {
        return lhs.m_Mock == rhs.m_Mock;
    }

    inline friend bool operator!=(const MockObject& lhs, const MockObject& rhs)
    {
        return lhs.m_Mock != rhs.m_Mock;
    }
};

template<>
struct Actias::Hash<MockObject> final
{
    inline size_t operator()(const MockObject& value) const
    {
        return value.GetHash();
    }
};

class TestAllocator : public Actias::IAllocator
{
};
