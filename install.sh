#!/bin/sh

if [ '!' -f "lib/libjmalloc.a" -o '!' -f "lib/libjmalloc.so" -o '!' -f "lib/libjmalloc.so.1" ]; then
  echo "jmalloc not made"
  exit 1
fi

PREFIX="$1"

if [ "a$PREFIX" = "a" ]; then
  PREFIX=/usr/local
fi

P="$PREFIX"
H="`hostname`"

if [ '!' -w "$P" ]; then
  echo "No write permissions to $P"
  exit 1
fi
if [ '!' -d "$P" ]; then
  echo "Not a valid directory: $P"
  exit 1
fi

instlib2()
{
  if [ -e "$P/lib/$1/$2" ]; then
    ln "$P/lib/$1/$2" "$P/lib/.$2.jmallocinstold.$$.$H" || exit 1
  fi
  cp "$1/$2" "$P/lib/.$2.jmallocinstnew.$$.$H" || exit 1
  mv "$P/lib/.$2.jmallocinstnew.$$.$H" "$P/lib/$2" || exit 1
  if [ -e "$P/lib/.$2.jmallocinstold.$$.$H" ]; then
    # If you mount binaries across NFS, and run this command on the NFS server,
    # you might want to comment out this rm command.
    rm "$P/lib/.$2.jmallocinstold.$$.$H" || exit 1
  fi
}
instinc2()
{
  if [ -e "$P/include/$2" ]; then
    ln "$P/include/$2" "$P/include/.$2.jmallocinstold.$$.$H" || exit 1
  fi
  cp "$1/$2" "$P/include/.$2.jmallocinstnew.$$.$H" || exit 1
  mv "$P/include/.$2.jmallocinstnew.$$.$H" "$P/include/$2" || exit 1
  if [ -e "$P/include/.$2.jmallocinstold.$$.$H" ]; then
    # If you mount binaries across NFS, and run this command on the NFS server,
    # you might want to comment out this rm command.
    rm "$P/include/.$2.jmallocinstold.$$.$H" || exit 1
  fi
}
instsym()
{
  if [ "`readlink "$P/lib/$1"`" != "libjmalloc.so.1" ]; then
    ln -s libjmalloc.so.1 "$P/lib/.$1.jmallocinstnew.$$.$H" || exit 1
    mv "$P/lib/.$1.jmallocinstnew.$$.$H" "$P/lib/$1" || exit 1
  fi
}

instlib2 lib libjmalloc.a
instlib2 lib libjmalloc.so.1
instsym libjmalloc.so

instinc2 lib jmalloclib.h

echo "All done, jmalloc has been installed to $P"
