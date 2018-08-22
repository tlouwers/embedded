/**
 * \file BitmaskEnum.hpp
 *
 * \licence (C) Copyright 2015 Just Software Solutions Ltd
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *
 *          Boost Software License - Version 1.0 - August 17th, 2003
 *
 *          Permission is hereby granted, free of charge, to any person or
 *          organization obtaining a copy of the software and accompanying
 *          documentation covered by this license (the "Software") to use,
 *          reproduce, display, distribute, execute, and transmit the
 *          Software, and to prepare derivative works of the Software, and
 *          to permit third-parties to whom the Software is furnished to
 *          do so, all subject to the following:
 *
 *          The copyright notices in the Software and this entire
 *          statement, including the above license grant, this restriction
 *          and the following disclaimer, must be included in all copies
 *          of the Software, in whole or in part, and all derivative works
 *          of the Software, unless such copies or derivative works are
 *          solely in the form of machine-executable object code generated
 *          by a source language processor.
 *
 *          THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
 *          KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *          WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 *          PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 *          COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE
 *          LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN
 *          CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 *          CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *          THE SOFTWARE.
 *
 * \brief   Convenience template to be able to use enum classes as type-safe
 *          bitmasks. The template allows per enum to enable this
 *          functionality.
 *
 * \note    https://github.com/tlouwers/embedded/tree/master/BitmaskEnum
 *
 * \details This code is intended to use strongly typed enum classes as
 *          bitmasks. Doing so will improve readability and reduce the
 *          chance of errors, getting feedback at compile time.
 *
 *          Some examples:
 *          // Given the enum Dummy:
 *          enum class Dummy : uint8_t
 *          {
 *              BLUE  = 0x01,   // Bit 1
 *              RED   = 0x02,   // Bit 2
 *              GREEN = 0x04,   // Bit 3
 *          };
 *          ENABLE_BITMASK_ENUM(Dummy)      // Enable bitmask functionality
 *
 *          // After the 'enable', we can use the enum as bitmask:
 *          Dummy d1 = ( Dummy::BLUE | Dummy::GREEN );      // d1: Bit 1 and 3
 *          Dummy d2 = d1 & Dummy::BLUE;                    // d2: Bit 1
 *          Dummy d3 = d1 | Dummy::RED;                     // d3: Bit 1 and 2
 *          d1 |= Dummy::RED;                               // d1: Bit 1, 2 and 3
 *          d1 &= ~Dummy::BLUE;                             // d1: Bit 2 and 3
 *
 * \note    If you happen to find an issue, and are able to provide a
 *          reproducible scenario I am happy to have a look. If you have a fix,
 *          or a refactoring that would improve the buffer please let me know
 *          so I can update the buffer.
 *
 * \note    Code was originally created by <b>Anthony Williams</b>, published here:
 *          https://www.justsoftwaresolutions.co.uk/cplusplus/using-enum-classes-as-bitfields.html
 *
 *          Later, <b>Andre Haupt</b> added a more convenient macro which makes
 *          enabling the bitmask functionality to an enum much easier. This
 *          trick is described here:
 *          http://blog.bitwigglers.org/using-enum-classes-as-type-safe-bitmasks/
 *
 *          I just grouped code mentioned and explained above into a single
 *          header file, added comments and examples.
 *
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.0
 * \date    08-2018
 */

#ifndef BITMASK_ENUM_HPP_
#define BITMASK_ENUM_HPP_

/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include <cstdbool>         // bool
#include <type_traits>      // bitmask scoped enums


/************************************************************************/
/* Macros                                                               */
/************************************************************************/
/**
 * \brief   Add this below an enum to enable bitmask operations for that
 *          struct. Do not add the ';' at the end.
 * \note    Be sure the enum has values as bitmask: 0x01, 0x02, 0x04, 0x08 etc.
 */
#define ENABLE_BITMASK_OPERATORS(x)      \
template<>                               \
struct EnableBitMaskOperators<x>         \
{                                        \
    static constexpr bool enable = true; \
};


/******************************************************************************
 * Structs                                                                    *
 *****************************************************************************/
/**
 * \brief Structure to prevent the template to be applied to all enums in the
 *        code. The macro above is needed to enable the bitmask feature to an
 *        enum.
 */
template<typename Enum>
struct EnableBitMaskOperators
{
    static constexpr bool enable = false;
};


/******************************************************************************
 * Template Methods                                                           *
 *****************************************************************************/
/**
 * \brief   Operator: |
 * \details Will perform bitwise OR operation to the enum.
 */
template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator |(Enum lhs, Enum rhs)
{
    using underlying = typename std::underlying_type<Enum>::type;
    return static_cast<Enum> (
        static_cast<underlying>(lhs) |
        static_cast<underlying>(rhs)
    );
}

/**
 * \brief   Operator: &
 * \details Will perform bitwise AND operation to the enum.
 */
template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator &(Enum lhs, Enum rhs)
{
    using underlying = typename std::underlying_type<Enum>::type;
    return static_cast<Enum> (
        static_cast<underlying>(lhs) &
        static_cast<underlying>(rhs)
    );
}

/**
 * \brief   Operator: ^
 * \details Will perform bitwise XOR operation to the enum.
 */
template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator ^(Enum lhs, Enum rhs)
{
    using underlying = typename std::underlying_type<Enum>::type;
    return static_cast<Enum> (
        static_cast<underlying>(lhs) ^
        static_cast<underlying>(rhs)
    );
}

/**
 * \brief   Operator: ~
 * \details Will perform bitwise NOT operation to the enum.
 */
template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator ~(Enum rhs)
{
    using underlying = typename std::underlying_type<Enum>::type;
    return static_cast<Enum> (
        ~static_cast<underlying>(rhs)
    );
}

/**
 * \brief   Operator: |=
 * \details Will perform bitwise OR operation to the enum and assign the result to the lhs.
 */
template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator |=(Enum &lhs, Enum rhs)
{
    using underlying = typename std::underlying_type<Enum>::type;
    lhs = static_cast<Enum> (
        static_cast<underlying>(lhs) |
        static_cast<underlying>(rhs)
    );

    return lhs;
}

/**
 * \brief   Operator: &=
 * \details Will perform bitwise AND operation to the enum and assign the result to the lhs.
 */
template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator &=(Enum &lhs, Enum rhs)
{
    using underlying = typename std::underlying_type<Enum>::type;
    lhs = static_cast<Enum> (
        static_cast<underlying>(lhs) &
        static_cast<underlying>(rhs)
    );

    return lhs;
}

/**
 * \brief   Operator: ^=
 * \details Will perform bitwise XOR operation to the enum and assign the result to the lhs.
 */
template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator ^=(Enum &lhs, Enum rhs)
{
    using underlying = typename std::underlying_type<Enum>::type;
    lhs = static_cast<Enum> (
        static_cast<underlying>(lhs) ^
        static_cast<underlying>(rhs)
    );

    return lhs;
}


#endif // BITMASK_ENUM_HPP_
