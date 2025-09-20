@echo off
call cloc . --exclude-dir=.vs --include-content="\.(h|c(pp)?)" --exclude-lang=JSON,XML
call cloc . --exclude-dir=.vs --include-content="\.(h|c(pp)?)" --exclude-lang=JSON,XML --md
pause
exit