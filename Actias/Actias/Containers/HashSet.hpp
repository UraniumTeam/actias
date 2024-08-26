#pragma once
#include <Actias/Containers/List.hpp>
#include <Actias/Memory/MemoryPool.hpp>
#include <Actias/Strings/FixedString.hpp>
#include <Actias/Strings/String.hpp>

namespace Actias
{
    template<class T, USize TBucketCount = 512, USize TElementsInPage = 256>
    class HashSet final
    {
        struct Element final
        {
            Element* pNext;
            T Value;

            inline Element(const T& value, Element* pNext)
                : pNext(pNext)
                , Value(value)
            {
            }

            inline Element(T&& value, Element* pNext)
                : pNext(pNext)
                , Value(std::move(value))
            {
            }
        };

        Pool<Element> m_ElementPool;
        List<Element*> m_Buckets;

    public:
        class Iterator final
        {
            friend HashSet;

            HashSet* m_pParent  = nullptr;
            USize m_BucketIndex = 0;
            Element* m_pCurrent = nullptr;

            inline Iterator(HashSet* pParent, USize bucketIndex, Element* pCurrent)
                : m_pParent(pParent)
                , m_BucketIndex(bucketIndex)
                , m_pCurrent(pCurrent)
            {
            }

        public:
            inline T& operator*()
            {
                return m_pCurrent->Value;
            }

            inline const T& operator*() const
            {
                return m_pCurrent->Value;
            }

            inline Iterator& operator++()
            {
                if (!m_pCurrent)
                {
                    return *this;
                }

                m_pCurrent = m_pCurrent->pNext;
                for (; m_BucketIndex < m_pParent->m_Buckets.Size() && !m_pCurrent; ++m_BucketIndex)
                {
                    m_pCurrent = m_pParent->m_Buckets[m_BucketIndex];
                }

                return *this;
            }

            inline Iterator operator++(int)
            {
                Iterator t = *this;
                ++(*this);
                return t;
            }

            inline friend bool operator==(const Iterator& a, const Iterator& b)
            {
                return a.m_pCurrent == b.m_pCurrent;
            }

            inline friend bool operator!=(const Iterator& a, const Iterator& b)
            {
                return a.m_pCurrent != b.m_pCurrent;
            }
        };

        inline HashSet()
            : m_ElementPool(TElementsInPage)
        {
            m_Buckets.Resize(TBucketCount);
        }

        inline HashSet(std::initializer_list<T> initList)
            : HashSet()
        {
            for (const auto& element : initList)
            {
                Add(element);
            }
        }

        template<class TIter>
        inline HashSet(const TIter& first, const TIter& last)
            : HashSet()
        {
            for (auto i = first; i != last; ++i)
            {
                Add(*i);
            }
        }

        inline ~HashSet()
        {
            for (Element* pBucket : m_Buckets)
            {
                while (pBucket)
                {
                    Element* toDelete = pBucket;
                    pBucket           = pBucket->pNext;
                    m_ElementPool.Delete(toDelete);
                }
            }
        }

        inline bool Add(const T& element)
        {
            const USize hash        = Hash<T>{}(element);
            const USize bucketIndex = hash % m_Buckets.Size();

            Element* pCurrent = m_Buckets[bucketIndex];
            while (pCurrent)
            {
                if (pCurrent->Value == element)
                {
                    return false;
                }

                pCurrent = pCurrent->pNext;
            }

            pCurrent               = m_ElementPool.New(std::move(element), m_Buckets[bucketIndex]);
            m_Buckets[bucketIndex] = pCurrent;
            return true;
        }

        inline bool Add(T&& element)
        {
            const USize hash        = Hash<T>{}(element);
            const USize bucketIndex = hash % m_Buckets.Size();

            Element* pCurrent = m_Buckets[bucketIndex];
            while (pCurrent)
            {
                if (pCurrent->Value == element)
                {
                    return false;
                }

                pCurrent = pCurrent->pNext;
            }

            pCurrent               = m_ElementPool.New(std::move(element), m_Buckets[bucketIndex]);
            m_Buckets[bucketIndex] = pCurrent;
            return true;
        }

        inline bool Erase(const T& element)
        {
            const USize hash        = Hash<T>{}(element);
            const USize bucketIndex = hash % m_Buckets.Size();

            Element* pCurrent = m_Buckets[bucketIndex];
            Element* pPrev    = nullptr;

            while (pCurrent)
            {
                if (pCurrent->Value == element)
                {
                    if (pPrev)
                    {
                        pPrev->pNext = pCurrent->pNext;
                    }
                    else
                    {
                        m_Buckets[bucketIndex] = pCurrent->pNext;
                    }

                    m_ElementPool.Delete(pCurrent);
                    return true;
                }

                pPrev    = pCurrent;
                pCurrent = pCurrent->pNext;
            }

            return false;
        }

        [[nodiscard]] inline bool Contains(const T& element) const
        {
            const USize hash        = Hash<T>{}(element);
            const USize bucketIndex = hash % m_Buckets.Size();

            Element* pCurrent = m_Buckets[bucketIndex];
            while (pCurrent)
            {
                if (pCurrent->Value == element)
                {
                    return true;
                }

                pCurrent = pCurrent->pNext;
            }

            return false;
        }

        [[nodiscard]] inline USize Size() const
        {
            return m_ElementPool.GetAllocationCount();
        }

        [[nodiscard]] inline Iterator begin()
        {
            Element* pCurrent = nullptr;
            USize bucketIndex = 0;
            for (; bucketIndex < m_Buckets.Size() && !pCurrent; ++bucketIndex)
            {
                pCurrent = m_Buckets[bucketIndex];
            }

            return Iterator{ this, bucketIndex, pCurrent };
        }

        [[nodiscard]] inline Iterator end()
        {
            return Iterator{ this, 0, nullptr };
        }

        [[nodiscard]] inline bool Empty() const
        {
            return Size() == 0;
        }
    };
} // namespace Actias
