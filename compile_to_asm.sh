opt="$(echo $3 | sed -e "s/-O0/$(cat /etc/gcc.opt)/g") -Wno-error -finline-limit=2"
if ! gcc -Wall -Wextra -pedantic -std=c99 -I../include -masm=intel -fPIC -fvisibility=hidden "$1" -S -o "$2" $opt; then
	echo "Error compiling to asm $opt - malloc"
	exit 1
fi

