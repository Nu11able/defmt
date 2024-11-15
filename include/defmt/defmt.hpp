#include "base.hpp"


namespace defmt {

#define DEFMT_NODISCARD //[[nodiscard]]
#define DEFMT_INLINE inline

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
template <typename... T>
DEFMT_NODISCARD DEFMT_INLINE int format(format_string<T...> fmt, T&&... args) {
  return vformat(fmt, fmt::make_format_args(args...));
}


}