#pragma once

#ifndef ENGINE_BIGINT_HPP
#define ENGINE_BIGINT_HPP

/// The default vector of sectors reserve size.
/// Default value is 32 sectors, total of 608 digits.
#define AEBI_RESERVE_SIZE 32

/// Macro to indicate whether to preallocate the sectors on default constructor (when no value is passed).
/// 
/// If enabled, the default constructor will preallocate the container of sectors with AEBI_RESERVE_SIZE.
/// Otherwise, preallocation will be done.
/// 
/// @note Set to 1 to enable, and 0 to disable this feature.
/// @see AEBigint::AEBigint(void)
#define AEBI_DEFAULT_CSTOR_PREALLOCATE 0

#include "AEBigint_declaration.hpp"
#include "AEBigint_definition.hpp"


#endif // !ENGINE_BIGINT_HPP
