set CURPATH=%cd%
cd ../Tool/commonTool-win-x64/
CommonTool.exe gen table --target-path ../../RawContent/Csv --output ../../Source/AZ_MHW/CommonSource/Table
cd %CURPATH%
PAUSE