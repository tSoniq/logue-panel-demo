/** Unique voice card identifier.
 */
#pragma once

#include "logue_dsp.h"

namespace logue
{
    /** Return a 32 bit hash code that is unique to this voice card.
     *
     *  This is essentially a working version of osc_mcu_hash().
     */
    class VoiceID
    {
    public:

        /** Constructor. Initialises the hash.
         */
        VoiceID()
        {
#if defined(STM32F401xC) || defined(STM32F446xE)
            static constexpr const uint32_t* UniqueId0  =   (const uint32_t*)0x1FFF7a10;
            static constexpr const uint32_t* UniqueId1  =   (const uint32_t*)0x1FFF7a14;
            static constexpr const uint32_t* UniqueId2  =   (const uint32_t*)0x1FFF7a18;
            m_hash = 0x811c9dc5u;       // FN1v basis
            hash32(*UniqueId0);
            hash32(*UniqueId1);
            hash32(*UniqueId2);
#else
#warning "Unsupported platform for VoiceID() -- hash will be zero"
            m_hash = 0;
#endif
        }

        /** Return the hash.
         *
         *  @return the hash code, as UQ32 in range [0, 0xffffffff].
         */
        inline uint32_t operator()() const
        {
            return m_hash;
        }

    private:

        void hash8(uint8_t n)
        {
            m_hash ^= n;
            m_hash *= 0x01000193u;      // FN1v prime.
        }

        void hash32(uint32_t n)
        {
            hash8(uint8_t((n >> 0) & 0xffu));
            hash8(uint8_t((n >> 8) & 0xffu));
            hash8(uint8_t((n >> 16) & 0xffu));
            hash8(uint8_t((n >> 24) & 0xffu));
        }

    private:

        uint32_t m_hash;
    };


}   // namespace
