#!/bin/bash

cd ../result-data/lcm_log
rm data.lcm
#lcm-logger ./data$(date +%F).lcm
lcm-logger ./data.lcm
