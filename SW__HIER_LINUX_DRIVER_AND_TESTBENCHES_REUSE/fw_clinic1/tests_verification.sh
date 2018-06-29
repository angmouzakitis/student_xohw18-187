#!/bin/bash

cat logs_all | grep -A3 "run_readok.x" | grep "^readPatientData" > /dev/null 2>&1 && echo "run_readok.x: OK" || echo "run_readok.x: FAILED"

cat logs_all | grep -A3 "run_readdenied.x" | grep "not found" > /dev/null 2>&1 && echo "run_readdenied.x: OK" || echo "run_readdenied.x: FAILED"

cat logs_all | grep -A2 "run_writedenied.x" | grep "Segmentation fault" > /dev/null 2>&1 && echo "run_writedenied.x: OK" || echo "run_writedenied.x: FAILED"
