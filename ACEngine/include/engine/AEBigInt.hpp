#pragma once

#ifndef ENGINE_BIGINT_HPP
#define ENGINE_BIGINT_HPP


#include "engine/AETypedefs.hpp"

/// The default vector of sectors reserve size.
/// Default value is 32 sectors, total of 608 digits.
/// 
/// @note Set to 0 to disable preallocation of the sectors.
constexpr std::size_t AEBI_RESERVE_SIZE = 32;

/// Macro to indicate whether to preallocate the sectors on default constructor (when no value is passed).
/// 
/// If enabled, the default constructor will preallocate the container of sectors with AEBI_RESERVE_SIZE.
/// Otherwise, preallocation will be done.
/// 
/// @attention If enabled, the default constructor could throw an exception if the memory allocation fails.
/// @note Set to 1 to enable, and 0 to disable this feature.
/// @see AEBigint::AEBigint(void)
#define AEBI_DEFAULT_CSTOR_PREALLOCATE 0

/// The type of the number cell to hold bigint values
typedef ullint AEBigintSector;

/// The type of the digit index in the bigint number
/// @note The largest int type available.
typedef ullint AEBigintDigitIndex;

#include "AEBigint_declaration.hpp"
#include "AEBigint_definition.hpp"

#endif // !ENGINE_BIGINT_HPP
