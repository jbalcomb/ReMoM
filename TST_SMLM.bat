@ECHO OFF

if not exist build (mkdir build)

pushd build

cl -FC -Zi ..\MoM_Rasm\TST_SMLM.C

popd
