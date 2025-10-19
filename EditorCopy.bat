del /q /f ".\External\Include\Editor\*.*"
xcopy /s /y  /exclude:Editor_exclude_list.txt ".\Project\Editor\*.h" ".\External\Include\Editor"
xcopy /s /y  /exclude:Editor_exclude_list.txt ".\Project\Editor\*.inl" ".\External\Include\Editor"