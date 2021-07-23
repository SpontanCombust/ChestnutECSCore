#include "chestnut/ecs/entity_signature.hpp"

#include <sstream>

namespace chestnut::ecs
{
    bool CEntitySignature::isEmpty() 
    {
        return m_setComponentTindices.empty();
    }

    CEntitySignature& CEntitySignature::operator+=( const CEntitySignature& other ) 
    {
        for( auto tindex : other.m_setComponentTindices )
        {
            m_setComponentTindices.insert( tindex );
        }

        return *this;
    }

    CEntitySignature& CEntitySignature::operator-=(const CEntitySignature& other) 
    {
        for( auto tindex : other.m_setComponentTindices )
        {
            m_setComponentTindices.erase( tindex );
        }

        return *this;
    }

    CEntitySignature operator+( const CEntitySignature& lhs, const CEntitySignature& rhs )
    {
        CEntitySignature newSign = lhs;

        newSign += rhs;

        return newSign;
    }

    CEntitySignature operator-( const CEntitySignature& lhs, const CEntitySignature& rhs )
    {
        CEntitySignature newSign = lhs;

        newSign -= rhs;

        return newSign;
    }

    bool operator==( const CEntitySignature& lhs, const CEntitySignature& rhs )
    {
        return lhs.m_setComponentTindices == rhs.m_setComponentTindices;
    }

} // namespace chestnut::ecs
