#ifndef __CHESTNUT_ECS_ENTITY_WORLD_H__
#define __CHESTNUT_ECS_ENTITY_WORLD_H__

#include "types.hpp"
#include "entity_registry.hpp"
#include "entity_signature.hpp"
#include "component_storage.hpp"
#include "component_batch_guard.hpp"
#include "component_handle.hpp"
#include "entity_query.hpp"
#include "component_wrapper.hpp"
#include "component_traits.hpp"

#include <vector>

namespace chestnut::ecs
{
    class CEntityWorld
    {
    private:
        // A bookkeeping object used to keep track of created entities and their signatures
        internal::CEntityRegistry m_entityRegistry;

        // A map pointing to direct component storages by the type of the component they store
        internal::CComponentStorageTypeMap m_mapCompTypeToStorage;

        // A vector of batch objects organizing components by entity signature
        // They're mutable, because we cache pending components inside them and want to update them when performing a query
        // This doesn't affect World's state
        mutable std::vector<internal::CComponentBatchGuard> m_vecBatchGuards;

    public:
        CEntityWorld();

        CEntityWorld( const CEntityWorld& ) = delete; // we don't allow copying components

        ~CEntityWorld();


        // Traits describe how memory for this component type is managed
        // You can setup component type only once
        // Make sure to use this function before ever using this component type with other functions,
        // because they may do it automatically, but with default traits
        // To know how to write traits and for more information, see component_traits.hpp
        template< typename C, typename Traits = chestnut::ecs::ComponentTraits<C> >
        void setupComponentType();


        entityid createEntity();

        std::vector<entityid> createEntities( entitysize amount );

        bool hasEntity( entityid entityID ) const;

        void destroyEntity( entityid entityID );

        void destroyEntities( const std::vector<entityid>& entityIDs );



        // Returns null if entity doesn't exist
        // Returns existing component if it was already created before
        // Otherwise returns newly created component
        // If you haven't setup this component type before, its traits will be made default
        // You can't setup custom traits after that
        template< typename C >
        CComponentHandle<C> createComponent( entityid entityID );

        template< typename C >
        bool hasComponent( entityid entityID ) const;

        // Returns null if entity doesn't exist or if it doesn't own that component
        // Otherwise returns component owned by the entity
        template< typename C >
        CComponentHandle<C> getComponent( entityid entityID ) const;

        template< typename C >
        void destroyComponent( entityid entityID );



        entityid createTemplateEntity();

        bool hasTemplateEntity( entityid templateEntityID ) const;

        void destroyTemplateEntity( entityid templateEntityID );

        // Creates entity based on component data owned by entity template with given ID
        // If no such template entity exists doesn't create any new entity or components and returns ENTITY_ID_INVALID
        entityid createEntityFromTemplate( entityid templateEntityID );

        // Creates entity based on component data owned by entity template with given ID
        // If no such template entity exists doesn't create any new entity or components and returns empty vector
        std::vector<entityid> createEntitiesFromTemplate( entityid templateEntityID, entitysize amount );

        

        // Do so that the total of at least `amount` components of type C are allocated and available
        // Result of calling this function is dependant on the memory setup for component type
        // See component_traits.hpp
        // If you haven't setup this component type before, its traits will be made default
        // You can't setup custom traits after that
        template< typename C > 
        void reserveComponentMemoryTotal( entitysize amount );

        // Allocate `amount` components of type C
        // Result of calling this function is dependant on the memory setup for component type
        // See component_traits.hpp
        // If you haven't setup this component type before, its traits will be made default
        // You can't setup custom traits after that
        template< typename C > 
        void reserveComponentMemoryAdditional( entitysize amount );

        // Try to deallocate memory from the maximum of `amount` of components of type C
        // Result of calling this function is dependant on the memory setup for component type
        // See component_traits.hpp
        template< typename C >
        void freeComponentMemory( entitysize amount );



        // Returns the number of entity variations (and thus batches) queried
        int queryEntities(SEntityQuery &query) const;


    private:
        template< typename C >
        void setupComponentTypeIfDidntAlready();

        internal::IComponentWrapper* createComponentInternal( std::type_index compType, entityid entityID );

        bool hasComponentInternal( std::type_index compType, entityid entityID ) const;

        internal::IComponentWrapper* getComponentInternal( std::type_index compType, entityid entityID ) const;

        void destroyComponentInternal( std::type_index compType, entityid entityID );

        // Throws std::invalid_argument if signature is empty
        // We don't make batches for empty signatures, so always check it before you call this function
        // In any other case, assures batch guard exists (creates one if there isn't one already)
        internal::CComponentBatchGuard& getBatchGuardWithSignature( const CEntitySignature& signature );
    };

} // namespace chestnut::ecs

#include "entity_world.tpp"

#endif // __CHESTNUT_ECS_ENTITY_WORLD_H__