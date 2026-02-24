#!/bin/bash

echo "======================================================"
echo " Starting Lottery Scheduler Fairness & Logging Test"
echo "======================================================"

# 1. Spawn a High-Priority CPU bound task (More tickets)
# 'nice -n -15' gives it a high weight
nice -n -15 stress-ng --cpu 1 --cpu-method matrix --quiet &
HIGH_PID=$!

# 2. Spawn a Low-Priority CPU bound task (Fewer tickets)
# 'nice -n 15' gives it a low weight
nice -n 15 stress-ng --cpu 1 --cpu-method matrix --quiet &
LOW_PID=$!

echo "Started High Priority (Nice -15) Task -> PID: $HIGH_PID"
echo "Started Low Priority  (Nice +15) Task -> PID: $LOW_PID"
echo "Letting them run and compete for CPU time via Lottery for 15 seconds..."

sleep 15

echo ""
echo "=== Scheduler Log Results (/proc/sched_log) ==="
# Print the header and the specific lines for our two stress-ng tasks
head -n 1 /proc/sched_log
grep -E "^($HIGH_PID|$LOW_PID)[[:space:]]" /proc/sched_log

echo ""
echo "Stopping stress workers..."
kill $HIGH_PID $LOW_PID
wait $HIGH_PID $LOW_PID 2>/dev/null

echo "Test Complete. The High Priority task should have significantly higher RunTime(ns)."