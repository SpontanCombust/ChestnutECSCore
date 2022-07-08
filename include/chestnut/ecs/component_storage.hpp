#pragma once

#include "sparse_set.hpp"
#include "types.hpp"

#include <typeindex>
#include <unordered_map>

namespace chestnut::ecs::internal
{
    class CComponentStorage
    {
    public:
        struct Iterator; friend Iterator;
        struct ConstIterator; friend ConstIterator;

    private:
        mutable std::unordered_map<std::type_index, void *> m_mapTypeToSparseSet;
        entityid_t m_highestId;


    public:
        CComponentStorage();


        template<typename T>
        const std::vector<int>& sparse() const noexcept;

        template<typename T>
        const std::vector<T>& dense() const noexcept;


        template<typename T>
        T& at(entityid_t id);

        template<typename T>
        const T& at(entityid_t id) const;

        
        template<typename T>
        bool empty() const noexcept;

        template<typename T>
        entitysize_t size() const noexcept;

        template<typename T>
        bool contains(entityid_t id) const noexcept;


        template<typename T>
        void clear() noexcept;

        template<typename T>
        void insert(entityid_t id, T&& arg) noexcept;

        template<typename T>
        void erase(entityid_t id) noexcept;


        Iterator begin() noexcept;
        Iterator end() noexcept;
        ConstIterator cbegin() const noexcept;
        ConstIterator cend() const noexcept;

    private:
        template<typename T>
        CSparseSet<T> &getSparseSet() const noexcept;
    };

} // namespace chestnut::ecs::internal


#include "component_storage_iterator.inl"
#include "component_storage.inl"