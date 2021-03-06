/**
 * @file types.hpp
 * @author Przemysław Cedro (SpontanCombust)
 * @brief Header file with library typedefs
 * @version 1.0
 * @date 2021-11-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef __CHESTNUT_ECS_TYPES_H__
#define __CHESTNUT_ECS_TYPES_H__

#include <cstdint> // uint16_t, uint32_t

namespace chestnut::ecs
{
    /**
     * @brief Type for the ID of the entity
     * 
     * @details Entity ID is specified as unsigned 32-bit integer.
     */
    typedef uint32_t entityid_t;
    /**
     * @brief Type for quantity of entities (the type is the same as entityid_t)
     */
    typedef entityid_t entitysize_t;

    /**
     * @brief Type for the ID of the segment in the component storage
     * 
     * @details Segment ID is specified as unsigned 16-bit integer.
     */
    typedef uint16_t segid_t;
    /**
     * @brief Type for the size of the segment in the component storage
     * 
     * @details 
     * A single segment can have the max amount of components equal to max amount of available entities.
     * Segment size is specified as unsigned 16-bit integer.
     */
    typedef entitysize_t segsize_t;

    /**
     * @brief Type for the entity of entity query
     * 
     * @details
     * Query ID is specified as unsigned 16-bit integer.
     */
    typedef uint16_t queryid_t;

} // namespace chestnut::ecs

#endif // __CHESTNUT_ECS_TYPES_H__