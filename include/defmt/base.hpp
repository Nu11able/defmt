#ifndef DEFMT__BASE__H__
#define DEFMT__BASE__H__

#include <vector>
#include <string_view>
#include <memory>

#ifdef DEFMT_DEBUG
#include <iostream>
#include <typeinfo>
#endif

#include "fast_float/fast_float.h"

namespace defmt {

namespace detail {

enum class type {
  none_type,
  // Integer types should go first,
  int_type,
  uint_type,
  long_long_type,
  ulong_long_type,
  // int128_type,
  // uint128_type,
  bool_type,
  char_type,
  last_integer_type = char_type,
  // followed by floating-point types.
  float_type,
  double_type,
  // long_double_type, // fast_float does not support long double
  last_numeric_type = double_type,
  // cstring_type,
  string_type,
  // pointer_type,
  // custom_type
};

// Maps core type T to the corresponding type enum constant.

struct undeformattable {};
template <typename T>
struct type_constant : std::integral_constant<type, type::none_type> {};

#define DEFMT_TYPE_CONSTANT(Type, constant) \
  template <> struct type_constant<Type>        \
      : std::integral_constant<type, type::constant> {}

DEFMT_TYPE_CONSTANT(int, int_type);
DEFMT_TYPE_CONSTANT(unsigned, uint_type);
DEFMT_TYPE_CONSTANT(long long, long_long_type);
DEFMT_TYPE_CONSTANT(unsigned long long, ulong_long_type);
// DEFMT_TYPE_CONSTANT(int128_opt, int128_type);
// DEFMT_TYPE_CONSTANT(uint128_opt, uint128_type);
DEFMT_TYPE_CONSTANT(bool, bool_type);
// DEFMT_TYPE_CONSTANT(Char, char_type);
DEFMT_TYPE_CONSTANT(float, float_type);
DEFMT_TYPE_CONSTANT(double, double_type);
// DEFMT_TYPE_CONSTANT(long double, long_double_type);
// DEFMT_TYPE_CONSTANT(const Char*, cstring_type);
// DEFMT_TYPE_CONSTANT(basic_string_view<Char>, string_type);
// DEFMT_TYPE_CONSTANT(const void*, pointer_type);

constexpr auto is_integral_type(type t) -> bool {
  return t > type::none_type && t <= type::last_integer_type;
}
constexpr auto is_arithmetic_type(type t) -> bool {
  return t > type::none_type && t <= type::last_numeric_type;
}

constexpr auto set(type rhs) -> int { return 1 << static_cast<int>(rhs); }
constexpr auto in(type t, int set) -> bool {
  return ((set >> static_cast<int>(t)) & 1) != 0;
}

// Bitsets of types.
// enum {
//   sint_set =
//       set(type::int_type) | set(type::long_long_type) | set(type::int128_type),
//   uint_set = set(type::uint_type) | set(type::ulong_long_type) |
//              set(type::uint128_type),
//   bool_set = set(type::bool_type),
//   char_set = set(type::char_type),
//   float_set = set(type::float_type) | set(type::double_type) |
//               set(type::long_double_type),
//   string_set = set(type::string_type),
//   cstring_set = set(type::cstring_type),
//   pointer_set = set(type::pointer_type)
// };


// Maps formatting arguments to core types.
// arg_mapper reports errors by returning unformattable instead of using
// static_assert because it's used in the is_formattable trait.
struct arg_mapper {
  auto map(signed char val) -> int { return val; }
  auto map(unsigned char val) -> unsigned { return val; }
  auto map(short val) -> int { return val; }
  auto map(unsigned short val) -> unsigned { return val; }
  auto map(int val) -> int { return val; }
  auto map(unsigned val) -> unsigned { return val; }
  auto map(long val) -> long { return val; }
  auto map(unsigned long val) -> unsigned long { return val; }
  auto map(long long val) -> long long { return val; }
  auto map(unsigned long long val) -> unsigned long long {
    return val;
  }
  // auto map(int128_opt val) -> int128_opt { return val; }
  // auto map(uint128_opt val) -> uint128_opt { return val; }
  auto map(bool val) -> bool { return val; }

  // template <typename T, FMT_ENABLE_IF(std::is_same<T, char>::value)>
  // auto map(T val) -> char_type {
  //   return val;
  // }

  auto map(float val) -> float { return val; }
  auto map(double val) -> double { return val; }
  auto map(long double val) -> long double { return val; }

  auto map(void* val) -> const void* { return val; }
  auto map(const void* val) -> const void* { return val; }
  auto map(std::nullptr_t val) -> const void* { return val; }

  // template <typename T, std::size_t N,
  //           FMT_ENABLE_IF(!std::is_same<T, wchar_t>::value)>
  // auto map(const T (&values)[N]) -> const T (&)[N] {
  //   return values;
  // }

  // // Only map owning types because mapping views can be unsafe.
  // template <typename T, typename U = format_as_t<T>,
  //           FMT_ENABLE_IF(std::is_arithmetic<U>::value)>
  // auto map(const T& val) -> decltype(FMT_DECLTYPE_THIS map(U())) {
  //   return map(format_as(val));
  // }

  // template <typename T, typename U = remove_const_t<T>>
  // struct formattable : bool_constant<has_const_formatter<U, Context>() ||
  //                                    (has_formatter<U, Context>::value &&
  //                                     !std::is_const<T>::value)> {};

  // template <typename T, FMT_ENABLE_IF(formattable<T>::value)>
  // auto do_map(T& val) -> T& {
  //   return val;
  // }
  // template <typename T, FMT_ENABLE_IF(!formattable<T>::value)>
  // auto do_map(T&) -> unformattable {
  //   return {};
  // }

  // // is_fundamental is used to allow formatters for extended FP types.
  // template <typename T, typename U = remove_const_t<T>,
  //           FMT_ENABLE_IF(
  //               (std::is_class<U>::value || std::is_enum<U>::value ||
  //                std::is_union<U>::value || std::is_fundamental<U>::value) &&
  //               !has_to_string_view<U>::value && !is_char<U>::value &&
  //               !is_named_arg<U>::value && !std::is_integral<U>::value &&
  //               !std::is_arithmetic<format_as_t<U>>::value)>
  // auto map(T& val) -> decltype(FMT_DECLTYPE_THIS do_map(val)) {
  //   return do_map(val);
  // }

  // template <typename T, FMT_ENABLE_IF(is_named_arg<T>::value)>
  // auto map(const T& named_arg)
  //     -> decltype(FMT_DECLTYPE_THIS map(named_arg.value)) {
  //   return map(named_arg.value);
  // }

  auto map(...) -> undeformattable { return {}; }
};

using ptr_prototype = void*[2];

struct deformat_arg {
    type arg_type;
    std::byte arg_ptr[sizeof(ptr_prototype)];

    template<typename T>
    T* get() {
        return *reinterpret_cast<T**>(arg_ptr);
    }

    template<typename T>
    deformat_arg(T&& arg) : arg_type(type_constant<decltype(arg_mapper().map(arg))>::value) {
        using arg_ptr_t = std::add_pointer_t<std::remove_reference_t<T>>;
        new (static_cast<void*>(arg_ptr)) std::add_pointer_t<std::remove_reference_t<T>>(std::addressof(arg));
        #ifdef DEFMT_DEBUG
        std::cout << std::addressof(arg) << " " << *reinterpret_cast<arg_ptr_t*>(arg_ptr) << " "
         << typeid(arg).name() << " " << typeid(arg_ptr_t).name() << " " << std::endl;
        #endif
    }
};

fast_float::from_chars_result_t<char> deformat_parse(std::string_view view, deformat_arg& arg) {
  #ifdef DEFMT_DEBUG
    std::cout << "type:" << static_cast<int>(arg.arg_type) << std::endl;
  #endif
  switch (arg.arg_type)
  {
  case type::int_type:
  #ifdef DEFMT_DEBUG
    std::cout << arg.get<int>() << std::endl;
  #endif
    return fast_float::from_chars(view.data(), view.data() + view.size(), *arg.get<int>());
  case type::uint_type:
    return fast_float::from_chars(view.data(), view.data() + view.size(), *arg.get<unsigned>());
  case type::long_long_type:
    return fast_float::from_chars(view.data(), view.data() + view.size(), *arg.get<long long>());
  case type::ulong_long_type:
    return fast_float::from_chars(view.data(), view.data() + view.size(), *arg.get<unsigned long long>());
  // case type::int128_type:
  //   break;
  // case type::uint128_type:
  //   break;
  // case type::bool_type:
  //   break;
  // case type::char_type:
  //   break;
  case type::float_type:
    return fast_float::from_chars(view.data(), view.data() + view.size(), *arg.get<float>());
  case type::double_type:
    return fast_float::from_chars(view.data(), view.data() + view.size(), *arg.get<double>());
  // case type::long_double_type:
  //   return fast_float::from_chars(view.data(), view.data() + view.size(), *arg.get<long double>());
  case type::string_type:
    break;
  // case type::custom_typ:
    // break;

  default:
    break;
  }
  return {};
}

size_t deformat_parse_fmt_view(std::string_view fmt, std::string_view str, std::vector<std::string_view>& views) {
  size_t nums = 0;
  size_t fmt_len = fmt.size();
  size_t str_len = str.size();

  for (size_t i = 0, j = 0; i < fmt_len && j < str_len; ++i, ++j) {
    if (fmt[i] == '{') {
      size_t start = j;
      while (i < fmt_len && fmt[i] != '}') {
        ++i;
      }
      ++i; // Skip the closing '}'
      if (i == fmt_len) {
        views.push_back(str.substr(start));
        ++nums;
        break;
      }
      while (j < str_len && str[j] != fmt[i]) {
        ++j;
      }
      views.push_back(str.substr(start, j - start));
      ++nums;
    } else if (fmt[i] != str[j]) {
        break; // If characters do not match, break the loop
    }
  }

  return nums;
}

template<typename T>
deformat_arg make_deformat_arg(T&& arg) {
  return deformat_arg(arg);
}

template<size_t NUM_ARGS>
struct deformat_arg_store {
    deformat_arg de_args[NUM_ARGS];

    template<typename ...Args>
    deformat_arg_store(Args&&...args) : de_args{make_deformat_arg(std::forward<Args>(args))...} {}
};

template<typename... Args, size_t NUM_ARGS = sizeof...(Args)>
auto make_deformat_args(Args&&... args) -> deformat_arg_store<NUM_ARGS> {
    return deformat_arg_store<NUM_ARGS>(std::forward<Args>(args)...);
}



}


}


#endif