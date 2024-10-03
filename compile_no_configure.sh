opt="$(echo $1 | sed -e "s/-O0/$(cat /etc/gcc.opt)/g") -Wno-error -finline-limit=2"
make clean > /dev/null
make EXTRA_CFLAGS="$opt" -n > log_make.txt
if ! make EXTRA_CFLAGS="$opt"; then
	echo "Error make"
	exit 1
fi
