# xo

A **lightweight** c++17 utility library. Originally intended to be a less bulky replacement for **boost**. It contains the following libraries:

## container
Library with various efficient containers and tools.

* **`xo/container/buffer_access.h`** 
* **`xo/container/circular_deque.h`** A deque stored in a ring buffer
* **`xo/container/circular_frame_buffer.h`**
* **`xo/container/container_algorithms.h`**
* **`xo/container/container_tools.h`**
* **`xo/container/dynarray.h`** An array with run-time allocation
* **`xo/container/flag_set.h`** Container type for storing flags
* **`xo/container/flat_map.h`** A map stored in a flat datastructure
* **`xo/container/label_vector.h`**
* **`xo/container/prop_node.h`** A hierarchical container that can contain any type
* **`xo/container/prop_node_tools.h`**
* **`xo/container/small_vector.h`**
* **`xo/container/sorted_vector.h`** A vector with sorted elements
* **`xo/container/storage.h`** A container for storing columns of data
* **`xo/container/storage_tools.h`**
* **`xo/container/table.h`**
* **`xo/container/view_if.h`** Iterate through elements of a cotainer that match a specific condition

## diagnose
Library for debugging, testing and performance analysis.

* **`xo/diagnose/debug_storage.h`**
* **`xo/diagnose/debug_tools.h`**
* **`xo/diagnose/profiler.h`** Light-weight invasive profiler that measures user-tagged function times
* **`xo/diagnose/profiler_config.h`**
* **`xo/diagnose/test_framework.h`** Framework for unit testing

## filesystem
A library with basic functionality found in the C++17 filesystem library.

* **`xo/filesystem/filesystem.h`**
* **`xo/filesystem/path.h`**

## geometry
A library for efficient and flexible 3d geometry.

* **`xo/geometry/angle.h`** Class for representing angles in radians or degrees, with efficient conversion between types
* **`xo/geometry/angle_type.h`**
* **`xo/geometry/axes_type.h`**
* **`xo/geometry/catmull_rom.h`** Catmull-Rom spline implementation
* **`xo/geometry/dynmat.h`**
* **`xo/geometry/dynvec.h`**
* **`xo/geometry/mat33.h`**
* **`xo/geometry/mat33_type.h`**
* **`xo/geometry/path_alg.h`**
* **`xo/geometry/quat.h`**
* **`xo/geometry/quat_type.h`**
* **`xo/geometry/transform.h`**
* **`xo/geometry/vec.h`**
* **`xo/geometry/vec3.h`**
* **`xo/geometry/vec3_type.h`**
* **`xo/geometry/vec4.h`**
* **`xo/geometry/vec4_type.h`**
* **`xo/geometry/vec_alg.h`**

## numerical
Library for dealing with numbers.

* **`xo/numerical/average.h`**
* **`xo/numerical/bounded.h`**
* **`xo/numerical/bounds.h`**
* **`xo/numerical/compare.h`**
* **`xo/numerical/constants.h`**
* **`xo/numerical/delayer.h`**
* **`xo/numerical/delta.h`**
* **`xo/numerical/filter.h`**
* **`xo/numerical/functions.h`**
* **`xo/numerical/math.h`**
* **`xo/numerical/piecewise_constant_function.h`**
* **`xo/numerical/piecewise_linear_function.h`**
* **`xo/numerical/polynomial.h`**
* **`xo/numerical/random.h`**
* **`xo/numerical/regression.h`**
* **`xo/numerical/regular_piecewise_linear_function.h`**
* **`xo/numerical/result_type.h`**

## serialization
Library for input / output to serial streams of data.

* **`xo/serialization/char_stream.h`**
* **`xo/serialization/prop_node_serializer.h`**
* **`xo/serialization/prop_node_serializer_ini.h`**
* **`xo/serialization/prop_node_serializer_xml.h`**
* **`xo/serialization/prop_node_serializer_zml.h`**
* **`xo/serialization/serialize.h`**

## shape

* **`xo/shape/aabb.h`**
* **`xo/shape/box.h`**
* **`xo/shape/capsule.h`**
* **`xo/shape/cone.h`**
* **`xo/shape/cylinder.h`**
* **`xo/shape/plane.h`**
* **`xo/shape/shape.h`**
* **`xo/shape/sphere.h`**

## string
Library for dealing with strings.

* **`xo/string/dictionary.h`**
* **`xo/string/pattern_matcher.h`** Match strings using globbing (i.e. using * and ?)
* **`xo/string/stack_string.h`** String that supports push / pop
* **`xo/string/string_cast.h`** Convert strings to / from any type
* **`xo/string/string_cast_enum.h`**
* **`xo/string/string_tools.h`** Many tools for string manipulation
* **`xo/string/string_type.h`**

## system
System related tools such as logging and versioning.

* **`xo/system/assert.h`** Support for assert with optional exceptions
* **`xo/system/error_code.h`**
* **`xo/system/log.h`** Advanced logging system
* **`xo/system/log_level.h`**
* **`xo/system/log_sink.h`**
* **`xo/system/settings.h`**
* **`xo/system/system_tools.h`**
* **`xo/system/type_class.h`**
* **`xo/system/version.h`** Class for storing version numbers
* **`xo/system/xo_config.h`**

## time
Library for dealing with time.

* **`xo/time/date_time.h`**
* **`xo/time/stopwatch.h`**
* **`xo/time/time.h`**
* **`xo/time/timer.h`**
* **`xo/time/timer_v1.h`**

## utility
Several utilities that don't fit in any other library.

* **`xo/utility/color.h`**
* **`xo/utility/color_gradient.h`**
* **`xo/utility/data_algorithms.h`**
* **`xo/utility/factory.h`**
* **`xo/utility/file_resource_cache.h`** Caching instances of classes created from a file
* **`xo/utility/handle.h`**
* **`xo/utility/hash.h`**
* **`xo/utility/interpolation.h`**
* **`xo/utility/interruptible.h`**
* **`xo/utility/irange.h`**
* **`xo/utility/memoize.h`** Class that remembers and reuses outcomes for computationally expensive functions
* **`xo/utility/memory_tools.h`**
* **`xo/utility/optional.h`** Efficient optional class that uses sentinel values for integer and floating point types
* **`xo/utility/pointer_iterator.h`**
* **`xo/utility/pointer_types.h`**
* **`xo/utility/sfinae_tools.h`**
