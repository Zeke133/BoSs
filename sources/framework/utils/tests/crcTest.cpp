/**
 * @file    crc.cpp
 * @author  Denis Homutovski
 * @version V1.0.0
 * @date    13-04-2019
 * @brief   CRC calculation tests
 * @details   Unit tests
 * @pre       -
 * @bug       -
 * @warning   -
 * @copyright GNU Public License.
 */

/* tested source */
#include "crc.hpp"

/* testing framework */
#include <gtest/gtest.h>

/* std */
#include <vector>

namespace UnitTests {

    using namespace CRC;

    struct CrcSamples {

        unsigned char crc;
        std::vector<unsigned char> sample;
    };

    std::vector<CrcSamples> crcSamples {

        {0xBC, {0x02}},
	    {0xAF, {0x02, 0x1C}},
        {0xA2, {0x02, 0x1C, 0xB8, 0x01, 0x00, 0x00, 0x00}},
        {0x6D, {0x12, 0x12, 0xA2, 0x14, 0x5F, 0x12, 0xAA, 0x12}}
    };

    class CrcAlgorithmTester : public ::testing::TestWithParam<struct CrcSamples> {};

    TEST_P(CrcAlgorithmTester, IterativeAlgorithm) {

        auto val = GetParam();

        unsigned char crc = 0;
        for(auto sample : val.sample) {

            crc = calcCRC_1wire(crc, sample);
        }

        EXPECT_EQ(val.crc, crc);
    }

    TEST_P(CrcAlgorithmTester, TableAlgorithm) {

        auto val = GetParam();

        unsigned char crc = 0;
        for(auto sample : val.sample) {

            crc = calcCRC_1wireQuick(crc, sample);
        }

        EXPECT_EQ(val.crc, crc);
    }

    INSTANTIATE_TEST_CASE_P(Crc8AlgorithmTester, CrcAlgorithmTester,
                            ::testing::ValuesIn(crcSamples));

} // namespace UnitTests

