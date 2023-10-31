#pragma once
#include <Actias/Base/Byte.hpp>
#include <Actias/Memory/Object.hpp>

namespace Actias
{
    class IBlob : public IObject
    {
    public:
        ACTIAS_RTTI_Class(IBlob, "782F2594-3407-4464-A5FB-88D89E644863");

        //! \brief Get a pointer to the buffer data.
        [[nodiscard]] virtual Byte* ACTIAS_ABI Data() = 0;

        //! \brief Get size of the blob in bytes.
        [[nodiscard]] virtual USize ACTIAS_ABI ByteSize() const = 0;
    };
} // namespace Actias
