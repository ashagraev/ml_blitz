tawk '{print $0, 0}' fr_test.tsv > fr_test.fixed
catboost fit -f fr_learn.tsv --cd fields.cd -t fr_test.fixed
catboost calc --input-path fr_test.fixed --cd fields.cd

wc -l fr_learn.tsv                                          # 1000000
tawk '$8 > 0' fr_learn.tsv | wc -l                          # 727628

tawk 'NR > 1 {if ($2 > 0.04) print 1; else print 0}' output.tsv > solution.tsv
tawk '$1' solution.tsv | wc -l                              #178925
