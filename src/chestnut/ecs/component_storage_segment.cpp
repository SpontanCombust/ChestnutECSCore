#include "chestnut/ecs/component_storage_segment.hpp"

#include <numeric> // iota

namespace chestnut::ecs::internal
{    
    CComponentStorageSegment_Base::CComponentStorageSegment_Base( segsize size ) 
    {
        m_size = size;

        m_vecAvailableIndices.resize( size );
        // fill vector with all possible slot numbers
        std::iota( m_vecAvailableIndices.rbegin(), m_vecAvailableIndices.rend(), 0 );

        m_mapEntityIDToIndex.reserve( size );
    }

    segsize CComponentStorageSegment_Base::getSize() const
    {
        return m_size;
    }

    bool CComponentStorageSegment_Base::isEmpty() const
    {
        if( m_mapEntityIDToIndex.empty() )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool CComponentStorageSegment_Base::isFull() const
    {
        if( m_vecAvailableIndices.empty() )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    segsize CComponentStorageSegment_Base::getTakenSlotCount() const
    {
        return m_size - m_vecAvailableIndices.size();
    }

    bool CComponentStorageSegment_Base::hasSlottedComponent( entityid entityID ) const
    {
        if( m_mapEntityIDToIndex.find( entityID ) != m_mapEntityIDToIndex.end() )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

} // namespace chestnut::ecs::internal
