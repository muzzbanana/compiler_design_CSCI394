#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "huffman.hh"

#include <limits.h>
#include <iostream>

using namespace huffman;

TEST_CASE("All symbols start with different codes", "[diff_codes]") {
  auto huff = Huffman();
  for (unsigned i = 0; i < 256; ++i) {
    for (unsigned j = 0; i < 256; ++i) {
      if (i != j) {
        REQUIRE(huff.encode(i) != huff.encode(j));
      }
    }
  }
}

TEST_CASE("Most frequent symbol encodes shortest", "[compression]") {
  auto huff = Huffman();
  huff.incFreq(7);
  huff.incFreq(7);
  huff.incFreq(7);
  huff.incFreq(7);
  huff.incFreq(5);
  huff.incFreq(5);
  huff.incFreq(5);
  const auto len0 = huff.encode(7).size();

  for (unsigned i = 0; i < 256; ++i) {
    if (i != 7){
    REQUIRE(huff.encode(i).size() > len0);
    }
  }
}

TEST_CASE("Most frequent symbol always encodes to single bit", "[compression]") {
  auto huff = Huffman();
  huff.incFreq('b');
  huff.incFreq('b');
  REQUIRE(huff.encode('b').size() == 1);
  REQUIRE(huff.encode(1).size() > 1);
  huff.incFreq(1);
  huff.incFreq(1);
  huff.incFreq(1);
  REQUIRE(huff.encode('b').size() > 1);
  REQUIRE(huff.encode(1).size() == 1);
}

TEST_CASE("value must be the same before encoding and after decoding", "[decompression]"){
  auto huff = Huffman();
  huff.incFreq('a');
  huff.incFreq('a');
  huff.incFreq('b');
  huff.incFreq('b');
  huff.incFreq('b');
  huffman::Huffman::encoding_t h = huff.encode('a');
  huffman::Huffman::enc_iter_t b = h.begin();
  const huffman::Huffman::enc_iter_t e = h.end();
  REQUIRE(huff.decode(b, e) == unsigned('a'));
}

