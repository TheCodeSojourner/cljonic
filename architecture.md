# cljonic - System Architecture

## Current Scope

λ current_scope(x). implemented_value_domain(x) ≡ Vector ∧ MapEntry ∧ Map ∧ Set ∧ Queue ∧ String ∧ Variant ∧ Range ∧ Repeat ∧ Cycle ∧ Iterate ∧ Repeatedly ∧ TakeWhile ∧ DropWhile
  ∧ direct_construction(x) ∧ member_observation(x)
  ∧ vector_factory(x) ∧ hash_map_factory(x) ∧ map_entry_factory(x) ∧ set_factory(x)
  ∧ queue_factory(x) ∧ string_factory(x) ∧ variant_factory(x)
  ∧ producer_factories(range ∧ repeat ∧ cycle ∧ iterate ∧ repeatedly ∧ take_while ∧ drop_while)(x)
  ∧ primitive_free_functions(x) ∧ producer_materialization(into ∧ preflight(fits_into))(x)
  | collection_callable_lookup(x) → Vector ∨ Map ∨ Set ∨ String
  | module3(x) → concrete_array_backed_bounded_types(x)
    ∧ contiguous_storage_strategies(x) ∧ linear_scan_lookup(x)
    ∧ swap_and_remove_policies(x) ∧ primitive_free_functions(x)
  | cpp_interoperability(x) → require(ConstRangeTraversal(x) ∧ ReadOnlyInteropAccessor(x))
    | Queue(x) → require(LogicalTraversalOrder(x)) ∧ ¬require(ContiguousConstView(x))
  | module4_active_slice(x) → Range(x) ∧ Repeat(x) ∧ Cycle(x) ∧ Iterate(x) ∧ Repeatedly(x) ∧ into(x) ∧ fits_into(x)
  | sequence_conversion(x) → seq(x) ∈ current_collection_api(x) under REQ-SEQ-010
  | stored_collection_building_blocks(x) → govern(higher_order_algorithms(x))
  | active_higher_order_algorithms(x) ≡ map ∧ reduce ∧ filter ∧ filterv ∧ take ∧ drop ∧ take_while ∧ drop_while
  | future_expansion(transformations_except(map ∧ reduce ∧ filter ∧ filterv ∧ take ∧ drop ∧ take_while ∧ drop_while) ∨ numeric_algorithms ∨ callable_adapters ∨ set_algebra
      ∨ relational_models ∨ regexes ∨ formatting ∨ KeywordEnumNameMapping ∨ Atom)
    → describe(approved_future_expansion) ∧ remain_inactive_until(module_propagated(x))

λ current_concept_model(x). two_level_concept_model(x) → gate(current_public_collection_surface ∧ current_public_producer_surface ∧ current_free_function_surface)
  | CollectionConcept_layer(x) → admit(ClosedNominalCollectionDomain)
  | ProducerConcept_layer(x) → admit(CljonicProducer)
  | CapabilityConcept_layer(x) → express(indexed ∨ lookup ∨ associative ∨ conjable semantic_capability)
  | model_active(x) → true_for(current_value_surface(x))

λ approved_predicate_producer_scope(x). contract_approved(TakeWhile ∧ DropWhile)(x)
  | requirements_binding(REQ-FN-039 ∧ REQ-FN-040 ∧ REQ-FN-041)
  | separate_human_implementation_approval_and_verification_per_operation = satisfied
  | include_in(implemented_value_domain ∧ current_public_producer_surface)
  | ¬include_in(current_public_collection_surface) ∧ preserve(conditional_collection_element_storage)

## S5 - Identity

The embedded-first policy keeps resource bounds, deterministic outcomes, and value ownership auditable on constrained targets; weakening those guarantees would change cljonic's purpose.

λ S5_identity_intro(x). identity(x) ≡ embedded_first ∧ HeaderOnlyDistribution ∧ C++23 ∧ functional_collection_system
  | identity(x) → bounded(x) ∧ deterministic_by_default(x) ∧ semantically_clear(x)
  | preserve_identity(x) → CopyOnModifyCollection(x) ∧ SentinelBasedAccess(x) ∧ ProbeFirstAccess(x) ∧ strict_resource_determinism(x)

λ S5_identity(x). embedded_first(x) ∧ determinism_by_default(x) ∧ fixed_capacity_value_semantics(x)
  | cljonic_code(x) → ¬heap_allocation(x) ∧ ¬heap_deallocation(x) ∧ ¬throw_exceptions(x)
  | user_code(x) → assume(¬heap_allocation(x) ∧ ¬heap_deallocation(x) ∧ ¬throw_exceptions(x))
    ∧ application_obligation(x) ∧ human_governed(x) ∧ not_generally_mechanically_verifiable(x)
    ∧ ¬claim(general_compile_time_verification_of_user_code(x))
  | canonical_vocabulary_governs(x)

λ S5_failure(x). violate(NoHeapConstraint) ∨ violate(NoExceptionConstraint)
  | violate(S5_authorized_effect_policy)
  | violate(CopyOnModifyCollection)
  → not_cljonic_identity(x)

λ S5_values(x). clarity(x) ∧ predictability(x) ∧ safety(x)
  | clojure_parity_when_constraints_allow(x)

λ S5_vector_creation_policy(x). user_facing_Vector_creation(x) → prefer(lowercase_vector_factory(x))
  | runtime_creation(x) ∧ constant_evaluation_creation(x) → same_factory_surface(x)
  | preserve(public_Vector_type ∧ existing_Vector_constructors)
    ∧ direct_external_cpp_range_or_view_import(x) → retain(SourceConstruction(x))
  | trace_to(REQ-FN-029A)

λ S5_map_creation_policy(x). user_facing_Map_creation(x) → prefer(lowercase_hash_map_factory(x))
  | runtime_creation(x) ∧ constant_evaluation_creation(x) → same_factory_surface(x)
  | preserve(public_Map_type ∧ existing_Map_constructors)
    ∧ direct_external_cpp_range_or_view_import(x) → retain(SourceConstruction(x))
  | trace_to(REQ-FN-029D)

λ S5_map_entry_creation_policy(x). user_facing_MapEntry_creation(x) → prefer(lowercase_map_entry_factory(x))
  | runtime_creation(x) ∧ constant_evaluation_creation(x) → same_factory_surface(x)
  | preserve(public_MapEntry_type ∧ existing_MapEntry_constructors)
  | trace_to(REQ-FN-029E)

λ S5_set_creation_policy(x). user_facing_Set_creation(x) → prefer(lowercase_hash_set_factory(x))
  | runtime_creation(x) ∧ constant_evaluation_creation(x) → same_factory_surface(x)
  | preserve(public_Set_type ∧ existing_Set_constructors)
    ∧ direct_external_cpp_range_or_view_import(x) → retain(SourceConstruction(x))
  | trace_to(REQ-FN-029F)

λ S5_queue_creation_policy(x). user_facing_Queue_creation(x) → prefer(lowercase_queue_factory(x))
  | runtime_creation(x) ∧ constant_evaluation_creation(x) → same_factory_surface(x)
  | preserve(public_Queue_type ∧ existing_Queue_constructors)
    ∧ direct_external_cpp_range_or_view_import(x) → retain(SourceConstruction(x))
  | trace_to(REQ-FN-029G)

λ S5_string_creation_policy(x). user_facing_String_creation(x) → prefer(lowercase_string_factory(x))
  | runtime_creation(x) ∧ constant_evaluation_creation(x) → same_factory_surface(x)
  | preserve(public_String_type ∧ existing_String_constructors)
    ∧ direct_external_cpp_range_or_view_import(x) → retain(SourceConstruction(x))
  | trace_to(REQ-FN-029H)

λ S5_variant_creation_policy(x). user_facing_Variant_creation(x) → prefer(lowercase_variant_factory(x))
  | runtime_creation(x) ∧ constant_evaluation_creation(x) → same_factory_surface(x)
  | preserve(public_Variant_type ∧ existing_Variant_constructors)
  | trace_to(REQ-CAP-012A)

λ S5_range_and_repeat_creation_policy(x). user_facing_Range_or_Repeat_creation(x) → prefer(lowercase_range_or_repeat_factory(x))
  | runtime_creation(x) ∧ constant_evaluation_creation(x) → same_factory_surface(x)
  | preserve(public_Range_and_Repeat_types ∧ existing_constructors)
  | Repeat_factory(x) → preserve(existing_admission ∧ count_first_ordering ∧ count_normalization ∧ ownership)
  | trace_to(REQ-FN-013I ∧ REQ-FN-013B)

λ S5_change_policy(x). public_change(x) → preserve(S5_identity(x))

λ S5_authorized_effect_policy(x). public_operation(x) → require(DeterministicBehavior(x) ∧ ReferentialTransparency(x))
    unless(explicit_governing_contract_authorizes_effect(x))
  | authorized_effect(x) → limited_to(explicitly_declared_effect_scope(x))
    ∧ ¬authorize(additional_effects(x) ∨ mutation_of_input_collection_storage(x))
    ∧ (hidden_library_state(x) → forbidden_unless(explicitly_selected_ConversionDiagnosticMode_error_code(x)))
  | authorized_effect(x) → preserve(NoHeapConstraint(x) ∧ NoExceptionConstraint(x)
    ∧ SingleThreadedExecutionModel(x))
  | effect_authorization(x) → ¬imply(implemented(x) ∨ implementation_ready(x))

λ S5_resource_boundaries(x). supported_operation(x) → bounded_inspectable_resources(x)
  ∧ NoHeapConstraint(x) ∧ NoExceptionConstraint(x) ∧ NoRttiConstraint(x)
  ∧ NoHiddenGlobalInitialization(x) ∧ SingleThreadedExecutionModel(x)
  | rationale: explicit resource and runtime-service boundaries keep embedded behavior auditable

λ S5_domain_and_conversion_safety(x). value_domain(x) → explicitly_governed(x)
  | argument_conversion(x) → preserve_information_and_semantic_domain(x)
    unless(explicitly_specified_exception_with_checked_or_exact_domain_policy(x))
  | rationale: admission must not silently lose information or change an operation's meaning

λ S5_checked_arithmetic_policy(x). CheckedArithmeticOperation(x) → preserve(ArithmeticOperandDomain ∧ ArithmeticResultType ∧ mathematical_failure_semantics)
  | policy_boundary(x) → keep_checked_arithmetic_behavior_authoritative_in_requirements_and_specs)
  | responsibility(x) → implement_failures_without_silent_overflow_underflow_or_domain_crossing)
  | rationale: predictable arithmetic failures and sign semantics protect embedded determinism)
  | traces_to(REQ-NUM-015A–REQ-NUM-015L)

λ S5_value_and_failure_semantics(x). collection_update(x) → independently_valid_persistent_result(x)
  | owning_result(x) → independent_of_source_lifetime(x)
  | failure_or_incomplete_result(x) → explicit_predictable_policy(x) ∧ applicable_preflight_or_checked_outcome(x)
  | rationale: callers must be able to reason about value ownership and unsuccessful outcomes without hidden state

## S4 - Intelligence

λ S4_intelligence_intro(x). adapt(x) → profile_gated_evolution(x)
  | profile_evolution(x) → preserve(stable_public_vocabulary ∧ backward_compatible_semantic_contracts)
  | new_capability(x) → introduce_as(explicit_mode) ∧ ¬silent_behavior_drift(x)

λ S4_learning(x). detect(assumption_break) → capture_decision ∧ refine_policy
  | prefer_explicit_deprecation_windows(x)
  | stable_handle_model(x)

λ stable_handle_model(x). profile_change(x) → preserve(public_api_names)
  ∧ preserve(argument_shapes)
  ∧ preserve(result_contracts)
  | capability_variance(x) → select_by_profile_not_renaming(x)

## S3 - Control

λ S3_control_intro(x). enforce(x) → NoHeapConstraint ∧ NoExceptionConstraint ∧ deterministic_sentinel_and_overflow_behavior ∧ strict_quality_gates
  | resource_usage(x) ∧ policy_outcomes(x) → bounded ∧ auditable

λ S3_constraints(x). NoHeapConstraint(x) ∧ NoExceptionConstraint(x)
  | NoRttiConstraint(x) ∧ NoHiddenGlobalInitialization(x)
  | SingleThreadedExecutionModel(x)
  | DeterministicOverflowPolicy(x) ∧ SentinelBasedAccess(x)
  | enforce_in_all_profiles(x)
  | traces_to(S5_resource_boundaries)

λ S3_quality(x). require(quality_gates(x))
  | quality_gates(x) → enforce(S5_change_policy(x))
  | traces_to(S5_change_policy)
  | require(tests)
  | require(sanitizers_host_profiles)
  | require(clang_tidy)
  | require(clang_format)
  | require(documentation_checks)

λ quality_gates(x). tests_pass(x)
  ∧ lint_passes(x)
  ∧ docs_checks_pass(x)
  ∧ quality_gate_thresholds_met(x)
  | host_profiles(x) → sanitizers_pass(x)
  | no_heap_claim(x) → no_heap_verification_passes(x)

λ quality_gate_thresholds_met(x). host_profiles(x) → coverage_core_mvp_line_percent(x) ≡ 100
  | host_profiles(x) → asan_enabled(x) ∧ ubsan_enabled(x)
  | documentation_samples(x) → compile_and_test_pass(x)
  | docs_site(x) → doxygen_html_generated(x)

λ quality_gate_enforcement(x). any_quality_gate_fails(x) → reject_change(x)

λ S3_resource_policy(x). static_storage_only(x)
  | collection_maximum_element_count(x) ≡ CollectionMaximumElementCount
  | StaticInspectableStorage(x)
  | automatic_or_static_storage_duration(x)
  | ClosedNominalCollectionDomain(x) ∧ NominalCollectionRecognition(x)
  | CollectionKind(x) → distinguish_supported_collection_families(x)
  | SimpleAggregateBoundary(x)
  | closed_numeric_domain(x)
  | reject_out_of_policy_inputs(x)
  | traces_to(S5_resource_boundaries ∧ S5_domain_and_conversion_safety)

λ S3_collection_storage_admission(x). stored_user_defined_type(x) → require(NothrowCollectionElement(x))
  | traces_to(S5_resource_boundaries ∧ S5_domain_and_conversion_safety)
  | NothrowCollectionElement(x) → require(nothrow_default_construction(x)
    ∧ nothrow_copy_construction(x) ∧ nothrow_copy_assignment(x) ∧ nothrow_destruction(x))
  | storage_admission(x) → not_imply(StableEquality ∨ TotalOrder ∨ hashing ∨ parsing ∨ traversal)
  | collection_storage_position(x) ≡ element ∨ map_key ∨ map_value
  | String(x) → apply(non_throwing_storage_and_destruction(x))

λ S3_checked_arithmetic_control(x). governed_by(REQ-NUM-015A–015L ∧ REQ-NUM-017 ∧ REQ-FN-002N)
  | arithmetic_operand_admission(x) → (supported_integer × supported_integer) ∨ (same_supported_binary_floating × same_supported_binary_floating)
    ∧ reject(integer_floating_crossing ∨ Boolean ∨ character ∨ byte ∨ enum ∨ pointer ∨ user_defined)
  | integer_result_type(x) → select_smallest_supported_lossless_static_common_type(x) ∨ reject_compile_time(x)
  | floating_result_type(x) ≡ identical_operand_type(x) ∧ mixed_floating_type(x) → reject_compile_time
  | working_type_conversion(x) → preserve_exact_mathematical_operand_value(x)
  | arithmetic_failure(x) → detect(overflow ∨ underflow ∨ division_by_zero ∨ NaN ∨ infinity)
  | compile_time_known_arithmetic_failure(x) → reject_constant_evaluation(x)
  | runtime_arithmetic_failure(x) → result_type{} ∧ unchanged_operands
  | floating_arithmetic(x) → admit(inherent_binary_floating_rounding) ∧ reject(nonfinite ∨ overflow_to_infinity ∨ nonzero_underflow_to_zero)
  | corresponding_preflight(x) → same_admission_and_failure_semantics(x) ∧ true_iff_operation_can_succeed
  | traces_to(S5_checked_arithmetic_policy ∧ S5_domain_and_conversion_safety ∧ S5_value_and_failure_semantics)

λ S3_explicit_conversion_surface(x). governed_by(REQ-NUM-008 ∧ REQ-NUM-009 ∧ REQ-NUM-010 ∧ REQ-NUM-010A–010E ∧ REQ-NUM-012 ∧ REQ-NUM-013 ∧ REQ-NUM-017)
  | mechanism(x) → centralize(named_destination_admission, loss_policy_selection, preflight_agreement, and diagnostic_mode_configuration)
  | responsibility(x) → share(numeric_character_byte_conversion_policy_across_free_functions_and_preflights)
  | component_boundary(x) → preserve(application_selected_diagnostic_handler_boundary_and_library_managed_error_code_boundary)
  | traces_to(S5_domain_and_conversion_safety)

λ S3_conversion_admission(destination, source). governed_by(REQ-CONV-001 ∧ REQ-CONV-002 ∧ REQ-CONV-003)
  | mechanism(x) → centralize(type_admission_and_element_construction_gates_at_conversion_boundaries)
  | responsibility(x) → share_one_safe_conversion_and_checked_normalization_service_across_public_admission_paths)
  | component_boundary(x) → keep_operation_specific_failure_and_diagnostic_policy_in_requirements_and_specs)
  | traces_to(S5_domain_and_conversion_safety)

λ S3_string_character_validity(character). valid(character) ≡ character ≠ NUL
  ∧ unsigned_byte_value(character) ≤ 0x7F
  | String::character_is_valid(character) ↔ valid(character)
  | traces_to(S5_domain_and_conversion_safety)

λ S2_character_is_valid(value). require(integral(value) ∧ ¬bool(value))
  | not(in_range<char>(value)) → return(false)
  | in_range<char>(value) → String::character_is_valid(convert_without_narrowing(value, char))
  | constexpr ∧ noexcept ∧ non_mutating ∧ non_allocating
  | unsupported_input_type(value) → targeted_dependent_static_assert(x)
    ∧ not(supported_call_target(x))

λ S2_character_is_valid_c_string(c_string). require(char_array(c_string))
  | static_extent(c_string) ≠ 2 → targeted_dependent_static_assert(c_string)
  | static_extent(c_string) = 2 ∧ c_string[1] ≠ NUL → return(false)
  | static_extent(c_string) = 2 ∧ c_string[1] = NUL → String::character_is_valid(c_string[0])
  | constexpr ∧ noexcept ∧ non_mutating ∧ non_allocating

λ S3_cpp_interoperability(x). supported_collection(x)
  → require(ConstRangeTraversal(x) ∧ ReadOnlyInteropAccessor(x))
  | Queue(x) → require(LogicalTraversalOrder(x)) ∧ ¬require(ContiguousConstView(x))
  | traces_to(S5_resource_boundaries ∧ S5_value_and_failure_semantics)

λ S3_value_policy(x). PersistentValueSemantics(x) ∧ DeepCopyUpdate(x)
  | enforce(S5_authorized_effect_policy(x))
  | conditional_on(user_defined_types_obeying_applicable_purity_effect_and_allocation_contracts(x))
  | user_code_effects(x) → limit_enforcement_to(statically_checkable_type_and_signature_constraints(x))
    ∧ ¬claim(general_verification_of_allocation_or_exception_effects(x))
  | preserve_input_collection_values(x) ∧ return(OwningValue(x))
  | traces_to(S5_value_and_failure_semantics)

λ S3_effect_control(x). explicitly_authorized_effect(x) → enforce(declared_effect_scope(x))
  | effectful_callback_or_reference_operation(x) → require(explicit_governing_contract(x))
  | unauthorized_effect(x) → reject(x)
  | traces_to(S5_authorized_effect_policy ∧ S5_resource_boundaries)

λ S3_result_contract(x). result_contract_rules(x) → active_for(all_public_operations)
  | approved_operation(x) → comply_with(result_contract_rules(x))
  | new_operation(x) → comply_with(S3_result_contract_policy ∧ S2_result_status_model)
  | traces_to(S5_value_and_failure_semantics ∧ S5_resource_boundaries)

λ S3_result_contract_policy(x). operation_result(x) → classify_as(CompleteResult ∨ BoundedResult ∨ BoundedPrefixResult ∨ DefaultReturningResult ∨ CheckedFailureResult ∨ ProducerOnlyResult)
  | traces_to(S5_value_and_failure_semantics ∧ S5_resource_boundaries)
  | may_fail_complete_result(x) → require(PreflightPredicate ∨ CheckedFailureResult)
  | preflight_and_operation(x) → require(semantic_equivalence_on_success_and_failure_conditions)
  | semantically_infinite_producer(x) → bound_synthesis_by(CollectionMaximumElementCount)
  | oversized_finite_producer(x) → classify_as(BoundedPrefixResult) ∧ bound_synthesis_by(CollectionMaximumElementCount)
  | SourceConstruction(x) → classify_as(BoundedResult) ∧ require(owned_copy ∧ no_checked_constructor)
  | SourceConstruction(x) → admit_only(external_non_cljonic_range_or_view(x)) ∧ ¬admit(CljonicCollection(x) ∨ CljonicProducer(x))
  | SameTypeArgumentIsOneElement(x) → single_argument_same_type_as_element(x) → select_pack_construction(x) ∧ ¬select(SourceConstruction(x))
  | EnclosureConstruction(x) → deduce_element_type_from_argument(x) ∧ capacity_one(x) ∧ produce_one_element(x) ∧ ¬copy(x) ∧ ¬materialize(x) ∧ mirrors(Clojure_literal_enclosure)
  | RangeViewMaterialization(x) → direct_source_construction(x) ∧ copy_prefix_to(destination_capacity(x))
  | static_extent_source_overflow(x) → reject_at_compile_time(x) ∧ diagnostic_not_result_status(x)
  | runtime_extent_source_overflow(x) → classify_as(BoundedPrefixResult) ∧ preserve(source_traversal_order)
  | complete_source_materialization(x) → require(fits_into(x) ∧ into(x))
  | fits_into((Vector ∨ Queue)(destination) ∧ finite_source) → compare(source_count ≤ remaining_destination_capacity) ∧ avoid(unsigned_addition_overflow)
  | fits_into((Set ∨ Map)(destination) ∧ finite_source) → measure(capacity_needed_by_ordered_insertion_fold) ∧ account_for(existing_and_source_duplicates ∨ matching_keys)
  | fits_into(destination ∧ unbounded_producer) → return(false)
  | finite_source(x) ∧ complete_consumption_exceeds_approved_traversal_bound(x)
    → fits_into(destination, x) = false ∧ ¬inspect(unobservable_suffix(x))
    ∧ applies_even_when(Set_duplicates ∨ Map_key_replacements avoid_capacity_growth)
  | finite_source_capacity_preflight(x) → require(complete_consumption_within_approved_traversal_bound(x))
  | compile_time_known_capacity_or_representability_failure(x) → reject_at_compile_time(x) ∧ diagnostic_not_result_status(x)
  | synthesis_cap(x) → classify_as(CollectionMaximumElementCount)
  | effective_size(x) → authoritative_for(ProducerIteration ∧ ProducerMaterialization)
  | effective_endpoint(x) → normalized_to(EffectiveBoundedPrefixBoundary)
  | producer_materialization(x) → require(ProducerMaterialization) ∧ enforce_synthesis_cap(x)

λ S3_domain_boundary(x). implemented_value_domain(x) ≡ Vector ∧ MapEntry ∧ Map ∧ Set ∧ Queue ∧ String ∧ Variant ∧ Range ∧ Repeat ∧ Cycle ∧ Iterate ∧ Repeatedly ∧ TakeWhile ∧ DropWhile
  | text_matching_domain(x) ≡ bounded_regex_values_and_match_results
  | symbolic_key_domain(x) ≡ supported_scoped_enumerations
  | domain_expansion(x) → require(explicit_approved_requirement)
  | traces_to(S5_domain_and_conversion_safety)

λ S3_vocabulary_enforcement(x). vocabulary_or_public_surface_change(x) → require(vocabulary_consistency_gate(x))
  | vocabulary_consistency_gate(x) → enforce(canonical_vocabulary_governs) ∧ traces_to(S5_identity)
  | vocabulary_consistency_gate_fails(x) → reject_change(x)

λ S3_rejection_diagnostic(x). governed_by(REQ-DIAG-001 ∧ REQ-DIAG-003 ∧ REQ-DIAG-009 ∧ REQ-DIAG-010)
  | mechanism(x) → use(concept_based_admission_overloads_and_instantiation_dependent_diagnostic_redirects)
  | responsibility(x) → keep_public_rejection_language_user_facing_and_stable_across_closed_domains)
  | component_boundary(x) → centralize(diagnostic_coverage_gate_for_free_functions_and_source_constructors)
  | traces_to(S5_domain_and_conversion_safety ∧ S5_value_and_failure_semantics)

λ S3_class_template_deduction_diagnostic(x). public_class_template(x) ∧ closed_type_argument_domain(x)
  | mechanism(x) → route_rejected_deduction_through_one_constrained_diagnostic_redirect_pattern)
  | responsibility(x) → keep_closed_template_admission_and_user_facing_diagnostic_behavior_consistent)
  | component_boundary(x) → isolate_diagnostic_specializations_from_admitted_primary_deduction)
    ∧ internal(x) ∧ ¬supported_construction_target(x) ∧ emits_message_only(x)
  | diagnostic_message(x) → identify(class_template ∧ rejected_argument_types ∧ violated_domain_rule(x))
    ∧ use(user_facing_language(x)) ∧ ¬depend_on(compiler_specific_wording(x))
  | admitted_deduction_or_explicit_construction(x) → select(real_specialization(x)) ∧ semantics_unchanged(x)
  | out_of_domain_type_argument(x) → ¬satisfy(any_admission_concept(x))
  | classification(x) → covered_by(diagnostic_coverage_gate(x) ∧ diagnostic_message_label_gate(x))
  | traces_to(REQ-DIAG-012 ∧ REQ-DIAG-004)

λ S3_diagnostic_message_label(x). targeted_compile_time_diagnostic_message(x) ∧ emitted_in(src/)
  → require(fixed_label(x))
  | fixed_label(x) ≡ "cljonic::" ∧ entity_name(x) ∧ ": " ∧ message_body(x)
  | entity_name(x) ≡ public_free_function_name(x) ∨ class_template_name(x)
  | message_body_language(x) → governed_separately_by(user_facing_diagnostic_language_decision(x)) ∧ ¬governed_by(this_rule(x))
  | applies_to(class_internal_assertions(x) ≡ capacity ∧ initializer_count ∧ constructor ∧ range_view ∧ static_extent)
  | concatenated_message_literals(x) → label_binds(first_literal(x))
  | ¬applies_to(class_template_gated_rejection_without_message(x))
  | label_convention(x) → mechanically_checkable(x) ∧ surfaced_by(diagnostic_message_label_gate(x))
  | traces_to(S3_rejection_diagnostic ∧ S5_value_and_failure_semantics)
  | violation(x) → conformance_defect(x) ∧ surface_by(diagnostic_message_label_gate(x))
  | forward_binding(x) → bind(every_future_diagnostic_message(x))

## S2 - Coordination

λ S2_coordination_intro(x). coordination(x) → driven_by(canonical_vocabulary ∧ interaction_protocols)
  | component_consistency(x) → ProbeFirstAccess(x) ∧ stable_handle_contracts(x) ∧ prefer(Clojure_semantic_consistency) when(embedded_constraints_permit(x))

λ S2_protocol(x). canonical_vocabulary_controls_interfaces(x)
  ∧ canonical_vocabulary_controls_docs(x)
  | caller_needs_to_distinguish_absence_or_invalidity_from_valid_default_value(x)
    → require(ProbeFirstAccess(x))
  | caller_accepts_documented_default_returning_access(x) → ¬require(prior_probe(x))
  | semantic_predicate_names(x) → enforce(StatePredicate ∧ VerbPredicate)
  | state_or_adjectival_predicate(x) → use(IsPredicatePrefix)
  | readable_verb_predicate(x) → retain(direct_verb_form(x))
  | established_capability_or_preflight_prefix(x)
    → retain(CanPredicatePrefix ∨ HasPredicatePrefix ∨ ValidPredicatePrefix)
  | api_surface_status(x) → enforce(LifecycleClassification)

λ S2_modular_header_taxonomy(x). collection_type_header(x) → name(cljonic-collection-<name>.hpp)
  | producer_type_header(x) → name(cljonic-producer-<name>.hpp)
  | standalone_FreeFunction_header(x) → name(cljonic-<operation>.hpp) ∧ ¬suffix(-function)
  | include_build_test_and_documentation_references(x) → follow(canonical_header_name(x))

λ S2_api_lifecycle_gate(x). public_function(x) → classify_as(RequirementsBacked ∨ CandidateStatus ∨ DeferredStatus ∨ ExcludedStatus)
  | classify_as(RequirementsBacked) → requires(approved_behavioral_contracts)
  | classify_as(CandidateStatus ∨ DeferredStatus ∨ ExcludedStatus) → not_supported_behavior(x)
  | public_function(x) → classified_before(api_surface_finalization)
  | RequirementsBacked(x) → require(approved_req_mapping)
  | implemented_supported_operation(x) → require(RequirementsBacked(x) ∧ executable_test_coverage(x))
  | CandidateStatus(x) → awaiting(specification)
  | DeferredStatus(x) → awaiting(approved_design)
  | ExcludedStatus(x) → out_of_scope(x)

λ S2_relational_operations_gate(x). relational_ops(index ∨ project ∨ rename ∨ join)
  → require(RelationModel ∧ approved_behavioral_contracts) before RequirementsBacked
  | relation_model_minimum(x) → require(row_representation ∧ key_value_capabilities ∧ duplicate_row_key_semantics ∧ nested_result_representation ∧ traversal_order_semantics ∧ capacity_arithmetic ∧ complete_result_preflight ∧ bounded_failure_behavior)

λ S2_profile_coherence(x). profile_change(x) → preserve(stable_handle_model(x))
  | preserve(api_shape)
  | preserve(core_semantics)

λ S2_semantic_alignment(x). prefer(clojure_parity)
  | compatible_with(EmbeddedConstraint ∧ S5_authorized_effect_policy)

λ S2_concept_bootstrap(x). concepts_gate_free_functions(x)
  | minimal_concept_requirements(x) → only_what_current_functions_need(x)
  | future_requirements(x) → add_when_first_function_needs_them(x)
  | yagni_evolution(x) → preserve(backward_compat) ∧ prevent(feature_creep)

λ S2_capability_model(x). public_capability(x) → express_as(named_concept_or_equivalent_constraint)
  | capability(x) → remain_orthogonal_to(nominal_collection_identity)
  | storage_admission(x) → not_imply(StableEquality ∨ TotalOrder ∨ other_operation_capability)
  | capability_required_by(operation) → constrain_at(public_api_boundary)
  | public_concept_name(x) → identify(capability_or_constraint)
  | vague_concept_name(x) → reject(x)

λ S2_canonical_capabilities(x). public_capability_names(x) ≡ Indexed ∧ Lookup ∧ Seqable ∧ Associative ∧ Conjable
  | Indexed(x) → constant_time_non_traversing_access_by_non_boolean_non_character_integer_index(x) ∧ bounded_logical_index_domain(x)
    ∧ non_mutating_access(x) ∧ non_throwing_non_allocating_membership_predicate(x)
    ∧ negative_index_outside_domain_when_representable(x)
  | Lookup(x) → collection_defined_lookup_domain(x) ∧ non_mutating_access(x)
    ∧ default_or_fallback_on_absence(x) ∧ non_throwing_non_allocating_membership_predicate(x)
  | Seqable(x) → documented_logical_traversal_representation(x)
    ∧ non_mutating_sequence_conversion(x) ∧ bounded_independently_valid_result(x)
    ∧ sequence_requirements_are_implementation_backed_through(seq ∧ REQ-SEQ-010)
    ∧ capability_concept(x) → closed_to_external_declaration(x) ∧ membership_owned_by(cljonic_collection_family(x)) ∧ trace_to(REQ-CAP-014)
  | Associative(x) → non_mutating_key_value_association(x) ∧ return(new_collection_value(x))
    ∧ preserve_source(x) ∧ define(key_value_and_capacity_policy(x))
    ∧ non_throwing_non_allocating_preflight(can_assoc(x))
  | capability_satisfaction(x) → compositional_and_operation_specific(x)
    ∧ remain_distinct(Indexed ∧ Lookup ∧ Seqable ∧ Associative ∧ Conjable)
  | IndexedCollection(x) → refine(Lookup(non_boolean_non_character_integer_key_domain(x)))
  | IndexedProducer(x) → not_imply(Lookup(x)) ∧ require(explicit_positional_retrieval_specification_before_value_access(x))
  | Associative(x) → not_imply(Indexed ∨ Lookup) unless(concrete_collection_requirement(x))
  | Seqable(x) → independent_of(Indexed ∧ Lookup ∧ Associative)
  | public_operation(x) → require(only_capabilities_needed_by(documented_behavior(x)))

λ S2_capability_participation(x). capability_matrix(x) ≡ {
  Vector: Indexed ∧ Lookup ∧ Associative ∧ Conjable,
  Map: Lookup ∧ Associative ∧ Conjable,
  Set: Lookup ∧ Conjable,
  String: Indexed ∧ Lookup ∧ Associative ∧ Conjable,
  Queue: none_of(Indexed ∧ Lookup ∧ Associative) ∧ Conjable
  }
  | Seqable(x) → closed_capability(x) for each_supported_collection(x) ∧ ¬∃ external_or_new_type_declares(Seqable(x))
    ∧ sequence_requirements_are_implementation_backed_through(seq ∧ REQ-SEQ-010) ∧ trace_to(REQ-CAP-014)
  | capability_matrix(x) → govern(public_concepts ∧ free_function_constraints ∧ documentation_names)

λ S2_vector_factory_contract(x). vector(args...) → coordinate_owning_Vector_construction_and_type_deduction
  | mechanism(x) → share_capacity_argument_construction_and_nothrow_element_admission_services)
  | responsibility(x) → preserve_one_factory_surface_and_copy_or_move_owning_result_boundary)
  | traces_to(REQ-FN-029A) ∧ realize(S5_vector_creation_policy)

λ S2_hash_map_factory_contract(x). hash_map(k1, v1, ...) → construct(owning_Map_by_associating_alternating_key_value_pairs_in_order(x))
  | mechanism(x) → coordinate_type_deduction_capacity_validation_and_alternating_key_value_insertion)
  | responsibility(x) → preserve_one_factory_surface_and_copy_on_associate_result_boundary)
  | trace_to(REQ-FN-029D) ∧ realize(S5_map_creation_policy)
  | trace_to(REQ-FN-029D) ∧ realize(S5_map_creation_policy)

λ S2_variant_factory_contract(x). variant<Alternatives...>() → construct(default_Variant_with_first_alternative_active(x))
  | mechanism(x) → coordinate_explicit_alternative_deduction_with_variant_storage_admission)
  | responsibility(x) → preserve_closed_variant_type_identity_and_non_allocating_result_boundary)
  | trace_to(REQ-CAP-012A) ∧ realize(S5_variant_creation_policy)

λ S2_set_factory_contract(x). hash_set(args...) → construct(owning_Set_from_individual_arguments_in_order(x))
  | mechanism(x) → coordinate_type_deduction_capacity_bounds_and_set_deduplication)
  | responsibility(x) → preserve_one_factory_surface_and_independent_owning_result_boundary)
  | trace_to(REQ-FN-029F) ∧ realize(S5_set_creation_policy)

λ S2_queue_factory_contract(x). queue(args...) → construct(owning_Queue_from_individual_arguments_in_Fifo_order(x))
  | mechanism(x) → coordinate_type_deduction_capacity_bounds_and_FIFO_construction_order)
  | responsibility(x) → preserve_one_factory_surface_and_independent_owning_result_boundary)
  | trace_to(REQ-FN-029G) ∧ realize(S5_queue_creation_policy)

λ S2_string_factory_contract(x). string(text_source) → construct(owning_String_from_text_source(x))
  | mechanism(x) → coordinate_text_source_admission_capacity_and_bounded_string_import)
  | responsibility(x) → preserve_one_factory_surface_and_independent_owning_text_result_boundary)
  | trace_to(REQ-FN-029H) ∧ realize(S5_string_creation_policy)

λ S2_map_entry_factory_contract(x). map_entry(key, value) → construct(owning_MapEntry_from_one_key_and_one_value(x))
  | mechanism(x) → coordinate_key_value_type_deduction_and_nothrow_owning_construction)
  | trace_to(REQ-FN-029E) ∧ realize(S5_map_entry_creation_policy)

λ S2_range_factory_contract(x). RangeFactory(x) → coordinate(type_deduction, constructor_defaults, and range_parameter_ownership)
  | mechanism(x) → reuse(Range_construction_rules_and_observation_cap_across_typed_untyped_factories)
  | responsibility(x) → target(one_compile_time_diagnostic_surface_and_one_public_factory_surface)
  | traces_to(REQ-FN-013I ∧ REQ-FN-012 ∧ REQ-FN-013 ∧ REQ-DIAG-009)

λ S2_repeat_factory_contract(x). repeat(value) → construct(unbounded_Repeat_owning_value_copy(x))
  | repeat(count, value) → construct(finite_Repeat_owning_value_copy_with_normalized_count(x))
  | preserve(existing_factory_domain ∧ count_first_ordering ∧ nonpositive_count_normalization)
  | equivalent_Repeat_constructor(x) → same_result_type_and_behavior(x)
  | trace_to(REQ-FN-013B ∧ REQ-CONV-002E) ∧ realize(S5_range_and_repeat_creation_policy)

λ S2_associative_contract(x). Associative(collection) → require(assoc(collection, key, value) ∧ can_assoc(collection, key, value))
  | assoc(x) ∧ can_assoc(x) → constexpr ∧ noexcept ∧ non_mutating ∧ non_allocating
  | can_assoc(collection, key, value) ↔ association_key_value_and_capacity_policy_allows_requested_assoc(x)
  | can_assoc(collection, key₁, value₁, key₂, value₂, …) → fold_preflight_pairs_left_to_right(x)
    ∧ each_pair_preflighted_against_the_accumulator_after_prior_successful_pairs(x)
    ∧ true_only_when_every_pair_preflight_succeeds(x)
    ∧ pair(x) → admit_under(collection_key_value_domain(x)) regardless_of_argument_position(x)
    ∧ odd_trailing_key(x) → reject_at_compile_time(x)
    ∧ require(at_least_two_pairs(x)) ∧ preserve(single_pair_arity(x))
    ∧ constexpr ∧ noexcept ∧ non_mutating ∧ non_allocating
  | can_assoc(String, index, char_value) → false_when_char_is_invalid(x)
  | assoc(Map, key, value) → replace_existing_value_or_insert_new_key_when_capacity_remains(x)
  | assoc(Vector, non_boolean_non_character_integer_index, value) → replace_existing_index_or_append_at_count_when_capacity_remains(x)
  | assoc(String, non_boolean_non_character_integer_content_index, char_value) → replace_existing_byte_or_append_at_count_when_capacity_remains(x)
    ∧ preserve_ascii_validity_policy(x) ∧ preserve_null_terminator(x)
    ∧ reject_invalid_character_at_compile_time_when_known(x)
    ∧ replace_invalid_runtime_character_with_period(x)
  | invalid_association_key_or_full_append(x) → return(unchanged_source(x))
    ∧ not(throw ∨ allocate ∨ mutate_source(x))
  | assoc(collection, key₁, value₁, key₂, value₂, …) → fold_assoc_pairs_left_to_right(x)
    ∧ assoc(c, key₁, value₁, key₂, value₂) ≡ assoc(assoc(c, key₁, value₁), key₂, value₂)
    ∧ pair(x) → admit_under(collection_key_value_domain(x))
    ∧ pair_outside_domain(x) → reject_at_compile_time(x) regardless_of_argument_position(x)
    ∧ pair_that_cannot_produce_documented_result_at_full_capacity(x)
        → no_op(accumulator(x)) ∧ continue(remaining_pairs(x)) ∧ not(compile_time_error ∨ throw(x))
    ∧ odd_trailing_argument_count(x) → reject_at_compile_time(x)
    ∧ require(at_least_two_pairs(x)) ∧ require(single_pair_arity_unchanged(x))
    ∧ constexpr ∧ noexcept ∧ non_mutating ∧ non_allocating
  | associative_operation(x) → classify_as(RequirementsBacked)
    ∧ trace_to(requirements/requirements-module-2.md ∧ requirements/requirements-module-3.md)

λ S2_dissoc_contract(x). dissoc(map, keys...) → require(CljonicMap(map))
  | zero_keys(x) → return(unchanged_map_copy(x))
  | keys(x) → require(SafeImplicitConversion(Map::key_type, each_key))
    ∧ apply_left_to_right(x)
  | present_key(x) → return(new_map_without_entry_via_swap_and_remove(x))
  | absent_or_repeated_key(x) → no_op(x)
  | dissoc(x) → constexpr ∧ noexcept ∧ non_mutating ∧ non_allocating
  | unsupported_collection_or_key_domain(x) → reject_at_public_API_boundary_with_targeted_diagnostic(x)
  | dissoc(x) → classify_as(RequirementsBacked) ∧ trace_to(requirements/requirements-module-3.md)

λ S2_conj_contract(x). conj(collection, value) → return(new_collection_value(x))
  | conj(x) ∧ can_conj(x) → constexpr ∧ noexcept ∧ non_mutating ∧ non_allocating
  | can_conj(collection) → return(true) ∧ preserve(collection(x))
  | can_conj(collection, value) ↔ conj(collection, value)_can_produce_documented_result
    ∧ true_when(duplicate_set_value_is_successful_noop(x))
    ∧ can_conj(String, character) → capacity_only(x) ∧ valid_character_only(x)
  | can_conj(collection, value₁, value₂, …) → fold_preflight_values_left_to_right(x)
    ∧ each_value_preflighted_against_the_accumulator_after_prior_successful_values(x)
    ∧ true_only_when_every_value_preflight_succeeds(x)
    ∧ value(x) → admit_under(collection_value_domain(x)) regardless_of_argument_position(x)
    ∧ require(at_least_two_values(x)) ∧ preserve(single_value_arity(x))
    ∧ constexpr ∧ noexcept ∧ non_mutating ∧ non_allocating
  | conj(Set, value) → require(StableEquality(x) ∧ LinearScan(x))
    ∧ insert_when_absent_or_preserve_count_when_present(x)
    ∧ require(capacity_for_new_value_when_absent(x))
  | conj(Queue, value) → append_at_rear(x) ∧ preserve_fifo_order(x)
    ∧ require(capacity_for_new_value(x))
  | conj(Vector, value) → append_at_logical_count(x)
    ∧ require(capacity_for_new_value(x))
  | conj(String, character) → append_at_logical_count(x)
    ∧ require(capacity_for_new_character(x))
    ∧ apply(String_character_validity_policy(x))
    ∧ preserve(null_terminator(x))
  | conj(Map, entry) → associate_entry(key ∧ value)(x)
    ∧ replace_value_when_key_present(x) ∧ require(capacity_for_new_key_when_absent(x))
    ∧ entry ≡ MapEntry(key ∧ value)(x)
  | conj_requiring_additional_capacity_when_full(x) → return(unchanged_source(x))
    ∧ not(throw ∨ allocate ∨ mutate_source(x))
  | unsupported_conj_argument_domain(x) → reject_at_compile_time(x)
  | conj(String, invalid_character)(x) → reject_during_constant_evaluation(x)
    ∧ runtime(x) → replace_with_period(x) ∧ append_only_when_capacity_remains(x)
  | duplicate_set_insertion(x) ∨ existing_map_key_replacement(x) → ¬require(additional_capacity(x))
  | conj(collection) → return(unchanged_copy_of_source(x))
  | conj(collection, value₁, value₂, …) → fold_conj_values_left_to_right(x)
    ∧ conj(c, x₁, x₂) ≡ conj(conj(c, x₁), x₂)
    ∧ value(x) → admit_under(collection_value_domain(x))
    ∧ value_outside_domain(x) → reject_at_compile_time(x) regardless_of_argument_position(x)
    ∧ conj(String, character)(x) → apply(String_character_validity_policy(x))
    ∧ value_that_cannot_produce_documented_result_at_full_capacity(x)
        → no_op(accumulator(x)) ∧ continue(remaining_values(x)) ∧ not(compile_time_error ∨ throw(x))
    ∧ require(at_least_two_values(x)) ∧ require(one_value_arity_unchanged(x))
    ∧ constexpr ∧ noexcept ∧ non_mutating ∧ non_allocating
  | conj_operation(x) ∧ can_conj(x) → classify_as(RequirementsBacked)
    ∧ trace_to(requirements/requirements-module-3.md)

λ S2_composite_value_domain(x). composite_value(x) ≡ cljonic_variant_over(
    scalar_literals ∨ scoped_enumerations ∨ supported_collections ∨ producers_with_stable_parameters
    ∨ MapEntry ∨ composite_values)
  | composite_as_key_or_set_element(x) → require(every_stored_component_admits_stable_equality(x))
    ∧ require(recursive_component_analysis(x))
  | callable_component(x) → reject_as_key_or_set_element(x)
    because(callables_do_not_admit_stable_value_equality(x))
  | variant_equality(x) → alternative_strict(x)
    ∧ ¬cross_type_numeric_unification(x) ∧ ¬hash_based_equality(x)
  | composite_domain(x) → trace_to(REQ-CAP-010 ∧ REQ-CAP-011)

λ S2_variant_value(x). cljonic::Variant<Alternatives...> → component_of(CompositeValueDomain)
  | mechanism(x) → enforce(nominal_alternative_admission, capability_gated_equality_and_ordering, and closed_component_recognition)
  | responsibility(x) → coordinate_variant_storage_with_collection_storage_and_composite_equality_policy)
  | component_boundary(x) → preserve_no_valueless_no_exception_variant_access_boundary)
  | variant_value(x) → trace_to(REQ-CAP-011 ∧ REQ-CAP-012)

λ S2_collection_equality(x). bounded_collection(x) ∧ stable_equality_components(x)
  | mechanism(x) → centralize_capability_gated_collection_equality_across_bounded_collection_kinds)
  | responsibility(x) → preserve_order_sensitive_and_semantically_unordered_comparison_boundaries)
    ∧ trace_to(REQ-COLL-021)

λ S2_producer_parameter_equality(x). producer(x) ∧ every_stored_component_admits_stable_equality(x)
  | mechanism(x) → separate_producer_parameter_equality_from_produced_sequence_equality)
  | responsibility(x) → keep_callable_parameters_out_of_stable_equality_and_container_admission)
    ∧ trace_to(REQ-FN-014B ∧ REQ-CAP-010 ∧ REQ-SEQ-016)

λ S2_general_equality_function(x). equal(lhs, rhs) → coordinate(named_general_equality_across_supported_domains)
  | mechanism(x) → reuse(stable_equality_implementation_and_left_to_right_pair_conjunction)
  | responsibility(x) → preserve(operator_equality_distinction_and_compile_time_domain_gating)
  | traces_to(REQ-FN-002E ∧ REQ-FN-002G ∧ REQ-NUM-018 ∧ REQ-NUM-019 ∧ REQ-SEQ-017)

λ S2_not_equal_function(x). not_equal(lhs, rhs) ≡ named_general_value_inequality(x)
  | not_equal_arity(x) ≡ three_clojure_arities(unary ∨ binary ∨ variadic)
    ∧ unary_form(x) → admit(single_supported_domain_operand(x)) ∧ return(false)
    ∧ variadic_form(x) → negate(conjoin(adjacent_pair_equality(x)))
      ∧ evaluate_left_to_right(x) ∧ short_circuit_on_first_unequal_pair(x)
    ∧ ∀adjacent_pair(x) ∧ ∀unary_operand(x) → satisfy(compile_time_domain_gating(x))
  | not_equal(x) → distinct_from(operator!= (x))
    ∧ scoped_to(same_supported_stable_equality_domain_as(equal)(x))
  | not_equal(x) → delegate_to(equal(x)) ∧ negate(result(x))
    ∧ domain_gating(x) ≡ domain_gating(equal)(x)
    ∧ family_classification(x) ≡ family_classification(equal)(x)
    ∧ termination(x) ≡ termination(equal)(x)
  | not_equal_operation(x) → classify_as(RequirementsBacked)
    ∧ trace_to(REQ-FN-002H ∧ REQ-FN-002C ∧ REQ-FN-002G)

λ S2_result_status_model(x). public_operation(x) → declare(CompleteResult ∨ BoundedResult ∨ BoundedPrefixResult
  ∨ DefaultReturningResult ∨ CheckedFailureResult ∨ ProducerOnlyResult)
  | operation(x) → document(status ∧ preflight ∧ failure_or_default_semantics)
  | complete_result_may_fail_to_fit(x) → require(PreflightPredicate(x))
  | preflight(x) → non_throwing(x) ∧ non_allocating(x)
  | preflight(x) → measure_same_domain_and_failure_conditions_as(corresponding_operation)
  | DefaultReturningResult(x) ≠ CheckedFailureResult(x)

λ S2_preflight_model(x). default_returning_access(x) → require(pre_access_predicate(x))
  | pre_access_predicate(x) → not(inspect_accessed_value(x))
  | pre_access_predicate(x) → distinguish(empty ∨ index_out_of_range ∨ missing_key ∨ full_state)
  | predicate_true(x) ↔ corresponding_operation_can_produce_valid_or_complete_result(x)
  | predicate_false(x) → follow(documented_default_bounded_or_failure_policy(x))
  | complete_materialization_into(destination, producer) → use(fits_into)
  | fits_into(destination, source) → require((CljonicVector ∨ CljonicSet ∨ CljonicMap ∨ CljonicQueue ∨ CljonicString)(destination) ∧ CljonicSource(source))
    ∧ require(same_compatibility_domain_as(into)) ∧ measure(complete_result_cardinality_and_insertion_semantics)
  | finite_source ∧ (Set ∨ Map)(destination) → allow(value_sensitive_source_traversal ∧ producer_callback_evaluation)
    ∧ account_for(existing_destination_and_prior_source_values_or_keys) ∧ reject_at(first_capacity_failure)
  | finite_source ∧ String(destination) → compare(source_count ≤ remaining_character_capacity) ∧ ¬evaluate(character_validity)
  | fits_into(destination, finite_source) ∧ (Set ∨ Map)(destination) → preserve(input_values) ∧ ¬share(realization_cache_with(into))
    ∧ allow(callback_effects_through_external_state) ∧ completeness_prediction_requires(same_values_on_subsequent_traversal)
  | fits_into(destination, unbounded_source) → return(false)
  | fits_into(destination, finite_source) → require(complete_consumption_within_approved_traversal_bound(source))
    ∧ otherwise_return(false) ∧ ¬inspect(unobservable_suffix(source))
    ∧ applies_even_when(duplicates_or_replacements_consume_no_capacity)

λ S2_into_function_contract(x). into(destination, source) → admit((CljonicVector ∨ CljonicSet ∨ CljonicMap ∨ CljonicQueue ∨ CljonicString)(destination) ∧ CljonicSource(source))
  | destination_domain(x) ≡ CljonicVector ∨ CljonicSet ∨ CljonicMap ∨ CljonicQueue ∨ CljonicString
    ∧ ¬admit(CljonicProducer ∨ external_destination)
  | ∀ source_element_reference ∈ source: require(NothrowElementConstruction(destination_insertion_type, source_element_reference))
  | Map(destination) → require(compatible_nominal_MapEntry_values) ∧ ¬admit(arbitrary_pairs ∨ alternating_keys_and_values)
  | source_domain(x) → CljonicSource ∧ ¬admit(external_Cpp_ranges_or_views)
  | result(x) → updated_destination_typed_collection(x)
    ∧ fold(source_values_in_traversal_order, destination_insertion_semantics)
    ∧ preserve(destination ∧ source)
  | (Vector ∨ Queue)(destination) → append_after(existing_logical_contents) ∧ preserve(Queue_FIFO_order)
  | Set(destination) → insert_distinct_values ∧ duplicates_consume_no_capacity
  | Map(destination) → assoc(entry_key, entry_value) ∧ matching_keys_replace_values_without_capacity_growth
    ∧ later_matching_entry_wins
  | String(destination) → append(characters_in_traversal_order) ∧ preserve(valid_non_NUL_ASCII)
    ∧ runtime(invalid_character) → append('.') ∧ continue_traversal
    ∧ constant_evaluation(invalid_character) → reject_with(String_compile_time_validation_diagnostic)
    ∧ stop_at(capacity_exhaustion)
  | Set(destination) → accept(duplicate_value_as_no_op, even_when_full)
  | Map(destination) → accept(MapEntry_with_existing_key_as_value_replacement, even_when_full)
  | fits_into(String(destination), finite_source) → compare(source_count ≤ remaining_character_capacity)
    ∧ ¬evaluate(character_validity)
  | source_traversal(x) → stop_before(first_value_requiring_unavailable_capacity)
    ∧ ¬skip_rejected_value
  | finite_source(x) → materialize(complete_result_when_it_fits(x)) ∨ materialize(bounded_prefix_result(x))
  | unbounded_source(x) → materialize(deterministic_bounded_prefix(x))
    ∧ bound_source_traversal_by(CollectionMaximumElementCount)
    ∧ bound(Set_new_distinct_values ∨ Map_new_keys, by: remaining_destination_capacity(x))
  | into_operation(x) → constexpr ∧ noexcept ∧ non_mutating ∧ non_allocating
  | rejected_destination_or_source(x) → provide(targeted_rejection_diagnostic(x))
  rationale: complete source materialization is explicit and owning; `fits_into` remains the completeness preflight, so an oversized source yields a documented bounded prefix rather than a failure

λ S2_get_function_contract(x). get(collection, key[, fallback]) → admit(LookupCollection ∨ IndexedCollection)(collection)
  | LookupCollection(collection) → require(SafeImplicitConversion(collection_lookup_type, key))
  | IndexedCollection(collection) → require(non_boolean_non_character_integer_index(key))
  | return(collection(key[, fallback])) ∧ preserve(collection_specific_lookup_result_type)
  | omitted_fallback(x) → use(collection_default_lookup_result(x))
  | supplied_fallback(x) → require(SafeImplicitConversion(lookup_result_type, fallback))
  | unsupported_collection_or_key_or_fallback(x) → reject_at_compile_time_with(targeted_diagnostic ∧ REQ-DIAG-009)
  | constexpr ∧ noexcept ∧ non_mutating ∧ non_allocating
  | classify_as(RequirementsBacked) ∧ trace_to(REQ-COLL-020R ∧ REQ-COLL-020Y)

λ S2_not_empty_function_contract(x). not_empty(collection) → admit(SequenceableCollection)(collection)
  | mechanism(x) → coordinate_empty_and_nonempty_collection_copy_paths_at_the_sequence_boundary)
  | responsibility(x) → preserve_independent_owning_result_and_collection_type_capacity)
  | classify_as(RequirementsBacked) ∧ trace_to(REQ-BOUNDS-010D ∧ REQ-FN-026)

λ S2_peek_function_contract(x). peek(collection) → coordinate_non_mutating_front_or_last_observation
  | mechanism(x) → dispatch_by_collection_specific_logical_access_rule)
  | responsibility(x) → preserve_source_value_no_heap_no_exception_observation_boundary)
  | traces_to(REQ-COLL-020Z ∧ REQ-DIAG-009)

λ S2_pop_function_contract(x). pop(collection) → coordinate_copy_on_remove_across_supported_stack_sources
  | mechanism(x) → dispatch_by_Vector_last_or_Queue_front_logical_policy)
  | responsibility(x) → preserve_source_capacity_type_order_and_independent_result_boundary)
  | traces_to(REQ-COLL-020AA ∧ REQ-COLL-022 ∧ REQ-DIAG-009)

λ S2_vector_stack_methods(x). Vector.peek() → return(last_logical_element ∨ value_type_default_value_if_empty)
  | mechanism(x) → align_Vector_member_and_free_function_stack_observation_mechanisms)
  | responsibility(x) → preserve_last_element_semantics_and_copy_on_pop_boundary)
  | classify_as(RequirementsBacked) ∧ trace_to(REQ-COLL-022)

λ S2_operation_vocabulary(x). canonical_collection_operations(x) ∧ requirements_backed_operations(x)
  | mechanism(x) → centralize_operation_naming_admission_and_header_taxonomy)
  | responsibility(x) → preserve_canonical_vocabulary_and_requirements_backed_lifecycle_boundaries)
  | deferred_collection_shaping_traversal_family(x) ≡ nth ∧ nthnext ∧ nthrest ∧ map_indexed ∧ rseq ∧ remaining_deferred_operations
  | deferred_collection_shaping_traversal_family(x) → remain_outside(current_collection_api(x))
  | traces_to(REQ-SEQ-022 ∧ REQ-DIAG-009)

λ S2_count_function_contract(x). count(value) → admit(SequenceableCollection ∨ SequenceableProducer)(x)
  | SequenceableCollection(x) → return(exact_logical_element_count(x))
  | finite_producer(x) → return(exact_runtime_result_count(x))
    except(finite_Range(x) → return(min(complete_cardinality(x), CollectionMaximumElementCount)))
  | UnboundedProducer(x) → return(CollectionMaximumElementCount)
    ∧ ¬claim(complete_cardinality(x))
  | TakeWhile(value) ∨ DropWhile(value) → compute(exact_yielded_count_by_fresh_bounded_traversal)
    ∧ permit(predicate_and_source_callback_evaluation) ∧ ¬cache_or_mutate(stored_parameters)
  | count_operation(x) → constexpr ∧ noexcept ∧ non_mutating ∧ non_allocating

λ S2_is_empty_function_contract(x). is_empty(value) → admit(SequenceableCollection ∨ SequenceableProducer)(x)
  | return(count(value) == 0) ∧ inherit(collection_or_producer_count_semantics(value))
  | unbounded_producer(value) → use(configured_observation_cap_not_complete_cardinality)
  | is_empty_operation(x) → constexpr ∧ noexcept ∧ non_mutating ∧ non_allocating

λ S2_empty_function_contract(x). empty(collection) → admit(SequenceableCollection(collection))(x)
  | return(empty_owning_value_of_same_type(collection)) ∧ preserve(capacity_type(collection))
  | result(x) → satisfies(is_empty(x)) ∧ independent_of_input_lifetime(x)
  | preserves(input_collection_value(x))
  | empty_operation(x) → constexpr ∧ noexcept ∧ non_mutating ∧ non_allocating
  | rejected_input(x) → provide(targeted_rejection_diagnostic(x))

λ S2_first_function_contract(x). first(value) → coordinate_one_needed_first_access_across_collections_producers_and_MapEntry
  | mechanism(x) → use_one_pass_source_window_and_first_element_presence_preflight_internals)
  | responsibility(x) → preserve_producer_state_copy_rules_and_collection_specific_projection_boundary)
  | traces_to(REQ-FN-002A1 ∧ REQ-DIAG-009)

λ S2_map_function_contract(x). map(transform, sources...) → coordinate_multi_source_transform_traversal_and_materialization
  | mechanism(x) → share(source_admission_callback_storage_and_capacity_bound_services)
  | responsibility(x) → terminate_one_step_at_first_source_exhaustion_and_preserve_source_values)
  | traces_to(REQ-FN-032 ∧ REQ-DIAG-009)

λ S2_filter_function_contract(x). filter(predicate, source) → coordinate_predicate_admission_and_filtered_materialization
  | mechanism(x) → share(predicate_inspection_source_bound_and_output_capacity_services)
  | responsibility(x) → evaluate_and_append_through_one_original_source_observation)
  | traces_to(REQ-FN-035 ∧ REQ-DIAG-009)

λ S2_filterv_alias_contract(x). filterv(predicate, source) → delegation_boundary_for_filter
  | mechanism(x) → delegate_alias_arguments_and_preserve_result_capacity_and_traversal_pipeline)
  | responsibility(x) → maintain_one_contract_surface_for_filter_and_filterv)
  | traces_to(REQ-FN-036 ∧ REQ-FN-035 ∧ REQ-DIAG-009)

λ S2_take_function_contract(x). take(count, source) → coordinate_bounded_prefix_materialization
  | mechanism(x) → share(SourceBound_normalization_iterator_exhaustion_and_output_Vector_construction)
  | responsibility(x) → stop_at_requested_prefix_or_original_source_window_endpoint)
  | traces_to(REQ-FN-037 ∧ REQ-SEQ-022 ∧ REQ-DIAG-009)

λ S2_drop_function_contract(x). drop(count, source) → coordinate_original_window_skip_and_suffix_materialization
  | mechanism(x) → reuse(SourceBound_and_one_window_traversal_services)
  | responsibility(x) → retain_suffix_without_fresh_producer_observation)
  | traces_to(REQ-FN-038 ∧ REQ-SEQ-022 ∧ REQ-DIAG-009)

λ S2_take_last_function_contract(x). take_last(count, source) → coordinate_bounded_suffix_selection
  | mechanism(x) → reuse_one_window_traversal_and_bounded_Vector_capacity_services)
  | responsibility(x) → retain_only_the_original_window_trailing_selection)
  | traces_to(REQ-FN-042 ∧ REQ-SEQ-022 ∧ REQ-DIAG-009)

λ S2_take_nth_function_contract(x). take_nth(stride, source) → coordinate_stride_selection_across_one_source_window
  | mechanism(x) → share_offset_selection_with_bounded_Vector_materialization)
  | responsibility(x) → preserve_original_window_endpoint_callback_and_source_state_rules)
  | traces_to(REQ-FN-045 ∧ REQ-SEQ-022 ∧ REQ-DIAG-009)

λ S2_drop_last_function_contract(x). drop_last(count, source) → coordinate_original_window_prefix_retention
  | mechanism(x) → share_suffix_exclusion_with_bounded_prefix_materialization)
  | responsibility(x) → avoid_second_traversal_or_result_cache)
  | traces_to(REQ-FN-043 ∧ REQ-SEQ-022 ∧ REQ-DIAG-009)

λ S2_last_function_contract(x). last(source) → coordinate_one_result_last_access_across_collections_MapEntry_and_producers
  | mechanism(x) → use_one_window_final_tracking_and_source_specific_projection_services)
  | responsibility(x) → preserve_no_prepass_no_cache_and_no_input_mutation_boundaries)
  | traces_to(REQ-FN-046 ∧ REQ-SEQ-022 ∧ REQ-DIAG-009)

λ S2_predicate_producer_contract(x). TakeWhile(x) ∨ DropWhile(x)
  | mechanism(x) → coordinate_owned_source_and_predicate_admission_with_fresh_bounded_traversal)
  | responsibility(x) → preserve_adapter_state_copy_effect_scope_and_consumer_integration_boundary)
  | component_boundary(x) → keep_take_while_and_drop_while_boundary_rules_in_requirements_and_specs)
  | classify_as(RequirementsBacked) ∧ trace_to(REQ-FN-041)

λ S2_take_while_producer_contract(x). take_while(predicate, source) → S2_predicate_producer_contract(TakeWhile)(x)
  | examine(each_source_value_once) ∧ yield(initial_consecutive_true_prefix)
  | first_predicate_failure → examine_but_exclude(failing_value) ∧ stop_without_further_source_advance
  | all_true_window → yield(entire_window) ∧ stop_at(window_endpoint)
  | count(x) → count(yielded_prefix_by_fresh_traversal_including_first_false_boundary_probe)
  | require(separate_take_while_implementation_approval_and_verification)
  | trace_to(REQ-FN-039 ∧ REQ-FN-041 ∧ REQ-SEQ-022)

λ S2_drop_while_producer_contract(x). drop_while(predicate, source) → S2_predicate_producer_contract(DropWhile)(x)
  | examine(initial_prefix_until_first_false) ∧ skip(initial_consecutive_true_prefix)
  | first_predicate_failure → retain(failing_value) ∧ yield(remaining_original_window)
    ∧ ¬invoke(predicate_after_first_failure)
  | all_true_window → yield(empty_traversal)
  | first_result_access → permit(scanning_entire_matching_prefix)
  | count(x) → count(yielded_suffix_by_fresh_traversal_without_retesting_suffix_values)
  | require(separate_drop_while_implementation_approval_and_verification)
  | trace_to(REQ-FN-040 ∧ REQ-FN-041 ∧ REQ-SEQ-022)

λ S2_reduce_function_contract(x). reduce(f, source) → coordinate_left_fold_across_supported_source_windows
  | mechanism(x) → share(callable_signature_accumulator_and_source_bound_services)
  | responsibility(x) → fold_one_original_observation_and_preserve_complete_result_semantics)
  | traces_to(REQ-FN-034 ∧ REQ-DIAG-009)

λ S2_checked_arithmetic_contract(x). CheckedArithmeticOperation(x)
  | mechanism(x) → share_operand_admission_result_type_and_checked_failure_services_across_arithmetic_operations)
  | responsibility(x) → preserve_left_fold_ordering_and_preflight_agreement_for_named_operations)
  | component_boundary(x) → keep_operation_specific_arity_and_overflow_rules_in_requirements_and_specs)
  | traces_to(REQ-NUM-015A–REQ-NUM-015L ∧ REQ-NUM-017 ∧ REQ-FN-002N)

λ S2_disj_function_contract(x). disj(set, values...) → admit(CljonicSet(set) ∧ ∀ value: SafeImplicitConversion(Set::value_type, value))
  | arity(values) = 0 → return(unchanged_copy(set))
  | arity(values) > 0 → fold(left_to_right, Set::disj, set, values)
    ∧ absent_or_repeated_value → unchanged_accumulator
  | disj_operation(x) → constexpr ∧ noexcept ∧ non_mutating ∧ non_allocating
  | rejected_collection_or_value(x) → provide(targeted_rejection_diagnostic(x))
  rationale: the canonical disj API accepts Clojure-style repeated removal values; Set removal is idempotent, so later occurrences are harmless absent-value no-ops

λ traceability_coverage_policy(x). active_implementation_backed_specification(x)
  → require(every_obligation_has_traceable_test(x))
  | deferred_behavioral_specification(x) → classify_as(DeferredStatus)
    ∧ preserve(contract ∧ optional_contract_tests)
    ∧ exclude_from(strict_spec_to_code_gate(x))
    ∧ report(deferred_obligations(x))
    ∧ ¬claim(implementation_backed(x) ∨ test_covered(x) ∨ converged(x))
    ∧ preserve(independently_approved_requirements_backed_api_scope(x))

## S1 - Operations

λ S1_operations_intro(x). operations(x) → C++23 ∧ FP_oriented ∧ HeaderOnlyDistribution
  | development(x) → CMake_orchestration ∧ Catch2_v3_testing ∧ deterministic_quality_tooling
  | distribution(x) → single_AmalgamatedHeader_artifact(x)

λ S1_language(x). programming_language_version(x) ≡ C++23
  | paradigm_preference(x) ≡ FP_oriented

λ S1_test(x). test_framework(x) ≡ Catch2_v3
  | compatible_with(C++23)

λ S1_build(x). build_system(x) ≡ CMake
  | compatible_with(C++23)

λ S1_package_manager(x). package_manager(x) ≡ none_vendored_only
  | optional_dev_dependency_manager(x) ≡ none

λ S1_delivery(x). deployment_method(x) ≡ single_amalgamated_header_release_artifact

λ S1_distribution(x). HeaderOnlyDistribution(x)
  | AmalgamatedHeader(x) → usable_with(one_public_include(x))
  | production_sources(x) → remain_header_only(x)
  | generated_distribution(x) → preserve(source_of_truth_in_modular_headers(x))

λ S1_modular_header_names(x). Collection_type_header(x) → file(cljonic-collection-<name>.hpp)
  | Producer_type_header(x) → file(cljonic-producer-<name>.hpp)
  | standalone_FreeFunction_header(x) → file(cljonic-<operation>.hpp) ∧ ¬suffix(-function)

λ S1_ci_cd(x). ci_cd(x) ≡ local_makefile_target_workflow
  | remote_ci_pipeline(x) ≡ not_required_for_current_scope

λ S1_quality_tools(x). linter_formatter(x) ≡ clang_tidy ∧ clang_format
  | optional_static_analysis(x) ≡ cppcheck ∧ include_what_you_use
  | docs(x) ≡ doxygen_html_site

λ S1_interfaces(x). current_interface_types(x) ≡ header_only_member_api
  ∧ concept_constrained_free_functions ∧ callable_collection_lookup ∧ producer_values
  ∧ explicit_materialization_functions
  | required_callable_member_adapters(x) → permitted_when(explicitly_specified_and_behaviorally_equivalent(x))
  | optional_member_wrappers(x) ≡ non_canonical

λ S1_checked_arithmetic_operations(x). active_implementation(x)
  | specification_propagated(REQ-NUM-015A–015H)
  | modular_header(x) → file(cljonic-arithmetic.hpp) ∧ amalgamated_distribution(x) ≡ generated_from(modular_header)
  | focused_tests(x) → cover(operand_admission ∧ result_type_selection ∧ checked_failure ∧ division_semantics ∧ floating_edge_cases ∧ preflight_equivalence)
  | public_documentation(x) → explain(checked_failure_fallback ∧ no_loss_conversion ∧ quotient_remainder_modulus_examples)
  | traces_to(S2_checked_arithmetic_contract ∧ S3_checked_arithmetic_control)

λ S1_collection_storage_and_algorithms(x). collection_storage(x) ≡ ContiguousStorage
  | MapEntry(x) → ContiguousStorage(x) ∧ value_semantics(x)
    ∧ owns(key_field(x) ∧ value_field(x)) ∧ supports(named_field_observation(x))
    ∧ semantic_free_function_access(key ∨ val)(x) → remain(deferred_future_work)
    ∧ semantic_free_function_access(last)(x) → active_under(REQ-FN-046)
    ∧ semantic_free_function_access(first)(x) → return(entry_key(x))
    ∧ fixed_two_element_sequence_behavior(x) → first_is_key ∧ value_is_value ∧ count_two ∧ seq_rejects_map_entry_input
  | search_algorithm(x) ≡ LinearScan
  | unsorted_removal(Map ∨ Set) ≡ SwapAndRemove
  | sequence_representation(seq) ≡ OwningValue(Vector)
  | string_storage(String) ≡ ContiguousStorage(ASCII_bytes) ∧ uncounted_null_terminator

λ S1_value_and_view_model(x). value_returns(x) → prefer(OwningValue)
  | collection_updates(x) → realize(PersistentValueSemantics ∧ DeepCopyUpdate)
  | public_operations(x) → obey(S5_authorized_effect_policy(x))
  | read_only_observation(x) → require(ReadOnlyInteropAccessor ∧ ConstRangeTraversal)
  | contiguous_observation(x) → use(ContiguousConstView) when(complete_active_logical_range_is_contiguous(x))
  | view_lifetime(x) → source_lifetime_bounded(x)

λ S1_sequence_guidance(x). sequence_and_materialization_rules(x) → active_for(approved_operations_and_producers)
  | producer_operation(x) → active(x) when((approved_by(Module4) ∨ approved_predicate_producer_scope(x))
    ∧ governed_by_behavioral_contract(x))

λ S1_sequence_materialization_model(x). unbounded_sequences(x) → represent_as(UnboundedProducer) ∧ attach_synthesis_cap(CollectionMaximumElementCount)
  | producer_family(range ∧ repeat ∧ cycle ∧ iterate ∧ repeatedly) → preserve_semantic_infinity(x) ∧ normalize_effective_bounds(x) ∧ before_materialization(x)
  | producer(x) → expose(count ∧ member(is_finite) ∧ const_begin_end)
  | unbounded_producer(x) → is_finite(x, false) ∧ count(CollectionMaximumElementCount) ∧ traversal_terminates_at_count(x)
  | finite_producer(x) → is_finite(x, true)
    ∧ count(exact_runtime_result_count(x) except(finite_Range(x) → min(complete_cardinality(x), CollectionMaximumElementCount)))
  | cycle(source) → constrain_source_by(CljonicSource) ∧ represent(UnboundedProducer) ∧ require(store(independent_owned_copy(source_value)) ∧ ¬borrow(source_storage) ∧ ¬own(result_storage))
  | producer_source_copy(source) → store(parameters ∧ state(source)) ∧ ¬materialize(source_result_sequence)
  | finite_cycle_source(source) → repeat(source_sequence_from_beginning_after_complete_pass)
  | unbounded_cycle_source(source) → preserve(source_observable_prefix) ∧ ¬require(complete_source_result)
  | Repeat(value) → store_owned(value) ∧ represent(unbounded)
  | Repeat(count ∧ value) → store_owned(value) ∧ store(normalized_runtime_count) ∧ represent(finite) ∧ mirror(Clojure_repeat_n_x)
  | repeat(value) → construct(Repeat(value))
  | repeat(count ∧ value) → construct(Repeat(count ∧ value))
  | counted_Repeat_factory_or_constructor(x) → require(count_first ∧ value_second) ∧ obey(S3_conversion_admission)
    ∧ exclude(value_first_overload ∨ argument_order_inference) ∧ trace_to(REQ-FN-013B ∧ REQ-CONV-002E)
  | counted_Repeat_deduction(x) → deduce(element_type_from_second_argument_value(x))
  | Repeat(x) → exclude(IndexedProducer ∧ CallableLookup ∧ Lookup)
  | Repeat_materialization(x) → emit_owned_value_copy_per_element(x) ∧ use(normalized_runtime_count) when(finite(x))
  | Iterate(step ∧ initial) → store(decay(step) ∧ initial) ∧ require(NothrowCollectionElement(initial)) ∧ require(const_step_invocation_is_noexcept_T_to_T(step))
  | Iterate(x) → emit(initial(x)) ∧ apply(step_to_previous_value(x)) ∧ represent(unbounded) ∧ support(constexpr ∧ runtime)
  | Iterate(x) → permit(named_function ∨ lambda ∨ function_object) ∧ satisfy(CljonicSource) ∧ exclude(IndexedProducer ∧ CallableLookup ∧ Lookup)
  | Repeatedly(step) → store(decay(step)) ∧ require(NothrowCollectionElement(element)) ∧ require(step_invocation_is_noexcept_nullary_to_T(step))
  | Repeatedly(traversal) → invoke(copy(stored_step)) ∧ keep(stored_step_unchanged)
    ∧ preserve(value_held_step_state_per_traversal) ∧ preserve(referenced_or_static_state_sharing)
  | Repeatedly(count ∧ step) → store(decay(step) ∧ normalized_runtime_count) ∧ represent(finite) ∧ mirror(Clojure_repeatedly_n_f)
  | counted_Repeat_or_Repeatedly_factory_or_constructor(x) → normalize_finite_count_by(S3_conversion_admission)
  | counted_Repeat_or_Repeatedly_nonpositive_count(x) → represent(empty_finite_producer) ∧ count(0)
  | counted_Repeatedly_nonpositive_count(x) → exclude(callback_invocation_during_construction_or_traversal)
  | Repeatedly(x) → authorize(only_callback_state_and_side_effects_within_declared_step_contract(x))
    ∧ invoke(step_once_per_produced_element(x)) ∧ ¬invoke(step_during_construction(x))
    ∧ require(callback_is_non_throwing_and_non_allocating(x))
    ∧ obey(S3_effect_control(x))
  | Repeatedly(x) → permit(named_function ∨ lambda ∨ function_object) ∧ satisfy(CljonicSource) ∧ exclude(IndexedProducer ∧ CallableLookup ∧ Lookup) ∧ exclude(step_evaluation_during_construction)
  | semantically_infinite_producer(x) → materialize_at_most(CollectionMaximumElementCount)
  | oversized_finite_producer(x) → materialize_as(BoundedPrefixResult) ∧ adjust_effective_endpoint(x)
  | compile_time_known_capacity_or_representability_failure(x) → reject_at_compile_time(x) ∧ diagnostic_not_result_status(x)
  | effective_size(x) → authoritative_for(free_function_observation ∧ producer_iteration ∧ producer_materialization)
  | Range(x) → range_slice_contract(x) ∧ effective_endpoint_normalized_before_iteration(x) ∧ span_arithmetic_avoids_signed_overflow(x)
  | range_member_accessors(start ∧ end ∧ step) → classify_as(non_canonical)
  | bounded_collection_results(x) → require(explicit ProducerMaterialization)
  | implicit_unbounded_nested_materialization(x) → reject(x)

λ bounded_prefix_semantics(x). oversized_producer(x) → materialize_as(BoundedPrefixResult)
  | prefix_determinism(x) → preserve(source_traversal_order)
  | prefix_cardinality(x) → limited_to(destination_capacity)

λ S1_construction(x). collection_construction(x) ≡ CapacityConstruction
  | literal_deduced(x) → ctad_deduction_guides(x)
  | explicit_capacity(x) → empty_default_construction_valid(x)
  | oversized_initializer(x) → compile_time_failure(x)
  | SourceConstruction(x) → copy_into(owned_bounded_storage(x)) ∧ ¬retain(source_lifetime(x)) ∧ admit_only(external_non_cljonic_range_or_view(x))
  | SameTypeArgumentIsOneElement(x) → single_argument_whose_type_is_element_type(x) → construct_one_element(x) ∧ ¬materialize(x)
  | EnclosureConstruction(x) → sole_cljonic_argument_without_explicit_template_arguments(x) → deduce(element_type ← argument_type ∧ capacity ← one) ∧ produce_one_element(x) ∧ ¬copy(x) ∧ ¬materialize(x)
  | RangeViewMaterialization(x) → preserve(source_traversal_order) ∧ bounded_by(destination_capacity(x))
  | static_extent_source_overflow(x) → compile_time_failure(x)
  | runtime_extent_source_overflow(x) → materialize_as(BoundedPrefixResult)
  | complete_source_materialization(x) → preflight(fits_into) ∧ apply(into)
  | checked_constructor_error_channel(x) → reject(x)

λ S1_type_expression_policy(x). public_template_constraints(x) → prefer(concepts)
  | domain_constraints(x) → encode_as(repo_specific_concepts)
  | traits_usage(x) → allowed_only_if(no_clear_or_portable_concept_form(x))

λ S1_diagnostic_policy(x). compile_time_known_capability_failure(x) → reject_at(public_api_boundary)
  | compile_time_known_capacity_or_representability_overflow(x) → reject_at_compile_time(x)
  | context_dependent_compile_time_failure(x) → prefer(targeted_static_assert_diagnostic(x))
  | diagnostic(x) → identify(relevant_cljonic_capability_capacity_or_value_constraint)
  | diagnostic_wording(x) → specify(meaning_not_compiler_specific_text)
  | runtime_unknown_materialization_cardinality(x) → may_warn(default_maximum_capacity_usage)
  | warning(x) → not_replace(correctness_constraint(x))
  | concept_constraint_diagnostics(x) → enforce_at(public_api_boundary)
  | diagnostic_content(x) → identify(cljonic_capability_or_constraint) ∧ ¬depend_on(compiler_specific_text)

λ S1_constant_evaluation_policy(x). constexpr_eligible_operation(x) → prefer(constexpr)
  | required_compile_time_operation(x) → test_in_constant_expression(x)
  | consteval(x) → use_only_when_compile_time_execution_is_semantically_required
  | valid_input(x) ∧ compile_time_and_runtime_evaluation(x) → produce(equivalent_observable_results)
  | invalid_input(x) → follow(explicit_operation_specific_failure_policy(x))
    ∧ permit(compile_time_rejection_versus_documented_runtime_result(x))
      only_when(explicitly_declared_by_operation_contract(x))

λ S1_user_clarity_policy(x). public_api_constraints(x) → prefer(named_cljonic_concepts)
  | trait_mechanics(x) → hide_inside(cljonic_concepts_namespace)
  | likely_user_error_paths(x) → require(explicit_compile_time_messages)
  | collection_header_examples(x) → focus_on(collection_construction)
  | user_facing_names(x) → prefer(canonical_clojure_terms_like_count_is_empty_and_capacity)
  | cross_cutting_free_operations(x) → document_in(operation_specific_headers)

λ S1_pattern(x). architectural_pattern(x) ≡ functional_core_with_profile_gated_capabilities
  ∧ layered_bounded_value_collection_model

## Cross-Cutting Concept Architecture (S3/S2)

λ concept_architecture_intro(x). public_collection_type(x) ∨ primitive_free_function(x) → participate_in(two_level_concept_model(x))
  | two_level_model(x) → govern(admission ∧ capability) ∧ ¬mix(admission ∨ capability)

### Level 1: CollectionConcept (NominalCollectionRecognition)

λ CollectionConcept_level(x). gate(type) → nominal_cljonic_identity(x) ≡ admission(ClosedNominalCollectionDomain) ∧ classification(CollectionKind)
  | recognition(x) → cljonic_owned_traits(x) ∧ ¬structural_similarity(external_or_standard_container(x))
  | nominal_pattern(x) → applies_to(CljonicVector ∧ CljonicMap ∧ CljonicSet ∧ CljonicQueue ∧ CljonicString)

```cpp
template<class Type>
concept CljonicCollection = detail::is_cljonic_collection_v<Type>;

template<class Type>
concept CljonicVector =
    CljonicCollection<Type> &&
    (detail::collection_kind_of_v<Type> == detail::collection_kind::vector);
```

### Level 1B: CljonicProducer (ProducerConcept) and CljonicSource

λ CljonicProducer_level(x). gate(type) → nominal_producer_identity(x) ≡ admission(producer_domain(x)) ∧ classification(ProducerKind)
  | recognition(x) → cljonic_owned_traits(x) ∧ distinct_domain_from(ClosedNominalCollectionDomain(x))
  | nominal_pattern(x) → applies_to(CljonicRange ∧ CljonicRepeat ∧ CljonicCycle ∧ CljonicIterate ∧ CljonicRepeatedly ∧ TakeWhile ∧ DropWhile)
  | ConstInputRange(x) ≡ std::ranges::input_range<const x>
  | NothrowConstInputRange(x) ≡ ConstInputRange(x) ∧ noexcept(begin ∧ end ∧ dereference ∧ increment ∧ sentinel_compare(x))
  | CljonicSource(x) ≡ (CljonicCollection(x) ∨ CljonicProducer(x)) ∧ NothrowConstInputRange(x)
  | materialization_operations(into ∧ preflight(fits_into)) → constrain_source_by(CljonicSource)
  | producer_observation_api(x) → expose(member(is_finite))

```cpp
template<class Type>
concept CljonicProducer = detail::is_cljonic_producer_v<Type>;

template<class Type>
concept CljonicRange =
    CljonicProducer<Type> &&
    (detail::producer_kind_of_v<Type> == detail::producer_kind::range);

template<class Type>
concept CljonicIterate =
  CljonicProducer<Type> &&
  (detail::producer_kind_of_v<Type> == detail::producer_kind::iterate);

template<class Type>
concept CljonicRepeatedly =
  CljonicProducer<Type> &&
  (detail::producer_kind_of_v<Type> == detail::producer_kind::repeatedly);

template<class Type>
concept CljonicSource =
    (CljonicCollection<Type> || CljonicProducer<Type>) &&
    NothrowConstInputRange<Type>;

```

### Level 1C: CljonicVariant (VariantConcept)

λ CljonicVariant_level(x). gate(type) → nominal_variant_identity(x) ≡ admission(cljonic_variant_traits(x))
  | recognition(x) → cljonic_owned_traits(x) ∧ ¬structural_similarity(std::variant(x))
    ∧ std::variant(x) → ¬admit(x) because(¬cljonic_value_type(x))
  | nominal_pattern(x) → applies_to(cljonic::Variant<Alternatives...>)
  | component_recognition(x) → detail::is_cljonic_variant_v(x) ∧ recurse_into(alternatives(x))
  | walker_mirroring(x) → mirror(contains_floating_point ∧ contains_callable ∧ contains_standard_range)(x)
    ∧ nothrow_equality_recurses_over_alternatives(x)
  | storage_admission(x) → NothrowCollectionElement(alternative(x))
  | equality_admission(x) → ComparableVariantAlternative(alternative(x))

```cpp
template<class Type>
concept CljonicVariant = detail::is_cljonic_variant_v<Type>;

// Alternative admission is layered, mirroring the two-level value model:
// storage admission admits an alternative for map values, vector elements,
// and queue elements; equality admission additionally admits the
// cljonic::Variant itself into equality positions.
template<class Type>
concept NothrowVariantAlternative = NothrowCollectionElement<Type>;

template<class Type>
concept ComparableVariantAlternative =
    NothrowStableEqualityComparable<Type> && NothrowEqualityComparable<Type>;
```

### Level 2: CapabilityConcept (semantic capability gates)

λ CapabilityConcept_level(x). require(admitted_collection(x)) → expose(semantic_capability(x)) to(participate_in(operation(x)))
  | capability(x) → layered_on(CollectionConcept_admission(x))
  | value_capabilities(x) → separate_from(structural_capabilities(x))

λ producer_step_concepts(x). coordinate(IterateStep ∧ RepeatedlyStep)
  | mechanism(x) → define_copyable_nothrow_step_admission_and_result_type_boundaries)
  | responsibility(x) → keep_callback_effect_and_element_type_rules_in_requirements_and_specs)
  | trace_to(REQ-NUM-018)

```

// Collection equality is conditional and component-wise: a collection
// provides operator== exactly when its element/key/value types admit
// stable equality, and comparison never traverses beyond its bounded
// logical count.
λ collection_equality(x). bounded_collection(x) && stable_equality_components(x)
  → provide(operator== (x) with (Vector ∧ String ∧ Queue → order_sensitive(x))
    ∧ (Map ∧ Set → order_insensitive(x)))
  | ¬stable_equality_components(x) → omit(operator== (x))
    ∧ usage_in_equality_position(x) → fail_at_compile_time(x)
  | collection_equality(x) → recurse_into(nested_collections ∧
    producers_with_stable_parameters ∧ composite_values)
    ∧ never_traverse_beyond(bounded_logical_count)

```cpp
// SequenceableCollection is the current observation baseline; it does not imply
// the independent, closed Seqable capability (REQ-CAP-014), which no type outside
// the cljonic-owned collection family can declare. Emptiness is derived by the
// is_empty free function from count(), not required as a collection member.
template<class C>
concept SequenceableCollection =
    CljonicCollection<C> &&
    requires(const C& c) {
      { c.count() } noexcept -> std::same_as<std::size_t>;
    };

template<class C>
concept IndexedCollection =
    CljonicCollection<C> &&
    requires(const C& c, std::size_t i) {
      { c(i) };
      { c.contains(i) } -> std::same_as<bool>;
    };

template<class C>
concept LookupCollection =
    CljonicCollection<C> &&
    requires(const C& c) {
      typename C::lookup_type;
      { c(std::declval<const typename C::lookup_type&>()) };
      { c.contains(std::declval<const typename C::lookup_type&>()) } -> std::same_as<bool>;
    };

template<class C>
concept AssociativeCollection =
    CljonicCollection<C> &&
    requires(const C& c) {
      typename C::key_type;
      typename C::association_value_type;
      { c.can_assoc(std::declval<const typename C::key_type&>(),
                    std::declval<const typename C::association_value_type&>()) } -> std::same_as<bool>;
      { c.assoc(std::declval<const typename C::key_type&>(),
                std::declval<const typename C::association_value_type&>()) } -> std::same_as<C>;
    };

template<class C>
concept ConjableCollection =
    CljonicCollection<C> &&
    requires(const C& c) {
      typename C::value_type;
      { c.conj(std::declval<const typename C::value_type&>()) } -> std::same_as<C>;
      { c.can_conj(std::declval<const typename C::value_type&>()) } -> std::same_as<bool>;
    };
```

### Level 2B: SequenceableProducer (producer baseline capability gate)

λ SequenceableProducer_level(x). require(admitted_producer(x)) → expose(non_throwing_count(x)) to(participate_in(operation(x)))
  | capability(x) → layered_on(CljonicProducer_admission(x))
  | finite_producer_count(x) → exact_runtime_result_count(x)
    except(finite_Range(x) → min(complete_cardinality(x), CollectionMaximumElementCount))
  | unbounded_producer_count(x) → bounded_observable_traversal_cap(x) ∧ saturated_by(CollectionMaximumElementCount) ∧ ¬equivalent(complete_cardinality(x))
  | current_producer_family(x) → provide(is_finite(x) ∧ const_begin_end_traversal_bounded_by_count(x))
    ∧ family_guarantees(x) → ¬additional_checks_of(SequenceableProducer(x))
  | ¬imply(IndexedProducer(x) ∨ LookupCollection(x)) because(no_callable_operator_parenthesis(x))

```cpp
template<class C>
concept SequenceableProducer =
    CljonicProducer<C> &&
    requires(const C& c) {
      { c.count() } noexcept -> std::same_as<std::size_t>;
    };
```

### Level 2C: IndexedProducer (reserved producer positional-access gate)

λ range_slice_contract(x). Range(x) → provide(x.contains(index))
  ∧ x.contains(index) ↔ index < x.count()
  ∧ ¬provide(contains(x, index) ∨ get(x, key) ∨ x(index))
  ∧ ¬satisfy(Indexed ∨ IndexedProducer ∨ Lookup ∨ Associative)(x)
  ∧ ¬provide(positional_value_retrieval(x))

λ IndexedProducer_level(x). require(admitted_producer(x)) → expose(positional_value_retrieval(x) ∧ available_index_predicate(x)) to(participate_in(operation(x)))
  | capability(x) → layered_on(CljonicProducer_admission(x))
  | Range(x) → range_slice_contract(x)
  | IndexedProducer(x) ≡ non_mutating_O1_positional_value_retrieval(x) ∧ available_index_predicate_over_same_bounded_domain(x)
  | producer_contains(x) → bounded_observation_availability_only(x) ∧ ¬imply(positional_value_retrieval(x) ∨ IndexedProducer(x))
  | IndexedProducer(x) ¬imply(CallableLookup(x)) because(Indexed_access_does_not_imply_invocability(x))
  | ¬expose(callable_operator_parenthesis(x)) on(producer(x)) because(operator_parenthesis_reserved_for_CallableLookup_mirroring(x))
  | positional_value_retrieval(x) → supported_by(take_nth_under(REQ-FN-045)) ∧ reserved_for(future_nth_position_api)
  | current_producers(x) ≡ Range ∨ Repeat ∨ Cycle ∨ Iterate ∨ Repeatedly ∨ TakeWhile ∨ DropWhile
  | ∀ current_producer(x) → ¬satisfy(IndexedProducer(x))
  | future_IndexedProducer(x) → require(explicit_positional_access_contract(x) ∧ matching_availability_predicate(x))

### Concept naming, scoping, and evolution

λ concept_guidelines_alignment(x). cpp_code(x) → follows(C++CoreGuidelines)
  | concept_identifier(x) → type_level_predicate(x) ∧ pascal_case(x) ∧ governed_by(NL.17)
  | function_identifier(x) → snake_case(x) ∧ namespace_identifier(x) → lowercase(x)
  | nominal_concept(x) → keep(Cljonic_domain_prefix) because(names_closed_collection_domain(x))
  | structural_or_value_capability_concept(x) → ¬require(Cljonic_prefix) because(gated_on_nominal_identity(x) ∧ scope_carried_by(suffix ∨ comparative_name))

λ clojure_library_names(x).
  clojure.core → cljonic::core
  | clojure.set → cljonic::lib_set
  | clojure.string → cljonic::lib_string
  | ∀ core_function(x) → declare_in(cljonic::core) ∧ ¬declare_in(cljonic::)
  | ∀ core_collection_type(x) → retain_in(cljonic::)
  | set_algebra_identifier(x) → prepend(set_, snake_case(x)) when(x ∈ {union, intersection, difference})
  | set_relation_predicate(x) → preserve(is_subset ∨ is_superset)

λ concept_naming(x). CollectionConcept_names(x) → identify_nominal_identity(x)
  | CapabilityConcept_names(x) → identify_semantic_capability(x)
  | public_concept_names(x) → capability_oriented(x) ∧ reject(vague_names)
  | concept_identifiers(x) → pascal_case(x) ∧ distinct_from_functions_and_namespaces(x)

λ concept_scoping(x). nominal_and_capability_concepts(x) → live_in(cljonic::concepts)
  | trait_mechanics(x) → hide_inside(cljonic::concepts_detail) ∨ detail
  | standard_concepts(x) → prefer_at_public_api_boundary

λ concept_storage_orthogonality(x). storage_admission(x) → not_imply(StableEquality ∨ TotalOrder ∨ other_capability)
  | capability_required_by(operation) → constrain_at(public_api_boundary)

λ concept_evolution(x). concepts_gate_free_functions(x)
  | minimal_concept_requirements(x) → only_what_current_functions_need(x)
  | future_requirements(x) → add_when_first_function_needs_them(x)
  | introduce_concept(x) → require(tested_api_boundary(x)) ∧ specify(consumers_and_diagnostics(x))

λ concept_member_naming(x). canonical_cardinality_member(x) ≡ count
  | is_empty(value) ↔ count(value) == 0
  | is_empty(x) → ¬require(collection_member_is_empty(x))
  | member_lookup(x) ≡ operator()(key, fallback = default_result) ∧ contains
  | index_bracket_lookup(x) → omitted_from_all_collections(x)

## Coherence Constraints (S5 → S1)

λ coherence_identity(x). S1 ∧ S2 ∧ S3 ∧ S4 → must_preserve(S5_identity)

λ coherence_control(x). operational_choice(x) → reject_if(violates_S3_constraints)

λ coherence_adaptation(x). adaptation(x) → valid_only_if(backward_compatible_vocabulary ∧ S5_authorized_effect_policy)

λ coherence_type_expression(x). implementation_expression(x) → reject_if(uses_traits_where_clear_concept_exists)

λ coherence_operation_surface(x). collection_operation_change(x) → reject_if(renames_or_redefines_canonical_operation_without_constraint_rationale)

λ coherence_result_semantics(x). architecture_change(x) → reject_if(omits_result_status_classification_or_preflight_alignment)

λ coherence_lifecycle_governance(x). api_scope_change(x) → reject_if(missing_LifecycleClassification_or_missing_RelationModel_gate)

## Traceability

λ traceability_authorities(x). foundation_authority(x) ≡ requirements/requirements-module-1.md ∧ Module1(nominal ∧ storage ∧ no_runtime_service ∧ persistent_value boundaries)
  | capability_authority(x) ≡ requirements/requirements-module-2.md ∧ Module2(concept ∧ result_status ∧ preflight ∧ diagnostic ∧ constant_evaluation ∧ vocabulary requirements)
  | vocabulary_authority(x) ≡ vocabulary.md ∧ canonical_terms(x) → govern(public_names ∧ architecture_references ∧ specifications ∧ tests)
  | module3_stored_collection_authority(x) ≡ requirements/requirements-module-3.md → govern(concrete_storage ∧ linear_scan ∧ swap_and_remove ∧ primitive_free_functions)
  | module4_producer_authority(x) ≡ requirements/requirements-module-4.md → govern(producers ∧ materialization ∧ interop)
  | module5_algorithm_authority(x) ≡ requirements/requirements-module-5.md → govern(composition ∧ traversal_family)
  | module6_numeric_authority(x) ≡ requirements/requirements-module-6.md → govern(checked_arithmetic ∧ callables)
  | module7_extended_domain_authority(x) ≡ requirements/requirements-module-7.md → govern(relations ∧ set_algebra ∧ regex ∧ Atom ∧ formatting ∧ KeywordEnumNameMapping)

λ current_implementation_boundary(x). active_surface(x) ≡ Vector ∧ MapEntry ∧ Map ∧ Set ∧ Queue ∧ String ∧ Variant ∧ Range ∧ Repeat ∧ Cycle ∧ Iterate ∧ Repeatedly ∧ TakeWhile ∧ DropWhile ∧ primitive_free_functions ∧ checked_arithmetic_operations ∧ explicit_conversions ∧ map ∧ reduce ∧ filter ∧ filterv ∧ take ∧ drop ∧ take_while ∧ drop_while ∧ vector_factory ∧ hash_map_factory ∧ map_entry_factory ∧ set_factory ∧ queue_factory ∧ string_factory ∧ variant_factory ∧ into ∧ fits_into
  | approved_but_unimplemented(algorithms_except(map ∧ reduce ∧ filter ∧ filterv ∧ take ∧ drop ∧ take_while ∧ drop_while) ∨ regexes) → remain_outside(active_surface(x))
  | approved_but_unimplemented(x) → require(specification_and_implementation_propagated(x)) before(active_surface_inclusion(x))
