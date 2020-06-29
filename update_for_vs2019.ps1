## run script to change PlatformToolset from v120 to v140

$NewToolset = '<PlatformToolset>v142'
$OldToolsets = @(
"<PlatformToolset>v120_xp",
"<PlatformToolset>v120"
)

$Paths = @(
"proj",
"sdks/bullet3/build3/vs2010",
"sdks/freetype-2.6/builds/windows/vc2013/",
"sdks/freetype-2.10.0/builds/windows/vc2010/",
"sdks/graphix/proj/",
"sdks/graphix/sdks/common/proj/",
"sdks/mital/proj/vs2013/mital/",
"source/common/proj/"
)

For ($i=0; $i -lt $Paths.Length; $i++) {
    ForEach ($File in (Get-ChildItem -Path $Paths[$i] -Filter *.vcxproj -Recurse | %{$_.FullName})) {
		ForEach ($OldToolset in $OldToolsets) {
			(Get-Content $File) -Replace $OldToolset,$NewToolset |
				Set-Content $File
		}
	}
}
