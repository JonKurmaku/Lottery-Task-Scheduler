#!/bin/bash

echo "======================================================"
echo " Starting Hackbench Scheduler Stress Test"
echo "======================================================"

# Clear the screen output slightly for readability
echo "Running hackbench (this will spawn hundreds of tasks)..."

# Run hackbench: 
# -s 512 (message size)
# -l 200 (messages per process)
# -g 15  (number of groups)
# -f 20  (number of file descriptors per group)
time hackbench -s 512 -l 200 -g 15 -f 20 -P

echo ""
echo "=== Top 10 Tasks by Context Switches ==="
# Read your custom log, sort it numerically by the 3rd column (CtxSw) in descending order
cat /proc/sched_log | sort -rn -k3 | head -n 11

echo ""
echo "Notice how the hackbench tasks accumulated massive context switch counts!"