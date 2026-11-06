#pragma once

#include <cljonic-config.hpp>

/** \mainpage
 * \anchor Core_Cheatsheet
 * # Cheatsheet
 *
 * ## Aliases
 *
 * Namespace                      | C++ Namespace Alias
 * ------------------------------ | ---------------------------------------------
 * \ref Namespace_Core "Core"               | namespace **core** = cljonic::core;
 * \ref Namespace_Set "Set Library"          | namespace **st** = cljonic::lib_set;
 * \ref Namespace_String "String Library"    | namespace **s** = cljonic::lib_string;
 *
 * ## Collection Types
 *
 * - \ref Map "Map", \ref MapEntry "MapEntry"
 * - \ref Queue "Queue"
 * - \ref Set "Set"
 * - \ref String "String"
 * - \ref Vector "Vector"
 *
 * ## Producer Types
 *
 * - \ref Cycle "Cycle"
 * - \ref DropWhile "DropWhile"
 * - \ref Iterate "Iterate"
 * - \ref Range "Range"
 * - \ref Repeat "Repeat"
 * - \ref Repeatedly "Repeatedly"
 * - \ref TakeWhile "TakeWhile"
 *
 * ## Variant Type
 *
 * - \ref Variant "Variant"
 *
 * ## Core Functions
 *
 * Every Core function is declared in \c cljonic::core. Entries linked to documentation have implemented documentation.
 * Plain names are planned entries that are not yet implemented.
 *
 * - add, apply, \ref Assoc "assoc", assoc_in
 * - bit_and, bit_and_not, bit_clear, bit_flip, bit_not, bit_or, bit_set, bit_shift_left, bit_shift_right, bit_test,
 *   bit_xor, butlast
 * - can_add, \ref CanAssoc "can_assoc", \ref CanConj "can_conj", can_multiply, can_subtract,
 *   \ref CharacterIsValid "character_is_valid", comp, compare, complement, \ref Conj "conj", constantly,
 *   \ref Contains "contains", \ref Count "count", \ref Cycle "cycle"
 * - dec, dedupe, deref, \ref Disj "disj", \ref Dissoc "dissoc", distinct, div, \ref Drop "drop", drop_last, \ref DropWhile "drop_while"
 * - emplace, \ref Empty "empty", \ref Equal "equal", every_pred
 * - ffirst, \ref FilterFunction "filter", \ref FiltervFunction "filterv", find, \ref First "first",
 *   \ref FitsInto "fits_into", fits_print, flatten, fnext, fnil, frequencies
 * - \ref Get "get", get_if, get_in, greater, greater_equal, group_by
 * - \ref HasFirst "has_first", \ref HasNext "has_next", has_re_find, has_re_matches, \ref HasRest "has_rest", has_some,
 *   \ref HashMapFactory "hash_map", \ref HashSetFactory "hash_set", holds
 * - identity, inc, index, interleave, interpose, \ref Into "into", \ref IsEmpty "is_empty", is_even, is_every,
 *   is_negative, is_odd, is_positive, is_zero, \ref Iterate "iterate"
 * - juxt
 * - keep, keep_indexed, keys
 * - less, less_equal
 * - make_keyword_enum_name_map, \ref MapFunction "map", \ref MapEntryFactory "map_entry", map_indexed, mapcat, mapv, max, max_key,
 *   merge, merge_with, min, min_key, mod, mult
 * - negate, \ref Next "next", nfirst, nnext, not_any, \ref NotEmpty "not_empty", \ref NotEqual "not_equal",
 *   not_every, nth, nthnext, nthrest
 * - partial, partition, partition_all, partition_by, partitionv, partitionv_all, \ref Peek "peek", \ref Pop "pop",
 *   pr_str, print_to, prn_str
 * - \ref QueueFactory "queue", quot
 * - \ref RangeFactory "range", re_find, re_groups, re_matcher, re_matches, re_pattern, re_seq,
 *   \ref ReduceFunction "reduce", reduce_kv, reductions, rem, remove, \ref RepeatFactory "repeat", \ref Repeatedly "repeatedly",
 *   replace, replicate, reset, reset_vals, \ref Rest "rest", rseq
 * - second, select_keys, \ref Seq "seq", some, some_fn, sort, sort_by, split_at, split_with, str, \ref StringFactory "string",
 *   sub, subvec, swap, swap_vals
 * - \ref Take "take", take_last, take_nth, \ref TakeWhile "take_while", to_byte, to_char, to_double, to_float, to_int, to_long, to_num, to_short,
 *   tree_seq
 * - unsigned_bit_shift_right, update, update_in, update_keys, update_vals
 * - vals, \ref VariantFactory "variant", variant_alternative, variant_size, vec, \ref cljonic::vector "vector", visit
 * - zipmap
 *
 * ## Set Library Functions
 *
 * Plain names are planned entries that are not yet implemented.
 *
 * - fits_join, fits_set_algebra
 * - index, is_subset, is_superset
 * - join
 * - map_invert
 * - project
 * - rename, rename_keys
 * - select
 * - set_difference, set_intersection, set_union
 *
 * ## String Library Functions
 *
 * Plain names are planned entries that are not yet implemented.
 *
 * - can_parse_bool, can_parse_float, can_parse_int, capitalize
 * - ends_with, escape
 * - includes, index_of, is_blank
 * - join
 * - last_index_of, lower_case
 * - parse_bool, parse_float, parse_int
 * - re_quote_replacement, replace, replace_first, reverse
 * - split, split_lines, starts_with
 * - trim, trim_newline, triml, trimr
 * - upper_case
 */

/** \anchor Namespace_Core
 * The \b Core namespace provides the vast majority of \b cljonic functions. The current Core surface provides the
 * bounded collections and the active producers, and its free-function collection operations are introduced module by
 * module as their requirements are approved.
 */

#include <cljonic-assoc.hpp>
#include <cljonic-can-assoc.hpp>
#include <cljonic-can-conj.hpp>
#include <cljonic-character-is-valid.hpp>
#include <cljonic-collection-map.hpp>
#include <cljonic-collection-queue.hpp>
#include <cljonic-collection-set.hpp>
#include <cljonic-collection-string.hpp>
#include <cljonic-collection-variant.hpp>
#include <cljonic-collection-vector.hpp>
#include <cljonic-concepts.hpp>
#include <cljonic-conj.hpp>
#include <cljonic-contains.hpp>
#include <cljonic-core-collection-maximum-element-count.hpp>
#include <cljonic-count.hpp>
#include <cljonic-disj.hpp>
#include <cljonic-dissoc.hpp>
#include <cljonic-drop.hpp>
#include <cljonic-empty.hpp>
#include <cljonic-equal.hpp>
#include <cljonic-filter.hpp>
#include <cljonic-first.hpp>
#include <cljonic-fits-into.hpp>
#include <cljonic-get.hpp>
#include <cljonic-has-first.hpp>
#include <cljonic-has-next.hpp>
#include <cljonic-has-rest.hpp>
#include <cljonic-into.hpp>
#include <cljonic-is-empty.hpp>
#include <cljonic-map.hpp>
#include <cljonic-next.hpp>
#include <cljonic-not-empty.hpp>
#include <cljonic-not-equal.hpp>
#include <cljonic-peek.hpp>
#include <cljonic-pop.hpp>
#include <cljonic-producer-cycle.hpp>
#include <cljonic-producer-drop-while.hpp>
#include <cljonic-producer-iterate.hpp>
#include <cljonic-producer-range.hpp>
#include <cljonic-producer-repeat.hpp>
#include <cljonic-producer-repeatedly.hpp>
#include <cljonic-producer-take-while.hpp>
#include <cljonic-reduce.hpp>
#include <cljonic-rest.hpp>
#include <cljonic-seq.hpp>
#include <cljonic-take.hpp>

namespace cljonic::core {}
