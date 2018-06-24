tawk '{print $0, 0}' fr_test.tsv > fr_test.fixed
catboost fit -f fr_learn.tsv --cd fields.cd -t fr_test.fixed
catboost calc --input-path fr_test.fixed --cd fields.cd

wc -l fr_learn.tsv
tawk '$8 > 0' fr_learn.tsv | wc -l

tawk 'NR > 1 {if ($2 > 0.07) print 1; else print 0}' output.tsv > solution.tsv
tawk '$1' solution.tsv | wc -l   #68261
