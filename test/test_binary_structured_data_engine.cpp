// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// ============================================================================
// Copyright (c) 2017-2019, by Vitaly Grigoriev, <Vit.link420@gmail.com>.
// This file is part of ProtocolAnalyzer open source project under MIT License.
// ============================================================================


#include <bitset>
#include <cassert>
#include <iostream>

#include "../include/framework/Timer.hpp"
#include "FrameworkApi.hpp"

using timer = analyzer::framework::diagnostic::Timer;
using namespace analyzer::framework::common::types;

#pragma pack(push, 1)
struct Data
{
    uint32_t SequenceNumber;
    uint32_t AcknowledgmentNumber;
    uint8_t  Offset_Reserved_NS;
    uint8_t  CWR_ECE_TcpFlags;
    uint16_t WindowSize;
    uint16_t Checksum;
    uint16_t UrgentPointer;
} const tcp { 0xF0E0D0C0, 0x0C0D0E0F, 0xE5, 0x92, 0x7468, 0x5CD5, 0xACAB };
#pragma pack(pop)

const uint16_t byte_pattern[7] = { 4, 4, 1, 1, 2, 2, 2 };
const uint16_t bit_pattern[9] = { 32, 32, 4, 3, 3, 6, 16, 16, 16 };


int32_t main (int32_t size, char** data)
{
    uint16_t count = 0;
    //while (count++ < 10000)
    {
        BinaryStructuredDataEngine buffer1;
        const bool res = buffer1.Constructor().AssignStructuredData<DATA_BIG_ENDIAN>(&tcp, 1, byte_pattern, 7, true);
        assert(res == true && buffer1 == true);

        assert(buffer1.Data().BitsInformation().Any(0, 3) == true);
        assert(buffer1.Data().BitsInformation().Any(4, 7) == false);

        const BinaryDataEngine binData(reinterpret_cast<const std::byte *>(&tcp), sizeof(tcp), DATA_BIG_ENDIAN);
        ConstantBinaryStructuredDataEngine buffer(binData, byte_pattern, sizeof(byte_pattern) / sizeof(uint16_t));
        assert(binData == true && buffer == true);

        timer timer(true);
        for (uint16_t idx = 0; idx < 100; ++idx)
        {
            buffer.GetNumericalField<uint16_t>(5).value();
        }
        std::cout << timer.GetCount().NanoSeconds() / 100 << std::endl;
        timer.Reset(true);
        for (uint16_t idx = 0; idx < 100; ++idx)
        {
            buffer.GetField(5).value().To16Bit().value();
        }
        std::cout << timer.GetCount().NanoSeconds() / 100 << std::endl;
        timer.Reset(true);
        for (uint16_t idx = 0; idx < 100; ++idx)
        {
            buffer.GetFieldByReference(5).value().To16Bit().value();
        }
        std::cout << timer.PauseAndGetCount().NanoSeconds() / 100 << std::endl;


        timer.Reset(true);
        for (uint16_t idx = 0; idx < 100; ++idx)
        {
            ConstantBinaryStructuredDataEngine res1 = buffer ^ buffer1;
            assert(res1.Data().BitsInformation().Any() == false);
        }
        std::cout << std::endl << timer.PauseAndGetCount().NanoSeconds() / 100 << std::endl;
        timer.Reset(true);
        for (uint16_t idx = 0; idx < 100; ++idx)
        {
            BinaryDataEngine res2 = buffer.Data() ^ buffer1.Data();
            assert(res2.BitsInformation().Any() == false);
        }
        std::cout << timer.PauseAndGetCount().NanoSeconds() / 100 << std::endl;
        timer.Reset(true);
        for (uint16_t idx = 0; idx < 100; ++idx)
        {
            BinaryDataEngine res3 = buffer.Data().BitsInformation() ^ buffer1.Data().BitsInformation();
            assert(res3.BitsInformation().Any() == false);
        }
        std::cout << timer.PauseAndGetCount().NanoSeconds() / 100 << std::endl << std::endl;


        timer.Reset(true);
        assert(buffer.GetNumericalField<uint32_t>(0).value() == 0xC0D0E0F0);
        assert(buffer.GetNumericalField<uint32_t>(1).value() == 0x0F0E0D0C);
        assert(buffer.GetNumericalField<uint8_t>(2).value() == 0xE5);
        assert(buffer.GetNumericalField<uint8_t>(3).value() == 0X92);
        assert(buffer.GetNumericalField<uint16_t>(4).value() == 0x6874);
        assert(buffer.GetNumericalField<uint16_t>(5).value() == 0xD55C);
        assert(buffer.GetNumericalField<uint16_t>(6).value() == 0xABAC);
        std::cout << timer.PauseAndGetCount().NanoSeconds() << std::endl;

        timer.Reset(true);
        assert(buffer.GetField(0).value().To32Bit().value() == 3234914544);
        assert(buffer.GetField(1).value().To32Bit().value() == 252579084);
        assert(buffer.GetField(2).value().To8Bit().value() == 229);
        assert(buffer.GetField(3).value().To8Bit().value() == 146);
        assert(buffer.GetField(4).value().To16Bit().value() == 26740);
        assert(buffer.GetField(5).value().To16Bit().value() == 54620);
        assert(buffer.GetField(6).value().To16Bit().value() == 43948);
        std::cout << timer.PauseAndGetCount().NanoSeconds() << std::endl;

        timer.Reset(true);
        assert(buffer.GetFieldByReference(0).value().To32Bit().value() == 0xC0D0E0F0);
        assert(buffer.GetFieldByReference(1).value().To32Bit().value() == 0x0F0E0D0C);
        assert(buffer.GetFieldByReference(2).value().To8Bit().value() == 0xE5);
        assert(buffer.GetFieldByReference(3).value().To8Bit().value() == 0x92);
        assert(buffer.GetFieldByReference(4).value().To16Bit().value() == 0x6874);
        assert(buffer.GetFieldByReference(5).value().To16Bit().value() == 0xD55C);
        assert(buffer.GetFieldByReference(6).value().To16Bit().value() == 0xABAC);
        std::cout << timer.PauseAndGetCount().NanoSeconds() << std::endl;


        timer.Reset(true);
        const uint16_t offset = buffer.GetSubField<uint16_t, DATA_MODE_INDEPENDENT>(2, 0, 4).value();
        assert(offset == 14);
        assert(buffer.GetFieldBit<DATA_MODE_INDEPENDENT>(2, 0) == true);
        assert(buffer.GetFieldBit<DATA_MODE_INDEPENDENT>(2, 1) == true);
        assert(buffer.GetFieldBit<DATA_MODE_INDEPENDENT>(2, 2) == true);
        assert(buffer.GetFieldBit<DATA_MODE_INDEPENDENT>(2, 3) == false);
        assert(buffer.GetFieldBit<DATA_MODE_INDEPENDENT>(2, 4) == false);
        assert(buffer.GetFieldBit<DATA_MODE_INDEPENDENT>(2, 5) == true);
        assert(buffer.GetFieldBit<DATA_MODE_INDEPENDENT>(2, 6) == false);
        assert(buffer.GetFieldBit<DATA_MODE_INDEPENDENT>(2, 7) == true);
        std::cout << timer.PauseAndGetCount().NanoSeconds() << std::endl;

        timer.Reset(true);
        const uint16_t flags = buffer.GetSubField<uint16_t, DATA_MODE_INDEPENDENT>(3, 2, 6).value();
        assert(flags == 18);
        assert(buffer.GetFieldBit<DATA_MODE_INDEPENDENT>(3, 0) == true);
        assert(buffer.GetFieldBit<DATA_MODE_INDEPENDENT>(3, 1) == false);
        assert(buffer.GetFieldBit<DATA_MODE_INDEPENDENT>(3, 2) == false);
        assert(buffer.GetFieldBit<DATA_MODE_INDEPENDENT>(3, 3) == true);
        assert(buffer.GetFieldBit<DATA_MODE_INDEPENDENT>(3, 4) == false);
        assert(buffer.GetFieldBit<DATA_MODE_INDEPENDENT>(3, 5) == false);
        assert(buffer.GetFieldBit<DATA_MODE_INDEPENDENT>(3, 6) == true);
        assert(buffer.GetFieldBit<DATA_MODE_INDEPENDENT>(3, 7) == false);
        std::cout << timer.PauseAndGetCount().NanoSeconds() << std::endl;


        //timer Timer(true);
        for (uint16_t idx = 0; idx < 100; ++idx)
        {
            buffer.GetNonemptyFieldIndex(0, bit_pattern, 9).value();
        }
        //std::cout << Timer.GetCount().MicroSeconds() / 100 << std::endl;
        for (uint16_t idx = 0; idx < 100; ++idx)
        {
            buffer.GetNonemptyFieldIndex(1, bit_pattern, 9).value();
        }
        //std::cout << Timer.UpdateAndGetCount().MicroSeconds() / 100 << std::endl;
        for (uint16_t idx = 0; idx < 100; ++idx)
        {
            buffer.GetNonemptyFieldIndex(2, bit_pattern, 9).value();
        }
        //std::cout << Timer.UpdateAndGetCount().MicroSeconds() / 100 << std::endl;
        for (uint16_t idx = 0; idx < 100; ++idx)
        {
            buffer.GetNonemptyFieldIndex(3, bit_pattern, 9).value();
        }
        //std::cout << Timer.UpdateAndGetCount().MicroSeconds() / 100 << std::endl;
        for (uint16_t idx = 0; idx < 100; ++idx)
        {
            buffer.GetNonemptyFieldIndex(4, bit_pattern, 9).value();
        }
        //std::cout << Timer.UpdateAndGetCount().MicroSeconds() / 100 << std::endl;
        for (uint16_t idx = 0; idx < 100; ++idx)
        {
            buffer.GetNonemptyFieldIndex(5, bit_pattern, 9).value();
        }
        //std::cout << Timer.UpdateAndGetCount().MicroSeconds() / 100 << std::endl;
        for (uint16_t idx = 0; idx < 100; ++idx)
        {
            buffer.GetNonemptyFieldIndex(6, bit_pattern, 9).value();
        }
        //std::cout << Timer.UpdateAndGetCount().MicroSeconds() / 100 << std::endl;
        for (uint16_t idx = 0; idx < 100; ++idx)
        {
            buffer.GetNonemptyFieldIndex(7, bit_pattern, 9).value();
        }
        //std::cout << Timer.UpdateAndGetCount().MicroSeconds() / 100 << std::endl;
        for (uint16_t idx = 0; idx < 100; ++idx)
        {
            buffer.GetNonemptyFieldIndex(8, bit_pattern, 9).value();
        }
        //std::cout << Timer.PauseAndGetCount().MicroSeconds() / 100 << std::endl;
    }

    return EXIT_SUCCESS;
}
