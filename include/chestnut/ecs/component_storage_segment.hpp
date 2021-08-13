#ifndef __CHESTNUT_ECS_COMPONENT_STORAGE_SEGMENT_H__
#define __CHESTNUT_ECS_COMPONENT_STORAGE_SEGMENT_H__

#include "types.hpp"
#include "component.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace chestnut::ecs::internal
{
    class CComponentStorageSegment_Base
    {
    protected:
        segsize m_size;

        std::vector< segsize > m_vecAvailableIndices;
        std::unordered_map< entityid, segsize > m_mapEntityIDToIndex;

    public:
        CComponentStorageSegment_Base( segsize size );
        CComponentStorageSegment_Base( const CComponentStorageSegment_Base& ) = delete; // we don't copy segments

        segsize getSize() const;
        bool isEmpty() const;
        bool isFull() const;

        // Returns the number of segment slots taken up by entity components
        segsize getTakenSlotCount() const;

        // Returns whether entity with id equal entityID has its component slotted in this segment 
        bool hasSlottedComponent( entityid entityID ) const;
    };




    template< class C >
    class CComponentStorageSegment : public CComponentStorageSegment_Base
    {
        CHESTNUT_STATIC_ASSERT_VALID_COMPONENT_CLASS(C);

    private:
        C *m_arrComponentSlots;

    public:
        CComponentStorageSegment( segsize size );
        CComponentStorageSegment( const CComponentStorageSegment& ) = delete; // we don't copy segments
        ~CComponentStorageSegment();

        
        // Returns the slotted component or null if there was no place left for it
        // If entity already has taken up a slot, no action is taken aside from returning component at that slot
        // Asserts entityID is not ENTITY_ID_INVALID
        C* tryTakeUpSlot( entityid entityID );

        // Returns the slotted component or null if there was no place left for it
        // If entity already has taken up a slot, no action is taken aside from returning component at that slot
        // Copies contents from copySrc into the slotted component
        // Asserts entityID is not ENTITY_ID_INVALID
        C* tryTakeUpSlot( entityid entityID, const C& copySrc );

        // Returns null if entity component is not slotted
        C* getSlottedComponent( entityid entityID ) const;

        // Nothing is done if entity doesn't take up any slot
        void freeSlot( entityid entityID );
        
        void clearSlots();
    };

} // namespace chestnut::ecs::internal


#include "component_storage_segment.tpp"


#endif // __CHESTNUT_ECS_COMPONENT_STORAGE_SEGMENT_H__