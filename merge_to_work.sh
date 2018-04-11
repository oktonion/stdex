git fetch
git checkout tmp
git merge -m "merging tested lib" origin/tests

git filter-branch --prune-empty --subdirectory-filter stdex --
git commit -m 'merged with tested lib'

git checkout work
git fetch origin
git merge --allow-unrelated-histories --no-commit origin/tmp
git rm -r stdex
git read-tree --prefix=stdex -u origin/tmp
git commit -m 'merged with tested lib'