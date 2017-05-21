./lemon lpmln_parser.y

rm -f output.mln

re2c -o lpmln_lexer.cc lpmln_lexer.re

g++ -o lpmln_to_mln lpmln_to_mln.cc

#echo 'Starting translation for ../sample_propositional_logic.lpmln : '

#./lpmln_to_mln ../sample_propositional_logic.lpmln sample_propositional_logic.mln

#echo 'Below is the generate file for ../sample_propositional_logic.lpmln : '

#cat sample_propositional_logic.mln

#echo 'Starting translation for ../sample_propositional_logic2.lpmln : '

#./lpmln_to_mln ../sample_propositional_logic2.lpmln  sample_propositional_logic2.mln

#echo 'Below is the generate file for ../sample_propositional_logic2.lpmln : '

#cat sample_propositional_logic2.mln

#echo 'Starting translation for ../sample_propositional_logic3.lpmln : '

#./lpmln_to_mln ../sample_propositional_logic3.lpmln sample_propositional_logic3.mln

#echo 'Below is the generate file for ../sample_propositional_logic3.lpmln : '

#cat sample_propositional_logic3.mln

#echo 'Starting translation for ../sample_propositional_logic4.lpmln : '

#./lpmln_to_mln ../sample_propositional_logic4.lpmln sample_propositional_logic4.mln

#echo 'Below is the generate file for ../sample_propositional_logic4.lpmln : '

#cat sample_propositional_logic4.mln

echo 'Starting translation for ../sample_propositional_logic5.lpmln : '

./lpmln_to_mln ../sample_propositional_logic5.lpmln sample_propositional_logic5.mln

echo 'Below is the generate file for ../sample_propositional_logic5.lpmln : '

cat sample_propositional_logic5.mln



