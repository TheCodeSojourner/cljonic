########################################################################################################################
## Makefile - cljonic
########################################################################################################################

########################################################################################################################
## Default make action
default: help
	@echo

########################################################################################################################
## Rebuild and execute unit test program
all: FORCE
	@scripts/make-format.sh
	@scripts/make-cljonic.sh
	@scripts/make-noheap.sh
	@scripts/make-all.sh unit-test
	@echo

########################################################################################################################
## Delete build artifacts
clean: FORCE
	@scripts/make-format.sh
	@scripts/make-clean.sh
	@echo

########################################################################################################################
## Builds the cljonic.hpp single header file
cljonic: FORCE
	@scripts/make-format.sh
	@scripts/make-cljonic.sh
	@echo

########################################################################################################################
## Rebuild, in gcov mode, execute unit test program, and generates coverage
coverage: FORCE
	@scripts/make-format.sh
	@scripts/make-coverage.sh
	@echo

########################################################################################################################
## Execute cppcheck on source files
cppcheck: FORCE
	@scripts/make-format.sh
	@scripts/make-cppcheck.sh
	@echo

########################################################################################################################
## Generate doxygen documentation
doc: FORCE
	@scripts/make-format.sh
	@scripts/make-doc.sh
	@echo

########################################################################################################################
## Generate list of files containing functions with 'lizard forgives', which disables metrics notifications
forgives: FORCE
	@scripts/make-forgives.sh
	@echo

########################################################################################################################
## Format all files with clang-format
format: FORCE
	@echo
	@echo "========================================"
	@echo "== make format"
	@echo "========================================"
	@scripts/make-format.sh
	@echo

########################################################################################################################
## Prepare for git commit by doing 'make clean format'
git: clean format cljonic doc
	@echo
	@echo "==============================================================================="
	@echo "== git status"
	@echo "==============================================================================="
	git status
	@echo

########################################################################################################################
## Display help
help: FORCE
	@echo
	@echo "==============================================================================================================="
	@echo "== cljonic Make Help"
	@echo "==============================================================================================================="
	@echo
	@echo "Usage:"
	@echo "    'make'                generates this help information"
	@echo "    'make all'            cleans, and rebuilds and executes unit test program"
	@echo "    'make clean'          deletes build environment and artifacts"
	@echo "    'make coverage        does a 'make all' in gcov mode, executes unit test program, and generates coverage"
	@echo "    'make cppcheck'       executes cppcheck on source files"
	@echo "    'make cljonic'        builds the cljonic.hpp single header file"
	@echo "    'make doc'            generate doxygen documentation"
	@echo "    'make format'         formats all files with clang-format"
	@echo "    'make forgives'       generates list of code/source files with disabled metrics notifications"
	@echo "    'make git'            prepare for git commit by doing 'make clean format doc cljonic'"
	@echo "    'make help'           generates this help information"
	@echo "    'make lcov'           does 'make test' then displays coverage analysis HTML in browser [1] [2]"
	@echo "    'make memcheck'       does 'make all' then executes unit test program in memcheck mode"
	@echo "    'make metrics'        generates metrics violations for code/source"
	@echo "    'make noheap'         verify that cljonic will compile without heap allocation allowed"
	@echo "    'make test'           builds only required files, and executes unit test program"
	@echo
	@echo "Note that 'make' will accept multiple targets, which it will evaluate in order.  For example"
	@echo "'make clean lcov'."
	@echo
	@echo "[1] Note that if one wishes to execute 'make lcov' one must execute 'make coverage' or 'make clean lcov'"
	@echo "first in order to ensure that all code is compiled for gcov mode.  Once 'make coverage' or 'make clean lcov'"
	@echo "has been executed one must execute 'make all', or 'make clean' in order to ensure that all code is compiled"
	@echo "for test mode."
	@echo
	@echo "[2] Note that the only interesting lines on the output are the ones for 'code/source/*'."
	@echo

########################################################################################################################
## Does 'make test' then displays coverage analysis HTML in browser
lcov: FORCE
	@scripts/make-format.sh
	@scripts/make-lcov.sh
	@sensible-browser build/html/index.html 2>/dev/null >/dev/null &
	@echo

########################################################################################################################
## Does 'make all' then executes unit test program in memcheck mode
memcheck: FORCE
	@scripts/make-memcheck.sh
	@echo

########################################################################################################################
## Generates metrics for source code with lizard
metrics: FORCE
	@scripts/make-format.sh
	@scripts/make-metrics-source.sh
	@echo

########################################################################################################################
## Verify that cljonic will compile without heap allocation allowed
noheap: FORCE
	@scripts/make-format.sh
	@scripts/make-cljonic.sh
	@scripts/make-noheap.sh
	@echo

########################################################################################################################
## Builds only required files, and executes unit test program
test: FORCE
	@scripts/make-format.sh
	@scripts/make-unit-test.sh unit-test
	@echo

FORCE:
