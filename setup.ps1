$vsexist=0
$gccexist=0
echo "�܂��̓A�C�R��������"
windres icon.rc -O coff -o icon.res #�A�C�R���t�@�C���̍쐬
echo "�A�C�R�����ł�����"
If((Test-Path .\3.22c_VS) -eq "TRUE"){
    $vsexist=1
}
If((Test-Path .\7_3_0) -eq "TRUE"){
    $gccexist=1
}
If($vsexist -eq 1 -And $gccexist -eq 1){
    echo "���̃X�N���v�g�͂���Ȃ���"
    exit(0)
}
If($vsexist -eq 0){
    echo "DX���C�u������VS�ł̃Z�b�g�A�b�v���n�߂܂�!!"
    echo "DX���C�u�����̃_�E�����[�h���n�߂܂�!!"
    Invoke-WebRequest -Uri "https://dxlib.xsrv.jp/DxLib/DxLib_VC3_22c.zip" -OutFile "dxlib.zip"
    echo "DX���C�u�����̉𓀂��n�߂܂�!!"
    Expand-Archive -Path "dxlib.zip" -DestinationPath "."
    New-Item -Path "3.22c_VS" -ItemType Directory
    Copy-Item ".\DxLib_VC\�v���W�F�N�g�ɒǉ����ׂ��t�@�C��_VC�p\*"  -Destination "./3.22c_VS"
    Remove-Item -Recurse -Path "DxLib_VC"
    Remove-Item "dxlib.zip"
}
If($gccexist -eq 0){
    echo "DX���C�u������GCC�ł̃Z�b�g�A�b�v���n�߂܂�!!"
    echo "DX���C�u�����̃_�E�����[�h���n�߂܂�!!"
    Invoke-WebRequest -Uri "https://dxlib.xsrv.jp/DxLib/DxLib_GCC3_22c.zip" -OutFile "dxlib.zip"
    echo "DX���C�u�����̉𓀂��n�߂܂�!!"
    Expand-Archive -Path "dxlib.zip" -DestinationPath "."
    New-Item -Path "7_3_0" -ItemType Directory
    Copy-Item ".\DxLib_GCC\�v���W�F�N�g�ɒǉ����ׂ��t�@�C��_GCC(MinGW)�p\7_3_0_x86_64_w64_win32_seh_rt_v5_rev0\*"  -Destination "./7_3_0"
    Remove-Item -Recurse -Path "DxLib_GCC"
    Remove-Item "dxlib.zip"
    echo "�Z�b�g�A�b�v�I�������!!"
    echo "�v���O���~���O����΂�!!" 
}

