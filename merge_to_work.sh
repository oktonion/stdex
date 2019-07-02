git checkout .
git remote update
git fetch
git checkout -b tmp
git checkout tmp
git merge -m "merging tested lib" origin/tests

git filter-branch --prune-empty --subdirectory-filter stdex --
git commit -m 'merged with tested lib'

git checkout -b work
git checkout work
git fetch origin
git merge --allow-unrelated-histories --no-commit origin/tmp
git rm -r stdex
git read-tree --prefix=stdex -u origin/tmp
git commit -m 'merged with tested lib'