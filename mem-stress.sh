#!/usr/bin/env sh

(echo -1000 > /proc/self/oom_score_adj && exec /usr/local/bin/mem-stress "$@")
