#ifndef DEFMT__BASE__H__
#define DEFMT__BASE__H__

namespace defmt {

namespace detail {

enum class type {
  none_type,
  // Integer types should go first,
  int_type,
  uint_type,
  long_long_type,
  ulong_long_type,
  int128_type,
  uint128_type,
  bool_type,
  char_type,
  last_integer_type = char_type,
  // followed by floating-point types.
  float_type,
  double_type,
  long_double_type,
  last_numeric_type = long_double_type,
  cstring_type,
  string_type,
  pointer_type,
  custom_type
};

// Maps core type T to the corresponding type enum constant.
template <typename T, typename Char>
struct type_constant : std::integral_constant<type, type::custom_type> {};

#define DEFMT_TYPE_CONSTANT(Type, constant) \
  template <typename Char>                \
  struct type_constant<Type, Char>        \
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
DEFMT_TYPE_CONSTANT(long double, long_double_type);
// DEFMT_TYPE_CONSTANT(const Char*, cstring_type);
// DEFMT_TYPE_CONSTANT(basic_string_view<Char>, string_type);
DEFMT_TYPE_CONSTANT(const void*, pointer_type);

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
enum {
  sint_set =
      set(type::int_type) | set(type::long_long_type) | set(type::int128_type),
  uint_set = set(type::uint_type) | set(type::ulong_long_type) |
             set(type::uint128_type),
  bool_set = set(type::bool_type),
  char_set = set(type::char_type),
  float_set = set(type::float_type) | set(type::double_type) |
              set(type::long_double_type),
  string_set = set(type::string_type),
  cstring_set = set(type::cstring_type),
  pointer_set = set(type::pointer_type)
};



/** A compile-time format string. */
template <typename Char, typename... Args> class basic_deformat_string {
 private:
  basic_destring_view<Char> str_;

 public:
  template <
      typename S,
      FMT_ENABLE_IF(
          std::is_convertible<const S&, basic_string_view<Char>>::value ||
          (detail::is_compile_string<S>::value &&
           std::is_constructible<basic_string_view<Char>, const S&>::value))>
  FMT_CONSTEVAL FMT_ALWAYS_INLINE basic_format_string(const S& s) : str_(s) {
    static_assert(
        detail::count<
            (std::is_base_of<detail::view, remove_reference_t<Args>>::value &&
             std::is_reference<Args>::value)...>() == 0,
        "passing views as lvalues is disallowed");
#if FMT_USE_CONSTEVAL
    if constexpr (detail::count_named_args<Args...>() ==
                  detail::count_statically_named_args<Args...>()) {
      using checker =
          detail::format_string_checker<Char, remove_cvref_t<Args>...>;
      detail::parse_format_string<true>(str_, checker(s));
    }
#else
    detail::check_format_string<Args...>(s);
#endif
  }
  basic_format_string(runtime_format_string<Char> fmt) : str_(fmt.str) {}

  FMT_ALWAYS_INLINE operator basic_string_view<Char>() const { return str_; }
  auto get() const -> basic_string_view<Char> { return str_; }
};

template <typename ...Args>
class deformat_string {



};


}


}


#endif