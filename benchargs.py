
# Update this list to change on how many cores
# each program is run. This is a global setting.
#core_counts = [16]
core_counts = [ 1, 5, 10, 15, 20, 25, 30, 35, 40, 80 ]

"""
NOTE: Any benchmark that has 'se' in the name will be run on ONE core, and will not
      run more than `runs` times [as opposed to `len(core_counts)*runs` times].
"""

# A map of the benchmark to run to its arguments and
# the number of times the benchmark should be run.
# See the NOTE sections above for important details.
#
# The same benchmark can be run with different arguments
# in the same execution of the script by adding another
# entry with a number appended to the binary name.
bench_args = {
        'fib-mr-fj1': { 'args': '-n 30 -b 15 -count 5000 -delay 0', 'runs': 10},
        'fib-mr-l1': { 'args': '-n 30 -b 15 -count 5000 -delay 0', 'runs': 10},
        'fib-mr-l2': { 'args': '-n 30 -b 15 -count 5000 -delay 1000', 'runs': 10},
        'fib-mr-fj2': { 'args': '-n 30 -b 15 -count 5000 -delay 1000', 'runs': 10},
        'fib-mr-l3': { 'args': '-n 30 -b 15 -count 5000 -delay 50000', 'runs': 10},
        'fib-mr-fj3': { 'args': '-n 30 -b 15 -count 5000 -delay 50000', 'runs': 10},
        'fib-mr-l4': { 'args': '-n 30 -b 15 -count 5000 -delay 100000', 'runs': 10},
        'fib-mr-fj4': { 'args': '-n 30 -b 15 -count 5000 -delay 100000', 'runs': 10},
}
