#!/bin/sh
# Run each SDK unit test individually.

# Remove any stale profiling gunk
find . -name '*.gcda' -print0 | xargs -0 rm

test_exec_files=`find build -type f -not -wholename '*/_deps/*' -not -wholename '*/CMakeFiles/*' -perm -a+x`

for test_exec in $test_exec_files ; do
	echo "---------- $test_exec ----------"
	$test_exec
done
