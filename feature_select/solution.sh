catboost fit --cd train.cd -f train.txt
catboost fstr --input-path train.txt --cd train.cd
head -30 feature_strength.tsv | sed 's/f//g' | awk '{printf $2 " "} END {print ""}'

# result: 193 129 292 48 337 283 111 208 345 110 14 482 28 241 469 136 483 175 449 90 119 97 7 117 289 435 374 30 264 163