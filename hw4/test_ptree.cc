#define CATCH_CONFIG_MAIN
/*
 * Tree class unit tests
 * Written by Eitan Frachtenberg.
 */

#include "catch.hpp"
#include "tree.hh"
#include "ptretree.hh"

using TreeType = tree::PtreTree;  // Change this to test other subclasses of Tree

TEST_CASE("size is computed correctly", "[PtreTree]") {
    const auto tree1 = new TreeType(15);
    const auto tree6 = new TreeType(7, *tree1, *tree1);

    SECTION("leaf case") {
        REQUIRE(tree1->size() == 1);
    }

    SECTION("compound tree") {
        const auto tree2 = new TreeType(10);
        const auto tree3 = new TreeType(7, *tree1, *tree2);
        const auto tree4 = new TreeType(10, *tree3, *tree3);
        
        REQUIRE(tree2->size() == 1);
        REQUIRE(tree3->size() == 3);
        REQUIRE(tree4->size() == 7);

        delete tree4;
        delete tree3;
        delete tree2;
    }

    SECTION("imbalanced trees") {
        const auto tree7 = new TreeType(5, *tree1, *tree6);
        const auto tree8 = new TreeType(14, *tree1, *tree7);
        const auto tree9 = new TreeType(3, *tree6, *tree7);

        REQUIRE(tree7->size() == 5);
        REQUIRE(tree8->size() == 7);
        REQUIRE(tree9->size() == 9);

        delete tree9;
        delete tree8;
        delete tree7;
    }
    
    delete tree6;
    delete tree1;
}


// TEST_CASE("Path is computed correctly", "[PtreTree]") {
//     const auto tree1 = new TreeType(15);
//     const auto tree2 = new TreeType(10);
//     const auto tree3 = new TreeType(7, *tree1, *tree2);
//     const auto tree4 = new TreeType(1);
//     const auto tree5 = new TreeType(97);
//     const auto tree6 = new TreeType(15, *tree5, *tree4);
//     const auto tree7 = new TreeType(8, *tree6, *tree3);
    

//     REQUIRE(tree3->pathTo(15) == "L");
//     REQUIRE(tree3->pathTo(10) == "R");
//     REQUIRE_THROWS(tree3->pathTo(2));
//     REQUIRE(tree4->pathTo(1) == "");
//     REQUIRE(tree7->pathTo(10) == "RR");
//     REQUIRE(tree7->pathTo(1) == "LR");


//     delete tree7;
//     delete tree6;
//     delete tree5;
//     delete tree4;
//     delete tree3;
//     delete tree2;
//     delete tree1;
    
// }

TEST_CASE("getByPath follows correct path", "[PtreTree]") {
	const auto tree1 = new TreeType(15);
    const auto tree2 = new TreeType(10);
    const auto tree3 = new TreeType(1, *tree2, *tree1);

    REQUIRE(tree1->getByPath("") == 15);
    REQUIRE(tree3->getByPath("R") == 15);
    REQUIRE(tree3->getByPath("L") == 10);
    REQUIRE(tree3->getByPath("LLL") == 10);
    REQUIRE(tree3->getByPath("RRR") == 15);

    delete tree3;
    delete tree2;
    delete tree1;
}