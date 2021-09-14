$vsexist=0
$gccexist=0
echo "まずはアイコンを作るよ"
windres icon.rc -O coff -o icon.res #アイコンファイルの作成
echo "アイコンができたよ"
If((Test-Path .\3.22c_VS) -eq "TRUE"){
    $vsexist=1
}
If((Test-Path .\7_3_0) -eq "TRUE"){
    $gccexist=1
}
If($vsexist -eq 1 -And $gccexist -eq 1){
    echo "このスクリプトはいらないよ"
    exit(0)
}
If($vsexist -eq 0){
    echo "DXライブラリのVS版のセットアップを始めます!!"
    echo "DXライブラリのダウンロードを始めます!!"
    Invoke-WebRequest -Uri "https://dxlib.xsrv.jp/DxLib/DxLib_VC3_22c.zip" -OutFile "dxlib.zip"
    echo "DXライブラリの解凍を始めます!!"
    Expand-Archive -Path "dxlib.zip" -DestinationPath "."
    New-Item -Path "3.22c_VS" -ItemType Directory
    Copy-Item ".\DxLib_VC\プロジェクトに追加すべきファイル_VC用\*"  -Destination "./3.22c_VS"
    Remove-Item -Recurse -Path "DxLib_VC"
    Remove-Item "dxlib.zip"
}
If($gccexist -eq 0){
    echo "DXライブラリのGCC版のセットアップを始めます!!"
    echo "DXライブラリのダウンロードを始めます!!"
    Invoke-WebRequest -Uri "https://dxlib.xsrv.jp/DxLib/DxLib_GCC3_22c.zip" -OutFile "dxlib.zip"
    echo "DXライブラリの解凍を始めます!!"
    Expand-Archive -Path "dxlib.zip" -DestinationPath "."
    New-Item -Path "7_3_0" -ItemType Directory
    Copy-Item ".\DxLib_GCC\プロジェクトに追加すべきファイル_GCC(MinGW)用\7_3_0_x86_64_w64_win32_seh_rt_v5_rev0\*"  -Destination "./7_3_0"
    Remove-Item -Recurse -Path "DxLib_GCC"
    Remove-Item "dxlib.zip"
    echo "セットアップ終わったよ!!"
    echo "プログラミングがんばれ!!" 
}

