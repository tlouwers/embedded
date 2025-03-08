/**
 * \file    BitmaskEnum.hpp
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
 * \author  Terry Louwers (terry.louwers@fourtress.nl)
 * \version 1.1
 * \date    03-2025
 */

#ifndef BITMASK_ENUM_HPP_
#define BITMASK_ENUM_HPP_

/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include <type_traits>


/************************************************************************/
/* Macros                                                               */
/************************************************************************/
/**
 * \brief   Add this below an enum to enable bitmask operations for that
 *          struct. Do not add the ';' at the end.
 * \note    Ensure enum values are powers of two (0x01, 0x02, etc.).
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
    return static_cast<Enum> (
        static_cast<std::underlying_type_t<Enum>>(lhs) |
        static_cast<std::underlying_type_t<Enum>>(rhs)
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
    return static_cast<Enum> (
        static_cast<std::underlying_type_t<Enum>>(lhs) &
        static_cast<std::underlying_type_t<Enum>>(rhs)
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
    return static_cast<Enum> (
        static_cast<std::underlying_type_t<Enum>>(lhs) ^
        static_cast<std::underlying_type_t<Enum>>(rhs)
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
    return static_cast<Enum> (
        ~static_cast<std::underlying_type_t<Enum>>(rhs)
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
    return lhs = lhs | rhs;
}

/**
 * \brief   Operator: &=
 * \details Will perform bitwise AND operation to the enum and assign the result to the lhs.
 */
template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator &=(Enum &lhs, Enum rhs)
{
    return lhs = lhs & rhs;
}

/**
 * \brief   Operator: ^=
 * \details Will perform bitwise XOR operation to the enum and assign the result to the lhs.
 */
template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator ^=(Enum &lhs, Enum rhs)
{
    return lhs = lhs ^ rhs;
}


#endif // BITMASK_ENUM_HPP_
