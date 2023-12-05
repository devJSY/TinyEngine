xcopy /s /y  /exclude:exclude_list.txt ".\Project\Engine\*.h" ".\External\Include\Engine"
xcopy /s /y  /exclude:exclude_list.txt ".\Project\Engine\*.inl" ".\External\Include\Engine"
xcopy /s /y  /exclude:exclude_list.txt ".\Project\Engine\*.fx" ".\OutputFile\content\shader"