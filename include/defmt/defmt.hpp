#include "base.hpp"

namespace defmt {

#define DEFMT_NODISCARD //[[nodiscard]]
#define DEFMT_INLINE inline


template <size_t N>
DEFMT_NODISCARD DEFMT_INLINE fast_float::from_chars_result_t<char> vdeformat(std::string_view fmt, std::string_view str, detail::deformat_arg_store<N> args) {
  std::vector<std::string_view> views;
  size_t num = detail::deformat_parse_fmt_view(fmt, str, views);
  for (size_t i = 0; i < num; ++i) {
    detail::deformat_parse(views[i], args.de_args[i]);
  }
  return {};
}


/**
  \rst
  Deformats ``args`` according to specifications in ``fmt`` and returns the result
  as an error code, if success then error is 0 oherwise error code is a specific number, see more detail in Enum DefmtError.

  **Example**::

    #include <defmt/deformat.hpp>
    string str{"this is a test num 123456."};
    int num = 0;
    int code = defmt::deformat("The answer is a test num {}.", str, num);
    cout << "num is: " << num << endl; // num is: 123456
  \endrst
*/
template <typename... Args>
DEFMT_NODISCARD DEFMT_INLINE fast_float::from_chars_result_t<char> deformat(std::string_view fmt, std::string_view str, Args&&... args) {
  // static_assert((!std::is_lvalue_reference_v<Args> || ...), "deformat arguments must not be lvalues");
  return vdeformat(fmt, str, detail::make_deformat_args(std::forward<Args>(args)...));
}


}