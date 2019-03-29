# xo

A **lightweight** c++17 utility library. Originally intended to be a less bulky replacement for **boost**. It contains the following libraries:

## container
Library with various efficient containers and tools.

* `**xo/container/buffer_access.h`** 
* `**xo/container/circular_deque.h`** A deque stored in a ring buffer
* `**xo/container/circular_frame_buffer.h`**
* `**xo/container/container_algorithms.h`**
* `**xo/container/container_tools.h`**
* `**xo/container/dynarray.h`** An array with run-time allocation
* `**xo/container/flag_set.h`** Container type for storing flags
* `**xo/container/flat_map.h`** A map stored in a flat datastructure
* `**xo/container/label_vector.h`**
* `**xo/container/prop_node.h`** A hierarchical container that can contain any type
* `**xo/container/prop_node_tools.h`**
* `**xo/container/small_vector.h`**
* `**xo/container/sorted_vector.h`** A vector with sorted elements
* `**xo/container/storage.h`** A container for storing columns of data
* `**xo/container/storage_tools.h`**
* `**xo/container/table.h`**
* `**xo/container/view_if.h`** Iterate through elements of a cotainer that match a specific condition

## diagnose
Library for debugging, testing and performance analysis.

| Class / File | Description |
| ------------ | ----------- |
| debug_storage | Output data to a file without worrying about the file |
| profiler | Very light-weight invasive profiler that measures user-tagged function times |
| test_framework.h/cpp | Framework for unit testing |

## filesystem
A library with basic functionality found in the C++17 filesystem library.

## geometry
A library for efficient and flexible 3d geometry.

| Class / File | Description |
| ------------ | ----------- |
| angle | Class for representing angles in radians or degrees, with efficient conversion between types |
| bounding_box | Axis-aligned bounding box |
| catmull_rom | Catmull-Rom spline implementation |
| quat | Quaternion |
| vec3 | 3D vector |
| vec4 | 4D vector |
| shape | Basic shape class |

## numerical
Library for dealing with numbers.

| Class / File | Description |
| ------------ | ----------- |
| average.h |  |
| bounds |  |
| constants.h |  |
| delayer |  |
| delta |  |
| filter |  |
| math.h |  |
| polynomial |  |
| random |  |
| regression |  |
| regular_piecewise_liner_function |  |

## serialization
Library for input / output to serial streams of data.

| Class / File | Description |
| ------------ | ----------- |
| char_stream | Text parser |
| prop_node_serializer | Read / write xml, ini, or zml into prop_node |

## string
Library for dealing with strings.

| Class / File | Description |
| ------------ | ----------- |
| dictionary | Efficient mapping for type <--> string |
| pattern_matcher | Match strings using globbing (i.e. using * and ?) |
| stack_string | String that supports push / pop |
| string_cast.h | Efficiently convert strings to / from any type |
| string_tools.h | Many tools for string manipulation |

## system
System related tools such as logging and versioning.

| Class / File | Description |
| ------------ | ----------- |
| assert.h | Support for assert that can use exceptions or not |
| error_code | Class for error_codes |
| log.h | Advanced logging system |
| log_sink | Output class for logging |
| platform.h | Platform-specific stuff |
| settings | Class for storing settings |
| system_tools.h/cpp | System tools |
| version | Class for versioning |

## time
Library for dealing with time.

| Class / File | Description |
| ------------ | ----------- |
| stopwatch | Stopwatch class |
| timer | Timer class |

## utility
Several utilities that don't fit in any other library.

| Class / File | Description |
| ------------ | ----------- |
| factory | Generic factory class |
| file_resource_cache | Class that can cache a class created from a file |
| handle | Generic handle class |
| interruptible | Class that signal interrupts from other threads |
| irange | Integer range class |
| memoize | Class that remembers and reuses outcomes for computationally expensive functions |
| optional | Efficient optional class, uses sentinel values for basic types |
| types.h | Several types |
