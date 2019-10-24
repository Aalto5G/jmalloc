#!/bin/sh
cat straceout.txt |grep brk|awk --non-decimal-data "{print \$3-$(cat straceout.txt |grep brk|head -1|awk --non-decimal-data '{print $3+0}')}"
