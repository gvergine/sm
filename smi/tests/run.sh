BINDIR=$1
EXE=$BINDIR/smi

# prints "usage" when running with --help
$EXE --help | grep Usage
