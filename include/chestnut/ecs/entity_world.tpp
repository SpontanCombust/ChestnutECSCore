#include <type_traits>

namespace chestnut::ecs
{
    namespace internal
    {
        template< typename T >
        inline constexpr bool is_valid_component_type = !std::is_fundamental_v<T> && 
                                                         std::is_default_constructible_v<T> && 
                                                         std::is_copy_assignable_v<T>;

    } // namespace internal


    // ========================= PUBLIC ========================= //

    template< typename C, typename Traits >
    void CEntityWorld::setupComponentType()
    {
        static_assert( internal::is_valid_component_type<C>, "Given type is not a valid component type!" );

        std::type_index type = typeid(C);

        auto it = m_mapCompTypeToStorage.find( type );
        
        if( it == m_mapCompTypeToStorage.end() )
        {
            m_mapCompTypeToStorage[ type ] = new internal::CComponentStorage<C>( 
                Traits::storageSegmentSize, 
                Traits::storageInitCapacity 
            );
        }
    }

    template< typename C >
    inline void CEntityWorld::setupComponentTypeIfDidntAlready() 
    {
        setupComponentType< C, chestnut::ecs::ComponentTraits<C> >();
    }





    template< typename C >
    CComponentHandle<C> CEntityWorld::createComponent( entityid_t entityID ) 
    {
        setupComponentTypeIfDidntAlready<C>();

        internal::IComponentWrapper *uncastedComp;
        uncastedComp = createComponentInternal( std::type_index( typeid( C ) ), entityID );
        auto handle = CComponentHandle<C>( entityID, static_cast< internal::SComponentWrapper<C>* >( uncastedComp ) );

        return handle;
    }

    template< typename C >
    bool CEntityWorld::hasComponent( entityid_t entityID ) const
    {
        return hasComponentInternal( std::type_index( typeid( C ) ), entityID );
    }

    template< typename C >
    CComponentHandle<C> CEntityWorld::getComponent( entityid_t entityID ) const
    {
        internal::IComponentWrapper *uncastedComp;
        uncastedComp = getComponentInternal( std::type_index( typeid( C ) ), entityID );
        auto handle = CComponentHandle<C>( entityID, static_cast< internal::SComponentWrapper<C>* >( uncastedComp ) );

        return handle;
    }

    template< typename C >
    void CEntityWorld::destroyComponent( entityid_t entityID ) 
    {
        destroyComponentInternal( std::type_index( typeid( C ) ), entityID );
    }





    template< typename C > 
    void CEntityWorld::reserveComponentMemoryTotal( entitysize_t amount )
    {
        setupComponentTypeIfDidntAlready<C>();

        // setupComponentTypeIfDidntAlready assures this won't return null
        internal::IComponentStorage *storage = m_mapCompTypeToStorage[ typeid(C) ];

        storage->reserve( amount );
    }

    template< typename C > 
    void CEntityWorld::reserveComponentMemoryAdditional( entitysize_t amount )
    {
        setupComponentTypeIfDidntAlready<C>();

        // setupComponentTypeIfDidntAlready assures this won't return null
        internal::IComponentStorage *storage = m_mapCompTypeToStorage[ typeid(C) ];

        storage->reserveAdditional( amount );
    }

    template< typename C >
    void CEntityWorld::freeComponentMemory( entitysize_t amount )
    {
        auto it = m_mapCompTypeToStorage.find( typeid(C) );
        if( it != m_mapCompTypeToStorage.end() )
        {
            internal::IComponentStorage *storage = it->second;

            storage->resize( storage->getCapacity() - amount );
        }
    }

} // namespace chestnut::ecs
