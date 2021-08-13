#include <catch2/catch.hpp>

#include "../include/chestnut/ecs/entity_registry.hpp"

using namespace chestnut::ecs;
using namespace chestnut::ecs::internal;

TEST_CASE( "Entity registry test" )
{
    CEntityRegistry registry;

    SECTION( "Simple registering and checking" )
    {
        REQUIRE( registry.getEntityCount() == 0 );

        registry.registerEntity(1, false);
        registry.registerEntity(2, false);
        registry.registerEntity(3, false);
        registry.registerEntity(5, false);

        REQUIRE( registry.getEntityCount() == 4 );

        REQUIRE_FALSE( registry.hasEntity(0) );
        REQUIRE( registry.hasEntity(1) );
        REQUIRE( registry.hasEntity(2) );
        REQUIRE( registry.hasEntity(3) );
        REQUIRE_FALSE( registry.hasEntity(4) );
        REQUIRE( registry.hasEntity(5) );

        // try register again
        registry.registerEntity(1, true);
        registry.registerEntity(2, true);

        REQUIRE( registry.getEntityCount() == 4 );
        REQUIRE( registry.hasEntity(1) );
        REQUIRE( registry.hasEntity(2) );
    }

    SECTION( "Registering with signature and checking the signature" )
    {
        CEntitySignature signature;
        signature.add<int>();
        signature.add<char>();

        registry.registerEntity( 1, false );
        registry.registerEntity( 2, false, signature );

        REQUIRE( registry.hasEntity(1) );
        REQUIRE_NOTHROW( signature = registry.getEntitySignature(1) );
        REQUIRE( signature.isEmpty() );
        REQUIRE_NOTHROW( signature = registry.getEntitySignature(2) );
        REQUIRE_FALSE( signature.isEmpty() );
        REQUIRE( ( signature.has<int>() && signature.has<char>() ) );
    }

    SECTION( "Test exception throw when fetching signature of unregistered entity" )
    {
        REQUIRE_THROWS( registry.getEntitySignature(0) );
    }

    SECTION( "Updating registry entity" )
    {
        CEntitySignature signature;

        registry.registerEntity(2, false);

        signature = registry.getEntitySignature(2);
        REQUIRE( signature.isEmpty() ); 

        signature.add<int>();
        registry.updateEntity( 2, signature );

        signature = registry.getEntitySignature(2);
        REQUIRE_FALSE( signature.isEmpty() );
        REQUIRE( signature.has<int>() );
    }

    SECTION( "Removing entities" )
    {
        registry.registerEntity(1, false);
        registry.registerEntity(2, false);
        registry.registerEntity(4, false);
        registry.registerEntity(5, false);

        REQUIRE( registry.hasEntity(1) );
        REQUIRE( registry.hasEntity(2) );
        REQUIRE_FALSE( registry.hasEntity(3) );
        REQUIRE( registry.hasEntity(4) );
        REQUIRE( registry.hasEntity(5) );
        
        registry.removeEntity(2);
        registry.registerEntity(3, false);
        registry.removeEntity(4);
        registry.removeEntity(5);

        REQUIRE( registry.hasEntity(1) );
        REQUIRE_FALSE( registry.hasEntity(2) );
        REQUIRE( registry.hasEntity(3) );
        REQUIRE_FALSE( registry.hasEntity(4) );
        REQUIRE_FALSE( registry.hasEntity(5) );


        registry.removeAllEntities();

        REQUIRE( registry.getEntityCount() == 0 );
        REQUIRE_FALSE( registry.hasEntity(1) );
        REQUIRE_FALSE( registry.hasEntity(2) );
        REQUIRE_FALSE( registry.hasEntity(3) );
        REQUIRE_FALSE( registry.hasEntity(4) );
        REQUIRE_FALSE( registry.hasEntity(5) );
    }

    SECTION( "Checking for template entities" )
    {
        registry.registerEntity(1, false);
        registry.registerEntity(2, true);
        registry.registerEntity(3, false);
        registry.registerEntity(4, false);
        registry.registerEntity(5, true);


        REQUIRE( registry.hasEntity(1, true) );
        REQUIRE( registry.hasEntity(1, false) );

        REQUIRE( registry.hasEntity(2, true) );
        REQUIRE_FALSE( registry.hasEntity(2, false) );

        REQUIRE( registry.hasEntity(4, true) );
        REQUIRE( registry.hasEntity(4, false) );

        REQUIRE( registry.hasEntity(5, true) );
        REQUIRE_FALSE( registry.hasEntity(5, false) );


        REQUIRE( registry.getEntityCount() == 5 );


        registry.removeEntity(2);
        REQUIRE_FALSE( registry.hasEntity(2, true) );
        REQUIRE_FALSE( registry.hasEntity(2, false) );
    }
}