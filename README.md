# xo

**xo** is a lightweight utility library for c++, intended to be a less bulky replacement for boost. It contains the following libraries:

## container
Library with various efficient containers and tools.
| Class / File | Description |
| ------------ | ----------- |
| circular_deque | A deque stored in a ring buffer |
| dynarray | An array with run-time allocation |
| flag_set | A set for enum-based flags |
| flat_map | A map stored in an array-like datastructure |
| local_vector | A vector stored on the stack |
| prop_node | A hierarchical container that can contain any type |
| sorted_vector | A vector with sorted elements |
| container_tools.h | Various algorithms that operate on containers |
| storage | A table with data |
| view_if | Iterate through a vector based on specific condition |

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
Several utilities.
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
