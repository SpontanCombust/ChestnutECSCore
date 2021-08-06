#ifndef __CHESTNUT_COMPONENT_STORAGE_H__
#define __CHESTNUT_COMPONENT_STORAGE_H__

#include "types.hpp"
#include "component.hpp"
#include "component_storage_segment.hpp"

#include <deque>
#include <typeindex>
#include <unordered_map>

namespace chestnut
{
    /**
     * @brief Interface for CComponentStorage.
     */
    class IComponentStorage
    {
    protected:
        IComponentStorage() = default;

    public:
        virtual ~IComponentStorage() = default;

        virtual segsize getSegmentSize() const = 0;

        // Returns created component or component that has already existed with that entityID
        virtual CComponent *storeComponent( entityid entityID ) = 0;
        virtual bool hasComponent( entityid entityID ) const = 0;
        // Returns null if component doesn't exists
        virtual CComponent *getComponent( entityid entityID ) const = 0;
        virtual void eraseComponent( entityid entityID ) = 0;
        // Erases all stored components; doesn't modify capacity
        virtual void clearComponents() = 0;

        // Allocates (if necessary) memory for the minimal number of segments 
        // that can fit in total specified number of components.
        // If specified totalSize is smaller than current capacity nothing is done.
        virtual void reserve( segsize totalSize ) = 0;
        // Allocates additional memory for specified amount of components
        // The amount of memory allocated also depends on the size of segment setting
        virtual void reserveAdditional( segsize additionalSize ) = 0;
        // Similair to reserve() with an exception that it attempts to deallocate memory
        // if specified targetSize is smaller than capacity 
        virtual void resize( segsize targetSize ) = 0;
        // Reduces the capacity by deallocating all segments that don't store any active components at the moment
        virtual void removeEmptySegments() = 0;

        // Returns the number of stored components
        virtual segsize getSize() const = 0;
        // Returns the total number of components that can be stored 
        virtual segsize getCapacity() const = 0;
        // Returns collective size of empty segments
        virtual segsize getEmptySegmentTotalSize() const = 0;   
    };

    /**
     * @brief Component storage class keeping its data in constant size segments
     * 
     * @details
     * Can store components in a series of contiguous segments. Segments between each other are not necesairly contiguous.
     * Memory for these segments never has to be moved. It only needs to be allocated and deallocated (during storage's 
     * lifetime this is optional).
     * 
     * Segmenting the storage allows for relative balance between speed and being able to access data using pointers.
     * The problem with using simple vector for this comes with that last part. Let's take simple situation:
     * at the beginning of the tick user decides to cache some component pointer. During the tick user creates a lot
     * of components. If the amount of these new ones exceeds the capacity of the vector the memory has to be reallocated
     * for bigger size. This can invalidate the pointer we talked about. This does not need to happen accross the whole tick.
     * You can simply have a situation where one thread is caching component and the other is creating them.
     * I decided to do it this way and focus on making sure that important objects like component batches will have their contents modified 
     * whenever component creation/eraseure happens. Using indices would be safer, but would require more preperation.
     * 
     * Using direct pointers is also more user friendly and transparent when it comes to code outside of systems, like scripts in my opinion. 
     * The only thing to remember is that user is not the *direct* owner of components themselves, so it's safer to repeat fetching components
     * (and checking if they're valid) from these places on every tick rather than clinging onto them for an unspecified amount of time. 
     * 
     * User does not need to care how the data is stored - all he/she cares about is that no weird stuff will happen to pointers they cached during a tick
     * if they decide to create some components and forget to explicitly preallocate more memory before that.
     * In practice this allows for safely caching pointers not just for the duration of a tick, but for the entire lifetime 
     * of the component.
     * 
     * The segment size set for the storage depends solely on how frequently and in which quantity components of given type have to be created.
     * You can even specify a very large segment size in the constructor and allocate memory for components only once, and if this amount
     * happens to run out anyways, nothing too bad will happen anyways.
     * When specifying segment size strike a balance between how big it will be and in which quantities will be created. It general it's best
     * to keep the number of segments that will have to be created to minimum, but at the same time think not to make these too big to be able
     * to load them from memory more easily.
     */
    template< class C >
    class CComponentStorage : public IComponentStorage
    {
        ASSERT_DERIVED_FROM_COMPONENT(C);

    private:
        using SegType = internal::CComponentStorageSegment<C>;

        segsize m_segmentSize;
        segid m_segmentIDCounter;

        std::unordered_map< segid, SegType * > m_mapSegmentIndexToSegment;
        std::deque< segid > m_dequeAvailableSegmentIndices;

    public:
        CComponentStorage( segsize segmentSize, segsize initCapacity );
        CComponentStorage( CComponentStorage& ) = delete; // no copying allowed
        ~CComponentStorage();

        segsize getSegmentSize() const override;

        // Returns created component or component that has already existed with that entityID,
        CComponent *storeComponent( entityid entityID ) override;
        bool hasComponent( entityid entityID ) const override;
        // Returns null if component doesn't exists
        CComponent *getComponent( entityid entityID ) const override;
        void eraseComponent( entityid entityID ) override;
        // Erases all stored components; doesn't modify capacity
        void clearComponents() override;

        // Allocates (if necessary) memory for the minimal number of segments 
        // that can fit in total specified number of components.
        // If specified totalSize is smaller than current capacity nothing is done.
        void reserve( segsize totalSize ) override;
        // Allocates additional memory for the minimal number of segments 
        // that can fit specified additional number of components.
        void reserveAdditional( segsize additionalSize ) override;
        // Similair to reserve() with an exception that it attempts to deallocate memory
        // if specified targetSize is smaller than capacity 
        void resize( segsize targetSize ) override;
        // Reduces the capacity by deallocating all segments that don't store any active components at the moment
        void removeEmptySegments() override;

        // Returns the number of stored components
        segsize getSize() const override;
        // Returns the total number of components that can be stored 
        segsize getCapacity() const override;
        // Returns collective size of empty segments
        segsize getEmptySegmentTotalSize() const override;

    private:
        // Returns index of that segment
        segid createNewSegment();
    };


    typedef std::unordered_map< std::type_index, IComponentStorage * > CComponentStorageTypeMap;


} // namespace chestnut


#include "component_storage.tpp"


#endif // __CHESTNUT_COMPONENT_STORAGE_H__