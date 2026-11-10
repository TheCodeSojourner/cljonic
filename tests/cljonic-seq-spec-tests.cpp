#include <catch2/catch_test_macros.hpp>

#include <concepts>
#include <type_traits>

#include "cljonic-test-api.hpp"

#define TRACE_ID(id_literal) INFO("trace-id: " id_literal)

namespace {
template <typename T>
concept HasAdmittedSeq =
    requires(const T& value) { requires !std::same_as<decltype(cljonic::core::seq(value)), void>; };
} // namespace

TEST_CASE("Seq free function operations", "[seq]") {
    using cljonic::Cycle;
    using cljonic::Iterate;
    using cljonic::Map;
    using cljonic::MapEntry;
    using cljonic::Queue;
    using cljonic::Range;
    using cljonic::Repeat;
    using cljonic::Repeatedly;
    using cljonic::Set;
    using cljonic::String;
    using cljonic::Vector;
    using cljonic::core::count;
    using cljonic::core::cycle;
    using cljonic::core::equal;
    using cljonic::core::first;
    using cljonic::core::hash_map;
    using cljonic::core::hash_set;
    using cljonic::core::map_entry;
    using cljonic::core::queue;
    using cljonic::core::repeat;
    using cljonic::core::seq;
    using cljonic::core::string;
    using cljonic::core::vector;

    TRACE_ID("entity-fields.Seq");
    STATIC_REQUIRE(cljonic::concepts::CljonicSource<Vector<int, 4>>);

    TRACE_ID("invariant.Seq.SeqFreeFunctionSupported");
    STATIC_REQUIRE(std::same_as<decltype(seq(Vector<int, 4>{1})), Vector<int, 4>>);

    TRACE_ID("invariant.Seq.SupportsSequenceableCollectionDomain");
    STATIC_REQUIRE(std::same_as<decltype(seq(Vector<int, 4>{1})), Vector<int, 4>>);
    STATIC_REQUIRE(std::same_as<decltype(seq(Map<int, int, 4>{})), Vector<MapEntry<int, int>, 4>>);
    STATIC_REQUIRE(std::same_as<decltype(seq(Set<int, 4>{})), Vector<int, 4>>);
    STATIC_REQUIRE(std::same_as<decltype(seq(Queue<int, 4>{})), Vector<int, 4>>);
    STATIC_REQUIRE(std::same_as<decltype(seq(String<4>{"AB"})), Vector<char, 4>>);

    TRACE_ID("invariant.Seq.SupportsSequenceableProducerDomain");
    STATIC_REQUIRE(std::same_as<decltype(seq(Range<int>{0, 5})),
                                Vector<int, cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE>>);
    STATIC_REQUIRE(std::same_as<decltype(seq(Repeat<int>{3, 7})),
                                Vector<int, cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE>>);
    STATIC_REQUIRE(std::same_as<decltype(seq(Cycle<Vector<int, 2>>{Vector<int, 2>{}})),
                                Vector<int, cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE>>);
    STATIC_REQUIRE(std::same_as<decltype(seq(Iterate<int, int (*)(int) noexcept>{})),
                                Vector<int, cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE>>);
    STATIC_REQUIRE(std::same_as<decltype(seq(Repeatedly<int, int (*)() noexcept>{})),
                                Vector<int, cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE>>);
    constexpr auto adapter_prefix =
        cljonic::core::take_while([](int value) noexcept { return value < 3; }, vector(1, 2, 4));
    constexpr auto adapter_suffix =
        cljonic::core::drop_while([](int value) noexcept { return value < 3; }, vector(1, 2, 4));
    STATIC_REQUIRE(std::same_as<decltype(seq(adapter_prefix)),
                                Vector<int, cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE>>);
    STATIC_REQUIRE(std::same_as<decltype(seq(adapter_suffix)),
                                Vector<int, cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE>>);

    TRACE_ID("invariant.Seq.ReturnsOwningBoundedVectorOfTraversalElements");
    STATIC_REQUIRE(equal(seq(vector(1, 2, 3)), vector(1, 2, 3)));
    STATIC_REQUIRE(equal(seq(Range{0, 5}), vector(0, 1, 2, 3, 4)));

    TRACE_ID("invariant.Seq.VectorInputReturnsByValueCopy");
    constexpr auto copied = seq(vector(1, 2));
    STATIC_REQUIRE(equal(copied, vector(1, 2)));
    STATIC_REQUIRE(std::same_as<decltype(seq(vector<int, 4>(1))), Vector<int, 4>>);
    {
        volatile int raw = 41;
        int value = raw;
        const auto source = Vector<int, 4>{value, 42};
        const auto result = seq(source);
        REQUIRE(equal(result, vector(41, 42)));
    }

    TRACE_ID("invariant.Seq.SetInputReturnsOneElementPerStoredElement");
    constexpr auto compile_time_set_result = seq(hash_set(7, 8));
    STATIC_REQUIRE((equal(compile_time_set_result, vector(7, 8)) ||
                    equal(compile_time_set_result, vector(8, 7))));
    {
        const auto stored = hash_set(7, 8, 9);
        const auto result = seq(stored);
        REQUIRE(count(result) == 3U);
        REQUIRE((equal(result, vector(7, 8, 9)) || equal(result, vector(7, 9, 8)) ||
                 equal(result, vector(8, 7, 9)) || equal(result, vector(8, 9, 7)) ||
                 equal(result, vector(9, 7, 8)) || equal(result, vector(9, 8, 7))));
    }

    TRACE_ID("invariant.Seq.QueueInputPreservesFifoOrder");
    STATIC_REQUIRE(equal(seq(queue(5, 6, 7)), vector(5, 6, 7)));
    {
        volatile int front_raw = 11;
        int front = front_raw;
        const auto stored = Queue<int, 4>{front, 12, 13};
        REQUIRE(equal(seq(stored), vector(11, 12, 13)));
    }

    TRACE_ID("invariant.Seq.StringInputReturnsVectorOfCharacters");
    STATIC_REQUIRE(equal(seq(string("AB")), vector('A', 'B')));
    {
        const auto text = string("CD");
        const auto result = seq(text);
        STATIC_REQUIRE(std::same_as<decltype(result), const Vector<char, 2>>);
        REQUIRE(equal(result, vector('C', 'D')));
    }

    TRACE_ID("invariant.Seq.MapInputReturnsValueSemanticMapEntries");
    constexpr auto compile_time_map_result = seq(hash_map(1, 100, 2, 200));
    STATIC_REQUIRE((equal(compile_time_map_result, vector(map_entry(1, 100), map_entry(2, 200))) ||
                    equal(compile_time_map_result, vector(map_entry(2, 200), map_entry(1, 100)))));
    {
        const auto stored = Map<int, int, 4>{MapEntry<int, int>{2, 200}, MapEntry<int, int>{3, 300}};
        const auto result = seq(stored);
        REQUIRE((equal(result, vector(map_entry(2, 200), map_entry(3, 300))) ||
                 equal(result, vector(map_entry(3, 300), map_entry(2, 200)))));
    }

    TRACE_ID("invariant.Seq.MapSetTraversalOrderIsNotSemanticallySignificant");
    constexpr auto unordered_map_result = seq(hash_map(1, 100, 2, 200));
    STATIC_REQUIRE((equal(unordered_map_result, vector(map_entry(1, 100), map_entry(2, 200))) ||
                    equal(unordered_map_result, vector(map_entry(2, 200), map_entry(1, 100)))));
    {
        const auto stored = hash_set(1, 2, 3);
        const auto result = seq(stored);
        REQUIRE(count(result) == 3U);
        REQUIRE((equal(result, vector(1, 2, 3)) || equal(result, vector(1, 3, 2)) ||
                 equal(result, vector(2, 1, 3)) || equal(result, vector(2, 3, 1)) ||
                 equal(result, vector(3, 1, 2)) || equal(result, vector(3, 2, 1))));
    }
    constexpr auto map_adapter_source = hash_map(1, 100, 2, 200);
    constexpr auto map_adapter_prefix =
        cljonic::core::take_while([](const auto&) noexcept { return true; }, map_adapter_source);
    constexpr auto map_adapter_suffix =
        cljonic::core::drop_while([](const auto&) noexcept { return false; }, map_adapter_source);
    STATIC_REQUIRE((equal(seq(map_adapter_prefix), vector(map_entry(1, 100), map_entry(2, 200))) ||
                    equal(seq(map_adapter_prefix), vector(map_entry(2, 200), map_entry(1, 100)))));
    STATIC_REQUIRE((equal(seq(map_adapter_suffix), vector(map_entry(1, 100), map_entry(2, 200))) ||
                    equal(seq(map_adapter_suffix), vector(map_entry(2, 200), map_entry(1, 100)))));
    constexpr auto set_adapter_source = hash_set(1, 2, 3);
    constexpr auto set_adapter_prefix =
        cljonic::core::take_while([](int) noexcept { return true; }, set_adapter_source);
    constexpr auto set_adapter_suffix =
        cljonic::core::drop_while([](int) noexcept { return false; }, set_adapter_source);
    STATIC_REQUIRE((equal(seq(set_adapter_prefix), vector(1, 2, 3)) ||
                    equal(seq(set_adapter_prefix), vector(1, 3, 2)) ||
                    equal(seq(set_adapter_prefix), vector(2, 1, 3)) ||
                    equal(seq(set_adapter_prefix), vector(2, 3, 1)) ||
                    equal(seq(set_adapter_prefix), vector(3, 1, 2)) ||
                    equal(seq(set_adapter_prefix), vector(3, 2, 1))));
    STATIC_REQUIRE((equal(seq(set_adapter_suffix), vector(1, 2, 3)) ||
                    equal(seq(set_adapter_suffix), vector(1, 3, 2)) ||
                    equal(seq(set_adapter_suffix), vector(2, 1, 3)) ||
                    equal(seq(set_adapter_suffix), vector(2, 3, 1)) ||
                    equal(seq(set_adapter_suffix), vector(3, 1, 2)) ||
                    equal(seq(set_adapter_suffix), vector(3, 2, 1))));

    TRACE_ID("invariant.Seq.ProducerInputMaterializesObservableTraversal");
    STATIC_REQUIRE(equal(seq(Range{0, 5}), vector(0, 1, 2, 3, 4)));
    STATIC_REQUIRE(equal(seq(Repeat{3U, 7}), vector(7, 7, 7)));
    STATIC_REQUIRE(equal(seq(adapter_prefix), vector(1, 2)));
    STATIC_REQUIRE(equal(seq(adapter_suffix), vector(4)));
    {
        const auto stepped = seq(Range{10, 40, 10});
        REQUIRE(equal(stepped, vector(10, 20, 30)));
    }
    {
        const auto runtime_prefix =
            cljonic::core::take_while([](int value) noexcept { return value < 3; }, vector(1, 2, 4));
        const auto runtime_suffix =
            cljonic::core::drop_while([](int value) noexcept { return value < 3; }, vector(1, 2, 4));
        REQUIRE(equal(seq(runtime_prefix), vector(1, 2)));
        REQUIRE(equal(seq(runtime_suffix), vector(4)));
    }
    volatile int runtime_first_raw = 1;
    volatile int runtime_second_raw = 2;
    volatile int runtime_third_raw = 3;
    const int runtime_first = runtime_first_raw;
    const int runtime_second = runtime_second_raw;
    const int runtime_third = runtime_third_raw;
    const auto runtime_map_adapter_source =
        hash_map(runtime_first, runtime_first * 100, runtime_second, runtime_second * 100);
    const auto runtime_map_adapter_prefix =
        cljonic::core::take_while([](const auto&) noexcept { return true; }, runtime_map_adapter_source);
    const auto runtime_map_adapter_suffix =
        cljonic::core::drop_while([](const auto&) noexcept { return false; }, runtime_map_adapter_source);
    REQUIRE((equal(seq(runtime_map_adapter_prefix),
                   vector(map_entry(runtime_first, runtime_first * 100),
                          map_entry(runtime_second, runtime_second * 100))) ||
             equal(seq(runtime_map_adapter_prefix),
                   vector(map_entry(runtime_second, runtime_second * 100),
                          map_entry(runtime_first, runtime_first * 100)))));
    REQUIRE((equal(seq(runtime_map_adapter_suffix),
                   vector(map_entry(runtime_first, runtime_first * 100),
                          map_entry(runtime_second, runtime_second * 100))) ||
             equal(seq(runtime_map_adapter_suffix),
                   vector(map_entry(runtime_second, runtime_second * 100),
                          map_entry(runtime_first, runtime_first * 100)))));
    const auto runtime_set_adapter_source = hash_set(runtime_first, runtime_second, runtime_third);
    const auto runtime_set_adapter_prefix =
        cljonic::core::take_while([](int) noexcept { return true; }, runtime_set_adapter_source);
    const auto runtime_set_adapter_suffix =
        cljonic::core::drop_while([](int) noexcept { return false; }, runtime_set_adapter_source);
    REQUIRE((equal(seq(runtime_set_adapter_prefix), vector(runtime_first, runtime_second, runtime_third)) ||
             equal(seq(runtime_set_adapter_prefix), vector(runtime_first, runtime_third, runtime_second)) ||
             equal(seq(runtime_set_adapter_prefix), vector(runtime_second, runtime_first, runtime_third)) ||
             equal(seq(runtime_set_adapter_prefix), vector(runtime_second, runtime_third, runtime_first)) ||
             equal(seq(runtime_set_adapter_prefix), vector(runtime_third, runtime_first, runtime_second)) ||
             equal(seq(runtime_set_adapter_prefix), vector(runtime_third, runtime_second, runtime_first))));
    REQUIRE((equal(seq(runtime_set_adapter_suffix), vector(runtime_first, runtime_second, runtime_third)) ||
             equal(seq(runtime_set_adapter_suffix), vector(runtime_first, runtime_third, runtime_second)) ||
             equal(seq(runtime_set_adapter_suffix), vector(runtime_second, runtime_first, runtime_third)) ||
             equal(seq(runtime_set_adapter_suffix), vector(runtime_second, runtime_third, runtime_first)) ||
             equal(seq(runtime_set_adapter_suffix), vector(runtime_third, runtime_first, runtime_second)) ||
             equal(seq(runtime_set_adapter_suffix), vector(runtime_third, runtime_second, runtime_first))));

    TRACE_ID("invariant.Seq.ProducerResultCapacityIsObservationCap");
    STATIC_REQUIRE(std::same_as<decltype(seq(Range<int>{0, 5})),
                                Vector<int, cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE>>);

    TRACE_ID("invariant.Seq.BoundedProducerContributesItsCount");
    STATIC_REQUIRE(count(seq(Repeat{3U, 7})) == 3U);
    STATIC_REQUIRE(count(seq(Range{0, 5})) == 5U);
    {
        volatile unsigned int count_raw = 2;
        unsigned int producer_count = count_raw;
        const auto finite = repeat(producer_count, 9);
        REQUIRE(count(seq(finite)) == 2U);
        REQUIRE(equal(seq(finite), vector(9, 9)));
    }

    TRACE_ID("invariant.Seq.UnboundedProducerContributesObservationCap");
    {
        const auto unbounded = seq(Repeat{7});
        REQUIRE(count(unbounded) == cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE);
        REQUIRE(first(unbounded) == 7);
        const auto zero_step = seq(Range{0, 0, 0});
        REQUIRE(count(zero_step) == cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE);
        const auto cycled = seq(cycle(vector(1, 2)));
        REQUIRE(count(cycled) == cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE);
        REQUIRE((cycled(0) == 1 && cycled(1) == 2 && cycled(2) == 1));
    }

    TRACE_ID("invariant.Seq.ResultIsIndependentlyValid");
    {
        volatile int raw = 60;
        int value = raw;
        auto source = Vector<int, 4>{value};
        const auto result = seq(source);
        source = Vector<int, 4>{99};
        REQUIRE(equal(result, vector(60)));
        REQUIRE(equal(source, vector(99)));
    }

    TRACE_ID("invariant.Seq.ResultUsesBoundedAutomaticOrStaticStorage");
    STATIC_REQUIRE(std::same_as<decltype(seq(vector(1))), Vector<int, 1>>);
    STATIC_REQUIRE(std::same_as<decltype(seq(string("A"))), Vector<char, 1>>);

    TRACE_ID("invariant.Seq.RejectsMapEntryInputWithTargetedDiagnostic");
    STATIC_REQUIRE_FALSE(HasAdmittedSeq<MapEntry<int, int>>);

    TRACE_ID("invariant.Seq.RejectsExternalCppRangesWithTargetedDiagnostic");
    STATIC_REQUIRE_FALSE(HasAdmittedSeq<int[2]>);
    STATIC_REQUIRE_FALSE(HasAdmittedSeq<int>);

    TRACE_ID("invariant.Seq.DiagnosticNamesSupportedDomains");
    STATIC_REQUIRE_FALSE(HasAdmittedSeq<MapEntry<int, int>>);

    TRACE_ID("invariant.Seq.PreservesInput");
    {
        volatile int raw = 30;
        int value = raw;
        const auto source = Vector<int, 4>{value, 31};
        const auto before = source;
        const auto result = seq(source);
        REQUIRE(equal(source, before));
        REQUIRE(equal(result, before));
    }

    TRACE_ID("invariant.Seq.NoHeapAllocation");
    STATIC_REQUIRE(noexcept(seq(vector(1))));
    STATIC_REQUIRE(noexcept(seq(Range{0, 5})));
    STATIC_REQUIRE(noexcept(seq(hash_map(1, 100))));
    STATIC_REQUIRE(noexcept(seq(string("A"))));
    STATIC_REQUIRE(noexcept(seq(Repeat{3U, 7})));

    TRACE_ID("invariant.Seq.NoRtti");
    STATIC_REQUIRE(std::same_as<decltype(seq(vector(1))), Vector<int, 1>>);

    TRACE_ID("invariant.Seq.NoExceptions");
    STATIC_REQUIRE(noexcept(seq(vector(1))));
    STATIC_REQUIRE(noexcept(seq(queue(1))));

    TRACE_ID("invariant.Seq.ConstexprCapable");
    constexpr auto compile_time_result = seq(vector(1, 2, 3));
    STATIC_REQUIRE(equal(compile_time_result, vector(1, 2, 3)));
    constexpr auto compile_time_entries = seq(hash_map(1, 100));
    STATIC_REQUIRE(first(compile_time_entries) == map_entry(1, 100));
    constexpr auto compile_time_range = seq(Range{0, 3});
    STATIC_REQUIRE(equal(compile_time_range, vector(0, 1, 2)));

    TRACE_ID("invariant.Seq.SingleThreadedExecutionModel");
    {
        volatile int raw = 5;
        int value = raw;
        const auto result = seq(vector(value));
        REQUIRE(first(result) == 5);
    }

    TRACE_ID("invariant.Seq.ReferentialTransparency");
    {
        volatile int raw = 21;
        int value = raw;
        REQUIRE(equal(seq(vector(value, 22)), seq(vector(value, 22))));
    }
}
