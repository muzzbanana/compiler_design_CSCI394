#define CATCH_CONFIG_MAIN
/*
 * Tree class unit tests
 * Written by Eitan Frachtenberg.
 */

#include "catch.hpp"
#include "tree.hh"
#include "vectree.hh"

using TreeType = tree::VecTree;  // Change this to test other subclasses of Tree

TEST_CASE("size is computed correctly", "[vectree]") {
    const auto tree1 = new TreeType(15);

    SECTION("leaf case") {
        REQUIRE(tree1->size() == 1);
    }

    SECTION("compound tree") {
        const auto tree2 = new TreeType(10);
        const auto tree3 = new TreeType(7, *tree1, *tree2);
        const auto tree4 = new TreeType(7, *tree3, *tree3);
        REQUIRE_THROWS(new TreeType(4, *tree2, *tree4));
        REQUIRE(tree2->size() == 1);
        REQUIRE(tree3->size() == 3);
        REQUIRE(tree4->size() == 7);

        delete tree4;
        delete tree3;
        delete tree2;
    }

    delete tree1;
}


TEST_CASE("Path is computed correctly", "[vectree]") {
    const auto tree1 = new TreeType(15);
    const auto tree2 = new TreeType(10);
    const auto tree3 = new TreeType(7, *tree1, *tree2);
    const auto tree4 = new TreeType(1);
    const auto tree5 = new TreeType(14);
    const auto tree6 = new TreeType(97);
    const auto tree7 = new TreeType(9, *tree5, *tree6); 
    const auto tree8 = new TreeType(11, *tree3, *tree7);

    REQUIRE(tree3->pathTo(15) == "L");
    REQUIRE(tree3->pathTo(10) == "R");
    REQUIRE_THROWS(tree3->pathTo(2));
    REQUIRE(tree4->pathTo(1) == "");
    REQUIRE(tree7->pathTo(97) == "R");
    REQUIRE(tree8->pathTo(10) == "LR");
    REQUIRE(tree8->pathTo(14) == "RL");
    REQUIRE_THROWS(tree8->pathTo(17));


    delete tree8;
    delete tree3;
    delete tree2;
    delete tree1;
    delete tree4;
    delete tree5;
    delete tree7;
    delete tree6;
}

TEST_CASE("getByPath follows correct path", "[vectree]") {
	const auto tree1 = new TreeType(15);
    const auto tree2 = new TreeType(10);
    const auto tree3 = new TreeType(1, *tree2, *tree1);
    const auto tree4 = new TreeType(4, *tree3, *tree3);

    REQUIRE(tree1->getByPath("") == 15);
    REQUIRE(tree3->getByPath("R") == 15);
    REQUIRE(tree3->getByPath("L") == 10);
    REQUIRE_THROWS(tree3->getByPath("L R"));
    REQUIRE(tree4->getByPath("LL") == 10);

    delete tree4;
    delete tree3;
    delete tree2;
    delete tree1;
}