#!/bin/bash

echo "======================================================"
echo " Starting lmbench Context Switch Latency Test"
echo "======================================================"

# Determine where lat_ctx is installed (depends on your distro)
LAT_CTX_BIN=$(command -v lat_ctx)
if [ -z "$LAT_CTX_BIN" ]; then
    # Fallback to common compiled path if not in standard PATH
    LAT_CTX_BIN="/usr/lib/lmbench/bin/x86_64-linux-gnu/lat_ctx"
fi

if [ ! -f "$LAT_CTX_BIN" ]; then
    echo "Error: lat_ctx (lmbench) not found. Please compile/install lmbench."
    exit 1
fi

echo "Running Context Switch Latency benchmark (lat_ctx)..."
echo "Size: 32KB. Processes: 2, 4, 8, 16, 24, 32"
echo ""

# Run lat_ctx with a 32k data size footprint across various process counts
$LAT_CTX_BIN -s 32 2 4 8 16 24 32

echo ""
echo "Check your /proc/sched_log to see lat_ctx runtime stats:"
grep "lat_ctx" /proc/sched_log